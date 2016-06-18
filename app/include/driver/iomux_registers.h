/*
 * iomux_register.h
 *
 *  Created on: 17.05.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _IOMUX_REGISTER_H_
#define _IOMUX_REGISTER_H_

#include "c_types.h"

#ifndef __I
#define	__I		volatile const	/*!< defines 'read only' permissions	*/
#endif	// #ifndef __I
#ifndef __O
#define	__O		volatile		/*!< defines 'write only' permissions	*/
#endif	// #ifndef __O
#ifndef __IO
#define	__IO	volatile		/*!< defines 'read / write' permissions*/
#endif	// #ifndef __IO

// correct bit ascending
// WRITE_PERI_REG (PERIPHS_IO_MUX, 0x105)
typedef struct
{
	__IO unsigned	spi0_0			:1;	// SPI0 ?
	__IO unsigned	dummy1			:1;
	__IO unsigned	spi0_2			:1;	// SPI0 ?
	__IO unsigned	dummy3			:1;
	__IO unsigned	dummy4			:1;
	__IO unsigned	dummy5			:1;
	__IO unsigned	dummy6			:1;
	__IO unsigned	dummy7			:1;
	__IO unsigned	spi0_sys_clk	:1;	// BIT 8 SPI0_CLK_EQU_SYS_CLK
	__IO unsigned	spi1_sys_clk	:1; // BIT 9 SPI1_CLK_EQU_SYS_CLK
	__IO unsigned	dummy10			:1;
	__IO unsigned	dummy11			:1;
	__IO unsigned	dummy			:20;
} gpio_mux_cfg_t;

typedef struct
{
	__IO unsigned	func_sel_0		:1;
	__IO unsigned	func_sel_1		:1;
	__IO unsigned	dummy2			:1;
	__IO unsigned	pulldown		:1;
	__IO unsigned	dummy4			:1;
	__IO unsigned	sleep_pulldown	:1;
	__IO unsigned	func_sel_2		:1;
	__IO unsigned	dummy			:25;
} gpio16_mux_t;

typedef struct
{
	__IO unsigned	sleep_oe		:1;
	__IO unsigned	sleep_sel		:1;
	__IO unsigned	sleep_pullup2	:1;
	__IO unsigned	sleep_pullup	:1;
	__IO unsigned	func_sel_0		:1;
	__IO unsigned	func_sel_1		:1;
	__IO unsigned	pullup2			:1;
	__IO unsigned	pullup			:1;
	__IO unsigned	func_sel_2		:1;
	__IO unsigned	dummy			:23;
} gpio_mux_t;

typedef struct
{
	union									// 0x60000800
	{
		__IO u32 			gpio_mux_cfg;
		__IO gpio_mux_cfg_t	gpio_mux_cfg_bits;
	};
	union									// 0x60000804
	{
		__IO u32			gpio12_mux;
		__IO u32			gpio12_mux_mtdi;
		__IO u32			gpio12_mux_i2si_data;
		__IO u32			gpio12_mux_spi1_miso;
		__IO u32			gpio12_mux_u0_dtr;
		__IO gpio_mux_t		gpio12_mux_bits;
	};
	union									// 0x60000808
	{
		__IO u32			gpio13_mux;
		__IO u32			gpio13_mux_mtck;
		__IO u32			gpio13_mux_i2si_bck;
		__IO u32			gpio13_mux_spi1_mosi;
		__IO u32			gpio13_mux_u0_cts;
		__IO gpio_mux_t		gpio13_mux_bits;
	};
	union									// 0x6000080C
	{
		__IO u32			gpio14_mux;
		__IO u32			gpio14_mux_mtms;
		__IO u32			gpio14_mux_i2si_ws;
		__IO u32			gpio14_mux_spi1_clk;
		__IO u32			gpio14_mux_u0_dsr;
		__IO gpio_mux_t		gpio14_mux_bits;
	};
	union									// 0x60000810
	{
		__IO u32			gpio15_mux;
		__IO u32			gpio15_mux_mtdo;
		__IO u32			gpio15_mux_i2so_bck;
		__IO u32			gpio15_mux_spi1_cs;
		__IO u32			gpio15_mux_u0_rts;
		__IO gpio_mux_t		gpio15_mux_bits;
	};
	union									// 0x60000814
	{
		__IO u32			gpio3_mux;
		__IO u32			gpio3_mux_u0_rxd;
		__IO u32			gpio3_mux_i2so_data;
		__IO u32			gpio3_mux_clk_xtal;
		__IO gpio_mux_t		gpio3_mux_bits;
	};
	union									// 0x60000818
	{
		__IO u32			gpio1_mux;
		__IO u32			gpio1_mux_u0_txd;
		__IO u32			gpio1_mux_spi0_cs1;
		__IO u32			gpio1_mux_clk_rtc;
		__IO gpio_mux_t		gpio1_mux_bits;
	};
	union									// 0x6000081C
	{
		__IO u32			gpio6_mux;
		__IO u32			gpio6_mux_sd_clk;
		__IO u32			gpio6_mux_spi0_clk;
		__IO u32			gpio6_mux_u1_cts;
		__IO gpio_mux_t		gpio6_mux_bits;
	};
	union									// 0x60000820
	{
		__IO u32			gpio7_mux;
		__IO u32			gpio7_mux_sd_data0;
		__IO u32			gpio7_mux_spi0_q;
		__IO u32			gpio7_mux_u1_txd;
		__IO gpio_mux_t		gpio7_mux_bits;
	};
	union									// 0x60000824
	{
		__IO u32			gpio8_mux;
		__IO u32			gpio8_mux_sd_data1;
		__IO u32			gpio8_mux_spi0_d;
		__IO u32			gpio8_mux_u1_rxd;
		__IO gpio_mux_t		gpio8_mux_bits;
	};
	union									// 0x60000828
	{
		__IO u32			gpio9_mux;
		__IO u32			gpio9_mux_sd_data2;
		__IO u32			gpio9_mux_spi0_hd;
		__IO u32			gpio9_mux_spi1_hd;
		__IO gpio_mux_t		gpio9_mux_bits;
	};
	union									// 0x6000082C
	{
		__IO u32			gpio10_mux;
		__IO u32			gpio10_mux_sd_data3;
		__IO u32			gpio10_mux_spi0_wp;
		__IO u32			gpio10_mux_spi1_wp;
		__IO gpio_mux_t		gpio10_mux_bits;
	};
	union									// 0x60000830
	{
		__IO u32			gpio11_mux;
		__IO u32			gpio11_mux_sd_cmd;
		__IO u32			gpio11_mux_spi0_cs0;
		__IO u32			gpio11_mux_u1_rts;
		__IO gpio_mux_t		gpio11_mux_bits;
	};
	union									// 0x60000834
	{
		__IO u32			gpio0_mux;
		__IO u32			gpio0_mux_spi0_cs2;
		__IO u32			gpio0_mux_clk_out;
		__IO gpio_mux_t		gpio0_mux_bits;
	};
	union									// 0x60000838
	{
		__IO u32			gpio2_mux;
		__IO u32			gpio2_mux_i2so_ws;
		__IO u32			gpio2_mux_u1_txd;
		__IO u32			gpio2_mux_u0_txd;
		__IO gpio_mux_t		gpio2_mux_bits;
	};
	union									// 0x6000083C
	{
		__IO u32			gpio4_mux;
		__IO u32			gpio4_mux_clk_xtal;
		__IO gpio_mux_t		gpio4_mux_bits;
	};
	union									// 0x60000840
	{
		__IO u32			gpio5_mux;
		__IO u32			gpio5_mux_clk_rtc;
		__IO gpio_mux_t		gpio5_mux_bits;
	};
} IOMUX_typedef;

#define	IOMUX_base_address		0x60000800

#define	IOMUX			((IOMUX_typedef *)IOMUX_base_address)

// address registers in downcase number
/*__IO gpio_mux_t * const gpio_mux_registers[16] =
    {
        &IOMUX->gpio0_mux_bits,
        &IOMUX->gpio1_mux_bits,
        &IOMUX->gpio2_mux_bits,
        &IOMUX->gpio3_mux_bits,
        &IOMUX->gpio4_mux_bits,
        &IOMUX->gpio5_mux_bits,
        &IOMUX->gpio6_mux_bits,
        &IOMUX->gpio7_mux_bits,
        &IOMUX->gpio8_mux_bits,
        &IOMUX->gpio9_mux_bits,
        &IOMUX->gpio10_mux_bits,
        &IOMUX->gpio11_mux_bits,
        &IOMUX->gpio12_mux_bits,
        &IOMUX->gpio13_mux_bits,
        &IOMUX->gpio14_mux_bits,
        &IOMUX->gpio15_mux_bits
    };*/
// reset states
#define	GPIO_MUX_CFG_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO0_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO1_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO2_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO3_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO4_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO5_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO6_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO7_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO8_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO9_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO10_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO11_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO12_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO13_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO14_RESET_VALUE		((u32)0x00000000)
#define	GPIO_MUX_GPIO15_RESET_VALUE		((u32)0x00000000)

// bitfield mask
#define	GPIO_MUX_CFG_MAGIC			((u32)0x00000105)
#define	GPIO_MUX_CFG_SPI0_0			((u32)0x00000001)
#define	GPIO_MUX_CFG_SPI0_2			((u32)0x00000004)
#define	GPIO_MUX_CFG_SPI0_SYS_CLK	((u32)0x00000100)
#define	GPIO_MUX_CFG_SPI1_SYS_CLK	((u32)0x00000200)

#define	GPIO_MUX_FUNC_MASK			((u32)0x00000130)

#define	GPIO_MUX_SLEEP_OE			((u32)0x00000001)
#define	GPIO_MUX_SLEEP_SEL			((u32)0x00000002)
#define	GPIO_MUX_SLEEP_PULLUP2		((u32)0x00000004)
#define	GPIO_MUX_SLEEP_PULLUP		((u32)0x00000008)
#define	GPIO_MUX_FUNC_SEL0			((u32)0x00000010)
#define	GPIO_MUX_FUNC_SEL1			((u32)0x00000020)
#define	GPIO_MUX_PULLUP2			((u32)0x00000040)
#define	GPIO_MUX_PULLUP				((u32)0x00000080)
#define	GPIO_MUX_FUNC_SEL2			((u32)0x00000100)

#define	GPIO16_MUX_FUNC_SEL0		((u32)0x00000001)
#define	GPIO16_MUX_FUNC_SEL1		((u32)0x00000002)
#define	GPIO16_MUX_PULLDOWN			((u32)0x00000008)
#define	GPIO16_MUX_SLEEP_PULLDOWN	((u32)0x00000020)
#define	GPIO16_MUX_	FUNC_SEL2		((u32)0x00000040)

#define	GPIO_MUX_FUNC_MASK		((u32)0x00000130)
#define	GPIO_MUX_FUNC_0			((u32)0x00000000)
#define	GPIO_MUX_FUNC_1			((u32)0x00000010)
#define	GPIO_MUX_FUNC_2			((u32)0x00000020)
#define	GPIO_MUX_FUNC_3			((u32)0x00000030)
#define	GPIO_MUX_FUNC_4			((u32)0x00000100)

#define	GPIO16_MUX_FUNC_MASK	((u32)0x00000043)
#define	GPIO16_MUX_FUNC_0		((u32)0x00000000)
#define	GPIO16_MUX_FUNC_1		((u32)0x00000001)
#define	GPIO16_MUX_FUNC_2		((u32)0x00000002)
#define	GPIO16_MUX_FUNC_3		((u32)0x00000003)
#define	GPIO16_MUX_FUNC_4		((u32)0x00000040)

#define	GPIO0_FUNC_GPIO			GPIO_MUX_FUNC_0
#define	GPIO0_FUNC_SPI0_CS2		GPIO_MUX_FUNC_1
#define	GPIO0_FUNC_CLKOUT		GPIO_MUX_FUNC_4

#define	GPIO1_FUNC_U0TXD		GPIO_MUX_FUNC_0
#define	GPIO1_FUNC_SPI0_CS1		GPIO_MUX_FUNC_1
#define	GPIO1_FUNC_GPIO			GPIO_MUX_FUNC_3
#define	GPIO1_FUNC_CLKRTC		GPIO_MUX_FUNC_4

#define	GPIO2_FUNC_GPIO			GPIO_MUX_FUNC_0
#define	GPIO2_FUNC_I2SO_WS		GPIO_MUX_FUNC_1
#define	GPIO2_FUNC_U1TXD		GPIO_MUX_FUNC_2
#define	GPIO2_FUNC_U0TXD		GPIO_MUX_FUNC_4

#define	GPIO3_FUNC_U0RXD		GPIO_MUX_FUNC_0
#define	GPIO3_FUNC_I2SO_DATA	GPIO_MUX_FUNC_1
#define	GPIO3_FUNC_GPIO			GPIO_MUX_FUNC_3
#define	GPIO3_FUNC_CLK_XTAL		GPIO_MUX_FUNC_4

#define	GPIO4_FUNC_GPIO			GPIO_MUX_FUNC_0
#define	GPIO4_FUNC_CLK_XTAL		GPIO_MUX_FUNC_1

#define	GPIO5_FUNC_GPIO			GPIO_MUX_FUNC_0
#define	GPIO5_FUNC_CLK_RTC		GPIO_MUX_FUNC_1

#define	GPIO6_FUNC_SD_CLK		GPIO_MUX_FUNC_0
#define	GPIO6_FUNC_SPI0_CLK		GPIO_MUX_FUNC_1
#define	GPIO6_FUNC_GPIO			GPIO_MUX_FUNC_3
#define	GPIO6_FUNC_U1CTS		GPIO_MUX_FUNC_4

#define	GPIO7_FUNC_SD_DATA0		GPIO_MUX_FUNC_0
#define	GPIO7_FUNC_SPI0_Q		GPIO_MUX_FUNC_1
#define	GPIO7_FUNC_GPIO			GPIO_MUX_FUNC_3
#define	GPIO7_FUNC_U1TXD		GPIO_MUX_FUNC_4

#define	GPIO8_FUNC_SD_DATA1		GPIO_MUX_FUNC_0
#define	GPIO8_FUNC_SPI0_D			GPIO_MUX_FUNC_1
#define	GPIO8_FUNC_GPIO			GPIO_MUX_FUNC_3
#define	GPIO8_FUNC_U1RXD		GPIO_MUX_FUNC_4

#define	GPIO9_FUNC_SD_DATA2		GPIO_MUX_FUNC_0
#define	GPIO9_FUNC_SPI0_HD		GPIO_MUX_FUNC_1
#define	GPIO9_FUNC_GPIO			GPIO_MUX_FUNC_3
#define	GPIO9_FUNC_SPI1_HD		GPIO_MUX_FUNC_4

#define	GPIO10_FUNC_SD_DATA3	GPIO_MUX_FUNC_0
#define	GPIO10_FUNC_SPI0_WP		GPIO_MUX_FUNC_1
#define	GPIO10_FUNC_GPIO		GPIO_MUX_FUNC_3
#define	GPIO10_FUNC_SPI1_WP		GPIO_MUX_FUNC_4

#define	GPIO11_FUNC_SD_CMD		GPIO_MUX_FUNC_0
#define	GPIO11_FUNC_SPI0_CS0	GPIO_MUX_FUNC_1
#define	GPIO11_FUNC_GPIO		GPIO_MUX_FUNC_3
#define	GPIO11_FUNC_U1RTS		GPIO_MUX_FUNC_4

#define	GPIO12_FUNC_MTDI		GPIO_MUX_FUNC_0
#define	GPIO12_FUNC_I2SI_DATA	GPIO_MUX_FUNC_1
#define	GPIO12_FUNC_SPI1_Q_MISO	GPIO_MUX_FUNC_2
#define	GPIO12_FUNC_GPIO		GPIO_MUX_FUNC_3
#define	GPIO12_FUNC_U0DTR		GPIO_MUX_FUNC_4

#define	GPIO13_FUNC_MTCK		GPIO_MUX_FUNC_0
#define	GPIO13_FUNC_I2SI_BCK	GPIO_MUX_FUNC_1
#define	GPIO13_FUNC_SPI1_D_MOSI	GPIO_MUX_FUNC_2
#define	GPIO13_FUNC_GPIO		GPIO_MUX_FUNC_3
#define	GPIO13_FUNC_U0CTS		GPIO_MUX_FUNC_4

#define	GPIO14_FUNC_MTMS		GPIO_MUX_FUNC_0
#define	GPIO14_FUNC_I2SI_WS		GPIO_MUX_FUNC_1
#define	GPIO14_FUNC_SPI1_CLK	GPIO_MUX_FUNC_2
#define	GPIO14_FUNC_GPIO		GPIO_MUX_FUNC_3
#define	GPIO14_FUNC_U0DSR		GPIO_MUX_FUNC_4

#define	GPIO15_FUNC_MTDO		GPIO_MUX_FUNC_0
#define	GPIO15_FUNC_I2SO_BCK	GPIO_MUX_FUNC_1
#define	GPIO15_FUNC_SPI1_CS		GPIO_MUX_FUNC_2
#define	GPIO15_FUNC_GPIO		GPIO_MUX_FUNC_3
#define	GPIO15_FUNC_U0RTS		GPIO_MUX_FUNC_4

#define	GPIO16_FUNC_XPD_DCDC	GPIO16_MUX_FUNC_0
#define	GPIO16_FUNC_RTC_GPIO0	GPIO16_MUX_FUNC_1
#define	GPIO16_FUNC_EXT_WAKEUP	GPIO16_MUX_FUNC_2
#define	GPIO16_FUNC_DEEP_SLEEP	GPIO16_MUX_FUNC_3
#define	GPIO16_FUNC_BT_XTAL_EN	GPIO16_MUX_FUNC_4

#define VAL_MUX_GPIO0_SDK_DEF	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO0, input
#define VAL_MUX_GPIO1_SDK_DEF	0		// UART0, TX0, Output
#define VAL_MUX_GPIO2_SDK_DEF	0		// UART1, TX1, Output
#define VAL_MUX_GPIO3_SDK_DEF	(1<<GPIO_MUX_PULLUP_BIT)	// UART0, RX0, Input
#define VAL_MUX_GPIO4_SDK_DEF	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO4, input
#define VAL_MUX_GPIO5_SDK_DEF	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO5, input
#define VAL_MUX_GPIO6_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0CLK
#define VAL_MUX_GPIO7_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0Q
#define VAL_MUX_GPIO8_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0D
#define VAL_MUX_GPIO9_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0HD
#define VAL_MUX_GPIO10_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0WP
#define VAL_MUX_GPIO11_SDK_DEF	(1<<GPIO_MUX_FUN_BIT0)	// SPI0CS0
#define VAL_MUX_GPIO12_SDK_DEF	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO12, input
#define VAL_MUX_GPIO13_SDK_DEF	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO13, input
#define VAL_MUX_GPIO14_SDK_DEF	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO14, input
#define VAL_MUX_GPIO15_SDK_DEF	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO15, input

#define VAL_MUX_GPIO0_IOPORT	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO0, input
#define VAL_MUX_GPIO1_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// UART0, TX0, Output
#define VAL_MUX_GPIO2_IOPORT	(1<<GPIO_MUX_PULLUP_BIT)	// UART1, TX1, Output
#define VAL_MUX_GPIO3_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// UART0, RX0, Input
#define VAL_MUX_GPIO4_IOPORT	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO4, input
#define VAL_MUX_GPIO5_IOPORT	(1<<GPIO_MUX_PULLUP_BIT)	// GPIO5, input
#define VAL_MUX_GPIO6_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0CLK
#define VAL_MUX_GPIO7_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0Q
#define VAL_MUX_GPIO8_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0D
#define VAL_MUX_GPIO9_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0HD
#define VAL_MUX_GPIO10_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0WP
#define VAL_MUX_GPIO11_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// SPI0CS0
#define VAL_MUX_GPIO12_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO12, input
#define VAL_MUX_GPIO13_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO13, input
#define VAL_MUX_GPIO14_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO14, input
#define VAL_MUX_GPIO15_IOPORT	((1<<GPIO_MUX_FUN_BIT0) | (1<<GPIO_MUX_FUN_BIT1) | (1<<GPIO_MUX_PULLUP_BIT))	// GPIO15, input

// С‚Р°Р±Р»РёС†Р° (РїРѕ 4 Р±РёС‚Р° РЅР° РЅРѕРјРµСЂ РїРёРЅР°) Р°РґСЂРµСЃРѕРІ IO_MUX РІ СЃРѕРѕС‚РІРµС‚СЃС‚РІРёРё СЃ РЅРѕРјРµСЂР°РјРё GPIOn
#define _IO_MUX_GPIO ((uint64_t)0x3210BA9876FE4D5Cull)
// РїРѕР»СѓС‡РёС‚СЊ Р°РґСЂРµСЃ IO_MUX РІ СЃРѕРѕС‚РІРµС‚СЃС‚РІРёРё СЃ РЅРѕРјРµСЂРѕРј GPIOn
#define GPIOx_MUX(PIN_NUM) iomux_[1 + (uint32)((_IO_MUX_GPIO >> ((PIN_NUM) << 2)) & 0x0F)]

// С‚Р°Р±Р»РёС†Р° (РїРѕ 2 Р±РёС‚Р° РЅР° РЅРѕРјРµСЂ РїРёРЅР°) РЅРѕРјРµСЂРѕРІ С„СѓРЅРєС†РёР№ РїРёРЅРѕРІ РґР»СЏ СѓСЃС‚Р°РЅРѕРІРєРё РІ СЂРµР¶РёРј GPIO
#define _FUN_IO_PORT  ((uint32_t)0xFFFFF0CCul)
// С‚Р°Р±Р»РёС†Р° (РїРѕ 4 Р±РёС‚Р° РЅР° РЅРѕРјРµСЂ РїРёРЅР°) РЅРѕРјРµСЂРѕРІ С„СѓРЅРєС†РёР№ РїРёРЅРѕРІ РґР»СЏ СѓСЃС‚Р°РЅРѕРІРєРё РІ СЂРµР¶РёРј РїРѕ СѓРјРѕС‡Р°РЅРёСЋ РІ SDK
#define _FUN_DEF_SDK  ((uint64_t)0x3333111111000200ull)

// РїРѕР»СѓС‡РёС‚СЊ РЅРѕРјРµСЂ С„СѓРЅРєС†РёРё РґР»СЏ СѓСЃС‚Р°РЅРѕРІРєРё I/O РїРёРЅР° РІ СЂРµР¶РёРј РїРѕСЂС‚Р° GPIOn
#define MUX_FUN_IO_PORT(PIN_NUM) ((uint32_t)(_FUN_IO_PORT >> (PIN_NUM << 1)) & 0x03)
// РїРѕР»СѓС‡РёС‚СЊ РЅРѕРјРµСЂ С„СѓРЅРєС†РёРё РґР»СЏ СѓСЃС‚Р°РЅРѕРІРєРё I/O РїРёРЅР° РІ СЂРµР¶РёРј РїРѕ СѓРјРѕС‡Р°РЅРёСЋ РІ SDK
#define MUX_FUN_DEF_SDK(PIN_NUM) ((uint32_t)(_FUN_DEF_SDK >> (PIN_NUM << 2)) & 0x07)

#define SET_PIN_FUNC(PIN_NUM, FUN) GPIOx_MUX(PIN_NUM) = \
	(GPIOx_MUX (PIN_NUM) & (~GPIO_MUX_FUN_MASK)) \
	| ((FUN & 3) << GPIO_MUX_FUN_BIT0) \
	| ((FUN & 4) << (GPIO_MUX_FUN_BIT2 - 2))
#define GET_PIN_FUNC(PIN_NUM) (((GPIOx_MUX(PIN_NUM) >> GPIO_MUX_FUN_BIT0) & 3) \
		| ((GPIOx_MUX(PIN_NUM) >> (GPIO_MUX_FUN_BIT2 - 2)) & 4))
#define SET_PIN_PULLUP_ENA(PIN_NUM)  GPIOx_MUX(PIN_NUM) |= 1 << GPIO_MUX_PULLUP_BIT
#define SET_PIN_PULLUP_DIS(PIN_NUM) GPIOx_MUX(PIN_NUM) &= ~(1 << GPIO_MUX_PULLUP_BIT)
#define SET_PIN_PULL_DIS(PIN_NUM) GPIOx_MUX(PIN_NUM) &= ~((1 << GPIO_MUX_PULLUP_BIT) | (1 << GPIO_MUX_PULLDOWN_BIT))
#define SET_PIN_PULLDOWN_ENA(PIN_NUM)  GPIOx_MUX(PIN_NUM) |= 1 << GPIO_MUX_PULLDOWN_BIT
#define SET_PIN_PULLDOWN_DIS(PIN_NUM) GPIOx_MUX(PIN_NUM) &= ~(1 << GPIO_MUX_PULLDOWN_BIT)

// СѓСЃС‚Р°РЅРѕРІРёС‚СЊ С„СѓРЅРєС†РёСЋ GPIOn РєР°Рє I/O port
#define SET_PIN_FUNC_IOPORT(PIN_NUM) SET_PIN_FUNC(PIN_NUM, MUX_FUN_IO_PORT(PIN_NUM))
// СѓСЃС‚Р°РЅРѕРІРёС‚СЊ С„СѓРЅРєС†РёСЋ GPIOn РїРѕ СѓРјРѕР»С‡Р°РЅРёСЋ РґР»СЏ SDK
#define SET_PIN_FUNC_DEF_SDK(PIN_NUM) SET_PIN_FUNC(PIN_NUM, MUX_FUN_DEF_SDK(PIN_NUM))

/* GPIOx_MUX:	0x60000800 */

#define MUX_CFG_MASK	0xFFF
#define MUX_SPI0_CLK_BIT	8 // =1 пїЅпїЅпїЅ 80 MHz, =0 пїЅпїЅпїЅ 40, 26, 20 MHz пїЅпїЅ SPI Flash

//#define PIN_PULLUP_DIS(PIN_NAME)		CLEAR_PERI_REG_MASK (PIN_NAME, PERIPHS_IO_MUX_PULLUP)
//#define PIN_PULLUP_EN(PIN_NAME)			SET_PERI_REG_MASK (PIN_NAME, PERIPHS_IO_MUX_PULLUP)

/*#define AND_PIN_FUNC_SELECT			(~(PERIPHS_IO_MUX_FUNC<<PERIPHS_IO_MUX_FUNC_S))
 #define OR_PIN_FUNC_SELECT(FUNC)	((((FUNC&BIT2) << 2)|(FUNC&0x3))<<PERIPHS_IO_MUX_FUNC_S)

 #define PIN_FUNC_SELECT(PIN_NAME, FUNC)  do { \
	WRITE_PERI_REG (PIN_NAME, \
		(READ_PERI_REG (PIN_NAME) & AND_PIN_FUNC_SELECT) | OR_PIN_FUNC_SELECT(FUNC)); \
} while (0)
 */

//#define	PIN_FUNC_SELECT(PIN, FUNC)	(PIN = (PIN & GPIO_MUX_FUNC_MASK) | (FUNC))


#endif /* _IOMUX_REGISTER_H_ */
