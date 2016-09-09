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
#include	"lwip/ip_addr.h"
#include	"lwip/udp.h"
//#include	"lwip/app/espconn.h"
#include	"lwip/err.h"


/************** настройки глобальных параметров *******************************/
#define	U_WI_FI_CH_MAX		(13)	// максимальный номер канала wi-fi 1-13
#define	U_GROUP_NO_MAX		(9)		// максимальный номер группы 0-9
#define	U_CHANNEL_NO_MAX	(9)		// максимальный номер канала 0-9

#define	NUM_BUFFERS			4		// количество буферов для передачи данных
#define	SIZE_BUFFERS		16		// количество слов u32 в буфере
#define	USER_FLASH_PAGE		0x000E	//0xFF8	// пользовательская страница в памяти SPI FLASH
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
	u8				wifi_ch;		// канал wi-fi 1-13
	u8				group_no;		// номер группы 0-9
	u8				channel_no;		// номер канала 1-9, 0 - групповой
	u_states		state;			// статус передачи 		
	u8				need_transmit;	// флаг для запуска отложенной передачи
	u8				spi_read_index;	// счётчик принятых посылок
	u8				spi_transmit_mode;	// 1-передача 0-приём
//	struct espconn	user_udp_struct;	// Статическая структура с параметрами UDP соединения
//	esp_udp			user_esp_udp;	// структура параметров UDP соединения
	struct udp_pcb *user_pcbv;
// тестовые переменные
	u32				transmit_index;
	u32				receive_index;
	u32				num_err;
	u32				save_index;
	u32				save_lost;
	u32				save_err;
	u32				count_err;
	u32				exp_index;
	u32				rec_index;
} global;

typedef struct _buffers
{
	u32	spi_receive [NUM_BUFFERS][SIZE_BUFFERS];	// буфер данных, принятых по SPI
	u32	spi_send [NUM_BUFFERS][SIZE_BUFFERS];	// буфер данных для отправки по SPI
} buffers;


extern global	g;
extern buffers	b;

#endif /* _GLOBAL_H_ */
