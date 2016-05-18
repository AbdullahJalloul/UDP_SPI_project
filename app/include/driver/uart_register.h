/******************************************************************************
 * Copyright 2013-2014 Espressif Systems
 *
 * FileName: uart_register.h
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2015/9/24, v1.0 create this file.
 *  Added on: 17 ма€ 2016 г.
 *      Author: »ль€ ѕетрухин
*******************************************************************************/

#ifndef UART_REGISTER_H_
#define UART_REGISTER_H_

#include "c_types.h"

#define REG_UART_BASE(i)                (0x60000000ul + (i)*0xf00)
//version value:32'h062000

#define UART_FIFO(i)                    (REG_UART_BASE(i) + 0x0)
#define UART_RXFIFO_RD_BYTE                 0x000000FF
#define UART_RXFIFO_RD_BYTE_S               0

#define UART_INT_RAW(i)                 (REG_UART_BASE(i) + 0x4)
#define UART_RXFIFO_TOUT_INT_RAW            BIT8
#define UART_BRK_DET_INT_RAW                BIT7
#define UART_CTS_CHG_INT_RAW                BIT6
#define UART_DSR_CHG_INT_RAW                BIT5
#define UART_RXFIFO_OVF_INT_RAW             BIT4
#define UART_FRM_ERR_INT_RAW                BIT3
#define UART_PARITY_ERR_INT_RAW             BIT2
#define UART_TXFIFO_EMPTY_INT_RAW           BIT1
#define UART_RXFIFO_FULL_INT_RAW            BIT0

#define UART_INT_ST(i)                  (REG_UART_BASE(i) + 0x8)
#define UART_RXFIFO_TOUT_INT_ST             BIT8
#define UART_BRK_DET_INT_ST                 BIT7
#define UART_CTS_CHG_INT_ST                 BIT6
#define UART_DSR_CHG_INT_ST                 BIT5
#define UART_RXFIFO_OVF_INT_ST              BIT4
#define UART_FRM_ERR_INT_ST                 BIT3
#define UART_PARITY_ERR_INT_ST              BIT2
#define UART_TXFIFO_EMPTY_INT_ST            BIT1
#define UART_RXFIFO_FULL_INT_ST             BIT0

#define UART_INT_ENA(i)                 (REG_UART_BASE(i) + 0xC)
#define UART_RXFIFO_TOUT_INT_ENA            BIT8
#define UART_BRK_DET_INT_ENA                BIT7
#define UART_CTS_CHG_INT_ENA                BIT6
#define UART_DSR_CHG_INT_ENA                BIT5
#define UART_RXFIFO_OVF_INT_ENA             BIT4
#define UART_FRM_ERR_INT_ENA                BIT3
#define UART_PARITY_ERR_INT_ENA             BIT2
#define UART_TXFIFO_EMPTY_INT_ENA           BIT1
#define UART_RXFIFO_FULL_INT_ENA            BIT0

#define UART_INT_CLR(i)                 (REG_UART_BASE(i) + 0x10)
#define UART_RXFIFO_TOUT_INT_CLR            BIT8
#define UART_BRK_DET_INT_CLR                BIT7
#define UART_CTS_CHG_INT_CLR                BIT6
#define UART_DSR_CHG_INT_CLR                BIT5
#define UART_RXFIFO_OVF_INT_CLR             BIT4
#define UART_FRM_ERR_INT_CLR                BIT3
#define UART_PARITY_ERR_INT_CLR             BIT2
#define UART_TXFIFO_EMPTY_INT_CLR           BIT1
#define UART_RXFIFO_FULL_INT_CLR            BIT0

#define UART_CLKDIV(i)                  (REG_UART_BASE(i) + 0x14)
#define UART_CLKDIV_CNT                     0x000FFFFF
#define UART_CLKDIV_S                       0

#define UART_AUTOBAUD(i)                (REG_UART_BASE(i) + 0x18)
#define UART_GLITCH_FILT                    0x000000FF
#define UART_GLITCH_FILT_S                  8
#define UART_AUTOBAUD_EN                    BIT0

#define UART_STATUS(i)                  (REG_UART_BASE(i) + 0x1C)
#define UART_TXD                            BIT31
#define UART_RTSN                           BIT30
#define UART_DTRN                           BIT29
#define UART_TXFIFO_CNT                     0x000000FF
#define UART_TXFIFO_CNT_S                   16
#define UART_RXD                            BIT15
#define UART_CTSN                           BIT14
#define UART_DSRN                           BIT13
#define UART_RXFIFO_CNT                     0x000000FF
#define UART_RXFIFO_CNT_S                   0

#define UART_CONF0(i)                   (REG_UART_BASE(i) + 0x20)
#define UART_DTR_INV                        BIT24
#define UART_RTS_INV                        BIT23
#define UART_TXD_INV                        BIT22
#define UART_DSR_INV                        BIT21
#define UART_CTS_INV                        BIT20
#define UART_RXD_INV                        BIT19
#define UART_TXFIFO_RST                     BIT18
#define UART_RXFIFO_RST                     BIT17
#define UART_IRDA_EN                        BIT16
#define UART_TX_FLOW_EN                     BIT15
#define UART_LOOPBACK                       BIT14
#define UART_IRDA_RX_INV                    BIT13
#define UART_IRDA_TX_INV                    BIT12
#define UART_IRDA_WCTL                      BIT11
#define UART_IRDA_TX_EN                     BIT10
#define UART_IRDA_DPLX                      BIT9
#define UART_TXD_BRK                        BIT8
#define UART_SW_DTR                         BIT7
#define UART_SW_RTS                         BIT6
#define UART_STOP_BIT_NUM                   0x00000003
#define UART_STOP_BIT_NUM_S                 4
#define UART_BIT_NUM                        0x00000003
#define UART_BIT_NUM_S                      2
#define UART_PARITY_EN                      BIT1
#define UART_PARITY                         BIT0

#define UART_CONF1(i)                   (REG_UART_BASE(i) + 0x24)
#define UART_RX_TOUT_EN                     BIT31
#define UART_RX_TOUT_THRHD                  0x0000007F
#define UART_RX_TOUT_THRHD_S                24
#define UART_RX_FLOW_EN                     BIT23
#define UART_RX_FLOW_THRHD                  0x0000007F
#define UART_RX_FLOW_THRHD_S                16
#define UART_TXFIFO_EMPTY_THRHD             0x0000007F
#define UART_TXFIFO_EMPTY_THRHD_S           8
#define UART_RXFIFO_FULL_THRHD              0x0000007F
#define UART_RXFIFO_FULL_THRHD_S            0

#define UART_LOWPULSE(i)                (REG_UART_BASE(i) + 0x28)
#define UART_LOWPULSE_MIN_CNT               0x000FFFFF
#define UART_LOWPULSE_MIN_CNT_S             0

#define UART_HIGHPULSE(i)               (REG_UART_BASE(i) + 0x2C)
#define UART_HIGHPULSE_MIN_CNT              0x000FFFFF
#define UART_HIGHPULSE_MIN_CNT_S            0

#define UART_PULSE_NUM(i)               (REG_UART_BASE(i) + 0x30)
#define UART_PULSE_NUM_CNT                  0x0003FF
#define UART_PULSE_NUM_CNT_S                0

#define UART_DATE(i)                    (REG_UART_BASE(i) + 0x78)
#define UART_ID(i)                      (REG_UART_BASE(i) + 0x7C)

#define     __I     volatile const   /*!< defines 'read only' permissions     */
#define     __O     volatile         /*!< defines 'write only' permissions    */
#define     __IO    volatile         /*!< defines 'read / write' permissions  */

// описани€ битовых полей
typedef struct
{
	__I u32		dummy			:24;
	__I u8		rx_fifo_rd_byte	:8;
} uart_fifo_t;

typedef struct
{
	__I u32			dummy			:23;
	__I unsigned	rx_fifo_tout	:1;
	__I unsigned	brk_det			:1;
	__I unsigned	cts_chg			:1;
	__I unsigned	dsr_chg			:1;
	__I unsigned	rx_fifo_ovf		:1;
	__I unsigned	frm_error		:1;
	__I unsigned	parity_err		:1;
	__I unsigned	tx_fifo_empty	:1;
	__I unsigned	rx_fifo_full	:1;
} uart_int_raw_t;

typedef struct
{
	__I u32			dummy			:23;
	__I unsigned	rx_fifo_tout	:1;
	__I unsigned	brk_det			:1;
	__I unsigned	cts_chg			:1;
	__I unsigned	dsr_chg			:1;
	__I unsigned	rx_fifo_ovf		:1;
	__I unsigned	frm_error		:1;
	__I unsigned	parity_err		:1;
	__I unsigned	tx_fifo_empty	:1;
	__I unsigned	rx_fifo_full	:1;
} uart_int_st_t;

typedef struct
{
	__IO u32		dummy			:23;
	__IO unsigned	rx_fifo_tout	:1;
	__IO unsigned	brk_det			:1;
	__IO unsigned	cts_chg			:1;
	__IO unsigned	dsr_chg			:1;
	__IO unsigned	rx_fifo_ovf		:1;
	__IO unsigned	frm_error		:1;
	__IO unsigned	parity_err		:1;
	__IO unsigned	tx_fifo_empty	:1;
	__IO unsigned	rx_fifo_full	:1;
} uart_int_ena_t;

typedef struct
{
	__O u32			dummy			:23;
	__O unsigned	rx_fifo_tout	:1;
	__O unsigned	brk_det			:1;
	__O unsigned	cts_chg			:1;
	__O unsigned	dsr_chg			:1;
	__O unsigned	rx_fifo_ovf		:1;
	__O unsigned	frm_error		:1;
	__O unsigned	parity_err		:1;
	__O unsigned	tx_fifo_empty	:1;
	__O unsigned	rx_fifo_full	:1;
} uart_int_clr_t;

typedef struct
{
	__IO u16		dummy		:12;
	__IO u32		clk_div		:20;
}uart_clk_div_t;

typedef struct
{
	__I u16			dummy		:16;
	__IO u8			glitch_flt	:8;
	__I u8			dummy1		:7;
	__IO unsigned	autobaud_en	:1;
}uart_autobaud_t;

typedef struct
{
	__I unsigned	txd			:1;
	__I unsigned	rtsn		:1;
	__I unsigned	dtrn		:1;
	__I u8			dummy		:5;
	__I u8			tx_fifo_cnt	:8;
	__I unsigned	rxd			:1;
	__I unsigned	ctsn		:1;
	__I unsigned	dsrn		:1;
	__I u8			dummy1		:5;
	__I u8			rx_fifo_cnt	:8;
}uart_status_t;

typedef enum
{
	STOP_1_BIT = 1,
	STOP_1_5_BIT = 2,
	STOP_2_BIT = 3
} uart_stop_bits_t;

typedef enum
{
	NUM_5BITS = 0,
	NUM_6BITS = 1,
	NUM_7BITS = 2,
	NUM_8BITS = 3
} uart_num_bits_t;

typedef struct
{
	__IO u8			dummy			:7;
	__IO unsigned	dyr_inv			:1;
	__IO unsigned	rts_inv			:1;
	__IO unsigned	txd_inv			:1;
	__IO unsigned	dsr_inv			:1;
	__IO unsigned	cts_inv			:1;
	__IO unsigned	rxd_inv			:1;
	__IO unsigned	tx_fifo_rst		:1;
	__IO unsigned	rx_fifo_rst		:1;
	__IO unsigned	irda_en			:1;
	__IO unsigned	tx_flow_en		:1;
	__IO unsigned	loopback		:1;
	__IO unsigned	irda_rx_inv		:1;
	__IO unsigned	irda_tx_inv		:1;
	__IO unsigned	irda_wct		:1;
	__IO unsigned	irda_tx_en		:1;
	__IO unsigned	irda_dplx		:1;
	__IO unsigned	txd_brk			:1;
	__IO unsigned	sw_dtr			:1;
	__IO unsigned	sw_rts			:1;
	__IO uart_stop_bits_t	stop_bits	:2;	// Set stop bit: 1:1bit  2:1.5bits  3:2bits
	__IO uart_num_bits_t	num_bits	:2;	// Set bit num:  0:5bits 1:6bits 2:7bits 3:8bits
	__IO unsigned	parity_en		:1;
	__IO unsigned	parity			:1;
} uart_conf0_t;

typedef struct
{
	__IO unsigned	rx_tout_en			:1;
	__IO u8			rx_tout_trhd		:7;
	__I unsigned	rx_flow_en			:1;
	__IO u8			rx_flow_trhd		:7;
	__I unsigned	dummy				:1;
	__IO u8			tx_fifi_empty_trhd	:7;
	__I unsigned	dummy1				:1;
	__IO u8			rx_fifo_full_trhd	:7;
} uart_conf1_t;

typedef struct
{
	__I u16			dummy				:12;
	__IO u32		lowpulse_min_cnt	:20;
} uart_lowpulse_t;

typedef struct
{
	__I u16			dummy				:12;
	__IO u32		highpulse_min_cnt	:20;
} uart_highpulse_t;

typedef struct
{
	__I u32			dummy			:22;
	__IO u16		rxd_edge_cnt	:10;
} uart_pulse_num_t;

typedef struct UART_struct
{
	union
	{
		__I u32			uart_fifo;
		__I uart_fifo_t	uart_fifo_bits;
	};
	union
	{
		__I u32				uart_int_raw;
		__I uart_int_raw_t	uart_int_raw_bits;
	};
	union
	{
		__I u32				uart_int_st;
		__I uart_int_st_t	uart_int_st_bits;
	};
	union
	{
		__IO u32			uart_int_ena;
		__IO uart_int_ena_t	uart_int_ena_bits;
	};
	union
	{
		__O u32				uart_int_clr;
		__O uart_int_clr_t	uart_int_clr_bits;
	};
	union
	{
		__IO u32			uart_clk_div;
		__IO uart_clk_div_t	uart_clk_div_bits;
	};
	union
	{
		__IO u32				uart_autobaud;
		__IO uart_autobaud_t	uart_autobaud_bits;
	};
	union
	{
		__I u32				uart_status;
		__I uart_status_t	uart_status_bits;
	};
	union
	{
		__IO u32			uart_conf0;
		__IO uart_conf0_t	uart_conf0_bits;
	};
	union
	{
		__IO u32			uart_conf1;
		__IO uart_conf1_t	uart_conf1_bits;
	};
	union
	{
		__I u32				uart_lowpulse;
		__I uart_lowpulse_t	uart_lowpulse_bits;
	};
	union
	{
		__I u32					uart_highpulse;
		__I uart_highpulse_t	uart_highpulse_bits;
	};
	union
	{
		__I u32					uart_pulse_num;
		__I uart_pulse_num_t	uart_pulse_num_bits;
	};
	__I u32		dummy0;
	__I u32		dummy1;
	__I u32		dummy2;
	__I u32		dummy3;
	__I u32		dummy4;
	__I u32		dummy5;
	__I u32		dummy6;
	__I u32		dummy7;
	__I u32		dummy8;
	__I u32		dummy9;
	__I u32		dummy10;
	__I u32		dummy11;
	__I u32		dummy12;
	__I u32		dummy13;
	__I u32		dummy14;
	__I u32		dummy15;
	__I u32		dummy16;
	__IO u32	uart_date;
	__IO u32	uart_id;
} UART_TypeDef;

// описани€ начальных состо€ний
#define UART_FIFO_RESET_VALUE			((u32)0x00000000)
#define UART_INT_RAW_RESET_VALUE		((u32)0x00000000)
#define UART_INT_ST_RESET_VALUE			((u32)0x00000000)
#define UART_INT_ENA_RESET_VALUE		((u32)0x00000000)
#define UART_INT_CLR_RESET_VALUE		((u32)0x00000000)
#define UART_CLK_DIV_RESET_VALUE		((u32)0x00000000)
#define UART_AUTOBAUD_RESET_VALUE		((u32)0x00000A00)
#define UART_STATUS_RESET_VALUE			((u32)0x00000000)
#define UART_CONF0_RESET_VALUE			((u32)0x0000001C)
#define UART_CONF1_RESET_VALUE			((u32)0x00006060)
#define UART_LOWPULSE_RESET_VALUE		((u32)0x000FFFFF)
#define UART_HIGHPULSE_RESET_VALUE		((u32)0x000FFFFF)
#define UART_RXD_CNT_RESET_VALUE		((u32)0x00000000)
#define UART_DATE_RESET_VALUE			((u32)0x00062000)
#define UART_ID_RESET_VALUE				((u32)0x00000500)

// описани€ битовых масок
#define UART_FIFO_RX_RD_BYTE_MASK		((u32)0x000000FF)

#define UART_INT_RAW_RXFIFO_TOUT		((u32)0x00000100)
#define UART_INT_RAW_BRK_DET			((u32)0x00000080)
#define UART_INT_RAW_CTS_CHG			((u32)0x00000040)
#define UART_INT_RAW_DSR_CHG			((u32)0x00000020)
#define UART_INT_RAW_RXFIFO_OVF			((u32)0x00000010)
#define UART_INT_RAW_FRM_ERR			((u32)0x00000008)
#define UART_INT_RAW_PARITY_ERR			((u32)0x00000004)
#define UART_INT_RAW_TXFIFO_EMPTY		((u32)0x00000002)
#define UART_INT_RAW_RXFIFO_FULL		((u32)0x00000001)

#define UART_INT_ST_RXFIFO_TOUT			((u32)0x00000100)
#define UART_INT_ST_BRK_DET				((u32)0x00000080)
#define UART_INT_ST_CTS_CHG				((u32)0x00000040)
#define UART_INT_ST_DSR_CHG				((u32)0x00000020)
#define UART_INT_ST_RXFIFO_OVF			((u32)0x00000010)
#define UART_INT_ST_FRM_ERR				((u32)0x00000008)
#define UART_INT_ST_PARITY_ERR			((u32)0x00000004)
#define UART_INT_ST_TXFIFO_EMPTY		((u32)0x00000002)
#define UART_INT_ST_RXFIFO_FULL			((u32)0x00000001)

#define UART_INT_ENA_RXFIFO_TOUT		((u32)0x00000100)
#define UART_INT_ENA_BRK_DET			((u32)0x00000080)
#define UART_INT_ENA_CTS_CHG			((u32)0x00000040)
#define UART_INT_ENA_DSR_CHG			((u32)0x00000020)
#define UART_INT_ENA_RXFIFO_OVF			((u32)0x00000010)
#define UART_INT_ENA_FRM_ERR			((u32)0x00000008)
#define UART_INT_ENA_PARITY_ERR			((u32)0x00000004)
#define UART_INT_ENA_TXFIFO_EMPTY		((u32)0x00000002)
#define UART_INT_ENA_RXFIFO_FULL		((u32)0x00000001)

#define UART_INT_CLR_RXFIFO_TOUT		((u32)0x00000100)
#define UART_INT_CLR_BRK_DET			((u32)0x00000080)
#define UART_INT_CLR_CTS_CHG			((u32)0x00000040)
#define UART_INT_CLR_DSR_CHG			((u32)0x00000020)
#define UART_INT_CLR_RXFIFO_OVF			((u32)0x00000010)
#define UART_INT_CLR_FRM_ERR			((u32)0x00000008)
#define UART_INT_CLR_PARITY_ERR			((u32)0x00000004)
#define UART_INT_CLR_TXFIFO_EMPTY		((u32)0x00000002)
#define UART_INT_CLR_RXFIFO_FULL		((u32)0x00000001)

#define UART_CLKDIV_CNT_MASK			((u32)0x000FFFFF)

#define UART_AUTOBAUD_GLITCH_FILT_MASK	((u32)0x0000FF00)
#define UART_AUTOBAUD_GLITCH_FILT_S		8
#define UART_AUTOBAUD_EN_				((u32)0x00000001)

#define UART_STATUS_TXD					((u32)0x80000000)
#define UART_STATUS_RTSN				((u32)0x40000000)
#define UART_STATUS_DTRN				((u32)0x20000000)
#define UART_STATUS_TXFIFO_CNT_MASK		((u32)0x00FF0000)
#define UART_STATUS_TXFIFO_CNT_S		16
#define UART_STATUS_RXD					((u32)0x00008000)
#define UART_STATUS_CTSN				((u32)0x00004000)
#define UART_STATUS_DSRN				((u32)0x00002000)
#define UART_STATUS_RXFIFO_CNT_MASK		((u32)0x000000FF)

#define UART_CONF0_DTR_INV				((u32)0x01000000)
#define UART_CONF0_RTS_INV				((u32)0x00800000)
#define UART_CONF0_TXD_INV				((u32)0x00400000)
#define UART_CONF0_DSR_INV				((u32)0x00200000)
#define UART_CONF0_CTS_INV				((u32)0x00100000)
#define UART_CONF0_RXD_INV				((u32)0x00080000)
#define UART_CONF0_TXFIFO_RST			((u32)0x00040000)
#define UART_CONF0_RXFIFO_RST			((u32)0x00020000)
#define UART_CONF0_IRDA_EN				((u32)0x00010000)
#define UART_CONF0_TX_FLOW_EN			((u32)0x00008000)
#define UART_CONF0_LOOPBACK				((u32)0x00004000)
#define UART_CONF0_IRDA_RX_INV			((u32)0x00002000)
#define UART_CONF0_IRDA_TX_INV			((u32)0x00001000)
#define UART_CONF0_IRDA_WCTL			((u32)0x00000800)
#define UART_CONF0_IRDA_TX_EN			((u32)0x00000400)
#define UART_CONF0_IRDA_DPLX			((u32)0x00000200)
#define UART_CONF0_TXD_BRK				((u32)0x00000100)
#define UART_CONF0_SW_DTR				((u32)0x00000080)
#define UART_CONF0_SW_RTS				((u32)0x00000040)
#define UART_CONF0_STOP_BIT_NUM_MASK	((u32)0x00000030)
#define UART_CONF0_STOP_BIT_NUM_S                 4
#define UART_CONF0_BIT_NUM_MASK			((u32)0x0000000C)
#define UART_CONF0_BIT_NUM_S                      2
#define UART_CONF0_PARITY_EN			((u32)0x00000002)
#define UART_CONF0_PARITY				((u32)0x00000001)

#define UART_CONF1_RX_TOUT_EN				((u32)0x80000000)
#define UART_CONF1_RX_TOUT_THRHD_MASK		((u32)0x7F000000)
#define UART_CONF1_RX_TOUT_THRHD_S			24
#define UART_CONF1_RX_FLOW_EN				((u32)0x08000000)
#define UART_CONF1_RX_FLOW_THRHD_MASK		((u32)0x007F0000)
#define UART_CONF1_RX_FLOW_THRHD_S			16
#define UART_CONF1_TXFIFO_EMPTY_THRHD_MASK	((u32)0x00007F00)
#define UART_CONF1_TXFIFO_EMPTY_THRHD_S		8
#define UART_CONF1_RXFIFO_FULL_THRHD_MASK	((u32)0x0000007F)

#define UART_LOWPULSE_MIN_CNT_MASK		((u32)0x000FFFFF)

#define UART_HIGHPULSE_MIN_CNT_MASK		((u32)0x000FFFFF)

#define UART_PULSE_NUM_CNT_MASK			((u32)0x000003FF)

#define UART0_BaseAddress		0x60000000
#define UART1_BaseAddress		0x60000F00

#define UART_0 		((UART_TypeDef *)UART0_BaseAddress)
#define UART_1 		((UART_TypeDef *)UART1_BaseAddress)


#endif // UART_REGISTER_H_INCLUDED

