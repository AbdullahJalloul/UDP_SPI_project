#include 	<c_types.h>
#include	<ESP8266_registers.h>
#include	<gpio.h>
//#include	"eagle_soc.h"
//#include	"bits.h"
#include	"driver/spi_overlap.h"
#include	"driver/spi.h"

#define SPI_FLASH_READ_MODE_MASK 0x196000
//#define WAIT_HSPI_IDLE() 	while(READ_PERI_REG(SPI_EXT2(HSPI))||(READ_PERI_REG(SPI_CMD(HSPI))&0xfffc0000));
//#define WAIT_HSPI_IDLE() 	while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));

//#define CONF_HSPI_CLK_DIV(div)	WRITE_PERI_REG(SPI_CLOCK(HSPI), (((div<<1)+1)<<12)+(div<<6)+(div<<1)+1)
#define CONF_HSPI_CLK_DIV(div)		SPI1->clock = ((((div << 1) + 1) << 12) + (div << 6) + (div << 1) + 1)
//#define HSPI_FALLING_EDGE_SAMPLE()		SET_PERI_REG_MASK(SPI_USER(HSPI),  SPI_CK_OUT_EDGE)

//#define HSPI_RISING_EDGE_SAMPLE()			CLEAR_PERI_REG_MASK(SPI_USER(HSPI),  SPI_CK_OUT_EDGE)

//#define ACTIVE_HSPI_CS0	 	CLEAR_PERI_REG_MASK(SPI_PIN(HSPI), SPI_CS0_DIS);\
//						SET_PERI_REG_MASK(SPI_PIN(HSPI), SPI_CS1_DIS |SPI_CS2_DIS)
//#define ACTIVE_HSPI_CS1		CLEAR_PERI_REG_MASK(SPI_PIN(HSPI), SPI_CS1_DIS);\
//						SET_PERI_REG_MASK(SPI_PIN(HSPI), SPI_CS0_DIS |SPI_CS2_DIS)
//#define ACTIVE_HSPI_CS2		CLEAR_PERI_REG_MASK(SPI_PIN(HSPI), SPI_CS2_DIS);\
//						SET_PERI_REG_MASK(SPI_PIN(HSPI), SPI_CS0_DIS |SPI_CS1_DIS)
//#define ENABLE_HSPI_DEV_CS()		pin_func_select (PERIPHS_IO_MUX_MTDO_U, 2)
//#define DISABLE_HSPI_DEV_CS()		GPIO->out_w1ts_bits.pin_15 = 1;\
//									IOMUX->gpio15_mux |= (IOMUX->gpio15_mux & ~GPIO_MUX_FUNC_MASK) | GPIO15_FUNC_GPIO

struct hspi_device_register hspi_dev_reg;
/******************************************************************************
* FunctionName : hspi_overlap_init
* Description  : enable hspi and spi module overlap mode
*******************************************************************************/
void ICACHE_FLASH_ATTR
hspi_overlap_init (void)
{
	//hspi overlap to spi, two spi masters on cspi
	DPORT->peri_io_bits.spi0_overlap = 1;

	//set higher priority for spi than hspi
	SPI0->ext3_bits.int_priority = 0x1;
	SPI1->ext3_bits.int_priority = 0x3;
	SPI1->user_bits.cs_setup = 1;
}

/******************************************************************************
* FunctionName : hspi_overlap_deinit
* Description  : recover hspi and spi module from overlap mode
*******************************************************************************/
void ICACHE_FLASH_ATTR
hspi_overlap_deinit (void)
{
	DPORT->peri_io_bits.spi0_overlap = 0;

	SPI0->ext3_bits.int_priority = 0;
	SPI1->ext3_bits.int_priority = 0;
	SPI1->user_bits.cs_setup = 0;
}

/******************************************************************************
* FunctionName : spi_reg_backup
* Description  : backup SPI normal operation register value and disable CPU cache to modify some flash registers.
* Parameters   : 	uint8 spi_no - SPI module number, Only "SPI" and "HSPI" are valid
*******************************************************************************/
void ICACHE_FLASH_ATTR
spi_reg_backup (uint8 spi_no, uint32* backup_mem)
{
	SPI_TypeDef *pSPI = SPI0;
	if (spi_no > 1) return; //handle invalid input number
	if (spi_no == 1)
	{
		pSPI = SPI1;
	}
	backup_mem[PERIPHS_IO_MUX_BACKUP] = IOMUX->gpio_mux_cfg;
	backup_mem[SPI_USER_BACKUP] = pSPI->user;
	backup_mem[SPI_CTRL_BACKUP] = pSPI->ctrl;
	backup_mem[SPI_CLOCK_BACKUP] = pSPI->clock;
	backup_mem[SPI_USER1_BACKUP] = pSPI->user1;
	backup_mem[SPI_USER2_BACKUP] = pSPI->user2;
	backup_mem[SPI_CMD_BACKUP] = pSPI->cmd;
	backup_mem[SPI_PIN_BACKUP] = pSPI->pin;
	backup_mem[SPI_SLAVE_BACKUP] = pSPI->slave;
}
/******************************************************************************
 * FunctionName : spi_reg_recover
 * Description  : recover SPI normal operation register value and enable CPU cache.
 * Parameters   : 	uint8 spi_no - SPI module number, Only "SPI" and "HSPI" are valid
 *******************************************************************************/
void ICACHE_FLASH_ATTR spi_reg_recover (uint8 spi_no, uint32* backup_mem)
{
	SPI_TypeDef *pSPI = SPI0;
	if (spi_no > 1) return; //handle invalid input number
	if (spi_no == 1)
	{
		pSPI = SPI1;
	}
//	IOMUX->gpio_mux_cfg = backup_mem[PERIPHS_IO_MUX_BACKUP];
	pSPI->user = backup_mem[SPI_USER_BACKUP];
	pSPI->ctrl = backup_mem[SPI_CTRL_BACKUP];
	pSPI->clock = backup_mem[SPI_CLOCK_BACKUP];
	pSPI->user1 = backup_mem[SPI_USER1_BACKUP];
	pSPI->user2 = backup_mem[SPI_USER2_BACKUP];
	pSPI->cmd = backup_mem[SPI_CMD_BACKUP];
	pSPI->pin = backup_mem[SPI_PIN_BACKUP];
//	pSPI->slave = backup_mem[SPI_SLAVE_BACKUP];
}

void ICACHE_FLASH_ATTR hspi_master_dev_init (uint8 dev_no, uint8 clk_polar, uint8 clk_div)
{
	uint32 regtemp;
	if ( (dev_no > 3) || (clk_polar > 1) || (clk_div > 0x1f))
	{
		os_printf ("hspi_master_dev_init parameter is out of range!\n\r");
		return;
	}

	while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
	if (!hspi_dev_reg.hspi_reg_backup_flag)
	{
		if (IOMUX->gpio_mux_cfg_bits.spi0_sys_clk)
		{
			hspi_dev_reg.spi_io_80m = 1;
			SPI1->clock_bits.equ_sysclk = 1;
		}
		else
		{
			hspi_dev_reg.spi_io_80m = 0;
			SPI1->clock_bits.equ_sysclk = 0;
		}
		regtemp = SPI0->ctrl & SPI_FLASH_READ_MODE_MASK;
		SPI1->ctrl = (SPI1->ctrl & ~SPI_FLASH_READ_MODE_MASK) | regtemp;

		spi_reg_backup (HSPI, hspi_dev_reg.hspi_flash_reg_backup);

		spi_master_init ();
		spi_reg_backup (HSPI, hspi_dev_reg.hspi_dev_reg_backup);

		hspi_dev_reg.hspi_reg_backup_flag = 1;

		//	spi_reg_recover(HSPI, hspi_dev_reg.hspi_flash_reg_backup);
		hspi_dev_reg.selected_dev_num = HSPI_IDLE;
	}

	hspi_dev_reg.hspi_dev_conf[dev_no].active = 1;
	hspi_dev_reg.hspi_dev_conf[dev_no].clk_div = clk_div;
	hspi_dev_reg.hspi_dev_conf[dev_no].clk_polar = clk_polar;

	switch (dev_no)
	{
	case HSPI_CS_DEV:
		pin_func_select (PERIPHS_IO_MUX_MTDI_U, 2);
		pin_func_select (PERIPHS_IO_MUX_MTCK_U, 2);
		pin_func_select (PERIPHS_IO_MUX_MTMS_U, 2);
		pin_func_select (PERIPHS_IO_MUX_MTDO_U, 2);
		IOMUX->gpio_mux_cfg_bits.spi1_sys_clk = 0;
		break;

	case SPI_CS1_DEV:
		pin_func_select (PERIPHS_IO_MUX_U0TXD_U, FUNC_SPI_CS1);
		if (hspi_dev_reg.spi_io_80m)
		{
			os_printf ("SPI CS1 device must work at 80Mhz");
		}
		break;

	case SPI_CS2_DEV:
		pin_func_select (PERIPHS_IO_MUX_GPIO0_U, FUNC_SPI_CS2);
		if (hspi_dev_reg.spi_io_80m)
		{
			os_printf ("SPI CS2 device must work at 80Mhz");
		}
		break;

	default:
		break;
	}
}

void ICACHE_FLASH_ATTR hspi_dev_sel (uint8 dev_no)
{
	uint32 regval;

	if (dev_no > 3)
	{
		os_printf ("hspi_dev_sel parameter is out of range!\n\r");
		return;
	}

	if (!hspi_dev_reg.hspi_dev_conf[dev_no].active)
	{
		os_printf ("device%d has not been initialized!\n\r", dev_no);
		return;
	}

	switch (hspi_dev_reg.selected_dev_num)
	{
	case HSPI_CS_DEV:
		if ( (dev_no == SPI_CS1_DEV) || (dev_no == SPI_CS2_DEV))
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			GPIO->out_w1ts_bits.pin_15 = 1;	// DISABLE_HSPI_DEV_CS();
			hspi_overlap_init ();
			if (hspi_dev_reg.spi_io_80m)
			{
				SPI1->clock_bits.equ_sysclk = 1;
			}
			else
			{
				CONF_HSPI_CLK_DIV(hspi_dev_reg.hspi_dev_conf[dev_no].clk_div);
			}
	
			if (hspi_dev_reg.hspi_dev_conf[dev_no].clk_polar)
			{
				SPI1->user_bits.ck_out_edge = 1;
			}
			else
			{
				SPI1->user_bits.ck_out_edge = 0;;
			}
	
			if (dev_no == SPI_CS1_DEV)
			{
				SPI1->pin_bits.cs0_dis = 1;
				SPI1->pin_bits.cs1_dis = 0;
				SPI1->pin_bits.cs2_dis = 1;	//ACTIVE_HSPI_CS1;
			}
			else
			{
				SPI1->pin_bits.cs0_dis = 1;
				SPI1->pin_bits.cs1_dis = 1;
				SPI1->pin_bits.cs2_dis = 0;	//ACTIVE_HSPI_CS2;
			}
		}
		else if (dev_no == SPI_CS0_FLASH)
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			GPIO->out_w1ts_bits.pin_15 = 1;	// DISABLE_HSPI_DEV_CS();
			hspi_overlap_init ();
			spi_reg_recover (HSPI, hspi_dev_reg.hspi_flash_reg_backup);
	
			if (hspi_dev_reg.spi_io_80m)
			{
				SPI1->clock_bits.equ_sysclk = 1;
			}
	
			SPI1->user_bits.ck_out_edge = 0;;
		
			SPI1->pin_bits.cs0_dis = 0;
			SPI1->pin_bits.cs1_dis = 1;
			SPI1->pin_bits.cs2_dis = 1;	// ACTIVE_HSPI_CS0;
		}
	break;

	case SPI_CS1_DEV:
		if (dev_no == SPI_CS2_DEV)
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			if (!hspi_dev_reg.spi_io_80m)
			{
				CONF_HSPI_CLK_DIV(hspi_dev_reg.hspi_dev_conf[dev_no].clk_div);
		}
	
		if (hspi_dev_reg.hspi_dev_conf[dev_no].clk_polar)
		{
			SPI1->user_bits.ck_out_edge = 1;
		}
		else
		{
			SPI1->user_bits.ck_out_edge = 0;;
		}
		SPI1->pin_bits.cs0_dis = 1;
		SPI1->pin_bits.cs1_dis = 1;
		SPI1->pin_bits.cs2_dis = 0;	// ACTIVE_HSPI_CS2;
	}
	else if (dev_no == SPI_CS0_FLASH)
	{
		while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
		spi_reg_recover (HSPI, hspi_dev_reg.hspi_flash_reg_backup);
		SPI1->user_bits.ck_out_edge = 0;;
		SPI1->pin_bits.cs0_dis = 0;
		SPI1->pin_bits.cs1_dis = 1;
		SPI1->pin_bits.cs2_dis = 1;	//ACTIVE_HSPI_CS0;
	}
	else if (dev_no == HSPI_CS_DEV)
	{
		while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
		
		IOMUX->gpio15_mux = GPIO15_FUNC_SPI1_CS;	// ENABLE_HSPI_DEV_CS();
		
		hspi_overlap_deinit ();
		CONF_HSPI_CLK_DIV(hspi_dev_reg.hspi_dev_conf[dev_no].clk_div);
	
		if (hspi_dev_reg.hspi_dev_conf[dev_no].clk_polar)
		{
			SPI1->user_bits.ck_out_edge = 1;
		}
		else
		{
			SPI1->user_bits.ck_out_edge = 0;;
		}
	
		SPI1->pin_bits.cs0_dis = 0;
		SPI1->pin_bits.cs1_dis = 1;
		SPI1->pin_bits.cs2_dis = 1;	//ACTIVE_HSPI_CS0;
	}
	break;
	
	case SPI_CS2_DEV:
		if (dev_no == SPI_CS1_DEV)
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			if (!hspi_dev_reg.spi_io_80m)
			{
				CONF_HSPI_CLK_DIV(hspi_dev_reg.hspi_dev_conf[dev_no].clk_div);
			}
			if (hspi_dev_reg.hspi_dev_conf[dev_no].clk_polar)
			{
				SPI1->user_bits.ck_out_edge = 1;
			}
			else
			{
				SPI1->user_bits.ck_out_edge = 0;;
			}
			SPI1->pin_bits.cs0_dis = 1;
			SPI1->pin_bits.cs1_dis = 0;
			SPI1->pin_bits.cs2_dis = 1;	// ACTIVE_HSPI_CS1;
		}
		else if (dev_no == SPI_CS0_FLASH)
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			spi_reg_recover (HSPI, hspi_dev_reg.hspi_flash_reg_backup);
			SPI1->user_bits.ck_out_edge = 0;;
			SPI1->pin_bits.cs0_dis = 0;
			SPI1->pin_bits.cs1_dis = 1;
			SPI1->pin_bits.cs2_dis = 1;	//ACTIVE_HSPI_CS0;
		}
		else if (dev_no == HSPI_CS_DEV)
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			IOMUX->gpio15_mux = GPIO15_FUNC_SPI1_CS;	// ENABLE_HSPI_DEV_CS();
			hspi_overlap_deinit ();
			CONF_HSPI_CLK_DIV(hspi_dev_reg.hspi_dev_conf[dev_no].clk_div);
			if (hspi_dev_reg.hspi_dev_conf[dev_no].clk_polar)
			{
				SPI1->user_bits.ck_out_edge = 1;
			}
			else
			{
				SPI1->user_bits.ck_out_edge = 0;;
			}
			SPI1->pin_bits.cs0_dis = 0;
			SPI1->pin_bits.cs1_dis = 1;
			SPI1->pin_bits.cs2_dis = 1;	//ACTIVE_HSPI_CS0;
		}
	break;
	
	case SPI_CS0_FLASH:
		if ( (dev_no == SPI_CS1_DEV) || (dev_no == SPI_CS2_DEV))
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			spi_reg_recover (HSPI, hspi_dev_reg.hspi_dev_reg_backup);
			if (hspi_dev_reg.spi_io_80m)
			{
				SPI1->clock_bits.equ_sysclk = 1;
			}
			else
			{
				CONF_HSPI_CLK_DIV(hspi_dev_reg.hspi_dev_conf[dev_no].clk_div);
			}
			if (hspi_dev_reg.hspi_dev_conf[dev_no].clk_polar)
			{
				SPI1->user_bits.ck_out_edge = 1;
			}
			else
			{
				SPI1->user_bits.ck_out_edge = 0;;
			}
			
			if (dev_no == SPI_CS1_DEV)
			{
				SPI1->pin_bits.cs0_dis = 1;
				SPI1->pin_bits.cs1_dis = 0;
				SPI1->pin_bits.cs2_dis = 1;	//ACTIVE_HSPI_CS1;
			}
			else
			{
				SPI1->pin_bits.cs0_dis = 1;
				SPI1->pin_bits.cs1_dis = 1;
				SPI1->pin_bits.cs2_dis = 0;	//ACTIVE_HSPI_CS2;
			}
		}
		else if (dev_no == HSPI_CS_DEV)
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			IOMUX->gpio15_mux = GPIO15_FUNC_SPI1_CS;	// ENABLE_HSPI_DEV_CS();
			hspi_overlap_deinit ();
			spi_reg_recover (HSPI, hspi_dev_reg.hspi_dev_reg_backup);
			CONF_HSPI_CLK_DIV(hspi_dev_reg.hspi_dev_conf[dev_no].clk_div);
			if (hspi_dev_reg.hspi_dev_conf[dev_no].clk_polar)
			{
				SPI1->user_bits.ck_out_edge = 1;
			}
			else
			{
				SPI1->user_bits.ck_out_edge = 0;;
			}
			SPI1->pin_bits.cs0_dis = 0;
			SPI1->pin_bits.cs1_dis = 1;
			SPI1->pin_bits.cs2_dis = 1;	//ACTIVE_HSPI_CS0;
		}
	break;
	
	default:
		if ( (dev_no == SPI_CS1_DEV) || (dev_no == SPI_CS2_DEV))
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			GPIO->out_w1ts_bits.pin_15 = 1;	// DISABLE_HSPI_DEV_CS();
			hspi_overlap_init ();
			spi_reg_recover (HSPI, hspi_dev_reg.hspi_dev_reg_backup);
			if (hspi_dev_reg.spi_io_80m)
			{
				SPI1->clock_bits.equ_sysclk = 1;
			}
			else
			{
				CONF_HSPI_CLK_DIV(hspi_dev_reg.hspi_dev_conf[dev_no].clk_div);
			}
			if (hspi_dev_reg.hspi_dev_conf[dev_no].clk_polar)
			{
				SPI1->user_bits.ck_out_edge = 1;
			}
			else
			{
				SPI1->user_bits.ck_out_edge = 0;;
			}
			
			if (dev_no == SPI_CS1_DEV)
			{
				SPI1->pin_bits.cs0_dis = 1;
				SPI1->pin_bits.cs1_dis = 0;
				SPI1->pin_bits.cs2_dis = 1;	//ACTIVE_HSPI_CS1;
			}
			else
			{
				SPI1->pin_bits.cs0_dis = 1;
				SPI1->pin_bits.cs1_dis = 1;
				SPI1->pin_bits.cs2_dis = 0;	//ACTIVE_HSPI_CS2;
			}
		}
		else if (dev_no == SPI_CS0_FLASH)
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			GPIO->out_w1ts_bits.pin_15 = 1;	// DISABLE_HSPI_DEV_CS();
			hspi_overlap_init ();
			spi_reg_recover (HSPI, hspi_dev_reg.hspi_flash_reg_backup);
			if (hspi_dev_reg.spi_io_80m)
			{
				SPI1->clock_bits.equ_sysclk = 1;
			}
			SPI1->user_bits.ck_out_edge = 0;;
			SPI1->pin_bits.cs0_dis = 0;
			SPI1->pin_bits.cs1_dis = 1;
			SPI1->pin_bits.cs2_dis = 1;	//ACTIVE_HSPI_CS0;
		}
		else if (dev_no == HSPI_CS_DEV)
		{
			while (SPI1->ext2 || (SPI1->cmd & 0xfffc0000));	// WAIT_HSPI_IDLE();
			IOMUX->gpio15_mux = GPIO15_FUNC_SPI1_CS;	// ENABLE_HSPI_DEV_CS();
			hspi_overlap_deinit ();
			spi_reg_recover (HSPI, hspi_dev_reg.hspi_dev_reg_backup);
			CONF_HSPI_CLK_DIV(hspi_dev_reg.hspi_dev_conf[dev_no].clk_div);
			
			if (hspi_dev_reg.hspi_dev_conf[dev_no].clk_polar)
			{
				SPI1->user_bits.ck_out_edge = 1;
			}
			else
			{
				SPI1->user_bits.ck_out_edge = 0;;
			}
			SPI1->pin_bits.cs0_dis = 0;
			SPI1->pin_bits.cs1_dis = 1;
			SPI1->pin_bits.cs2_dis = 1;	//ACTIVE_HSPI_CS0;
		}
	break;
	}
	hspi_dev_reg.selected_dev_num = dev_no;
}

/******************************************************************************
 * FunctionName : spi_read_data
 * Description  : use hspi to read flash data for stability test
 * Parameters   : 	SpiFlashChip * spi-- flash parameter structure pointer
 * 				uint32 flash_addr--flash start address
 *				uint32 * addr_dest--start address for preped destination memory space
 *				uint32 byte_length--length of the data which needs to be read from flash
 *******************************************************************************/
SpiFlashOpResult ICACHE_FLASH_ATTR
hspi_overlap_read_flash_data (SpiFlashChip * spi, uint32 flash_addr, uint32 * addr_dest, uint32 byte_length)
{
	uint32 temp_addr, reg_tmp;
	sint32 temp_length;
	uint8 i;
	uint8 remain_word_num;
	__IO u32 *spi_reg;
	
	hspi_dev_sel (SPI_CS0_FLASH);
	
	 //address range check 	
	if ( (flash_addr + byte_length) > (spi->chip_size))
	{
		return 0;	//SPI_FLASH_RESULT_ERR;
	}
	
	temp_addr = flash_addr;
	temp_length = byte_length;
	
	while (temp_length > 0)
	{
		if (temp_length >= SPI_BUFF_BYTE_NUM)
		{
			//   reg_tmp=((temp_addr&0xff)<<16)|(temp_addr&0xff00)|((temp_addr&0xff0000)>>16)|(SPI_BUFF_BYTE_NUM << SPI_FLASH_BYTES_LEN);
			SPI1->addr = temp_addr | (SPI_BUFF_BYTE_NUM << SPI_FLASH_BYTES_LEN);
			SPI1->cmd = SPI_CMD_READ;
			while (SPI1->cmd);
			for (i = 0, spi_reg = &SPI1->w0; i < (SPI_BUFF_BYTE_NUM >> 2); i++)
			{
				*addr_dest++ = *spi_reg++;	// READ_PERI_REG(SPI_W0(HSPI)+i*4);
			}
			temp_length = temp_length - SPI_BUFF_BYTE_NUM;
			temp_addr = temp_addr + SPI_BUFF_BYTE_NUM;
		}
		else
		{
			// WRITE_PERI_REG(SPI_ADDR(HSPI), temp_addr |(temp_length << SPI_FLASH_BYTES_LEN ));
			SPI1->addr = temp_addr | (temp_length << SPI_FLASH_BYTES_LEN);
			SPI1->cmd = SPI_CMD_READ;	// WRITE_PERI_REG(SPI_CMD(HSPI), SPI_FLASH_READ);
			while (SPI1->cmd);	
			remain_word_num = (0 == (temp_length & 0x3)) ? (temp_length >> 2) : (temp_length >> 2) + 1;
			for (i = 0, spi_reg = &SPI1->w0; i < remain_word_num; i++)
			{
				*addr_dest++ = *spi_reg++;	// READ_PERI_REG(SPI_W0(HSPI)+i*4);
			}
			temp_length = 0;
		}	
	}
	
	return 1;	//SPI_FLASH_RESULT_OK;
}

void ICACHE_FLASH_ATTR hspi_overlap_flash_init (void)
{
	hspi_master_dev_init (SPI_CS0_FLASH, 0, 0);
	
	spi_flash_set_read_func (hspi_overlap_read_flash_data);
}
