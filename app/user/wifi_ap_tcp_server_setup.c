/*------------------------------------------------------------------------------
 * wifi_ap_tcp_server_setup.c
 * Программы управления в режиме WiFi_SOFTAP и TCP_SERVER
 *  Created on: 18.10.2016
 *      Author: Ilya Petrukhin
 ------------------------------------------------------------------------------*/
/*******************************************************************************
 * режим работы и имя сети в esp8266.
 * Режим работы возле пульта - Access Point (0)
 * Переносной - station (1).
 * Номер WiFi канала
 * Имя сети состоит из базового имени "IP309_" и номера группы g.group 0-9
 * "IP309_0"..."IP309_9"
 * Пароль состоит из имени станции, перед ним символы
 * "k38sS" = "k38sSIP309_0"
 * IP адрес у AP 192.168.0.2X, где X-номер группы 0-9
 * IP адрес у station 192.168.0.3X, где X-номер группы 0-9
 * MAC адрес у AP "1a:fe:34:d6:dd:Xe", где X-номер группы 0-9
 * MAC адрес у station "1a:fe:34:d6:dd:X1", где X-номер группы 0-9
 * Режим работы UDP или TCP
 * одиночное соединение
 * Порт TCP/UDP у AP 1210
 * Порт TCP/UDP у ST 1212
 *
 *******************************************************************************/
#include	<c_types.h>
#include	<bits.h>
#include	<user_interface.h>
#include	"global.h"
#include	"at.h"
#include	"wifi_ap_tcp_server_setup.h"
#include	"user_spi.h"
#include	"ets.h"
#include	"lwip/err.h"
#include	"lwip/app/espconn.h"


void			wifi_ap_event_cb		(System_Event_t *evt);

static void		tcp_server_listen		(void *arg);
static void		tcp_server_recon_cb 	(void *arg, sint8 err);
static void		tcp_server_discon_cb	(void *arg);
//static void 	tcp_server_multi_send	(void);
static void		tcpserver_init			(void);

static inline bool		mem_compare				(u8 *a, u8 *b, u32 len);
static inline void		ip4_addr				(struct ip_addr *ipaddr, u8 a, u8 b, u8 c, u8 d);



/*******************************************************************************
 * конфигурация ESP8266 в режиме Access point (возле пульта)
 * вызывается в user_wifi_config ()
 *******************************************************************************/
bool ICACHE_FLASH_ATTR wifi_config_ap (void)
{
	static struct softap_config	user_softap_config;
	static struct ip_info		get_ip;
	static u8 					mac[6]	= WIFI_MAC;

	os_memset (g.ssid, 0, sizeof (g.ssid));
	os_sprintf (g.ssid, "%s", WIFI_AP_SSID);
	g.ssid[6] = (char)(g.group_no + '0');

	os_memset (g.password, 0, sizeof (g.password));
	os_sprintf (g.password, "%s", WIFI_AP_PASSWORD);
	g.password[11] = (char)(g.group_no + '0');

	os_memcpy (g.local_mac, mac, sizeof (mac));				// MAC адрес (заготовка)
	g.local_mac[5] = g.group_no * 16 + 14;					// Корректировка моего MAC
	g.remote_mac[5] = g.group_no * 16 + 1;

	// запись IP адресов, жёстко заданных в программе
	ip4_addr (&g.local_ip.ip, WIFI_AP_IP + g.group_no);	// свой IP адрес
	ip4_addr (&g.local_ip.netmask, WIFI_IP_MASK);		// маска сети
	g.local_ip.gw.addr = g.local_ip.ip.addr;			// гейтвей - я же
	ip4_addr (&g.remote_ip, WIFI_STA_IP + g.group_no);	// IP партнёра

	g.local_port = TCP_AP_PORT;
	g.remote_port = TCP_STA_PORT;

	system_update_cpu_freq (160);
	// режим точки доступа - на пульте
	if (wifi_get_opmode () != SOFTAP_MODE)
	{
		wifi_set_opmode_current (SOFTAP_MODE);
		ets_uart_printf ("Setup access point mode\n");
	}
	if (wifi_get_phy_mode () != PHY_MODE_11G)
	{
		wifi_set_phy_mode (PHY_MODE_11G);
	}

	// Значения настроек Access point
	os_memcpy (user_softap_config.ssid, g.ssid, sizeof (g.ssid));
	os_memcpy (user_softap_config.password, g.password, sizeof (g.password));
	user_softap_config.ssid_len = os_strlen (g.ssid);
	user_softap_config.ssid_hidden = 0;
	user_softap_config.channel = g.wifi_ch;
	user_softap_config.authmode = AUTH_OPEN;// AUTH_WPA_PSK;	// режим шифрования
	user_softap_config.max_connection = 4;
	user_softap_config.beacon_interval = 100;


	// Установка имени сети и пароля
	wifi_softap_set_config_current (&user_softap_config);
	// установка MAC адресов, если текущий отличается от заданного
	wifi_get_macaddr (SOFTAP_IF, mac);
	if (!mem_compare (mac, g.local_mac, sizeof (g.local_mac)))
	{
		wifi_set_macaddr (SOFTAP_IF, g.local_mac);
	}

	// остановить DHCPS, IP статические
	wifi_softap_dhcps_stop ();
	wifi_station_dhcpc_stop ();

	// задать ip адреса и маски
	if (FALSE == wifi_set_ip_info (SOFTAP_IF, &g.local_ip))
	{
		wifi_get_ip_info (SOFTAP_IF, &get_ip);
		ets_uart_printf ("Read ip info:\n");
		ets_uart_printf ("ip = " IPSTR "\n", IP2STR(&get_ip.ip));
		ets_uart_printf ("gw = " IPSTR "\n", IP2STR(&get_ip.gw));
		ets_uart_printf ("mask = " IPSTR "\n", IP2STR(&get_ip.netmask));
	}
	wifi_softap_set_dhcps_lease_time (600);
//	wifi_softap_dhcps_start ();

	// Установка мощности передатчика
	system_phy_set_max_tpw (g.wifi_pw);
	wifi_set_sleep_type (NONE_SLEEP_T);

	// регистрация обработчика событий WiFi
	wifi_set_event_handler_cb (wifi_ap_event_cb);

	// ограничение скорости для большей дальности
	wifi_set_user_fixed_rate (FIXED_RATE_MASK_ALL, PHY_RATE_12);
	wifi_set_user_sup_rate (RATE_11B5M, RATE_11G12M);				// TODO: нужно проверить работу 11b11m
//	wifi_set_user_rate_limit (RC_LIMIT_11B, 0x01, RATE_11B_B11M, RATE_11B_B11M);
//	wifi_set_user_limit_rate_mask (LIMIT_RATE_MASK_ALL);

	wifi_softap_get_config (&user_softap_config);
	ets_uart_printf ("Mode = %d\r\n", g.wifi_mode);
	ets_uart_printf ("ssid = %s\n", user_softap_config.ssid);
	ets_uart_printf ("password = %s\n", user_softap_config.password);
	wifi_get_macaddr (SOFTAP_IF, mac);
	ets_uart_printf ("mac =" MACSTR "\n", MAC2STR(mac));
	wifi_get_ip_info (SOFTAP_IF, &get_ip);
//	ets_uart_printf ("read info:\n");
	ets_uart_printf ("ip = " IPSTR "\n", IP2STR(&get_ip.ip));
	ets_uart_printf ("gw = " IPSTR "\n", IP2STR(&get_ip.gw));
	ets_uart_printf ("mask = " IPSTR "\n", IP2STR(&get_ip.netmask));
	return (true);
}


/*******************************************************************************
 * коллбек при событиях на Wifi
 *******************************************************************************/
void ICACHE_FLASH_ATTR wifi_ap_event_cb (System_Event_t *evt)
{
	const char *temp;
	if (evt != NULL)
	{
		switch (evt->event)
		{
		case EVENT_SOFTAPMODE_STACONNECTED:
			ets_uart_printf ("connect to station: " MACSTR " join, AID = %d\n",
					MAC2STR(evt->event_info.sta_connected.mac),
					evt->event_info.sta_connected.aid);
			g.state = U_STATE_SEARCH;
			tcpserver_init ();
	//		user_udp_init ();
			break;

		case EVENT_SOFTAPMODE_STADISCONNECTED:
			ets_uart_printf ("station: " MACSTR " leave, AID = %d\n", MAC2STR(evt->event_info.sta_disconnected.mac), evt->event_info.sta_disconnected.aid);
	// при обрыве связи нужен рестарт
			g.state = U_STATE_STOP;
			system_restart ();
			break;

		case EVENT_SOFTAPMODE_PROBEREQRECVED:
	//		ets_uart_printf ("Probe request received.\n");
			break;

		default:
			ets_uart_printf ("Do not recognize event = %D.\n", evt->event);
			break;
		}
	}
}


/******************************************************************************
* FunctionName : tcpserver_init
* Description  : parameter initialize as a TCP server
* Parameters   : port -- server port
* Returns      : none
* конфигурация TCP сервера
*******************************************************************************/
void ICACHE_FLASH_ATTR tcpserver_init (void)
{
	sint8 ret;
	g.user_tcp_conn.type = ESPCONN_TCP;
	g.user_tcp_conn.state = ESPCONN_NONE;
	g.user_tcp_conn.proto.tcp = &g.user_tcp;
	g.user_tcp.local_port = g.local_port;
	g.user_tcp.connect_callback = (espconn_connect_callback)tcp_server_listen;
	ret = espconn_accept (&g.user_tcp_conn);	// создание TCP сервера
	os_printf ("espconn_accept [%d] !!! \r\n", ret);

	espconn_regist_time (&g.user_tcp_conn, 300, 0);
	g.state = U_STATE_SEARCH;
/*	os_timer_disarm (&g.timer);
	os_timer_setfn (&g.timer, (os_timer_func_t *)user_check_ip, NULL);
	os_timer_arm(&g.timer, 500, 1);
*/
}

/******************************************************************************
 * FunctionName : tcp_server_listen
 * Description  : TCP server listened a connection successfully
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
 *******************************************************************************/
static void ICACHE_FLASH_ATTR tcp_server_listen (void *arg)
{
	struct espconn *pesp_conn = arg;
	os_printf ("tcp_server_listen !!! \r\n");

	pesp_conn->recv_callback 					= (espconn_recv_callback)user_tcp_receive_cb;
	pesp_conn->sent_callback 					= NULL;
	pesp_conn->proto.tcp->reconnect_callback 	= (espconn_reconnect_callback)tcp_server_recon_cb;
	pesp_conn->proto.tcp->disconnect_callback 	= (espconn_connect_callback)tcp_server_discon_cb;

	espconn_set_opt (pesp_conn, ESPCONN_NODELAY);	// отключить буферизацию передачи

	g.state = U_STATE_TRANSMIT;
}

/******************************************************************************
 * FunctionName : tcp_server_discon_cb
 * Description  : disconnect callback.
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
 *******************************************************************************/
static void ICACHE_FLASH_ATTR tcp_server_discon_cb (void *arg)
{
	//tcp disconnect successfully
	g.state = U_STATE_STOP;
	os_printf ("tcp disconnect succeed !!! \r\n");
}

/******************************************************************************
 * FunctionName : tcp_server_recon_cb
 * Description  : reconnect callback, error occured in TCP connection.
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
 *******************************************************************************/
static void ICACHE_FLASH_ATTR tcp_server_recon_cb (void *arg, sint8 err)
{
	//error occured , tcp connection broke.
	g.state = U_STATE_STOP;
	os_printf ("reconnect callback, error code %d !!! \r\n", err);
}


/******************************************************************************
* отправка сообщения всем подключенным станциям на порт.
* кто умеет слушать - ответит
******************************************************************************/
/*static void ICACHE_FLASH_ATTR tcp_server_multi_send (void)
{
//	struct espconn *pesp_conn = &g.user_tcp_conn;
	remot_info *premot = NULL;
	uint8 count = 0;

	if (espconn_get_connection_info (&g.user_tcp_conn, &premot, 0) == ESPCONN_OK)
	{
		char *pbuf = "tcp_server_multi_send\n";
		for (count = 0; count < g.user_tcp_conn.link_cnt; count++)
		{
			g.user_tcp.remote_port = premot[count].remote_port;
			*(u32*)g.user_tcp.remote_ip = *(u32*)premot[count].remote_ip;
			espconn_sent (&g.user_tcp_conn, pbuf, os_strlen (pbuf));
		}
	}
}
*/

/******************************************************************************
* Передача данных из буфера b.spi_receive по TCP
* в режиме TCP сервера
******************************************************************************/
void ICACHE_FLASH_ATTR tcp_server_send (void)
{
	g.user_tcp.remote_port = g.remote_port;			// нужны ли эти два присваивания?
	*(u32*)g.user_tcp.remote_ip = g.remote_ip.addr;
	espconn_sent (&g.user_tcp_conn, (u8 *)b.spi_receive, sizeof (b.spi_receive));
}


/*******************************************************************************
* Сравнение двух областей памяти.
* Возврат - результат сравнения(равенство == true)
*******************************************************************************/
static inline bool ICACHE_FLASH_ATTR mem_compare (u8 *a, u8 *b, u32 len)
{
	while (len)
	{
		if (*a != *b)
		{
			return false;
		}
		a++;
		b++;
		len--;
	}
	return true;
}



/*******************************************************************************
* Преобразование переменных в структуру IP адреса
*******************************************************************************/
static inline void ICACHE_FLASH_ATTR ip4_addr (struct ip_addr *ipaddr, u8 a, u8 b, u8 c, u8 d)
{
	ipaddr->addr = ((u32_t) ( (d) & 0xff) << 24) | ((u32_t) ( (c) & 0xff) << 16)
	        | ((u32_t) ( (b) & 0xff) << 8) | (u32_t) ( (a) & 0xff);
}

