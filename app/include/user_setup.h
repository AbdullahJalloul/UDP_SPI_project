/*------------------------------------------------------------------------------
* user_setup.h
*
*  Created on: 29.05.2016
*      Author: Ilya Petrukhin
------------------------------------------------------------------------------*/

#ifndef _USER_SETUP_H_
#define _USER_SETUP_H_

void	user_wifi_config	(void);
void	user_wifi_setup		(void);
bool	mem_compare			(u8 *a, u8 *b, u32 len);
void 	user_wifi_test		(void);

#endif /* _USER_SETUP_H_ */
