/*
 * global.h
 *
 *  Created on: 24.05.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include	<c_types.h>
#include	<bits.h>
#include	<ets_sys.h>
#include	"lwip/ip_addr.h"
#include	"lwip/udp.h"
#include	"lwip/err.h"
#include	"lwip/app/espconn.h"


/************** настройки глобальных параметров *******************************/
#define	U_WI_FI_CH_MAX		(13)	// максимальный номер канала wi-fi 1-13
#define	U_GROUP_NO_MAX		(9)		// максимальный номер группы 0-9
#define	U_CHANNEL_NO_MAX	(9)		// максимальный номер канала 0-9

#define	NUM_BUFFERS			4		// количество буферов для передачи данных
#define	SIZE_BUFFERS		16		// количество слов u32 в буфере
#define	USER_FLASH_PAGE		0x000E	//0xFF8	// пользовательская страница в памяти SPI FLASH

// заготовка мак адреса для всех режимов
#define	WIFI_MAC			{0x1a, 0xfe, 0x34, 0xd6, 0xdd, 0x00}
#define	WIFI_IP_MASK		255, 255, 255, 0
// параметры для режима WiFi_station_mode
#define WIFI_STA_SSID		"IP309_0"
#define WIFI_STA_PASSWORD	"k38sSIP309_0"
#define	WIFI_STA_IP			192, 168, 0, 30
#define	WIFI_STA_MAC		{0x1a, 0xfe, 0x34, 0xd6, 0xdd, 0x00}
#define TCP_STA_PORT		1212
// параметры для режима WiFi_assess_point_mode
#define WIFI_AP_SSID		"IP309_0"
#define WIFI_AP_PASSWORD	"k38sSIP309_0"
#define	WIFI_AP_MAC			{0x1a, 0xfe, 0x34, 0xd6, 0xdd, 0x00}
#define	WIFI_AP_IP			192, 168, 0, 20
#define TCP_AP_PORT			1210
/******************************************************************************/


typedef enum 			// режим работы модуля
{
	U_WIFI_AP_MODE = 0,
	U_WIFI_STA_MODE
} u_wifi_modes;

typedef enum			// состояние передачи данных модуля
{
	U_STATE_STOP = 0,
	U_STATE_SEARCH,
	U_STATE_TRANSMIT
} u_states;

//typedef struct udp_pcb

typedef struct _global
{
	char			ssid[32];		// имя сети
	char			password[64];	// пароль подключения к сети
	struct ip_info	local_ip;		// ip адреса и маски своей станции
	uint8			local_mac[6];	// MAC адрес своей станции
	u16				local_port;		//
	ip_addr_t		remote_ip;		// ip адреса и маски станции - партнёра
	uint8			remote_mac[6];	// MAC адрес станции - партнёра
	u16				remote_port;	//
	u_wifi_modes	wifi_mode;		// режим wi-fi
	u_states		state;			// статус передачи
	u8				wifi_ch;		// канал wi-fi 1-13
	u8				wifi_pw;		// мощность WiFi передатчика
	u8				group_no;		// номер группы 0-9
	u8				channel_no;		// номер канала 1-9, 0 - групповой
	u8				need_transmit;	// флаг для запуска отложенной передачи
	u8				spi_transmit_mode;	// 1-передача 0-приём
	u8				spi_read_index;		// номер буфера для приёма данных от SPI
	u8				spi_transmit_index;	// номер буфера для передачи данных по SPI
	u32				spi_read_size;		// размер принятых по SPI данных
	u32				spi_transmit_size;	// объём данных для передачи по SPI

//	struct espconn	user_udp_struct;	// Статическая структура с параметрами UDP соединения
//	esp_udp			user_esp_udp;	// структура параметров UDP соединения
	struct udp_pcb *user_pcbv;

	struct espconn	user_tcp_conn;	//
	esp_tcp			user_tcp;
//	struct espconn	client_tcp_conn;	//
//	esp_tcp			client_tcp;

	ETSTimer		timer;
	// тестовые переменные

} global;

typedef struct _buffers
{
	u32	spi_receive[NUM_BUFFERS][SIZE_BUFFERS];	// буфер данных, принятых по SPI
	u32	spi_send[NUM_BUFFERS][SIZE_BUFFERS];	// буфер данных для отправки по SPI
} buffers;


extern global	g;
extern buffers	b;

#endif /* _GLOBAL_H_ */
