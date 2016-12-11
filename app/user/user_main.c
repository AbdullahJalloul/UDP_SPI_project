/*------------------------------------------------------------------------------
* user_main.c
*
*  Created on: 21.05.2016
*      Author: Ilya Petrukhin
------------------------------------------------------------------------------*/
#include	<ets_sys.h>
#include	<osapi.h>
#include	<c_types.h>
#include	<bits.h>
//#include	<at.h>
#include	<at_custom.h>
#include	"driver/uart.h"
//#include "at_custom.h"
#include	<user_interface.h>
#include	<ESP8266_registers.h>
#include	"lwip/app/espconn.h"
#include	"lwip/app/espconn_udp.h"
//#include	"user_tcp_server.h"

#include	"global.h"

#include	"wifi_ap_tcp_server_setup.h"
#include	"wifi_station_tcp_client.h"
#include	"user_spi.h"
#include	"user_flash.h"

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
* Порт 5002 - radio ethernet (для справки)
*
*******************************************************************************/

/*********************** define constants *************************************/


buffers	b __attribute__((aligned(4), section(".iram.data")));
global g __attribute__((aligned(4), section(".iram.data")));


/************************ typedef func ****************************************/
int ets_uart_printf (const char *fmt, ...);

static void 	at_u_setup_test (uint8_t id);
static void 	at_u_setup_query (uint8_t id);
static void 	at_u_setup_setup (uint8_t id, char *pPara);
static void	 	at_u_setup_exe (uint8_t id);

static void 	at_u_start_test (uint8_t id);
static void 	at_u_start_query (uint8_t id);
static void 	at_u_start_exe (uint8_t id);

static void 	at_u_stop_test (uint8_t id);
static void 	at_u_stop_query (uint8_t id);
static void 	at_u_stop_exe (uint8_t id);

static void 	init_done (void);
static void		data_init (void);
static void		user_wifi_setup		(void);

at_funcationType at_custom_cmd[3] =
{
	{"+USETUP", 7, at_u_setup_test, at_u_setup_query,	at_u_setup_setup,	at_u_setup_exe},
	{"+USTART", 7, at_u_start_test, at_u_start_query,	NULL,				at_u_start_exe},
	{"+USTOP",	6, at_u_stop_test,	at_u_stop_query,	NULL,				at_u_stop_exe},
};

const char *WiFiMode[] =
{
	"NULL",		// 0x00
	"STATION",	// 0x01
	"SOFTAP", 	// 0x02
	"STATIONAP"	// 0x03
};

/*******************************************************************************
 * Функция ответа на команду AT+USTART=?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_start_test (uint8_t id)
{
	if (g.state == U_STATE_TRANSMIT)
	{
		ets_uart_printf ("Ready to user communication\n");		
		at_response_ok ();
	}
	else
	{
		ets_uart_printf ("No ready to user communication\n");
		at_response_error ();
	}
}

/*******************************************************************************
* Функция ответа на команду AT+USTART
* Мы получили подтверждение соединения WiFi
* Теперь соединяемся по UDP
******************************************************************************/
void ICACHE_FLASH_ATTR at_u_start_exe (uint8_t id)
{
	int val;
	struct ip_info	get_ip;
	
	// Начать трансляцию, когда найдётся партнёр
/*	if (g.state == U_STATE_TRANSMIT)
	{
		val = user_udp_init ();
	}
*/
	wifi_get_ip_info (STATION_IF, &get_ip);
	ets_uart_printf ("Read station info:\n");
	ets_uart_printf ("ip = " IPSTR "\n", IP2STR (&get_ip.ip));
	ets_uart_printf ("gw = " IPSTR "\n", IP2STR (&get_ip.gw));
	ets_uart_printf ("mask = " IPSTR "\n", IP2STR (&get_ip.netmask));
	wifi_get_ip_info (SOFTAP_IF, &get_ip);
	ets_uart_printf ("Read soft ap info:\n");
	ets_uart_printf ("ip = " IPSTR "\n", IP2STR (&get_ip.ip));
	ets_uart_printf ("gw = " IPSTR "\n", IP2STR (&get_ip.gw));
	ets_uart_printf ("mask = " IPSTR "\n", IP2STR (&get_ip.netmask));

	at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USTART?
 * Ответ на запрос состояния
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_start_query (uint8_t id)
{
	if (g.state == U_STATE_STOP)
	{
		ets_uart_printf ("TCP stop\n");
		at_response_error ();
	}
	else if (g.state == U_STATE_SEARCH)
	{
		ets_uart_printf ("TCP search\n");
		at_response_error ();
	}
	else	// U_STATE_TRANSMIT
	{
		ets_uart_printf ("TCP transmit\n");
		at_response_ok ();
	}
}

/*******************************************************************************
 * Функция ответа на команду AT+USTOP=?
 * Справочная функция
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_stop_test (uint8_t id)
{
	ets_uart_printf ("To stop user communication\n");		
	at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USTOP?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_stop_query (uint8_t id)
{
	if (g.state == U_STATE_STOP)
	{
		ets_uart_printf ("UDP stop\n");
	}
	else if (g.state == U_STATE_SEARCH)
	{
		ets_uart_printf ("UDP search\n");
	}
	else	// U_STATE_TRANSMIT
	{
		ets_uart_printf ("UDP transmit\n");
	}
	at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USTOP
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_stop_exe (uint8_t id)
{
	// TODO: остановить передачу пакетов
	g.state = U_STATE_STOP;
	at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USETUP=?
 * Справочная функция
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_setup_test (uint8_t id)
{
	ets_uart_printf ("User setup interface:\r\nAP/STA mode (0-1), wifi_ch (0-13), group (0-9), channel (0-9)");
	at_response_ok ();
}

/*******************************************************************************
 * Функция ответа на команду AT+USETUP?
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_setup_query (uint8_t id)
{
	// Ответ для человека
	ets_uart_printf ("User setup state:\r\n");
	ets_uart_printf ("Wi-Fi mode: %s\r\n", WiFiMode[wifi_get_opmode ()]);
/*	if (g.wifi_mode == U_WIFI_AP_MODE)
	{
		ets_uart_printf ("WiFi in Access Point mode\r\n");
	}
	else
	{
		ets_uart_printf ("WiFi in Station mode\r\n");
	}
*/
	ets_uart_printf ("WiFi channel no %d\r\n", g.wifi_ch);
	ets_uart_printf ("Group no %d\r\n", g.group_no);
	if (g.channel_no)
	{
		ets_uart_printf ("Channel no %d\n", g.channel_no);
	}
	else
	{
		ets_uart_printf ("Group mode\n");
	}
//	ets_uart_printf ("Expected index %d Receive index %d\n", g.exp_index, g.rec_index);
	// Ответ для процессора
	ets_uart_printf ("+USETUP=%d,%d,%d,%d\r\n", g.wifi_mode, g.wifi_ch, g.group_no, g.channel_no);
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
	int		result = 0;
	int		err = 0;
	bool	flag = 0;
	char	ssid[32]		= "IP309_0";
	char	password[64]	= "k38sSIP309_0";

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
	// ограничение по максимальным значениям
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
	// записать параметры
	g.wifi_mode = wifi_mode;
	g.wifi_ch = wifi_ch;
	g.group_no = group_no;
	g.channel_no = channel_no;
	at_response_ok ();
	// настроить режим Wi-fi, адреса и маски
	user_wifi_setup ();
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


/*******************************************************************************
 * Функция ответа на команду AT+USETUP
 ******************************************************************************/
void ICACHE_FLASH_ATTR at_u_setup_exe (uint8_t id)
{
	u8 status = wifi_station_get_connect_status ();
	ets_uart_printf ("WiFi connect status is %d\r\n", status);
	at_response_ok ();
}

/*******************************************************************************
 * Коллбэк после старта ESP8266
 ******************************************************************************/
void ICACHE_FLASH_ATTR init_done (void)
{
	// Print system info
/*	ets_uart_printf ("\r\n");
	ets_uart_printf ("ESP8266 platform starting...\r\n");
	ets_uart_printf ("System info:\r\n");
	ets_uart_printf ("SDK version: %s\r\n", system_get_sdk_version ());
	ets_uart_printf ("Time = %ld\r\n", system_get_time ());
	ets_uart_printf ("Chip id = 0x%x\r\n", system_get_chip_id ());
	ets_uart_printf ("CPU freq = %d MHz\r\n", system_get_cpu_freq ());
	ets_uart_printf ("Free heap size = %d\r\n", system_get_free_heap_size ());
*/
	if (g.wifi_mode == U_WIFI_STA_MODE)
	{
		wifi_station_connect ();
	}

	ets_uart_printf ("\r\nready\r\n");
}

/*******************************************************************************
* Главная программа
*******************************************************************************/
void ICACHE_FLASH_ATTR user_init (void)
{		
	// Configure the UART
	uart_init (BIT_RATE_115200, BIT_RATE_115200);
	// read user flash data and set default data
	data_init ();
	// init AT commands
	at_init ();
	at_cmd_array_regist (at_custom_cmd, 3);	// sizeof (at_custom_cmd) / sizeof (at_custom_cmd[0]));	// setup user AT commands
	// init spi1
	spi_init ();
	// установки wifi
	if (g.wifi_mode == U_WIFI_AP_MODE)
	{
		wifi_config_ap ();
	}
	else
	{
		wifi_config_station ();
	}
	
	system_init_done_cb (init_done);
}


/*******************************************************************************
* заполнение глобальных структур
*******************************************************************************/
void ICACHE_FLASH_ATTR data_init (void)
{

	read_setup_flash ();
	
	g.state = U_STATE_STOP;
	g.need_transmit = 0;		// сбросим флаг для ошибки #1
	g.spi_transmit_mode = 0;	// приём
	g.user_pcbv = NULL;
	g.wifi_pw = 20;

	g.spi_read_size = 0;
	g.spi_read_index = 0;
	g.spi_transmit_size = 0;
	g.spi_transmit_index = 0;

	// тестовые значения

}


/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABBBCDDD
 *                A : rf cal
 *                B : at parameters
 *                C : rf init data
 *                D : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set (void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 8;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}


