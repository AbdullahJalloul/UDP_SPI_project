/*
 * user_flash.h
 *
 *  Created on: 18.08.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _USER_FLASH_H_
#define _USER_FLASH_H_


typedef struct _flash_parameters
{
	u32		flash_id;			// id, прочитанный у флеш
	u8		mfgr_id;			// 0xEF is WinBond
	u8		type_id;
	u8		size_id;			// 
	u8		high_id;			// 4-th byte id
	u32		flash_size;			// flash size in bytes
	u32		sector_size;		// sector size in bytes
	u32		max_num_sector;
	u32		*user_addr;			// адрес пользовательского сектора в памяти SPI 
	u16		user_num_sector;
} flash_parameters;

typedef enum _flash_valid_values
{
	FLASH_RECORD_DELETED = 0,
	FLASH_RECORD_VALID = 0x62fe0a51,
	FLASH_RECORD_FREE = 0xffffffff
} flash_valid_values;

typedef struct _flash_record
{
	flash_valid_values	valid;
	u32					wifi_mode;
	u32					wifi_channel;
	u32					group_no;
	u32					channel_no;
	u32					dummy;
	u32					crc;
} flash_record;


bool	read_setup_flash		(void);
bool	write_setup_flash		(void);
void	read_flash_parameters	(flash_parameters *data);
bool	compare_user_data		(void);

#endif /* _USER_FLASH_H_ */
