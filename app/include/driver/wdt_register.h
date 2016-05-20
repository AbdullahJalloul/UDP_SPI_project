/*
 * wdt_register.h
 *
 *  Created on: 19.05.2016
 *      Author: Ilya Petrukhin
 */

#ifndef _WDT_REGISTER_H_
#define _WDT_REGISTER_H_

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

/******************* WDT bitfields ********************************************/

typedef struct
{
	__IO unsigned	dummy0	:1;
} wdt_control_t;

typedef struct
{
	union
	{
		__IO u32	control;	// 0x0, 0x01, 0x3C
		__IO wdt_control_t	control_bits;
	};
	__IO u32	reg_1;	// 0x0, 0xB, 0xC, 0xD
	__IO u32	reg_2;	// 0x0, 0xB, 0xC, 0xD
	__I u32		dummy0C;
	__I u32		dummy10;
	__IO u32	feed;	
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

#define WDT_FEED_MAGIC					((u32)0x00000073)


#endif /* _WDT_REGISTER_H_ */
