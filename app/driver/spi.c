#include 	"c_types.h"
#include 	"driver/spi.h"
#include 	"driver/gpio.h"
#include 	"driver/dport_registers.h"
#include 	"driver/iomux_registers.h"
#include	"driver/dport_registers.h"
#include 	"user_interface.h"
#include 	"mem.h"
#include	"eagle_soc.h"

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
	while (DPORT->reg_1_bits.cashe_empty)
	{
		DPORT->reg_1_bits.cashe_flush = 0;
		DPORT->reg_1_bits.cashe_flush = 1;
	}
	while(!DPORT->reg_1_bits.cashe_empty);

	DPORT->reg_1_bits.cashe_flush = 0;
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
#define SPI_PREDIV1_CLK		(0)	// ������������ SPI
#define SPI_DIV1_CLK_N		(3)	// �������� SPI �� �������� ��c���� F = 80��� / (SPI_DIV_CLK_N + 1) = 20MHz
#define SPI_DIV1_CLK_H		((SPI_DIV_CLK_N + 1) / 2 - 1)	// ��� ������� ���
#define SPI_DIV1_CLK_L		(SPI_DIV_CLK_N)					// ��� ������� ���

//	pin_func_select (PERIPHS_IO_MUX_MTDI_U, GPIO_MUX_FUNC_2);
	IOMUX->gpio12_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio12_mux |= GPIO12_FUNC_SPI1_Q_MISO;//configure io12 to hspi_miso mode
//	pin_func_select (PERIPHS_IO_MUX_MTCK_U, 2);
	IOMUX->gpio13_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio13_mux |= GPIO13_FUNC_SPI1_D_MOSI;//configure io13 to Hspi_mosi mode
//	pin_func_select (PERIPHS_IO_MUX_MTMS_U, 2);
	IOMUX->gpio14_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio14_mux |= GPIO14_FUNC_SPI1_CLK;//configure io14 to Hspi_clk mode
//	pin_func_select (PERIPHS_IO_MUX_MTDO_U, 2);//configure io15 to Hspi_cs mode
	IOMUX->gpio15_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio15_mux |= GPIO15_FUNC_SPI1_CS;

	SPI1->user |= SPI_USER_CS_SETUP | SPI_USER_CS_HOLD | SPI_USER_COMMAND;
	SPI1->user &= ~SPI_USER_MODE;

	SPI1->clock_bits.pre_s = SPI_PREDIV1_CLK;
	SPI1->clock_bits.clkcnt_n = SPI_DIV1_CLK_N;
	SPI1->clock_bits.clkcnt_h = SPI_DIV1_CLK_H;
	SPI1->clock_bits.clkcnt_l = SPI_DIV1_CLK_L;

}

/*****************************************************************************
 * ������������� ���� GPIO4 ��� ������� �� ��������
 * ��������� ���������� �� ������ � ������� ����������
 ****************************************************************************/
void spi_req_intr_init (void)
{
	IOMUX->gpio4_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio4_mux |= GPIO4_FUNC_GPIO;		//configure io4 to GPIO mode
}

/******************************************************************************
 * FunctionName : spi_lcd_9bit_write
 * Description  : SPI 9bits transmission function for driving LCD TM035PDZV36
 * Parameters   : uint8 high_bit - first high bit of the data, 0 is for "0", the other value 1-255 is for "1"
 *				uint8 low_8bit- the rest 8bits of the data.
 *******************************************************************************/
void ICACHE_FLASH_ATTR spi_lcd_9bit_write (uint8 high_bit, uint8 low_8bit)
{
	spi_user2_t regvalue;
	uint8 bytetemp;

	if (high_bit)
		bytetemp = (low_8bit >> 1) | 0x80;
	else
		bytetemp = (low_8bit >> 1) & 0x7f;

	regvalue.command_bitlen = 8;
	regvalue.command_value = bytetemp;//configure transmission variable, 9bit transmission length and first 8 command bit
	if (low_8bit & 0x01)
	{
		regvalue.command_value |= BIT15; //write the 9th bit
	}
	while (SPI1->cmd_bits.usr)
		;	//waiting for spi module available
	SPI1->cmd_bits.usr = 1;		// transmission start
	//	while(SPI1->cmd_bits.usr);
}

/******************************************************************************
 * FunctionName : spi_mast_byte_write
 * Description  : SPI master 1 byte transmission function
 * Parameters	: uint8 data- transmitted data
 *******************************************************************************/
void ICACHE_FLASH_ATTR spi_mast_byte_write (uint8 data)
{
	uint32 regvalue;

	while (SPI1->cmd_bits.usr)
		;	//waiting for spi module available

	SPI1->user &= ~ (SPI_USER_MISO | SPI_USER_MOSI);
	SPI1->user_bits.command = 1;
	SPI1->user2_bits.command_bitlen = 7;	// cmd bit length is value(0-15)+1,
	SPI1->user2_bits.command_value = data;
	SPI1->cmd_bits.usr = 1;	// transmission start
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
	while (SPI1->cmd_bits.usr)
		;	//waiting for spi module available

	SPI1->user |= SPI_USER_MOSI | SPI_USER_COMMAND;
	SPI1->user &= ~ (SPI_USER_MISO | SPI_USER_ADDR | SPI_USER_DUMMY);
	SPI1->user2_bits.command_bitlen = 7;
	SPI1->user2_bits.command_value = 0x04;
	SPI1->user1_bits.mosi_bitlen = 7;
	SPI1->w0 = data;
	SPI1->cmd_bits.usr = 1;	// transmission start
}

/******************************************************************************
 * FunctionName : spi_byte_read_espslave
 * Description  : SPI master 1 byte read function for esp8266 slave,
 * 		 read 1byte data from esp8266 slave buffer needs 16bit transmission ,
 * 		 first byte is command 0x06 to read slave buffer, second byte is received data
 * Parameters	 :	 uint8* data- received data address
 *******************************************************************************/
void ICACHE_FLASH_ATTR spi_byte_read_espslave (uint8 *data)
{
	uint32 regvalue;
	while (SPI1->cmd_bits.usr)
		;	//waiting for spi module available

	SPI1->user |= SPI_USER_MISO | SPI_USER_COMMAND;
	SPI1->user &= ~ (SPI_USER_MOSI | SPI_USER_ADDR | SPI_USER_DUMMY);
	SPI1->user2_bits.command_bitlen = 7;
	SPI1->user2_bits.command_value = 0x06;
	//in register SPI_FLASH_USER1, bit 8-16 stores MOSI bit length value
	//The value shall be (bit_num-1).
	//for example, we READ 1 byte data which has length of 8 bits,
	//therefore the MISO bit length value of 7 should be written into the related register bits.
	SPI1->user1_bits.miso_bitlen = 7;

	SPI1->cmd_bits.usr = 1;	// transmission start
	while (SPI1->cmd_bits.usr)
		;	//waiting for spi module available
	*data = SPI1->w0 & 0xFF;
}

/******************************************************************************
 * FunctionName :	   spi_WR_espslave(uint8 1)
 * Description  : SPI master byte units write and read
 * Parameters	 :
 *******************************************************************************/

void ICACHE_FLASH_ATTR spi_WR_espslave (void)
{
	uint32 regvalue;

	while (SPI1->cmd_bits.usr)
		;	//waiting for spi module available

	SPI1->user |= (SPI_USR_MOSI | SPI_USR_DOUTDIN);
	SPI1->user &= ~ (SPI_USR_COMMAND | SPI_USR_ADDR | SPI_USR_DUMMY);

	//in register SPI_FLASH_USER1, bit 17-25 stores MOSI bit length value
	//The value shall be (bit_num-1).
	//for example, we output 64-byte data which has length of 512 bits,
	//therefore the MOSI bit length value of 511(0x1FF) should be written into the related register bits.
	SPI1->user1_bits.mosi_bitlen = 511;

	SPI1->cmd_bits.usr = 1;	// transmission start
}

void ICACHE_FLASH_ATTR set_data (void)
{
	SPI1->w0 = 0x05040302;
	SPI1->w1 = 0x09080706;
	SPI1->w2 = 0x0d0c0b0a;
	SPI1->w3 = 0x11100f0e;
	SPI1->w4 = 0x15141312;
	SPI1->w5 = 0x19181716;
	SPI1->w6 = 0x1d1c1b1a;
	SPI1->w7 = 0x21201f1e;
	SPI1->w8 = 0x05040302;
	SPI1->w9 = 0x09080706;
	SPI1->w10 = 0x0d0c0b0a;
	SPI1->w11 = 0x11100f0e;
	SPI1->w12 = 0x15141312;
	SPI1->w13 = 0x19181716;
	SPI1->w14 = 0x1d1c1b1a;
	SPI1->w15 = 0x21201f1e;
}

//-
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
	IOMUX->gpio_mux_cfg = 0x105;	//clear bit9//TEST

	IOMUX->gpio12_mux_spi1_miso &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio12_mux_spi1_miso |= GPIO12_FUNC_SPI1_Q_MISO;	//configure io12 to spi1_miso mode
	IOMUX->gpio13_mux_spi1_mosi &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio13_mux_spi1_mosi |= GPIO13_FUNC_SPI1_D_MOSI;	//configure io13 to spi1_mosi mode
	IOMUX->gpio14_mux_spi1_clk &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio14_mux_spi1_clk |= GPIO14_FUNC_SPI1_CLK;	//configure io14 to spi1_clk mode
	IOMUX->gpio15_mux_spi1_cs &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio15_mux_spi1_cs |= GPIO15_FUNC_SPI1_CS;// configure io15 to spi1_cs mode

	//slave mode, slave use buffers which are register "SPI_FLASH_C0~C15", enable trans done isr
	//set bit 30 bit 29 bit9, bit9 is trans done isr mask
	SPI1->slave |= SPI_SLAVE_INT_EN_TRANS_DONE 	// all interrupts
					| SPI_SLAVE_WR_RD_BUF_EN
					| SPI_SLAVE_SLAVE_MODE;

	SPI1->slave_bits.trans_done_en = 0;	//disable general trans intr 
	SPI1->user_bits.flash_mode = 0;	//disable flash operation mode
	SPI1->user_bits.dout_highpart = 1;	//SLAVE SEND DATA BUFFER IN C8-C15

	//////**************RUN WHEN SLAVE RECIEVE*******************///////
	//two lines below is to configure spi timing.
	SPI1->ctrl2_bits.mosi_delay_num = 2;	//delay num
	os_printf ("SPI_CTRL2 is %08x\n", SPI1->ctrl2);
	SPI1->clock = 0;

	/////***************************************************//////

	//set 8 bit slave command length, because slave must have at least one bit addr, 
	//8 bit slave+8bit addr, so master device first 2 bytes can be regarded as a command 
	//and the  following bytes are datas, 
	//32 bytes input wil be stored in SPI_FLASH_C0-C7
	//32 bytes output data should be set to SPI_FLASH_C8-C15
	SPI1->user2_bits.command_bitlen = 7;

	// set 8 bit slave receive buffer length, the buffer is SPI_FLASH_C0-C7
	//set 8 bit slave status register, which is the low 8 bit of register "SPI_FLASH_STATUS"
	SPI1->slave1_bits.buf_bitlen = 0xFF;
	SPI1->slave1_bits.status_bitlen = 0x7;
	SPI1->slave1_bits.wr_addr_bitlen = 0x7;
	SPI1->slave1_bits.rd_addr_bitlen = 0x7;
	SPI1->slave1_bits.status_readback = 0;	// CHOOSE ACTIVE STATUS REG
	SPI1->pin_bits.clk1_cs0 = 1;	// BIT19

	SPI1->cmd_bits.usr = 1;	// maybe enable slave transmission listen 

	//register level2 isr function, which contains spi, hspi and i2s events
#if TWO_INTR_LINE_PROTOCOL
	ets_isr_attach (ETS_SPI_INUM, (spi_slave_isr_handler), (void *)(NULL));
#elif ONE_INTR_LINE_WITH_STATUS
	ets_isr_attach (ETS_SPI_INUM, (spi_slave_isr_sta), (void *) (NULL));
#endif
	//enable level2 isr, which contains spi, hspi and i2s events
	ets_isr_unmask ( (1 << ETS_SPI_INUM));
}

/******************************************************************************
 * FunctionName : spi_slave_isr_handler
 * Description  : SPI interrupt function, SPI HSPI and I2S interrupt can trig this function
 some basic operation like clear isr flag has been done, 
 and it is availible	for adding user coder in the funtion
 * Parameters  : void *para- function parameter address, which has been registered in function spi_slave_init
 *******************************************************************************/

static uint8 spi_data[32] = {0};
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
	IOMUX->gpio0_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio0_mux |= GPIO0_FUNC_GPIO;

#if	TWO_INTR_LINE_PROTOCOL

	IOMUX->gpio2_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio2_mux |= GPIO2_FUNC_GPIO;

	IOMUX->gpio4_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio4_mux |= GPIO4_FUNC_GPIO;

	GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_0;
	GPIO->enable_w1ts = GPIO_ENABLE_W1TS_PIN_0;

	GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_2;
	GPIO->enable_w1ts = GPIO_ENABLE_W1TS_PIN_2;

	GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_4;
	GPIO->enable_w1ts = GPIO_ENABLE_W1TS_PIN_4;

#endif	// #if	TWO_INTR_LINE_PROTOCOL
}

void spi_slave_isr_handler (void *para)
{
	spi_slave_t regvalue;
	dport_isr_flag_t dport_value = DPORT->isr_flag_bits;
	u32 calvalue;
	static uint8 state = 0;
	uint32 recv_data;
	u32 send_data;
	__IO u32 * p_reg;
	__IO u32 * p_reg2;

	if (dport_value.spi0_isr)
	{
		//following 3 lines is to clear isr signal
		SPI0->slave &= ~ (SPI_SLAVE_INT_STT_MASK | SPI_SLAVE_INT_EN_MASK);
	}
	else if (dport_value.spi1_isr)
	{ //bit7 is for hspi isr,
		regvalue = SPI1->slave_bits;	// READ_PERI_REG (SPI_SLAVE(HSPI));
		SPI1->slave &= ~ (SPI_SLAVE_INT_EN_MASK);	// запретим прерывания
		SPI1->slave_bits.sync_reset = 1;
		SPI1->slave &= ~ (SPI_SLAVE_INT_STT_MASK);
		SPI1->slave |= SPI_SLAVE_INT_EN_MASK;

		if (regvalue.wr_buf_done && regvalue.rd_buf_done)
		{
//			system_os_post (USER_TASK_PRIO_1, WR_RD, regvalue);
		}

		else if (regvalue.wr_buf_done)
		{
			GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_0;	// GPIO_OUTPUT_SET (0, 0);
			state = 0;
			idx = 0;
			p_reg = &SPI1->w0;
			while (idx < 8)	// читают принятые 32 байта
			{
				recv_data = *p_reg;
				// хз, но наверно так избавляются от привязки к endian mode
				spi_data[idx << 2] = recv_data & 0xff;
				spi_data[(idx << 2) + 1] = (recv_data >> 8) & 0xff;
				spi_data[(idx << 2) + 2] = (recv_data >> 16) & 0xff;
				spi_data[(idx << 2) + 3] = (recv_data >> 24) & 0xff;
				idx++;
				p_reg++;
			}
			p_reg = &SPI1->w8;
			for (idx = 0; idx < 8; idx++)
			{
				send_data = *p_reg++;
				calvalue = send_data & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[idx << 2] != calvalue)
				{
					GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_4;	// GPIO_OUTPUT_SET(4, 0);
//					system_os_post(USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data >> 8) & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[ (idx << 2) + 1] != calvalue)
				{
					GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_4;	// GPIO_OUTPUT_SET(4, 0);
//					system_os_post(USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data >> 16) & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[ (idx << 2) + 2] != calvalue)
				{
					GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_4; // GPIO_OUTPUT_SET(4, 0);
//					system_os_post(USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data >> 24) & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[ (idx << 2) + 3] != calvalue)
				{
					GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_4;	// GPIO_OUTPUT_SET (4, 0);
//					system_os_post (USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
			}
			p_reg = &SPI1->w8;
			p_reg2 = &SPI1->w0;
			for (idx = 0; idx < 8; idx++)
			{
				*p_reg++ = *p_reg2++;
			}
			GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_2;	// GPIO_OUTPUT_SET(2, 1);		
			GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_0;	// GPIO_OUTPUT_SET(0, 1);
			GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_4;	// GPIO_OUTPUT_SET(4, 1);
			SPI1->slave_bits.wr_buf_done_en = 1;
		}

		else if (regvalue.rd_buf_done)
		{
			GPIO->out_w1tc = GPIO_OUT_W1TS_PIN_2;	// GPIO_OUTPUT_SET(2, 0);
			state = 1;
		}

		if (regvalue.rd_done)
		{
			GPIO->out_w1tc = GPIO_OUT_W1TS_PIN_4;	// GPIO_OUTPUT_SET(4, 0);
			if (state)
			{
//				system_os_post(USER_TASK_PRIO_1, STATUS_R_IN_WR, regvalue);
				state = 0;
			}
			else
			{
//				system_os_post(USER_TASK_PRIO_1, STATUS_R_IN_RD, regvalue);
				state = 1;
			}
			GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_4;	// GPIO_OUTPUT_SET(4, 1);
		}

		if (regvalue.wr_done)
		{
			GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_4;	// GPIO_OUTPUT_SET(4, 0);
//			system_os_post(USER_TASK_PRIO_1, STATUS_W, regvalue);
			GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_4;	// GPIO_OUTPUT_SET(4, 1);
		}

		if (regvalue.trans_done
		        && !regvalue.wr_done
		        && !regvalue.rd_done
		        && !regvalue.wr_buf_done
		        && !regvalue.rd_buf_done)
		{
			GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_4;	// GPIO_OUTPUT_SET(4, 0);
//			system_os_post(USER_TASK_PRIO_1, TR_DONE_ALONE, regvalue);
			GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_4;	// GPIO_OUTPUT_SET(4, 1);	
		}

	}
	else if (dport_value.i2s_isr)
	{ //bit7 is for i2s isr,

	}
}

void spi_slave_isr_sta (void *para)
{
	spi_slave_t regvalue;
	uint32 calvalue;
	static uint8 state = 0;
	uint32 recv_data, send_data;
	__IO u32 * p_reg;
	__IO u32 * p_reg2;

	union spi_slave_status spi_sta;

	if (DPORT->isr_flag_bits.spi0_isr)
	{
		//following 3 lines is to clear isr signal
		SPI0->slave &= ~(SPI_SLAVE_INT_STT_MASK | SPI_SLAVE_INT_EN_MASK);
	}
	else if (DPORT->isr_flag_bits.spi1_isr)
	{ //bit7 is for hspi isr,
		regvalue = SPI1->slave_bits;
		SPI1->slave &= ~SPI_SLAVE_INT_EN_MASK;
		SPI1->slave_bits.sync_reset = 1;
		SPI1->slave &= ~SPI_SLAVE_INT_STT_MASK;
		SPI1->slave |= SPI_SLAVE_INT_EN_MASK;

		if (regvalue.wr_buf_done)
		{
			SPI1->rd_status_bits.wr_busy = 1;
			SPI1->rd_status_bits.comm_cnt++;

			state = 0;
			idx = 0;
			p_reg = &SPI1->w0;
			while (idx < 8)
			{
				recv_data = *p_reg++;
				spi_data[idx << 2] = recv_data & 0xff;
				spi_data[ (idx << 2) + 1] = (recv_data >> 8) & 0xff;
				spi_data[ (idx << 2) + 2] = (recv_data >> 16) & 0xff;
				spi_data[ (idx << 2) + 3] = (recv_data >> 24) & 0xff;
				idx++;
			}
			p_reg = &SPI1->w8;
			for (idx = 0; idx < 8; idx++)
			{
				send_data = *p_reg++;
				calvalue = send_data & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[idx << 2] != calvalue)
				{
					GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_4;	// GPIO_OUTPUT_SET(4, 0);
//					system_os_post (USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data >> 8) & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[ (idx << 2) + 1] != calvalue)
				{
					GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_4;	// GPIO_OUTPUT_SET(4, 0);
//					system_os_post (USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data >> 16) & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[ (idx << 2) + 2] != calvalue)
				{
					GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_4;	// GPIO_OUTPUT_SET (4, 0);
//					system_os_post (USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
				calvalue = (send_data >> 24) & 0xff;
				calvalue = (calvalue * calvalue * calvalue) % 255;
				if (spi_data[ (idx << 2) + 3] != calvalue)
				{
					GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_4;	// GPIO_OUTPUT_SET(4, 0);
//					system_os_post (USER_TASK_PRIO_1, DATA_ERROR, regvalue);
				}
			}

			SPI1->rd_status_bits.wr_busy = 0;

			p_reg = &SPI1->w8;
			p_reg2 = &SPI1->w0;
			for (idx = 0; idx < 8; idx++)
			{
				*p_reg++ = *p_reg2++;
			}
			SPI1->rd_status_bits.rd_empty = 0;
			GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_0;	// GPIO_OUTPUT_SET(0, 1);

		}
		else if (regvalue.rd_buf_done)
		{
			SPI1->rd_status_bits.comm_cnt++;
			SPI1->rd_status_bits.rd_empty = 1;
			GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_0;	// GPIO_OUTPUT_SET(0, 1);
			state = 1;
		}

		if (regvalue.rd_done)
		{
			GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_0;	// GPIO_OUTPUT_SET(0, 0);
		}

		if (regvalue.wr_done)
		{

		}

		if (regvalue.trans_done && !regvalue.rd_buf_done)
		{

		}

	}

	else if (DPORT->isr_flag_bits.i2s_isr)
	{ //bit7 is for i2s isr,

	}
}

void ICACHE_FLASH_ATTR set_miso_data (void)
{
	if (!GPIO->in_bits.pin_2)	// ���� GPIO2 = =  0
	{
		WRITE_PERI_REG(SPI_W8 (HSPI), 0x05040302);
		WRITE_PERI_REG(SPI_W9 (HSPI), 0x09080706);
		WRITE_PERI_REG(SPI_W10 (HSPI), 0x0d0c0b0a);
		WRITE_PERI_REG(SPI_W11 (HSPI), 0x11100f0e);

		WRITE_PERI_REG(SPI_W12 (HSPI), 0x15141312);
		WRITE_PERI_REG(SPI_W13 (HSPI), 0x19181716);
		WRITE_PERI_REG(SPI_W14 (HSPI), 0x1d1c1b1a);
		WRITE_PERI_REG(SPI_W15 (HSPI), 0x21201f1e);
		GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_2;	// GPIO_OUTPUT_SET (2, 1);
	}
}

void ICACHE_FLASH_ATTR disp_spi_data (void)
{
	uint8 i = 0;
	for (i = 0; i < 32; i++)
	{
		os_printf ("data %d : 0x%02x\n\r", i, spi_data[i]);
	}
}

void ICACHE_FLASH_ATTR spi_task (os_event_t *e)
{
	uint8 data;
	switch (e->sig)
	{
	case MOSI:
		disp_spi_data ();
		break;
	case STATUS_R_IN_WR:
		os_printf ("SR ERR in WRPR, Reg:%08x \n", e->par);
		break;
	case STATUS_W:
		os_printf ("SW ERR, Reg:%08x\n", e->par);
		break;
	case TR_DONE_ALONE:
		os_printf ("TD ALO ERR, Reg:%08x\n", e->par);
		break;
	case WR_RD:
		os_printf ("WR&RD ERR, Reg:%08x\n", e->par);
		break;
	case DATA_ERROR:
		os_printf ("Data ERR, Reg:%08x\n", e->par);
		break;
	case STATUS_R_IN_RD:
		os_printf ("SR ERR in RDPR, Reg:%08x\n", e->par);
		break;
	default:
		break;
	}
}

void ICACHE_FLASH_ATTR spi_task_init (void)
{
	spiQueue = (os_event_t*)os_malloc (sizeof(os_event_t) * SPI_QUEUE_LEN);
	system_os_task (spi_task, USER_TASK_PRIO_1, spiQueue, SPI_QUEUE_LEN);
}

os_timer_t spi_timer_test;

void ICACHE_FLASH_ATTR spi_test_init (void)
{
	os_printf ("spi init\n\r");
	spi_slave_init ();
	os_printf ("gpio init\n\r");
	gpio_init ();
	os_printf ("spi task init \n\r");
	spi_task_init ();
#ifdef SPI_MISO
	os_printf ("spi miso init\n\r");
	set_miso_data ();
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
void hspi_cr_start (const uint8_t cmd, const uint8_t read_length)
{

	SPI1->user_bits.command = 1;
	if (read_length)
	{
		SPI1->user_bits.miso = 1;
		SPI1->user_bits.addr = 0;
		SPI1->user_bits.dummy = 0;
		SPI1->user_bits.mosi = 0;
		SPI1->user1_bits.miso_bitlen = read_length * 8 - 1;
	}
	else
	{
		SPI1->user1_bits.miso_bitlen = 7;
	}
	//set output buffer length, the buffer is the register"SPI_FLASH_C0"
	SPI1->user1_bits.mosi_bitlen = 7;

	//SPI_FLASH_USER2 bit28-31 is cmd length,cmd bit length is value(0-15)+1,
	// bit15-0 is cmd value.
	SPI1->user2_bits.command_bitlen = 7;
	SPI1->user2_bits.command_value = cmd;

	SPI1->cmd_bits.usr = 1;   // send
}



/******************************************************************************/
/* Из примера ILI9341 */

#define SPIFIFOSIZE 16 //16 words length

extern uint32_t *spi_fifo;

//extern void hspi_init(void);
extern void hspi_send_data (const uint8_t * data, uint8_t datasize);
//extern void hspi_send_uint16_r(const uint16_t data, int32_t repeats);
inline void hspi_wait_ready (void)
{
	while (SPI1->cmd_bits.usr);
}

inline void hspi_prepare_tx (uint32_t bytecount)
{
	uint32_t bitcount = bytecount * 8 - 1;
	SPI1->user1_bits.mosi_bitlen = bitcount;
}

inline void hspi_start_tx ()
{
	SPI1->cmd_bits.usr = 1;
}

inline void hspi_send_uint8 (uint8_t data)
{
	hspi_prepare_tx (1);
	SPI1->w0 = data;
	hspi_start_tx ();
}

inline void hspi_send_uint16 (uint16_t data)
{
	hspi_prepare_tx (2);
	SPI1->w0 = data;
	hspi_start_tx ();
}

inline void hspi_send_uint32 (uint32_t data)
{
	hspi_prepare_tx (4);
	SPI1->w0 = data;
	hspi_start_tx ();
}

//#include "hspi.h"

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
uint32_t *spi_fifo = (uint32_t*)&SPI1->w0;

void hspi_init(void)
{
	spi_fifo = (uint32_t*)SPI_FLASH_C0(HSPI);
	
	// 	IOMUX->gpio_mux_cfg = GPIO_MUX_CFG_MAGIC; //clear bit9
	IOMUX->gpio_mux_cfg_bits.spi1_sys_clk = 0; //clear bit9
	
	IOMUX->gpio12_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio12_mux |= GPIO12_FUNC_SPI1_Q_MISO;	// MISO GPIO12
	IOMUX->gpio14_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio14_mux |= GPIO14_FUNC_SPI1_CLK;		// CLK GPIO14
	IOMUX->gpio15_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio15_mux |= GPIO15_FUNC_SPI1_CS;		// CS GPIO15
	IOMUX->gpio13_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio13_mux |= GPIO13_FUNC_MTCK;			// HSPID  GPIO13


	// SPI clock = CPU clock / 10 / 4
	// time length HIGHT level = (CPU clock / 10 / 2) ^ -1,
	// time length LOW level = (CPU clock / 10 / 2) ^ -1
	SPI1->clock_bits.pre_s = HSPI_PRESCALER - 1;
	SPI1->clock_bits.clkcnt_n = 1;
	SPI1->clock_bits.clkcnt_h = 0;
	SPI1->clock_bits.clkcnt_l = 1;

	SPI1->ctrl1 = 0;
	
	//clear bit 2 see example IoT_Demo
	SPI1->user_bits.addr = 0;
	SPI1->user_bits.dummy = 0;
	SPI1->user_bits.miso = 0;
	SPI1->user_bits.command = 0;
	SPI1->user_bits.dout_din = 0;
}
	
void hspi_send_uint16_r (uint16_t data, int32_t repeats)
{
	uint32_t i;
	uint32_t word = data << 16 | data;
	
	while (repeats > 0)
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

void hspi_send_data (const uint8_t * data, uint8_t datasize)
{
	uint32_t *_data = (uint32_t*)data;
	uint8_t i;

	uint8_t words_to_send = __min( (datasize + 3) / 4, SPIFIFOSIZE);
	hspi_prepare_tx (datasize);
	for (i = 0; i < words_to_send; i++)
		spi_fifo[i] = _data[i];
	hspi_start_tx ();
}

