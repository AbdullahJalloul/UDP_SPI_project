#ifndef _SPI_FLASH_ADD_H
#define _SPI_FLASH_ADD_H

#include "spi_flash.h"

SpiFlashOpResult spi_flash_read_byte(uint32 src_addr, void *des_addr);
SpiFlashOpResult spi_flash_read_array(uint32 src_addr, void *des_addr, uint32 size);

#endif
