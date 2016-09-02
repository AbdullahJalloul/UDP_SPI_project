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
#include	"lwip/err.h"
#include	"lwip/app/espconn.h"


bool	wifi_config_ap	(void);
bool	wifi_config_sta	(void);
void	ip4_addr		(struct ip_addr *ipaddr, u8 a, u8 b, u8 c, u8 d);
void	wifi_user_ap_event_cb	(System_Event_t *evt);
void	wifi_user_sta_event_cb	(System_Event_t *evt);

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
	static struct softap_config	user_softap_config;
	struct softap_config		get_softap_config;
	uint8						mac[6]		= {0x1a, 0xfe, 0x34, 0xd6, 0xdd, 0x0e};
	struct ip_info				get_ip;

	// запись IP адресов, жёстко заданных в программе
	ip4_addr (&g.ip_main.ip, 192, 168, 0, 20 + g.group_no);	// свой IP адрес
	ip4_addr (&g.ip_main.netmask, 255, 255, 255, 0);		// маска сети
	os_memcpy (&g.ip_main.gw, &g.ip_main.ip, 4);			// гейтвей - я же
	ip4_addr (&g.ip_other, 192, 168, 0, 30 + g.group_no);	// IP партнёра
	os_memcpy (g.mac_main, mac, sizeof (mac));				// MAC адрес (заготовка)
	os_memcpy (g.mac_other, mac, sizeof (mac));				// MAC адрес партнёра (заготовка)
	g.mac_main[5] = g.group_no * 16 + 14;					// Корректировка моего MAC
	g.mac_other[5] = g.group_no * 16 + 1;					// Корректировка партнёрского MAC
	
	// Значения настроек Access point
	os_memcpy (user_softap_config.ssid, g.ssid, 32);
	os_memcpy (user_softap_config.password, g.password, 64);
	user_softap_config.ssid_len = os_strlen (g.ssid);
	user_softap_config.ssid_hidden = 0;
	user_softap_config.channel = g.wifi_ch;
	user_softap_config.authmode = AUTH_OPEN;	// AUTH_WPA_PSK;	// режим шифрования
	user_softap_config.ssid_hidden = 0;
	user_softap_config.max_connection = 4;
	user_softap_config.beacon_interval = 100;
	
	// Установка параметров UDP соединения
	os_memcpy (g.user_esp_udp.local_ip, &g.ip_main.ip, 4);	// свой IP адрес
	os_memcpy (g.user_esp_udp.remote_ip, &g.ip_other, 4);	// IP партнёра
	g.user_esp_udp.local_port = 1100;
	g.user_esp_udp.remote_port = 1102;
	
	// Инициализируем структуру espconn
	g.user_udp_struct.type = ESPCONN_UDP;
	g.user_udp_struct.state = ESPCONN_NONE;
	g.user_udp_struct.proto.udp = &g.user_esp_udp;
	g.user_udp_struct.recv_callback = user_udp_receive_cb;
	g.user_udp_struct.sent_callback = NULL;
	g.user_udp_struct.link_cnt = 0;
	g.user_udp_struct.reverse = 0;
	
	// режим точки доступа - на пульте
	if (wifi_get_opmode () != SOFTAP_MODE)
	{
		wifi_set_opmode (SOFTAP_MODE);
		ets_uart_printf ("Setup soft AP mode\n");
	}	
	wifi_set_phy_mode (PHY_MODE_11B);
	// Установка имени сети и пароля, если установки не совпадают
	wifi_softap_get_config (&get_softap_config);
//	if (!mem_compare ((u8 *)&user_softap_config, (u8 *)&get_softap_config, sizeof (struct softap_config)))
//	{
		wifi_softap_set_config_current (&user_softap_config);
//		ets_uart_printf ("Soft AP config\n");
//	}
	// установка MAC адресов, если текущий отличается от заданного
	wifi_get_macaddr (SOFTAP_IF, mac);
	if (!mem_compare (mac, g.mac_main, sizeof (mac)))
	{
		wifi_set_macaddr (SOFTAP_IF, g.mac_main);
	}	
	// остановить DHCPS, IP жёстко заданы
	wifi_softap_dhcps_stop ();
	wifi_station_dhcpc_stop ();
	
	// задать ip адреса и маски
	if (!wifi_set_ip_info (SOFTAP_IF, &g.ip_main))
	{
		wifi_get_ip_info (SOFTAP_IF, &get_ip);
		ets_uart_printf ("Error to write soft ap ip info, read info:\n");
		ets_uart_printf ("ip = " IPSTR "\n", IP2STR (&get_ip.ip));
		ets_uart_printf ("gw = " IPSTR "\n", IP2STR (&get_ip.gw));
		ets_uart_printf ("mask = " IPSTR "\n", IP2STR (&get_ip.netmask));
	}
	
	// регистрация обработчика событий WiFi
	wifi_set_event_handler_cb (wifi_user_ap_event_cb);

	wifi_softap_dhcps_start ();

	espconn_create (&g.user_udp_struct);   // create udp
	
	// ограничение скорости для большей дальности
//	wifi_set_user_fixed_rate (FIXED_RATE_MASK_ALL, PHY_RATE_12);
//	wifi_set_user_sup_rate (RATE_11B11M, RATE_11G12M);				// TODO: нужно проверить работу 11b11m
//	wifi_set_user_rate_limit (RC_LIMIT_11B, 0x01, RATE_11B_B11M, RATE_11B_B11M);
//	wifi_set_user_limit_rate_mask (LIMIT_RATE_MASK_ALL);
	
	
	ets_uart_printf ("ssid=%s\n", user_softap_config.ssid);
	ets_uart_printf ("password=%s\n", user_softap_config.password);
	ets_uart_printf ("mac=" MACSTR "\n", MAC2STR (user_softap_config.ssid));
	ets_uart_printf ("ip=" IPSTR "\n", IP2STR (&g.ip_main.ip));
	ets_uart_printf ("gw=" IPSTR "\n", IP2STR (&g.ip_main.gw));
	wifi_get_ip_info (SOFTAP_IF, &get_ip);
	ets_uart_printf ("read info:\n");
	ets_uart_printf ("ip = " IPSTR "\n", IP2STR (&get_ip.ip));
	ets_uart_printf ("gw = " IPSTR "\n", IP2STR (&get_ip.gw));
	ets_uart_printf ("mask = " IPSTR "\n", IP2STR (&get_ip.netmask));
	
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
		ets_uart_printf ("connect to ssid %s, channel %d\n", evt->event_info.connected.ssid, evt->event_info.connected.channel);
		g.state = U_STATE_TRANSMIT;
		break;
		
	case EVENT_STAMODE_DISCONNECTED:
		ets_uart_printf ("disconnect from ssid %s, reason %d\n", evt->event_info.disconnected.ssid, evt->event_info.disconnected.reason);
		g.state = U_STATE_STOP;
		break;
		
	case EVENT_STAMODE_AUTHMODE_CHANGE:
		ets_uart_printf ("mode: %d -> %d\n", evt->event_info.auth_change.old_mode, evt->event_info.auth_change.new_mode);
		break;
		
	case EVENT_STAMODE_GOT_IP:
		ets_uart_printf ("ip:" IPSTR, IP2STR(&evt->event_info.got_ip.ip));
		ets_uart_printf (", mask:" IPSTR, IP2STR(&evt->event_info.got_ip.mask)); 
		ets_uart_printf (", gw:" IPSTR, IP2STR(&evt->event_info.got_ip.gw)); 
		ets_uart_printf ("\n");
		break;
		
	case EVENT_SOFTAPMODE_STACONNECTED:
		ets_uart_printf ("station: " MACSTR " join, AID = %d\n", MAC2STR(evt->event_info.sta_connected.mac), evt->event_info.sta_connected.aid);
		g.state = U_STATE_TRANSMIT;
		break;
		
	case EVENT_SOFTAPMODE_STADISCONNECTED:
		ets_uart_printf ("station: " MACSTR " leave, AID = %d\n", MAC2STR(evt->event_info.sta_disconnected.mac), evt->event_info.sta_disconnected.aid);
		g.state = U_STATE_STOP;
		break;
		
	case EVENT_SOFTAPMODE_PROBEREQRECVED:
		ets_uart_printf ("Probe request received.\n");
		break;
		
	default:
		ets_uart_printf ("Do not recognize event.\n");
		break;
	}
}

/*******************************************************************************
* конфигурация ESP8266 в режиме Station (у музыканта)
*******************************************************************************/
bool ICACHE_FLASH_ATTR wifi_config_sta (void)
{
	static struct station_config	user_sta_config;
	struct station_config			get_sta_config;
	uint8							mac[6]		= {0x1a, 0xfe, 0x34, 0xd6, 0xdd, 0x01};
	struct ip_info					get_ip;
	
	// запись IP адресов, жёстко заданных в программе
	ip4_addr (&g.ip_main.ip, 192, 168, 0, 30 + g.group_no);
	ip4_addr (&g.ip_main.netmask, 255, 255, 255, 0);
	ip4_addr (&g.ip_main.gw, 192, 168, 0, 20 + g.group_no);
	os_memcpy (&g.ip_other, &g.ip_main.gw, 4);				// партнёр - гейтвей
	os_memcpy (g.mac_main, mac, sizeof (mac));
	os_memcpy (g.mac_other, mac, sizeof (mac));
	g.mac_main[5] = g.group_no * 16 + 1;
	g.mac_other[5] = g.group_no * 16 + 14;
	
	// запись имени сети, пароля и MAC
	os_memcpy (user_sta_config.ssid, g.ssid, 32);
	os_memcpy (user_sta_config.password, g.password, 64);
	user_sta_config.bssid_set = 1;
	os_memcpy (user_sta_config.bssid, g.mac_main, sizeof (mac));
	
	// Установка параметров UDP соединения - IP адресов и портов
	os_memcpy (g.user_esp_udp.local_ip, &g.ip_main.ip, 4);	// свой IP адрес
	os_memcpy (g.user_esp_udp.remote_ip, &g.ip_other, 4);	// IP партнёра
	g.user_esp_udp.local_port = 1102;		// порты
	g.user_esp_udp.remote_port = 1100;

	// Инициализируем структуру espconn
	g.user_udp_struct.type = ESPCONN_UDP;
	g.user_udp_struct.state = ESPCONN_NONE;
	g.user_udp_struct.proto.udp = &g.user_esp_udp;
	g.user_udp_struct.recv_callback = user_udp_receive_cb;
	g.user_udp_struct.sent_callback = NULL;
	g.user_udp_struct.reverse = 0;
	g.user_udp_struct.link_cnt = 0;
	
	if (wifi_get_opmode () != STATION_MODE)
	{
		wifi_set_opmode (STATION_MODE);
		ets_uart_printf ("Setup station mode\n");
	}	
	wifi_set_phy_mode (PHY_MODE_11B);
	// проверка на соответствие MAC адресов
	wifi_get_macaddr (STATION_IF, mac);
	if (!mem_compare (mac, g.mac_main, sizeof (mac)))
	{
		wifi_set_macaddr (STATION_IF, g.mac_main);
	}
	// отключение службы назначения IP адресов
	wifi_station_dhcpc_stop ();
	wifi_softap_dhcps_stop ();

	// задать ip адреса и маски
	if (!wifi_set_ip_info (STATION_IF, &g.ip_main))
	{
		wifi_get_ip_info (STATION_IF, &get_ip);
		ets_uart_printf ("Error to write station ip info, read info:\n");
		ets_uart_printf ("ip = " IPSTR "\n", IP2STR (&get_ip.ip));
		ets_uart_printf ("gw = " IPSTR "\n", IP2STR (&get_ip.gw));
		ets_uart_printf ("mask = " IPSTR "\n", IP2STR (&get_ip.netmask));
	}
//	wifi_set_ip_info (SOFTAP_IF, &g.ip_main);
	
	// проверка записи
	
	// пропишем DNS сервер вручную
	espconn_dns_setserver (0, &g.ip_other);
	// установка текущей конфигурации
//	wifi_station_get_config (&get_sta_config);
//	if (!mem_compare ((u8*)&user_sta_config, (u8*)&get_sta_config, sizeof (struct station_config)))
//	{
		wifi_station_set_config_current (&user_sta_config);	// wifi_station_set_config переписывает флэш
		ets_uart_printf ("Setup wifi station config\n");
//	}
		
	// запись коллбэка по событиям WiFi
	wifi_set_event_handler_cb (wifi_user_sta_event_cb);
	
	wifi_station_set_hostname ("IP_309_GUITAR");
		
//	wifi_station_connect ();
//	wifi_station_dhcpc_start ();
//	wifi_station_set_auto_connect (1);
	
	// установка скорости для большей дальности
//	wifi_set_user_fixed_rate (FIXED_RATE_MASK_ALL, PHY_RATE_12);
//	wifi_set_user_sup_rate (RATE_11B11M, RATE_11G12M);				// TODO: нужно проверить работу 11b11m
//	wifi_set_user_rate_limit (RC_LIMIT_11B, 0x01, RATE_11B_B11M, RATE_11B_B11M);
//	wifi_set_user_limit_rate_mask (LIMIT_RATE_MASK_ALL);
	
	espconn_create (&g.user_udp_struct);   // create udp
	
	//	тестовая печать параметров
	ets_uart_printf ("ssid = %s\r\n", user_sta_config.ssid);
	ets_uart_printf ("password = %s\r\n", user_sta_config.password);
	ets_uart_printf ("mac = " MACSTR "\n", MAC2STR(user_sta_config.bssid));
	ets_uart_printf ("ip = " IPSTR "\n", IP2STR (&g.ip_main.ip));
	ets_uart_printf ("gw = " IPSTR "\n", IP2STR (&g.ip_main.gw));
	ets_uart_printf ("mask = " IPSTR "\n", IP2STR (&g.ip_main.netmask));
	wifi_get_ip_info (STATION_IF, &get_ip);
	ets_uart_printf ("Read info:\n");
	ets_uart_printf ("ip = " IPSTR "\n", IP2STR (&get_ip.ip));
	ets_uart_printf ("gw = " IPSTR "\n", IP2STR (&get_ip.gw));
	ets_uart_printf ("mask = " IPSTR "\n", IP2STR (&get_ip.netmask));
	
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
		ets_uart_printf ("connect to ssid %s, channel %d\n", evt->event_info.connected.ssid, evt->event_info.connected.channel);
		g.state = U_STATE_TRANSMIT;
		break;
		
	case EVENT_STAMODE_DISCONNECTED:
		ets_uart_printf ("disconnect from ssid %s, reason %d\n", evt->event_info.disconnected.ssid, evt->event_info.disconnected.reason);
		g.state = U_STATE_STOP;
		break;
		
	case EVENT_STAMODE_AUTHMODE_CHANGE:
		ets_uart_printf ("mode: %d -> %d\n", evt->event_info.auth_change.old_mode, evt->event_info.auth_change.new_mode);
		break;
		
	case EVENT_STAMODE_GOT_IP:
		ets_uart_printf ("ip:" IPSTR, IP2STR(&evt->event_info.got_ip.ip));
		ets_uart_printf (", mask:" IPSTR, IP2STR(&evt->event_info.got_ip.mask)); 
		ets_uart_printf (", gw:" IPSTR, IP2STR(&evt->event_info.got_ip.gw)); 
		ets_uart_printf ("\n");
		break;
		
	case EVENT_SOFTAPMODE_STACONNECTED:
		ets_uart_printf ("station: " MACSTR " join, AID = %d\n", MAC2STR(evt->event_info.sta_connected.mac), evt->event_info.sta_connected.aid);
		g.state = U_STATE_TRANSMIT;
		break;
		
	case EVENT_SOFTAPMODE_STADISCONNECTED:
		ets_uart_printf ("station: " MACSTR " leave, AID = %d\n", MAC2STR(evt->event_info.sta_disconnected.mac), evt->event_info.sta_disconnected.aid);
		g.state = U_STATE_STOP;
		break;
		
	case EVENT_SOFTAPMODE_PROBEREQRECVED:
		ets_uart_printf ("probe requested\n");
		break;
		
	default:
		ets_uart_printf ("Do not recognize event.\n");
		break;
	}
	
}


/*******************************************************************************
* Преобразование переменных в структуру IP адреса
*******************************************************************************/
void ICACHE_FLASH_ATTR ip4_addr (struct ip_addr *ipaddr, u8 a, u8 b, u8 c, u8 d)
{
	ipaddr->addr = ((u32_t)((d) & 0xff) << 24) | 
					((u32_t)((c) & 0xff) << 16) | 
					((u32_t)((b) & 0xff) << 8)  | 
					(u32_t)((a) & 0xff);
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
	if (false == compare_user_data ())
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

