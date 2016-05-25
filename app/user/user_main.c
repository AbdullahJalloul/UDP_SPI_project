#include "ets_sys.h"
#include "osapi.h"
#include "at_custom.h"
#include "user_interface.h"
#include "driver/uart.h"

#include "user_tcp_server.h"
#include "global.h"


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

/*********************** define constants *************************************/

global g =
{
		"IP309_0",
		"k38sSIP309_0",
		IP4_WORD (192, 168, 0, 20),
		IP4_WORD (192, 168, 0, 20),
		IP4_WORD (255, 255, 255, 0),

};

/************************ typedef func ****************************************/
void	at_u_setup_test		(uint8_t id);
void	at_u_setup_query	(uint8_t id);
void	at_u_setup_setup	(uint8_t id, char *pPara);
void	at_u_setup_exe		(uint8_t id);

void	at_u_start_test		(uint8_t id);
void	at_u_start_query	(uint8_t id);
void	at_u_start_exe		(uint8_t id);

void	at_u_stop_test		(uint8_t id);
void	at_u_stop_query		(uint8_t id);
void	at_u_stop_exe		(uint8_t id);

um_t um = 
{
		U_WIFI_AP_MODE,
		0,
		1,
		1,
		U_STATE_STOP
};

at_funcationType at_custom_cmd[] =
{
    {"+USETUP", 7, at_u_setup_test, at_u_setup_query, at_u_setup_setup, at_u_setup_exe},
    {"+USTART", 7, at_u_start_test, at_u_start_query, NULL, at_u_start_exe},
    {"+USTOP", 7, at_u_stop_test, at_u_stop_query, NULL, at_u_stop_exe},
};

uint8 buffer[128] = {0};


/*******************************************************************************
 * Функция ответа на команду AT+USETUP?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_setup_test (uint8_t id)
{
	at_port_print ("User setup interface:\r\n AP/STA mode (0-1), wifi_ch (0-13), group (0-9), channel (0-9)\r\n");
    at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USTART?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_start_test (uint8_t id)
{
	at_port_print ("Start user communication\r\n");
    at_response_ok ();	
}

/*******************************************************************************
 * Функция ответа на команду AT+USTOP?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_stop_test (uint8_t id)
{
	at_port_print ("Stop user communication\r\n");
    at_response_ok ();	
}

/*******************************************************************************
 * Функция ответа на команду AT+USETUP=?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_setup_query (uint8_t id)
{
	ets_sprintf (buffer, "%s\r\n", "User setup state:");
    at_port_print (buffer);
    if (um.wifi_mode == U_WIFI_AP_MODE)
    {
    	at_port_print ("WiFi Access Point mode\r\n");
    }
    else
    {
    	at_port_print ("WiFi Station mode\r\n");
    }
    os_sprintf (buffer, "%s%d\r\n", "WiFi channel no ", um.wifi_ch);
    at_port_print (buffer);
    os_sprintf (buffer, "%s%d\r\n", "Group no ", um.group_no);
    at_port_print (buffer);
    if (um.channel_no)
    {
    	os_sprintf (buffer, "%s%d\r\n", "Channel no ", um.channel_no);
    }
    else
    {
    	os_sprintf (buffer, "%s\r\n", "Group mode");
    }
    at_port_print (buffer);
    at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USTART=?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_start_query (uint8_t id)
{
	if (um.state == U_STATE_STOP)
	{
		at_port_print ("USTOP");
	}
	else if (um.state == U_STATE_SEARCH)
	{
		at_port_print ("USEARCH");
	}
	else
	{
		// U_STATE_TRANSMIT
		at_port_print ("UTRANSMIT");
	}
    at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USTOP=?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_stop_query (uint8_t id)
{
	if (um.state == U_STATE_STOP)
	{
		at_port_print ("USTOP");
	}
	else if (um.state == U_STATE_SEARCH)
	{
		at_port_print ("USEARCH");
	}
	else
	{
		// U_STATE_TRANSMIT
		at_port_print ("UTRANSMIT");
	}
    at_response_ok ();	
}

/*******************************************************************************
 * Функция ответа на команду AT+USETUP=1, 3, 2, 5
 * Первый параметр: 0 - wifi AP mode, 1 - wifi STA mode
 * Второй параметр: номер wifi канала 0-13
 * Третий параметр: номер группы инструментов 1-9
 * Четвёртый параметр: номер канала в группе 0 - групповая работа
 * 						1-9 канал в группе
 ******************************************************************************/
// test :AT+TEST=1,"abc"<,3>
void ICACHE_FLASH_ATTR at_u_setup_setup (uint8_t id, char *pPara)
{
    int result = 0;
    int err = 0;
    int	flag = 0;
    
    pPara++; // skip '='
    //get the first digit parameter
    flag = at_get_next_int_dec (&pPara, &result, &err);
    // flag must be true because there are more parameter
    if (flag == FALSE)
    {
        at_response_error ();
        return;
    }
    if (result)
    {
    	um.wifi_mode = U_WIFI_STA_MODE;
    }
    else
    {
    	um.wifi_mode = U_WIFI_AP_MODE;
    }
    //get the second digit parameter
    if (*pPara++ != ',')  // skip ','
    {
        at_response_error ();
        return;
    }
    flag = at_get_next_int_dec (&pPara, &result, &err);
    if (flag == FALSE)
    {
        at_response_error ();
        return;
    }
    um.wifi_ch = result;
    //get the third digit parameter
    if (*pPara++ != ',')  // skip ','
    {
        at_response_error ();
        return;
    }
    flag = at_get_next_int_dec (&pPara, &result, &err);
    if (flag == FALSE)
    {
        at_response_error ();
        return;
    }
    um.group_no = result;
    //get the fourth digit parameter
    if (*pPara++ != ',')  // skip ','
    {
        at_response_error ();
        return;
    }
    flag = at_get_next_int_dec (&pPara, &result, &err);
    if (flag == FALSE)
    {
        at_response_error ();
        return;
    }
    um.channel_no = result;

    if (*pPara != '\r')
    {
        at_response_error();
        return;
    }
    at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USETUP
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_setup_exe (uint8_t id)
{
    uint8 buffer[32] = {0};

    os_sprintf (buffer, "%s\r\n", "at_u_setup_exe");
    at_port_print (buffer);
    at_response_ok ();
}


/*******************************************************************************
 * Функция ответа на команду AT+USTART
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_start_exe (uint8_t id)
{
	// TODO: начать трансляцию, когда найдётся партнёр
}

/*******************************************************************************
 * Функция ответа на команду AT+USTOP
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_stop_exe (uint8_t id)
{
	// TODO: остановить передачу пакетов
}


extern int ets_uart_printf (const char *fmt, ...);
int (*console_printf) (const char *fmt, ...) = ets_uart_printf;
static char macaddr[6];

const char *WiFiMode[] =
{ "NULL",		// 0x00
        "STATION",	// 0x01
        "SOFTAP", 	// 0x02
        "STATIONAP"	// 0x03
        };

void setup_wifi_ap_mode (void)
{
	wifi_set_opmode ( (wifi_get_opmode () | STATIONAP_MODE) & USE_WIFI_MODE);
	struct softap_config apconfig;
	if (wifi_softap_get_config ( &apconfig))
	{
		wifi_softap_dhcps_stop ();
		os_memset (apconfig.ssid, 0, sizeof (apconfig.ssid));
		os_memset (apconfig.password, 0, sizeof (apconfig.password));
		apconfig.ssid_len = os_sprintf (apconfig.ssid, WIFI_AP_NAME);
		os_sprintf (apconfig.password, "%s", WIFI_AP_PASSWORD);
		apconfig.authmode = AUTH_WPA_WPA2_PSK;
		apconfig.ssid_hidden = 0;
		apconfig.channel = 7;
		apconfig.max_connection = 4;
		if ( !wifi_softap_set_config ( &apconfig))
		{
#if DEBUG_LEVEL > 0
			ets_uart_printf("ESP8266 not set AP config!\r\n");
#endif
		};
		struct ip_info ipinfo;
		wifi_get_ip_info (SOFTAP_IF, &ipinfo);
		IP4_ADDR ( &ipinfo.ip, 192, 168, 4, 1);
		IP4_ADDR ( &ipinfo.gw, 192, 168, 4, 1);
		IP4_ADDR ( &ipinfo.netmask, 255, 255, 255, 0);
		wifi_set_ip_info (SOFTAP_IF, &ipinfo);
		wifi_softap_dhcps_start ();
	}
#if DEBUG_LEVEL > 0
	ets_uart_printf ("ESP8266 in AP mode configured.\r\n");
#endif
}

void setup_wifi_st_mode (void)
{
	wifi_set_opmode ( (wifi_get_opmode () | STATIONAP_MODE) & USE_WIFI_MODE);
	struct station_config stconfig;
	wifi_station_disconnect ();
	wifi_station_dhcpc_stop ();
	if (wifi_station_get_config ( &stconfig))
	{
		os_memset (stconfig.ssid, 0, sizeof (stconfig.ssid));
		os_memset (stconfig.password, 0, sizeof (stconfig.password));
		os_sprintf (stconfig.ssid, "%s", WIFI_CLIENTSSID);
		os_sprintf (stconfig.password, "%s", WIFI_CLIENTPASSWORD);
		if ( !wifi_station_set_config ( &stconfig))
		{
#if DEBUG_LEVEL > 0
			ets_uart_printf("ESP8266 not set station config!\r\n");
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

void init_done (void)
{
#if DEBUG_LEVEL > 0
	char temp[80];
	// Print system info
	ets_uart_printf("\r\n");
	ets_uart_printf("ESP8266 platform starting...\r\n");
	ets_uart_printf("System info:\r\n");
	os_sprintf(temp, "SDK version: %s\r\n", system_get_sdk_version());
	ets_uart_printf(temp);
	os_sprintf(temp, "Time = %ld\r\n", system_get_time());
	ets_uart_printf(temp);
	os_sprintf(temp, "Chip id = 0x%x\r\n", system_get_chip_id());
	ets_uart_printf(temp);
	os_sprintf(temp, "CPU freq = %d MHz\r\n", system_get_cpu_freq());
	ets_uart_printf(temp);
	os_sprintf(temp, "Free heap size = %d\r\n", system_get_free_heap_size());
	ets_uart_printf(temp);
#endif	// #if DEBUG_LEVEL > 0

	//if(wifi_get_opmode() != USE_WIFI_MODE)
	{
#if DEBUG_LEVEL > 0
		console_printf("ESP8266 is %s mode, restarting in %s mode...\r\n", WiFiMode[wifi_get_opmode()], WiFiMode[USE_WIFI_MODE]);
#endif
		if (USE_WIFI_MODE & SOFTAP_MODE)
		{
			setup_wifi_ap_mode ();
		}
		if (USE_WIFI_MODE & STATION_MODE)
		{
			setup_wifi_st_mode ();
		}
	}
	if (USE_WIFI_MODE & SOFTAP_MODE)
	{
		wifi_get_macaddr (SOFTAP_IF, macaddr);
	}
	if (USE_WIFI_MODE & STATION_MODE)
	{
		wifi_get_macaddr (STATION_IF, macaddr);
	}
	if (wifi_get_phy_mode () != PHY_MODE_11N)
	{
		wifi_set_phy_mode (PHY_MODE_11N);
	}
	if (wifi_station_get_auto_connect () == 0)
	{
		wifi_station_set_auto_connect (1);
	}

#if DEBUG_LEVEL > 0
	console_printf("Wi-Fi mode: %s\r\n", WiFiMode[wifi_get_opmode()]);
	if(USE_WIFI_MODE & SOFTAP_MODE)
	{
		struct softap_config apConfig;
		if (wifi_softap_get_config (&apConfig))
		{
			console_printf("AP config: SSID: %s, PASSWORD: %s\r\n",
					apConfig.ssid,
					apConfig.password);
		}
	}
	if(USE_WIFI_MODE & STATION_MODE)
	{
		struct station_config stationConfig;
		if (wifi_station_get_config (&stationConfig))
		{
			console_printf("STA config: SSID: %s, PASSWORD: %s\r\n",
					stationConfig.ssid,
					stationConfig.password);
		}
	}
#endif

	// ������������� � ������ tcp ������� �� ����� 80
	TCP_SERV_CFG *p = tcpsrv_init (80);
	if (p != NULL)
	{
		// ������� ������ �� ���� ����������:
		p->max_conn = 3;
		p->time_wait_rec = 30;
		p->time_wait_cls = 30;
		p->min_heap = 16384;
		ets_uart_printf (
		        "Max connection %d, time waits %d & %d, min heap size %d\n",
		        p->max_conn, p->time_wait_rec, p->time_wait_cls, p->min_heap);
		// �������� � ��������� �����������:
		//p->func_discon_cb = NULL;
		//p->func_listen =
		//p->func_sent_cb =
		//p->func_recv =
		if ( !tcpsrv_start (p))
			tcpsrv_close (p);
	}
	// ������������� � ������ tcp ������� �� ����� 8080
	p = tcpsrv_init (8080);
	if (p != NULL)
	{
		// ������� ������ �� ���� ����������:
		p->max_conn = 99;
		p->time_wait_rec = 30;
		p->time_wait_cls = 30;
		p->min_heap = 16384;
		ets_uart_printf (
		        "Max connection %d, time waits %d & %d, min heap size %d\n",
		        p->max_conn, p->time_wait_rec, p->time_wait_cls, p->min_heap);
		if ( !tcpsrv_start (p))
		{
			tcpsrv_close (p);
		}
	}
#if DEBUG_LEVEL > 0
	ets_uart_printf ("ESP8266 platform started!\r\n");
#endif
}

void ICACHE_FLASH_ATTR user_init(void)
{
	char buf[64] = {0};
	
	at_init ();					// init AT commands
	at_set_custom_info (buf);
    at_port_print ("\r\nready\r\n");
    at_cmd_array_regist (&at_custom_cmd[0], sizeof (at_custom_cmd) / sizeof (at_custom_cmd[0]));	// setup user AT commands

	spi_master_init ();// ������������� HSPI ��� �������, 20���
	spi_req_intr_init ();// ������������� ���������� ������� �� �������� �� SPI
	spi_mast_byte_write (0xAA);

	// Configure the UART
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	system_init_done_cb (init_done);
}
