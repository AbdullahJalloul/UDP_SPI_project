/*
 *  Copyright (c) 2010 - 2011 Espressif System
 *  Added on: 17.05.2016
 *      Author: Ilya Petrukhin
 *
 */

#ifndef SPI_REGISTER_H_INCLUDED
#define SPI_REGISTER_H_INCLUDED

#include "c_types.h"


#define REG_SPI_BASE(i)		(0x60000200 - i * 0x100)
#define SPI_CMD(i)			(REG_SPI_BASE(i) + 0x00)
#define SPI_FLASH_READ		BIT31
#define SPI_FLASH_WREN		BIT30
#define SPI_FLASH_WRDI		BIT29
#define SPI_FLASH_RDID		BIT28
#define SPI_FLASH_RDSR		BIT27
#define SPI_FLASH_WRSR		BIT26
#define SPI_FLASH_PP		BIT25
#define SPI_FLASH_SE		BIT24
#define SPI_FLASH_BE		BIT23
#define SPI_FLASH_CE		BIT22
#define SPI_FLASH_RES		BIT20
#define SPI_USR				BIT18

#define SPI_ADDR(i)			(REG_SPI_BASE(i) + 0x04)

#define SPI_CTRL(i)			(REG_SPI_BASE(i) + 0x08)
#define SPI_WR_BIT_ORDER	BIT26
#define SPI_RD_BIT_ORDER	BIT25
#define SPI_QIO_MODE		BIT24
#define SPI_DIO_MODE		BIT23
#define SPI_QOUT_MODE		BIT20
#define SPI_DOUT_MODE		BIT14
#define SPI_FASTRD_MODE		BIT13

#define SPI_CTRL1(i)		(REG_SPI_BASE(i) + 0x0C)
#define  SPI_CS_HOLD_DELAY					((u32)0x0000000f)
#define  SPI_CS_HOLD_DELAY_RES				((u32)0x00000fff)

#define SPI_RD_STATUS(i)	(REG_SPI_BASE(i) + 0x10)

#define SPI_CTRL2(i)		(REG_SPI_BASE(i) + 0x14)

#define SPI_CS_DELAY_NUM		0x0000000F
#define SPI_CS_DELAY_NUM_S 		28
#define SPI_CS_DELAY_MODE 		0x00000003
#define SPI_CS_DELAY_MODE_S 	26
#define SPI_MOSI_DELAY_NUM 		0x00000007
#define SPI_MOSI_DELAY_NUM_S 	23
#define SPI_MOSI_DELAY_MODE 	0x00000003
#define SPI_MOSI_DELAY_MODE_S 	21
#define SPI_MISO_DELAY_NUM 		0x00000007
#define SPI_MISO_DELAY_NUM_S 	18
#define SPI_MISO_DELAY_MODE 	0x00000003
#define SPI_MISO_DELAY_MODE_S	16

#define SPI_CLOCK(i)		(REG_SPI_BASE(i) + 0x18)
#define SPI_CLK_EQU_SYSCLK	(BIT31)
#define SPI_CLKDIV_PRE		0x00001FFF
#define SPI_CLKDIV_PRE_S	18
#define SPI_CLKCNT_N 		0x0000003F
#define SPI_CLKCNT_N_S		12
#define SPI_CLKCNT_H		0x0000003F
#define SPI_CLKCNT_H_S		6
#define SPI_CLKCNT_L		0x0000003F
#define SPI_CLKCNT_L_S		0

#define SPI_USER(i)			(REG_SPI_BASE(i) + 0x1C)
#define SPI_USR_COMMAND		BIT31
#define SPI_USR_ADDR 		BIT30
#define SPI_USR_DUMMY 		BIT29
#define SPI_USR_MISO 		BIT28
#define SPI_USR_MOSI 		BIT27

#define SPI_USR_MOSI_HIGHPART	BIT25
#define SPI_USR_MISO_HIGHPART	BIT24

#define SPI_SIO				BIT16
#define SPI_FWRITE_QIO		BIT15
#define SPI_FWRITE_DIO		BIT14
#define SPI_FWRITE_QUAD		BIT13
#define SPI_FWRITE_DUAL		BIT12
#define SPI_WR_BYTE_ORDER	BIT11
#define SPI_RD_BYTE_ORDER	BIT10
#define SPI_CK_OUT_EDGE		BIT7
#define SPI_CK_I_EDGE		BIT6
#define SPI_CS_SETUP		BIT5
#define SPI_CS_HOLD			BIT4
#define SPI_FLASH_MODE		BIT2
#define SPI_USR_DOUTDIN		BIT0

#define SPI_USER1(i)			(REG_SPI_BASE(i) + 0x20)
#define SPI_USR_ADDR_BITLEN		0x0000003F
#define SPI_USR_ADDR_BITLEN_S	26
#define SPI_USR_MOSI_BITLEN 	0x000001FF
#define SPI_USR_MOSI_BITLEN_S 	17
#define SPI_USR_MISO_BITLEN 	0x000001FF
#define SPI_USR_MISO_BITLEN_S 	8

#define SPI_USR_DUMMY_CYCLELEN		0x000000FF
#define SPI_USR_DUMMY_CYCLELEN_S	0

#define SPI_USER2(i)				(REG_SPI_BASE(i) + 0x24)
#define SPI_USR_COMMAND_BITLEN 		0x0000000F
#define SPI_USR_COMMAND_BITLEN_S 	28
#define SPI_USR_COMMAND_VALUE 		0x0000FFFF
#define SPI_USR_COMMAND_VALUE_S 	0

#define SPI_WR_STATUS(i)			(REG_SPI_BASE(i) + 0x28)
#define SPI_PIN(i)					(REG_SPI_BASE(i) + 0x2C)
#define SPI_CS2_DIS				BIT2
#define SPI_CS1_DIS 			BIT1
#define SPI_CS0_DIS 			BIT0
#define SPI_CS_DIS 				BIT30

#define SPI_SLAVE(i)				(REG_SPI_BASE(i) + 0x30)
#define SPI_SYNC_RESET			BIT31
#define SPI_SLAVE_MODE			BIT30
#define SPI_SLV_WR_RD_BUF_EN	BIT29
#define SPI_SLV_WR_RD_STA_EN	BIT28
#define SPI_SLV_CMD_DEFINE		BIT27
#define SPI_TRANS_CNT			0x0000000F
#define SPI_TRANS_CNT_S			23
#define SPI_TRANS_DONE_EN 		BIT9
#define SPI_SLV_WR_STA_DONE_EN 	BIT8
#define SPI_SLV_RD_STA_DONE_EN 	BIT7
#define SPI_SLV_WR_BUF_DONE_EN 	BIT6
#define SPI_SLV_RD_BUF_DONE_EN 	BIT5

#define SLV_SPI_INT_EN			0x0000001f
#define SLV_SPI_INT_EN_S		5

#define SPI_TRANS_DONE 			BIT4
#define SPI_SLV_WR_STA_DONE 	BIT3
#define SPI_SLV_RD_STA_DONE 	BIT2
#define SPI_SLV_WR_BUF_DONE 	BIT1
#define SPI_SLV_RD_BUF_DONE 	BIT0

#define SPI_SLAVE1(i)				(REG_SPI_BASE(i)  + 0x34)
#define SPI_SLV_STATUS_BITLEN		0x0000001F
#define SPI_SLV_STATUS_BITLEN_S		27
#define SPI_SLV_BUF_BITLEN			0x000001FF
#define SPI_SLV_BUF_BITLEN_S 		16
#define SPI_SLV_RD_ADDR_BITLEN 		0x0000003F
#define SPI_SLV_RD_ADDR_BITLEN_S 	10
#define SPI_SLV_WR_ADDR_BITLEN 		0x0000003F
#define SPI_SLV_WR_ADDR_BITLEN_S 	4

#define SPI_SLV_WRSTA_DUMMY_EN 		BIT3
#define SPI_SLV_RDSTA_DUMMY_EN 		BIT2
#define SPI_SLV_WRBUF_DUMMY_EN 		BIT1
#define SPI_SLV_RDBUF_DUMMY_EN 		BIT0

#define SPI_SLAVE2(i)					(REG_SPI_BASE(i)  + 0x38)
#define SPI_SLV_WRBUF_DUMMY_CYCLELEN	0X000000FF
#define SPI_SLV_WRBUF_DUMMY_CYCLELEN_S	24
#define SPI_SLV_RDBUF_DUMMY_CYCLELEN	0X000000FF
#define SPI_SLV_RDBUF_DUMMY_CYCLELEN_S	16
#define SPI_SLV_WRSTR_DUMMY_CYCLELEN	0X000000FF
#define SPI_SLV_WRSTR_DUMMY_CYCLELEN_S	8
#define SPI_SLV_RDSTR_DUMMY_CYCLELEN	0x000000FF
#define SPI_SLV_RDSTR_DUMMY_CYCLELEN_S	0

#define SPI_SLAVE3(i)					(REG_SPI_BASE(i)  + 0x3C)
#define SPI_SLV_WRSTA_CMD_VALUE			0x000000FF
#define SPI_SLV_WRSTA_CMD_VALUE_S		24
#define SPI_SLV_RDSTA_CMD_VALUE			0x000000FF
#define SPI_SLV_RDSTA_CMD_VALUE_S		16
#define SPI_SLV_WRBUF_CMD_VALUE			0x000000FF
#define SPI_SLV_WRBUF_CMD_VALUE_S		8
#define SPI_SLV_RDBUF_CMD_VALUE 		0x000000FF
#define SPI_SLV_RDBUF_CMD_VALUE_S		0

#define SPI_W0(i) 						(REG_SPI_BASE(i) +0x40)
#define SPI_W1(i) 						(REG_SPI_BASE(i) +0x44)
#define SPI_W2(i) 						(REG_SPI_BASE(i) +0x48)
#define SPI_W3(i) 						(REG_SPI_BASE(i) +0x4C)
#define SPI_W4(i) 						(REG_SPI_BASE(i) +0x50)
#define SPI_W5(i) 						(REG_SPI_BASE(i) +0x54)
#define SPI_W6(i) 						(REG_SPI_BASE(i) +0x58)
#define SPI_W7(i) 						(REG_SPI_BASE(i) +0x5C)
#define SPI_W8(i) 						(REG_SPI_BASE(i) +0x60)
#define SPI_W9(i) 						(REG_SPI_BASE(i) +0x64)
#define SPI_W10(i) 						(REG_SPI_BASE(i) +0x68)
#define SPI_W11(i) 						(REG_SPI_BASE(i) +0x6C)
#define SPI_W12(i) 						(REG_SPI_BASE(i) +0x70)
#define SPI_W13(i) 						(REG_SPI_BASE(i) +0x74)
#define SPI_W14(i) 						(REG_SPI_BASE(i) +0x78)
#define SPI_W15(i) 						(REG_SPI_BASE(i) +0x7C)

#define SPI_EXT2(i)                           (REG_SPI_BASE(i)  + 0xF8)

#define SPI_EXT3(i)                           (REG_SPI_BASE(i)  + 0xFC)
#define SPI_INT_HOLD_ENA 0x00000003
#define SPI_INT_HOLD_ENA_S 0

/******************************************************************************/
//Generated at 2014-07-29 11:03:29
/*
 *  Copyright (c) 2010 - 2011 Espressif System
 *
 */


#define SPI_FLASH_CMD(i)                            (REG_SPI_BASE(i)  + 0x0)
#define SPI_FLASH_READ					BIT31
#define SPI_FLASH_WREN					BIT30
#define SPI_FLASH_WRDI					BIT29
#define SPI_FLASH_RDID					BIT28
#define SPI_FLASH_RDSR					BIT27
#define SPI_FLASH_WRSR					BIT26
#define SPI_FLASH_PP					BIT25
#define SPI_FLASH_SE					BIT24
#define SPI_FLASH_BE					BIT23
#define SPI_FLASH_CE					BIT22
#define SPI_FLASH_DP					BIT21
#define SPI_FLASH_RES					BIT20
#define SPI_FLASH_HPM					BIT19
#define SPI_FLASH_USR					BIT18

#define SPI_FLASH_ADDR(i)				(REG_SPI_BASE(i)  + 0x4)

#define SPI_FLASH_CTRL(i)				(REG_SPI_BASE(i)  + 0x8)
#define SPI_WR_BIT_ODER					BIT26
#define SPI_RD_BIT_ODER					BIT25
#define SPI_QIO_MODE					BIT24
#define SPI_DIO_MODE					BIT23
#define SPI_TWO_BYTE_STATUS_EN			BIT22
#define SPI_WP_REG						BIT21
#define SPI_QOUT_MODE					BIT20
#define SPI_SHARE_BUS					BIT19
#define SPI_HOLD_MODE					BIT18
#define SPI_ENABLE_AHB					BIT17
#define SPI_SST_AAI						BIT16
#define SPI_RESANDRES					BIT15
#define SPI_DOUT_MODE					BIT14
#define SPI_FASTRD_MODE					BIT13

#define SPI_FLASH_CTRL1(i)				(REG_SPI_BASE (i) + 0xC)
#define SPI_T_CSH						0x0000000F
#define SPI_T_CSH_S						28
#define SPI_T_RES						0x00000FFF
#define SPI_T_RES_S						16
#define SPI_BUS_TIMER_LIMIT				0x0000FFFF
#define SPI_BUS_TIMER_LIMIT_S			0

#define SPI_FLASH_STATUS(i)				(REG_SPI_BASE(i)  + 0x10)
#define SPI_STATUS_EXT					0x000000FF
#define SPI_STATUS_EXT_S				24
#define SPI_WB_MODE						0x000000FF
#define SPI_WB_MODE_S					16
#define SPI_FLASH_STATUS_PRO_FLAG		BIT7
#define SPI_FLASH_TOP_BOT_PRO_FLAG		BIT5
#define SPI_FLASH_BP2					BIT4
#define SPI_FLASH_BP1					BIT3
#define SPI_FLASH_BP0					BIT2
#define SPI_FLASH_WRENABLE_FLAG			BIT1
#define SPI_FLASH_BUSY_FLAG				BIT0

#define SPI_FLASH_CTRL2(i)				(REG_SPI_BASE(i)  + 0x14)
#define SPI_CS_DELAY_NUM				0x0000000F
#define SPI_CS_DELAY_NUM_S				28
#define SPI_CS_DELAY_MODE				0x00000003
#define SPI_CS_DELAY_MODE_S				26
#define SPI_MOSI_DELAY_NUM				0x00000007
#define SPI_MOSI_DELAY_NUM_S			23
#define SPI_MOSI_DELAY_MODE				0x00000003
#define SPI_MOSI_DELAY_MODE_S			21
#define SPI_MISO_DELAY_NUM				0x00000007
#define SPI_MISO_DELAY_NUM_S			18
#define SPI_MISO_DELAY_MODE				0x00000003
#define SPI_MISO_DELAY_MODE_S			16
#define SPI_CK_OUT_HIGH_MODE			0x0000000F
#define SPI_CK_OUT_HIGH_MODE_S			12
#define SPI_CK_OUT_LOW_MODE				0x0000000F
#define SPI_CK_OUT_LOW_MODE_S			8
#define SPI_HOLD_TIME					0x0000000F
#define SPI_HOLD_TIME_S					4
#define SPI_SETUP_TIME					0x0000000F
#define SPI_SETUP_TIME_S				0

#define SPI_FLASH_CLOCK(i)				(REG_SPI_BASE(i)  + 0x18)
#define SPI_CLKDIV_PRE					0x00001FFF
#define SPI_CLKDIV_PRE_S				18
#define SPI_CLKCNT_N					0x0000003F
#define SPI_CLKCNT_N_S					12
#define SPI_CLKCNT_H					0x0000003F
#define SPI_CLKCNT_H_S 6
#define SPI_CLKCNT_L					0x0000003F
#define SPI_CLKCNT_L_S					0

#define SPI_FLASH_USER(i)				(REG_SPI_BASE(i)  + 0x1C)
#define SPI_FLASH_USR_ADDR				BIT30
#define SPI_FLASH_USR_DUMMY				BIT29
#define SPI_FLASH_USR_DIN				BIT28
#define SPI_FLASH_DOUT					BIT27
#define SPI_USR_DUMMY_IDLE				BIT26
#define SPI_USR_DOUT_HIGHPART			BIT25
#define SPI_USR_DIN_HIGHPART			BIT24
#define SPI_USR_PREP_HOLD				BIT23
#define SPI_USR_CMD_HOLD				BIT22
#define SPI_USR_ADDR_HOLD				BIT21
#define SPI_USR_DUMMY_HOLD				BIT20
#define SPI_USR_DIN_HOLD				BIT19
#define SPI_USR_DOUT_HOLD				BIT18
#define SPI_USR_HOLD_POL				BIT17
#define SPI_AHB_ENDIAN_MODE				0x00000003
#define SPI_AHB_ENDIAN_MODE_S			8
#define SPI_AHB_USR_COMMAND				BIT3
#define SPI_AHB_USR_COMMAND_4BYTE		BIT1
#define SPI_DOUTDIN						BIT0

#define SPI_FLASH_USER1(i)				(REG_SPI_BASE(i) + 0x20)
#define SPI_USR_ADDR_BITLEN				0x0000003F
#define SPI_USR_ADDR_BITLEN_S			26
#define SPI_USR_OUT_BITLEN				0x000001FF
#define SPI_USR_OUT_BITLEN_S			17
#define SPI_USR_DIN_BITLEN				0x000001FF
#define SPI_USR_DIN_BITLEN_S			8
#define SPI_USR_DUMMY_CYCLELEN			0x000000FF
#define SPI_USR_DUMMY_CYCLELEN_S		0

#define SPI_FLASH_USER2(i)				(REG_SPI_BASE(i)  + 0x24)
#define SPI_USR_COMMAND_BITLEN			0x0000000F
#define SPI_USR_COMMAND_BITLEN_S		28
#define SPI_USR_COMMAND_VALUE			0x0000FFFF
#define SPI_USR_COMMAND_VALUE_S			0

#define SPI_FLASH_USER3(i)				(REG_SPI_BASE(i)  + 0x28)
#define SPI_FLASH_PIN(i)				(REG_SPI_BASE(i)  + 0x2C)
#define SPI_FLASH_SLAVE(i)				(REG_SPI_BASE(i)  + 0x30)
#define SPI_TRANS_CNT					0x0000000F
#define SPI_TRANS_CNT_S					23
#define SPI_SLV_LAST_STATE				0x00000007
#define SPI_SLV_LAST_STATE_S			20
#define SPI_SLV_LAST_COMMAND			0x00000007
#define SPI_SLV_LAST_COMMAND_S			17
#define SPI_CS_I_MODE					0x00000003
#define SPI_CS_I_MODE_S					10
#define SPI_INT_EN						0x0000001F
#define SPI_INT_EN_S					5

#define SPI_FLASH_SLAVE1(i)				(REG_SPI_BASE(i)  + 0x34)
#define SPI_SLV_STATUS_BITLEN			0x0000001F
#define SPI_SLV_STATUS_BITLEN_S			27
#define SPI_SLV_STATUS_FAST_EN			BIT26
#define SPI_SLV_STATUS_READBACK			BIT25
#define SPI_SLV_BUF_BITLEN				0x000001FF
#define SPI_SLV_BUF_BITLEN_S			16
#define SPI_SLV_RD_ADDR_BITLEN			0x0000003F
#define SPI_SLV_RD_ADDR_BITLEN_S		10
#define SPI_SLV_WR_ADDR_BITLEN			0x0000003F
#define SPI_SLV_WR_ADDR_BITLEN_S		4

#define SPI_FLASH_SLAVE2(i)				(REG_SPI_BASE(i)  + 0x38)
#define SPI_SLV_WRSTA_DUMMY_CYCLELEN	0x000000FF
#define SPI_SLV_WRSTA_DUMMY_CYCLELEN_S	8
#define SPI_SLV_RDSTA_DUMMY_CYCLELEN	0x000000FF
#define SPI_SLV_RDSTA_DUMMY_CYCLELEN_S	0

#define SPI_FLASH_SLAVE3(i)				(REG_SPI_BASE(i)  + 0x3C)
#define SPI_SLV_WRSTA_CMD_VALUE			0x000000FF
#define SPI_SLV_WRSTA_CMD_VALUE_S		24
#define SPI_SLV_RDSTA_CMD_VALUE			0x000000FF
#define SPI_SLV_RDSTA_CMD_VALUE_S		16
#define SPI_SLV_WRBUF_CMD_VALUE			0x000000FF
#define SPI_SLV_WRBUF_CMD_VALUE_S		8
#define SPI_SLV_RDBUF_CMD_VALUE			0x000000FF
#define SPI_SLV_RDBUF_CMD_VALUE_S		0

#define SPI_FLASH_C0(i)					(REG_SPI_BASE(i) +0x40)
#define SPI_FLASH_C1(i)					(REG_SPI_BASE(i) +0x44)
#define SPI_FLASH_C2(i)					(REG_SPI_BASE(i) +0x48)
#define SPI_FLASH_C3(i)					(REG_SPI_BASE(i) +0x4C)
#define SPI_FLASH_C4(i)					(REG_SPI_BASE(i) +0x50)
#define SPI_FLASH_C5(i)					(REG_SPI_BASE(i) +0x54)
#define SPI_FLASH_C6(i)					(REG_SPI_BASE(i) +0x58)
#define SPI_FLASH_C7(i)					(REG_SPI_BASE(i) +0x5C)

#define SPI_FLASH_EXT0(i)				(REG_SPI_BASE(i)  + 0xF0)
#define SPI_T_PP_ENA					BIT31
#define SPI_T_PP_SHIFT					0x0000000F
#define SPI_T_PP_SHIFT_S				16
#define SPI_T_PP_TIME					0x00000FFF
#define SPI_T_PP_TIME_S					0

#define SPI_FLASH_EXT1(i)				(REG_SPI_BASE(i)  + 0xF4)
#define SPI_T_ERASE_ENA					BIT31
#define SPI_T_ERASE_SHIFT				0x0000000F
#define SPI_T_ERASE_SHIFT_S				16
#define SPI_T_ERASE_TIME				0x00000FFF
#define SPI_T_ERASE_TIME_S				0

#define SPI_FLASH_EXT2(i)				(REG_SPI_BASE(i)  + 0xF8)
#define SPI_ST							0x00000007
#define SPI_ST_S						0

#define SPI_FLASH_EXT3(i)				(REG_SPI_BASE(i)  + 0xFC)
#define SPI_INT_HOLD_ENA				0x00000003
#define SPI_INT_HOLD_ENA_S				0


#define SPI_CS_DELAY_INT                   0x0000000F // кол-во тактов SPI-CLK задержки до выставления прерывания и готовности
#define SPI_CS_DELAY_INT_S               28



/******************************************************************************/



#ifndef __I
#define     __I     volatile const   /*!< defines 'read only' permissions     */
#endif	// #ifndef __I

#ifndef __O
#define     __O     volatile         /*!< defines 'write only' permissions    */
#endif	// #ifndef __O

#ifndef __IO
#define     __IO    volatile         /*!< defines 'read / write' permissions  */
#endif	// #ifndef __IO



/**************** SPI Defined bit fields **************************************/

typedef struct
{
	__IO u32		dummy0	:12;
	__IO unsigned	dummy12	:1;	// BIT12 = 1 ???
	__IO unsigned	dummy13	:1;	// BIT13 = 1 ???
	__IO unsigned	dummy14	:1;	// BIT14
	__IO unsigned	dummy15	:1;	// BIT15
	__IO unsigned	dummy16	:1;	// BIT16
	__IO unsigned	dummy17	:1;	// BIT17
	__IO unsigned	usr		:1;	// BIT18 spi module busy - transmission start
	__IO unsigned	hpm		:1;	// BIT19
	__IO unsigned	res		:1;	// BIT20
	__IO unsigned	dp		:1;	// BIT21
	__IO unsigned	ce		:1;	// BIT22
	__IO unsigned	be		:1;	// BIT23
	__IO unsigned	se		:1;	// BIT24
	__IO unsigned	pp		:1;	// BIT25
	__IO unsigned	wrsr	:1;	// BIT26
	__IO unsigned	rdsr	:1;	// BIT27
	__IO unsigned	rdid	:1;	// BIT28
	__IO unsigned	wrdi	:1;	// BIT29
	__IO unsigned	wren	:1;	// BIT30
	__IO unsigned	read	:1;	// BIT31
} spi_cmd_t;

typedef struct
{
	__IO unsigned	dummy0			:12;
	__IO unsigned	dummy12			:1;	// BIT12=1 ???
	__IO unsigned	fastrd_mode		:1;	// BIT13
	__IO unsigned	dout_mode		:1;	// BIT14
	__IO unsigned	res_and_res		:1;	// BIT15
	__IO unsigned	sst_aai			:1;	// BIT16
	__IO unsigned	enable_ahb		:1;	// BIT17
	__IO unsigned	hold_mode		:1;	// BIT18
	__IO unsigned	share_bus		:1;	// BIT19
	__IO unsigned	qout_mode		:1;	// BIT20
	__IO unsigned	wp_reg			:1;	// BIT21
	__IO unsigned	two_byte_status_en	:1;	// BIT22
	__IO unsigned	dio_mode		:1;	// BIT23
	__IO unsigned	qio_mode		:1;	// BIT24
	__IO unsigned	rd_bit_order	:1;	// BIT25 порядок бит внутри байта 0-MSB first 1-LSB first
	__IO unsigned	wr_bit_order	:1;	// BIT26 порядок бит внутри байта 0-MSB first 1-LSB first
	__IO unsigned	dummy27			:5;
} spi_ctrl_t;

typedef struct
{
	__IO unsigned	bus_timer_limit		:16;
	__IO unsigned	cs_hold_delay_res	:12;
	__IO unsigned	cs_hold_delay		:4;	// кол-во тактов SPI-CLK задержки до выставления прерывания и готовности
} spi_ctrl1_t;

typedef struct
{
	__IO unsigned	wr_busy				:1;	//
	__IO unsigned	rd_empty			:1; //
	__IO unsigned	comm_cnt			:3;
	__IO unsigned	top_bot_pro_flag	:1;
	__IO unsigned	dummy6				:1;
	__IO unsigned	status_pro_flag		:1;
	__IO unsigned	dummy8				:8;
	__IO unsigned	wb_mode				:8;
	__IO unsigned	status_ext			:8;
} spi_rd_status_t;

typedef struct
{
	__IO unsigned	setup_time			:4;	// 0x1 
	__IO unsigned	hold_time			:4;	// 0x1
	__IO unsigned	ck_out_low_mode		:4;
	__IO unsigned	cl_out_high_mode	:4;	// bit 12-15 SPI_CTRL2_CLK_HDIV2 BIT12 Уменьшает длительность "1" на CLK в 2 раза
	__IO unsigned	miso_delay_mode		:2;	// bit 16-17 SPI_CTRL2_CLK_HDIV4 BIT15 Уменьшает длительность "1" на CLK в 4 раза
	__IO unsigned	miso_delay_num		:3;	// bit 18-20
	__IO unsigned	mosi_delay_mode		:2;	// bit 21-22 = 1(без команды и адреса)
	__IO unsigned	mosi_delay_num		:3;	// bit 23-25	//:3;!!!
	__IO unsigned	cs_delay_mode		:2;	// bit 26-27
	__IO unsigned	cs_delay_num		:4;	// bit 28-31
} spi_ctrl2_t;

typedef struct
{
	__IO unsigned	clkcnt_l	:6;	// In the master mode, it must be equal to spi_clkcnt_N
	__IO unsigned	clkcnt_h	:6;	// In the master mode, it must be floor((spi_clkcnt_N+1)/2-1)
	__IO unsigned	clkcnt_n	:6;	// In the master mode, it is the divider of spi_clk. 
									// So spi_clk frequency is 80MHz/(spi_clkdiv_pre+1)/(spi_clkcnt_N+1)
	__IO unsigned	pre_s		:13;
	__IO unsigned	equ_sysclk	:1;
} spi_clock_t;

typedef struct
{
	__IO unsigned	dout_din			:1;	// bit0 двухсторонний обмен данными ???
	__IO unsigned	ahb_command_4byte	:1;	// bit1
	__IO unsigned	flash_mode			:1;	// bit2 flash operation mode 0 - disable 1 - enable
	__IO unsigned	ahb_command			:1;
	__IO unsigned	cs_hold				:1;	// bit4 после данных cs останется на 1 такт низким
	__IO unsigned	cs_setup			:1;	// bit5 после установки cs будет пауза 3 такта перед данными
	__IO unsigned	ck_in_edge			:1;	// bit6 полярность CLK: 0 - данные по фронту, 1 - по спаду
	__IO unsigned	ck_out_edge			:1;	// bit7 полярность CLK: 0 - данные по фронту, 1 - по спаду
	__IO unsigned	ahb_endian_mode		:2;
	__IO unsigned	rd_byte_order		:1;	// bit10 порядок передачи байт: 0-начиная с младшего, 1-начиная со старшего байта
	__IO unsigned	wr_byte_order		:1;	// bit11 порядок передачи байт: 0-начиная с младшего, 1-начиная со старшего байта
	__IO unsigned	fwrite_dual			:1;	// bit12
	__IO unsigned	fwrite_quad			:1;	// bit13
	__IO unsigned	fwrite_dio			:1;	// bit14
	__IO unsigned	fwrite_qio			:1;	// bit15
	__IO unsigned	sio					:1;	// bit16 = 1 для передачи slave 
	__IO unsigned	hold_pol			:1;
	__IO unsigned	dout_hold			:1;
	__IO unsigned	din_hold			:1;
	__IO unsigned	dummy_hold			:1;
	__IO unsigned	addr_hold			:1;
	__IO unsigned	cmd_hold			:1;
	__IO unsigned	prep_hold			:1;
	__IO unsigned	din_highpart		:1;	// bit24 in DATA BUFFER IN w8-w15
	__IO unsigned	dout_highpart		:1;	// bit25 out DATA BUFFER IN w8-w15
	__IO unsigned	dummy_idle			:1;
	__IO unsigned	mosi				:1;	// bit27 передача данных --
	__IO unsigned	miso				:1;	// bit28 приём данных
	__IO unsigned	dummy				:1;	// bit29 пауза
	__IO unsigned	addr				:1;	// bit30 адрес
	__IO unsigned	command				:1;	// bit31 команда -- биты разрешения посылок SPI
} spi_user_t;

typedef struct
{
	__IO unsigned	dummy_cyclelen	:8;		// dummy cycle length - 1
	__IO unsigned	miso_bitlen 	:9;		// MISO bit length - 1
	__IO unsigned	mosi_bitlen 	:9;		// MOSI bit length - 1 Для управления пакетной передачей
	__IO unsigned	addr_bitlen 	:6;		// ADDR bit length - 1
} spi_user1_t;

typedef struct
{
	__IO unsigned	command_value	:16;	// bit15-0 is cmd value.
	__IO unsigned	dummy16			:12;
	__IO unsigned	command_bitlen	:4;		// bit28-31 is cmd length, cmd bit length is value(0-15)+1
} spi_user2_t;

typedef struct
{
	__IO unsigned	cs0_dis		:1;	// запрещение использовать выводы CS0-CS2 для выбора SPI памяти
	__IO unsigned	cs1_dis		:1;	// при SPI overlap (наверное, не только)
	__IO unsigned	cs2_dis		:1;	// разрешить один из трёх
	__IO unsigned	dummy3		:1;	// BIT3
	__IO unsigned	dummy4		:1;
	__IO unsigned	clk_dis		:1; // Выход CLK = "0"
	__IO unsigned	cs_inv		:1; // инверсия сигнала CS
	__IO unsigned	dummy7		:1;
	__IO unsigned	dummy8		:1;
	__IO unsigned	dummy9		:1;
	__IO unsigned	dummy10		:1;
	__IO unsigned	cs_clk		:1; // сигнал на CS = CLK
	__IO unsigned	dummy12		:1;
	__IO unsigned	dummy13		:1;
	__IO unsigned	dummy14		:1;
	__IO unsigned	dummy15		:1;
	__IO unsigned	mosi_miso	:1; // сигнал MISO выводится на MOSI 0-GPIO12=MOSI 1-GPIO12=MISO
	__IO unsigned	dummy17		:1;
	__IO unsigned	dummy18		:1;
	__IO unsigned	clk1_cs0	:1; // CS = "0", CLK = "1"
	__IO unsigned	dummy20		:1;
	__IO unsigned	dummy21		:1;
	__IO unsigned	dummy22		:1;
	__IO unsigned	dummy23		:1;
	__IO unsigned	dummy24		:1;
	__IO unsigned	dummy25		:1;
	__IO unsigned	dummy26		:1;
	__IO unsigned	dummy27		:1;
	__IO unsigned	dummy28		:1;
	__IO unsigned	clk_inv		:1;	// стабильное положение CLK 0-0 1-1, фронты не меняются
	__IO unsigned	cs_0		:1; // CS = "0"
	__IO unsigned	dummy31		:1;
} spi_pin_t;

typedef struct
{
	__IO unsigned	rd_buf_done			:1;	// bit0 статус прерывания по заполнению буфера
	__IO unsigned	wr_buf_done			:1;	// bit1 статус прерывания по опустошению буфера
	__IO unsigned	rd_done				:1;	// bit2 статус прерывания по чтению одного цикла SPI_SLV_RD_STA_DONE
	__IO unsigned	wr_done				:1;	// bit3 статус прерывания по записи одного цикла SPI_SLV_WR_STA_DONE
	__IO unsigned	trans_done			:1;	// bit4 статус прерывания по окончанию любой передачи
	__IO unsigned	rd_buf_done_en		:1;	// bit5 разрешение прерывания по заполнению буфера
	__IO unsigned	wr_buf_done_en		:1;	// bit6 разрешение прерывания по опустошению буфера
	__IO unsigned	rd_done_en			:1;	// bit7 разрешение прерывания по чтению одного цикла
	__IO unsigned	wr_done_en			:1;	// bit8 разрешение прерывания по записи одного цикла
	__IO unsigned	trans_done_en		:1;	// bit9 общее разрешение прерывания по окончанию передачи
	__IO unsigned	cs_i_mode			:2;
	__IO unsigned	dummy12				:5;
	__IO unsigned	last_command		:3;
	__IO unsigned	last_state			:3;
	__IO unsigned	trans_cnt			:4;
	__IO unsigned	cmd_define			:1;
	__IO unsigned	wr_rd_sta_en		:1;	// bit28
	__IO unsigned	wr_rd_buf_en		:1;	// bit29
	__IO unsigned	slave_mode			:1;	// bit30 1-режим SLAVE
	__IO unsigned	sync_reset			:1;	// bit31
} spi_slave_t;

typedef struct
{
	__IO unsigned	rdbuf_dummy_en		:1;	// BIT0
	__IO unsigned	wrbuf_dummy_en		:1;	// BIT1
	__IO unsigned	rdsta_dummy_en		:1;	// BIT2
	__IO unsigned	wrsta_dummy_en		:1;	// BIT3
	__IO unsigned	wr_addr_bitlen		:6;	// длина посылки адреса записи в битах - 1
	__IO unsigned	rd_addr_bitlen		:6;	// длина посылки адреса чтения в битах - 1
	__IO unsigned	buf_bitlen			:9;	// длина буфера в битах - 1
	__IO unsigned	status_readback		:1;	// =0 CHOOSE ACTIVE STATUS REG
	__IO unsigned	status_fast_en		:1;	// BIT26
	__IO unsigned	status_bitlen		:5;	// длина посылки статуса в битах - 1
} spi_slave1_t;

typedef struct
{
	__IO unsigned	rdsta_dummy_cyclelen	:8;
	__IO unsigned	wrsta_dummy_cyclelen	:8;
	__IO unsigned	rdbuf_dummy_cyclelen	:8;
	__IO unsigned	wrbuf_dummy_cyclelen	:8;
} spi_slave2_t;

typedef struct
{
	__IO unsigned	rdbuf_cmd_value 	:8;
	__IO unsigned	wrbuf_cmd_value 	:8;
	__IO unsigned	rdsta_cmd_value 	:8;
	__IO unsigned	wrsta_cmd_value 	:8;
} spi_slave3_t;

typedef struct
{
	__IO unsigned	t_pp_time	:12;
	__IO unsigned	dummy12		:4;
	__IO unsigned	pp_shift	:4;
	__IO unsigned	dummy20		:11;
	__IO unsigned	t_pp_ena	:1;
} spi_ext0_t;

typedef struct
{
	__IO unsigned	erase_time	:12;
	__IO unsigned	dummy12		:4;
	__IO unsigned	erase_shift	:4;
	__IO unsigned	dummy20		:11;
	__IO unsigned	erase_ena	:1;
} spi_ext1_t;

typedef struct
{
	__IO unsigned	st			:3;
	__IO unsigned	dummy2		:29;
} spi_ext2_t;

typedef struct
{
	__IO unsigned	int_priority	:2;	// 1-высокий приоритет 3-низкий при overlap SPI
	__IO unsigned	dummy1			:30;
} spi_ext3_t;

/************** Define SPI struct *********************************************/

typedef struct spi_struct
{
	union							// 0x60000?00
	{
		__IO u32			cmd;
		__IO spi_cmd_t		cmd_bits;
	};
	__IO u32 				addr;	// 0x60000?04
	union							// 0x60000?08
	{
		__IO u32			ctrl;
		__IO spi_ctrl_t		ctrl_bits;
	};
	union							// 0x60000?0C
	{
		__IO u32			ctrl1;
		__IO spi_ctrl1_t	ctrl1_bits;
	};
	union							// 0x60000?10
	{
		__IO u32				rd_status;
		__IO spi_rd_status_t	rd_status_bits;
	};
	union							// 0x60000?14
	{
		__IO u32			ctrl2;
		__IO spi_ctrl2_t	ctrl2_bits;
	};
	union							// 0x60000?18
	{
		__IO u32			clock;
		__IO spi_clock_t	clock_bits;
	};
	union							// 0x60000?1C
	{
		__IO u32			user;
		__IO spi_user_t		user_bits;
	};
	union							// 0x60000?20
	{
		__IO u32			user1;
		__IO spi_user1_t	user1_bits;
	};
	union							// 0x60000?24
	{
		__IO u32			user2;
		__IO spi_user2_t	user2_bits;
	};
	__IO u32			wr_status;	// 0x60000?28
	union							// 0x60000?2C
	{
		__IO u32			pin;
		__IO spi_pin_t		pin_bits;
	};
	union							// 0x60000?30
	{
		__IO u32			slave;
		__IO spi_slave_t	slave_bits;
	};
	union							// 0x60000?34
	{
		__IO u32			slave1;
		__IO spi_slave1_t	slave1_bits;
	};
	union							// 0x60000?38
	{
		__IO u32			slave2;
		__IO spi_slave2_t	slave2_bits;
	};
	union							// 0x60000?3C
	{
		__IO u32			slave3;
		__IO spi_slave3_t	slave3_bits;
	};
	__IO u32 w0;					// 0x60000?40
	__IO u32 w1;					// 0x60000?44
	__IO u32 w2;					// 0x60000?48
	__IO u32 w3;					// 0x60000?4C
	__IO u32 w4;					// 0x60000?50
	__IO u32 w5;					// 0x60000?54
	__IO u32 w6;					// 0x60000?58
	__IO u32 w7;					// 0x60000?5C
	__IO u32 w8;					// 0x60000?60
	__IO u32 w9;					// 0x60000?64
	__IO u32 w10;					// 0x60000?68
	__IO u32 w11;					// 0x60000?6C
	__IO u32 w12;					// 0x60000?70
	__IO u32 w13;					// 0x60000?74
	__IO u32 w14;					// 0x60000?78
	__IO u32 w15;					// 0x60000?7C
	__IO u32 dummy80;				// 0x60000?80
	__IO u32 dummy84;				// 0x60000?84
	__IO u32 dummy88;				// 0x60000?88
	__IO u32 dummy8C;				// 0x60000?8C
	__IO u32 dummy90;				// 0x60000?90
	__IO u32 dummy94;				// 0x60000?94
	__IO u32 dummy98;				// 0x60000?98
	__IO u32 dummy9C;				// 0x60000?9C
	__IO u32 dummyA0;				// 0x60000?A0
	__IO u32 dummyA4;				// 0x60000?A4
	__IO u32 dummyA8;				// 0x60000?A8
	__IO u32 dummyAC;				// 0x60000?AC
	__IO u32 dummyB0;				// 0x60000?B0
	__IO u32 dummyB4;				// 0x60000?B4
	__IO u32 dummyB8;				// 0x60000?B8
	__IO u32 dummyBC;				// 0x60000?BC
	__IO u32 dummyC0;				// 0x60000?C0
	__IO u32 dummyC4;				// 0x60000?C4
	__IO u32 dummyC8;				// 0x60000?C8
	__IO u32 dummyCC;				// 0x60000?CC
	__IO u32 dummyD0;				// 0x60000?D0
	__IO u32 dummyD4;				// 0x60000?D4
	__IO u32 dummyD8;				// 0x60000?D8
	__IO u32 dummyDC;				// 0x60000?DC
	__IO u32 dummyE0;				// 0x60000?E0
	__IO u32 dummyE4;				// 0x60000?E4
	__IO u32 dummyE8;				// 0x60000?E8
	__IO u32 dummyEC;				// 0x60000?EC
	union							// 0x60000?F0
	{
		__IO u32		ext0;
		__IO spi_ext0_t	ext0_bits;
	};
	union							// 0x60000?F4
	{
		__IO u32		ext1;
		__IO spi_ext1_t	ext1_bits;
	};
	union							// 0x60000?F8
	{
		__IO u32		ext2;
		__IO spi_ext2_t	ext2_bits;
	};
	union							// 0x60000?FC
	{
		__IO u32		ext3;
		__IO spi_ext3_t	ext3_bits;
	};
} SPI_TypeDef;

#define SPI0_BaseAddress			0x60000200
#define SPI1_BaseAddress			0x60000100

#define SPI0 		((SPI_TypeDef *) SPI0_BaseAddress)
#define SPI1 		((SPI_TypeDef *) SPI1_BaseAddress)

//#define SPI 		((SPI_TypeDef *) SPI0_BaseAddress)
//#define HSPI		((SPI_TypeDef *) SPI1_BaseAddress)

/***************** define registers reset value *******************************/

#define SPI_CMD_RESET_VALUE			((u32)0x00000000)
#define SPI_ADDR_RESET_VALUE		((u32)0x00000000)
#define SPI_CTRL_RESET_VALUE		((u32)0x00001000)
#define SPI_CTRL1_RESET_VALUE		((u32)0x00000000)
#define SPI_RD_STATUS_RESET_VALUE	((u32)0x00000000)
#define SPI_CTRL2_RESET_VALUE		((u32)0x00000011)
#define SPI_CLOCK_RESET_VALUE		((u32)0x80003023)
#define SPI_USER_RESET_VALUE		((u32)0x80000020)
#define SPI_USER1_RESET_VALUE		((u32)0x3C000000)
#define SPI_USER2_RESET_VALUE		((u32)0x70000000)
#define SPI_WR_STATUS_RESET_VALUE	((u32)0x00000000)
#define SPI_PIN_RESET_VALUE			((u32)0x0000001E)
#define SPI_SLAVE_RESET_VALUE		((u32)0x00000200)
#define SPI_SLAVE1_RESET_VALUE		((u32)0x00000000)
#define SPI_SLAVE2_RESET_VALUE		((u32)0x00000000)
#define SPI_SLAVE3_RESET_VALUE		((u32)0x00000000)
#define SPI_W0_RESET_VALUE			((u32)0x00000000)
#define SPI_W1_RESET_VALUE			((u32)0x00000000)
#define SPI_W2_RESET_VALUE			((u32)0x00000000)
#define SPI_W3_RESET_VALUE			((u32)0x00000000)
#define SPI_W4_RESET_VALUE			((u32)0x00000000)
#define SPI_W5_RESET_VALUE			((u32)0x00000000)
#define SPI_W6_RESET_VALUE			((u32)0x00000000)
#define SPI_W7_RESET_VALUE			((u32)0x00000000)
#define SPI_W8_RESET_VALUE			((u32)0x00000000)
#define SPI_W9_RESET_VALUE			((u32)0x00000000)
#define SPI_W10_RESET_VALUE			((u32)0x00000000)
#define SPI_W11_RESET_VALUE			((u32)0x00000000)
#define SPI_W12_RESET_VALUE			((u32)0x00000000)
#define SPI_W13_RESET_VALUE			((u32)0x00000000)
#define SPI_W14_RESET_VALUE			((u32)0x00000000)
#define SPI_W15_RESET_VALUE			((u32)0x00000000)

/******************* define SPI bit masks *************************************/ 
#define SPI_CMD_USR					((u32)0x00040000)	// BIT18
#define SPI_CMD_RES					((u32)0x00100000)	// BIT20
#define SPI_CMD_CE					((u32)0x00400000)	// BIT22
#define SPI_CMD_BE					((u32)0x00800000)	// BIT23
#define SPI_CMD_SE					((u32)0x01000000)	// BIT24
#define SPI_CMD_PP					((u32)0x02000000)	// BIT25
#define SPI_CMD_WRSR				((u32)0x04000000)	// BIT26
#define SPI_CMD_RDSR				((u32)0x08000000)	// BIT27
#define SPI_CMD_RDID				((u32)0x10000000)	// BIT28
#define SPI_CMD_WRDI				((u32)0x20000000)	// BIT29
#define SPI_CMD_WREN				((u32)0x40000000)	// BIT30
#define SPI_CMD_READ				((u32)0x80000000)	// BIT31

#define SPI_CTRL_FASTRD_MODE		((u32)0x00002000)	// BIT13
#define SPI_CTRL_DOUT_MODE			((u32)0x00004000)	// BIT14
#define SPI_CTRL_QOUT_MODE			((u32)0x00100000)	// BIT20
#define SPI_CTRL_DIO_MODE			((u32)0x00800000)	// BIT23
#define SPI_CTRL_QIO_MODE			((u32)0x01000000)	// BIT24
#define SPI_CTRL_RD_BIT_ORDER		((u32)0x02000000)	// BIT25
#define SPI_CTRL_WR_BIT_ORDER		((u32)0x04000000)	// BIT26

#define SPI_CTRL1_CS_HOLD_DELAY_RES_MASK	((u32)0x0FFF0000)
#define SPI_CTRL1_CS_HOLD_DELAY_MASK		((u32)0xF0000000)

#define SPI_RD_STATUS_MASK					((u32)0x000000FF)

#define SPI_CTRL2_CLK_HDIV2					((u32)0x00001000)	// BIT12 Уменьшает длительность "1" на CLK в 2 раза
#define SPI_CTRL2_CLK_HDIV4					((u32)0x00008000)	// BIT15 Уменьшает длительность "1" на CLK в 4 раза
#define SPI_CTRL2_MISO_DELAY_MODE_MASK		((u32)0x00030000)
#define SPI_CTRL2_MISO_DELAY_NUM_MASK		((u32)0x00180000)
#define SPI_CTRL2_MOSI_DELAY_MODE_MASK		((u32)0x00600000)
#define SPI_CTRL2_MOSI_DELAY_NUM_MASK		((u32)0x03800000)
#define SPI_CTRL2_CS_DELAY_MODE_MASK		((u32)0x0C000000)
#define SPI_CTRL2_CS_DELAY_NUM_MASK			((u32)0xF0000000)

// predefined settings for SPI1 clock
#define SPI_PREDIV_CLK_20M		(0)	// 
#define SPI_PREDIV_CLK_10M		(0)	// 
#define SPI_PREDIV_CLK_500K		(2)	// 

#define SPI_DIV_CLK_20M			(3)	// Fspi = 80MHz / (SPI_DIV_CLK_N + 1) = 20MHz
#define SPI_DIV_CLK_10M			(7)
#define SPI_DIV_CLK_500K		(63)

#define SPI_PREDIV_CLK			(SPI_PREDIV_CLK_10M)			// defined predefine value
#define SPI_DIV_CLK_N			(SPI_DIV_CLK_10M)				// defined predefine value
#define SPI_DIV_CLK_H			((SPI_DIV_CLK_N + 1) / 2 - 1)
#define SPI_DIV_CLK_L			(SPI_DIV_CLK_N)				

#define SPI_CLOCK_CLKCNT_L_MASK				((u32)0x0000003F)
#define SPI_CLOCK_CLKCNT_H_MASK				((u32)0x00000FC0)
#define SPI_CLOCK_CLKCNT_N_MASK				((u32)0x0003F000)
#define SPI_CLOCK_CLKDIV_PRE_MASK			((u32)0x7FFC0000)
#define SPI_CLOCK_EQU_SYSCLK				((u32)0x80000000)	// BIT31

#define SPI_USER_DOUTDIN					((u32)0x00000001)	// BIT0
#define SPI_USER_AHB_COMMAND_4BYTE			((u32)0x00000002)	// BIT1
#define SPI_USER_MODE						((u32)0x00000004)	// BIT2
#define SPI_USER_AHB_COMMAND				((u32)0x00000008)	// BIT3
#define SPI_USER_CS_HOLD					((u32)0x00000010)	// BIT4
#define SPI_USER_CS_SETUP					((u32)0x00000020)	// BIT5
#define SPI_USER_CK_IN_EDGE					((u32)0x00000040)	// BIT6
#define SPI_USER_CK_OUT_EDGE				((u32)0x00000080)	// BIT7
#define SPI_USER_AHB_ENDIAN_MODE_MASK		((u32)0x00000300)	// 8-9
#define SPI_USER_RD_BYTE_ORDER				((u32)0x00000400)	// BIT10
#define SPI_USER_WR_BYTE_ORDER				((u32)0x00000800)	// BIT11
#define SPI_USER_FWRITE_DUAL				((u32)0x00001000)	// BIT12
#define SPI_USER_FWRITE_QUAD				((u32)0x00002000)	// BIT13
#define SPI_USER_FWRITE_DIO					((u32)0x00004000)	// BIT14
#define SPI_USER_FWRITE_QIO					((u32)0x00008000)	// BIT15
#define SPI_USER_SIO						((u32)0x00010000)	// BIT16
#define SPI_USER_HOLD_POL					((u32)0x00020000)	// BIT17
#define SPI_USER_DOUT_HOLD					((u32)0x00040000)	// BIT18
#define SPI_USER_DIN_HOLD					((u32)0x00080000)	// BIT19
#define SPI_USER_DUMMY_HOLD					((u32)0x00100000)	// BIT20
#define SPI_USER_ADDR_HOLD					((u32)0x00200000)	// BIT21
#define SPI_USER_CMD_HOLD					((u32)0x00400000)	// BIT22
#define SPI_USER_PREP_HOLD					((u32)0x00800000)	// BIT23
#define SPI_USER_DIN_HIGHPART				((u32)0x01000000)	// BIT24
#define SPI_USER_DOUT_HIGHPART				((u32)0x02000000)	// BIT25
#define SPI_USER_MOSI						((u32)0x08000000)	// BIT27
#define SPI_USER_MISO						((u32)0x10000000)	// BIT28
#define SPI_USER_DUMMY						((u32)0x20000000)	// BIT29
#define SPI_USER_ADDR						((u32)0x40000000)	// BIT30
#define SPI_USER_COMMAND					((u32)0x80000000)	// BIT31

#define SPI_USER1_DUMMY_CYCLELEN_MASK		((u32)0x00000FF)
#define SPI_USER1_MISO_BITLLEN_MASK			((u32)0x0001FF00)
#define SPI_USER1_MOSI_BITLLEN_MASK			((u32)0x03CE0000)
#define SPI_USER1_ADDR_BITLLEN_MASK			((u32)0xFC000000)

#define SPI_USER2_COMMAND_VALUE_MASK		((u32)0x0000FFFF)
#define SPI_USER2_COMMAND_BITLEN_MASK		((u32)0xF0000000)

// SPI_PIN:
#define SPI_PIN_CS0_DIS						((u32)0x00000001)	// BIT0
#define SPI_PIN_CS1_DIS						((u32)0x00000002)	// BIT1
#define SPI_PIN_CS2_DIS						((u32)0x00000004)	// BIT2
#define SPI_PIN_CLK_DIS					((u32)0x00000020)	// BIT5	Выход CLK = "0"
#define SPI_PIN_CS_INV						((u32)0x00000040)	// BIT6	инверсия сигнала CS
#define SPI_PIN_CS_CLK						((u32)0x00000400)	// BIT11 сигнал на CS = CLK
#define SPI_PIN_MOSI_MISO					((u32)0x00010000)	// BIT16 сигнал MISO выводится на MOSI
#define SPI_PIN_CLK1_CS0					((u32)0x00080000)	// BIT19 CS = "0", CLK = "1"
#define SPI_PIN_CLK_INV						((u32)0x20000000)	// BIT29 инверсия сигнала CLK
#define SPI_PIN_CS0							((u32)0x40000000)	// BIT30 CS = "0"
#define SPI_PIN_CS_0						((u32)0x80000000)	// BIT31 CS = "0"

#define SPI_SLAVE_INT_STT_MASK				((u32)0x0000001F)
#define SPI_SLAVE_INT_STT_RD_BUF_DONE		((u32)0x00000001)
#define SPI_SLAVE_INT_STT_WR_BUF_DONE		((u32)0x00000002)	// 
#define SPI_SLAVE_INT_STT_RD_STA_DONE		((u32)0x00000004)	// BIT2
#define SPI_SLAVE_INT_STT_WR_STA_DONE		((u32)0x00000008)	// BIT3
#define SPI_SLAVE_INT_STT_TRANS_DONE		((u32)0x00000010)	// BIT4

#define SPI_SLAVE_INT_EN_MASK				((u32)0x000003E0)
#define SPI_SLAVE_INT_EN_RD_BUF_DONE		((u32)0x00000020)	// BIT5
#define SPI_SLAVE_INT_EN_WR_BUF_DONE		((u32)0x00000040)	// BIT6
#define SPI_SLAVE_INT_EN_RD_STA_DONE		((u32)0x00000080)	// BIT7
#define SPI_SLAVE_INT_EN_WR_STA_DONE		((u32)0x00000100)	// BIT8
#define SPI_SLAVE_INT_EN_TRANS_DONE			((u32)0x00000200)	// BIT9

#define SPI_SLAVE_CS_I_MODE_MASK			((u32)0x00000C00)
#define SPI_SLAVE_LAST_COMMAND_MASK			((u32)0x000E0000)
#define SPI_SLAVE_LAST_STATE_MASK			((u32)0x00300000)
#define SPI_SLAVE_TRANS_CNT_MASK			((u32)0x07800000)
#define SPI_SLAVE_CMD_DEFINE				((u32)0x08000000)	// BIT27
#define SPI_SLAVE_WR_RD_STA_EN				((u32)0x10000000)	// BIT28
#define SPI_SLAVE_WR_RD_BUF_EN				((u32)0x20000000)	// BIT29
#define SPI_SLAVE_SLAVE_MODE				((u32)0x40000000)	// BIT30
#define SPI_SLAVE_SYNC_RESET				((u32)0x80000000)	// BIT31

#define SPI_SLAVE1_RDBUF_DUMMY_EN			((u32)0x00000001)	// BIT0
#define SPI_SLAVE1_WRBUF_DUMMY_EN			((u32)0x00000002)	// BIT1
#define SPI_SLAVE1_RDSTA_DUMMY_EN			((u32)0x00000004)	// BIT2
#define SPI_SLAVE1_WRSTA_DUMMY_EN			((u32)0x00000008)	// BIT3
#define SPI_SLAVE1_WR_ADDR_BITLEN_MASK		((u32)0x000003F0)
#define SPI_SLAVE1_RD_ADDR_BITLEN_MASK		((u32)0x0000FC00)
#define SPI_SLAVE1_BUF_BITLEN_MASK			((u32)0x01FF0000)
#define SPI_SLAVE1_STATUS_BITLEN_MASK		((u32)0xF8000000)

#define SPI_SLAVE2_RDSTA_DUMMY_CYCLELEN_MASK	((u32)0x000000FF)
#define SPI_SLAVE2_WRSTA_DUMMY_CYCLELEN_MASK	((u32)0x0000FF00)
#define SPI_SLAVE2_RDBUF_DUMMY_CYCLELEN_MASK	((u32)0x00FF0000)
#define SPI_SLAVE2_WRBUF_DUMMY_CYCLELEN_MASK	((u32)0xFF000000)

#define SPI_SLAVE3_RDBUF_CMD_VALUE_MASK		((u32)0x000000FF)
#define SPI_SLAVE3_WRBUF_CMD_VALUE_MASK		((u32)0x0000FF00)
#define SPI_SLAVE3_RDSTA_CMD_VALUE_MASK		((u32)0x00FF0000)
#define SPI_SLAVE3_WRSTA_CMD_VALUE_MASK		((u32)0xFF000000)

#endif // SPI_REGISTER_H_INCLUDED
