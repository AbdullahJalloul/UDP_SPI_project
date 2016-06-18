/*
 * sar_registers.h
 *
 *  Created on: 02.06.2016.
 *      Author: Ilya Petrukhin
 */

#ifndef _SAR_REGISTERS_H_
#define _SAR_REGISTERS_H_

#include "c_types.h"

/******************************************************************************/

#ifndef __I
#define     __I     volatile const   /*!< defines 'read only' permissions     */
#endif	// #ifndef __I

#ifndef __O
#define     __O     volatile         /*!< defines 'write only' permissions    */
#endif	// #ifndef __O

#ifndef __IO
#define     __IO    volatile         /*!< defines 'read / write' permissions  */
#endif	// #ifndef __IO

/************************ SAR Bitfield ****************************************/
typedef struct
{
	__IO unsigned	start		:1;	// Бит 1: запуск нового замера SAR
	__IO unsigned	num_data	:3;	// Бит 2..4: кол-во значений в SAR_DATA 0..7 -> 1..8	
	__I unsigned	dummy3		:20;	// 
	__IO unsigned	r_state		:3;		// Бит 24..26: готовность r_state = 0
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



#endif /* _SAR_REGISTERS_H_ */
