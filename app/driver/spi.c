#include "driver/spi.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"

#define CACHE_FLASH_CTRL_REG 0x3ff0000C
#define CACHE_FLUSH_START_BIT BIT0
#define CACHE_EMPTY_FLAG_BIT BIT1
/******************************************************************************
 * FunctionName : cache_flush
 * Description  : clear all the cpu cache data for stability test.
 *******************************************************************************/
#if 0
void cache_flush (void)
{
	while(READ_PERI_REG (CACHE_FLASH_CTRL_REG) & CACHE_EMPTY_FLAG_BIT)
	{
		CLEAR_PERI_REG_MASK (CACHE_FLASH_CTRL_REG, CACHE_FLUSH_START_BIT);
		SET_PERI_REG_MASK (CACHE_FLASH_CTRL_REG, CACHE_FLUSH_START_BIT);
	}
	while(!(READ_PERI_REG (CACHE_FLASH_CTRL_REG) & CACHE_EMPTY_FLAG_BIT));

	CLEAR_PERI_REG_MASK (CACHE_FLASH_CTRL_REG, CACHE_FLUSH_START_BIT);
}
#endif

/******************************************************************************
 * FunctionName : spi_master_init
 * Description  : SPI master initial function for common byte units transmission
 * Parameters   : no
 *******************************************************************************/
void ICACHE_FLASH_ATTR spi_master_init (void)
{
	//	uint32 regvalue;
#define SPI_PREDIV_CLK		(0)	// ������������ SPI
#define SPI_DIV_CLK_N		(3)	// �������� SPI �� �������� ��c���� F = 80��� / (SPI_DIV_CLK_N + 1) = 20MHz
#define SPI_DIV_CLK_H		((SPI_DIV_CLK_N + 1) / 2 - 1)	// ��� ������� ���
#define SPI_DIV_CLK_L		(SPI_DIV_CLK_N)					// ��� ������� ���

	pin_func_select (PERIPHS_IO_MUX_MTDI_U, 2);	//configure io12 to hspi_miso mode
	pin_func_select (PERIPHS_IO_MUX_MTCK_U, 2);//configure io13 to Hspi_mosi mode
	pin_func_select (PERIPHS_IO_MUX_MTMS_U, 2);//configure io14 to Hspi_clk mode
	pin_func_select (PERIPHS_IO_MUX_MTDO_U, 2);//configure io15 to Hspi_cs mode

	SPI1->user |= SPI_USER_CS_SETUP | SPI_USER_CS_HOLD | SPI_USER_COMMAND;
	SPI1->user &= ~SPI_USER_FLASH_MODE;
	SPI1->clock_bits.equ_sysclk = 0;
	SPI1->clock_bits.pre_s = SPI_PREDIV_CLK;
	SPI1->clock_bits.clkcnt_n = SPI_DIV_CLK_N;
	SPI1->clock_bits.clkcnt_h = SPI_DIV_CLK_H;
	SPI1->clock_bits.clkcnt_l = SPI_DIV_CLK_L;

	/*	SET_PERI_REG_MASK (SPI_USER (HSPI),
	 SPI_CS_SETUP | SPI_CS_HOLD | SPI_USR_COMMAND);

	 CLEAR_PERI_REG_MASK (SPI_USER (HSPI), SPI_FLASH_MODE);
	 WRITE_PERI_REG (SPI_CLOCK (HSPI),
	 ((SPI_PREDIV_CLK & SPI_CLKDIV_PRE) << SPI_CLKDIV_PRE_S)
	 | ((SPI_DIV_CLK_N & SPI_CLKCNT_N) << SPI_CLKCNT_N_S)
	 | ((SPI_DIV_CLK_H & SPI_CLKCNT_H) << SPI_CLKCNT_H_S)
	 | ((SPI_DIV_CLK_L & SPI_CLKCNT_L) << SPI_CLKCNT_L_S));//clear bit 31, set SPI clock div
	 */
}

/*****************************************************************************
 * ������������� ���� GPIO4 ��� ������� �� ��������
 * ��������� ���������� �� ������ � ������� ����������
 ****************************************************************************/
void spi_req_intr_init(void)
{
	pin_func_select (PERIPHS_IO_MUX_GPIO4_U, 0);//configure io4 to GPIO mode - ���������� �� ������ - ������ �� ��������

}

/******************************************************************************
 * FunctionName : spi_lcd_9bit_write
 * Description  : SPI 9bits transmission function for driving LCD TM035PDZV36
 * Parameters   : uint8 high_bit - first high bit of the data, 0 is for "0", the other value 1-255 is for "1"
 *				uint8 low_8bit- the rest 8bits of the data.
 *******************************************************************************/
void ICACHE_FLASH_ATTR spi_lcd_9bit_write (uint8 high_bit, uint8 low_8bit)
{
	uint32 regvalue;
	uint8 bytetemp;

	if (high_bit) bytetemp = (low_8bit >> 1) | 0x80;
	else bytetemp = (low_8bit >> 1) & 0x7f;

	regvalue = ((8 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S) | ((uint32)bytetemp);//configure transmission variable, 9bit transmission length and first 8 command bit
	if (low_8bit & 0x01)
	{
		regvalue |= BIT15;		//write the 9th bit
	}
//	while (READ_PERI_REG (SPI_CMD (HSPI)) & SPI_USR);
	while (SPI1->cmd_bits.usr);	//waiting for spi module available
//	WRITE_PERI_REG (SPI_USER2 (HSPI), regvalue);
	SPI1->user2 = regvalue;		//write  command and command length into spi reg
//	SET_PERI_REG_MASK (SPI_CMD (HSPI), SPI_USR);
	SPI1->cmd_bits.usr = 1;	// transmission start
	//	while(READ_PERI_REG (SPI_CMD (HSPI)) & SPI_USR);
}

/******************************************************************************
 * FunctionName : spi_mast_byte_write
 * Description  : SPI master 1 byte transmission function
 * Parameters	: uint8 data- transmitted data
 *******************************************************************************/
void ICACHE_FLASH_ATTR spi_mast_byte_write (uint8 data)
{
	uint32 regvalue;

	while (SPI1->cmd_bits.usr);	//waiting for spi module available

//	CLEAR_PERI_REG_MASK (SPI_USER (HSPI), SPI_USR_MOSI | SPI_USR_MISO);	// ��������� ������ � ������
	SPI1->user &= ~(SPI_USER_MISO | SPI_USER_MOSI);
//	SET_PERI_REG_MASK (SPI_USER (HSPI), SPI_USR_COMMAND);
	SPI1->user_bits.command = 1;
	//SPI_FLASH_USER2 bit28-31 is cmd length, cmd bit length is value(0-15)+1,
	// bit15-0 is cmd value.
/*	WRITE_PERI_REG (SPI_USER2 (HSPI),
			((7 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S)
			| ((uint32) data));
*/
	SPI1->user2_bits.command_bitlen = 7;
	SPI1->user2_bits.command_value = data;
//	SET_PERI_REG_MASK (SPI_CMD(HSPI), SPI_USR);
	SPI1->cmd_bits.usr = 1;	// transmission start
//	while (READ_PERI_REG (SPI_CMD(HSPI)) & SPI_USR);

}

/******************************************************************************
 * FunctionName : spi_byte_write_espslave
 * Description  : SPI master 1 byte transmission function for esp8266 slave,
 * 	transmit 1byte data to esp8266 slave buffer needs 16bit transmission ,
 *	first byte is command 0x04 to write slave buffer, second byte is data
 * Parameters	 :	 uint8 1 - SPI module number, Only "SPI" and "HSPI" are valid
 * 			 uint8 data- transmitted data
 *******************************************************************************/
void ICACHE_FLASH_ATTR spi_byte_write_espslave (uint8 data)
{
	while (SPI1->cmd_bits.usr);	//waiting for spi module available
	SPI1->user |= SPI_USER_MOSI | SPI_USER_COMMAND;
	SPI1->user &= (SPI_USER_MISO | SPI_USER_ADDR | SPI_USER_DUMMY);
	SPI1->user2_bits.command_bitlen = 7;
	SPI1->user2_bits.command_value = 0x04;
	SPI1->user1_bits.mosi_bitlen = 7;
	SPI1->w0 = data;
	SPI1->cmd_bits.usr = 1;	// transmission start
	
//	SET_PERI_REG_MASK (SPI_USER(HSPI), SPI_USR_MOSI|SPI_USR_COMMAND);
//	CLEAR_PERI_REG_MASK (SPI_USER(HSPI), SPI_USR_MISO|SPI_USR_ADDR|SPI_USR_DUMMY);
	//SPI_FLASH_USER2 bit28-31 is cmd length, cmd bit length is value(0-15)+1,
	// bit15-0 is cmd value.
	//0x70000000 is for 8bits cmd, 0x04 is eps8266 slave write cmd value
//	WRITE_PERI_REG (SPI_USER2(HSPI), ((7 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S) | 4);
	//in register SPI_FLASH_USER1, bit 8-16 stores MOSI bit length value
	//The value shall be (bit_num-1).
	//for example, we WRITE 1 byte data which has length of 8 bits,
	//therefore the MOSI bit length value of 7 should be written into the related register bits.
//	WRITE_PERI_REG (SPI_USER1(HSPI), ((7 & SPI_USR_MOSI_BITLEN) << SPI_USR_MOSI_BITLEN_S));
//	WRITE_PERI_REG (SPI_W0(HSPI), (uint32)(data));
//	SET_PERI_REG_MASK (SPI_CMD(HSPI), SPI_USR);
//	while(READ_PERI_REG (SPI_CMD(HSPI)) & SPI_USR);
}
/******************************************************************************
 * FunctionName : spi_byte_read_espslave
 * Description  : SPI master 1 byte read function for esp8266 slave,
 * 		 read 1byte data from esp8266 slave buffer needs 16bit transmission ,
 * 		 first byte is command 0x06 to read slave buffer, second byte is recieved data
 * Parameters	 :	 uint8* data- recieved data address
 *******************************************************************************/
void ICACHE_FLASH_ATTR spi_byte_read_espslave (uint8 *data)
{
	uint32 regvalue;
	while (SPI1->cmd_bits.usr);	//waiting for spi module available

//	SET_PERI_REG_MASK (SPI_USER (HSPI), SPI_USR_MISO|SPI_USR_COMMAND);
	SPI1->user |= SPI_USER_MISO | SPI_USER_COMMAND;
	SPI1->user &= ~(SPI_USER_MOSI | SPI_USER_ADDR | SPI_USER_DUMMY);
//	CLEAR_PERI_REG_MASK (SPI_USER (HSPI), SPI_USR_MOSI|SPI_USR_ADDR|SPI_USR_DUMMY);
	//SPI_FLASH_USER2 bit28-31 is cmd length, cmd bit length is value(0-15)+1,
	// bit15-0 is cmd value.
	//0x70000000 is for 8bits cmd, 0x06 is eps8266 slave read cmd value
//	WRITE_PERI_REG (SPI_USER2 (HSPI), ((7 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S) | 6);
	SPI1->user2_bits.command_bitlen = 7;
	SPI1->user2_bits.command_value = 0x06;
	//in register SPI_FLASH_USER1, bit 8-16 stores MOSI bit length value
	//The value shall be (bit_num-1).
	//for example, we READ 1 byte data which has length of 8 bits,
	//therefore the MISO bit length value of 7 should be written into the related register bits.
//	WRITE_PERI_REG (SPI_USER1 (HSPI), ((7 & SPI_USR_MISO_BITLEN) << SPI_USR_MISO_BITLEN_S));
	SPI1->user1_bits.miso_bitlen = 7;
	
//	SET_PERI_REG_MASK (SPI_CMD (HSPI), SPI_USR);
	SPI1->cmd_bits.usr = 1;	// transmission start
//	while(READ_PERI_REG (SPI_CMD (HSPI)) & SPI_USR);
	while (SPI1->cmd_bits.usr);	//waiting for spi module available
//	*data = (uint8)(READ_PERI_REG (SPI_W0 (HSPI)) & 0xff);
	*data = SPI1->w0  & 0xFF;
}

/******************************************************************************
 * FunctionName :	   spi_WR_espslave(uint8 1)
 * Description  : SPI master byte units write and read
 * Parameters	 :
 *******************************************************************************/

void ICACHE_FLASH_ATTR spi_WR_espslave (void)
{
	uint32 regvalue;

	while(READ_PERI_REG (SPI_CMD(HSPI)) & SPI_USR);

	SET_PERI_REG_MASK (SPI_USER(HSPI), SPI_USR_MOSI | SPI_USR_DOUTDIN);

	CLEAR_PERI_REG_MASK (SPI_USER(HSPI), SPI_USR_COMMAND | SPI_USR_ADDR | SPI_USR_DUMMY);

	//in register SPI_FLASH_USER1, bit 17-25 stores MOSI bit length value
	//The value shall be (bit_num-1).
	//for example, we output 64-byte data which has length of 512 bits,
	//therefore the MOSI bit length value of 511(0x1FF) should be written into the related register bits.
	WRITE_PERI_REG (SPI_USER1(HSPI), ((0x1ff & SPI_USR_MOSI_BITLEN) << SPI_USR_MOSI_BITLEN_S));

	SET_PERI_REG_MASK (SPI_CMD(HSPI), SPI_USR);

	while(READ_PERI_REG (SPI_CMD(HSPI)) & SPI_USR);

}

void ICACHE_FLASH_ATTR set_data (void)	// ���������� ������ SPI (64 �����)
{
	WRITE_PERI_REG (SPI_W0(HSPI), 0x05040302);
	WRITE_PERI_REG (SPI_W1(HSPI), 0x09080706);
	WRITE_PERI_REG (SPI_W2(HSPI), 0x0d0c0b0a);
	WRITE_PERI_REG (SPI_W3(HSPI), 0x11100f0e);
	WRITE_PERI_REG (SPI_W4(HSPI), 0x15141312);
	WRITE_PERI_REG (SPI_W5(HSPI), 0x19181716);
	WRITE_PERI_REG (SPI_W6(HSPI), 0x1d1c1b1a);
	WRITE_PERI_REG (SPI_W7(HSPI), 0x21201f1e);
	WRITE_PERI_REG (SPI_W8(HSPI), 0x05040302);
	WRITE_PERI_REG (SPI_W9(HSPI), 0x09080706);
	WRITE_PERI_REG (SPI_W10(HSPI), 0x0d0c0b0a);
	WRITE_PERI_REG (SPI_W11(HSPI), 0x11100f0e);
	WRITE_PERI_REG (SPI_W12(HSPI), 0x15141312);
	WRITE_PERI_REG (SPI_W13(HSPI), 0x19181716);
	WRITE_PERI_REG (SPI_W14(HSPI), 0x1d1c1b1a);
	WRITE_PERI_REG (SPI_W15(HSPI), 0x21201f1e);
}

//-----------------------------------------------------------------------------------------------------------------------------
#ifdef SPI_SLAVE_DEBUG
/******************************************************************************
 * FunctionName : spi_slave_init
 * Description  : SPI slave mode initial funtion, including mode setting,
 * 			IO setting, transmission interrupt opening, interrupt function registration
 * Parameters   : 	uint8 1 - SPI module number, Only "SPI" and "HSPI" are valid
 *******************************************************************************/
void spi_slave_init (void)
{
	uint32 regvalue;

	//clear bit9, bit8 of reg PERIPHS_IO_MUX
	//bit9 should be cleared when HSPI clock doesn't equal CPU clock
	//bit8 should be cleared when SPI clock doesn't equal CPU clock
	////WRITE_PERI_REG (PERIPHS_IO_MUX, 0x105); //clear bit9//TEST
	pin_func_select (PERIPHS_IO_MUX_MTDI_U, 2);//configure io to Hspi mode
	pin_func_select (PERIPHS_IO_MUX_MTCK_U, 2);//configure io to Hspi mode
	pin_func_select (PERIPHS_IO_MUX_MTMS_U, 2);//configure io to Hspi mode
	pin_func_select (PERIPHS_IO_MUX_MTDO_U, 2);//configure io to Hspi mode

	//regvalue = READ_PERI_REG (SPI_FLASH_SLAVE(HSPI));
	//slave mode, slave use buffers which are register "SPI_FLASH_C0~C15", enable trans done isr
	//set bit 30 bit 29 bit9, bit9 is trans done isr mask
	SET_PERI_REG_MASK ( SPI_SLAVE(HSPI),
			SPI_SLAVE_MODE|SPI_SLV_WR_RD_BUF_EN|
			SPI_SLV_WR_BUF_DONE_EN|SPI_SLV_RD_BUF_DONE_EN|
			SPI_SLV_WR_STA_DONE_EN|SPI_SLV_RD_STA_DONE_EN|
			SPI_TRANS_DONE_EN);
	//disable general trans intr 
	//CLEAR_PERI_REG_MASK (SPI_SLAVE(HSPI), SPI_TRANS_DONE_EN);

	CLEAR_PERI_REG_MASK (SPI_USER(HSPI), SPI_FLASH_MODE);//disable flash operation mode
	SET_PERI_REG_MASK (SPI_USER(HSPI), SPI_USR_MISO_HIGHPART);//SLAVE SEND DATA BUFFER IN C8-C15

	//////**************RUN WHEN SLAVE RECIEVE*******************///////
	//tow lines below is to configure spi timing.
	SET_PERI_REG_MASK (SPI_CTRL2(HSPI), (0x2 & SPI_MOSI_DELAY_NUM) << SPI_MOSI_DELAY_NUM_S);//delay num
	os_printf("SPI_CTRL2 is %08x\n", READ_PERI_REG (SPI_CTRL2(HSPI)));
	WRITE_PERI_REG (SPI_CLOCK(HSPI), 0);

	/////***************************************************//////

	//set 8 bit slave command length, because slave must have at least one bit addr, 
	//8 bit slave+8bit addr, so master device first 2 bytes can be regarded as a command 
	//and the  following bytes are datas, 
	//32 bytes input wil be stored in SPI_FLASH_C0-C7
	//32 bytes output data should be set to SPI_FLASH_C8-C15
	WRITE_PERI_REG (SPI_USER2(HSPI), (0x7 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S);//0x70000000

	//set 8 bit slave recieve buffer length, the buffer is SPI_FLASH_C0-C7
	//set 8 bit slave status register, which is the low 8 bit of register "SPI_FLASH_STATUS"
	SET_PERI_REG_MASK (SPI_SLAVE1(HSPI), ((0xff & SPI_SLV_BUF_BITLEN) << SPI_SLV_BUF_BITLEN_S)|
			((0x7 & SPI_SLV_STATUS_BITLEN) << SPI_SLV_STATUS_BITLEN_S)|
			((0x7 & SPI_SLV_WR_ADDR_BITLEN) << SPI_SLV_WR_ADDR_BITLEN_S)|
			((0x7 & SPI_SLV_RD_ADDR_BITLEN) << SPI_SLV_RD_ADDR_BITLEN_S));
	CLEAR_PERI_REG_MASK (SPI_SLAVE1(HSPI), BIT25);//CHOOSE ACTIVE STATUS REG
	SET_PERI_REG_MASK (SPI_PIN (HSPI), BIT19);//BIT19

	//maybe enable slave transmission liston 
	SET_PERI_REG_MASK (SPI_CMD (HSPI), SPI_USR);
	//register level2 isr function, which contains spi, hspi and i2s events
#if TWO_INTR_LINE_PROTOCOL
	ETS_SPI_INTR_ATTACH (spi_slave_isr_handler, NULL);
#elif ONE_INTR_LINE_WITH_STATUS
	ETS_SPI_INTR_ATTACH (spi_slave_isr_sta, NULL);
#endif
	//enable level2 isr, which contains spi, hspi and i2s events
	ETS_SPI_INTR_ENABLE();
}

/******************************************************************************
 * FunctionName : spi_slave_isr_handler
 * Description  : SPI interrupt function, SPI HSPI and I2S interrupt can trig this function
 some basic operation like clear isr flag has been done, 
 and it is availible	for adding user coder in the funtion
 * Parameters  : void *para- function parameter address, which has been registered in function spi_slave_init
 *******************************************************************************/

static uint8 spi_data[32] =
{	0};
static uint8 idx = 0;
//static uint8 spi_flg = 0;

#define SPI_MISO
#define SPI_QUEUE_LEN 8
os_event_t * spiQueue;
#define MOSI  0
#define MISO  1
#define STATUS_R_IN_WR 2
#define STATUS_W  3
#define TR_DONE_ALONE  4
#define WR_RD 5
#define DATA_ERROR 6
#define STATUS_R_IN_RD 7
//init the two intr line of slave
//gpio0: wr_ready , and
//gpio2: rd_ready , controlled by slave
void ICACHE_FLASH_ATTR gpio_init (void)
{
	pin_func_select ((PERIPHS_IO_MUX_GPIO0_U), FUNC_GPIO0);

#if	TWO_INTR_LINE_PROTOCOL

	pin_func_select (PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
	pin_func_select (PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	GPIO_OUTPUT_SET (0, 1);
	GPIO_OUTPUT_SET (2, 0);
	GPIO_OUTPUT_SET (4, 1);

#endif	// #if	TWO_INTR_LINE_PROTOCOL
}

void spi_slave_isr_handler (void *para)
{
	uint32 regvalue, calvalue;
	static uint8 state = 0;
	uint32 recv_data, send_data;

	if (READ_PERI_REG (0x3ff00020) & BIT4)
	{
		//following 3 lines is to clear isr signal
		CLEAR_PERI_REG_MASK (SPI_SLAVE(SPI), 0x3ff);
	}
	else if(READ_PERI_REG (0x3ff00020) & BIT7)
	{ //bit7 is for hspi isr,
		regvalue = READ_PERI_REG (SPI_SLAVE(HSPI));
		CLEAR_PERI_REG_MASK (SPI_SLAVE(HSPI),
				SPI_TRANS_DONE_EN
				| SPI_SLV_WR_STA_DONE_EN
				| SPI_SLV_RD_STA_DONE_EN
				| SPI_SLV_WR_BUF_DONE_EN
				| SPI_SLV_RD_BUF_DONE_EN);
		SET_PERI_REG_MASK (SPI_SLAVE(HSPI), SPI_SYNC_RESET);
		CLEAR_PERI_REG_MASK (SPI_SLAVE(HSPI),
				SPI_TRANS_DONE
				| SPI_SLV_WR_STA_DONE
				| SPI_SLV_RD_STA_DONE
				| SPI_SLV_WR_BUF_DONE
				| SPI_SLV_RD_BUF_DONE);
		SET_PERI_REG_MASK (SPI_SLAVE(HSPI),
				SPI_TRANS_DONE_EN
				| SPI_SLV_WR_STA_DONE_EN
				| SPI_SLV_RD_STA_DONE_EN
				| SPI_SLV_WR_BUF_DONE_EN
				| SPI_SLV_RD_BUF_DONE_EN);

		if ((regvalue & SPI_SLV_WR_BUF_DONE) && (regvalue & SPI_SLV_RD_BUF_DONE))
		{
			system_os_post (USER_TASK_PRIO_1, WR_RD, regvalue);
		}

		else if (regvalue & SPI_SLV_WR_BUF_DONE)
		{
			GPIO_OUTPUT_SET (0, 0);
			state = 0;
			idx = 0;
			while (idx < 8)
			{
				recv_data = READ_PERI_REG (SPI_W0(HSPI)+(idx << 2));
				spi_data[idx << 2] = recv_data & 0xff;
				spi_data[(idx << 2) + 1] = (recv_data >> 8) & 0xff;
				spi_data[(idx << 2) + 2] = (recv_data >> 16) & 0xff;
				spi_data[(idx << 2) + 3] = (recv_data >> 24) & 0xff;
				idx++;
			}

			for (idx = 0; idx < 8; idx++)
			{
				send_data = READ_PERI_REG (SPI_W8(HSPI)+(idx << 2));
				calvalue = send_data & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[idx << 2] != calvalue)
				{
					GPIO_OUTPUT_SET(4, 0);
					system_os_post(USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data>>8) & 0xff;
				calvalue = (calvalue*calvalue*calvalue)%255;
				if (spi_data[(idx << 2) + 1] != calvalue)
				{
					GPIO_OUTPUT_SET(4, 0);
					system_os_post(USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data>>16) & 0xff;
				calvalue = (calvalue*calvalue*calvalue)%255;
				if (spi_data[(idx << 2) + 2] != calvalue)
				{
					GPIO_OUTPUT_SET(4, 0);
					system_os_post(USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data>>24) & 0xff;
				calvalue = (calvalue*calvalue*calvalue)%255;
				if (spi_data[(idx << 2) + 3] != calvalue)
				{
					GPIO_OUTPUT_SET (4, 0);
					system_os_post (USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
			}

			for(idx = 0;idx<8;idx++)
			{
				WRITE_PERI_REG (SPI_W8(HSPI)+(idx << 2), READ_PERI_REG (SPI_W0(HSPI)+(idx << 2)));
			}
			GPIO_OUTPUT_SET(2, 1);
			GPIO_OUTPUT_SET(0, 1);
			GPIO_OUTPUT_SET(4, 1);
			SET_PERI_REG_MASK (SPI_SLAVE(HSPI), SPI_SLV_WR_BUF_DONE_EN);

		}

		else if(regvalue & SPI_SLV_RD_BUF_DONE)
		{
			GPIO_OUTPUT_SET(2, 0);
			state = 1;
		}

		if(regvalue & SPI_SLV_RD_STA_DONE)
		{
			GPIO_OUTPUT_SET(4, 0);
			if(state)
			{
				system_os_post(USER_TASK_PRIO_1, STATUS_R_IN_WR, regvalue);
				state = 0;
			}
			else
			{
				system_os_post(USER_TASK_PRIO_1, STATUS_R_IN_RD, regvalue);
				state = 1;
			}
			GPIO_OUTPUT_SET(4, 1);
		}

		if(regvalue & SPI_SLV_WR_STA_DONE)
		{
			GPIO_OUTPUT_SET(4, 0);
			system_os_post(USER_TASK_PRIO_1, STATUS_W, regvalue);
			GPIO_OUTPUT_SET(4, 1);
		}

		if ((regvalue & SPI_TRANS_DONE) && ((regvalue & 0xf) == 0))
		{
			//		GPIO_OUTPUT_SET(4, 0);
			system_os_post(USER_TASK_PRIO_1, TR_DONE_ALONE, regvalue);
			//		GPIO_OUTPUT_SET(4, 1);	
		}

	}

	else if(READ_PERI_REG (0x3ff00020) & BIT9)
	{ //bit7 is for i2s isr,

	}
}

void spi_slave_isr_sta(void *para)
{
	uint32 regvalue, calvalue;
	static uint8 state = 0;
	uint32 recv_data, send_data;

	union spi_slave_status spi_sta;

	if(READ_PERI_REG (0x3ff00020) & BIT4)
	{
		//following 3 lines is to clear isr signal
		CLEAR_PERI_REG_MASK (SPI_SLAVE(SPI), 0x3ff);
	}
	else if(READ_PERI_REG (0x3ff00020) & BIT7)
	{ //bit7 is for hspi isr,
		regvalue = READ_PERI_REG (SPI_SLAVE(HSPI));
		CLEAR_PERI_REG_MASK (SPI_SLAVE(HSPI),
				SPI_TRANS_DONE_EN
				| SPI_SLV_WR_STA_DONE_EN
				| SPI_SLV_RD_STA_DONE_EN
				| SPI_SLV_WR_BUF_DONE_EN
				| SPI_SLV_RD_BUF_DONE_EN);
		SET_PERI_REG_MASK (SPI_SLAVE(HSPI), SPI_SYNC_RESET);
		CLEAR_PERI_REG_MASK (SPI_SLAVE(HSPI),
				SPI_TRANS_DONE
				| SPI_SLV_WR_STA_DONE
				| SPI_SLV_RD_STA_DONE
				| SPI_SLV_WR_BUF_DONE
				| SPI_SLV_RD_BUF_DONE);
		SET_PERI_REG_MASK (SPI_SLAVE(HSPI),
				SPI_TRANS_DONE_EN
				| SPI_SLV_WR_STA_DONE_EN
				| SPI_SLV_RD_STA_DONE_EN
				| SPI_SLV_WR_BUF_DONE_EN
				| SPI_SLV_RD_BUF_DONE_EN);

		if (regvalue & SPI_SLV_WR_BUF_DONE)
		{
			spi_sta.byte_value = READ_PERI_REG (SPI_RD_STATUS(HSPI)) & 0xff;
			spi_sta.elm_value.wr_busy = 1;
			spi_sta.elm_value.comm_cnt++;
			WRITE_PERI_REG (SPI_RD_STATUS(HSPI), (uint32)spi_sta.byte_value);

			state = 0;
			idx = 0;
			while (idx < 8)
			{
				recv_data = READ_PERI_REG (SPI_W0(HSPI)+(idx << 2));
				spi_data[idx << 2] = recv_data & 0xff;
				spi_data[(idx << 2) + 1] = (recv_data >> 8) & 0xff;
				spi_data[(idx << 2) + 2] = (recv_data >> 16) & 0xff;
				spi_data[(idx << 2) + 3] = (recv_data >> 24) & 0xff;
				idx++;
			}

			for(idx = 0; idx < 8; idx++)
			{
				send_data = READ_PERI_REG (SPI_W8(HSPI) + (idx << 2));
				calvalue = send_data & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[idx << 2] != calvalue)
				{
					GPIO_OUTPUT_SET(4, 0);
					system_os_post(USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data >> 8) & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[(idx << 2) + 1] != calvalue)
				{
					GPIO_OUTPUT_SET(4, 0);
					system_os_post(USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data>>16) & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if(spi_data[(idx << 2) + 2] != calvalue)
				{
					GPIO_OUTPUT_SET (4, 0);
					system_os_post (USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data >> 24) & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if(spi_data[(idx << 2) + 3] != calvalue)
				{
					GPIO_OUTPUT_SET(4, 0);
					system_os_post (USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
			}
			spi_sta.byte_value = READ_PERI_REG (SPI_RD_STATUS(HSPI)) & 0xff;
			spi_sta.elm_value.wr_busy = 0;
			WRITE_PERI_REG (SPI_RD_STATUS(HSPI), (uint32)spi_sta.byte_value);

			for(idx = 0; idx < 8; idx++)
			{
				WRITE_PERI_REG (SPI_W8(HSPI)+(idx << 2), READ_PERI_REG (SPI_W0(HSPI)+(idx << 2)));
			}
			spi_sta.byte_value = READ_PERI_REG (SPI_RD_STATUS(HSPI)) & 0xff;
			spi_sta.elm_value.rd_empty = 0;
			WRITE_PERI_REG (SPI_RD_STATUS(HSPI), (uint32)spi_sta.byte_value);
			GPIO_OUTPUT_SET(0, 1);

		}
		else if (regvalue & SPI_SLV_RD_BUF_DONE)
		{
			spi_sta.byte_value = READ_PERI_REG (SPI_RD_STATUS(HSPI)) & 0xff;
			spi_sta.elm_value.comm_cnt++;
			spi_sta.elm_value.rd_empty = 1;
			WRITE_PERI_REG (SPI_RD_STATUS(HSPI), (uint32)spi_sta.byte_value);
			GPIO_OUTPUT_SET(0, 1);
			state = 1;
		}

		if (regvalue & SPI_SLV_RD_STA_DONE)
		{
			GPIO_OUTPUT_SET(0, 0);
		}

		if (regvalue & SPI_SLV_WR_STA_DONE)
		{

		}

		if ((regvalue & SPI_TRANS_DONE) && ((regvalue & 0xf) == 0))
		{

		}

	}

	else if(READ_PERI_REG (0x3ff00020) & BIT9)
	{ //bit7 is for i2s isr,

	}
}

void ICACHE_FLASH_ATTR set_miso_data (void)
{
	if (!(gpio_input_get () & BIT2))	// ���� GPIO2 = =  0
	//if (GPIO_INPUT_GET (2) = =  0)
	{
		WRITE_PERI_REG (SPI_W8 (HSPI), 0x05040302);
		WRITE_PERI_REG (SPI_W9 (HSPI), 0x09080706);
		WRITE_PERI_REG (SPI_W10 (HSPI), 0x0d0c0b0a);
		WRITE_PERI_REG (SPI_W11 (HSPI), 0x11100f0e);

		WRITE_PERI_REG (SPI_W12 (HSPI), 0x15141312);
		WRITE_PERI_REG (SPI_W13 (HSPI), 0x19181716);
		WRITE_PERI_REG (SPI_W14 (HSPI), 0x1d1c1b1a);
		WRITE_PERI_REG (SPI_W15 (HSPI), 0x21201f1e);
		GPIO_OUTPUT_SET (2, 1);
	}
}

void ICACHE_FLASH_ATTR disp_spi_data (void)
{
	uint8 i = 0;
	for(i = 0;i<32;i++)
	{
		os_printf ("data %d : 0x%02x\n\r", i, spi_data[i]);
	}
}

void ICACHE_FLASH_ATTR spi_task (os_event_t *e)
{
	uint8 data;
	switch(e->sig)
	{
		case MOSI:
		disp_spi_data();
		break;
		case STATUS_R_IN_WR :
		os_printf("SR ERR in WRPR, Reg:%08x \n", e->par);
		break;
		case STATUS_W:
		os_printf("SW ERR, Reg:%08x\n", e->par);
		break;
		case TR_DONE_ALONE:
		os_printf("TD ALO ERR, Reg:%08x\n", e->par);
		break;
		case WR_RD:
		os_printf("WR&RD ERR, Reg:%08x\n", e->par);
		break;
		case DATA_ERROR:
		os_printf("Data ERR, Reg:%08x\n", e->par);
		break;
		case STATUS_R_IN_RD :
		os_printf("SR ERR in RDPR, Reg:%08x\n", e->par);
		break;
		default:
		break;
	}
}

void ICACHE_FLASH_ATTR spi_task_init (void)
{
	spiQueue = (os_event_t*)os_malloc(sizeof(os_event_t)*SPI_QUEUE_LEN);
	system_os_task(spi_task, USER_TASK_PRIO_1, spiQueue, SPI_QUEUE_LEN);
}

os_timer_t spi_timer_test;

void ICACHE_FLASH_ATTR spi_test_init (void)
{
	os_printf("spi init\n\r");
	spi_slave_init ();
	os_printf("gpio init\n\r");
	gpio_init();
	os_printf("spi task init \n\r");
	spi_task_init();
#ifdef SPI_MISO
	os_printf("spi miso init\n\r");
	set_miso_data();
#endif

}
#endif


/*******************************************************************************
* Примеры работы с прерываниями из форума
* In the master mode, it is the divider of spi_clk. So spi_clk frequency is 80MHz/(spi_clkdiv_pre+1)/(spi_clkcnt_N+1)
* In the master mode, SPI_CLKCNT_H must be floor((spi_clkcnt_N+1)/2-1)
* In the master mode, SPI_CLKCNT_L must be eqaul to spi_clkcnt_N
* 
******************************************************************************/

#define SPI_CLOCK_10MHZ (((0 & SPI_CLKDIV_PRE) << SPI_CLKDIV_PRE_S) |\
        ((7 & SPI_CLKCNT_N) << SPI_CLKCNT_N_S) |\
        ((3 & SPI_CLKCNT_H) << SPI_CLKCNT_H_S) |\
        ((7 & SPI_CLKCNT_L) << SPI_CLKCNT_L_S))
#define SPI_CLOCK_500KHZ (((2 & SPI_CLKDIV_PRE) << SPI_CLKDIV_PRE_S) |\
        ((63 & SPI_CLKCNT_N) << SPI_CLKCNT_N_S) |\
        ((31 & SPI_CLKCNT_H) << SPI_CLKCNT_H_S) |\
        ((63 & SPI_CLKCNT_L) << SPI_CLKCNT_L_S))
#define SET_SPI_CLOCK_FAST() WRITE_PERI_REG(SPI_CLOCK(HSPI),SPI_CLOCK_10MHZ)
#define SET_SPI_CLOCK_SLOW() WRITE_PERI_REG(SPI_CLOCK(HSPI),SPI_CLOCK_500KHZ)
 
 
 
/*******************************************************************************
* передает команду, потом читает read_length
* только запуск операции, прием данных в обработчике прерываний
*******************************************************************************/
static volatile uint8_t mem1=0; // проверка влияния обращений к регистрам на скорость
void hspi_cr_start(const uint8_t cmd, const uint8_t read_length)
{
    uint32_t regvalue;
    uint16_t numberBit;
 
    if (mem1 ==0)
    {
        regvalue =  SPI_USR_COMMAND  /*| SPI_CS_HOLD */ ;
        if (0 != read_length)
        {
            regvalue |= SPI_USR_MISO;
            numberBit = read_length * 8 -1;
        }
        else
            numberBit = 7;
 
        // отключаем фазы обмена address, dummy, data_out
        //regvalue &= ~(BIT2 | SPI_USR_ADDR | SPI_USR_DUMMY | SPI_USR_MOSI /*| SPI_USR_MISO | SPI_USR_COMMAND*/); //clear bit 2 see example IoT_Demo
        WRITE_PERI_REG(SPI_USER(HSPI), regvalue);
 
        //set output buffer length, the buffer is the register"SPI_FLASH_C0"
        WRITE_PERI_REG(SPI_USER1(HSPI),
                ((7 & SPI_USR_MOSI_BITLEN)<<SPI_USR_MOSI_BITLEN_S)|
                ((numberBit & SPI_USR_MISO_BITLEN)<<SPI_USR_MISO_BITLEN_S));
 
        //SPI_FLASH_USER2 bit28-31 is cmd length,cmd bit length is value(0-15)+1,
        // bit15-0 is cmd value.
        //0x70000000 is for 8bits cmd
        WRITE_PERI_REG(SPI_USER2(HSPI),
                ((7 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S) |
                ((cmd & SPI_USR_COMMAND_VALUE) << SPI_USR_COMMAND_VALUE_S) );
        mem1=1;
    }
    SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);   // send
}
 
void  __attribute__((optimize("O2"))) hspi_int_handler(void* *para)
{
    enum spi_rx_state temp_rx_state;  // локальная копия volatile переменной
    uint32_t regvalue;
 
    if(READ_PERI_REG(0x3ff00020) & BIT4)
    {
        //following 3 lines is to clear isr signal
        CLEAR_PERI_REG_MASK(SPI_SLAVE(SPI), 0x3ff);
//        ++spicount;   // проверка работы прерываний
    }
    else if (READ_PERI_REG(0x3ff00020) & BIT7)
	{ //bit7 is for hspi isr,

		// обнуляем все запросы прерываний spi (хотя использется только SPI_TRANS_DONE)
		CLEAR_PERI_REG_MASK(SPI_SLAVE(HSPI),
				SPI_TRANS_DONE | SPI_SLV_WR_STA_DONE |
				SPI_SLV_RD_STA_DONE | SPI_SLV_WR_BUF_DONE |
				SPI_SLV_RD_BUF_DONE);

// TODO:		hspi_cr_start (INSTRUCTION_READ_STATUS, 62);
//		mcp251x.mcp_rx_state = RXSTATE_FAST_POLLING ;
	}
}
 
void ICACHE_FLASH_ATTR hspi_init()
{
    uint32 regvalue;
 
    // gpio mux
    WRITE_PERI_REG (PERIPHS_IO_MUX, 0x105);
    pin_func_select (PERIPHS_IO_MUX_MTDI_U, 2); // HSPIQ MISO
    pin_func_select (PERIPHS_IO_MUX_MTCK_U, 2); // HSPID MOSI
    pin_func_select (PERIPHS_IO_MUX_MTMS_U, 2); // CLK
    pin_func_select (PERIPHS_IO_MUX_MTDO_U, 2); // CS
 
#if 0
    WRITE_PERI_REG(SPI_CLOCK(HSPI),
            ((1 & SPI_CLKDIV_PRE) << SPI_CLKDIV_PRE_S) |
            ((3 & SPI_CLKCNT_N) << SPI_CLKCNT_N_S) |  /*In the master mode, it is the divider of spi_clk. So spi_clk frequency is 80MHz/(spi_clkdiv_pre+1)/(spi_clkcnt_N+1)*/
            ((1 & SPI_CLKCNT_H) << SPI_CLKCNT_H_S) |  /*In the master mode, it must be floor((spi_clkcnt_N+1)/2-1)*/
            ((3 & SPI_CLKCNT_L) << SPI_CLKCNT_L_S));  /*In the master mode, it must be eqaul to spi_clkcnt_N*/
#else
    SET_SPI_CLOCK_FAST();
#endif
 
    // Disable the interrupt
    CLEAR_PERI_REG_MASK(SPI_SLAVE(HSPI),
            SPI_TRANS_DONE_EN | SPI_SLV_WR_STA_DONE_EN |
            SPI_SLV_RD_STA_DONE_EN | SPI_SLV_WR_BUF_DONE_EN |
            SPI_SLV_RD_BUF_DONE_EN     );
    //register level2 isr function, which contains spi, hspi and i2s events
    ETS_SPI_INTR_ATTACH(hspi_int_handler, NULL);
    //enable level2 isr, which contains spi, hspi and i2s events
    ETS_SPI_INTR_ENABLE();
}
 

/******************************************************************************/
/* Из примера ILI9341 */

#define SPIFIFOSIZE 16 //16 words length

extern uint32_t *spi_fifo;

extern void hspi_init(void);
extern void hspi_send_data(const uint8_t * data, uint8_t datasize);
extern void hspi_send_uint16_r(const uint16_t data, int32_t repeats);
inline void hspi_wait_ready(void){while (READ_PERI_REG(SPI_FLASH_CMD(HSPI))&SPI_FLASH_USR);}

inline void hspi_prepare_tx(uint32_t bytecount)
{
	uint32_t bitcount = bytecount * 8 - 1;

	WRITE_PERI_REG(SPI_FLASH_USER1(HSPI), (bitcount & SPI_USR_OUT_BITLEN) << SPI_USR_OUT_BITLEN_S);
}


inline void hspi_start_tx()
{
	SET_PERI_REG_MASK(SPI_FLASH_CMD(HSPI), SPI_FLASH_USR);   // send
}

inline void hspi_send_uint8(uint8_t data)
{
	hspi_prepare_tx(1);
	*spi_fifo = data;
	hspi_start_tx();
}

inline void hspi_send_uint16(uint16_t data)
{
	hspi_prepare_tx(2);
	*spi_fifo = data;
	hspi_start_tx();
}

inline void hspi_send_uint32(uint32_t data)
{
	hspi_prepare_tx(4);
	*spi_fifo = data;
	hspi_start_tx();
}


#include "hspi.h"

/*
Pinout:
MISO GPIO12
MOSI GPIO13
CLK GPIO14
CS GPIO15
DC GPIO2
*/

#define HSPI_PRESCALER 4// target hspi clock speed is 40MHz/HSPI_PRESCALER, so that with prescaler 2 the hspi clock is 30MHz

#define __min(a,b) ((a > b) ? (b):(a))
uint32_t *spi_fifo;

void hspi_init(void)
{
	spi_fifo = (uint32_t*)SPI_FLASH_C0(HSPI);

	WRITE_PERI_REG(PERIPHS_IO_MUX, 0x105); //clear bit9

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, 2); // HSPIQ MISO GPIO12
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, 2); // HSPID MOSI GPIO13
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, 2); // CLK GPIO14
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, 2); // CS GPIO15


	// SPI clock = CPU clock / 10 / 4
	// time length HIGHT level = (CPU clock / 10 / 2) ^ -1,
	// time length LOW level = (CPU clock / 10 / 2) ^ -1
	WRITE_PERI_REG(SPI_FLASH_CLOCK(HSPI),
	   (((HSPI_PRESCALER - 1) & SPI_CLKDIV_PRE) << SPI_CLKDIV_PRE_S) |
	   ((1 & SPI_CLKCNT_N) << SPI_CLKCNT_N_S) |
	   ((0 & SPI_CLKCNT_H) << SPI_CLKCNT_H_S) |
	   ((1 & SPI_CLKCNT_L) << SPI_CLKCNT_L_S));

	WRITE_PERI_REG(SPI_FLASH_CTRL1(HSPI), 0);

	uint32_t regvalue = SPI_FLASH_DOUT;
    regvalue &= ~(BIT2 | SPI_FLASH_USR_ADDR | SPI_FLASH_USR_DUMMY | SPI_FLASH_USR_DIN | SPI_USR_COMMAND | SPI_DOUTDIN); //clear bit 2 see example IoT_Demo
	WRITE_PERI_REG(SPI_FLASH_USER(HSPI), regvalue);
}

void hspi_send_uint16_r(uint16_t data, int32_t repeats)
{
	uint32_t i;
	uint32_t word = data << 16 | data;

	while(repeats > 0)
	{
		uint16_t bytes_to_transfer = __min(repeats * sizeof(uint16_t) , SPIFIFOSIZE * sizeof(uint32_t));
		hspi_wait_ready();
		hspi_prepare_tx(bytes_to_transfer);
		for(i = 0; i < (bytes_to_transfer + 3) / 4;i++)
			spi_fifo[i] = word;
		hspi_start_tx();
		repeats -= bytes_to_transfer / 2;
	}
}

void hspi_send_data(const uint8_t * data, uint8_t datasize)
{
	uint32_t *_data = (uint32_t*)data;
	uint8_t i;

	uint8_t words_to_send = __min((datasize + 3) / 4, SPIFIFOSIZE);
	hspi_prepare_tx(datasize);
	for(i = 0; i < words_to_send;i++)
		spi_fifo[i] = _data[i];
	hspi_start_tx();
}
