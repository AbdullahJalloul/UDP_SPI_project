/*
 * global.h
 *
 *  Created on: 24.05.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "lwip/ip_addr.h"

/************** настройки глобальных параметров *******************************/
#define	U_WI_FI_CH_MAX		(13)	// максимальный номер канала wi-fi 1-13
#define	U_GROUP_NO_MAX		(9)		// максимальный номер группы 0-9
#define	U_CHANNEL_NO_MAX	(9)		// максимальный номер канала 0-9

#define	NUM_BUFFERS			2	// количество буферов для передачи данных
#define	SIZE_BUFFERS		15	// количество слов u32 в буфере

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

typedef struct _global
{
	char			base_name[16];	// имя сети
	char			password[16];	// пароль подключения к сети
	struct ip_info	ipinfo;			// ip адреса и маски своей станции
	struct ip_addr	ip_other;		// ip адреса и маски станции - партнёра
	u_wifi_modes	wifi_mode;		// режим wi-fi
	u8				wifi_ch;		// канал wi-fi 1-13
	u8				group_no;		// номер группы 0-9
	u8				channel_no;		// номер канала 1-9, 0 - групповой
	u_states		state;			// статус передачи 		
	u32 *			p_write;		// адрес буфера для передачи по spi
	u32 *			p_read;			// адрес буфера для приёма данных от spi
	u8				need_transmit;	// флаг для запуска отложенной передачи
	u8				spi_transmit_mode;	// 1-передача 0-приём
} global;

typedef struct _buffers
{
	u32	receive[NUM_BUFFERS][SIZE_BUFFERS];
	u32	send[NUM_BUFFERS][SIZE_BUFFERS];
} buffers;


extern global	g;
extern buffers	b;

#endif /* _GLOBAL_H_ */
