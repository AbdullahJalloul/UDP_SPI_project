/*
 *  Copyright (C) 2014 -2016  Espressif System
 *
 */

#ifndef __GPIO_FUNC_H__
#define __GPIO_FUNC_H__

#include 	<c_types.h>
//#include 	<ets_sys.h>
//#include	<ESP8266_registers.h>
//#include 	<gpio.h>

/*
 * Set the specified GPIO register to the specified value.
 * This is a very general and powerful interface that is not
 * expected to be used during normal operation.  It is intended
 * mainly for debug, or for unusual requirements.
 */
//void	gpio_register_set	(uint32 reg_id, uint32 value);

//void	gpio_config1		(gpio_int_st_t *config);
void	gpio16_output_conf	(void);
void	gpio16_output_set	(uint8 value);
void	gpio16_input_conf	(void);
uint8	gpio16_input_get	(void);

void	gpio_output_conf	(uint32 set_mask, uint32 clear_mask, uint32 enable_mask,
        					uint32 disable_mask);
void	gpio_intr_handler_register	(void *fn);
void	gpio_pin_wakeup_enable		(uint32 i, gpio_pin_int_t intr_state);
void	gpio_pin_wakeup_disable		(void);
void	gpio_pin_intr_state_set		(uint32 i, gpio_pin_int_t intr_state);
uint32	gpio_input_get				(void);

#endif	// __GPIO_FUNC_H__
