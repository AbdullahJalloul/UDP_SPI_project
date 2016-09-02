/*
 *  Copyright (C) 2014 -2016  Espressif System
 *
 */

//#include "espressif/esp_common.h"
//#include "freertos/portmacro.h"
#include	<ESP8266_registers.h>
#include	<gpio.h>
#include	<eagle_soc.h>
#include	<ets_sys.h>
#include	"driver/gpio_func.h"


/*void ICACHE_FLASH_ATTR gpio_config1 (gpio_config_t *config)
{
	u32 pin_mask = 1 << (config->num_pin);
	__IO GPIO_pin_t *p_pin = &GPIO->pin_0_bits + sizeof(GPIO_pin_t) * (config->num_pin);
	__IO u32 *p_mux = (__IO u32 *)gpio_mux_registers[config->num_pin];
//	__IO gpio_mux_t *p_mux_bits = gpio_mux_registers[config->num_pin];

	switch (config->mode)
	{
	case GPIO_MODE_INPUT:
		GPIO->enable_w1tc = pin_mask;
		break;

	case GPIO_MODE_OUTPUT_OD:
		p_pin->pin_driver = OPEN_DRAIN;
		break;

	case GPIO_MODE_OUTPUT_PP:
		GPIO->enable_w1ts = pin_mask;
		break;

	case GPIO_MODE_SIGMA_DELTA:
		p_pin->gpio_source = SIGMA_DELTA;
		break;

	default:	// == GPIO_MODE_INPUT
		GPIO->enable_w1tc = pin_mask;
		break;
	}
// pin in GPIO mode
	if ( (config->num_pin == 0) || (config->num_pin == 2)
	        || (config->num_pin == 4) || (config->num_pin == 5))
	{
		*p_mux = (*p_mux & ~GPIO_MUX_FUNC_MASK) | GPIO_MUX_FUNC_0;
	}
	else
	{
		*p_mux = (*p_mux & ~GPIO_MUX_FUNC_MASK) | GPIO_MUX_FUNC_3;
	}
// pull-up
	((gpio_mux_t *)p_mux)->pullup = config->pullup;
// interrupt state
	p_pin->pin_int = config->interrupt;
}*/

void ICACHE_FLASH_ATTR gpio_config (GPIO_ConfigTypeDef *pGPIOConfig)
{
	uint16 gpio_pin_mask = pGPIOConfig->GPIO_Pin;
	uint32 io_reg;
	uint8 io_num = 0;
	uint32 pin_reg;

	if (pGPIOConfig->GPIO_Mode == GPIO_Mode_Input)
	{
//		GPIO_AS_INPUT (gpio_pin_mask);
		GPIO->enable_w1tc = gpio_pin_mask;
	}
	else if (pGPIOConfig->GPIO_Mode == GPIO_Mode_Output)
	{
//		GPIO_AS_OUTPUT (gpio_pin_mask);
		GPIO->enable_w1ts = gpio_pin_mask;
	}
	do
	{
		if ( (gpio_pin_mask >> io_num) & 0x1)
		{
			io_reg = *(GPIO_PIN_REG(io_num));

			if ( (0x1 << io_num)
			        & (GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5))
			{
				pin_func_select ( (GPIO_PIN_REG(io_num)), 0);
			}
			else
			{
				pin_func_select ( (GPIO_PIN_REG(io_num)), 3);
			}

			if (pGPIOConfig->GPIO_Pullup)
			{
				PIN_PULLUP_EN(io_reg);
			}
			else
			{
				PIN_PULLUP_DIS(io_reg);
			}

			if (pGPIOConfig->GPIO_Mode == GPIO_Mode_Out_OD)
			{
				//				portENTER_CRITICAL();

				pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR(io_num));
//				pin_reg &= (~GPIO_PIN_DRIVER);
				pin_reg |= (GPIO_PIN_DRIVER);
				GPIO_REG_WRITE(GPIO_PIN_ADDR(io_num), pin_reg);

				//				portEXIT_CRITICAL();
			}
			else if (pGPIOConfig->GPIO_Mode == GPIO_Mode_Sigma_Delta)
			{
				//				portENTER_CRITICAL();

				pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR(io_num));
				pin_reg &= (~GPIO_OUT_DATA_MASK);
				pin_reg |= (0x1 << GPIO_PIN_SOURCE_LSB);
				GPIO_REG_WRITE(GPIO_PIN_ADDR(io_num), pin_reg);
				GPIO->sigma_delta_bits.enable = 1;
//				GPIO_REG_WRITE(GPIO_SIGMA_DELTA_ADDRESS, SIGMA_DELTA_ENABLE);

				//				portEXIT_CRITICAL();
			}
			gpio_pin_intr_state_set (io_num, pGPIOConfig->GPIO_IntrType);
		}
		io_num++;
	}
	while (io_num < 16);
}

/*
 * Change GPIO pin output by setting, clearing, or disabling pins.
 * In general, it is expected that a bit will be set in at most one
 * of these masks.  If a bit is clear in all masks, the output state
 * remains unchanged.
 *
 * There is no particular ordering guaranteed; so if the order of
 * writes is significant, calling code should divide a single call
 * into multiple calls.
 */
void ICACHE_FLASH_ATTR gpio_output_conf (uint32 set_mask, uint32 clear_mask,
        uint32 enable_mask, uint32 disable_mask)
{
//	GPIO_REG_WRITE (GPIO_OUT_W1TS_ADDRESS, set_mask);
	GPIO->out_w1ts = set_mask;
//	GPIO_REG_WRITE (GPIO_OUT_W1TC_ADDRESS, clear_mask);
	GPIO->out_w1tc = clear_mask;
//	GPIO_REG_WRITE (GPIO_ENABLE_W1TS_ADDRESS, enable_mask);
	GPIO->enable_w1ts = enable_mask;
//	GPIO_REG_WRITE (GPIO_ENABLE_W1TC_ADDRESS, disable_mask);
	GPIO->enable_w1tc = disable_mask;
}

/*
 * Sample the value of GPIO input pins and returns a bitmask.
 */
uint32 ICACHE_FLASH_ATTR gpio_input_get (void)
{
//	return GPIO_REG_READ (GPIO_IN_ADDRESS);
	return GPIO->in;
}

/*
 * Register an application-specific interrupt handler for GPIO pin
 * interrupts.  Once the interrupt handler is called, it will not
 * be called again until after a call to gpio_intr_ack.  Any GPIO
 * interrupts that occur during the interim are masked.
 *
 * The application-specific handler is called with a mask of
 * pending GPIO interrupts.  After processing pin interrupts, the
 * application-specific handler may wish to use gpio_intr_pending
 * to check for any additional pending interrupts before it returns.
 */
void ICACHE_FLASH_ATTR gpio_intr_handler_register (void *fn)
{
	_xt_isr_attach (ETS_GPIO_INUM, fn, NULL);
}

/*
 only highlevel and lowlevel intr can use for wakeup
 */
void ICACHE_FLASH_ATTR gpio_pin_wakeup_enable (uint32 i, gpio_pin_int_t intr_state)
{
	uint32 pin_reg;
	volatile gpio_pin_t *p_pin = &GPIO->pin_0_bits + sizeof(gpio_pin_t) * i;

	//	if ((intr_state == GPIO_PIN_INTR_LOLEVEL) || (intr_state == GPIO_PIN_INTR_HILEVEL))
	if ( (intr_state == GPIO_INT_LOW_LEVEL) || (intr_state == GPIO_INT_HIGH_LEVEL))
	{
		//		portENTER_CRITICAL();
		p_pin->pin_int = intr_state;
/*		pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR (i));
		pin_reg &= (~GPIO_PIN_INT_TYPE_MASK);
		pin_reg |= (intr_state << GPIO_PIN_INT_TYPE_LSB);
		pin_reg |= GPIO_PIN_WAKEUP_ENABLE_SET(GPIO_WAKEUP_ENABLE);
		GPIO_REG_WRITE(GPIO_PIN_ADDR(i), pin_reg);
*/
		//		portEXIT_CRITICAL();
	}
}

void ICACHE_FLASH_ATTR gpio_pin_wakeup_disable (void)
{
	uint8 i;
	volatile gpio_pin_t *pin_reg_p = &GPIO->pin_0_bits;

	for (i = 0; i < GPIO_PIN_COUNT; i++)
	{
		pin_reg_p->wakeup_enable = 0;
		pin_reg_p++;
	}
}

void ICACHE_FLASH_ATTR gpio_pin_intr_state_set (uint32 i, gpio_pin_int_t intr_state)
{
	//	portENTER_CRITICAL();
	volatile gpio_pin_t	*pin = &GPIO->pin_0_bits + (sizeof (gpio_pin_t) * i);
	pin->pin_int = intr_state;
//	pin->.pin_int = intr_state;
	//	portEXIT_CRITICAL();
}

void ICACHE_FLASH_ATTR gpio16_output_conf (void)
{
	RTC->gpio3_conf_bits.fun_bit0 = 1; // gpio16 mux configuration for XPD_DCDC to output rtc_gpio0
	RTC->gpio_conf_bits.gpio0_mux = 0; // gpio16 mux configuration for out enable
	RTC->gpio_enable_bits.gpio0_en = 1; // gpio16 out enable
}

void ICACHE_FLASH_ATTR gpio16_output_set (uint8 value)
{
	RTC->gpio_out_bits.gpio0 = value;
}

void ICACHE_FLASH_ATTR gpio16_input_conf (void)
{
//	WRITE_PERI_REG(PAD_XPD_DCDC_CONF,
//	        (READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbc) | (uint32)0x1);
	RTC->gpio3_conf_bits.pullup = 0;
	RTC->gpio3_conf_bits.pulldown = 0;
	RTC->gpio3_conf_bits.fun_bit0 = 1; // mux configuration for XPD_DCDC and rtc_gpio0 connection
//	WRITE_PERI_REG(RTC_GPIO_CONF,
//	        (READ_PERI_REG(RTC_GPIO_CONF) & (uint32)0xfffffffe) | (uint32)0x0);
	RTC->gpio_conf_bits.gpio0_mux = 0; //mux configuration for out enable
//	WRITE_PERI_REG(RTC_GPIO_ENABLE,
//	        READ_PERI_REG(RTC_GPIO_ENABLE) & (uint32)0xfffffffe); 
	RTC->gpio_enable_bits.gpio0_en = 0;	//out disable
}

uint8 ICACHE_FLASH_ATTR gpio16_input_get (void)
{
//	return (uint8) (READ_PERI_REG(RTC_GPIO_IN_DATA) & 1);
	return (u8) RTC->gpio_in_bits.gpio0;
}

