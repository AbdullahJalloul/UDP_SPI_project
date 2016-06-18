/*------------------------------------------------------------------------------
* user_main.c
*
*  Created on: 21.05.2016
*      Author: Ilya Petrukhin
------------------------------------------------------------------------------*/
#include	"ets_sys.h"
#include	"osapi.h"
//#include "at_custom.h"
#include	"user_interface.h"
#include	"driver/uart.h"
#include	"driver/iomux_registers.h"
#include	"driver/spi_registers.h"
#include 	"at.h"

#include	"user_tcp_server.h"
#include	"global.h"

#include	"user_setup.h"

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

static char macaddr[6];

const char *WiFiMode[] =
{
	"NULL",		// 0x00
	"STATION",	// 0x01
	"SOFTAP", 	// 0x02
	"STATIONAP"	// 0x03
};

buffers	b = {0};
global g =
{
	"IP309_0",
	"k38sSIP309_0",
	IP4_WORD(192, 168, 0, 20),	// ip адрес станции
	IP4_WORD(255, 255, 255, 0),	// маска сети
	IP4_WORD(192, 168, 0, 20),	// ip адрес гейтвея
	IP4_WORD(192, 168, 0, 21),	// ip адрес партнёра
	U_WIFI_AP_MODE,				// режим режим wi-fi
	0,							// канал wi-fi 1-13
	1,							// номер группы 0-9
	1,							// номер канала 0-9
	U_STATE_STOP,				// статус передачи
	b.receive[0],
	b.send[0]
};



/************************ typedef func ****************************************/
extern int ets_uart_printf (const char *fmt, ...);

void at_u_setup_test (uint8_t id);
void at_u_setup_query (uint8_t id);
void at_u_setup_setup (uint8_t id, char *pPara);
void at_u_setup_exe (uint8_t id);

void at_u_start_test (uint8_t id);
void at_u_start_query (uint8_t id);
void at_u_start_exe (uint8_t id);

void at_u_stop_test (uint8_t id);
void at_u_stop_query (uint8_t id);
void at_u_stop_exe (uint8_t id);


at_funcationType at_custom_cmd[] =
{
	{"+USETUP", 7, at_u_setup_test, at_u_setup_query,	at_u_setup_setup,	at_u_setup_exe},
	{"+USTART", 7, at_u_start_test, at_u_start_query,	NULL,				at_u_start_exe},
	{"+USTOP",	7, at_u_stop_test,	at_u_stop_query,	NULL,				at_u_stop_exe},
};

uint8 buffer[128] =
{0};

/*******************************************************************************
 * Функция ответа на команду AT+USETUP?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_setup_test (uint8_t id)
{
	ets_uart_printf ("User setup interface:\r\nAP/STA mode (0-1), wifi_ch (0-13), group (0-9), channel (0-9)\r\n");
	at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USTART?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_start_test (uint8_t id)
{
	ets_uart_printf ("Start user communication\r\n");
	at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USTOP?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_stop_test (uint8_t id)
{
	ets_uart_printf ("Stop user communication\r\n");
	at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USETUP=?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_setup_query (uint8_t id)
{
	ets_uart_printf ("User setup state:\r\n");
	if (g.wifi_mode == U_WIFI_AP_MODE)
	{
		ets_uart_printf ("WiFi in Access Point mode\r\n");
	}
	else
	{
		ets_uart_printf ("WiFi in Station mode\r\n");
	}
	ets_uart_printf ("WiFi channel no %d\r\n", g.wifi_ch);
	ets_uart_printf ("Group no %d\r\n", g.group_no);
	if (g.channel_no)
	{
		ets_uart_printf ("Channel no %d\r\n", g.channel_no);
	}
	else
	{
		ets_uart_printf ("Group mode\r\n");
	}
	at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USTART=?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_start_query (uint8_t id)
{
	if (g.state == U_STATE_STOP)
	{
		ets_uart_printf ("USTOP");
	}
	else if (g.state == U_STATE_SEARCH)
	{
		ets_uart_printf ("USEARCH");
	}
	else
	{
		// U_STATE_TRANSMIT
		ets_uart_printf ("UTRANSMIT");
	}
	at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USTOP=?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_stop_query (uint8_t id)
{
	if (g.state == U_STATE_STOP)
	{
		ets_uart_printf ("USTOP");
	}
	else if (g.state == U_STATE_SEARCH)
	{
		ets_uart_printf ("USEARCH");
	}
	else
	{
		// U_STATE_TRANSMIT
		ets_uart_printf ("UTRANSMIT");
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
void ICACHE_FLASH_ATTR at_u_setup_setup (uint8_t id, char *pPara)
{
	int result = 0;
	int err = 0;
	int flag = 0;
	u_wifi_modes	wifi_mode	= U_WIFI_STA_MODE;
	u8				wifi_ch		= 0;
	u8				group_no	= 0;
	u8				channel_no	= 0;
	
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
		wifi_mode = U_WIFI_STA_MODE;
	}
	else
	{
		wifi_mode = U_WIFI_AP_MODE;
	}
	while (*pPara == ',' || *pPara == ' ')
	{
		pPara++;
	}
	//get the second digit parameter
	flag = at_get_next_int_dec (&pPara, &result, &err);
	if (flag == FALSE)
	{
		at_response_error ();
		return;
	}
	wifi_ch = result;
	//get the third digit parameter
	while (*pPara == ',' || *pPara == ' ')
	{
		pPara++;
	}
	flag = at_get_next_int_dec (&pPara, &result, &err);
	if (flag == FALSE)
	{
		at_response_error ();
		return;
	}
	group_no = result;
	//get the fourth digit parameter
	while (*pPara == ',' || *pPara == ' ')
	{
		pPara++;
	}
	flag = at_get_next_int_dec (&pPara, &result, &err);
	// last parameter without check flag
	channel_no = result;
	if (wifi_ch > U_WI_FI_CH_MAX)
	{
		wifi_ch = U_WI_FI_CH_MAX;
	}
	if (group_no > U_GROUP_NO_MAX)
	{
		group_no = U_GROUP_NO_MAX;
	}
	if (channel_no > U_CHANNEL_NO_MAX)
	{
		channel_no = U_CHANNEL_NO_MAX;
	}
	g.wifi_mode = wifi_mode;
	g.wifi_ch = wifi_ch;
	g.group_no = group_no;
	g.channel_no = channel_no;
	at_response_ok ();
	// TODO: настроить режим Wi-fi, адреса и маски
}

/*******************************************************************************
 * Функция ответа на команду AT+USETUP
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_setup_exe (uint8_t id)
{
	ets_uart_printf ("at_u_setup_exe\r\n");
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

void init_done (void)
{
#if DEBUG_LEVEL > 0
	// Print system info
	ets_uart_printf ("\r\n");
	ets_uart_printf ("ESP8266 platform starting...\r\n");
	ets_uart_printf ("System info:\r\n");
	ets_uart_printf ("SDK version: %s\r\n", system_get_sdk_version ());
	ets_uart_printf ("Time = %ld\r\n", system_get_time ());
	ets_uart_printf ("Chip id = 0x%x\r\n", system_get_chip_id ());
	ets_uart_printf ("CPU freq = %d MHz\r\n", system_get_cpu_freq ());
	ets_uart_printf ("Free heap size = %d\r\n", system_get_free_heap_size ());
#endif	// #if DEBUG_LEVEL > 0
	
	//if(wifi_get_opmode() != USE_WIFI_MODE)
	{
#if DEBUG_LEVEL > 0
		ets_uart_printf ("ESP8266 is %s mode, restarting in %s mode...\r\n", WiFiMode[wifi_get_opmode ()], WiFiMode[USE_WIFI_MODE]);
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
	ets_uart_printf ("Wi-Fi mode: %s\r\n", WiFiMode[wifi_get_opmode ()]);
	if (USE_WIFI_MODE & SOFTAP_MODE)
	{
		struct softap_config apConfig;
		if (wifi_softap_get_config (&apConfig))
		{
			ets_uart_printf ("AP config: SSID: %s, PASSWORD: %s\r\n",
			        apConfig.ssid,
			        apConfig.password);
		}
	}
	if (USE_WIFI_MODE & STATION_MODE)
	{
		struct station_config stationConfig;
		if (wifi_station_get_config (&stationConfig))
		{
			ets_uart_printf ("STA config: SSID: %s, PASSWORD: %s\r\n",
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
		if (!tcpsrv_start (p))
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
		if (!tcpsrv_start (p))
		{
			tcpsrv_close (p);
		}
	}
#if DEBUG_LEVEL > 0
	ets_uart_printf ("ESP8266 platform started!\r\n");
#endif
}

/*******************************************************************************
* Главная программа
*******************************************************************************/
void ICACHE_FLASH_ATTR user_init (void)
{
	char buf[128] =	{0};
	
	// Configure the UART
	uart_init (BIT_RATE_115200, BIT_RATE_115200);

	
//	ets_uart_printf ("Macro address %x, macro mask %x\n", addr1, test1);
//	ets_uart_printf ("Struct address %x, struct mask %x\n", addr2, test2);
	
	ets_uart_printf ("\r\nready\r\n");
	at_init ();					// init AT commands
	at_set_custom_info (buf);
	at_cmd_array_regist (&at_custom_cmd[0], sizeof (at_custom_cmd) / sizeof (at_custom_cmd[0]));	// setup user AT commands
	
	spi_init ();	// init spi1
	set_udata ();	// set data at b[0]
	
	
//	system_init_done_cb (init_done);
}
