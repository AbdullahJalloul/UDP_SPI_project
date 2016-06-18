/*-----------------------------------------------------------------------------
* user_spi.h
*
*  Created on: 29.05.2016
*      Author: Ilya Petrukhin
------------------------------------------------------------------------------*/

#ifndef _USER_SPI_H_
#define _USER_SPI_H_

#include	"c_types.h"

void	spi_init			(void);
void	spi_byte_write		(u8 byte);
//void	spi_massive_write	(u32 *buf);
//void	spi_massive_read	(u32 *buf);
void	set_udata			(void);

#endif /* _USER_SPI_H_ */
