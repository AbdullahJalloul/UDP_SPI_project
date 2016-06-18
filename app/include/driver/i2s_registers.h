/*
 * i2s_registers.h
 *
 *  Created on: 2.06.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _I2S_REGISTERS_H_
#define _I2S_REGISTERS_H_

#include "c_types.h"

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

/************************ SAR Bitfield ****************************************/

typedef struct
{
	__IO unsigned	tx_reset		:1;
	__IO unsigned	rx_reset		:1;
	__IO unsigned	tx_fifo_reset	:1;
	__IO unsigned	rx_fifo_reset	:1;
	__IO unsigned	trans_slave_mod	:1;
	__IO unsigned	rec_slave_mod	:1;
	__IO unsigned	i2s_right_first	:1;
	__IO unsigned	msb_right		:1;
	__IO unsigned	i2s_tx_start	:1;
	__IO unsigned	i2s_rx_start	:1;
	__IO unsigned	trans_msb_shift	:1;
	__IO unsigned	rec_msb_shift	:1;
	__IO unsigned	clk_div_num		:10;
	__IO unsigned	bck_div_num		:10;
} i2s_cfg_t;

typedef struct
{
	__IO unsigned	rx_take_data	:1;	
	__IO unsigned	tx_put_data		:1;	
	__IO unsigned	rx_wfull		:1;	
	__IO unsigned	rx_rempty		:1;	
	__IO unsigned	tx_wfull		:1;	
	__IO unsigned	tx_rempty		:1;	
	__I unsigned		dummy6			:26;
} i2s_int_t;

typedef struct
{
	__IO unsigned	trans_bck_in_delay	:2;	// bit0-1
	__IO unsigned	trans_ws_in_delay	:2;	// bit2-3
	__IO unsigned	rec_bck_in_delay	:2;	// bit4-5
	__IO unsigned	rec_ws_in_delay	:2;	// bit6-7
	__IO unsigned	rec_sd_in_delay	:2;	// bit8-9
	__IO unsigned	trans_bck_out_delay	:2;	// bit10-11
	__IO unsigned	trans_ws_out_delay	:2;	// bit12-13
	__IO unsigned	trans_sd_out_delay	:2;	// bit14-15
	__IO unsigned	rec_ws_out_delay	:2;	// bit16-17
	__IO unsigned	rec_bck_out_delay	:2;	// bit18-19
	__IO unsigned	trans_dcync_sw		:1;	// bit20
	__IO unsigned	rec_dsync_sw		:1;	// bit21
	__IO unsigned	trans_bck_in_inv	:1;	// bit22
	__I unsigned	dummy23				:9;	// bit23-31
} i2s_timing_t;

typedef struct
{
	__IO unsigned	i2s_rx_data_num		:6;	// bit0-5
	__IO unsigned	i2s_tx_data_num		:6;	// bit6-11
	__IO unsigned	i2s_dcr_en			:1;	// bit12
	__IO unsigned	i2s_tx_fifo_mod		:3;	// bit13-15
	__IO unsigned	i2s_rx_fifo_mod		:3;	// bit16-18
	__I unsigned	dummy19				:13;// bit19-31
} i2s_fifo_cfg_t;

typedef struct
{
	__IO unsigned	tx_chan_mod			:3;	// bit0-2
	__IO unsigned	rx_chan_mod			:3;	// bit3-5
	__I unsigned	dummy6				:26;	// bit6-31
} i2s_cfg_chan_addr_t;

typedef struct
{
	__IO u32	tx_fifo;	// 0x60000E00
	__IO u32	rx_fifo;	// 0x60000E04
	union					// 0x60000E08
	{
		__IO u32		cfg;
		__IO i2s_cfg_t	cfg_bits;
	};
	union					// 0x60000E0C
	{
		__IO u32		int_raw;
		__IO i2s_int_t	int_raw_bits;
	};
	union					// 0x60000E10
	{
		__IO u32		int_st;
		__IO i2s_int_t	int_st_bits;
	};
	union					// 0x60000E14
	{
		__IO u32		int_ena;
		__IO i2s_int_t	int_ena_bits;
	};
	union					// 0x60000E18
	{
		__IO u32		int_clr;
		__IO i2s_int_t	int_clr_bits;
	};
	union					// 0x60000E1C
	{
		__IO u32			timing;
		__IO i2s_timing_t	timing_bits;
	};
	union					// 0x60000E20
	{
		__IO u32			fifo_cfg;
		__IO i2s_fifo_cfg_t	fifo_cfg_bits;
	};
	__IO u32				rx_eof_num;		// 0x60000E24
	__IO u32				single_data;	// 0x60000E28
	union					// 0x60000E2C
	{
		__IO u32					cfg_chan_addr;
		__IO i2s_cfg_chan_addr_t	cfg_chan_addr_bits;
	};
} I2S_TypeDef;

#define I2S_BaseAddress			((u32)0x60000E00)

#define I2S 		((I2S_TypeDef *) SAR_BasI2S_BaseAddresseAddress)



#define	I2SBASEFREQ	(12000000L)

#define DR_REG_I2S_BASE (0x60000e00)

#define I2STXFIFO_ADDRESS  (DR_REG_I2S_BASE + 0x0000)

#define I2SRXFIFO_ADDRESS  (DR_REG_I2S_BASE + 0x0004)

#define I2S_CFG_RESET_MASK 0xf
#define I2S_CFG_TX_RESET (BIT(0))
#define I2S_CFG_RX_RESET (BIT(1))
#define I2S_CFG_TX_FIFO_RESET (BIT(2))
#define I2S_CFG_RX_FIFO_RESET (BIT(3))
#define I2S_CFG_TRANS_SLAVE_MOD (BIT(4))
#define I2S_CFG_RECE_SLAVE_MOD (BIT(5))
#define I2S_CFG_RIGHT_FIRST (BIT(6))
#define I2S_CFG_MSB_RIGHT (BIT(7))
#define I2S_CFG_TX_START (BIT(8))
#define I2S_CFG_RX_START (BIT(9))
#define I2S_CFG_TRANS_MSB_SHIFT (BIT(10))
#define I2S_CFG_RECE_MSB_SHIFT (BIT(11))
#define I2S_CFG_BITS_MOD_S 12
#define I2S_CFG_BITS_MOD 0x0000000F
#define I2S_CFG_CLK_DIV_NUM_S 16
#define I2S_CFG_CLK_DIV_NUM 0x0000003F
#define I2S_CFG_BCK_DIV_NUM_S 22
#define I2S_CFG_BCK_DIV_NUM 0x0000003F


#define I2SINT_RAW_ADDRESS (DR_REG_I2S_BASE + 0x000c)
#define I2S_I2S_RX_TAKE_DATA_INT_RAW (BIT(0))
#define I2S_I2S_TX_PUT_DATA_INT_RAW (BIT(1))
#define I2S_I2S_RX_WFULL_INT_RAW (BIT(2))
#define I2S_I2S_RX_REMPTY_INT_RAW (BIT(3))
#define I2S_I2S_TX_WFULL_INT_RAW (BIT(4))
#define I2S_I2S_TX_REMPTY_INT_RAW (BIT(5))


#define I2SINT_ST_ADDRESS (DR_REG_I2S_BASE + 0x0010)
#define I2S_I2S_TX_REMPTY_INT_ST (BIT(5))
#define I2S_I2S_TX_WFULL_INT_ST (BIT(4))
#define I2S_I2S_RX_REMPTY_INT_ST (BIT(3))
#define I2S_I2S_RX_WFULL_INT_ST (BIT(2))
#define I2S_I2S_TX_PUT_DATA_INT_ST (BIT(1))
#define I2S_I2S_RX_TAKE_DATA_INT_ST (BIT(0))

#define I2SINT_ENA_ADDRESS (DR_REG_I2S_BASE + 0x0014)
#define I2S_I2S_TX_REMPTY_INT_ENA (BIT(5))
#define I2S_I2S_TX_WFULL_INT_ENA (BIT(4))
#define I2S_I2S_RX_REMPTY_INT_ENA (BIT(3))
#define I2S_I2S_RX_WFULL_INT_ENA (BIT(2))
#define I2S_I2S_TX_PUT_DATA_INT_ENA (BIT(1))
#define I2S_I2S_RX_TAKE_DATA_INT_ENA (BIT(0))

#define I2SINT_CLR_ADDRESS (DR_REG_I2S_BASE + 0x0018)
#define I2S_I2S_TX_REMPTY_INT_CLR (BIT(5))
#define I2S_I2S_TX_WFULL_INT_CLR (BIT(4))
#define I2S_I2S_RX_REMPTY_INT_CLR (BIT(3))
#define I2S_I2S_RX_WFULL_INT_CLR (BIT(2))
#define I2S_I2S_PUT_DATA_INT_CLR (BIT(1))
#define I2S_I2S_TAKE_DATA_INT_CLR (BIT(0))

#define I2STIMING_ADDRESS (DR_REG_I2S_BASE + 0x001c)
#define I2S_TRANS_BCK_IN_INV (BIT(22))
#define I2S_RECE_DSYNC_SW (BIT(21))
#define I2S_TRANS_DSYNC_SW (BIT(20))
#define I2S_RECE_BCK_OUT_DELAY 0x00000003
#define I2S_RECE_BCK_OUT_DELAY_S 18
#define I2S_RECE_WS_OUT_DELAY 0x00000003
#define I2S_RECE_WS_OUT_DELAY_S 16
#define I2S_TRANS_SD_OUT_DELAY 0x00000003
#define I2S_TRANS_SD_OUT_DELAY_S 14
#define I2S_TRANS_WS_OUT_DELAY 0x00000003
#define I2S_TRANS_WS_OUT_DELAY_S 12
#define I2S_TRANS_BCK_OUT_DELAY 0x00000003
#define I2S_TRANS_BCK_OUT_DELAY_S 10
#define I2S_RECE_SD_IN_DELAY 0x00000003
#define I2S_RECE_SD_IN_DELAY_S 8
#define I2S_RECE_WS_IN_DELAY 0x00000003
#define I2S_RECE_WS_IN_DELAY_S 6
#define I2S_RECE_BCK_IN_DELAY 0x00000003
#define I2S_RECE_BCK_IN_DELAY_S 4
#define I2S_TRANS_WS_IN_DELAY 0x00000003
#define I2S_TRANS_WS_IN_DELAY_S 2
#define I2S_TRANS_BCK_IN_DELAY 0x00000003
#define I2S_TRANS_BCK_IN_DELAY_S 0

#define I2S_FIFO_CONF_ADDRESS (DR_REG_I2S_BASE + 0x0020)
#define I2S_I2S_RX_FIFO_MOD 0x00000007
#define I2S_I2S_RX_FIFO_MOD_S 16
#define I2S_I2S_TX_FIFO_MOD 0x00000007
#define I2S_I2S_TX_FIFO_MOD_S 13
#define I2S_I2S_DSCR_EN (BIT(12))
#define I2S_I2S_TX_DATA_NUM 0x0000003F
#define I2S_I2S_TX_DATA_NUM_S 6
#define I2S_I2S_RX_DATA_NUM 0x0000003F
#define I2S_I2S_RX_DATA_NUM_S 0


#define I2SRXEOF_NUM_ADDRESS (DR_REG_I2S_BASE + 0x0024)
#define I2S_I2S_RX_EOF_NUM 0xFFFFFFFF
#define I2S_I2S_RX_EOF_NUM_S 0

#define I2SCONF_SIGLE_DATA_ADDRESS (DR_REG_I2S_BASE + 0x0028)
#define I2S_I2S_SIGLE_DATA 0xFFFFFFFF
#define I2S_I2S_SIGLE_DATA_S 0

#define I2SCONF_CHAN_ADDRESS (DR_REG_I2S_BASE + 0x002c)
#define I2S_RX_CHAN_MOD 0x00000003
#define I2S_RX_CHAN_MOD_S 3
#define I2S_TX_CHAN_MOD 0x00000007
#define I2S_TX_CHAN_MOD_S 0


#endif /* _I2S_REGISTERS_H_ */
