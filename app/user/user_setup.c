/*------------------------------------------------------------------------------
* user_setup.c
* Программы управления состоянием модуля
*  Created on: 29.05.2016
*      Author: Ilya Petrukhin
------------------------------------------------------------------------------*/
#include	"global.h"
#include	"at.h"
#include	"user_setup.h"
#include	"user_interface.h"

#define USE_WIFI_MODE		STATIONAP_MODE
#define WIFI_CLIENTSSID		"ESP8266-AP"
#define WIFI_CLIENTPASSWORD	"1234567890"
#define WIFI_AP_NAME		"ESP8266"
#define WIFI_AP_PASSWORD	"00000000"



/*******************************************************************************
* режим работы и имя сети в esp8266.
* Режим работы возле пульта - Access Point (0)
* Переносной - station (1).
* Имя сети состоит из базового имени "IP309_" и номера группы c.group 0-9 "IP309_0"..."IP309_9"
* Пароль состоит из имени станции, перед ним символы "k38sS" = "k38sSIP309_0"
* IP адрес у AP 192.168.0.2X, где X-номер группы 0-9
* IP у station 192.168.0.3X, где X-номер группы 0-9
* MAC адрес у AP "1a:fe:34:d6:dd:Xe", где X-номер группы 0-9
* MAC адрес у station "1a:fe:34:d6:dd:X1", где X-номер группы 0-9
* Режим работы UDP 
* одиночное соединение
* Порт у AP 1100
* Порт у ST 1102
*
*******************************************************************************/



void ICACHE_FLASH_ATTR setup_wifi_ap_mode (void)
{
	wifi_set_opmode ( (wifi_get_opmode () | STATIONAP_MODE) & USE_WIFI_MODE);
	struct softap_config apconfig;
	if (wifi_softap_get_config (&apconfig))
	{
		wifi_softap_dhcps_stop ();
		os_memset (apconfig.ssid, 0, sizeof (apconfig.ssid));
		os_memset (apconfig.password, 0, sizeof (apconfig.password));
		apconfig.ssid_len = ets_uart_printf (apconfig.ssid, WIFI_AP_NAME);
		ets_uart_printf (apconfig.password, "%s", WIFI_AP_PASSWORD);
		apconfig.authmode = AUTH_WPA_WPA2_PSK;
		apconfig.ssid_hidden = 0;
		apconfig.channel = 7;
		apconfig.max_connection = 4;
		if (!wifi_softap_set_config (&apconfig))
		{
#if DEBUG_LEVEL > 0
			ets_uart_printf ("ESP8266 not set AP config!\r\n");
#endif
		};
		struct ip_info ipinfo;
		wifi_get_ip_info (SOFTAP_IF, &g.ipinfo);
		IP4_ADDR(&g.ipinfo.ip, 192, 168, 4, 1);
		IP4_ADDR(&g.ipinfo.gw, 192, 168, 4, 1);
		IP4_ADDR(&g.ipinfo.netmask, 255, 255, 255, 0);
		wifi_set_ip_info (SOFTAP_IF, &g.ipinfo);
		wifi_softap_dhcps_start ();
	}
#if DEBUG_LEVEL > 0
	ets_uart_printf ("ESP8266 in AP mode configured.\r\n");
#endif
}

void ICACHE_FLASH_ATTR setup_wifi_st_mode (void)
{
	wifi_set_opmode ( (wifi_get_opmode () | STATIONAP_MODE) & USE_WIFI_MODE);
	struct station_config stconfig;
	wifi_station_disconnect ();
	wifi_station_dhcpc_stop ();
	if (wifi_station_get_config (&stconfig))
	{
		os_memset (stconfig.ssid, 0, sizeof (stconfig.ssid));
		os_memset (stconfig.password, 0, sizeof (stconfig.password));
		ets_uart_printf (stconfig.ssid, "%s", WIFI_CLIENTSSID);
		ets_uart_printf (stconfig.password, "%s", WIFI_CLIENTPASSWORD);
		if (!wifi_station_set_config (&stconfig))
		{
#if DEBUG_LEVEL > 0
			ets_uart_printf ("ESP8266 not set station config!\r\n");
#endif
		}
	}
	wifi_station_connect ();
	wifi_station_dhcpc_start ();
	wifi_station_set_auto_connect (1);
#if DEBUG_LEVEL > 0
	ets_uart_printf ("ESP8266 in STA mode configured.\r\n");
#endif
}

