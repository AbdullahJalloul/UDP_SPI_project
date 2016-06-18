/*
 * dport_registers.h
 *
 *  Created on: 19.05.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _DPORT_REGISTERS_H_
#define _DPORT_REGISTERS_H_

#include "c_types.h"

#ifndef __I
#define     __I     volatile const   /*!< defines 'read only' permissions     */
#endif	// #ifndef __I

#ifndef __O
#define     __O     volatile         /*!< defines 'write only' permissions    */
#endif	// #ifndef __O

#ifndef __IO
#define     __IO    volatile         /*!< defines 'read / write' permissions  */
#endif	// #ifndef __IO

/******************* DPORT bitfields ******************************************/
// correct bit ascending

typedef struct
{
	__IO unsigned	wdt_edge_int_en		:1;	// bit0: WDT edge int enable
	__IO unsigned	timer0_edge_int_en	:1;	// bit1: TIMER0 edge int enable
	__I unsigned	dummy2				:30;
} dport_reg_1_t;

typedef struct
{
	__IO unsigned	cashe_flush		:1;	// bit0: cache flush start bit
	__IO unsigned	cashe_empty		:1;	// bit1: cache empty flag bit
	__I unsigned	dummy2			:7;
	__IO unsigned	spi_busy		:1;	// bit9: status SPI 0-wait, 1- busy
	__I unsigned	dummy10			:22;
} dport_reg_3_t;

typedef struct
{
	__IO unsigned	clk_pre			:1;	// bit0: =1 CPU 160 MHz, = 0 CPU 80 MHz
	__I unsigned	dummy1			:31;
} dport_reg_5_t;

typedef struct
{
	__IO unsigned	clk_pre			:1;	// bit0: =1 CPU 160 MHz, = 0 CPU 80 MHz
	__I unsigned	dummy1			:31;
} dport_reg_6_t;

typedef struct
{
	__IO unsigned	uart0_isr		:1;	// bit0 is for uart0 isr
	__I unsigned	dummy1			:1;
	__IO unsigned	uart1_isr		:1;	// bit2 is for uart1 isr
	__I unsigned	dummy3			:1;
	__IO unsigned	spi0_isr		:1;	// bit4 is for spi0 isr
	__I unsigned	dummy5			:1;
	__I unsigned	dummy6			:1;
	__IO unsigned	spi1_isr		:1;	// bit7 is for spi1 isr
	__I unsigned	dummy8			:1;
	__IO unsigned	i2s_isr			:1;	// bit9 is for i2s isr
	__I unsigned	dummy10			:22;
} dport_isr_flag_t;

typedef struct
{
	__I unsigned	dummy0			:7;
	__IO unsigned	spi_8000		:1;	// bit7 16k IRAM base 0x40108000 = SPI cache flash
	__IO unsigned	spi_C000		:1;	// bit8 16k IRAM base 0x4010C000 = SPI cache flash
	__I unsigned	dummy9			:23;
} dport_reg_9_t;

typedef struct		// 0x3ff00028
{
	__IO unsigned	swap_two_uart	:1;	// bit0: swap two uart
	__IO unsigned	swap_two_spi	:1;	// bit1: swap two spi
	__IO unsigned	swap_uart0		:1;	// bit2: swap uart0 pins (u0rxd <-> u0cts), (u0txd <-> u0rts)
	__IO unsigned	swap_uart1		:1;	// bit3: swap uart1 pins (u1rxd <-> u1cts), (u1txd <-> u1rts)
	__I unsigned	dummy4			:1;
	__IO unsigned	spi1_high_prior	:1;	// bit5: hspi is with the higher prior
	__IO unsigned	spi1_overlap	:1;	// bit6: two spi masters on spi1
	__IO unsigned	spi0_overlap	:1;	// bit7: two spi masters on spi0 (spi0_overlap)
	__I unsigned	dummy8			:24;
} dport_peri_io_t;

typedef struct
{
	__IO unsigned	tx_desc_debug	:16;	// SLC_TX_DESC_DEBUG[15:0] set to 0xCCCC
	__I unsigned	dummy16			:16;
} dport_reg_11_t;

typedef struct
{
	__IO u32	nmi_int;	// 0x3FF00000
	union					// 0x3FF00004
	{
		__IO u32			reg_1;
		__IO dport_reg_1_t	reg_1_bits;
	};
	__IO u32	reg_08;		// 0x3FF00008
	union					// 0x3FF0000C
	{
		__IO u32			reg_0C;
		__IO dport_reg_3_t	reg_0C_bits;
	};
	__IO u32	reg_10;		// 0x3FF00010
	union					// 0x3FF00014
	{
		__IO u32			reg_14;
		__IO dport_reg_5_t	reg_14_bits;
	};
	union					// 0x3FF00018
	{
		__IO u32			reg_18;	// use clockgate_watchdog(flg) { if(flg) 0x3FF00018 &= 0x77; else 0x3FF00018 |= 8; }
		__IO dport_reg_6_t	reg_18_bits;
	};
	__IO u32	reg_1C;		// 0x3FF0001C
	union					// 0x3FF00020
	{
		__IO u32				isr_flag;	// isr flag register, (ESP8266 SPI Module User Guide)
		__IO dport_isr_flag_t	isr_flag_bits;
	};
	union					// 0x3FF00024
	{
		__IO u32			reg_24;	// IRAM base = SPI cache flash
		__IO dport_reg_9_t	reg_24_bits;
	};
	union					// 0x3FF00028
	{
		__IO u32				peri_io;	// PERI_IO_SWAP:
		__IO dport_peri_io_t	peri_io_bits;
	};
	union					// 0x3FF0002C
	{
		__IO u32			reg_2C;	// SLC_TX_DESC_DEBUG_REG: 0x3ff0002c
		__IO dport_reg_11_t	reg_2C_bits;
	};
	__I u32	dummy30;		// 0x3FF00030
	__I u32	dummy34;		// 0x3FF00034
	__I u32	dummy38;		// 0x3FF00038
	__I u32	dummy3C;		// 0x3FF0003C
	__I u32	dummy40;		// 0x3FF00040
	__I u32	dummy44;		// 0x3FF00044
	__I u32	dummy48;		// 0x3FF00048
	__I u32	dummy4C;		// 0x3FF0004C
	__IO u32	otp_mac0;	// 0x3FF00050
	__IO u32	otp_mac1;	// 0x3FF00054
	__IO u32	chip_id;	// 0x3FF00058
} DPORT_typedef;

#define DPORT_BaseAddress		0x3FF00000

#define DPORT 		((DPORT_typedef *)DPORT_BaseAddress)


/**************** DPORT reset value *******************************************/

#define DPORT_NMI_INT_RESET_VALUE			((u32)0x00000000)



/**************** DPORT bit mask **********************************************/

#define DPORT_CONTROL_DISABLE_MASK		((u32)0x0000007E) // Disable WDT

#define DPORT_REG1_WDT_EDGE_IN_EN		((u32)0x00000001)	// bit0: WDT edge int enable
#define DPORT_REG1_TIMER0_EDGE_IN_EN	((u32)0x00000002)	// bit1: TIMER0 edge int enable

#define DPORT_REG3_CASHE_FLUSH_START	((u32)0x00000001)	// bit0: cache flush start bit
#define DPORT_REG3_CASHE_EMPTY			((u32)0x00000002)	// bit1: cache empty flag bit
#define DPORT_REG3_WAIT_SPI_IDLE		((u32)0x00000200)	// bit9: use wait SPI idle

#define DPORT_REG5_CLK_PRE				((u32)0x00000001)	// bit0: =1 CPU 160 MHz, = 0 CPU 80 MHz 

#define DPORT_REG6_CLKGATE_WDT_ON		((u32)0x00000077)	// if(flg) 0x3FF00018 &= 0x77 
#define DPORT_REG6_CLKGATE_WDT_OFF		((u32)0x00000008)	// else 0x3FF00018 |= 8 

#define DPORT_REG8_UART0_ISR			((u32)0x00000001)	// bit0 is for uart0 isr
#define DPORT_REG8_UART1_ISR			((u32)0x00000004)	// bit2 is for uart1 isr
#define DPORT_REG8_SPI0_ISR				((u32)0x00000010)	// bit4 is for spi isr
#define DPORT_REG8_SPI1_ISR				((u32)0x00000080)	// bit7 is for hspi isr
#define DPORT_REG8_I2S_ISR				((u32)0x00000200)	// bit9 is for i2s isr

#define DPORT_REG9_SPI_8000				((u32)0x00000080)	// bit7 16k IRAM base 0x40108000 = SPI cache flash
#define DPORT_REG9_SPI_C000				((u32)0x00000100)	// bit8 16k IRAM base 0x4010C000 = SPI cache flash

#define DPORT_REG10_SWAP_UART			((u32)0x00000001)	// bit0: swap two uart
#define DPORT_REG10_SWAP_SPI			((u32)0x00000002)	// bit1: swap two spi
#define DPORT_REG10_SWAP_UART0			((u32)0x00000004)	// bit2: swap uart0 pins (u0rxd <-> u0cts), (u0txd <-> u0rts)
#define DPORT_REG10_SWAP_UART1			((u32)0x00000008)	// bit3: swap uart1 pins (u1rxd <-> u1cts), (u1txd <-> u1rts)
#define DPORT_REG10_SPI1_HI_PRIOR		((u32)0x00000020)	// bit5: hspi is with the higher prior
#define DPORT_REG10_TWO_ON_SPI1			((u32)0x00000040)	// bit6: two spi masters on hspi
#define DPORT_REG10_TWO_ON_CSPI			((u32)0x00000080)	// bit7: two spi masters on cspi (reg_cspi_overlap)

#define DPORT_REG11_TX_DESC_DEBUG		((u32)0x0000CCCC)	// [15:0] set to 0xcccc 


#endif /* _DPORT_REGISTERS_H_ */
