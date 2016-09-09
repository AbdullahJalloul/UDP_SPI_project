/*------------------------------------------------------------------------------
 * user_setup.c
 * Программы управления состоянием модуля
 *  Created on: 29.05.2016
 *      Author: Ilya Petrukhin
 ------------------------------------------------------------------------------*/
#include	<c_types.h>
#include	<bits.h>
#include	<user_interface.h>
#include	"global.h"
#include	"at.h"
#include	"user_setup.h"
#include	"user_spi.h"
#include	"ets.h"
#include	"lwip/err.h"
#include	"lwip/app/espconn.h"

/* interrupt related */
#define ETS_SLC_INUM		1
#define ETS_SPI_INUM        2
#define ETS_RTC_INUM        3 // см. ets_rtc_int_register()
#define ETS_GPIO_INUM       4
#define ETS_UART_INUM       5
#define ETS_MAX_INUM        6
#define ETS_SOFT_INUM       7
#define ETS_WDT_INUM        8
#define ETS_FRC_TIMER0_INUM 9

bool	wifi_config_ap			(void);
bool	wifi_config_sta			(void);
void	ip4_addr				(struct ip_addr *ipaddr, u8 a, u8 b, u8 c, u8 d);
void	wifi_user_ap_event_cb	(System_Event_t *evt);
void	wifi_user_sta_event_cb	(System_Event_t *evt);
void	user_udp_stop			(void);
void	user_tx					(void);


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
 * Режим работы UDP
 * одиночное соединение
 * Порт у AP 1100
 * Порт у ST 1102
 *
 *******************************************************************************/

/*******************************************************************************
 * Инициализация WiFi
 * Вход: данные структуры g
 * Выход: настройки ESP8266
 * 		Режим работы
 * 		Имя станции
 * 		Канал WiFi
 * 		Скорость
 * 		Мак адрес
 * 		IP адреса и маски
 * 		UDP порт
 *******************************************************************************/
void ICACHE_FLASH_ATTR user_wifi_config (void)
{
	if (g.wifi_mode == U_WIFI_AP_MODE)
	{
		wifi_config_ap ();
	}
	else
	{
		wifi_config_sta ();
	}
}

/*******************************************************************************
 * конфигурация ESP8266 в режиме Access point (возле пульта)
 * вызывается в user_init ()
 *******************************************************************************/
bool ICACHE_FLASH_ATTR wifi_config_ap (void)
{
	static struct softap_config user_softap_config;
	struct ip_info	get_ip;
	u8				mac[6];

	// запись IP адресов, жёстко заданных в программе
	ip4_addr (&g.local_ip.ip, 192, 168, 0, 20 + g.group_no);	// свой IP адрес
	ip4_addr (&g.local_ip.netmask, 255, 255, 255, 0);		// маска сети
	g.local_ip.gw.addr = g.local_ip.ip.addr;				// гейтвей - я же
	ip4_addr (&g.remote_ip, 192, 168, 0, 30 + g.group_no);	// IP партнёра
	g.local_mac[5] = g.group_no * 16 + 14;					// Корректировка моего MAC
	g.remote_mac[5] = g.group_no * 16 + 1;					// Корректировка партнёрского MAC
	g.local_port = 1100;
	g.remote_port = 1102;

	// Значения настроек Access point
	os_memcpy (user_softap_config.ssid, g.ssid, 32);
	os_memcpy (user_softap_config.password, g.password, 64);
	user_softap_config.ssid_len = os_strlen (g.ssid);
	user_softap_config.ssid_hidden = 0;
	user_softap_config.channel = g.wifi_ch;
	user_softap_config.authmode = AUTH_OPEN;// AUTH_WPA_PSK;	// режим шифрования
	user_softap_config.max_connection = 4;
	user_softap_config.beacon_interval = 500;

	system_update_cpu_freq (160);
	// режим точки доступа - на пульте
	if (wifi_get_opmode () != SOFTAP_MODE)
	{
		wifi_set_opmode (SOFTAP_MODE);
	}
	wifi_set_phy_mode (PHY_MODE_11G);	//PHY_MODE_11B);
	// Установка имени сети и пароля
	wifi_softap_set_config_current (&user_softap_config);
	// установка MAC адресов, если текущий отличается от заданного
	wifi_get_macaddr (SOFTAP_IF, mac);
	if (!mem_compare (mac, g.local_mac, sizeof (mac)))
	{
		wifi_set_macaddr (SOFTAP_IF, g.local_mac);
	}
	// остановить DHCPS, IP жёстко заданы
	wifi_softap_dhcps_stop ();
	wifi_station_dhcpc_stop ();

	// задать ip адреса и маски
	if (!wifi_set_ip_info (SOFTAP_IF, &g.local_ip))
	{
		wifi_get_ip_info (SOFTAP_IF, &get_ip);
		ets_uart_printf ("Error to write soft ap ip info, read info:\n");
		ets_uart_printf ("ip = " IPSTR "\n", IP2STR(&get_ip.ip));
		ets_uart_printf ("gw = " IPSTR "\n", IP2STR(&get_ip.gw));
		ets_uart_printf ("mask = " IPSTR "\n", IP2STR(&get_ip.netmask));
	}
	// Установка мощности передатчика
	system_phy_set_max_tpw (10);
	wifi_set_sleep_type (NONE_SLEEP_T);

	// регистрация обработчика событий WiFi
	wifi_set_event_handler_cb (wifi_user_ap_event_cb);

	wifi_softap_dhcps_start ();

//	espconn_create (&g.user_udp_struct);   // create udp

	// ограничение скорости для большей дальности
//	wifi_set_user_fixed_rate (FIXED_RATE_MASK_ALL, PHY_RATE_12);
//	wifi_set_user_sup_rate (RATE_11B11M, RATE_11G12M);				// TODO: нужно проверить работу 11b11m
//	wifi_set_user_rate_limit (RC_LIMIT_11B, 0x01, RATE_11B_B11M, RATE_11B_B11M);
//	wifi_set_user_limit_rate_mask (LIMIT_RATE_MASK_ALL);

	ets_uart_printf ("Mode = %d\r\n", g.wifi_mode);
	ets_uart_printf ("ssid=%s\n", user_softap_config.ssid);
	ets_uart_printf ("password=%s\n", user_softap_config.password);
	ets_uart_printf ("mac=" MACSTR "\n", MAC2STR(user_softap_config.ssid));
	ets_uart_printf ("ip=" IPSTR "\n", IP2STR(&g.local_ip.ip));
	ets_uart_printf ("gw=" IPSTR "\n", IP2STR(&g.local_ip.gw));
	wifi_get_ip_info (SOFTAP_IF, &get_ip);
	ets_uart_printf ("read info:\n");
	ets_uart_printf ("ip = " IPSTR "\n", IP2STR(&get_ip.ip));
	ets_uart_printf ("gw = " IPSTR "\n", IP2STR(&get_ip.gw));
	ets_uart_printf ("mask = " IPSTR "\n", IP2STR(&get_ip.netmask));

	return (true);
}

/*******************************************************************************
 * коллбек при событиях на Wifi в режиме AP
 *******************************************************************************/
void wifi_user_ap_event_cb (System_Event_t *evt)
{
	switch (evt->event)
	{
	case EVENT_STAMODE_CONNECTED:
		ets_uart_printf ("connect to ssid %s, channel %d\n",
		        evt->event_info.connected.ssid,
		        evt->event_info.connected.channel);
		g.state = U_STATE_TRANSMIT;
		user_udp_init ();
		break;

	case EVENT_STAMODE_DISCONNECTED:
//		ets_uart_printf ("disconnect from ssid %s, reason %d\n", evt->event_info.disconnected.ssid, evt->event_info.disconnected.reason);
		g.state = U_STATE_STOP;
		break;

	case EVENT_STAMODE_AUTHMODE_CHANGE:
//		ets_uart_printf ("mode: %d -> %d\n", evt->event_info.auth_change.old_mode, evt->event_info.auth_change.new_mode);
		break;

	case EVENT_STAMODE_GOT_IP:
/*		ets_uart_printf ("ip:" IPSTR, IP2STR(&evt->event_info.got_ip.ip));
		ets_uart_printf (", mask:" IPSTR, IP2STR(&evt->event_info.got_ip.mask));
		ets_uart_printf (", gw:" IPSTR, IP2STR(&evt->event_info.got_ip.gw));
		ets_uart_printf ("\n");
*/		break;

	case EVENT_SOFTAPMODE_STACONNECTED:
		ets_uart_printf ("station: " MACSTR " join, AID = %d\n",
		        MAC2STR(evt->event_info.sta_connected.mac),
		        evt->event_info.sta_connected.aid);
		g.state = U_STATE_TRANSMIT;
		user_udp_init ();
		break;

	case EVENT_SOFTAPMODE_STADISCONNECTED:
//		ets_uart_printf ("station: " MACSTR " leave, AID = %d\n", MAC2STR(evt->event_info.sta_disconnected.mac), evt->event_info.sta_disconnected.aid);
// при обрыве связи нужен рестарт
		g.state = U_STATE_STOP;
		system_restart ();
		break;

	case EVENT_SOFTAPMODE_PROBEREQRECVED:
//		ets_uart_printf ("Probe request received.\n");
		break;

	default:
//		ets_uart_printf ("Do not recognize event.\n");
		break;
	}
}

/*******************************************************************************
 * конфигурация ESP8266 в режиме Station (у музыканта)
 *******************************************************************************/
bool ICACHE_FLASH_ATTR wifi_config_sta (void)
{
	static struct station_config user_sta_config;
	u8				mac[6];
	struct ip_info	get_ip;

	// запись IP адресов, жёстко заданных в программе
	ip4_addr (&g.local_ip.ip, 192, 168, 0, 30 + g.group_no);
	ip4_addr (&g.local_ip.netmask, 255, 255, 255, 0);
	ip4_addr (&g.local_ip.gw, 192, 168, 0, 20 + g.group_no);
	g.remote_ip.addr = g.local_ip.gw.addr;				// партнёр - гейтвей
	g.local_mac[5] = g.group_no * 16 + 1;
	g.remote_mac[5] = g.group_no * 16 + 14;
	g.local_port = 1102;
	g.remote_port = 1100;

	// запись имени сети, пароля и MAC
	os_memcpy (user_sta_config.ssid, g.ssid, 32);
	os_memcpy (user_sta_config.password, g.password, 64);
	user_sta_config.bssid_set = 1;
	os_memcpy (user_sta_config.bssid, g.local_mac, sizeof (mac));

	system_update_cpu_freq (160);
	if (wifi_get_opmode () != STATION_MODE)
	{
		wifi_set_opmode (STATION_MODE);
		ets_uart_printf ("Setup station mode\n");
	}
	wifi_set_phy_mode (PHY_MODE_11G);	//PHY_MODE_11B);
	// проверка на соответствие MAC адресов
	wifi_get_macaddr (STATION_IF, mac);
	if (!mem_compare (mac, g.local_mac, sizeof (mac)))
	{
		wifi_set_macaddr (STATION_IF, g.local_mac);
	}
	// отключение службы назначения IP адресов
	wifi_station_dhcpc_stop ();
	wifi_softap_dhcps_stop ();

	// задать ip адреса и маски
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
	// запись коллбэка по событиям WiFi
	wifi_set_event_handler_cb (wifi_user_sta_event_cb);

	wifi_station_set_hostname ("IP_309_GUITAR");
	// Установка мощности передатчика
	system_phy_set_max_tpw (10);
	wifi_set_sleep_type (NONE_SLEEP_T);

//	wifi_station_connect ();
//	wifi_station_dhcpc_start ();
//	wifi_station_set_auto_connect (1);

	// установка скорости для большей дальности
//	wifi_set_user_fixed_rate (FIXED_RATE_MASK_ALL, PHY_RATE_12);
//	wifi_set_user_sup_rate (RATE_11B11M, RATE_11G12M);				// TODO: нужно проверить работу 11b11m
//	wifi_set_user_rate_limit (RC_LIMIT_11B, 0x01, RATE_11B_B11M, RATE_11B_B11M);
//	wifi_set_user_limit_rate_mask (LIMIT_RATE_MASK_ALL);

	//	тестовая печать параметров
	ets_uart_printf ("Mode = %d\r\n", g.wifi_mode);
	ets_uart_printf ("ssid = %s\r\n", user_sta_config.ssid);
	ets_uart_printf ("password = %s\r\n", user_sta_config.password);
	ets_uart_printf ("mac = " MACSTR "\n", MAC2STR(user_sta_config.bssid));
	ets_uart_printf ("ip = " IPSTR "\n", IP2STR(&g.local_ip.ip));
	ets_uart_printf ("gw = " IPSTR "\n", IP2STR(&g.local_ip.gw));
	ets_uart_printf ("mask = " IPSTR "\n", IP2STR(&g.local_ip.netmask));
	wifi_get_ip_info (STATION_IF, &get_ip);
	ets_uart_printf ("Read info:\n");
	ets_uart_printf ("ip = " IPSTR "\n", IP2STR(&get_ip.ip));
	ets_uart_printf ("gw = " IPSTR "\n", IP2STR(&get_ip.gw));
	ets_uart_printf ("mask = " IPSTR "\n", IP2STR(&get_ip.netmask));

	return (true);
}

/*******************************************************************************
 * коллбек при событиях на Wifi в режиме STATION
 *******************************************************************************/
void wifi_user_sta_event_cb (System_Event_t *evt)
{
	switch (evt->event)
	{
	case EVENT_STAMODE_CONNECTED:
		ets_uart_printf ("connect to ssid %s, channel %d\n",
		        evt->event_info.connected.ssid,
		        evt->event_info.connected.channel);
		g.state = U_STATE_TRANSMIT;
		user_udp_init ();
		break;

	case EVENT_STAMODE_DISCONNECTED:
//		ets_uart_printf ("disconnect from ssid %s, reason %d\n", evt->event_info.disconnected.ssid, evt->event_info.disconnected.reason);
		g.state = U_STATE_STOP;
		break;

	case EVENT_STAMODE_AUTHMODE_CHANGE:
//		ets_uart_printf ("mode: %d -> %d\n", evt->event_info.auth_change.old_mode, evt->event_info.auth_change.new_mode);
		break;

	case EVENT_STAMODE_GOT_IP:
/*		ets_uart_printf ("ip:" IPSTR, IP2STR(&evt->event_info.got_ip.ip));
		ets_uart_printf (", mask:" IPSTR, IP2STR(&evt->event_info.got_ip.mask));
		ets_uart_printf (", gw:" IPSTR, IP2STR(&evt->event_info.got_ip.gw));
		ets_uart_printf ("\n");
*/		break;

	case EVENT_SOFTAPMODE_STACONNECTED:
		ets_uart_printf ("station: " MACSTR " join, AID = %d\n",
		        MAC2STR(evt->event_info.sta_connected.mac),
		        evt->event_info.sta_connected.aid);
		g.state = U_STATE_TRANSMIT;
		user_udp_init ();
		break;

	case EVENT_SOFTAPMODE_STADISCONNECTED:
		ets_uart_printf ("station: " MACSTR " leave, AID = %d\n",
		        MAC2STR(evt->event_info.sta_disconnected.mac),
		        evt->event_info.sta_disconnected.aid);
// при обрыве связи нужен перезапуск модуля
		g.state = U_STATE_STOP;
		system_restart ();
		break;

	case EVENT_SOFTAPMODE_PROBEREQRECVED:
//		ets_uart_printf ("probe requested\n");
		break;

	default:
//		ets_uart_printf ("Do not recognize event.\n");
		break;
	}

}

/*******************************************************************************
 * Преобразование переменных в структуру IP адреса
 *******************************************************************************/
void ICACHE_FLASH_ATTR ip4_addr (struct ip_addr *ipaddr, u8 a, u8 b, u8 c, u8 d)
{
	ipaddr->addr = ((u32_t)((d) & 0xff) << 24)
				| ((u32_t)((c) & 0xff) << 16)
				| ((u32_t)((b) & 0xff) << 8)
				| (u32_t)((a) & 0xff);
}

/*******************************************************************************
 * Сравнение двух областей памяти.
 * Возврат - результат сравнения(равенство == true)
 *******************************************************************************/
bool ICACHE_FLASH_ATTR mem_compare (u8 *a, u8 *b, u32 len)
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
 * Сохранение данных после команды AT+USETUP=
 * и перезапуск ESP8266
 *******************************************************************************/
void ICACHE_FLASH_ATTR user_wifi_setup (void)
{
	// сравнение с сохранёнными настройками
	if (compare_user_data () == false)
	{
		// записи отличаются - сохранить во флеш
		write_setup_flash ();
		// и перезапуск ESP8266
		system_restart ();
	}
	else
	{
		// печать "ready" для совместимости со стартом
		ets_uart_printf ("\r\nready\r\n");
	}
}

int ICACHE_FLASH_ATTR user_udp_init (void)
{
	g.user_pcbv = udp_new ();	// (struct udp_pcb *)
	g.user_pcbv->local_ip.addr = g.local_ip.ip.addr;
	g.user_pcbv->local_port = g.local_port;

	udp_connect (g.user_pcbv, &g.remote_ip, (u16)g.remote_port);	// соединиться

/*	if (user_pcbv != NULL)
	{
		udp_disconnect (user_pcbv);
		udp_remove (user_pcbv);
		user_pcbv = NULL;
	}

	user_pcbv = udp_new ();	// (struct udp_pcb *)
	if (user_pcbv == NULL
	        || (udp_bind (user_pcbv, &g.local_ip.ip, g.local_port) != ERR_OK))
	{
		udp_disconnect (user_pcbv);	// error
		udp_remove (user_pcbv);
		user_pcbv = NULL;
		return -1;
	}
*/
	udp_recv (g.user_pcbv, user_udp_receive_cb, g.user_pcbv);	// зарегистрировать функцию приёма

	return 1;
}

/******************************************************************************
 *
 ******************************************************************************/
void ICACHE_FLASH_ATTR user_udp_stop (void)
{
	/*	ets_isr_mask (BIT(ETS_FRC_TIMER0_INUM));
	 DPORT->reg_1_bits.timer0_edge_int_en = 0;
	 //	INTC_EDGE_EN &= ~BIT(1);

	 if (out_wave_pbuf != NULL)
	 {
	 os_free (out_wave_pbuf);
	 out_wave_pbuf = NULL;
	 sar_off (); // отключить SAR
	 }
	 */
}

/******************************************************************************
 * Передача данных из буфера b.spi_receive по UDP
 ******************************************************************************/
void ICACHE_FLASH_ATTR user_tx (void)
{
	struct pbuf *z;

	z = pbuf_alloc (PBUF_TRANSPORT, sizeof (b.spi_receive), PBUF_RAM);
	if (z != NULL)
	{
		err_t err = pbuf_take (z, b.spi_receive, sizeof (b.spi_receive));
		if (err == ERR_OK)
		{
			udp_sendto (g.user_pcbv, z, &g.remote_ip, g.remote_port);
		}
		pbuf_free (z);
	}
}

