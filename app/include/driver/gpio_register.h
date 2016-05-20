/*
 * ESP8266 GPIO registers
 *  Added on: 17 ��� 2016 �.
 *      Author: ���� ��������
 */

#ifndef _GPIO_REGISTER_H_
#define _GPIO_REGISTER_H_

#include "c_types.h"

#define PERIPHS_GPIO_BASEADDR				0x60000300

#define GPIO_OUT_ADDRESS					0x00
#define GPIO_BT_SEL							0x0000FFFF
#define GPIO_BT_SEL_S						16
#define GPIO_OUT_DATA						0x0000FFFF
#define GPIO_OUT_DATA_S						0

#define GPIO_OUT_W1TS_ADDRESS				0x04
#define GPIO_OUT_DATA_W1TS					0x0000FFFF
#define GPIO_OUT_DATA_W1TS_S				0

#define GPIO_OUT_W1TC_ADDRESS				0x08
#define GPIO_OUT_DATA_W1TC					0x0000FFFF
#define GPIO_OUT_DATA_W1TC_S				0
//#define GPIO_OUT_DATA_MASK				0x0000FFFF

#define GPIO_ENABLE_ADDRESS					0x0c
#define GPIO_SDIO_SEL						0x0000003f
#define GPIO_SDIO_SEL_S						16
#define GPIO_ENABLE_DATA					0x0000FFFF
#define GPIO_ENABLE_DATA_S					0

#define GPIO_ENABLE_W1TS_ADDRESS			0x10
#define GPIO_ENABLE_DATA_W1TS				0x0000FFFF
#define GPIO_ENABLE_DATA_W1TS_s				0

#define GPIO_ENABLE_W1TC_ADDRESS			0x14
#define GPIO_ENABLE_DATA_W1TC				0x0000FFFF
#define GPIO_ENABLE_DATA_W1TC_S				0
#define GPIO_ENABLE_DATA_DATA_MASK			0x0000FFFF

#define GPIO_IN_ADDRESS						0x18
#define GPIO_STRAPPING						0x0000FFFF
#define GPIO_STRAPPING_S					16
#define GPIO_IN_DATA						0x0000FFFF
#define GPIO_IN_DATA_S						0

#define GPIO_STATUS_ADDRESS					0x1c
#define GPIO_STATUS_INTERRUPT				0x0000FFFF
#define GPIO_STATUS_INTERRUPT_S				0

#define GPIO_STATUS_W1TS_ADDRESS			0x20
#define GPIO_STATUS_INTERRUPT_W1TS			0x0000FFFF
#define GPIO_STATUS_INTERRUPT_W1TS_S		0

#define GPIO_STATUS_W1TC_ADDRESS			0x24
#define GPIO_STATUS_INTERRUPT_W1TC			0x0000FFFF
#define GPIO_STATUS_INTERRUPT_W1TC_S		0
#define GPIO_STATUS_INTERRUPT_DATA_MASK		0x0000FFFF

//Region1: used for gpio config for GPIO_PIN0_ADDRESS~GPIO_PIN15_ADDRESS
#define GPIO_ID_PIN0						0
#define GPIO_ID_PIN(n)						(GPIO_ID_PIN0+(n))
#define GPIO_LAST_REGISTER_ID				GPIO_ID_PIN(15)
//#define GPIO_ID_NONE						0xFFFFFFFF
#define GPIO_PIN_COUNT						16

#define GPIO_PIN_CONFIG_MSB					12
#define GPIO_PIN_CONFIG_LSB					11
//#define GPIO_PIN_CONFIG_MASK				(0x00000003<<GPIO_PIN_CONFIG_LSB)
#define GPIO_PIN_CONFIG_GET(x)				(((x) & GPIO_PIN_CONFIG_MASK) >> GPIO_PIN_CONFIG_LSB)
#define GPIO_PIN_CONFIG_SET(x)				(((x) << GPIO_PIN_CONFIG_LSB) & GPIO_PIN_CONFIG_MASK)

#define GPIO_WAKEUP_ENABLE					1
#define GPIO_WAKEUP_DISABLE					(~GPIO_WAKEUP_ENABLE)
#define GPIO_PIN_WAKEUP_ENABLE_MSB			10
#define GPIO_PIN_WAKEUP_ENABLE_LSB			10
//#define GPIO_PIN_WAKEUP_ENABLE_MASK		(0x00000001<<GPIO_PIN_WAKEUP_ENABLE_LSB)
//#define GPIO_PIN_WAKEUP_ENABLE_GET(x)		(((x) & GPIO_PIN_CONFIG_MASK) >> GPIO_PIN_CONFIG_LSB)
#define GPIO_PIN_WAKEUP_ENABLE_SET(x)		(((x) << GPIO_PIN_WAKEUP_ENABLE_LSB) & GPIO_PIN_WAKEUP_ENABLE_MASK)

#define GPIO_PIN_INT_TYPE_MSB				9
#define GPIO_PIN_INT_TYPE_LSB				7
//#define GPIO_PIN_INT_TYPE_MASK			(0x00000007<<GPIO_PIN_INT_TYPE_LSB)
#define GPIO_PIN_INT_TYPE_GET(x)			(((x) & GPIO_PIN_INT_TYPE_MASK) >> GPIO_PIN_INT_TYPE_LSB)
#define GPIO_PIN_INT_TYPE_SET(x)			(((x) << GPIO_PIN_INT_TYPE_LSB) & GPIO_PIN_INT_TYPE_MASK)

#define GPIO_PAD_DRIVER_ENABLE				1
#define GPIO_PAD_DRIVER_DISABLE				(~GPIO_PAD_DRIVER_ENABLE)
#define GPIO_PIN_DRIVER_MSB					2
#define GPIO_PIN_DRIVER_LSB					2
#define GPIO_PIN_DRIVER_MASK				(0x00000001<<GPIO_PIN_DRIVER_LSB)
#define GPIO_PIN_DRIVER_GET(x)				(((x) & GPIO_PIN_INT_TYPE_MASK) >> GPIO_PIN_INT_TYPE_LSB)
//#define GPIO_PIN_PAD_DRIVER_SET(x)		(((x) << GPIO_PIN_DRIVER_LSB) & GPIO_PIN_DRIVER_MASK)

#define GPIO_PIN_SOURCE_MSB					0
#define GPIO_PIN_SOURCE_LSB					0
//#define GPIO_PIN_SOURCE_MASK				(0x00000001<<GPIO_PIN_SOURCE_LSB)
//#define GPIO_PIN_SOURCE_GET(x)			(((x) & GPIO_PIN_INT_TYPE_MASK) >> GPIO_PIN_INT_TYPE_LSB)
#define GPIO_PIN_SOURCE_SET(x)				(((x) << GPIO_PIN_SOURCE_LSB) & GPIO_PIN_SOURCE_MASK)
//end of region1

#define GPIO_PIN0_ADDRESS					0x28
#define GPIO_PIN0_CONFIG					0x00000003
#define GPIO_PIN0_CONFIG_S					11
#define GPIO_PIN0_WAKEUP_ENABLE				BIT10
#define GPIO_PIN0_WAKEUP_ENABLE_S			10
#define GPIO_PIN0_INT_TYPE					0x00000007
#define GPIO_PIN0_INT_TYPE_S				7
#define GPIO_PIN0_DRIVER					BIT2
#define GPIO_PIN0_DRIVER_S					2
#define GPIO_PIN0_SOURCE					BIT0
#define GPIO_PIN0_SOURCE_S					0

#define GPIO_PIN1_ADDRESS					0x2c
#define GPIO_PIN1_CONFIG					0x00000003
#define GPIO_PIN1_CONFIG_S					11
#define GPIO_PIN1_WAKEUP_ENABLE				BIT10
#define GPIO_PIN1_WAKEUP_ENABLE_S			10
#define GPIO_PIN1_INT_TYPE					0x00000007
#define GPIO_PIN1_INT_TYPE_S				7
#define GPIO_PIN1_DRIVER					BIT2
#define GPIO_PIN1_DRIVER_S					2
#define GPIO_PIN1_SOURCE					BIT0
#define GPIO_PIN1_SOURCE_S					0

#define GPIO_PIN2_ADDRESS					0x30
#define GPIO_PIN2_CONFIG					0x00000003
#define GPIO_PIN2_CONFIG_S					11
#define GPIO_PIN2_WAKEUP_ENABLE				BIT10
#define GPIO_PIN2_WAKEUP_ENABLE_S			10
#define GPIO_PIN2_INT_TYPE					0x00000007
#define GPIO_PIN2_INT_TYPE_S				7
#define GPIO_PIN2_DRIVER					BIT2
#define GPIO_PIN2_DRIVER_S					2
#define GPIO_PIN2_SOURCE					BIT0
#define GPIO_PIN2_SOURCE_S					0

#define GPIO_PIN3_ADDRESS					0x34
#define GPIO_PIN3_CONFIG					0x00000003
#define GPIO_PIN3_CONFIG_S					11
#define GPIO_PIN3_WAKEUP_ENABLE				BIT10
#define GPIO_PIN3_WAKEUP_ENABLE_S			10
#define GPIO_PIN3_INT_TYPE					0x00000007
#define GPIO_PIN3_INT_TYPE_S				7
#define GPIO_PIN3_DRIVER					BIT2
#define GPIO_PIN3_DRIVER_S					2
#define GPIO_PIN3_SOURCE					BIT0
#define GPIO_PIN3_SOURCE_S					0

#define GPIO_PIN4_ADDRESS					0x38
#define GPIO_PIN4_CONFIG					0x00000003
#define GPIO_PIN4_CONFIG_S					11
#define GPIO_PIN4_WAKEUP_ENABLE				BIT10
#define GPIO_PIN4_WAKEUP_ENABLE_S			10
#define GPIO_PIN4_INT_TYPE					0x00000007
#define GPIO_PIN4_INT_TYPE_S				7
#define GPIO_PIN4_DRIVER					BIT2
#define GPIO_PIN4_DRIVER_S					2
#define GPIO_PIN4_SOURCE					BIT0
#define GPIO_PIN4_SOURCE_S					0

#define GPIO_PIN5_ADDRESS					0x3c
#define GPIO_PIN5_CONFIG					0x00000003
#define GPIO_PIN5_CONFIG_S					11
#define GPIO_PIN5_WAKEUP_ENABLE				BIT10
#define GPIO_PIN5_WAKEUP_ENABLE_S			10
#define GPIO_PIN5_INT_TYPE					0x00000007
#define GPIO_PIN5_INT_TYPE_S				7
#define GPIO_PIN5_DRIVER					BIT2
#define GPIO_PIN5_DRIVER_S					2
#define GPIO_PIN5_SOURCE					BIT0
#define GPIO_PIN5_SOURCE_S					0

#define GPIO_PIN6_ADDRESS					0x40
#define GPIO_PIN6_CONFIG					0x00000003
#define GPIO_PIN6_CONFIG_S					11
#define GPIO_PIN6_WAKEUP_ENABLE				BIT10
#define GPIO_PIN6_WAKEUP_ENABLE_S			10
#define GPIO_PIN6_INT_TYPE					0x00000007
#define GPIO_PIN6_INT_TYPE_S				7
#define GPIO_PIN6_DRIVER					BIT2
#define GPIO_PIN6_DRIVER_S					2
#define GPIO_PIN6_SOURCE					BIT0
#define GPIO_PIN6_SOURCE_S					0

#define GPIO_PIN7_ADDRESS					0x44
#define GPIO_PIN7_CONFIG					0x00000003
#define GPIO_PIN7_CONFIG_S					11
#define GPIO_PIN7_WAKEUP_ENABLE				BIT10
#define GPIO_PIN7_WAKEUP_ENABLE_S			10
#define GPIO_PIN7_INT_TYPE					0x00000007
#define GPIO_PIN7_INT_TYPE_S				7
#define GPIO_PIN7_DRIVER					BIT2
#define GPIO_PIN7_DRIVER_S					2
#define GPIO_PIN7_SOURCE					BIT0
#define GPIO_PIN7_SOURCE_S					0

#define GPIO_PIN8_ADDRESS					0x48
#define GPIO_PIN8_CONFIG					0x00000003
#define GPIO_PIN8_CONFIG_S					11
#define GPIO_PIN8_WAKEUP_ENABLE				BIT10
#define GPIO_PIN8_WAKEUP_ENABLE_S			10
#define GPIO_PIN8_INT_TYPE					0x00000007
#define GPIO_PIN8_INT_TYPE_S				7
#define GPIO_PIN8_DRIVER					BIT2
#define GPIO_PIN8_DRIVER_S					2
#define GPIO_PIN8_SOURCE					BIT0
#define GPIO_PIN8_SOURCE_S					0

#define GPIO_PIN9_ADDRESS					0x4c
#define GPIO_PIN9_CONFIG					0x00000003
#define GPIO_PIN9_CONFIG_S					11
#define GPIO_PIN9_WAKEUP_ENABLE				BIT10
#define GPIO_PIN9_WAKEUP_ENABLE_S			10
#define GPIO_PIN9_INT_TYPE					0x00000007
#define GPIO_PIN9_INT_TYPE_S				7
#define GPIO_PIN9_DRIVER					BIT2
#define GPIO_PIN9_DRIVER_S					2
#define GPIO_PIN9_SOURCE					BIT0
#define GPIO_PIN9_SOURCE_S					0

#define GPIO_PIN10_ADDRESS					0x50
#define GPIO_PIN10_CONFIG					0x00000003
#define GPIO_PIN10_CONFIG_S					11
#define GPIO_PIN10_WAKEUP_ENABLE			BIT10
#define GPIO_PIN10_WAKEUP_ENABLE_S			10
#define GPIO_PIN10_INT_TYPE					0x00000007
#define GPIO_PIN10_INT_TYPE_S				7
#define GPIO_PIN10_DRIVER					BIT2
#define GPIO_PIN10_DRIVER_S					2
#define GPIO_PIN10_SOURCE					BIT0
#define GPIO_PIN10_SOURCE_S					0

#define GPIO_PIN11_ADDRESS					0x54
#define GPIO_PIN11_CONFIG					0x00000003
#define GPIO_PIN11_CONFIG_S					11
#define GPIO_PIN11_WAKEUP_ENABLE			BIT10
#define GPIO_PIN11_WAKEUP_ENABLE_S			10
#define GPIO_PIN11_INT_TYPE					0x00000007
#define GPIO_PIN11_INT_TYPE_S				7
#define GPIO_PIN11_DRIVER					BIT2
#define GPIO_PIN11_DRIVER_S					2
#define GPIO_PIN11_SOURCE					BIT0
#define GPIO_PIN11_SOURCE_S					0

#define GPIO_PIN12_ADDRESS					0x58
#define GPIO_PIN12_CONFIG					0x00000003
#define GPIO_PIN12_CONFIG_S					11
#define GPIO_PIN12_WAKEUP_ENABLE			BIT10
#define GPIO_PIN12_WAKEUP_ENABLE_S			10
#define GPIO_PIN12_INT_TYPE					0x00000007
#define GPIO_PIN12_INT_TYPE_S				7
#define GPIO_PIN12_DRIVER					BIT2
#define GPIO_PIN12_DRIVER_S					2
#define GPIO_PIN12_SOURCE					BIT0
#define GPIO_PIN12_SOURCE_S					0

#define GPIO_PIN13_ADDRESS					0x5c
#define GPIO_PIN13_CONFIG					0x00000003
#define GPIO_PIN13_CONFIG_S					11
#define GPIO_PIN13_WAKEUP_ENABLE			BIT10
#define GPIO_PIN13_WAKEUP_ENABLE_S			10
#define GPIO_PIN13_INT_TYPE					0x00000007
#define GPIO_PIN13_INT_TYPE_S				7
#define GPIO_PIN13_DRIVER					BIT2
#define GPIO_PIN13_DRIVER_S					2
#define GPIO_PIN13_SOURCE					BIT0
#define GPIO_PIN13_SOURCE_S					0

#define GPIO_PIN14_ADDRESS					0x60
#define GPIO_PIN14_CONFIG					0x00000003
#define GPIO_PIN14_CONFIG_S					11
#define GPIO_PIN14_WAKEUP_ENABLE			BIT10
#define GPIO_PIN14_WAKEUP_ENABLE_S			10
#define GPIO_PIN14_INT_TYPE					0x00000007
#define GPIO_PIN14_INT_TYPE_S				7
#define GPIO_PIN14_DRIVER					BIT2
#define GPIO_PIN14_DRIVER_S					2
#define GPIO_PIN14_SOURCE					BIT0
#define GPIO_PIN14_SOURCE_S					0

#define GPIO_PIN15_ADDRESS					0x64
#define GPIO_PIN15_CONFIG					0x00000003
#define GPIO_PIN15_CONFIG_S					11
#define GPIO_PIN15_WAKEUP_ENABLE			BIT10
#define GPIO_PIN15_WAKEUP_ENABLE_S			10
#define GPIO_PIN15_INT_TYPE					0x00000007
#define GPIO_PIN15_INT_TYPE_S				7
#define GPIO_PIN15_DRIVER					BIT2
#define GPIO_PIN15_DRIVER_S					2
#define GPIO_PIN15_SOURCE					BIT0
#define GPIO_PIN15_SOURCE_S					0

#define GPIO_SIGMA_DELTA_ADDRESS			0x68

#define SIGMA_DELTA_ENABLE					(1<<16)
#define SIGMA_DELTA_ENABLE_S				16
#define SIGMA_DELTA_PRESCALAR				0x000000ff
#define SIGMA_DELTA_PRESCALAR_S				8
#define SIGMA_DELTA_TARGET					0x000000ff
#define SIGMA_DELTA_TARGET_S				0
#define SIGMA_DELTA_SETTING_MASK			0x00000001ff

#define GPIO_RTC_CALIB_SYNC_ADDRESS			0x6c
//#define RTC_CALIB_START					(1<<31)
#define RTC_CALIB_START_S					31
#define RTC_PERIOD_NUM						0x000003ff
#define RTC_PERIOD_NUM_S					0

#define GPIO_RTC_CALIB_VALUE_ADDRESS		0x70
#define RTC_CALIB_RDY						(1<<31)
#define RTC_CALIB_RDY_S						31
#define RTC_CALIB_RDY_REAL					(1<<30)
#define RTC_CALIB_RDY_REAL_S				30
//#define RTC_CALIB_VALUE_MASK				0x000FFFFf
#define RTC_CALIB_VALUE_S					0

#define GPIO_REG_READ(reg)					READ_PERI_REG(PERIPHS_GPIO_BASEADDR + reg)
#define GPIO_REG_WRITE(reg, val)			WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + reg, val)

#ifndef __I
#define	__I		volatile const	/*!< defines 'read only' permissions	*/
#endif	//#ifndef __I

#ifndef __O
#define	__O		volatile		/*!< defines 'write only' permissions	*/
#endif	//#ifndef __O

#ifndef __IO
#define	__IO	volatile		/*!< defines 'read / write' permissions*/
#endif	// #ifndef __IO

/*********************** GPIO structures **************************************/
typedef struct
{
	__IO u16		bt_sel	:16;	// BT-Coexist Selection register
	__IO unsigned	pin_15	:1;		// The output value when the GPIO pin is set as output.
	__IO unsigned	pin_14	:1;
	__IO unsigned	pin_13	:1;
	__IO unsigned	pin_12	:1;
	__IO unsigned	pin_11	:1;
	__IO unsigned	pin_10	:1;
	__IO unsigned	pin_9	:1;
	__IO unsigned	pin_8	:1;
	__IO unsigned	pin_7	:1;
	__IO unsigned	pin_6	:1;
	__IO unsigned	pin_5	:1;
	__IO unsigned	pin_4	:1;
	__IO unsigned	pin_3	:1;
	__IO unsigned	pin_2	:1;
	__IO unsigned	pin_1	:1;
	__IO unsigned	pin_0	:1;
} GPIO_out_t;

typedef struct
{
	__O u16			dummy16	:16;
	__O unsigned	pin_15	:1;
	__O unsigned	pin_14	:1;
	__O unsigned	pin_13	:1;
	__O unsigned	pin_12	:1;
	__O unsigned	pin_11	:1;
	__O unsigned	pin_10	:1;
	__O unsigned	pin_9	:1;
	__O unsigned	pin_8	:1;
	__O unsigned	pin_7	:1;
	__O unsigned	pin_6	:1;
	__O unsigned	pin_5	:1;
	__O unsigned	pin_4	:1;
	__O unsigned	pin_3	:1;
	__O unsigned	pin_2	:1;
	__O unsigned	pin_1	:1;
	__O unsigned	pin_0	:1;
} GPIO_out_w_t;

typedef struct
{
	__IO u16		dummy21	:10;
	__IO u8			sdio_sel :6;	// SDIO-dis selection register
	__IO unsigned	pin_15	:1;		// The output enable register.
	__IO unsigned	pin_14	:1;
	__IO unsigned	pin_13	:1;
	__IO unsigned	pin_12	:1;
	__IO unsigned	pin_11	:1;
	__IO unsigned	pin_10	:1;
	__IO unsigned	pin_9	:1;
	__IO unsigned	pin_8	:1;
	__IO unsigned	pin_7	:1;
	__IO unsigned	pin_6	:1;
	__IO unsigned	pin_5	:1;
	__IO unsigned	pin_4	:1;
	__IO unsigned	pin_3	:1;
	__IO unsigned	pin_2	:1;
	__IO unsigned	pin_1	:1;
	__IO unsigned	pin_0	:1;
} GPIO_enable_t;

typedef struct
{
	__IO unsigned	strap_pin_15	:1;	// The values of the strapping pins.
	__IO unsigned	strap_pin_14	:1;
	__IO unsigned	strap_pin_13	:1;
	__IO unsigned	strap_pin_12	:1;
	__IO unsigned	strap_pin_11	:1;
	__IO unsigned	strap_pin_10	:1;
	__IO unsigned	strap_pin_9		:1;
	__IO unsigned	strap_pin_8		:1;
	__IO unsigned	strap_pin_7		:1;
	__IO unsigned	strap_pin_6		:1;
	__IO unsigned	strap_pin_5		:1;
	__IO unsigned	strap_pin_4		:1;
	__IO unsigned	strap_pin_3		:1;
	__IO unsigned	strap_pin_2		:1;
	__IO unsigned	strap_pin_1		:1;
	__IO unsigned	strap_pin_0		:1;
	__I unsigned	pin_15			:1;	// The values of the GPIO pins when the GPIO pin is set as input.
	__I unsigned	pin_14			:1;
	__I unsigned	pin_13			:1;
	__I unsigned	pin_12			:1;
	__I unsigned	pin_11			:1;
	__I unsigned	pin_10			:1;
	__I unsigned	pin_9			:1;
	__I unsigned	pin_8			:1;
	__I unsigned	pin_7			:1;
	__I unsigned	pin_6			:1;
	__I unsigned	pin_5			:1;
	__I unsigned	pin_4			:1;
	__I unsigned	pin_3			:1;
	__I unsigned	pin_2			:1;
	__I unsigned	pin_1			:1;
	__I unsigned	pin_0			:1;
} GPIO_in_t;

typedef struct
{
	__IO u16		dummy16	:16;
	__IO unsigned	pin_15	:1;	// Interrupt enable register.
	__IO unsigned	pin_14	:1;
	__IO unsigned	pin_13	:1;
	__IO unsigned	pin_12	:1;
	__IO unsigned	pin_11	:1;
	__IO unsigned	pin_10	:1;
	__IO unsigned	pin_9	:1;
	__IO unsigned	pin_8	:1;
	__IO unsigned	pin_7	:1;
	__IO unsigned	pin_6	:1;
	__IO unsigned	pin_5	:1;
	__IO unsigned	pin_4	:1;
	__IO unsigned	pin_3	:1;
	__IO unsigned	pin_2	:1;
	__IO unsigned	pin_1	:1;
	__IO unsigned	pin_0	:1;
} GPIO_status_t;

typedef enum _gpio_pin_source_t
{
	GPIO_DATA	= 0,
	SIGMA_DELTA	= 1
} gpio_pin_source_t;

typedef enum _gpio_pin_driver_t
{
	NORMAL		= 0,
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

typedef enum
{
	GPIO_MODE_INPUT = 0,
	GPIO_MODE_OUTPUT_OD,
	GPIO_MODE_OUTPUT_PP,
	GPIO_MODE_SIGMA_DELTA,
} gpio_mode_t;

typedef enum
{
	GPIO_PULLUP_DISABLE = 0,
	GPIO_PULLUP_ENABLE
} gpio_pullup_t;

typedef struct
{
	u8 				num_pin;
	gpio_mode_t 	mode;
	gpio_pullup_t 	pullup;
	gpio_pin_int_t	interrupt;
} gpio_config_t;

typedef struct
{
	__IO u32				dummy13			:19;
	__IO u8					config			:2;
	__IO unsigned			wakeup_enable	:1;	// 0: disable; 1: enable GPIO wakeup CPU, only when GPIO_PIN0_INT_TYPE is 0x4 or 0x5
	__IO gpio_pin_int_t		pin_int			:3;	// 0: disable; 1: positive edge; 2: negative edge; 3: both types of edge; 4: low-level; 5: high-level
	__IO u8					dummy3			:4;
	__IO gpio_pin_driver_t	pin_driver		:1;	// 1: open drain; 0: normal
	__IO unsigned			dummy1			:1;
	__IO gpio_pin_source_t	gpio_source		:1;	// 1: sigma-delta; 0: GPIO_DATA
} GPIO_pin_t;

typedef struct
{
	__IO u16		dummy17		:15;
	__IO unsigned	enable		:1;	// 1: enable sigma-delta; 0: disable
	__IO u8			prescaler	:8;	// Clock pre-divider for sigma-delta.
	__IO u8 		target		:8;	// target level of the sigma-delta. It is a signed byte.
} GPIO_sigma_delta_t;

typedef struct
{
	__IO unsigned	rtc_calib_start	:1;		// Positvie edge of this bit will trigger the RTC-clock-calibration process.
	__IO u32		dummy10			:21;
	__IO u16		rtc_period_num	:10;	// The cycle number of RTC-clock during RTC-clock-calibration
} GPIO_rtc_calib_sync_t;

typedef struct
{
	__I unsigned	calib_rdy		:1;	// 0: during RTC-clock-calibration; 1: RTC-clock-calibration is done
	__I unsigned	calib_rdy_real	:1;	// 0: during RTC-clock-calibration; 1: RTC-clock-calibration is done
	__I u16			dummy20			:10;
	__I u32			rtc_calib_value	:20;	// The cycle number of clk_xtal (crystal clock) for the RTC_PERIOD_NUM cycles of RTC-clock
} GPIO_rtc_calib_value_t;

typedef struct GPIO_struct
{
	union
	{
		__IO u32			out;
		__IO GPIO_out_t 	out_bits;	// The output value when the GPIO pin is set as output.
	};
	union
	{
		__O u32				out_w1ts;
		__O GPIO_out_w_t	out_w1ts_bits;	// Writing 1 into a bit in this register will set the related bit in GPIO_OUT_DATA
	};
	union
	{
		__O u32				out_w1tc;
		__O GPIO_out_w_t	out_w1tc_bits;	// Writing 1 into a bit in this register will clear the related bit in GPIO_OUT_DATA
	};
	union
	{
		__IO u32			enable;
		__IO GPIO_enable_t	enable_bits;	// The output enable register.
	};
	union
	{
		__O u32				enable_w1ts;
		__O GPIO_out_w_t	enable_w1ts_bits;	// Writing 1 into a bit in this register will set the related bit in GPIO_ENABLE_DATA
	};
	union
	{
		__O u32				enable_w1tc;
		__O GPIO_out_w_t	enable_w1tc_bits;	// Writing 1 into a bit in this register will clear the related bit in GPIO_ENABLE_DATA
	};
	union
	{
		__IO u32			in;
		__IO GPIO_in_t		in_bits;	// The values of the GPIO pins when the GPIO pin is set as input.
	};
	union
	{
		__IO u32			status;
		__IO GPIO_status_t	status_bits;	// Interrupt enable register.
	};
	union
	{
		__O u32				status_w1ts;
		__O GPIO_out_w_t	status_w1ts_bits;	// Writing 1 into a bit in this register will set the related bit in GPIO_STATUS_INTERRUPT
	};
	union
	{
		__O u32				status_w1tc;
		__O GPIO_out_w_t	statust_w1tc_bits;	// Writing 1 into a bit in this register will clear the related bit in GPIO_STATUS_INTERRUPT
	};
	union
	{
		__IO u32			pin_0;
		__IO GPIO_pin_t		pin_0_bits;
	};
	union
	{
		__IO u32			pin_1;
		__IO GPIO_pin_t		pin_1_bits;
	};
	union
	{
		__IO u32			pin_2;
		__IO GPIO_pin_t		pin_2_bits;
	};
	union
	{
		__IO u32			pin_3;
		__IO GPIO_pin_t		pin_3_bits;
	};
	union
	{
		__IO u32			pin_4;
		__IO GPIO_pin_t		pin_4_bits;
	};
	union
	{
		__IO u32			pin_5;
		__IO GPIO_pin_t		pin_5_bits;
	};
	union
	{
		__IO u32			pin_6;
		__IO GPIO_pin_t		pin_6_bits;
	};
	union
	{
		__IO u32			pin_7;
		__IO GPIO_pin_t		pin_7_bits;
	};
	union
	{
		__IO u32			pin_8;
		__IO GPIO_pin_t		pin_8_bits;
	};
	union
	{
		__IO u32			pin_9;
		__IO GPIO_pin_t		pin_9_bits;
	};
	union
	{
		__IO u32			pin_10;
		__IO GPIO_pin_t		pin_10_bits;
	};
	union
	{
		__IO u32			pin_11;
		__IO GPIO_pin_t		pin_11_bits;
	};
	union
	{
		__IO u32			pin_12;
		__IO GPIO_pin_t		pin_12_bits;
	};
	union
	{
		__IO u32			pin_13;
		__IO GPIO_pin_t		pin_13_bits;
	};
	union
	{
		__IO u32			pin_14;
		__IO GPIO_pin_t		pin_14_bits;
	};
	union
	{
		__IO u32			pin_15;
		__IO GPIO_pin_t		pin_15_bits;
	};
	union
	{
		__IO u32				sigma_delta;
		__IO GPIO_sigma_delta_t	sigma_delta_bits;
	};
	union
	{
		__IO u32					rtc_calib_sync;
		__IO GPIO_rtc_calib_sync_t	rtc_calib_sync_bits;
	};
	union
	{
		__IO u32					rtc_calib_value;
		__IO GPIO_rtc_calib_value_t	rtc_calib_value_bits;
	};
} GPIO_typedef;

#define GPIO_BaseAddress			0x60000300

#define GPIO 		((GPIO_typedef *)GPIO_BaseAddress)

// ��������� ��������� ����� ������
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

//  �������� ������� �����
#define GPIO_OUT_BT_SEL_MASK			((u32)0xFFFF0000)
#define GPIO_OUT_BT_SEL_S				16
#define GPIO_OUT_DATA_MASK				((u32)0x0000FFFF)
#define GPIO_OUT_DATA_PIN_15			((u32)0x00008000)
#define GPIO_OUT_DATA_PIN_14			((u32)0x00004000)
#define GPIO_OUT_DATA_PIN_13			((u32)0x00002000)
#define GPIO_OUT_DATA_PIN_12			((u32)0x00001000)
#define GPIO_OUT_DATA_PIN_11			((u32)0x00000800)
#define GPIO_OUT_DATA_PIN_10			((u32)0x00000400)
#define GPIO_OUT_DATA_PIN_9				((u32)0x00000200)
#define GPIO_OUT_DATA_PIN_8				((u32)0x00000100)
#define GPIO_OUT_DATA_PIN_7				((u32)0x00000080)
#define GPIO_OUT_DATA_PIN_6				((u32)0x00000040)
#define GPIO_OUT_DATA_PIN_5				((u32)0x00000020)
#define GPIO_OUT_DATA_PIN_4				((u32)0x00000010)
#define GPIO_OUT_DATA_PIN_3				((u32)0x00000008)
#define GPIO_OUT_DATA_PIN_2				((u32)0x00000004)
#define GPIO_OUT_DATA_PIN_1				((u32)0x00000002)
#define GPIO_OUT_DATA_PIN_0				((u32)0x00000001)

#define GPIO_OUT_W1TS_MASK				((u32)0x0000FFFF)
#define GPIO_OUT_W1TS_PIN_15			((u32)0x00008000)
#define GPIO_OUT_W1TS_PIN_14			((u32)0x00004000)
#define GPIO_OUT_W1TS_PIN_13			((u32)0x00002000)
#define GPIO_OUT_W1TS_PIN_12			((u32)0x00001000)
#define GPIO_OUT_W1TS_PIN_11			((u32)0x00000800)
#define GPIO_OUT_W1TS_PIN_10			((u32)0x00000400)
#define GPIO_OUT_W1TS_PIN_9				((u32)0x00000200)
#define GPIO_OUT_W1TS_PIN_8				((u32)0x00000100)
#define GPIO_OUT_W1TS_PIN_7				((u32)0x00000080)
#define GPIO_OUT_W1TS_PIN_6				((u32)0x00000040)
#define GPIO_OUT_W1TS_PIN_5				((u32)0x00000020)
#define GPIO_OUT_W1TS_PIN_4				((u32)0x00000010)
#define GPIO_OUT_W1TS_PIN_3				((u32)0x00000008)
#define GPIO_OUT_W1TS_PIN_2				((u32)0x00000004)
#define GPIO_OUT_W1TS_PIN_1				((u32)0x00000002)
#define GPIO_OUT_W1TS_PIN_0				((u32)0x00000001)

#define GPIO_OUT_W1TC_MASK				((u32)0x0000FFFF)
#define GPIO_OUT_W1TC_PIN_15			((u32)0x00008000)
#define GPIO_OUT_W1TC_PIN_14			((u32)0x00004000)
#define GPIO_OUT_W1TC_PIN_13			((u32)0x00002000)
#define GPIO_OUT_W1TC_PIN_12			((u32)0x00001000)
#define GPIO_OUT_W1TC_PIN_11			((u32)0x00000800)
#define GPIO_OUT_W1TC_PIN_10			((u32)0x00000400)
#define GPIO_OUT_W1TC_PIN_9				((u32)0x00000200)
#define GPIO_OUT_W1TC_PIN_8				((u32)0x00000100)
#define GPIO_OUT_W1TC_PIN_7				((u32)0x00000080)
#define GPIO_OUT_W1TC_PIN_6				((u32)0x00000040)
#define GPIO_OUT_W1TC_PIN_5				((u32)0x00000020)
#define GPIO_OUT_W1TC_PIN_4				((u32)0x00000010)
#define GPIO_OUT_W1TC_PIN_3				((u32)0x00000008)
#define GPIO_OUT_W1TC_PIN_2				((u32)0x00000004)
#define GPIO_OUT_W1TC_PIN_1				((u32)0x00000002)
#define GPIO_OUT_W1TC_PIN_0				((u32)0x00000001)

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

#define GPIO_STATUS_INT_MASK			((u32)0x0000FFFF)
#define GPIO_STATUS_INT_PIN_15			((u32)0x00008000)
#define GPIO_STATUS_INT_PIN_14			((u32)0x00004000)
#define GPIO_STATUS_INT_PIN_13			((u32)0x00002000)
#define GPIO_STATUS_INT_PIN_12			((u32)0x00001000)
#define GPIO_STATUS_INT_PIN_11			((u32)0x00000800)
#define GPIO_STATUS_INT_PIN_10			((u32)0x00000400)
#define GPIO_STATUS_INT_PIN_9			((u32)0x00000200)
#define GPIO_STATUS_INT_PIN_8			((u32)0x00000100)
#define GPIO_STATUS_INT_PIN_7			((u32)0x00000080)
#define GPIO_STATUS_INT_PIN_6			((u32)0x00000040)
#define GPIO_STATUS_INT_PIN_5			((u32)0x00000020)
#define GPIO_STATUS_INT_PIN_4			((u32)0x00000010)
#define GPIO_STATUS_INT_PIN_3			((u32)0x00000008)
#define GPIO_STATUS_INT_PIN_2			((u32)0x00000004)
#define GPIO_STATUS_INT_PIN_1			((u32)0x00000002)
#define GPIO_STATUS_INT_PIN_0			((u32)0x00000001)

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
#define GPIO_PIN_CONFIG_S				11
#define GPIO_PIN_WAKEUP_ENABLE			((u32)0x00000400)
#define GPIO_PIN_WAKEUP_ENABLE_S		10
#define GPIO_PIN_INT_TYPE__MASK			((u32)0x00000380)
#define GPIO_PIN_INT_TYPE_S				7
#define GPIO_PIN_DRIVER					((u32)0x00000004)
#define GPIO_PIN_DRIVER_S				2
#define GPIO_PIN_SOURCE					((u32)0x00000001)

#define GPIO_SIGMA_DELTA_ENABLE			((u32)0x00010000)
#define GPIO_SIGMA_DELTA_PRESCALER_MASK	((u32)0x0000FF00)
#define GPIO_SIGMA_DELTA_PRESCALER_S	8
#define GPIO_SIGMA_DELTA_TARGET_MASK	((u32)0x000000FF)

#define GPIO_RTC_CALIB_START			((u32)0x80000000)
#define GPIO_RTC_PERIOD_NUM_MASK		((u32)0x000003ff)

#define GPIO_RTC_CALIB_RDY				((u32)0x80000000)
#define GPIO_RTC_CALIB_RDY_REAL			((u32)0x40000000)
#define GPIO_RTC_CALIB_VALUE_MASK		((u32)0x000FFFFF)

#endif	// #ifndef _GPIO_REGISTER_H_
