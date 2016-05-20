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

typedef struct
{
	__I u32			dummy2				:30;
	__IO unsigned	timer0_edge_int_en	:1;	// bit1: TIMER0 edge int enable
	__IO unsigned	wdt_edge_int_en		:1;	// bit0: WDT edge int enable
} dport_reg_1_t;

typedef struct
{
	__I u32			dummy10			:22;
	__IO unsigned	wait_spi_idle	:1;	// bit9: use wait SPI idle
	__I u8			dummy2			:7;
	__IO unsigned	cashe_empty		:1;	// bit1: cache empty flag bit
	__IO unsigned	cashe_flush		:1;	// bit0: cache flush start bit
} dport_reg_3_t;

typedef struct
{
	__I u32			dummy1			:31;
	__IO unsigned	clk_pre			:1;	// bit0: =1 CPU 160 MHz, = 0 CPU 80 MHz
} dport_reg_5_t;

typedef struct
{
	__I u32			dummy1			:31;
	__IO unsigned	clk_pre			:1;	// bit0: =1 CPU 160 MHz, = 0 CPU 80 MHz
} dport_reg_6_t;

typedef struct
{
	__I u32			dummy10			:22;
	__IO unsigned	i2s_isr			:1;	// bit9 is for i2s isr
	__I unsigned	dummy8			:1;
	__IO unsigned	spi1_isr		:1;	// bit7 is for hspi isr
	__I unsigned	dummy6			:1;
	__I unsigned	dummy5			:1;
	__IO unsigned	spi0_isr		:1;	// bit4 is for spi isr
	__I unsigned	dummy3			:1;
	__IO unsigned	uart1_isr		:1;	// bit2 is for uart1 isr
	__I unsigned	dummy1			:1;
	__IO unsigned	uart0_isr		:1;	// bit0 is for uart0 isr
} dport_reg_8_t;

typedef struct
{
	__I u32			dummy9			:23;
	__IO unsigned	spi_C000		:1;	// bit8 16k IRAM base 0x4010C000 = SPI cache flash
	__IO unsigned	spi_8000		:1;	// bit7 16k IRAM base 0x40108000 = SPI cache flash
	__I u8			dummy0			:7;
} dport_reg_9_t;

typedef struct
{
	__I u32			dummy8			:24;
	__IO unsigned	two_on_cspi		:1;	// bit7: two spi masters on cspi (reg_cspi_overlap)
	__IO unsigned	two_on_spi1		:1;	// bit6: two spi masters on hspi
	__IO unsigned	spi1_high_prior	:1;	// bit5: hspi is with the higher prior
	__I unsigned	dummy4			:1;
	__IO unsigned	swap_uart1		:1;	// bit3: swap uart1 pins (u1rxd <-> u1cts), (u1txd <-> u1rts)
	__IO unsigned	swap_uart0		:1;	// bit2: swap uart0 pins (u0rxd <-> u0cts), (u0txd <-> u0rts)
	__IO unsigned	swap_two_spi	:1;	// bit1: swap two spi
	__IO unsigned	swap_two_uart	:1;	// bit0: swap two uart
} dport_reg_10_t;

typedef struct
{
	__I u16			dummy16			:16;
	__IO u16		tx_desc_debug	:16;	// SLC_TX_DESC_DEBUG[15:0] set to 0xCCCC
} dport_reg_11_t;

typedef struct
{
	__IO u32	nmi_int;
	union
	{
		__IO u32			reg_1;
		__IO dport_reg_1_t	reg_1_bits;
	};
	__IO u32	reg_2;
	union
	{
		__IO u32			reg_3;
		__IO dport_reg_3_t	reg_3_bits;
	};
	__IO u32	reg_4;
	union
	{
		__IO u32			reg_5;
		__IO dport_reg_5_t	reg_5_bits;
	};
	union
	{
		__IO u32			reg_6;	// use clockgate_watchdog(flg) { if(flg) 0x3FF00018 &= 0x77; else 0x3FF00018 |= 8; }
		__IO dport_reg_6_t	reg_6_bits;
	};
	__IO u32	reg_7;
	union
	{
		__IO u32			reg_8;	// 0x3ff00020 is isr flag register, (ESP8266 SPI Module User Guide)
		__IO dport_reg_8_t	reg_8_bits;
	};
	union
	{
		__IO u32			reg_9;	// IRAM base = SPI cache flash
		__IO dport_reg_9_t	reg_9_bits;
	};
	union
	{
		__IO u32			reg_10;	// PERI_IO_SWAP: 0x3FF00028
		__IO dport_reg_10_t	reg_10_bits;
	};
	union
	{
		__IO u32			reg_11;	// SLC_TX_DESC_DEBUG_REG: 0x3ff0002c
		__IO dport_reg_11_t	reg_11_bits;
	};
	__I u32	dummy30;
	__I u32	dummy34;
	__I u32	dummy38;
	__I u32	dummy3C;
	__I u32	dummy40;
	__I u32	dummy44;
	__I u32	dummy48;
	__I u32	dummy4C;
	__IO u32	otp_mac0;
	__IO u32	otp_mac1;
	__IO u32	chip_id;
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
