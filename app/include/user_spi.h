/*-----------------------------------------------------------------------------
* user_spi.h
*
*  Created on: 29.05.2016
*      Author: Ilya Petrukhin
------------------------------------------------------------------------------*/

#ifndef _USER_SPI_H_
#define _USER_SPI_H_

#include	"c_types.h"
#include	"lwip/udp.h"
#include	"lwip/pbuf.h"

enum
{
	USER_UDP_SEND = 0,
	USER_UDP_INIT,
	USER_UDP_STOP,
	USER_UDP_START,
	USER_UDP_RECEIVE,
	USER_TCP_SEND
};

void	spi_init			(void);
void	set_udata			(void);
void	user_tcp_receive_cb	(void *arg, char *pusrdata, unsigned short length);

#endif /* _USER_SPI_H_ */
