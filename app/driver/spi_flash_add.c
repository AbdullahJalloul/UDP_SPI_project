/******************************************************************************
 * PV` FileName: spi_flash_add.c
*******************************************************************************/

#include	"ets_sys.h"
#include	"osapi.h"
#include	"os_type.h"
#include	"driver/spi_flash.h"
#include	"driver/spi_flash_add.h"

/******************************************************************************
 * FunctionName : spi_flash_read_byte
 * Description  : ������ ������ ����� �� flash
 * Parameters   : flash Addr, pointer
 * Returns      : SpiFlashOpResult 0 - ok
*******************************************************************************/
SpiFlashOpResult spi_flash_read_byte(uint32 src_addr, void *des_addr)
{
	uint8 flash_buf[4];
	SpiFlashOpResult ret = spi_flash_read (src_addr, (uint32 *)flash_buf, 1);
	uint8 *pbd = des_addr;
	*pbd = flash_buf[0];
	return ret;
}
/******************************************************************************
 * FunctionName : spi_flash_read_array
 * Description  : ������ ����� ���� �� flash
 * Parameters   : flash Addr, pointer, ���-��
 * Returns      : SpiFlashOpResult 0 - ok
*******************************************************************************/
SpiFlashOpResult spi_flash_read_array (uint32 src_addr, void *des_addr, uint32 size)
{
	SpiFlashOpResult ret = SPI_FLASH_RESULT_OK;
	uint8 *pbd = (uint8 *)des_addr;
	
	if (des_addr == NULL)
	{
		return ret;
	}
	while (size) 
	{
		int x = (int)pbd & 3;
		if (x) 
		{
			uint8 flash_buf[4];
			if (x > size) x = size;
			ret = spi_flash_read (src_addr, (uint32 *)flash_buf, x);
			if (ret != SPI_FLASH_RESULT_OK)
			{
				return ret;
			}
			uint8 *pbs = flash_buf;
			while (x--) 
			{
				*pbd++ = *pbs++;
				src_addr++;
				size--;
			};
		}
		else 
		{
			return spi_flash_read (src_addr, (uint32 *)pbd, size);
		}
	};
	return (ret);
}

