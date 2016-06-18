/*
 * timer_register.h
 *
 *  Created on: 18.05.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _TIMER_REGISTER_H_
#define _TIMER_REGISTER_H_

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
	__IO unsigned	interrupt_type	:1;	// bit[0]: interrupt type, 0:edge, 1:level
	__I unsigned	dummy1			:1;
	__IO unsigned	prescaler		:2;	// bit[3:2]: prescale-divider, 0: divided by 1, 1: divided by 16, 2 or 3: divided by 256
	__I	unsigned	dummy4			:2;
	__IO unsigned	reload			:1;	// bit[6]: automatically reload, when the counter  is equal to zero
	__IO unsigned	enable			:1;	// bit[7]: timer enable
	__IO unsigned	interrupt		:1;	// the status of the interrupt, when the count is equal to the alarm value
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


#endif /* _TIMER_REGISTER_H_ */
