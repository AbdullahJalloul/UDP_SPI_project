/*
 * slc_registers.h
 *
 *  Created on: 30.05.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _SLC_REGISTERS_H_
#define _SLC_REGISTERS_H_

#include "c_types.h"

#ifndef __I
#define __I		volatile const   /*!< defines 'read only' permissions     */
#endif	// #ifndef __I

#ifndef __O
#define __O		volatile         /*!< defines 'write only' permissions    */
#endif	// #ifndef __O

#ifndef __IO
#define __IO	volatile         /*!< defines 'read / write' permissions  */
#endif	// #ifndef __IO

/******************* SLC bitfields *******************************************/
// correct bit ascending
#define SLC_MODE 0x00000003
#define SLC_MODE_S 12

typedef struct
{
	__IO unsigned	txlink_rst			:1;	// bit 0
	__IO unsigned	rxlink_rst			:1;	// bit 1
	__IO unsigned	ahbm_fifo_rst		:1;	// bit 2
	__IO unsigned	ahbm_rst			:1;	// bit 3
	__IO unsigned	tx_loop_test		:1;	// bit 4
	__IO unsigned	rx_loop_test		:1;	// bit 5
	__IO unsigned	rx_auto_wrback		:1;	// bit 6
	__IO unsigned	rx_no_restart_clr	:1;	// bit 7
	__IO unsigned	dscr_burst_en		:1;	// bit 8
	__IO unsigned	data_burst_en		:1;	// bit 9
	__I unsigned	dummy10				:2;	// bits 10-11
	__IO unsigned	slc_mode			:2;	// bits 12-13
	__I unsigned	dummy14				:18;	// bits 14-31
} slc_conf0_t;

typedef struct
{
	__IO unsigned	frhost_bit0_int		:1;	// bit 0
	__IO unsigned	frhost_bit1_int		:1;	// bit 1
	__IO unsigned	frhost_bit2_int		:1;	// bit 2
	__IO unsigned	frhost_bit3_int		:1;	// bit 3
	__IO unsigned	frhost_bit4_int		:1;	// bit 4
	__IO unsigned	frhost_bit5_int		:1;	// bit 5
	__IO unsigned	frhost_bit6_int		:1;	// bit 6
	__IO unsigned	frhost_bit7_int		:1;	// bit 7
	__IO unsigned	rx_start_int		:1;	// bit 8
	__IO unsigned	tx_start_int		:1;	// bit 9
	__IO unsigned	rx_udf_int			:1;	// bit 10
	__IO unsigned	tx_ovf_int			:1;	// bit 11
	__IO unsigned	token0_1to0_int		:1;	// bit 12
	__IO unsigned	token1_1to0_int		:1;	// bit 13
	__IO unsigned	tx_done_int			:1;	// bit 14
	__IO unsigned	tx_eof_int			:1;	// bit 15
	__IO unsigned	rx_done_int			:1;	// bit 16
	__IO unsigned	rx_eof_int			:1;	// bit 17
	__IO unsigned	tohost_int			:1;	// bit 18
	__IO unsigned	tx_dscr_err_int		:1;	// bit 19
	__IO unsigned	rx_dscr_err_int		:1;	// bit 20
	__IO unsigned	tx_dscr_empty_int	:1;	// bit 21
	__I unsigned	dummy22				:10;
} slc_int_t;

typedef struct
{
	__IO unsigned	rx_full			:1;	// bit 0
	__IO unsigned	rx_empty		:1;	// bit 1
	__I unsigned	dummy2			:30;
} slc_rx_status_t;

typedef struct
{
	__IO unsigned	rx_fifo_data	:9;
	__I unsigned	dummy9			:7;
	__IO unsigned	rx_fifo_push	:1;	// bit 16
	__I unsigned	dummy17			:15;	
} slc_rx_fifo_push_t;

typedef struct
{
	__IO unsigned	tx_full			:1;	// bit 0
	__IO unsigned	tx_empty		:1;	// bit 1
	__I unsigned	dummy2			:30;
} slc_tx_status_t;

typedef struct
{
	__IO unsigned	tx_fifo_rdata	:11;
	__I unsigned	dummy11			:5;
	__IO unsigned	rx_fifo_pop		:1;	// bit 16
	__I unsigned	dummy17			:15;
} slc_tx_fifo_pop_t;

typedef struct
{
	__IO unsigned	local_wdata		:12;
	__IO unsigned	local_wr		:1;	// bit 12
	__IO unsigned	local_inc		:1;	// bit 13
	__IO unsigned	local_inc_more	:1;	// bit 14
	__I unsigned	dummy15			:1;
	__IO unsigned	mask			:12;
	__IO unsigned	dummy28			:4;
} slc_token_t;

typedef struct
{
	__IO unsigned	desc_addr		:20;
	__I unsigned	dummy20			:8;
	__IO unsigned	stop			:1;	// bit 28
	__IO unsigned	start			:1;	// bit 29
	__IO unsigned	restart			:1;	// bit 30
	__IO unsigned	park			:1;	// bit 31
} slc_link_t;

typedef struct
{
	__IO unsigned	tohost_intvect		:8;
	__I unsigned	dummy8				:24;
} slc_int_vect_t;

typedef struct
{
	__IO unsigned	txeof_ena		:6;
	__I unsigned	dummy6			:2;
	__IO unsigned	fifo_map_ena	:4;
	__IO unsigned	dummy_mode		:1;
	__I	unsigned	dummy13			:3;
	__IO unsigned	push_idle_num	:16;
} slc_bridge_conf_t;

typedef struct
{
	__IO unsigned	ahb_testmode	:3;
	__I unsigned	dummy3			:1;
	__IO unsigned	ahb_testaddr	:2;
	__I unsigned	dummy6			:26;
} slc_ahb_test_t;


typedef struct
{
	__IO unsigned	cmd_status		:3;
	__I unsigned	dummy3			:1;
	__IO unsigned	func_status		:4;
	__IO unsigned	sdio_wakeup		:1;	// bit 8
	__I unsigned	dummy8			:3;
	__IO unsigned	bus_status		:3;
	__I unsigned	dummy15			:17;
} slc_sdio_status_t;

typedef struct
{
	__IO unsigned	pop_idle_count		:16;
	__IO unsigned	token_no_replace	:1;	// bit 16
	__IO unsigned	infor_no_replace	:1;	// bit 17
	__IO unsigned	rx_fill_mode		:1;	// bit 18
	__IO unsigned	rx_eof_mode			:1;	// bit 19
	__IO unsigned	rx_fill_en			:1;	// bit 20
} slc_rx_dscr_conf_t;

typedef union
{
	__I unsigned	dummy0			:12;
	__IO unsigned	host_mode		:4;	//[15:12]: 0x3ff9xxxx -- 0b01  from_host ???
										//         0x3ffaxxxx -- 0b10  general
										//         0x3ffbxxxx -- 0b11  to_host 
	__I unsigned	dummy16			:7;
	__IO unsigned	host_intr_ena	:1;	// bit 23
	__I unsigned	dummy24			:8;
} slc_host_intr_raw_t;

typedef struct
{
	__I unsigned	dummy0			:12;
	__IO unsigned	host_intr_sof	:1;	// bit 12
	__I unsigned	dummy13			:19;	
} slc_host_intr_clear_t;

typedef struct
{
	__IO unsigned	to_host_int_ena	:1;	// bit 0
	__I unsigned	dummy1			:22;	
	__IO unsigned	trx_new_packet_int_ena	:1;	// bit 23
	__I unsigned	dummy24			:8;	
} slc_host_intr_ena_t;

typedef struct
{
	union								// 0x60000B00
	{
		__IO u32			conf0;
		__IO slc_conf0_t	conf0_bits;
	};
	union								// 0x60000B04
	{
		__IO u32			int_raw;
		__IO slc_int_t		int_raw_bits;
	};
	union								// 0x60000B08
	{
		__IO u32			int_status;
		__IO slc_int_t	int_status_bits;
	};
	union								// 0x60000B0C
	{
		__IO u32			int_ena;
		__IO slc_int_t	int_ena_bits;
	};
	union								// 0x60000B10
	{
		__IO u32			int_clr;
		__IO slc_int_t		int_clr_bits;
	};
	union								// 0x60000B14
	{
		__IO u32				rx_status;
		__IO slc_rx_status_t	rx_status_bits;
	};
	union								// 0x60000B18
	{
		__IO u32				rx_fifo_push;
		__IO slc_rx_fifo_push_t	rx_fifo_push_bits;
	};
	union								// 0x60000B1C
	{
		__IO u32				tx_status;
		__IO slc_tx_status_t	tx_status_bits;
	};
	union								// 0x60000B20
	{
		__IO u32				tx_fifo_pop;
		__IO slc_tx_fifo_pop_t	tx_fifo_pop_bits;
	};
	union								// 0x60000B24
	{
		__IO u32				rx_link;
		__IO slc_link_t			rx_link_bits;
	};
	union								// 0x60000B28
	{
		__IO u32				tx_link;
		__IO slc_link_t			tx_link_bits;
	};
	union								// 0x60000B2C
	{
		__IO u32				int_vect;
		__IO slc_int_vect_t		int_vect_bits;
	};
	union								// 0x60000B30
	{
		__IO u32				token0;
		__IO slc_token_t		token0_bits;
	};
	union								// 0x60000B34
	{
		__IO u32				token1;
		__IO slc_token_t		token1_bits;
	};
	__IO u32	conf1;					// 0x60000B38
	__IO u32	state0;					// 0x60000B3C
	__IO u32	state1;					// 0x60000B40
	union								// 0x60000B44
	{
		__IO u32				bridge_conf;
		__IO slc_bridge_conf_t	bridge_conf_bits;
	};
	union								// 0x60000B48
	{
		__IO u32	rx_eof_des_addr;
		__IO u32	to_host_last_desc;
	};
	union								// 0x60000B4C
	{
		__IO u32	tx_eof_des_addr;
		__IO u32	from_host_last_desc;
	};
	__IO u32	rx_eof_bfr_des_addr;	// 0x60000B50
	union								// 0x60000B54
	{
		__IO u32				ahb_test;
		__IO slc_ahb_test_t		ahb_test_bits;
	};
	union								// 0x60000B58
	{
		__IO u32				sdio_status;
		__IO slc_sdio_status_t	sdio_status_bits;
	};
	union								// 0x60000B5C
	{
		__IO u32				rx_dscr_conf;
		__IO slc_rx_dscr_conf_t	rx_dscr_conf_bits;
	};
	__IO u32	txlink_dscr;			// 0x60000B60
	__IO u32	txlink_dscr_bf0;		// 0x60000B64
	__IO u32	txlink_dscr_bf1;		// 0x60000B68
	__IO u32	rxlink_dscr;			// 0x60000B6C
	__IO u32	rxlink_dscr_bf0;		// 0x60000B70
	__IO u32	rxlink_dscr_bf1;		// 0x60000B74
	__IO u32	slc_date;				// 0x60000B78
	__IO u32	slc_id;					// 0x60000B7C
	__I u32		dummy80;				// 0x60000B80
	__I u32		dummy84;				// 0x60000B84
	union								// 0x60000B88
	{
		__IO u32					host_intr_raw;
		__IO slc_host_intr_raw_t	host_intr_raw_bits;
	};
	__I u32		dummy8C;				// 0x60000B8C
	__I u32		dummy90;				// 0x60000B90
	__IO u32	host_conf_w0;			// 0x60000B94
	__IO u32	host_conf_w1;			// 0x60000B98
	__IO u32	host_intr_status;		// 0x60000B9C
	__IO u32	host_conf_w2;			// 0x60000BA0
	__IO u32	host_conf_w3;			// 0x60000BA4
	__IO u32	host_conf_w4;			// 0x60000BA8
	__I u32		dummyAC;				// 0x60000BAC
	union								// 0x60000BB0
	{
		__IO u32					host_intr_clear;
		__IO slc_host_intr_clear_t	host_intr_clear_bits;
	};
	union								// 0x60000BB4
	{
		__IO u32					host_intr_ena;
		__IO slc_host_intr_ena_t	host_intr_ena_bits;
	};
	__IO u32	host_conf_w5;			// 0x60000BBC
	
} SLC_TypeDef;

#define SLC_BaseAddress		0x60000B00
#define SLC 				((SLC_TypeDef *)SLC_BaseAddress)

/********** SLC registers reset values ****************************************/

#define SLC_CONF0_RESET_VALUE				((u32)0x00000000)
#define SLC_INT_RAW_RESET_VALUE				((u32)0x00000000)
#define SLC_INT_STATUS_RESET_VALUE			((u32)0x00000000)
#define SLC_INT_ENA_RESET_VALUE				((u32)0x00000000)
#define SLC_INT_CLR_RESET_VALUE				((u32)0x00000000)
#define SLC_RX_STATUS_RESET_VALUE			((u32)0x00000000)
#define SLC_RX_FIFO_PUSH_RESET_VALUE		((u32)0x00000000)
#define SLC_TX_STATUS_RESET_VALUE			((u32)0x00000000)
#define SLC_TX_FIFO_POP_RESET_VALUE			((u32)0x00000000)
#define SLC_RX_LINK_RESET_VALUE				((u32)0x00000000)
#define SLC_TX_LINK_RESET_VALUE				((u32)0x00000000)
#define SLC_INT_VECT_RESET_VALUE			((u32)0x00000000)
#define SLC_TOKEN0_RESET_VALUE				((u32)0x00000000)
#define SLC_TOKEN1_RESET_VALUE				((u32)0x00000000)
#define SLC_CONF1_RESET_VALUE				((u32)0x00000000)
#define SLC_STATE0_RESET_VALUE				((u32)0x00000000)
#define SLC_STATE1_RESET_VALUE				((u32)0x00000000)
#define SLC_BRIDGE_CONF_RESET_VALUE			((u32)0x00000000)
#define SLC_RX_EOF_DES_ADDR_RESET_VALUE		((u32)0x00000000)
#define SLC_TX_EOF_DES_ADDR_RESET_VALUE		((u32)0x00000000)
#define SLC_RX_EOF_BUF_DES_ADDR_RESET_VALUE		((u32)0x00000000)
#define SLC_AHB_TEST_RESET_VALUE			((u32)0x00000000)
#define SLC_SDIO_STATUS_RESET_VALUE			((u32)0x00000000)
#define SLC_RX_DSR_CONF_RESET_VALUE			((u32)0x00000000)
#define SLC_TXLINK_DSCR_RESET_VALUE			((u32)0x00000000)
#define SLC_TXLINK_DSCR_BF0_RESET_VALUE		((u32)0x00000000)
#define SLC_TXLINK_DSCR_BF1_RESET_VALUE		((u32)0x00000000)
#define SLC_RXLINK_DSCR_RESET_VALUE			((u32)0x00000000)
#define SLC_RXLINK_DSCR_BF0_RESET_VALUE		((u32)0x00000000)
#define SLC_RXLINK_DSCR_BF1_RESET_VALUE		((u32)0x00000000)
#define SLC_DATE_RESET_VALUE				((u32)0x00000000)
#define SLC_ID_RESET_VALUE					((u32)0x00000000)
#define SLC_HOST_INT_RAW_RESET_VALUE		((u32)0x00000000)
#define SLC_HOST_CONF_W0_RESET_VALUE		((u32)0x00000000)
#define SLC_HOST_CONF_W1_RESET_VALUE		((u32)0x00000000)
#define SLC_HOST_INTR_STATUS_RESET_VALUE		((u32)0x00000000)
#define SLC_HOST_CONF_W2_RESET_VALUE		((u32)0x00000000)
#define SLC_HOST_CONF_W3_RESET_VALUE		((u32)0x00000000)
#define SLC_HOST_CONF_W4_RESET_VALUE		((u32)0x00000000)
#define SLC_HOST_INTR_CLEAR_RESET_VALUE		((u32)0x00000000)
#define SLC_HOST_INTR_ENA_RESET_VALUE		((u32)0x00000000)
#define SLC_HOST_CONF_W5_RESET_VALUE		((u32)0x00000000)

/************* SLC BIT REGISTERS MASKS ****************************************/

#define SLC_CONF0_MODE_MASK 				((u32)0x00003000)
#define SLC_CONF0_TXLINK_RST				((u32)0x00000001)	// (BIT(0))
#define SLC_CONF0_RXLINK_RST				((u32)0x00000002)	// (BIT(1))
#define SLC_CONF0_AHBM_FIFO_RST				((u32)0x00000004)	// (BIT(2))
#define SLC_CONF0_AHBM_RST					((u32)0x00000008)	// (BIT(3))
#define SLC_CONF0_TX_LOOP_TEST				((u32)0x00000010)	// (BIT(4))
#define SLC_CONF0_RX_LOOP_TEST				((u32)0x00000020)	// (BIT(5))
#define SLC_CONF0_RX_AUTO_WRBACK			((u32)0x00000040)	// (BIT(6))
#define SLC_CONF0_RX_NO_RESTART_CLR			((u32)0x00000080)	// (BIT(7))
#define SLC_CONF0_DSCR_BURST_EN				((u32)0x00000100)	// (BIT(8))
#define SLC_CONF0_DATA_BURST_EN				((u32)0x00000200)	// (BIT(9))

#define SLC_INT_RAW_FRHOST_BIT0				((u32)0x00000001)	// (BIT(0))
#define SLC_INT_RAW_FRHOST_BIT1				((u32)0x00000002)	// (BIT(1))
#define SLC_INT_RAW_FRHOST_BIT2				((u32)0x00000004)	// (BIT(2))
#define SLC_INT_RAW_FRHOST_BIT3				((u32)0x00000008)	// (BIT(3))
#define SLC_INT_RAW_FRHOST_BIT4				((u32)0x00000010)	// (BIT(4))
#define SLC_INT_RAW_FRHOST_BIT5				((u32)0x00000020)	// (BIT(5))
#define SLC_INT_RAW_FRHOST_BIT6				((u32)0x00000040)	// (BIT(6))
#define SLC_INT_RAW_FRHOST_BIT7				((u32)0x00000080)	// (BIT(7))
#define SLC_INT_RAW_RX_START				((u32)0x00000100)	// (BIT(8))
#define SLC_INT_RAW_TX_START				((u32)0x00000200)	// (BIT(9))
#define SLC_INT_RAW_RX_UDF					((u32)0x00000400)	// (BIT(10))
#define SLC_INT_RAW_TX_OVF					((u32)0x00000800)	// (BIT(11))
#define SLC_INT_RAW_TOKEN0_1TO0				((u32)0x00001000)	// (BIT(12))
#define SLC_INT_RAW_TOKEN1_1TO0				((u32)0x00002000)	// (BIT(13))
#define SLC_INT_RAW_TX_DONE					((u32)0x00004000)	// (BIT(14))
#define SLC_INT_RAW_TX_EOF					((u32)0x00008000)	// (BIT(15))
#define SLC_INT_RAW_RX_DONE					((u32)0x00010000)	// (BIT(16))
#define SLC_INT_RAW_RX_EOF					((u32)0x00020000)	// (BIT(17))
#define SLC_INT_RAW_TOHOST					((u32)0x00040000)	// (BIT(18))
#define SLC_INT_RAW_TX_DSCR_ERR				((u32)0x00080000)	// (BIT(19))
#define SLC_INT_RAW_RX_DSCR_ERR				((u32)0x00100000)	// (BIT(20))
#define SLC_INT_RAW_TX_DSCR_EMPTY			((u32)0x00200000)	// (BIT(21))

#define SLC_ST_INT_FRHOST_BIT0				((u32)0x00000001)	// (BIT(0))
#define SLC_ST_INT_FRHOST_BIT1				((u32)0x00000002)	// (BIT(1))
#define SLC_ST_INT_FRHOST_BIT2				((u32)0x00000004)	// (BIT(2))
#define SLC_ST_INT_FRHOST_BIT3				((u32)0x00000008)	// (BIT(3))
#define SLC_ST_INT_FRHOST_BIT4				((u32)0x00000010)	// (BIT(4))
#define SLC_ST_INT_FRHOST_BIT5				((u32)0x00000020)	// (BIT(5))
#define SLC_ST_INT_FRHOST_BIT6				((u32)0x00000040)	// (BIT(6))
#define SLC_ST_INT_FRHOST_BIT7				((u32)0x00000080)	// (BIT(7))
#define SLC_ST_INT_RX_START					((u32)0x00000100)	// (BIT(8))
#define SLC_ST_INT_TX_START					((u32)0x00000200)	// (BIT(9))
#define SLC_ST_INT_RX_UDF					((u32)0x00000400)	// (BIT(10))
#define SLC_ST_INT_TX_OVF					((u32)0x00000800)	// (BIT(11))
#define SLC_ST_INT_TOKEN0_1TO0				((u32)0x00001000)	// (BIT(12))
#define SLC_ST_INT_TOKEN1_1TO0				((u32)0x00002000)	// (BIT(13))
#define SLC_ST_INT_TX_DONE					((u32)0x00004000)	// (BIT(14))
#define SLC_ST_INT_TX_EOF					((u32)0x00008000)	// (BIT(15))
#define SLC_ST_INT_RX_DONE					((u32)0x00010000)	// (BIT(16))
#define SLC_ST_INT_RX_EOF					((u32)0x00020000)	// (BIT(17))
#define SLC_ST_INT_TOHOST					((u32)0x00040000)	// (BIT(18))
#define SLC_ST_INT_TX_DSCR_ERR				((u32)0x00080000)	// (BIT(19))
#define SLC_ST_INT_RX_DSCR_ERR				((u32)0x00100000)	// (BIT(20))
#define SLC_ST_INT_TX_DSCR_EMPTY			((u32)0x00200000)	// (BIT(21))

#define SLC_ENA_INT_FRHOST_BIT_MASK			((u32)0x000000FF)
#define SLC_ENA_INT_FRHOST_BIT0				((u32)0x00000001)	// (BIT(0))
#define SLC_ENA_INT_FRHOST_BIT1				((u32)0x00000002)	// (BIT(1))
#define SLC_ENA_INT_FRHOST_BIT2				((u32)0x00000004)	// (BIT(2))
#define SLC_ENA_INT_FRHOST_BIT3				((u32)0x00000008)	// (BIT(3))
#define SLC_ENA_INT_FRHOST_BIT4				((u32)0x00000010)	// (BIT(4))
#define SLC_ENA_INT_FRHOST_BIT5				((u32)0x00000020)	// (BIT(5))
#define SLC_ENA_INT_FRHOST_BIT6				((u32)0x00000040)	// (BIT(6))
#define SLC_ENA_INT_FRHOST_BIT7				((u32)0x00000080)	// (BIT(7))
#define SLC_ENA_INT_RX_START				((u32)0x00000100)	// (BIT(8))
#define SLC_ENA_INT_TX_START				((u32)0x00000200)	// (BIT(9))
#define SLC_ENA_INT_RX_UDF					((u32)0x00000400)	// (BIT(10))
#define SLC_ENA_INT_TX_OVF					((u32)0x00000800)	// (BIT(11))
#define SLC_ENA_INT_TOKEN0_1TO0				((u32)0x00001000)	// (BIT(12))
#define SLC_ENA_INT_TOKEN1_1TO0				((u32)0x00002000)	// (BIT(13))
#define SLC_ENA_INT_TX_DONE					((u32)0x00004000)	// (BIT(14))
#define SLC_ENA_INT_TX_EOF					((u32)0x00008000)	// (BIT(15))
#define SLC_ENA_INT_RX_DONE					((u32)0x00010000)	// (BIT(16))
#define SLC_ENA_INT_RX_EOF					((u32)0x00020000)	// (BIT(17))
#define SLC_ENA_INT_TOHOST					((u32)0x00040000)	// (BIT(18))
#define SLC_ENA_INT_TX_DSCR_ERR				((u32)0x00080000)	// (BIT(19))
#define SLC_ENA_INT_RX_DSCR_ERR				((u32)0x00100000)	// (BIT(20))
#define SLC_ENA_INT_TX_DSCR_EMPTY			((u32)0x00200000)	// (BIT(21))

#define SLC_INT_CLR_FRHOST_BIT0				((u32)0x00000001)	// (BIT(0))
#define SLC_INT_CLR_FRHOST_BIT1				((u32)0x00000002)	// (BIT(1))
#define SLC_INT_CLR_FRHOST_BIT2				((u32)0x00000004)	// (BIT(2))
#define SLC_INT_CLR_FRHOST_BIT3				((u32)0x00000008)	// (BIT(3))
#define SLC_INT_CLR_FRHOST_BIT4				((u32)0x00000010)	// (BIT(4))
#define SLC_INT_CLR_FRHOST_BIT5				((u32)0x00000020)	// (BIT(5))
#define SLC_INT_CLR_FRHOST_BIT6				((u32)0x00000040)	// (BIT(6))
#define SLC_INT_CLR_FRHOST_BIT7				((u32)0x00000080)	// (BIT(7))
#define SLC_INT_CLR_RX_START				((u32)0x00000100)	// (BIT(8))
#define SLC_INT_CLR_TX_START				((u32)0x00000200)	// (BIT(9))
#define SLC_INT_CLR_RX_UDF					((u32)0x00000400)	// (BIT(10))
#define SLC_INT_CLR_TX_OVF					((u32)0x00000800)	// (BIT(11))
#define SLC_INT_CLR_TOKEN0_1TO0				((u32)0x00001000)	// (BIT(12))
#define SLC_INT_CLR_TOKEN1_1TO0				((u32)0x00002000)	// (BIT(13))
#define SLC_INT_CLR_TX_DONE					((u32)0x00004000)	// (BIT(14))
#define SLC_INT_CLR_TX_EOF					((u32)0x00008000)	// (BIT(15))
#define SLC_INT_CLR_RX_DONE					((u32)0x00010000)	// (BIT(16))
#define SLC_INT_CLR_RX_EOF					((u32)0x00020000)	// (BIT(17))
#define SLC_INT_CLR_TOHOST					((u32)0x00040000)	// (BIT(18))
#define SLC_INT_CLR_TX_DSCR_ERR				((u32)0x00080000)	// (BIT(19))
#define SLC_INT_CLR_RX_DSCR_ERR				((u32)0x00100000)	// (BIT(20))
#define SLC_INT_CLR_TX_DSCR_EMPTY			((u32)0x00200000)	// (BIT(21))

#define SLC_RX_STATUS_FULL					((u32)0x00000001)	// (BIT(0))
#define SLC_RX_STATUS_EMPTY					((u32)0x00000002)	// (BIT(1))

#define SLC_RX_FIFO_WDATA					((u32)0x000001FF)
#define SLC_RX_FIFO_PUSH					((u32)0x00010000)	// (BIT(16))

#define SLC_TX_STATUS_FULL					((u32)0x00000001)	// (BIT(0))
#define SLC_TX_STATUS_EMPTY					((u32)0x00000002)	// (BIT(1))

#define SLC_TX_FIFO_POP						((u32)0x00010000)	// (BIT(16))
#define SLC_TX_FIFO_RDATA					((u32)0x000007FF)

#define SLC_RX_LINK_DESCADDR_MASK			((u32)0x000FFFFF)
#define SLC_RX_LINK_STOP					((u32)0x10000000)	// (BIT(28))
#define SLC_RX_LINK_START					((u32)0x20000000)	// (BIT(29))
#define SLC_RX_LINK_RESTART					((u32)0x40000000)	// (BIT(30))
#define SLC_RX_LINK_PARK					((u32)0x80000000)	// (BIT(31))

#define SLC_TX_LINK_DESCADDR_MASK			((u32)0x000FFFFF)
#define SLC_TX_LINK_STOP					((u32)0x10000000)	// (BIT(28))
#define SLC_TX_LINK_START					((u32)0x20000000)	// (BIT(29))
#define SLC_TX_LINK_RESTART					((u32)0x40000000)	// (BIT(30))
#define SLC_TX_LINK_PARK					((u32)0x80000000)	// (BIT(31))

#define SLC_TOHOST_INTVEC					((u32)0x000000FF)

#define SLC_TOKEN0_LOCAL_WDATA_MASK			((u32)0x00000FFF)
#define SLC_TOKEN0_LOCAL_WR					((u32)0x00001000)	// (BIT(12))
#define SLC_TOKEN0_LOCAL_INC				((u32)0x00002000)	// (BIT(13))
#define SLC_TOKEN0_LOCAL_INC_MORE			((u32)0x00004000)	// (BIT(14))
#define SLC_TOKEN0_MASK						((u32)0x0FFF0000)

#define SLC_TOKEN1_LOCAL_WDATA				((u32)0x00000FFF)
#define SLC_TOKEN1_LOCAL_WR					((u32)0x00001000)	// (BIT(12))
#define SLC_TOKEN1_LOCAL_INC				((u32)0x00002000)	// (BIT(13))
#define SLC_TOKEN1_LOCAL_INC_MORE			((u32)0x00004000)	// (BIT(14))
#define SLC_TOKEN1_MASK						((u32)0x0FFF0000)

#define SLC_BRIDGE_TXEOF_ENA				((u32)0x0000003F)
#define SLC_BRIDGE_FIFO_MAP_ENA				((u32)0x00000F00)
#define SLC_BRIDGE_TX_DUMMY_MODE			((u32)0x00001000)	// (BIT(12))
#define SLC_BRIDGE_TX_PUSH_IDLE_NUM			((u32)0xFFFF0000)

#define SLC_AHB_TEST_MODE_MASK				((u32)0x00000007)
#define SLC_AHB_TEST_ADDR_MASK				((u32)0x00000030)

#define SLC_SDIO_ST_CMD_MASK				((u32)0x00000007)
#define SLC_SDIO_ST_FUNC_MASK				((u32)0x000000F0)
#define SLC_SDIO_ST_SDIO_WAKEUP				((u32)0x00000100)	// (BIT(8))
#define SLC_SDIO_ST_BUS_MASK				((u32)0x00007000)

#define SLC_RX_POP_IDLE_CNT_MASK			((u32)0x0000FFFF)
#define SLC_RX_TOKEN_NO_REPLACE				((u32)0x00010000)	// (BIT(16))
#define SLC_RX_INFOR_NO_REPLACE				((u32)0x00020000)	// (BIT(17))
#define SLC_RX_FILL_MODE					((u32)0x00040000)	// (BIT(18))
#define SLC_RX_EOF_MODE						((u32)0x00080000)	// (BIT(19))
#define SLC_RX_FILL_EN						((u32)0x00100000)	// (BIT(20))

#define SLC_HOST_INTR_CLR_SOF				((u32)0x00001000)	// (BIT(12))

#define SLC_INT_ENA_RX_NEW_PACKET			((u32)0x00800000)	// (BIT23)
#define SLC_INT_ENA_HOST_TOHOST_BIT0		((u32)0x00000001)	// (BIT0)

#define SLC_HOST_INTR_RAW_ENA_BIT			((u32)0x00800000)	// (BIT(23))
//[15:12]: 0x3ff9xxxx -- 0b01  from_host
//         0x3ffaxxxx -- 0b10  general
//         0x3ffbxxxx -- 0b11  to_host
#define SLC_DATA_ADDR_CLEAR_MASK			((u32)0x0000F000) 
#define SLC_FROM_HOST_ADDR_MASK				((u32)0x00001000)
#define SLC_TO_HOST_ADDR_MASK				((u32)0x00003000)

#define SLC_SET_FROM_HOST_ADDR_MASK(v)   do { \
    (v) &= SLC_DATA_ADDR_CLEAR_MASK;  \
    (v) |= SLC_FROM_HOST_ADDR_MASK;   \
} while(0);

#define SLC_SET_TO_HOST_ADDR_MASK(v)   do { \
    (v) &= SLC_DATA_ADDR_CLEAR_MASK;  \
    (v) |= SLC_TO_HOST_ADDR_MASK;   \
} while(0);

#define SLC_TX_DESC_DEBUG_REG  0x3ff0002C  //[15:0] set to 0xcccc

#endif /* _SLC_REGISTERS_H_ */
