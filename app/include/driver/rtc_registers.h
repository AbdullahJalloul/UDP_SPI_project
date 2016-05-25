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
// correct bit ascending
typedef struct _rtc_0_t
{
	__I u32			dummy0				:31;
	__IO unsigned	sw_reset			:1;	// bit31 - software_reset()
} rtc_0_t;

typedef struct _rtc_2_t
{
	__I u32			dummy0				:20;
	__IO unsigned	rtc_enter_sleep		:1;	// bit20 = 1 - rtc_enter_sleep()
	__IO unsigned	get_reset_reason	:1;	// bit21 - rtc_get_reset_reason() { & (1<<21) }, 
	__I u16			dummy22				:10;
} rtc_2_t;

typedef struct _rtc_4_t
{
	__I u32			dummy0				:25;
	__IO unsigned	sar_init			:1;	// bit25 sar_init, 
	__IO unsigned	sar_clk_sel			:1;	// bit25,26 =1 источник тактирования для SAR ... = PLL (80MHz)
	__I u8			dummy27				:4;
	__IO unsigned	i2s_clk_sel			:1;	// bit31 =1 источник тактирования для I2S, ... = PLL (80MHz)
} rtc_4_t;

typedef struct _rtc_5_t
{
	__IO unsigned	reason0			:1;	// bit0..3: rtc_get_reset_reason
	__IO unsigned	reason1			:1;	// bit0..3: rtc_get_reset_reason
	__IO unsigned	reason2			:1;	// bit0..3: rtc_get_reset_reason
	__I u32			dummy3			:29;
} rtc_5_t;

typedef struct _gpio_out_t
{
	__IO unsigned	gpio0	:1;	// gpio16 value bit
	__IO u32		dummy1	:31;
} gpio_out_t;

typedef struct _gpio_enable_t
{
	__IO unsigned	gpio0_en	:1;	// gpio16 out enable
	__IO u32		dummy1		:31;
} gpio_enable_t;

typedef struct _gpio_in_t
{
	__IO unsigned	gpio0	:1;	// gpio16 in value bit
	__IO u32		dummy1	:31;
} gpio_in_t;

typedef struct _gpio_conf_t
{
	__IO unsigned	gpio0_mux	:1;	// mux = 0 gpio16 mux configuration for out enable
	__IO u32		dummy1		:31;
} gpio_conf_t;

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
	__IO u32		dummy8			:24;
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
	union
	{
		__IO u32			gpio_out;	// used by gpio16
		__IO gpio_out_t		gpio_out_bits;
	};
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
	union
	{
		__IO u32			gpio_in;	//
		__IO gpio_in_t		gpio_in_bits;
	};
	union
	{
		__IO u32			gpio_conf;			// RTC_GPIO_CONF:	0x60000790 
		__IO gpio_conf_t	gpio_conf_bits;		// 
	};
	__IO u32	gpio0_conf;
	__IO u32	gpio1_conf;
	__IO u32	gpio2_conf;
	union
	{
		__IO u32			gpio3_conf;			// IO_PAD_XPD_DCDC_CONF	GPIO16, XPD_DCDC:	0x600007A0
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

//0x60000700
#define IO_RTC_0				rtc_[0] // bit31 - software_reset()
/* RTC_SLP_VAL: 0x60000704 */
#define IO_RTC_SLP_VAL			rtc_[1]	// the target value of RTC_COUNTER for wakeup from light-sleep/deep-sleep
//0x60000708
#define IO_RTC_2				rtc_[2]	// bit21 - rtc_get_reset_reason() { & (1<<21) }, bit20 = 1 - rtc_enter_sleep()
//0x6000070C
#define IO_RTC_3				rtc_[3]
//0x60000710
#define IO_RTC_4				rtc_[4] // rtc_enter_sleep() = 0;
// IO_RTC_4 = 0 - РѕС‚РєР»СЋС‡РµРЅРёРµ WiFi
//bit31 =1 РёСЃС‚РѕС‡РЅРёРє С‚Р°РєС‚РёСЂРѕРІР°РЅРёСЏ РґР»СЏ I2S, ... = PLL (80MHz)
//bit25,26 =1 РёСЃС‚РѕС‡РЅРёРє С‚Р°РєС‚РёСЂРѕРІР°РЅРёСЏ РґР»СЏ SAR ... = PLL (80MHz)
//0x60000714
#define IO_RTC_5				rtc_[5]	// bit0..3: rtc_get_reset_reason()
//0x60000718
#define IO_RTC_6				rtc_[6]	// rtc_get_reset_reason()
/* RTC_SLP_CNT_VAL:	0x6000071C */
#define IO_RTC_SLP_CNT_VAL		rtc_[7]	// the current value of RTC_COUNTER
/* IO_RTC_INT_ST:	0x60000720 */
#define IO_RTC_INT_ST			rtc_[8]
/* IO_RTC_INT_CLR:	0x60000724 */
#define IO_RTC_INT_CLR			rtc_[9]
/* IO_RTC_INT_ENA:	0x60000728 */
#define IO_RTC_INT_ENA			rtc_[10]
/* RTC_SCRATCH0: 0x60000730 */
#define IO_RTC_SCRATCH0			rtc_[12] // the register for software to save some values for watchdog reset
/* RTC_SCRATCH1:	0x60000734 */
#define IO_RTC_SCRATCH1			rtc_[13] // the register for software to save some values for watchdog reset
/* RTC_SCRATCH2:	0x60000738 */
#define IO_RTC_SCRATCH2			rtc_[14] // the register for software to save some values for watchdog reset
/* RTC_SCRATCH3:	0x6000073C */
#define IO_RTC_POWERUP			rtc_[15] // phy_set_powerup_option()
/* RTC_GPIO_OUT:	0x60000768 */
#define IO_RTC_GPIO_OUT			rtc_[26] // used by gpio16
/* RTC_GPIO_ENABLE:	0x60000774 */
#define IO_RTC_GPIO_ENABLE		rtc_[29]
/* RTC_GPIO_IN_DATA:0x6000078C */
#define IO_RTC_GPIO_IN_DATA		rtc_[35]
/* RTC_GPIO_CONF:	0x60000790 */
#define IO_RTC_GPIO_CONF		rtc_[36]
#define RTC_GPI0_CFG			rtc_[37]
#define RTC_GPI1_CFG			rtc_[38]
#define RTC_GPI2_CFG			rtc_[39]
/* RTC_GPIO0_CFG:	0x600007A0 */
#define IO_PAD_XPD_DCDC_CONF	rtc_[40] // GPIO16, XPD_DCDC
#define RTC_GPIO3_CFG			rtc_[40]
/* RTC_GPIO4_CFG:	0x600007A4 */
#define RTC_GPIO4_CFG			rtc_[41]
/* RTC_GPIO5_CFG:	0x600007A8 */
#define RTC_GPIO5_CFG			rtc_[42]


#endif /* _RTC_REGISTER_H_ */
