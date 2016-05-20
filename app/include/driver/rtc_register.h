/*
 * rtc_register.h
 *
 *  Created on: 19.05.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _RTC_REGISTER_H_
#define _RTC_REGISTER_H_

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

/******************* RTC bitfields *******************************************/

typedef struct _rtc_0_t
{
	__IO unsigned	sw_reset			:1;
	__I u32			dummy0				:31;	// bit31 - software_reset()
} rtc_0_t;

typedef struct _rtc_2_t
{
	__I u16			dummy22				:10;
	__IO unsigned	get_reset_reason	:1;	// bit21 - rtc_get_reset_reason() { & (1<<21) }, 
	__IO unsigned	rtc_enter_sleep		:1;	// bit20 = 1 - rtc_enter_sleep()
	__I u32			dummy0				:20;
} rtc_2_t;

typedef struct _rtc_4_t
{
	__IO unsigned	i2s_clk_sel			:1;	// bit31 =1 источник тактирования для I2S, ... = PLL (80MHz)
	__I u8			dummy27				:4;
	__IO unsigned	sar_clk_sel			:1;	// bit25,26 =1 источник тактирования для SAR ... = PLL (80MHz)
	__IO unsigned	sar_init			:1;	// bit25 sar_init, 
	__I u32			dummy0				:25;
} rtc_4_t;

typedef struct _rtc_5_t
{
	__I u32			dummy3			:29;
	__IO unsigned	reason2			:1;	// bit0..3: rtc_get_reset_reason
	__IO unsigned	reason1			:1;	// bit0..3: rtc_get_reset_reason
	__IO unsigned	reason0			:1;	// bit0..3: rtc_get_reset_reason
} rtc_5_t;

typedef struct _gpio_enable_t
{
	__IO u32		dummy1	:31;
	__IO unsigned	rtc_gpio0_en	:1;	// out enable
} gpio_enable_t;

typedef struct _gpio_conf_t
{
	__IO u32		dummy1	:31;
	__IO unsigned	rtc_gpio0_mux	:1;	// mux configuration for out enable
} gpio_conf_t;

typedef struct _xpd_dcdc_conf_t
{
	__IO u32		dummy8			:24;
	__IO unsigned	fun_bit2		:1;
	__IO unsigned	dummy6			:1;
	__IO unsigned	sleep_pulldown	:1;
	__IO unsigned	sleep_pullup	:1;
	__IO unsigned	pulldown		:1;
	__IO unsigned	pullup			:1;
	__IO unsigned	fun_bit1		:1;
	__IO unsigned	fun_bit0		:1;	// mux configuration for XPD_DCDC to output rtc_gpio0
} gpio3_conff_t;

typedef struct
{
	union
	{
		__IO u32		rtc_0;
		__IO rtc_0_t	rtc_0_bits;
	};
	__IO u32	sleep_val;	// the target value of RTC_COUNTER for wakeup from light-sleep/deep-sleep
	union
	{
		__IO u32		rtc_2;		// bit21 - rtc_get_reset_reason() { & (1<<21) }, bit20 = 1 - rtc_enter_sleep()
		__IO rtc_2_t	rtc_2_bits;
	};
	__IO u32	rtc_3;
	union
	{
		__IO u32		rtc_4;		//IO_RTC_4 = 0 - отключение WiFi
		__IO rtc_4_t	rtc_4_bits;
	};
	union
	{
		__IO u32	rtc_5;			// bit0..3: rtc_get_reset_reason(
		__IO rtc_5_t	rtc_5_bits;
	};
	__IO u32	rtc_6;				// rtc_get_reset_reason()
	__IO u32	sleep_count_val;	// the current value of RTC_COUNTER
	__IO u32	int_status;
	__IO u32	int_clear;
	__IO u32	int_ena;
	__I u32	dummy2C;
	__IO u32	scratch_0;	// the register for software to save some values for watchdog reset
	__IO u32	scratch_1;	// the register for software to save some values for watchdog reset
	__IO u32	scratch_2;	// the register for software to save some values for watchdog reset
	__IO u32	powerup;	// phy_set_powerup_option()
	__I u32	dummy40;
	__I u32	dummy44;
	__I u32	dummy48;
	__I u32	dummy4C;
	__I u32	dummy50;
	__I u32	dummy54;
	__I u32	dummy58;
	__I u32	dummy5C;
	__I u32	dummy60;
	__I u32	dummy64;
	__IO u32	gpio_out;	// used by gpio16
	__I u32	dummy6C;
	__I u32	dummy70;
	union
	{
		__IO u32			gpio_enable;	//
		__IO gpio_enable_t	gpio_enable_bits;	// 
	};
	__I u32	dummy78;
	__I u32	dummy7C;
	__I u32	dummy80;
	__I u32	dummy84;
	__I u32	dummy88;
	__IO u32	gpio_in_data;	//
	union
	{
		__IO u32			gpio_conf;			//
		__IO gpio_conf_t	gpio_conf_bits;		// 
	};
	__IO u32	gpio0_conf;
	__IO u32	gpio1_conf;
	__IO u32	gpio2_conf;
	union
	{
		__IO u32			gpio3_conf;	//
		__IO gpio3_conff_t	gpio3_conf_bits;	// 
	};
	__IO u32	gpio4_conf;
	__IO u32	gpio5_conf;
	__I u32	dummyAC;
	__I u32	dummyB0;
	__I u32	dummyB4;
	__I u32	dummyB8;
	__I u32	dummyBC;
	__I u32	dummyC0;
	__I u32	dummyC4;
	__I u32	dummyC8;
	__I u32	dummyCC;
	__I u32	dummyD0;
	__I u32	dummyD4;
	__I u32	dummyD8;
	__I u32	dummyDC;
	__I u32	dummyE0;
	__I u32	dummyE4;
	__I u32	dummyE8;
	__I u32	dummyEC;
	__I u32	dummyF0;
	__I u32	dummyF4;
	__I u32	dummyF8;
	__I u32	dummyFC;
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



#endif /* _RTC_REGISTER_H_ */
