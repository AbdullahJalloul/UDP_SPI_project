/*
 * global.h
 *
 *  Created on: 24.05.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "lwip/ip_addr.h"

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

typedef struct _um_t	// структура управления модулем ESP8266
{
	u_wifi_modes	wifi_mode;	// режим wi-fi
	u8				wifi_ch;	// канал wi-fi 1-13
	u8				group_no;	// номер группы 0-9
	u8				channel_no;	// номер канала 1-9, 0 - групповой
	u_states		state;		// статус передачи 
} um_t;


typedef struct _global
{
	char			base_name[16];
	char			password[16];
	struct ip_info	ipinfo;
	struct ip_addr	ip_other;
} global;

extern global g;

#endif /* _GLOBAL_H_ */
