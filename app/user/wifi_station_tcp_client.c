/*------------------------------------------------------------------------------
 * wifi_station_tcp_client.c
 * Программы управления в режиме WiFi_STATION и TCP_CLIENT
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
#include	<osapi.h>
#include	"global.h"
#include	"at.h"
#include	"wifi_station_tcp_client.h"
#include	"user_spi.h"
#include	"ets.h"
#include	"lwip/err.h"
#include	"lwip/app/espconn.h"

static void		station_check_ip		(void *arg);
void			wifi_sta_event_cb		(System_Event_t *evt);

static void		tcp_client_connect_cb	(void *arg);
static void		tcp_client_recon_cb		(void *arg, sint8 err);
static void		tcp_client_discon_cb	(void *arg);
static const char *reason_decode		(u8 reason);
static const char *espconn_reason_decode	(s8 reason);

static inline bool		mem_compare			(u8 *a, u8 *b, u32 len);
static inline void		ip4_addr			(struct ip_addr *ipaddr, u8 a, u8 b, u8 c, u8 d);


/*******************************************************************************
 * конфигурация ESP8266 в режиме Station (у музыканта)
 *******************************************************************************/
bool ICACHE_FLASH_ATTR wifi_config_station (void)
{
	static struct station_config	user_sta_config;
	static struct ip_info			get_ip;
	static u8 						mac[6]	= WIFI_MAC;

	os_memset (g.ssid, 0, sizeof (g.ssid));
	os_sprintf (g.ssid, WIFI_STA_SSID);
	g.ssid[6] = (char)(g.group_no + '0');

	os_memset (g.password, 0, sizeof (g.password));
	os_sprintf (g.password, "%s", WIFI_STA_PASSWORD);
	g.password[11] = (char)(g.group_no + '0');

	os_memcpy (g.local_mac, mac, sizeof (mac));				// MAC адрес (заготовка)
	g.local_mac[5] = g.group_no * 16 + 1;					// Корректировка моего MAC
	g.remote_mac[5] = g.group_no * 16 + 14;

	// запись IP адресов, жёстко заданных в программе
	ip4_addr (&g.local_ip.ip, WIFI_STA_IP + g.group_no);
	ip4_addr (&g.local_ip.netmask, WIFI_IP_MASK);
	ip4_addr (&g.local_ip.gw, WIFI_AP_IP + g.group_no);
	g.remote_ip.addr = g.local_ip.gw.addr;				// партнёр - гейтвей
	g.local_port = TCP_STA_PORT;
	g.remote_port = TCP_AP_PORT;

	// запись имени сети, пароля и MAC
	os_memcpy (user_sta_config.ssid, g.ssid, sizeof (g.ssid));
	os_memcpy (user_sta_config.password, g.password, sizeof (g.password));
	user_sta_config.bssid_set = 0;		// отключаем проверку mac
	os_memcpy (user_sta_config.bssid, g.local_mac, sizeof (g.local_mac));

	system_update_cpu_freq (160);
	if (wifi_get_opmode () != STATION_MODE)
	{
		wifi_set_opmode_current (STATION_MODE);
		ets_uart_printf ("Setup station mode\n");
	}
	// отключение службы назначения IP адресов
	wifi_station_dhcpc_stop ();
//	wifi_softap_dhcps_stop ();
	if (wifi_get_phy_mode () != PHY_MODE_11G)
	{
		wifi_set_phy_mode (PHY_MODE_11G);
	}
	// проверка на соответствие MAC адресов
	wifi_get_macaddr (STATION_IF, mac);
	if (!mem_compare (mac, g.local_mac, sizeof (g.local_mac)))
	{
		wifi_set_macaddr (STATION_IF, g.local_mac);
	}

	// задать статические ip адреса и маски
	wifi_set_ip_info (SOFTAP_IF, &g.local_ip);
	if (!wifi_set_ip_info (STATION_IF, &g.local_ip))
	{
		wifi_get_ip_info (STATION_IF, &get_ip);
		ets_uart_printf ("Error to write station ip info, read info:\n");
		ets_uart_printf ("ip = " IPSTR "\n", IP2STR(&get_ip.ip));
		ets_uart_printf ("gw = " IPSTR "\n", IP2STR(&get_ip.gw));
		ets_uart_printf ("mask = " IPSTR "\n", IP2STR(&get_ip.netmask));
	}

	// пропишем DNS сервер вручную
	espconn_dns_setserver (0, &g.remote_ip);
	// установка текущей конфигурации
	wifi_station_set_config_current (&user_sta_config);	// wifi_station_set_config переписывает флэш
//	wifi_softap_dhcps_start ();
	// запись коллбэка по событиям WiFi
	wifi_set_event_handler_cb (wifi_sta_event_cb);

	wifi_station_set_hostname ("IP_309_GUITAR");
	// Установка мощности передатчика
	system_phy_set_max_tpw (g.wifi_pw);
	wifi_set_sleep_type (NONE_SLEEP_T);

	if (!wifi_station_get_auto_connect ())
	{
		wifi_station_set_auto_connect (1);
	}
	wifi_station_set_reconnect_policy (true);

	// установка скорости для большей дальности
	wifi_set_user_fixed_rate (FIXED_RATE_MASK_ALL, PHY_RATE_12);
	wifi_set_user_sup_rate (RATE_11B5M, RATE_11G12M);				// TODO: нужно проверить работу 11b11m
//	wifi_set_user_rate_limit (RC_LIMIT_11B, 0x01, RATE_11B_B11M, RATE_11B_B11M);
//	wifi_set_user_limit_rate_mask (LIMIT_RATE_MASK_ALL);
	//	тестовая печать параметров
	ets_uart_printf ("Mode = %d\r\n", g.wifi_mode);
	ets_uart_printf ("ssid = %s\r\n", user_sta_config.ssid);
	ets_uart_printf ("password = %s\r\n", user_sta_config.password);
	ets_uart_printf ("mac = " MACSTR "\n", MAC2STR(user_sta_config.bssid));
//	ets_uart_printf ("ip = " IPSTR "\n", IP2STR(&g.local_ip.ip));
//	ets_uart_printf ("gw = " IPSTR "\n", IP2STR(&g.local_ip.gw));
//	ets_uart_printf ("mask = " IPSTR "\n", IP2STR(&g.local_ip.netmask));
	wifi_get_ip_info (STATION_IF, &get_ip);
//	ets_uart_printf ("Read info:\n");
	ets_uart_printf ("ip = " IPSTR "\n", IP2STR(&get_ip.ip));
	ets_uart_printf ("gw = " IPSTR "\n", IP2STR(&get_ip.gw));
	ets_uart_printf ("mask = " IPSTR "\n", IP2STR(&get_ip.netmask));

	//set a timer to check whether got ip from router succeed or not.
	 os_timer_disarm (&g.timer);
	 os_timer_setfn (&g.timer, (os_timer_func_t *)station_check_ip, NULL);
	 os_timer_arm (&g.timer, 500, OS_TIMER_REPEAT_OFF);

	return (true);
}



/*******************************************************************************
* коллбек при событиях на Wifi
* TODO: проверить срабатывание всех источников событий.
*******************************************************************************/
void ICACHE_FLASH_ATTR wifi_sta_event_cb (System_Event_t *evt)
{
	const char *temp;
	switch (evt->event)
	{
	case EVENT_STAMODE_CONNECTED:
		ets_uart_printf ("connect to ap: ssid %s, channel %d\n",
		        evt->event_info.connected.ssid,
		        evt->event_info.connected.channel);
		g.state = U_STATE_SEARCH;
//		station_check_ip ();
//		user_udp_init ();
		break;

	case EVENT_STAMODE_DISCONNECTED:
		temp = reason_decode (evt->event_info.disconnected.reason);
		ets_uart_printf ("disconnect from ssid %s, ", evt->event_info.disconnected.ssid);
		ets_uart_printf (temp);
		ets_uart_printf ("\n");
		g.state = U_STATE_STOP;
		break;

	case EVENT_STAMODE_AUTHMODE_CHANGE:
		ets_uart_printf ("Station change mode\n");
		ets_uart_printf ("mode: %d -> %d\n", evt->event_info.auth_change.old_mode, evt->event_info.auth_change.new_mode);
		g.state = U_STATE_STOP;
		break;

	case EVENT_STAMODE_GOT_IP:
		ets_uart_printf ("Station got IP\n");
		ets_uart_printf ("ip:" IPSTR, IP2STR(&evt->event_info.got_ip.ip));
		ets_uart_printf (", mask:" IPSTR, IP2STR(&evt->event_info.got_ip.mask));
		ets_uart_printf (", gw:" IPSTR, IP2STR(&evt->event_info.got_ip.gw));
		ets_uart_printf ("\n");
		g.state = U_STATE_SEARCH;
		break;

	default:
		ets_uart_printf ("Do not recognize event = %D.\n", evt->event);
		break;
	}
}

/******************************************************************************
* Функция проверки состояния WiFi соединения в режиме STATION
* вызывается по таймеру
******************************************************************************/
static void ICACHE_FLASH_ATTR station_check_ip (void *arg)
{
	struct ip_info	ipConfig;
	s8				espcon_status;
	u8				connect_status;

	os_timer_disarm (&g.timer);

	// проблема: вызывается два раза после соединения
	connect_status = wifi_station_get_connect_status ();
	switch (connect_status)
	{
	case STATION_GOT_IP:
		wifi_get_ip_info (STATION_IF, &ipConfig);
		if (ipConfig.ip.addr != 0)
		{
			if (g.state != U_STATE_TRANSMIT)
			{
				g.user_tcp_conn.proto.tcp = &g.user_tcp;
				g.user_tcp_conn.type = ESPCONN_TCP;
				g.state = U_STATE_SEARCH;
				ets_uart_printf ("WiFi connected\r\n");
				ets_uart_printf ("TCP handshaking...\r\n");

				// когда соединились по WiFi, будем проверять TCP соединение
				g.user_tcp_conn.state = ESPCONN_NONE;

				*(u32*)g.user_tcp.remote_ip = g.remote_ip.addr;

				g.user_tcp.local_port = g.local_port;	// static port of ESP8266
				g.user_tcp.remote_port = g.remote_port;	// remote port
				g.user_tcp.connect_callback = (espconn_connect_callback)tcp_client_connect_cb;
				g.user_tcp.reconnect_callback = (espconn_reconnect_callback)tcp_client_recon_cb;
				g.user_tcp.disconnect_callback = (espconn_connect_callback)tcp_client_discon_cb;
			}

			espcon_status = espconn_connect (&g.user_tcp_conn);
			switch (espcon_status)
			{
			case ESPCONN_OK:
				ets_uart_printf ("TCP created.\r\n");
				g.state = U_STATE_TRANSMIT;
				break;

			case ESPCONN_ISCONN:
				ets_uart_printf ("TCP already created.\r\n");
				g.state = U_STATE_TRANSMIT;
				break;

			default:
				ets_uart_printf ("Connection wait, reason: ");
				ets_uart_printf (espconn_reason_decode (espcon_status));
				ets_uart_printf ("\r\n");
				os_timer_arm (&g.timer, 500, OS_TIMER_REPEAT_OFF);
				break;
			}
		}
		break;

	case STATION_WRONG_PASSWORD:
		g.state = U_STATE_STOP;
		ets_uart_printf ("WiFi connecting error, wrong password\r\n");
		os_timer_setfn (&g.timer, (os_timer_func_t *)station_check_ip, NULL);
		os_timer_arm (&g.timer, 500, OS_TIMER_REPEAT_OFF);
		break;

	case STATION_NO_AP_FOUND:
		g.state = U_STATE_STOP;
		ets_uart_printf ("WiFi connecting error, ap not found\r\n");
		os_timer_setfn (&g.timer, (os_timer_func_t *)station_check_ip, NULL);
		os_timer_arm (&g.timer, 500, OS_TIMER_REPEAT_OFF);
		break;

	case STATION_CONNECT_FAIL:
		g.state = U_STATE_STOP;
		ets_uart_printf ("WiFi connecting fail\r\n");
		os_timer_setfn (&g.timer, (os_timer_func_t *)station_check_ip, NULL);
		os_timer_arm (&g.timer, 500, OS_TIMER_REPEAT_OFF);
		break;

	default:
		g.state = U_STATE_SEARCH;
		ets_uart_printf ("WiFi connecting...\r\n");
		os_timer_setfn (&g.timer, (os_timer_func_t *)station_check_ip, NULL);
		os_timer_arm (&g.timer, 500, OS_TIMER_REPEAT_OFF);
		break;
	}
}

/******************************************************************************
 * FunctionName : g.user_tcp_connect_cb
 * Description  : A new incoming tcp connection has been connected.
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
 *******************************************************************************/
static void ICACHE_FLASH_ATTR tcp_client_connect_cb (void *arg)
{
	struct espconn *pespconn = arg;

	os_printf ("connect succeed !!! \r\n");

	pespconn->recv_callback = user_tcp_receive_cb;
	pespconn->sent_callback = NULL;
	pespconn->proto.tcp->disconnect_callback = tcp_client_discon_cb;

	//user_sent_data (pespconn);
}



/******************************************************************************
 * FunctionName : user_tcp_discon_cb
 * Description  : disconnect callback.
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
 *******************************************************************************/
static void ICACHE_FLASH_ATTR tcp_client_discon_cb (void *arg)
{
	//tcp disconnect successfully
	struct espconn *pespconn = arg;
	g.state = U_STATE_SEARCH;
	if (pespconn == NULL)
	{
		os_printf ("tcpclient_discon_cb - conn is NULL!\r\n");
	}
	else
	{
		os_timer_disarm (&g.timer);
		os_timer_setfn (&g.timer, (os_timer_func_t *)station_check_ip, NULL);
		os_timer_arm (&g.timer, 500, OS_TIMER_REPEAT_OFF);

		os_printf ("tcp disconnect!!! \r\n");
	}
}


/******************************************************************************
 * FunctionName : user_tcp_recon_cb
 * Description  : reconnect callback, error occured in TCP connection.
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
 *******************************************************************************/
static void ICACHE_FLASH_ATTR tcp_client_recon_cb (void *arg, sint8 err)
{
	//error occured , tcp connection broke. user can try to reconnect here.

	os_printf ("reconnect callback, error code %d !!! \r\n", err);
}



/******************************************************************************
* Передача данных из буфера b.spi_receive по TCP
* в режиме TCP клиента
******************************************************************************/
void ICACHE_FLASH_ATTR tcp_client_send (void)
{
	g.user_tcp.remote_port = g.remote_port;
	*(u32*)g.user_tcp.remote_ip = g.remote_ip.addr;
	espconn_sent (&g.user_tcp_conn, (u8 *)&b.spi_receive, sizeof (b.spi_receive));
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



const char * ICACHE_FLASH_ATTR reason_decode (u8 reason)
{
	static const char ICACHE_RODATA_ATTR reason1[] = "REASON_UNSPECIFIED";
	static const char ICACHE_RODATA_ATTR reason2[] = "REASON_AUTH_EXPIRE";
	static const char reason3[] = "reason AUTH_LEAVE";
	static const char reason4[] = "reason ASSOC_EXPIRE";
	static const char reason5[] = "reason ASSOC_TOOMANY";
	static const char reason6[] = "reason NOT_AUTHED";
	static const char reason7[] = "reason NOT_ASSOCED";
	static const char reason8[] = "reason ASSOC_LEAVE";
	static const char reason9[] = "reason ASSOC_NOT_AUTHED";
	static const char reason10[] = "reason DISASSOC_PWRCAP_BAD";
	static const char reason11[] = "reason DISASSOC_SUPCHAN_BAD";
	static const char reason13[] = "reason IE_INVALID";
	static const char reason14[] = "reason MIC_FAILURE";
	static const char reason15[] = "reason 4WAY_HANDSHAKE_TIMEOUT";
	static const char reason16[] = "reason GROUP_KEY_UPDATE_TIMEOUT";
	static const char reason17[] = "reason IE_IN_4WAY_DIFFERS";
	static const char reason18[] = "reason GROUP_CIPHER_INVALID";
	static const char reason19[] = "reason PAIRWISE_CIPHER_INVALID";
	static const char reason20[] = "reason AKMP_INVALID";
	static const char reason21[] = "reason UNSUPP_RSN_IE_VERSION";
	static const char reason22[] = "reason INVALID_RSN_IE_CAP";
	static const char reason23[] = "reason 802_1X_AUTH_FAILED";
	static const char reason24[] = "reason CIPHER_SUITE_REJECTED";
	static const char reason200[] = "reason BEACON_TIMEOUT";
	static const char reason201[] = "reason NO_AP_FOUND";
	static const char reason202[] = "reason AUTH_FAIL";
	static const char reason203[] = "reason ASSOC_FAIL";
	static const char reason204[] = "reason HANDSHAKE_TIMEOUT";
	static const char reason0[] = "No recognize reason";

	switch (reason)
	{
	case 1:
		return reason1;
		break;
	case 2:
		return reason2;
		break;
	case 3:
		return reason3;
		break;
	case 4:
		return reason4;
		break;
	case 5:
		return reason5;
		break;
	case 6:
		return reason6;
		break;
	case 7:
		return reason7;
		break;
	case 8:
		return reason8;
		break;
	case 9:
		return reason9;
		break;
	case 10:
		return reason10;
		break;
	case 11:
		return reason11;
		break;
	case 13:
		return reason13;
		break;
	case 14:
		return reason14;
		break;
	case 15:
		return reason15;
		break;
	case 16:
		return reason16;
		break;
	case 17:
		return reason17;
		break;
	case 18:
		return reason18;
		break;
	case 19:
		return reason19;
		break;
	case 20:
		return reason20;
		break;
	case 21:
		return reason21;
		break;
	case 22:
		return reason22;
		break;
	case 23:
		return reason23;
		break;
	case 24:
		return reason24;
		break;
	case 200:
		return reason200;
		break;
	case 201:
		return reason201;
		break;
	case 202:
		return reason202;
		break;
	case 203:
		return reason203;
		break;
	case 204:
		return reason204;
		break;
	default:
		return reason0;
		break;
	}
}

const char * ICACHE_FLASH_ATTR espconn_reason_decode (s8 reason)
{
static const char esp_reason0[] = "ESPCONN_OK";			/* No error, everything OK. 0 */
static const char esp_reason1[] = "ESPCONN_MEMP";		/* Out of memory error.    -1  */
static const char esp_reason3[] = "ESPCONN_TIMEOUT";	/* Timeout.             -3   */
static const char esp_reason4[] = "ESPCONN_RTE";		/* Routing problem.      -4   */
static const char esp_reason5[] = "ESPCONN_INPROGRESS";	/* Operation in progress  -5  */
static const char esp_reason7[] = "ESPCONN_MAXNUM";		/* Total number exceeds the set maximum -7*/
static const char esp_reason8[] = "ESPCONN_ABRT";		/* Connection aborted.  -8    */
static const char esp_reason9[] = "ESPCONN_RST";		/* Connection reset.   -9     */
static const char esp_reason10[] = "ESPCONN_CLSD";		/* Connection closed. -10      */
static const char esp_reason11[] = "ESPCONN_CONN";		/* Not connected.    -11       */
static const char esp_reason12[] = "ESPCONN_ARG";		/* Illegal argument.  -12      */
static const char esp_reason14[] = "ESPCONN_IF";		/* Low_level error	-14		 */
static const char esp_reason15[] = "ESPCONN_ISCONN";	/* Already connected. -15      */
static const char esp_reason16[] = "ESPCONN_TIME";		/* Sync Time error		-16	 */
static const char esp_reason17[] = "ESPCONN_NODATA";	/* No data can be read	-17     */
static const char esp_reason28[] = "ESPCONN_HANDSHAKE";		/* ssl handshake failed	-28 */
static const char esp_reason29[] = "ESPCONN_RESP_TIMEOUT";	/* ssl handshake no response -29*/
static const char esp_reason61[] = "ESPCONN_PROTO_MSG";		/* ssl application invalid	-61 */

	switch (reason)
	{
	case 0:
		return esp_reason0;
		break;
	case -1:
		return esp_reason1;
		break;
	case -3:
		return esp_reason3;
		break;
	case -4:
		return esp_reason4;
		break;
	case -5:
		return esp_reason5;
		break;
	case -7:
		return esp_reason7;
		break;
	case -8:
		return esp_reason8;
		break;
	case -9:
		return esp_reason9;
		break;
	case -10:
		return esp_reason10;
		break;
	case -11:
		return esp_reason11;
		break;
	case -12:
		return esp_reason12;
		break;
	case -14:
		return esp_reason14;
		break;
	case -15:
		return esp_reason15;
		break;
	case -16:
		return esp_reason16;
		break;
	case -17:
		return esp_reason17;
		break;
	case -28:
		return esp_reason28;
		break;
	case -29:
		return esp_reason29;
		break;
	case -61:
		return esp_reason61;
		break;
	default:
		return esp_reason0;
		break;
	}
}

