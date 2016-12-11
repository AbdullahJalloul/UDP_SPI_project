/*------------------------------------------------------------------------------
* user_setup.h
*
*  Created on: 29.05.2016
*      Author: Ilya Petrukhin
------------------------------------------------------------------------------*/

#ifndef _USER_SETUP_H_
#define _USER_SETUP_H_

extern struct udp_pcb *user_pcbv;

void		user_wifi_config	(void);
void		user_wifi_setup		(void);
void 		user_wifi_test		(void);


#endif /* _USER_SETUP_H_ */
