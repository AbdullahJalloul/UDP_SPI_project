/*------------------------------------------------------------------------------
* user_flash.c
* Программы xntybz и записи данных на флеш
*  Created on: 18.08.2016
*      Author: Ilya Petrukhin
------------------------------------------------------------------------------*/
#include	<c_types.h>
#include	<bits.h>
#include	"global.h"
#include	"user_flash.h"
#include	"driver/spi_flash.h"
#include	"driver/spi_flash_add.h"
#include	"lwip/mem.h"

u32				crc_32				(const u32 *data, u32 len);
bool			memory_control		(flash_record *data);
flash_record	*find_user_record	(void);

flash_parameters		f;

/*******************************************************************************
* Чтение данных из флеш. Проверка на валидность.
* При ошибке - заполнение дефолтными значениями
* Выход: заполнение структуры g.
*******************************************************************************/
bool ICACHE_FLASH_ATTR read_setup_flash (void)
{
	flash_record	*user_record;
	flash_record	data;
	int				memory_err		= 0;
	u32				crc				= 0;
//	SpiFlashOpResult ret;
	
	// определение параметров флеш на плате
	read_flash_parameters (&f);
/*// тестовая печать
	ets_uart_printf ("id of flash = %x\n", f.flash_id);
	ets_uart_printf ("Flash size = %d\n", f.flash_size);
	ets_uart_printf ("User sector = %x\n", f.user_num_sector);
*/	
/*// тест - печать первой записи
	spi_flash_read_array ((u32)f.user_addr, &data, sizeof (data));
	ets_uart_printf ("read first data\n");
	ets_uart_printf ("at address %x\n", f.user_addr);
	ets_uart_printf ("valid value %x\n", data.valid);
	ets_uart_printf ("wifi mode %d\n", data.wifi_mode);
	ets_uart_printf ("wifi channel %d\n", data.wifi_channel);
	ets_uart_printf ("group no %d\n", data.group_no);
	ets_uart_printf ("channel no %d\n", data.channel_no);
	ets_uart_printf ("crc %x\n", data.crc);
*/	
	user_record = find_user_record ();
	if (user_record == NULL)	// не нашли запись
	{
		memory_err = 1;
		user_record = (flash_record *)f.user_addr;
//		ets_uart_printf ("don't find user record\n");
	}
	else						// запись есть, проверяем данные
	{
		spi_flash_read_array ((u32)user_record, &data, sizeof (data));
//		ets_uart_printf ("result of read flash = %d\n", ret);
		// проверка на соответствие CRC записи
		crc = crc_32 ((u32 *)&data, (sizeof (data) / sizeof (u32)));
		if (crc)	// если не правильная запись во FLASH - ошибка
		{
//			ets_uart_printf ("error user flash crc = %x\n", crc);
			memory_err = 1;
		}
		if (!memory_control (&data))
		{
/*			ets_uart_printf ("error user memory control\n", crc);
			ets_uart_printf ("read:\nwifi mode %d\n", data.wifi_mode);
			ets_uart_printf ("wifi channel %d\n", data.wifi_channel);
			ets_uart_printf ("group no %d\n", data.group_no);
			ets_uart_printf ("channel no %d\n", data.channel_no);
*/			memory_err = 1;
		}
	}
	if (!memory_err)// если нет ошибок - читаем данные из FLASH
	{
		g.wifi_mode = (u_wifi_modes)data.wifi_mode;
		g.wifi_ch = (u8)data.wifi_channel;
		g.group_no = (u8)data.group_no;
		g.channel_no = (u8)data.channel_no;
		ets_uart_printf ("\nRead user data at address %x\n", user_record);
	}
	else	// есть ошибки - заполняем дефолтными значениями
	{
		g.wifi_mode = U_WIFI_AP_MODE;
		g.wifi_ch = 3;
		g.group_no = 1;
		g.channel_no = 1;
		ets_uart_printf ("write default user data\n");
		write_setup_flash ();
	}
	
	return true;
}

/*******************************************************************************
* запись пользовательских данных на флеш.
* Вход: нет
* Выход: нет
*******************************************************************************/
bool ICACHE_FLASH_ATTR write_setup_flash (void)
{
	flash_record	*user_record;
	flash_record	user_data;
//	SpiFlashOpResult w;
	u32				del = FLASH_RECORD_DELETED;
	u32				temp;
	
	user_record = find_user_record ();
//	ets_uart_printf ("Current record %x\n", user_record);
	
	if (user_record == 0)	// запись не найдена
	{
		// читаем первое слово
		spi_flash_read_array ((u32)f.user_addr, &temp, sizeof (temp));
//		ets_uart_printf ("spi_flash_read_array: %x\n", w);
		if (temp != FLASH_RECORD_FREE)	// сектор нужно стереть
		{
//			ets_uart_printf ("At first erasing sector number %x\n", f.user_num_sector);
			spi_flash_read_array ((u32)f.user_addr, &temp, sizeof (temp));
//			ets_uart_printf ("spi_flash_read_array: %x\n", w);
//			ets_uart_printf ("Before erasing address %x value %x\n", f.user_addr, temp);
			ETS_UART_INTR_DISABLE();
			spi_flash_erase_sector (f.user_num_sector);	// стираем сектор
			ETS_UART_INTR_ENABLE();
//			ets_uart_printf ("spi_flash_erase_sector: %x\n", w);
			spi_flash_read_array ((u32)f.user_addr, &temp, sizeof (u32));
//			ets_uart_printf ("spi_flash_read_array: %x\n", w);
//			ets_uart_printf ("After erasing address %x value %x\n", f.user_addr, temp);
		}
		user_record = (flash_record *)f.user_addr;	// установка начального адреса
	}
	else	// запись найдена
	{
		// определяем, есть ли свободное место в секторе
		if (((u32 *)user_record + sizeof (user_record)) >= (f.user_addr + f.sector_size))	// нет места -
		{
/*			ets_uart_printf ("No free space, erasing sector\n");
			spi_flash_read_array ((u32)f.user_addr, &temp, sizeof (temp));
			ets_uart_printf ("Byte before erasing = %x\n", temp);
*/
			ETS_UART_INTR_DISABLE();
			spi_flash_erase_sector (f.user_num_sector);	// стираем сектор
			ETS_UART_INTR_ENABLE();
//			spi_flash_read_array ((u32)f.user_addr, &temp, sizeof (temp));
//			ets_uart_printf ("Byte after erasing = %x\n", temp);
			user_record = (flash_record *)f.user_addr;	// устанавливаем указатель на начало
		}
		else	// есть место - делаем текущую запись недействительной
		{
/*			ets_uart_printf ("Deleting current record\n");
			spi_flash_read_array ((u32)user_record, &temp, sizeof (temp));
			ets_uart_printf ("Byte before erasing = %x\n", temp);
*/
			ETS_UART_INTR_DISABLE();
			spi_flash_write ((u32)user_record, &del, sizeof(u32));
			ETS_UART_INTR_ENABLE();
//			spi_flash_read_array ((u32)user_record, &temp, sizeof (temp));
//			ets_uart_printf ("Byte after erasing = %x\n", temp);
			user_record++;
		}
	}
//	ets_uart_printf ("New current record %x\n", user_record);	

	// пишем новую запись
	user_data.valid = FLASH_RECORD_VALID;
	user_data.wifi_mode = (u32)g.wifi_mode;
	user_data.wifi_channel = (u32)g.wifi_ch;
	user_data.channel_no = (u32)g.channel_no;
	user_data.group_no = (u32)g.group_no;
	user_data.crc = 0;
	user_data.crc = crc_32 ((u32 *)&user_data, (sizeof (flash_record) / sizeof (u32)));

//	ets_uart_printf ("Write current record\n");
	ETS_UART_INTR_DISABLE();
	spi_flash_write ((u32)user_record, (u32 *)&user_data, sizeof (user_data));
	ETS_UART_INTR_ENABLE();
/*	
	ets_uart_printf ("write flash data\n");
	ets_uart_printf ("at address %x\n", user_record);
	ets_uart_printf ("valid value %x\n", user_data.valid);
	ets_uart_printf ("wifi mode %d\n", user_data.wifi_mode);
	ets_uart_printf ("wifi channel %d\n", user_data.wifi_channel);
	ets_uart_printf ("group no %d\n", user_data.group_no);
	ets_uart_printf ("channel no %d\n", user_data.channel_no);
	ets_uart_printf ("crc %x\n", user_data.crc);
*/
	return true;
}



/******************************************************************************
* подсчёт контрольной суммы с дополнением самой к себе
* для массива из 32 битных слов
******************************************************************************/
u32 ICACHE_FLASH_ATTR crc_32 (const u32 *data, u32 len)
{
	u32		crc = 0;

	while (len)
	{
		crc += *data;
 		if (crc < *data)
		{
			crc++;
		}
		data++;
		len--;
 	}
	crc = ~crc;
	return crc;
}

/*******************************************************************************
* Проверка сохранённых значений на валидность
*******************************************************************************/
bool ICACHE_FLASH_ATTR memory_control (flash_record *data)
{
	bool ret_value = true;
	if (data->wifi_mode > U_WIFI_STA_MODE)
	{
		ret_value = false;
	}
	if (data->wifi_channel > U_WI_FI_CH_MAX)
	{
		ret_value = false;
	}
	if (data->channel_no > U_CHANNEL_NO_MAX)
	{
		ret_value = false;
	}
	if (data->group_no > U_GROUP_NO_MAX)
	{
		ret_value = false;
	}
	return ret_value;
}

/*******************************************************************************
* Возвращает true, если данные не равны
* Выход - результат сравнения (равно == true)
*******************************************************************************/
bool ICACHE_FLASH_ATTR compare_user_data (void)
{
	flash_record	*user_record;
	flash_record 	data;
	bool			ret_value = true;
	u32				crc;

	user_record = find_user_record ();
	if (user_record)
	{
		// чтение записи
		spi_flash_read_array ((u32)user_record, &data, sizeof (data));
		// проверка CRC
		crc = crc_32 ((u32 *)&data, (sizeof (data) / sizeof (u32)));
		if (crc)	// если не правильная запись во FLASH - ошибка
		{
			ret_value = false;	// не сходится CRC
		}
		if (data.wifi_mode != (u32)g.wifi_mode)
		{
			ret_value = false;	// не равны режимы
		}
		if (data.wifi_channel != (u32)g.wifi_ch)
		{
			ret_value = false;	// не равны каналы wifi
		}
		if (data.group_no != (u32)g.group_no)
		{
			ret_value = false;	// не равны группы
		}
		if (data.channel_no != (u32)g.channel_no)
		{
			ret_value = false;	// не равны номера каналов
		}
	}
	else
	{
		ret_value = false;	// запись не найдена
	}
		
	return (ret_value);
}

/*******************************************************************************
* Чтение id SPI FLASH и заполнение структуры параметров
*******************************************************************************/
void ICACHE_FLASH_ATTR read_flash_parameters (flash_parameters *data)
{
	data->flash_id = spi_flash_get_id ();
	data->mfgr_id = data->flash_id && 0xff;
	data->type_id = (data->flash_id >> 8) & 0xff;
	data->size_id = (data->flash_id >> 16) & 0xff;
	data->high_id = (data->flash_id >> 24) & 0xff;
	data->flash_size = 1 << data->size_id;
	data->sector_size = SPI_FLASH_SEC_SIZE;
	data->max_num_sector = (data->flash_size / SPI_FLASH_SEC_SIZE) - 1;
//	data->user_num_sector = data->max_num_sector - 4;					// 16кБ=4сектора размер системной памяти
	data->user_num_sector = USER_FLASH_PAGE;
	data->user_addr = (u32 *)(data->user_num_sector * SPI_FLASH_SEC_SIZE);
}


/*******************************************************************************
* Поиск первой валидной записи внутри сектора
*******************************************************************************/
flash_record * ICACHE_FLASH_ATTR find_user_record (void)
{
	u32 validness;
	flash_record *address = (flash_record *)f.user_addr;
	
	while ((u32)address < ((u32)f.user_addr + f.sector_size))
	{
		spi_flash_read_array ((u32)address, &validness, sizeof (validness));
		if (validness == FLASH_RECORD_VALID)
		{
//			ets_uart_printf ("find user record = %x\n", address);
			return address;
		}
		address++;
	}
		ets_uart_printf ("no find user record\n");
	return NULL;
}

