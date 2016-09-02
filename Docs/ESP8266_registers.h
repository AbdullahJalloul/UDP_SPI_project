/*
* All registers of ESP8266 in STM style
* Created on 2016.08.21
* Author Ilya Petrukhun
*/

#ifndef _ESP8266_REGISTERS_H_
#define _ESP8266_REGISTERS_H_

#include	"c_types.h"
#include	"bits.h"

#ifndef	__I
#define	__I     volatile const   /*!< defines 'read only' permissions     */
#endif	// #ifndef __I

#ifndef __O
#define	__O     volatile         /*!< defines 'write only' permissions    */
#endif	// #ifndef __O

#ifndef __IO
#define	__IO    volatile         /*!< defines 'read / write' permissions  */
#endif	// #ifndef __IO

/******************* DPORT bitfields ******************************************/
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

// 40108000h Instruction RAM/FLASH cache ram. OTA bootloader uses it. Mapped here if bit 4 (bit mask 0x10) is clear in the dport0 register 0x3ff00024
// 4010C000h Instruction RAM/FLASH cache ram. Mapped here if bit 3 (bit mask 0x8) is clear in the dport0 register 0x3ff00024
typedef struct		// 0x3ff00024
{
	__I unsigned	dummy0			:3;
	__IO unsigned	cash_C000		:1;	// bit3 Mapped 4010C000h RAM/FLASH here if bit 3 is clear
	__IO unsigned	cash_8000		:1;	// bit4 Mapped 40108000h RAM/FLASH here if bit 4 is clear
	__I unsigned	dummy5			:2;
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


/*********************** IO2 registers ****************************************/
// WiFi registers
typedef struct
{
	__IO u32	reg_0;	// 0x3FF20000
	__IO u32	reg_1;	// 0x3FF20004
	__IO u32	reg_2;	// 0x3FF20008
	__IO u32	reg_3;	// 0x3FF2000C
	__IO u32	reg_4;	// 0x3FF20010
	__IO u32	reg_5;	// 0x3FF20014
	__IO u32	reg_6;	// 0x3FF20018
	__IO u32	reg_7;	// 0x3FF2001C
	__IO u32	reg_8;	// 0x3FF20020
	__IO u32	reg_9;	// 0x3FF20024
	__IO u32	reg_10;	// 0x3FF20028
	__IO u32	reg_11;	// 0x3FF2002C
	__IO u32	reg_12;	// 0x3FF20030
	__IO u32	reg_13;	// 0x3FF20034
	__IO u32	reg_14;	// 0x3FF20038
	__IO u32	reg_15;	// 0x3FF2003C
	__IO u32	reg_16;	// 0x3FF20040
	__IO u32	reg_17;	// 0x3FF20044
	__IO u32	reg_18;	// 0x3FF20048
	__IO u32	reg_19;	// 0x3FF2004C
	__IO u32	reg_20;	// 0x3FF20050
	__IO u32	reg_21;	// 0x3FF20054
	__IO u32	reg_22;	// 0x3FF20058
	__IO u32	reg_23;	// 0x3FF2005C
	__IO u32	reg_24;	// 0x3FF20060
	__IO u32	reg_25;	// 0x3FF20064
	__IO u32	reg_26;	// 0x3FF20068
	__IO u32	reg_27;	// 0x3FF2006C
	__IO u32	reg_28;	// 0x3FF20070
	__IO u32	reg_29;	// 0x3FF20074
	__IO u32	reg_30;	// 0x3FF20078
	__IO u32	reg_31;	// 0x3FF2007C
} IO2_typedef;


#define IO2_BaseAddress		0x3FF20000
#define IO2 		((IO2_typedef *)IO2_BaseAddress)

/******************* random generator *****************************************/
typedef struct
{
	__I u32	reg;
} RANDOM_typedef;
#define RANDOM_BaseAddress	0x3FF20E44
#define RANDOM		((RANDOM_typedef *)RANDOM_BaseAddress)

/*********************** GPIO structures **************************************/

typedef struct
{
	__IO unsigned	pin_0	:1;
	__IO unsigned	pin_1	:1;
	__IO unsigned	pin_2	:1;
	__IO unsigned	pin_3	:1;
	__IO unsigned	pin_4	:1;
	__IO unsigned	pin_5	:1;
	__IO unsigned	pin_6	:1;
	__IO unsigned	pin_7	:1;
	__IO unsigned	pin_8	:1;
	__IO unsigned	pin_9	:1;
	__IO unsigned	pin_10	:1;
	__IO unsigned	pin_11	:1;
	__IO unsigned	pin_12	:1;
	__IO unsigned	pin_13	:1;
	__IO unsigned	pin_14	:1;
	__IO unsigned	pin_15	:1;		// The output value when the GPIO pin is set as output.
	__IO unsigned	bt_sel	:16;	// BT-Coexist Selection register
} gpio_out_t;

typedef struct
{
	__O unsigned	pin_0	:1;
	__O unsigned	pin_1	:1;
	__O unsigned	pin_2	:1;
	__O unsigned	pin_3	:1;
	__O unsigned	pin_4	:1;
	__O unsigned	pin_5	:1;
	__O unsigned	pin_6	:1;
	__O unsigned	pin_7	:1;
	__O unsigned	pin_8	:1;
	__O unsigned	pin_9	:1;
	__O unsigned	pin_10	:1;
	__O unsigned	pin_11	:1;
	__O unsigned	pin_12	:1;
	__O unsigned	pin_13	:1;
	__O unsigned	pin_14	:1;
	__O unsigned	pin_15	:1;
	__O unsigned	dummy16	:16;
} gpio_out_w_t;

typedef struct
{
	__IO unsigned	pin_0	:1;	// The output enable register.
	__IO unsigned	pin_1	:1;
	__IO unsigned	pin_2	:1;
	__IO unsigned	pin_3	:1;
	__IO unsigned	pin_4	:1;
	__IO unsigned	pin_5	:1;
	__IO unsigned	pin_6	:1;
	__IO unsigned	pin_7	:1;
	__IO unsigned	pin_8	:1;
	__IO unsigned	pin_9	:1;
	__IO unsigned	pin_10	:1;
	__IO unsigned	pin_11	:1;
	__IO unsigned	pin_12	:1;
	__IO unsigned	pin_13	:1;
	__IO unsigned	pin_14	:1;
	__IO unsigned	pin_15	:1;	
	__IO unsigned	sdio_sel :6;	// SDIO-dis selection register (sigma-delta mode)
	__IO unsigned	dummy21	:10;
} gpio_enable_t;

typedef struct
{
	__I unsigned	pin_0			:1;	// The values of the GPIO pins when the GPIO pin is set as input.
	__I unsigned	pin_1			:1;
	__I unsigned	pin_2			:1;
	__I unsigned	pin_3			:1;
	__I unsigned	pin_4			:1;
	__I unsigned	pin_5			:1;
	__I unsigned	pin_6			:1;
	__I unsigned	pin_7			:1;
	__I unsigned	pin_8			:1;
	__I unsigned	pin_9			:1;
	__I unsigned	pin_10			:1;
	__I unsigned	pin_11			:1;
	__I unsigned	pin_12			:1;
	__I unsigned	pin_13			:1;
	__I unsigned	pin_14			:1;
	__I unsigned	pin_15			:1;
	__I unsigned	strap_pin_0		:1;	// The values of the strapping pins.
	__I unsigned	strap_pin_1		:1;	// ����� �����, ����������� ����������� �-���
	__I unsigned	strap_pin_2		:1;
	__I unsigned	strap_pin_3		:1;
	__I unsigned	strap_pin_4		:1;
	__I unsigned	strap_pin_5		:1;
	__I unsigned	strap_pin_6		:1;
	__I unsigned	strap_pin_7		:1;
	__I unsigned	strap_pin_8		:1;
	__I unsigned	strap_pin_9		:1;
	__I unsigned	strap_pin_10	:1;
	__I unsigned	strap_pin_11	:1;
	__I unsigned	strap_pin_12	:1;
	__I unsigned	strap_pin_13	:1;
	__I unsigned	strap_pin_14	:1;
	__I unsigned	strap_pin_15	:1;
} gpio_in_t;

typedef struct
{
	__IO unsigned	pin_0	:1;	// Interrupt enable register.
	__IO unsigned	pin_1	:1;
	__IO unsigned	pin_2	:1;
	__IO unsigned	pin_3	:1;
	__IO unsigned	pin_4	:1;
	__IO unsigned	pin_5	:1;
	__IO unsigned	pin_6	:1;
	__IO unsigned	pin_7	:1;
	__IO unsigned	pin_8	:1;
	__IO unsigned	pin_9	:1;
	__IO unsigned	pin_10	:1;
	__IO unsigned	pin_11	:1;
	__IO unsigned	pin_12	:1;
	__IO unsigned	pin_13	:1;
	__IO unsigned	pin_14	:1;
	__IO unsigned	pin_15	:1;
	__IO unsigned	dummy16	:16;
} gpio_int_st_t;

typedef enum _gpio_pin_source_t
{
	GPIO_DATA	= 0,
	SIGMA_DELTA	= 1
} gpio_pin_source_t;

typedef enum _gpio_pin_driver_t
{
	PUSH_PULL	= 0,
	OPEN_DRAIN	= 1
} gpio_pin_driver_t;

typedef enum _gpio_pin_int_t
{
	GPIO_INT_DISABLE	= 0,
	GPIO_INT_POS_EDGE	= 1,
	GPIO_INT_NEG_EDGE	= 2,
	GPIO_INT_ANY_EDGE	= 3,
	GPIO_INT_LOW_LEVEL	= 4,
	GPIO_INT_HIGH_LEVEL	= 5
} gpio_pin_int_t;

typedef struct
{
	__IO unsigned	gpio_source		:1;	// 1: sigma-delta; 0: GPIO_DATA
	__IO unsigned	dummy1			:1;
	__IO unsigned	pin_driver		:1;	// 1: open drain; 0: Push-Pull
	__IO unsigned	dummy3			:4;
	__IO unsigned	pin_int			:3;	// 0: disable; 1: positive edge; 2: negative edge; 3: both types of edge; 4: low-level; 5: high-level
	__IO unsigned	wakeup_enable	:1;	// 0: disable; 1: enable GPIO wakeup CPU, only when GPIO_PIN0_INT_TYPE is 0x4 or 0x5
	__IO unsigned	config			:2;	// 0- 1- 2- 3-
	__IO unsigned	dummy13			:19;
} gpio_pin_t;

typedef struct
{
	__IO unsigned	target		:8;	// target level of the sigma-delta. It is a signed byte.
	__IO unsigned	prescaler	:8;	// Clock pre-divider for sigma-delta.
	__IO unsigned	enable		:1;	// 1: enable sigma-delta; 0: disable
	__IO unsigned	dummy17		:15;
} gpio_sigma_delta_t;

typedef struct
{
	__IO unsigned	rtc_period_num	:10;	// The cycle number of RTC-clock during RTC-clock-calibration
	__IO unsigned	dummy10			:21;
	__IO unsigned	rtc_calib_start	:1;		// Positvie edge of this bit will trigger the RTC-clock-calibration process.
} gpio_rtc_calib_sync_t;

typedef struct
{
	__I unsigned	rtc_calib_value	:20;	// The cycle number of clk_xtal (crystal clock) for the RTC_PERIOD_NUM cycles of RTC-clock
	__I unsigned	dummy20			:10;
	__I unsigned	calib_rdy_real	:1;	// 0: during RTC-clock-calibration; 1: RTC-clock-calibration is done
	__I unsigned	calib_rdy		:1;	// 0: during RTC-clock-calibration; 1: RTC-clock-calibration is done
} gpio_rtc_calib_value_t;

typedef struct GPIO_struct
{
	union							// 0x60000300
	{
		__IO u32			out;
		__IO gpio_out_t 	out_bits;	// The output value when the GPIO pin is set as output.
	};
	union							// 0x60000304
	{
		__O u32				out_w1ts;
		__O gpio_out_w_t	out_w1ts_bits;	// Writing 1 into a bit in this register will set the related bit in GPIO_OUT_DATA
	};
	union							// 0x60000308
	{
		__O u32				out_w1tc;
		__O gpio_out_w_t	out_w1tc_bits;	// Writing 1 into a bit in this register will clear the related bit in GPIO_OUT_DATA
	};
	union							// 0x6000030C
	{
		__IO u32			enable;
		__IO gpio_enable_t	enable_bits;	// The output enable register.
	};
	union							// 0x60000310
	{
		__O u32				enable_w1ts;
		__O gpio_out_w_t	enable_w1ts_bits;	// Writing 1 into a bit in this register will set the related bit in GPIO_ENABLE_DATA
	};
	union							// 0x60000314
	{
		__O u32				enable_w1tc;
		__O gpio_out_w_t	enable_w1tc_bits;	// Writing 1 into a bit in this register will clear the related bit in GPIO_ENABLE_DATA
	};
	union							// 0x60000318
	{
		__IO u32			in;
		__IO gpio_in_t		in_bits;	// The values of the GPIO pins when the GPIO pin is set as input.
	};
	union							// 0x6000031C
	{
		__IO u32			int_st;			// status of interrupts;
		__IO gpio_int_st_t	int_st_bits;	// Interrupt state register.
	};
	union							// 0x60000320
	{
		__O u32				int_st_w1ts;		// status_w1ts;
		__O gpio_out_w_t	int_st_w1ts_bits;	// Writing 1 into a bit in this register will set the related bit in GPIO_STATUS_INTERRUPT
	};
	union							// 0x60000324
	{
		__O u32				int_st_w1tc;
		__O gpio_out_w_t	int_st_w1tc_bits;	// Writing 1 into a bit in this register will clear the related bit in GPIO_STATUS_INTERRUPT
	};
	union							// 0x60000328
	{
		__IO u32			pin_0;
		__IO gpio_pin_t		pin_0_bits;
	};
	union							// 0x6000032C
	{
		__IO u32			pin_1;
		__IO gpio_pin_t		pin_1_bits;
	};
	union							// 0x60000330
	{
		__IO u32			pin_2;
		__IO gpio_pin_t		pin_2_bits;
	};
	union							// 0x60000334
	{
		__IO u32			pin_3;
		__IO gpio_pin_t		pin_3_bits;
	};
	union							// 0x60000338
	{
		__IO u32			pin_4;
		__IO gpio_pin_t		pin_4_bits;
	};
	union							// 0x6000033C
	{
		__IO u32			pin_5;
		__IO gpio_pin_t		pin_5_bits;
	};
	union							// 0x60000340
	{
		__IO u32			pin_6;
		__IO gpio_pin_t		pin_6_bits;
	};
	union							// 0x60000344
	{
		__IO u32			pin_7;
		__IO gpio_pin_t		pin_7_bits;
	};
	union							// 0x60000348
	{
		__IO u32			pin_8;
		__IO gpio_pin_t		pin_8_bits;
	};
	union							// 0x6000034C
	{
		__IO u32			pin_9;
		__IO gpio_pin_t		pin_9_bits;
	};
	union							// 0x60000350
	{
		__IO u32			pin_10;
		__IO gpio_pin_t		pin_10_bits;
	};
	union							// 0x60000354
	{
		__IO u32			pin_11;
		__IO gpio_pin_t		pin_11_bits;
	};
	union							// 0x60000358
	{
		__IO u32			pin_12;
		__IO gpio_pin_t		pin_12_bits;
	};
	union							// 0x6000035C
	{
		__IO u32			pin_13;
		__IO gpio_pin_t		pin_13_bits;
	};
	union							// 0x60000360
	{
		__IO u32			pin_14;
		__IO gpio_pin_t		pin_14_bits;
	};
	union							// 0x60000364
	{
		__IO u32			pin_15;
		__IO gpio_pin_t		pin_15_bits;
	};
	union							// 0x60000368
	{
		__IO u32				sigma_delta;
		__IO gpio_sigma_delta_t	sigma_delta_bits;
	};
	union							// 0x6000036C
	{
		__IO u32					rtc_calib_sync;
		__IO gpio_rtc_calib_sync_t	rtc_calib_sync_bits;
	};
	union							// 0x60000370
	{
		__IO u32					rtc_calib_value;
		__IO gpio_rtc_calib_value_t	rtc_calib_value_bits;
	};
} GPIO_typedef;

#define GPIO_BaseAddress			0x60000300

#define GPIO 		((GPIO_typedef *)GPIO_BaseAddress)

// 
#define GPIO_OUT_RESET_VALUE			((u32)0x00000000)
#define GPIO_OUT_W1TS_RESET_VALUE		((u32)0x00000000)
#define GPIO_OUT_W1TC_RESET_VALUE		((u32)0x00000000)
#define GPIO_ENABLE_RESET_VALUE			((u32)0x00000000)
#define GPIO_ENABLE_W1TS_RESET_VALUE	((u32)0x00000000)
#define GPIO_ENABLE_W1TC_RESET_VALUE	((u32)0x00000000)
#define GPIO_IN_RESET_VALUE				((u32)0x00000000)
#define GPIO_STATUS_RESET_VALUE			((u32)0x00000000)
#define GPIO_STATUS_W1TS_RESET_VALUE	((u32)0x00000000)
#define GPIO_STATUS_W1TC_RESET_VALUE	((u32)0x00000000)
#define GPIO_PIN0_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN1_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN2_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN3_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN4_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN5_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN6_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN7_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN8_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN9_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN10_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN11_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN12_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN13_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN14_RESET_VALUE			((u32)0x00000000)
#define GPIO_PIN15_RESET_VALUE			((u32)0x00000000)
#define GPIO_SIGMA_DELTA_RESET_VALUE	((u32)0x00000000)
#define GPIO_RTC_CALIB_SYNC_RESET_VALUE		((u32)0x00000000)
#define GPIO_RTC_CALIB_VALUE_RESET_VALUE	((u32)0x00000000)

// 
#define GPIO_OUT_BT_SEL_MASK			((u32)0xFFFF0000)
#define GPIO_OUT_DATA_MASK				((u32)0x0000FFFF)
#define GPIO_OUT_DATA_PIN_0				((u32)0x00000001)
#define GPIO_OUT_DATA_PIN_1				((u32)0x00000002)
#define GPIO_OUT_DATA_PIN_2				((u32)0x00000004)
#define GPIO_OUT_DATA_PIN_3				((u32)0x00000008)
#define GPIO_OUT_DATA_PIN_4				((u32)0x00000010)
#define GPIO_OUT_DATA_PIN_5				((u32)0x00000020)
#define GPIO_OUT_DATA_PIN_6				((u32)0x00000040)
#define GPIO_OUT_DATA_PIN_7				((u32)0x00000080)
#define GPIO_OUT_DATA_PIN_8				((u32)0x00000100)
#define GPIO_OUT_DATA_PIN_9				((u32)0x00000200)
#define GPIO_OUT_DATA_PIN_10			((u32)0x00000400)
#define GPIO_OUT_DATA_PIN_11			((u32)0x00000800)
#define GPIO_OUT_DATA_PIN_12			((u32)0x00001000)
#define GPIO_OUT_DATA_PIN_13			((u32)0x00002000)
#define GPIO_OUT_DATA_PIN_14			((u32)0x00004000)
#define GPIO_OUT_DATA_PIN_15			((u32)0x00008000)

#define GPIO_OUT_W1TS_MASK				((u32)0x0000FFFF)
#define GPIO_OUT_W1TS_PIN_0				((u32)0x00000001)
#define GPIO_OUT_W1TS_PIN_1				((u32)0x00000002)
#define GPIO_OUT_W1TS_PIN_2				((u32)0x00000004)
#define GPIO_OUT_W1TS_PIN_3				((u32)0x00000008)
#define GPIO_OUT_W1TS_PIN_4				((u32)0x00000010)
#define GPIO_OUT_W1TS_PIN_5				((u32)0x00000020)
#define GPIO_OUT_W1TS_PIN_6				((u32)0x00000040)
#define GPIO_OUT_W1TS_PIN_7				((u32)0x00000080)
#define GPIO_OUT_W1TS_PIN_8				((u32)0x00000100)
#define GPIO_OUT_W1TS_PIN_9				((u32)0x00000200)
#define GPIO_OUT_W1TS_PIN_10			((u32)0x00000400)
#define GPIO_OUT_W1TS_PIN_11			((u32)0x00000800)
#define GPIO_OUT_W1TS_PIN_12			((u32)0x00001000)
#define GPIO_OUT_W1TS_PIN_13			((u32)0x00002000)
#define GPIO_OUT_W1TS_PIN_14			((u32)0x00004000)
#define GPIO_OUT_W1TS_PIN_15			((u32)0x00008000)

#define GPIO_OUT_W1TC_MASK				((u32)0x0000FFFF)
#define GPIO_OUT_W1TC_PIN_0				((u32)0x00000001)
#define GPIO_OUT_W1TC_PIN_1				((u32)0x00000002)
#define GPIO_OUT_W1TC_PIN_2				((u32)0x00000004)
#define GPIO_OUT_W1TC_PIN_3				((u32)0x00000008)
#define GPIO_OUT_W1TC_PIN_4				((u32)0x00000010)
#define GPIO_OUT_W1TC_PIN_5				((u32)0x00000020)
#define GPIO_OUT_W1TC_PIN_6				((u32)0x00000040)
#define GPIO_OUT_W1TC_PIN_7				((u32)0x00000080)
#define GPIO_OUT_W1TC_PIN_8				((u32)0x00000100)
#define GPIO_OUT_W1TC_PIN_9				((u32)0x00000200)
#define GPIO_OUT_W1TC_PIN_10			((u32)0x00000400)
#define GPIO_OUT_W1TC_PIN_11			((u32)0x00000800)
#define GPIO_OUT_W1TC_PIN_12			((u32)0x00001000)
#define GPIO_OUT_W1TC_PIN_13			((u32)0x00002000)
#define GPIO_OUT_W1TC_PIN_14			((u32)0x00004000)
#define GPIO_OUT_W1TC_PIN_15			((u32)0x00008000)

#define GPIO_ENABLE_MASK				((u32)0x0000FFFF)
#define GPIO_ENABLE_PIN_15				((u32)0x00008000)
#define GPIO_ENABLE_PIN_14				((u32)0x00004000)
#define GPIO_ENABLE_PIN_13				((u32)0x00002000)
#define GPIO_ENABLE_PIN_12				((u32)0x00001000)
#define GPIO_ENABLE_PIN_11				((u32)0x00000800)
#define GPIO_ENABLE_PIN_10				((u32)0x00000400)
#define GPIO_ENABLE_PIN_9				((u32)0x00000200)
#define GPIO_ENABLE_PIN_8				((u32)0x00000100)
#define GPIO_ENABLE_PIN_7				((u32)0x00000080)
#define GPIO_ENABLE_PIN_6				((u32)0x00000040)
#define GPIO_ENABLE_PIN_5				((u32)0x00000020)
#define GPIO_ENABLE_PIN_4				((u32)0x00000010)
#define GPIO_ENABLE_PIN_3				((u32)0x00000008)
#define GPIO_ENABLE_PIN_2				((u32)0x00000004)
#define GPIO_ENABLE_PIN_1				((u32)0x00000002)
#define GPIO_ENABLE_PIN_0				((u32)0x00000001)

#define GPIO_ENABLE_W1TS_MASK			((u32)0x0000FFFF)
#define GPIO_ENABLE_W1TS_PIN_15			((u32)0x00008000)
#define GPIO_ENABLE_W1TS_PIN_14			((u32)0x00004000)
#define GPIO_ENABLE_W1TS_PIN_13			((u32)0x00002000)
#define GPIO_ENABLE_W1TS_PIN_12			((u32)0x00001000)
#define GPIO_ENABLE_W1TS_PIN_11			((u32)0x00000800)
#define GPIO_ENABLE_W1TS_PIN_10			((u32)0x00000400)
#define GPIO_ENABLE_W1TS_PIN_9			((u32)0x00000200)
#define GPIO_ENABLE_W1TS_PIN_8			((u32)0x00000100)
#define GPIO_ENABLE_W1TS_PIN_7			((u32)0x00000080)
#define GPIO_ENABLE_W1TS_PIN_6			((u32)0x00000040)
#define GPIO_ENABLE_W1TS_PIN_5			((u32)0x00000020)
#define GPIO_ENABLE_W1TS_PIN_4			((u32)0x00000010)
#define GPIO_ENABLE_W1TS_PIN_3			((u32)0x00000008)
#define GPIO_ENABLE_W1TS_PIN_2			((u32)0x00000004)
#define GPIO_ENABLE_W1TS_PIN_1			((u32)0x00000002)
#define GPIO_ENABLE_W1TS_PIN_0			((u32)0x00000001)

#define GPIO_ENABLE_W1TC_MASK			((u32)0x0000FFFF)
#define GPIO_ENABLE_W1TC_PIN_15			((u32)0x00008000)
#define GPIO_ENABLE_W1TC_PIN_14			((u32)0x00004000)
#define GPIO_ENABLE_W1TC_PIN_13			((u32)0x00002000)
#define GPIO_ENABLE_W1TC_PIN_12			((u32)0x00001000)
#define GPIO_ENABLE_W1TC_PIN_11			((u32)0x00000800)
#define GPIO_ENABLE_W1TC_PIN_10			((u32)0x00000400)
#define GPIO_ENABLE_W1TC_PIN_9			((u32)0x00000200)
#define GPIO_ENABLE_W1TC_PIN_8			((u32)0x00000100)
#define GPIO_ENABLE_W1TC_PIN_7			((u32)0x00000080)
#define GPIO_ENABLE_W1TC_PIN_6			((u32)0x00000040)
#define GPIO_ENABLE_W1TC_PIN_5			((u32)0x00000020)
#define GPIO_ENABLE_W1TC_PIN_4			((u32)0x00000010)
#define GPIO_ENABLE_W1TC_PIN_3			((u32)0x00000008)
#define GPIO_ENABLE_W1TC_PIN_2			((u32)0x00000004)
#define GPIO_ENABLE_W1TC_PIN_1			((u32)0x00000002)
#define GPIO_ENABLE_W1TC_PIN_0			((u32)0x00000001)

#define GPIO_IN_STRAPPING_MASK			((u32)0xFFFF0000)
#define GPIO_IN_STRAPPING_PIN_15		((u32)0x80000000)
#define GPIO_IN_STRAPPING_PIN_14		((u32)0x40000000)
#define GPIO_IN_STRAPPING_PIN_13		((u32)0x20000000)
#define GPIO_IN_STRAPPING_PIN_12		((u32)0x10000000)
#define GPIO_IN_STRAPPING_PIN_11		((u32)0x08000000)
#define GPIO_IN_STRAPPING_PIN_10		((u32)0x04000000)
#define GPIO_IN_STRAPPING_PIN_9			((u32)0x02000000)
#define GPIO_IN_STRAPPING_PIN_8			((u32)0x01000000)
#define GPIO_IN_STRAPPING_PIN_7			((u32)0x00800000)
#define GPIO_IN_STRAPPING_PIN_6			((u32)0x00400000)
#define GPIO_IN_STRAPPING_PIN_5			((u32)0x00200000)
#define GPIO_IN_STRAPPING_PIN_4			((u32)0x00100000)
#define GPIO_IN_STRAPPING_PIN_3			((u32)0x00080000)
#define GPIO_IN_STRAPPING_PIN_2			((u32)0x00040000)
#define GPIO_IN_STRAPPING_PIN_1			((u32)0x00020000)
#define GPIO_IN_STRAPPING_PIN_0			((u32)0x00010000)

#define GPIO_IN_DATA_MASK				((u32)0x0000FFFF)
#define GPIO_IN_DATA_PIN_15				((u32)0x00008000)
#define GPIO_IN_DATA_PIN_14				((u32)0x00004000)
#define GPIO_IN_DATA_PIN_13				((u32)0x00002000)
#define GPIO_IN_DATA_PIN_12				((u32)0x00001000)
#define GPIO_IN_DATA_PIN_11				((u32)0x00000800)
#define GPIO_IN_DATA_PIN_10				((u32)0x00000400)
#define GPIO_IN_DATA_PIN_9				((u32)0x00000200)
#define GPIO_IN_DATA_PIN_8				((u32)0x00000100)
#define GPIO_IN_DATA_PIN_7				((u32)0x00000080)
#define GPIO_IN_DATA_PIN_6				((u32)0x00000040)
#define GPIO_IN_DATA_PIN_5				((u32)0x00000020)
#define GPIO_IN_DATA_PIN_4				((u32)0x00000010)
#define GPIO_IN_DATA_PIN_3				((u32)0x00000008)
#define GPIO_IN_DATA_PIN_2				((u32)0x00000004)
#define GPIO_IN_DATA_PIN_1				((u32)0x00000002)
#define GPIO_IN_DATA_PIN_0				((u32)0x00000001)

#define GPIO_INT_STATUS_MASK			((u32)0x0000FFFF)
#define GPIO_INT_STATUS_PIN_15			((u32)0x00008000)
#define GPIO_INT_STATUS_PIN_14			((u32)0x00004000)
#define GPIO_INT_STATUS_PIN_13			((u32)0x00002000)
#define GPIO_INT_STATUS_PIN_12			((u32)0x00001000)
#define GPIO_INT_STATUS_PIN_11			((u32)0x00000800)
#define GPIO_INT_STATUS_PIN_10			((u32)0x00000400)
#define GPIO_INT_STATUS_PIN_9			((u32)0x00000200)
#define GPIO_INT_STATUS_PIN_8			((u32)0x00000100)
#define GPIO_INT_STATUS_PIN_7			((u32)0x00000080)
#define GPIO_INT_STATUS_PIN_6			((u32)0x00000040)
#define GPIO_INT_STATUS_PIN_5			((u32)0x00000020)
#define GPIO_INT_STATUS_PIN_4			((u32)0x00000010)
#define GPIO_INT_STATUS_PIN_3			((u32)0x00000008)
#define GPIO_INT_STATUS_PIN_2			((u32)0x00000004)
#define GPIO_INT_STATUS_PIN_1			((u32)0x00000002)
#define GPIO_INT_STATUS_PIN_0			((u32)0x00000001)

#define GPIO_STATUS_W1TS_MASK			((u32)0x0000FFFF)
#define GPIO_STATUS_W1TS_PIN_15			((u32)0x00008000)
#define GPIO_STATUS_W1TS_PIN_14			((u32)0x00004000)
#define GPIO_STATUS_W1TS_PIN_13			((u32)0x00002000)
#define GPIO_STATUS_W1TS_PIN_12			((u32)0x00001000)
#define GPIO_STATUS_W1TS_PIN_11			((u32)0x00000800)
#define GPIO_STATUS_W1TS_PIN_10			((u32)0x00000400)
#define GPIO_STATUS_W1TS_PIN_9			((u32)0x00000200)
#define GPIO_STATUS_W1TS_PIN_8			((u32)0x00000100)
#define GPIO_STATUS_W1TS_PIN_7			((u32)0x00000080)
#define GPIO_STATUS_W1TS_PIN_6			((u32)0x00000040)
#define GPIO_STATUS_W1TS_PIN_5			((u32)0x00000020)
#define GPIO_STATUS_W1TS_PIN_4			((u32)0x00000010)
#define GPIO_STATUS_W1TS_PIN_3			((u32)0x00000008)
#define GPIO_STATUS_W1TS_PIN_2			((u32)0x00000004)
#define GPIO_STATUS_W1TS_PIN_1			((u32)0x00000002)
#define GPIO_STATUS_W1TS_PIN_0			((u32)0x00000001)

#define GPIO_STATUS_W1TC_MASK			((u32)0x0000FFFF)
#define GPIO_STATUS_W1TC_PIN_15			((u32)0x00008000)
#define GPIO_STATUS_W1TC_PIN_14			((u32)0x00004000)
#define GPIO_STATUS_W1TC_PIN_13			((u32)0x00002000)
#define GPIO_STATUS_W1TC_PIN_12			((u32)0x00001000)
#define GPIO_STATUS_W1TC_PIN_11			((u32)0x00000800)
#define GPIO_STATUS_W1TC_PIN_10			((u32)0x00000400)
#define GPIO_STATUS_W1TC_PIN_9			((u32)0x00000200)
#define GPIO_STATUS_W1TC_PIN_8			((u32)0x00000100)
#define GPIO_STATUS_W1TC_PIN_7			((u32)0x00000080)
#define GPIO_STATUS_W1TC_PIN_6			((u32)0x00000040)
#define GPIO_STATUS_W1TC_PIN_5			((u32)0x00000020)
#define GPIO_STATUS_W1TC_PIN_4			((u32)0x00000010)
#define GPIO_STATUS_W1TC_PIN_3			((u32)0x00000008)
#define GPIO_STATUS_W1TC_PIN_2			((u32)0x00000004)
#define GPIO_STATUS_W1TC_PIN_1			((u32)0x00000002)
#define GPIO_STATUS_W1TC_PIN_0			((u32)0x00000001)

#define GPIO_PIN_CONFIG__MASK			((u32)0x00001800)
#define GPIO_PIN_WAKEUP_ENABLE			((u32)0x00000400)
#define GPIO_PIN_INT_TYPE__MASK			((u32)0x00000380)
#define GPIO_PIN_DRIVER					((u32)0x00000004)
#define GPIO_PIN_SOURCE					((u32)0x00000001)

#define GPIO_SIGMA_DELTA_ENABLE			((u32)0x00010000)
#define GPIO_SIGMA_DELTA_PRESCALER_MASK	((u32)0x0000FF00)
#define GPIO_SIGMA_DELTA_TARGET_MASK	((u32)0x000000FF)

#define GPIO_RTC_CALIB_START			((u32)0x80000000)
#define GPIO_RTC_PERIOD_NUM_MASK		((u32)0x000003ff)

#define GPIO_RTC_CALIB_RDY				((u32)0x80000000)
#define GPIO_RTC_CALIB_RDY_REAL			((u32)0x40000000)
#define GPIO_RTC_CALIB_VALUE_MASK		((u32)0x000FFFFF)


/*********************** HDRF registers ****************************************/
// WiFi radio registers
typedef struct
{
	__IO u32	reg_0;	// 0x60000300
	__IO u32	reg_1;	// 0x60000304
	__IO u32	reg_2;	// 0x60000308
	__IO u32	reg_3;	// 0x6000030C
	__IO u32	reg_4;	// 0x60000310
	__IO u32	reg_5;	// 0x60000314
	__IO u32	reg_6;	// 0x60000318
	__IO u32	reg_7;	// 0x6000031C
	__IO u32	reg_8;	// 0x60000320
	__IO u32	reg_9;	// 0x60000324
	__IO u32	reg_10;	// 0x60000328
	__IO u32	reg_11;	// 0x6000032C
	__IO u32	reg_12;	// 0x60000330
	__IO u32	reg_13;	// 0x60000334
	__IO u32	reg_14;	// 0x60000338
	__IO u32	reg_15;	// 0x6000033C
	__IO u32	reg_16;	// 0x60000340
	__IO u32	reg_17;	// 0x60000344
	__IO u32	reg_18;	// 0x60000348
	__IO u32	reg_19;	// 0x6000034C
	__IO u32	reg_20;	// 0x60000350
	__IO u32	reg_21;	// 0x60000354
	__IO u32	reg_22;	// 0x60000358
	__IO u32	reg_23;	// 0x6000035C
	__IO u32	reg_24;	// 0x60000360
	__IO u32	reg_25;	// 0x60000364
	__IO u32	reg_26;	// 0x60000368
	__IO u32	reg_27;	// 0x6000036C
	__IO u32	reg_28;	// 0x60000370
	__IO u32	reg_29;	// 0x60000374
	__IO u32	reg_30;	// 0x60000378
	__IO u32	reg_31;	// 0x6000037C
	__IO u32	reg_32;	// 0x60000380
	__IO u32	reg_33;	// 0x60000384
	__IO u32	reg_34;	// 0x60000388
	__IO u32	reg_35;	// 0x6000038C
	__IO u32	reg_36;	// 0x60000390
	__IO u32	reg_37;	// 0x60000394
	__IO u32	reg_38;	// 0x60000398
	__IO u32	reg_39;	// 0x6000039C
	__IO u32	reg_40;	// 0x600003A0
	__IO u32	reg_41;	// 0x600003A4
	__IO u32	reg_42;	// 0x600003A8
	__IO u32	reg_43;	// 0x600003AC
	__IO u32	reg_44;	// 0x600003B0
	__IO u32	reg_45;	// 0x600003B4
	__IO u32	reg_46;	// 0x600003B8
	__IO u32	reg_47;	// 0x600003BC
	__IO u32	reg_48;	// 0x600003C0
	__IO u32	reg_49;	// 0x600003C4
	__IO u32	reg_50;	// 0x600003C8
	__IO u32	reg_51;	// 0x600003CC
	__IO u32	reg_52;	// 0x600003D0
	__IO u32	reg_53;	// 0x600003D4
	__IO u32	reg_54;	// 0x600003D8
	__IO u32	reg_55;	// 0x600003DC
	__IO u32	reg_56;	// 0x600003E0
	__IO u32	reg_57;	// 0x600003E4
	__IO u32	reg_58;	// 0x600003E8
	__IO u32	reg_59;	// 0x600003EC
	__IO u32	reg_60;	// 0x600003F0
	__IO u32	reg_61;	// 0x600003F4
	__IO u32	reg_62;	// 0x600003F8
	__IO u32	reg_63;	// 0x600003FC
} HDRF_typedef;


#define HDRF_BaseAddress		0x60000300
#define HDRF 					((HDRF_typedef *)HDRF_BaseAddress)

/************************ PHY registers ***************************************/
typedef struct
{
	__IO u32	reg_0;	// 0x60009800
} PHY_typedef;
#define PHY_BaseAddress			0x60009800
#define PHY 					((PHY_typedef *)PHY_BaseAddress)

/************************ RTC_RAM registers ***********************************/
typedef struct
{
	__IO u32	reg_0;	// 0x60001000
} RTC_RAM_typedef;
#define RTC_RAM_BaseAddress		0x60001000
#define RTC_RAM 				((RTC_RAM_typedef *)RTC_RAM_BaseAddress)

/************************ RTC_MEM registers ***********************************/
typedef struct
{
	__IO u32	reg_0;	// 0x60001100
} RTC_MEM_typedef;
#define RTC_MEM_BaseAddress		0x60001100
#define RTC_MEM 				((RTC_MEM_typedef *)RTC_MEM_BaseAddress)

/************************ i2s registers ****************************************/
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


/************************** iomux registers **********************************/
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


/******************* RTC bitfields *******************************************/
typedef struct _rtc_0_t
{
	__I unsigned	dummy0				:31;
	__IO unsigned	sw_reset			:1;	// bit31 - software_reset()
} rtc_0_t;

typedef struct _rtc_2_t
{
	__I unsigned	dummy0				:20;
	__IO unsigned	rtc_enter_sleep		:1;	// bit20 = 1 - rtc_enter_sleep()
	__IO unsigned	get_reset_reason	:1;	// bit21 - rtc_get_reset_reason() { & (1<<21) }, 
	__I unsigned	dummy22				:10;
} rtc_2_t;

typedef struct _rtc_4_t
{
	__I unsigned	dummy0				:25;
	__IO unsigned	sar_init			:1;	// bit25 sar_init, 
	__IO unsigned	sar_clk_sel			:1;	// bit25,26 =1 �������� ������������ ��� SAR ... = PLL (80MHz)
	__I unsigned	dummy27				:4;
	__IO unsigned	i2s_clk_sel			:1;	// bit31 =1 �������� ������������ ��� I2S, ... = PLL (80MHz)
} rtc_4_t;

typedef struct _rtc_5_t
{
	__IO unsigned	reason0			:1;	// bit0..3: rtc_get_reset_reason
	__IO unsigned	reason1			:1;	// bit0..3: rtc_get_reset_reason
	__IO unsigned	reason2			:1;	// bit0..3: rtc_get_reset_reason
	__I unsigned	dummy3			:29;
} rtc_5_t;

typedef struct _gpio_out_t
{
	__IO unsigned	gpio0	:1;	// gpio16 value bit
	__IO unsigned	dummy1	:31;
} rtc_gpio0_out_t;

typedef struct _gpio_enable_t
{
	__IO unsigned	gpio0_en	:1;	// gpio16 out enable
	__IO unsigned	dummy1		:31;
} rtc_gpio0_en_t;

typedef struct _gpio_in_t
{
	__IO unsigned	gpio0	:1;	// gpio16 in value bit
	__IO unsigned	dummy1	:31;
} rtc_gpio0_in_t;

typedef struct _gpio_conf_t
{
	__IO unsigned	gpio0_mux	:1;	// mux = 0 gpio16 mux configuration for out enable
	__IO unsigned	dummy1		:31;
} rtc_gpio0_conf_t;

typedef struct _xpd_dcdc_conf_t		/* RTC_GPIO0_CFG:	0x600007A0 */
{
	__IO unsigned	fun_bit0		:1;	// = 1, mux configuration for XPD_DCDC to output rtc_gpio16 (gpio0)
	__IO unsigned	fun_bit1		:1;
	__IO unsigned	pullup			:1;
	__IO unsigned	pulldown		:1;
	__IO unsigned	sleep_pullup	:1;
	__IO unsigned	sleep_pulldown	:1;
	__IO unsigned	dummy6			:1;
	__IO unsigned	fun_bit2		:1;
	__IO unsigned	dummy8			:24;
} rtc_gpio3_conf_t;

typedef struct
{
	union					// 0x60000700
	{
		__IO u32		rtc_0;
		__IO rtc_0_t	rtc_0_bits;
	};
	__IO u32	sleep_val;	// 0x60000704 the target value of RTC_COUNTER for wakeup from light-sleep/deep-sleep
	union					// 0x60000708
	{
		__IO u32		rtc_2;		// bit21 - rtc_get_reset_reason() { & (1<<21) }, bit20 = 1 - rtc_enter_sleep()
		__IO rtc_2_t	rtc_2_bits;
	};
	__IO u32	rtc_3;		// 0x6000070C
	union					// 0x60000710
	{
		__IO u32		rtc_4;		//IO_RTC_4 = 0 - disable WiFi
		__IO rtc_4_t	rtc_4_bits;
	};
	union					// 0x60000714
	{
		__IO u32	rtc_5;			// bit0..3: rtc_get_reset_reason(
		__IO rtc_5_t	rtc_5_bits;
	};
	__IO u32	rtc_6;				// 0x60000718 rtc_get_reset_reason()
	__IO u32	sleep_count_val;	// 0x6000071C the current value of RTC_COUNTER
	__IO u32	int_status;			// 0x60000720
	__IO u32	int_clear;			// 0x60000724
	__IO u32	int_ena;			// 0x60000728
	__I u32	dummy2C;				// 0x6000072C
	__IO u32	scratch_0;			// 0x60000730 the register for software to save some values for watchdog reset
	__IO u32	scratch_1;			// 0x60000734 the register for software to save some values for watchdog reset
	__IO u32	scratch_2;			// 0x60000738 the register for software to save some values for watchdog reset
	__IO u32	powerup;			// 0x6000073C phy_set_powerup_option()
	__I u32	dummy40;				// 0x60000740
	__I u32	dummy44;				// 0x60000744
	__I u32	dummy48;				// 0x60000748
	__I u32	dummy4C;				// 0x6000074C
	__I u32	dummy50;				// 0x60000750
	__I u32	dummy54;				// 0x60000754
	__I u32	dummy58;				// 0x60000758
	__I u32	dummy5C;				// 0x6000075C
	__I u32	dummy60;				// 0x60000760
	__I u32	dummy64;				// 0x60000764
	union							// 0x60000768
	{
		__IO u32				gpio_out;	// used by gpio16
		__IO rtc_gpio0_out_t	gpio_out_bits;
	};
	__I u32	dummy6C;				// 0x6000076C
	__I u32	dummy70;				// 0x60000770
	union							// 0x60000774
	{
		__IO u32			gpio_enable;	//
		__IO rtc_gpio0_en_t	gpio_enable_bits;	// 
	};
	__I u32	dummy78;				// 0x60000778
	__I u32	dummy7C;				// 0x6000077C
	__I u32	dummy80;				// 0x60000780
	__I u32	dummy84;				// 0x60000784
	__I u32	dummy88;				// 0x60000788
	union							// 0x6000078C
	{
		__IO u32			gpio_in;	//
		__IO rtc_gpio0_in_t		gpio_in_bits;
	};
	union							// 0x60000790
	{
		__IO u32			gpio_conf;			// RTC_GPIO_CONF:	0x60000790 
		__IO rtc_gpio0_conf_t	gpio_conf_bits;		// 
	};
	__IO u32	gpio0_conf;			// 0x60000794
	__IO u32	gpio1_conf;			// 0x60000798
	__IO u32	gpio2_conf;			// 0x6000079C
	union							// 0x600007A0
	{
		__IO u32			gpio3_conf;			// IO_PAD_XPD_DCDC_CONF	GPIO16, XPD_DCDC:	0x600007A0
		__IO rtc_gpio3_conf_t	gpio3_conf_bits;	// 
	};
	__IO u32	gpio4_conf;			// 0x600007A4
	__IO u32	gpio5_conf;			// 0x600007A8
	__I u32	dummyAC;				// 0x600007AC
	__I u32	dummyB0;				// 0x600007B0
	__I u32	dummyB4;				// 0x600007B4
	__I u32	dummyB8;				// 0x600007B8
	__I u32	dummyBC;				// 0x600007BC
	__I u32	dummyC0;				// 0x600007C0
	__I u32	dummyC4;				// 0x600007C4
	__I u32	dummyC8;				// 0x600007C8
	__I u32	dummyCC;				// 0x600007CC
	__I u32	dummyD0;				// 0x600007D0
	__I u32	dummyD4;				// 0x600007D4
	__I u32	dummyD8;				// 0x600007D8
	__I u32	dummyDC;				// 0x600007DC
	__I u32	dummyE0;				// 0x600007E0
	__I u32	dummyE4;				// 0x600007E4
	__I u32	dummyE8;				// 0x600007E8
	__I u32	dummyEC;				// 0x600007EC
	__I u32	dummyF0;				// 0x600007F0
	__I u32	dummyF4;				// 0x600007F4
	__I u32	dummyF8;				// 0x600007F8
	__I u32	dummyFC;				// 0x600007FC
} RTC_TypeDef;

#define RTC_BaseAddress		0x60000700

#define RTC 		((RTC_TypeDef *)RTC_BaseAddress)

/********* RTC registers reset value ******************************************/
#define RTC0_RESET_VALUE				((u32)0x00000000)
#define RTC_SLEEP_VAL_RESET_VALUE		((u32)0x00000000)
#define RTC1_RESET_VALUE				((u32)0x00000000)
#define RTC2_RESET_VALUE				((u32)0x00000000)
#define RTC3_RESET_VALUE				((u32)0x00000000)
#define RTC4_RESET_VALUE				((u32)0x00000000)
#define RTC5_RESET_VALUE				((u32)0x00000000)
#define RTC6_RESET_VALUE				((u32)0x00000000)
#define RTC_SLEEP_COUNT_RESET_VALUE		((u32)0x00000000)
#define RTC_INT_STATUS_RESET_VALUE		((u32)0x00000000)
#define RTC_INT_CLEAR_RESET_VALUE		((u32)0x00000000)
#define RTC_INT_ENA_RESET_VALUE			((u32)0x00000000)
#define RTC_SCRATCH_0_RESET_VALUE		((u32)0x00000000)
#define RTC_SCRATCH_1_RESET_VALUE		((u32)0x00000000)
#define RTC_SCRATCH_2_RESET_VALUE		((u32)0x00000000)
#define RTC_POWERUP_RESET_VALUE			((u32)0x00000000)
#define RTC_GPIO_OUT_RESET_VALUE		((u32)0x00000000)
#define RTC_GPIO_ENABLE_RESET_VALUE		((u32)0x00000000)
#define RTC_GPIO_IN_DATA_RESET_VALUE	((u32)0x00000000)
#define RTC_GPIO_CONF_RESET_VALUE		((u32)0x00000000)
#define RTC_GPIO0_CONF_RESET_VALUE		((u32)0x00000000)
#define RTC_GPIO1_CONF_RESET_VALUE		((u32)0x00000000)
#define RTC_GPIO2_CONF_RESET_VALUE		((u32)0x00000000)
#define RTC_GPIO3_CONF_RESET_VALUE		((u32)0x00000000)
#define RTC_GPIO4_CONF_RESET_VALUE		((u32)0x00000000)
#define RTC_GPIO5_CONF_RESET_VALUE		((u32)0x00000000)

/***************** RTC bit mask ***********************************************/
#define RTC_RTC0_SOFTVARE_RESET			((u32)0x80000000)

#define RTC_RTC2_GET_RESET_REASON		((u32)0x00200000)
#define RTC_RTC2_ENTER_SLEEP			((u32)0x00100000)

#define RTC_RTC4_I2S_CLK_SEL			((u32)0x80000000)
#define RTC_RTC4_SAR_CLK_SEL			((u32)0x04000000)
#define RTC_RTC4_SAR_INIT				((u32)0x02000000)

#define RTC_RTC5_REASON_MASK			((u32)0x00000007)

#define RTC_GPIO3_FUN_BIT0				((u32)0x00000001)
#define RTC_GPIO3_FUN_BIT1				((u32)0x00000002)
#define RTC_GPIO3_PULLUP				((u32)0x00000004)
#define RTC_GPIO3_PULLDOWN				((u32)0x00000008)
#define RTC_GPIO3_SLEEP_PULLUP			((u32)0x00000010)
#define RTC_GPIO3_SLEEP_PULLDOWN		((u32)0x00000020)
#define RTC_GPIO3_FUN_BIT2				((u32)0x00000100)
#define RTC_GPIO3_FUN_MASK				((u32)0x00000103)


/************************ SAR Bitfield ****************************************/
typedef struct
{
	__IO unsigned	start		:1;	// ��� 1: ������ ������ ������ SAR
	__IO unsigned	num_data	:3;	// ��� 2..4: ���-�� �������� � SAR_DATA 0..7 -> 1..8	
	__I unsigned	dummy3		:20;	// 
	__IO unsigned	r_state		:3;		// ��� 24..26: ���������� r_state = 0
	__I unsigned	dummy27		:5;
} sar_cfg_t;

typedef struct
{
	
} sar_tim1_t;

typedef struct
{
	
} sar_tim2_t;

typedef struct
{
	
} sar_cfg1_t;

typedef struct
{
	
} sar_cfg2_t;

typedef struct
{
	__I u32	dummy00;	// 0x60000D00
	__I u32	dummy04;	// 0x60000D04
	__I u32	dummy08;	// 0x60000D08
	__I u32	dummy0C;	// 0x60000D0C
	__I u32	dummy10;	// 0x60000D10
	__I u32	dummy14;	// 0x60000D14
	__I u32	dummy18;	// 0x60000D18
	__I u32	dummy1C;	// 0x60000D1C
	__I u32	dummy20;	// 0x60000D20
	__I u32	dummy24;	// 0x60000D24
	__I u32	dummy28;	// 0x60000D28
	__I u32	dummy2C;	// 0x60000D2C
	__I u32	dummy30;	// 0x60000D30
	__I u32	dummy34;	// 0x60000D34
	__I u32	dummy38;	// 0x60000D38
	__I u32	dummy3C;	// 0x60000D3C
	__I u32	dummy40;	// 0x60000D40
	__I u32	dummy44;	// 0x60000D44
	__I u32	dummy48;	// 0x60000D48
	__I u32	dummy4C;	// 0x60000D4C
	union					// 0x60000D50
	{
		__IO u32		cfg;
		__IO sar_cfg_t	cfg_bits;
	};
	union					// 0x60000D54
	{
		__IO u32		tim1;
		__IO sar_tim1_t	tim1_bits;
	};
	union					// 0x60000D58
	{
		__IO u32		tim2;
		__IO sar_tim2_t	tim2_bits;
	};
	union					// 0x60000D5C
	{
		__IO u32		cfg1;
		__IO sar_cfg1_t	cfg1_bits;
	};
	union					// 0x60000D60
	{
		__IO u32		cfg2;
		__IO sar_cfg2_t	cfg2_bits;
	};
	__I u32	dummy64;	// 0x60000D64
	__I u32	dummy68;	// 0x60000D68
	__I u32	dummy6C;	// 0x60000D6C
	__I u32	dummy70;	// 0x60000D70
	__I u32	dummy74;	// 0x60000D74
	__I u32	dummy78;	// 0x60000D78
	__I u32	dummy7C;	// 0x60000D7C
	union				// 0x60000D80
	{
		__IO u32	data;
		__IO u32	w0;
	};
	__IO u32		w1;	// 0x60000D84
	__IO u32		w2;	// 0x60000D88
	__IO u32		w3;	// 0x60000D8C
	__IO u32		w4;	// 0x60000D90
	__IO u32		w5;	// 0x60000D94
	__IO u32		w6;	// 0x60000D98
	__IO u32		w7;	// 0x60000D9C
} SAR_TypeDef;

#define SAR_BaseAddress			((u32)0x60000D00)

#define SAR 		((SAR_TypeDef *) SAR_BaseAddress)


/******************* SLC bitfields *******************************************/
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
		__IO slc_int_t		int_status_bits;
	};
	union								// 0x60000B0C
	{
		__IO u32			int_ena;
		__IO slc_int_t		int_ena_bits;
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


/**************** SPI registers **************************************/
typedef struct
{
	__IO u32		dummy0	:12;
	__IO unsigned	dummy12	:1;	// BIT12 = 1 ???
	__IO unsigned	dummy13	:1;	// BIT13 = 1 ???
	__IO unsigned	dummy14	:1;	// BIT14
	__IO unsigned	dummy15	:1;	// BIT15
	__IO unsigned	dummy16	:1;	// BIT16
	__IO unsigned	dummy17	:1;	// BIT17
	__IO unsigned	usr		:1;	// BIT18 spi module busy - transmission start In the master mode, it is the start bit of a single operation. Self-clear by hardware
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
	__IO unsigned	fastrd_mode		:1;	// BIT13 this bit enable the bits: spi_qio_mode, spi_dio_mode, spi_qout_mode and spi_dout_mode
	__IO unsigned	dout_mode		:1;	// BIT14 In the read operations, "read-data" phase apply 2 signals
	__IO unsigned	res_and_res		:1;	// BIT15
	__IO unsigned	sst_aai			:1;	// BIT16
	__IO unsigned	enable_ahb		:1;	// BIT17 Mapping of FLASH to this region is controlled by bit 17 of SPI CTRL register (0x60000208): setting this bit maps the FLASH, clearing it unmaps it.
	__IO unsigned	hold_mode		:1;	// BIT18
	__IO unsigned	share_bus		:1;	// BIT19
	__IO unsigned	qout_mode		:1;	// BIT20 In the read operations, "read-data" phase apply 4 signals
	__IO unsigned	wp_reg			:1;	// BIT21
	__IO unsigned	two_byte_status_en	:1;	// BIT22
	__IO unsigned	dio_mode		:1;	// BIT23 In the read operations, "address" phase and "read-data" phase apply 2 signals
	__IO unsigned	qio_mode		:1;	// BIT24 In the read operations, "address" phase and "read-data" phase apply 4 signals
	__IO unsigned	rd_bit_order	:1;	// BIT25 In "read-data" (MISO) phase, 1: LSB first; 0: MSB first
	__IO unsigned	wr_bit_order	:1;	// BIT26 In "command", "address", "write-data" (MOSI) phases, 1: LSB first; 0: MSB first
	__IO unsigned	dummy27			:5;
} spi_ctrl_t;

typedef struct
{
	__IO unsigned	bus_timer_limit		:16;
	__IO unsigned	cs_hold_delay_res	:12;
	__IO unsigned	cs_hold_delay		:4;	// ���-�� ������ SPI-CLK �������� �� ����������� ���������� � ����������
} spi_ctrl1_t;

typedef struct	// In the slave mode, this register are the status register for the master to read out.
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
	__IO unsigned	cl_out_high_mode	:4;	// bit 12-15 SPI_CTRL2_CLK_HDIV2 BIT12 ��������� ������������ "1" �� CLK � 2 ����
	__IO unsigned	miso_delay_mode		:2;	// bit 16-17 MISO signals are delayed by spi_clk. 0: zero; 1: half cycle; 2: one cycle
	__IO unsigned	miso_delay_num		:3;	// bit 18-20 MISO signals are delayed by 80MHz clock cycles
	__IO unsigned	mosi_delay_mode		:2;	// bit 21-22 MOSI signals are delayed by spi_clk. 0: zero; 1: half cycle; 2: one cycle
	__IO unsigned	mosi_delay_num		:3;	// bit 23-25 MOSI signals are delayed by 80MHz clock cycles
	__IO unsigned	cs_delay_mode		:2;	// bit 26-27 spi_cs signal is delayed by spi_clk. 0: zero; 1: half cycle; 2: one cycle
	__IO unsigned	cs_delay_num		:4;	// bit 28-31 spi_cs signal is delayed by 80MHz clock cycles
} spi_ctrl2_t;

typedef struct
{
	__IO unsigned	clkcnt_l	:6;	// In the master mode, it must be eqaul to spi_clkcnt_N. In the slave mode, it must be 0.
	__IO unsigned	clkcnt_h	:6;	// In the master mode, it must be floor((spi_clkcnt_N+1)/2-1). In the slave mode, it must be 0.
	__IO unsigned	clkcnt_n	:6;	// In the master mode, it is the divider of spi_clk. 
									// So spi_clk frequency is 80MHz/(spi_clkdiv_pre+1)/(spi_clkcnt_N+1)
	__IO unsigned	pre_s		:13;	// In the master mode, it is pre-divider of spi_clk.
	__IO unsigned	equ_sysclk	:1;	// In the master mode, 1: spi_clk is eqaul to 80MHz, 0: spi_clk is divided from 80 MHz clock.
} spi_clock_t;

typedef struct
{
	__IO unsigned	dout_din			:1;	// bit0 ������������� ����� ������� ???
	__IO unsigned	ahb_command_4byte	:1;	// bit1
	__IO unsigned	flash_mode			:1;	// bit2 flash operation mode 0 - disable 1 - enable
	__IO unsigned	ahb_command			:1;
	__IO unsigned	cs_hold				:1;	// bit4 ����� ������ cs ��������� �� 1 ���� ������
	__IO unsigned	cs_setup			:1;	// bit5 ����� ��������� cs ����� ����� 3 ����� ����� �������
	__IO unsigned	ck_in_edge			:1;	// bit6 In the slave mode, 1: rising-edge; 0: falling-edge
	__IO unsigned	ck_out_edge			:1;	// bit7 In the master mode, 1: rising-edge; 0: falling-edge
	__IO unsigned	ahb_endian_mode		:2;
	__IO unsigned	rd_byte_order		:1;	// bit10 In "read-data" (MISO) phase, 1: little-endian; 0: big_endian
	__IO unsigned	wr_byte_order		:1;	// bit11 In "command", "address", "write-data" (MOSI) phases, 1: little-endian; 0: big_endian
	__IO unsigned	fwrite_dual			:1;	// bit12 In the write operations, "read-data" phase apply 2 signals
	__IO unsigned	fwrite_quad			:1;	// bit13 In the write operations, "read-data" phase apply 4 signals
	__IO unsigned	fwrite_dio			:1;	// bit14 In the write operations, "address" phase and "read-data" phase apply 2 signals
	__IO unsigned	fwrite_qio			:1;	// bit15 In the write operations, "address" phase and "read-data" phase apply 4 signals
	__IO unsigned	sio					:1;	// bit16 1: mosi and miso signals share the same pin
	__IO unsigned	hold_pol			:1;
	__IO unsigned	dout_hold			:1;
	__IO unsigned	din_hold			:1;
	__IO unsigned	dummy_hold			:1;
	__IO unsigned	addr_hold			:1;
	__IO unsigned	cmd_hold			:1;
	__IO unsigned	prep_hold			:1;
	__IO unsigned	din_highpart		:1;	// bit24 1: "read-data" phase only access to high-part of the buffer spi_w8~spi_w15
	__IO unsigned	dout_highpart		:1;	// bit25 1: "write-data" phase only access to high-part of the buffer spi_w8~spi_w15
	__IO unsigned	dummy_idle			:1;
	__IO unsigned	mosi				:1;	// bit27 This bit enable the "write-data" phase of an operation.
	__IO unsigned	miso				:1;	// bit28 This bit enable the "read-data" phase of an operation.
	__IO unsigned	dummy				:1;	// bit29 This bit enable the "dummy" phase of an operation.
	__IO unsigned	addr				:1;	// bit30 This bit enable the "address" phase of an operation.
	__IO unsigned	command				:1;	// bit31 This bit enable the "command" phase of an operation.
} spi_user_t;

typedef struct
{
	__IO unsigned	dummy_cyclelen	:8;		// The length in spi_clk cycles of "dummy" phase. The register value shall be (cycle_num-1)
	__IO unsigned	miso_bitlen 	:9;		// MISO The length in bits of "read-data" phase. The register value shall be (bit_num-1)
	__IO unsigned	mosi_bitlen 	:9;		// MOSI The length in bits of "write-data" phase. The register value shall be (bit_num-1)
	__IO unsigned	addr_bitlen 	:6;		// The length in bits of "address" phase. The register value shall be (bit_num-1)
} spi_user1_t;

typedef struct
{
	__IO unsigned	command_value	:16;	// bit15-0 is cmd value.
	__IO unsigned	dummy16			:12;
	__IO unsigned	command_bitlen	:4;		// bit28-31 The length in bits of "command" phase. The register value shall be (bit_num-1)
} spi_user2_t;

typedef struct
{
	__IO unsigned	cs0_dis		:1;	// 1: disable CS0; 0: spi_cs signal is from/to CS0 pin
	__IO unsigned	cs1_dis		:1;	// SPI overlap1: disable CS1; 0: spi_cs signal is from/to CS1 pin
	__IO unsigned	cs2_dis		:1;	// 1: disable CS2; 0: spi_cs signal is from/to CS2 pin
	__IO unsigned	dummy3		:1;	// BIT3
	__IO unsigned	dummy4		:1;
	__IO unsigned	clk_dis		:1; // ����� CLK = "0"
	__IO unsigned	cs_inv		:1; // �������� ������� CS
	__IO unsigned	dummy7		:1;
	__IO unsigned	dummy8		:1;
	__IO unsigned	dummy9		:1;
	__IO unsigned	dummy10		:1;
	__IO unsigned	cs_clk		:1; // ������ �� CS = CLK
	__IO unsigned	dummy12		:1;
	__IO unsigned	dummy13		:1;
	__IO unsigned	dummy14		:1;
	__IO unsigned	dummy15		:1;
	__IO unsigned	mosi_miso	:1; // ������ MISO ��������� �� MOSI 0-GPIO12=MOSI 1-GPIO12=MISO
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
	__IO unsigned	clk_inv		:1;	// ���������� ��������� CLK 0-0 1-1, ������ �� ��������
	__IO unsigned	cs_0		:1; // CS = "0"
	__IO unsigned	dummy31		:1;
} spi_pin_t;

typedef struct
{
	__IO unsigned	rd_buf_done			:1;	// bit0 The interrupt raw bit for the completement of "read-buffer" operation in the slave mode
	__IO unsigned	wr_buf_done			:1;	// bit1 The interrupt raw bit for the completement of "write-buffer" operation in the slave mode.
	__IO unsigned	rd_done				:1;	// bit2 The interrupt raw bit for the completement of "read-status" operation in the slave mode. SPI_SLV_RD_STA_DONE
	__IO unsigned	wr_done				:1;	// bit3 The interrupt raw bit for the completement of "write-status" operation in the slave mode SPI_SLV_WR_STA_DONE
	__IO unsigned	trans_done			:1;	// bit4 The interrupt raw bit for the completement of any operation in both the master mode and the slave mode.
	__IO unsigned	rd_buf_done_en		:1;	// bit5 Interrupt enable bits for the below 5 sources
	__IO unsigned	wr_buf_done_en		:1;	// bit6 
	__IO unsigned	rd_done_en			:1;	// bit7
	__IO unsigned	wr_done_en			:1;	// bit8
	__IO unsigned	trans_done_en		:1;	// bit9
	__IO unsigned	cs_i_mode			:2;
	__IO unsigned	dummy12				:5;
	__IO unsigned	last_command		:3;
	__IO unsigned	last_state			:3;
	__IO unsigned	trans_cnt			:4;	// bit23-26 The operations counter in both the master mode and the slave mode.
	__IO unsigned	cmd_define			:1;	//  1: slave mode commands are defined in SPI_SLAVE3. 
											// 0: slave mode commands are fixed as 1: "writestatus"; 4: "read-status"; 2: "write-buffer" and 3: "read-buffer"
	__IO unsigned	wr_rd_sta_en		:1;	// bit28
	__IO unsigned	wr_rd_buf_en		:1;	// bit29
	__IO unsigned	slave_mode			:1;	// bit30 1: slave mode, 0: master mode.
	__IO unsigned	sync_reset			:1;	// bit31 It is the synchronous reset signal of the module. This bit is self-cleared by hardware.
} spi_slave_t;

typedef struct
{
	__IO unsigned	rdbuf_dummy_en		:1;	// BIT0 In the slave mode, it is the enable bit of "dummy" phase for "read-buffer" operations.
	__IO unsigned	wrbuf_dummy_en		:1;	// BIT1 In the slave mode, it is the enable bit of "dummy" phase for "write-buffer" operations.
	__IO unsigned	rdsta_dummy_en		:1;	// BIT2 In the slave mode, it is the enable bit of "dummy" phase for "read-status" operations.
	__IO unsigned	wrsta_dummy_en		:1;	// BIT3 In the slave mode, it is the enable bit of "dummy" phase for "write-status" operations.
	__IO unsigned	wr_addr_bitlen		:6;	// In the slave mode, it is the address length in bits for "write-buffer" operation. The register value shall be (bit_num-1)
	__IO unsigned	rd_addr_bitlen		:6;	// In the slave mode, it is the address length in bits for "read-buffer" operation. The register value shall be (bit_num-1)
	__IO unsigned	buf_bitlen			:9;	// In the slave mode, it is the length in bits for "write-buffer" and "read-buffer" operations. The register value shall be (bit_num-1)
	__IO unsigned	status_readback		:1;	// =0 CHOOSE ACTIVE STATUS REG
	__IO unsigned	status_fast_en		:1;	// BIT26
	__IO unsigned	status_bitlen		:5;	// In the slave mode, it is the length in bits for "write-status" and "read-status" operations. The register value shall be (bit_num-1)
	
} spi_slave1_t;

typedef struct
{
	__IO unsigned	rdsta_dummy_cyclelen	:8;	// In the slave mode, it is the length in spi_clk cycles of "dummy" phase for "read-status" operations. The register value shall be (cycle_num-1)
	__IO unsigned	wrsta_dummy_cyclelen	:8;	// In the slave mode, it is the length in spi_clk cycles of "dummy" phase for "write-status" operations. The register value shall be (cycle_num-1)
	__IO unsigned	rdbuf_dummy_cyclelen	:8;	// In the slave mode, it is the length in spi_clk cycles of "dummy" phase for "read-buffer" operations. The register value shall be (cycle_num-1)
	__IO unsigned	wrbuf_dummy_cyclelen	:8;	// In the slave mode, it is the length in spi_clk cycles of "dummy" phase for "write-buffer" operations. The register value shall be (cycle_num-1)
} spi_slave2_t;

typedef struct
{
	__IO unsigned	rdbuf_cmd_value 	:8;	// In slave mode, it is the value of "read-buffer" command
	__IO unsigned	wrbuf_cmd_value 	:8;	// In slave mode, it is the value of "write-buffer" command
	__IO unsigned	rdsta_cmd_value 	:8;	// n slave mode, it is the value of "read-status" command
	__IO unsigned	wrsta_cmd_value 	:8;	// In slave mode, it is the value of "write-status" command
} spi_slave3_t;

typedef struct	// This register is for two SPI masters to share the same cs, clock and data signals.
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
	__IO unsigned	int_priority	:2;	// 1-������� ��������� 3-������ ��� overlap SPI
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
	__IO u32			wr_status;	// 0x60000?28 In the slave mode, this register are the status register for the master to write into.
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

#define SPI_CTRL2_CLK_HDIV2					((u32)0x00001000)	// BIT12 ��������� ������������ "1" �� CLK � 2 ����
#define SPI_CTRL2_CLK_HDIV4					((u32)0x00008000)	// BIT15 ��������� ������������ "1" �� CLK � 4 ����
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
#define SPI_PIN_CLK_DIS						((u32)0x00000020)	// BIT5	����� CLK = "0"
#define SPI_PIN_CS_INV						((u32)0x00000040)	// BIT6	�������� ������� CS
#define SPI_PIN_CS_CLK						((u32)0x00000400)	// BIT11 ������ �� CS = CLK
#define SPI_PIN_MOSI_MISO					((u32)0x00010000)	// BIT16 ������ MISO ��������� �� MOSI
#define SPI_PIN_CLK1_CS0					((u32)0x00080000)	// BIT19 CS = "0", CLK = "1"
#define SPI_PIN_CLK_INV						((u32)0x20000000)	// BIT29 �������� ������� CLK
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


/********************** timer registers **************************************/
// correct bit ascending

typedef struct
{
	__IO unsigned	load_value	:23;	// the load value into the counter
	__I unsigned	dummy23		:9;
} timer_frc1_load_t;

typedef struct
{
	__I unsigned	count		:23;	// the current value of the counter. It is a decreasing counter.
	__I unsigned	dummy23		:9;
} timer_frc1_count_t;

typedef enum _frc_interrupt_t
{
	FRC_INTERRUPT_EDGE	= 0,
	FRC_INTERRUPT_LEVEL	= 1
} timer_frc_interrupt_t;

typedef enum _frc_prescaler_t
{
	FRC_PRESCALER_1		= 0,
	FRC_PRESCALER_16	= 1,
	FRC_PRESCALER_256	= 2	
} timer_frc_prescaler_t;

typedef struct
{
	__IO unsigned	interrupt_type	:1;	// bit0 interrupt type, 0:edge, 1:level
	__I unsigned	dummy1			:1;
	__IO unsigned	prescaler		:2;	// bit3-2 prescale-divider, 0: divided by 1, 1: divided by 16, 2 or 3: divided by 256
	__I	unsigned	dummy4			:2;
	__IO unsigned	reload			:1;	// bit6 automatically reload, when the counter  is equal to zero
	__IO unsigned	enable			:1;	// bit7 timer enable
	__I unsigned	interrupt		:1;	// bit8 the status of the interrupt, when the count is equal to the alarm value
	__I unsigned	dummy8			:23;
} timer_frc_control_t;

typedef struct
{
	__IO unsigned	interrupt_clear	:1;	// write to clear the status of the interrupt, if the interrupt type is "level"
	__I unsigned	dummy1			:31;
} timer_frc_int_t;

typedef struct
{
	union										// 0x60000600
	{
		__IO u32					frc1_load;		// the load value into the counter
		__IO timer_frc1_load_t		frc1_load_bits;
	};
	union										// 0x60000604
	{
		__I u32						frc1_count;		// the current value of the counter. It is a decreasing counter.
		__I timer_frc1_count_t		frc1_count_bits;
	};
	union										// 0x60000608
	{
		__IO u32					frc1_control;
		__IO timer_frc_control_t	frc1_control_bits;
	};
	union										// 0x6000060C
	{
		__IO u32					frc1_int;
		__IO timer_frc_int_t		frc1_int_bits;
	};
	__I u32		dummy10;						// 0x60000610
	__I u32		dummy14;						// 0x60000614
	__I u32		dummy18;						// 0x60000618
	__I u32		dummy1C;						// 0x6000061C
	__IO u32						frc2_load;	// 0x60000620 the load value into the counter
	__I u32							frc2_count;	// 0x60000624 the current value of the counter. It is a increasing counter.
	union										// 0x60000628
	{
		__IO u32					frc2_control;
		__IO timer_frc_control_t	frc2_control_bits;
	};
	union										// 0x6000062C
	{
		__IO u32					frc2_int;
		__IO timer_frc_int_t		frc2_int_bits;
	};
	__IO u32	alarm_value;					// 0x60000630 the alarm value for the counter
} TIMER_typedef;

#define	TIMER_base_address		0x60000600

#define	TIMER			((TIMER_typedef *)TIMER_base_address)

/********************* TIMER reset value **************************************/

#define FRC1_LOAD_RESET_VALUE		((u32)0x00000000)
#define FRC1_COUNT_RESET_VALUE		((u32)0x007FFFFF)
#define FRC1_CONTROL_RESET_VALUE	((u32)0x00000000)
#define FRC1_INT_RESET_VALUE		((u32)0x00000000)
#define FRC2_LOAD_RESET_VALUE		((u32)0x00000000)
#define FRC2_COUNT_RESET_VALUE		((u32)0xFFFFFFFF)
#define FRC2_CONTROL_RESET_VALUE	((u32)0x00000000)
#define FRC2_INT_RESET_VALUE		((u32)0x00000000)
#define FRC2_ALARM_RESET_VALUE		((u32)0x00000000)

/********************* TIMER bit mask *****************************************/

#define FRC1_LOAD_DATA_MASK			((u32)0x007FFFFF)
#define FRC1_COUNT_DATA_MASK		((u32)0x007FFFFF)
#define FRC1_CTRL_DATA_MASK			((u32)0x000001FF)
#define FRC1_INT_CLR_MASK			((u32)0x00000001)
#define FRC2_CTRL_DATA_MASK			((u32)0x000001FF)
#define FRC2_INT_CLR_MASK			((u32)0x00000001)


/******************* UART registers *******************************************/
typedef struct
{
	__I unsigned	rx_fifo_rd_byte	:8;		// R/W share the same address
	__I unsigned	dummy8			:24;	// UART FIFO,length 128
} uart_fifo_t;

typedef struct
{
	__I unsigned	rx_fifo_full	:1;	// The interrupt raw bit for rx fifo full interrupt(depands on UART_RXFIFO_FULL_THRHD bits)
	__I unsigned	tx_fifo_empty	:1;	// The interrupt raw bit for tx fifo empty interrupt(depands on UART_TXFIFO_EMPTY_THRHD bits)
	__I unsigned	parity_err		:1;	// The interrupt raw bit for parity check error
	__I unsigned	frm_error		:1;	// The interrupt raw bit for other rx error
	__I unsigned	rx_fifo_ovf		:1;	// The interrupt raw bit for rx fifo overflow
	__I unsigned	dsr_chg			:1;	// The interrupt raw bit for DSR changing level
	__I unsigned	cts_chg			:1;	// The interrupt raw bit for CTS changing level
	__I unsigned	brk_det			:1;	// The interrupt raw bit for Rx byte start error
	__I unsigned	rx_fifo_tout	:1;	// The interrupt raw bit for Rx time-out interrupt(depands on the UART_RX_TOUT_THRHD)
	__I unsigned	dummy9			:23;
} uart_int_raw_t;

typedef struct
{
	__I unsigned	rx_fifo_full	:1;	// The interrupt state bit for RX fifo full event
	__I unsigned	tx_fifo_empty	:1;	// The interrupt state bit for TX fifo empty
	__I unsigned	parity_err		:1;	// The interrupt state bit for rx parity error
	__I unsigned	frm_error		:1;	// The interrupt state for other rx error
	__I unsigned	rx_fifo_ovf		:1;	// The interrupt state bit for RX fifo overflow
	__I unsigned	dsr_chg			:1;	// The interrupt state bit for DSR changing level
	__I unsigned	cts_chg			:1;	// The interrupt state bit for CTS changing level
	__I unsigned	brk_det			:1;	// The interrupt state bit for rx byte start error
	__I unsigned	rx_fifo_tout	:1;	// The interrupt state bit for Rx time-out event
	__I unsigned	dummy9			:23;
} uart_int_st_t;

typedef struct
{
	__IO unsigned	rx_fifo_full	:1;	// The interrupt enable bit for rx fifo full event
	__IO unsigned	tx_fifo_empty	:1;	// The interrupt enable bit for tx fifo empty event
	__IO unsigned	parity_err		:1;	// The interrupt enable bit for parity error
	__IO unsigned	frm_error		:1;	// The interrupt enable bit for other rx error
	__IO unsigned	rx_fifo_ovf		:1;	// The interrupt enable bit for rx fifo overflow
	__IO unsigned	dsr_chg			:1;	// The interrupt enable bit for DSR changing level
	__IO unsigned	cts_chg			:1;	// The interrupt enable bit for CTS changing level
	__IO unsigned	brk_det			:1;	// The interrupt enable bit for rx byte start error
	__IO unsigned	rx_fifo_tout	:1;	// The interrupt enable bit for rx time-out interrupt
	__IO unsigned	dummy9			:23;
} uart_int_ena_t;

typedef struct
{
	__O unsigned	rx_fifo_full	:1;	// Set this bit to clear the rx fifo full interrupt
	__O unsigned	tx_fifo_empty	:1;	// Set this bit to clear the tx fifo empty interrupt
	__O unsigned	parity_err		:1;	// Set this bit to clear the parity error interrupt
	__O unsigned	frm_error		:1;	// Set this bit to clear other rx error interrupt
	__O unsigned	rx_fifo_ovf		:1;	// Set this bit to clear the rx fifo over-flow interrupt
	__O unsigned	dsr_chg			:1;	// Set this bit to clear the DSR changing interrupt
	__O unsigned	cts_chg			:1;	// Set this bit to clear the CTS changing interrupt
	__O unsigned	brk_det			:1;	// Set this bit to clear the rx byte start interrupt
	__O unsigned	rx_fifo_tout	:1;	// Set this bit to clear the rx time-out interrupt
	__O unsigned	dummy9			:23;
} uart_int_clr_t;

typedef struct
{
	__IO unsigned	clk_div		:20;	// BAUDRATE = UART_CLK_FREQ / UART_CLKDIV
	__IO unsigned	dummy20		:12;
} uart_clk_div_t;

typedef struct
{
	__IO unsigned	autobaud_en	:1;	// Set this bit to enable baudrate detect
	__I unsigned	dummy1		:7;
	__IO unsigned	glitch_flt	:8;
	__I unsigned	dummy16		:16;
} uart_autobaud_t;

typedef struct
{
	__I unsigned	rx_fifo_cnt	:8;	// Number of data in uart rx fifo
	__I unsigned	dummy8		:5;
	__I unsigned	dsrn		:1;	// The level of uart dsr pin
	__I unsigned	ctsn		:1;	// The level of uart cts pin
	__I unsigned	rxd			:1;	// The level of uart rxd pin
	__I unsigned	tx_fifo_cnt	:8;	// Number of data in UART TX fifo
	__I unsigned	dummy24		:5;
	__I unsigned	dtrn		:1;	// The level of uart dtr pin
	__I unsigned	rtsn		:1;	// The level of uart rts pin
	__I unsigned	txd			:1;	// The level of the uart txd pin
} uart_status_t;

typedef struct
{
	__IO unsigned	parity			:1;	// bit0 Set parity check:  0:even 1:odd
	__IO unsigned	parity_en		:1;	// bit1 Set this bit to enable uart parity check
	__IO unsigned	num_bits		:2;	// bit2-3 Set bit num:  0:5bits 1:6bits 2:7bits 3:8bits
	__IO unsigned	stop_bits		:2;	// bit4-5 Set stop bit: 1:1bit  2:1.5bits  3:2bits
	__IO unsigned	sw_rts			:1;	// bit6 sw rts
	__IO unsigned	sw_dtr			:1;	// bit7 sw dtr
	__IO unsigned	txd_brk			:1;	// bit8 Set this bit to send a tx break signal(need fifo reset first)
	__IO unsigned	irda_dplx		:1;	// bit9
	__IO unsigned	irda_tx_en		:1;	// bit10
	__IO unsigned	irda_wct		:1;	// bit11
	__IO unsigned	irda_tx_inv		:1;	// bit12
	__IO unsigned	irda_rx_inv		:1;	// bit13
	__IO unsigned	loopback		:1;	// Set this bit to enable uart loopback test mode
	__IO unsigned	tx_flow_en		:1;	// Set this bit to enable uart tx hardware flow control
	__IO unsigned	irda_en			:1;
	__IO unsigned	rx_fifo_rst		:1;	// Set this bit to reset uart rx fifo
	__IO unsigned	tx_fifo_rst		:1;	// Set this bit to reset uart tx fifo
	__IO unsigned	rxd_inv			:1;	// Set this bit to inverse uart rxd level
	__IO unsigned	cts_inv			:1;	// Set this bit to inverse uart cts level
	__IO unsigned	dsr_inv			:1;	// Set this bit to inverse uart dsr level
	__IO unsigned	txd_inv			:1;	// Set this bit to inverse uart txd level
	__IO unsigned	rts_inv			:1;	// Set this bit to inverse uart rts level
	__IO unsigned	dyr_inv			:1;	// Set this bit to inverse uart dtr level
	__IO unsigned	dummy25			:7;
} uart_conf0_t;

typedef struct
{
	__IO unsigned	rx_fifo_full_trhd	:7;	// The config bits for rx fifo full threshold,0-127
	__I unsigned	dummy7				:1;
	__IO unsigned	tx_fifi_empty_trhd	:7;	// The config bits for tx fifo empty threshold,0-127
	__I unsigned	dummy15				:1;
	__IO unsigned	rx_flow_trhd		:7;	// The config bits for rx flow control threshold,0-127
	__IO unsigned	rx_flow_en			:1;	// Set this bit to enable rx hardware flow control
	__IO unsigned	rx_tout_trhd		:7;	// Config bits for rx time-out threshold,uint: byte,0-127
	__IO unsigned	rx_tout_en			:1;	// Set this bit to enable rx time-out function
} uart_conf1_t;

typedef struct
{
	__IO unsigned	lowpulse_min_cnt	:20;	// used in baudrate detect
	__I unsigned	dummy20				:12;
} uart_lowpulse_t;

typedef struct
{
	__IO unsigned	highpulse_min_cnt	:20;	// used in baudrate detect
	__I unsigned	dummy20				:12;
} uart_highpulse_t;

typedef struct
{
	__IO unsigned	rxd_edge_cnt		:10;	// used in baudrate detect
	__I unsigned	dummy20				:22;
} uart_pulse_num_t;



typedef struct UART_struct
{
	union										// 	0x60000?00
	{
		__IO u32				fifo;
		__IO uart_fifo_t		fifo_bits;
	};
	union										// 	0x60000?04
	{
		__I u32				int_raw;
		__I uart_int_raw_t	int_raw_bits;
	};
	union										// 	0x60000?08
	{
		__IO u32			int_st;
		__IO uart_int_st_t	int_st_bits;
	};
	union										// 	0x60000?0C
	{
		__IO u32			int_ena;
		__IO uart_int_ena_t	int_ena_bits;
	};
	union										// 	0x60000?10
	{
		__O u32				int_clr;
		__O uart_int_clr_t	int_clr_bits;
	};
	union										// 	0x60000?14
	{
		__IO u32			clk_div;
		__IO uart_clk_div_t	clk_div_bits;
	};
	union										// 	0x60000?18
	{
		__IO u32				autobaud;
		__IO uart_autobaud_t	autobaud_bits;
	};
	union										// 	0x60000?1C
	{
		__I u32				status;
		__I uart_status_t	status_bits;
	};
	union										// 	0x60000?20
	{
		__IO u32			conf0;
		__IO uart_conf0_t	conf0_bits;
	};
	union										//	0x60000?24
	{
		__IO u32			conf1;
		__IO uart_conf1_t	conf1_bits;
	};
	union										// 	0x60000?28
	{
		__I u32				lowpulse;
		__I uart_lowpulse_t	lowpulse_bits;
	};
	union										// 	0x60000?2C
	{
		__I u32					highpulse;
		__I uart_highpulse_t	highpulse_bits;
	};
	union										// 	0x60000?30
	{
		__I u32					pulse_num;
		__I uart_pulse_num_t	pulse_num_bits;
	};
	__I u32		dummy34;	// 	0x60000?34
	__I u32		dummy38;	// 	0x60000?38
	__I u32		dummy3C;	// 	0x60000?3C
	__I u32		dummy40;	// 	0x60000?40
	__I u32		dummy44;	// 	0x60000?44
	__I u32		dummy48;	// 	0x60000?48
	__I u32		dummy4C;	// 	0x60000?4C
	__I u32		dummy50;	// 	0x60000?50
	__I u32		dummy54;	// 	0x60000?54
	__I u32		dummy58;	// 	0x60000?58
	__I u32		dummy5C;	// 	0x60000?5C
	__I u32		dummy60;	// 	0x60000?60
	__I u32		dummy64;	// 	0x60000?64
	__I u32		dummy68;	// 	0x60000?68
	__I u32		dummy6C;	// 	0x60000?6C
	__I u32		dummy70;	// 	0x60000?70
	__I u32		dummy74;	// 	0x60000?74
	__IO u32	date;		//	0x60000?78 UART HW INFO
	__IO u32	id;			// 	0x60000?7C 
} UART_TypeDef;

#define UART0_BaseAddress		0x60000000
#define UART1_BaseAddress		0x60000F00

#define UART_0 		((UART_TypeDef *)UART0_BaseAddress)
#define UART_1 		((UART_TypeDef *)UART1_BaseAddress)

/**************** UART reset value ********************************************/

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

/******************* UART bit mask ********************************************/
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

#define UART_INT_CLR_MASK				((u32)0x0000FFFF)
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


/******************* WDT bitfields ********************************************/
// correct bit ascending
typedef struct
{
	__IO unsigned	dummy0	:1;
	__I unsigned	dummy1	:31;
} wdt_control_t;

typedef struct
{
	union						// 0x60000900
	{
		__IO u32	control;	// 0x0, 0x01, 0x3C
		__IO wdt_control_t	control_bits;
	};
	__IO u32	reg_1;			// 0x60000904 0x0, 0xB, 0xC, 0xD
	__IO u32	reg_2;			// 0x60000908 0x0, 0xB, 0xC, 0xD
	__I u32		dummy0C;		// 0x6000090C
	__I u32		dummy10;		// 0x60000910
	__IO u32	feed;			// 0x60000914 (0x73 WTD)
} WDT_typedef;

#define WDT_BaseAddress		0x60000900

#define WDT 		((WDT_typedef *)WDT_BaseAddress)


/**************** WDT reset value *********************************************/

#define WDT_CONTROL_RESET_VALUE			((u32)0x00000000)
#define WDT_REG1_RESET_VALUE			((u32)0x00000000)
#define WDT_REG2_RESET_VALUE			((u32)0x00000000)
#define WDT_FEED_RESET_VALUE			((u32)0x00000000)

/**************** WDT bit mask ************************************************/

#define WDT_CONTROL_DISABLE_MASK		((u32)0x0000007E) // Disable WDT
#define WDT_CONTROL_INIT_MASK			((u32)0x00000FFE) // ets_wdt_init (

#define WDT_FEED_MAGIC					((u32)0x00000073) // WTD


#endif	// _ESP8266_REGISTERS_H_

