/*
 ESPRSSIF MIT License

 Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>

 Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case, it is free of charge, to any person obtaining a copy of this software and associated documentation files (the ��Software��), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED ��AS IS��, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include	"ets_sys.h"
#include	"osapi.h"
//#include	"gpio16.h"
#include	"ESP8266_registers.h"

void ICACHE_FLASH_ATTR gpio16_output_conf (void)
{
	RTC->gpio3_conf_bits.fun_bit0 = 1; // mux configuration for XPD_DCDC to output rtc_gpio0
	RTC->gpio3_conf_bits.fun_bit1 = 0;
	RTC->gpio3_conf_bits.fun_bit2 = 0;
//	WRITE_PERI_REG(PAD_XPD_DCDC_CONF,
//	        (READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbc) | (uint32)0x1); // mux configuration for XPD_DCDC to output rtc_gpio0
	RTC->gpio_conf_bits.gpio0_mux = 0;	//mux configuration for out enable
//	WRITE_PERI_REG(RTC_GPIO_CONF,
//	        (READ_PERI_REG(RTC_GPIO_CONF) & (uint32)0xfffffffe) | (uint32)0x0);	//mux configuration for out enable

	RTC->gpio_enable_bits.gpio0_en = 1;	//out enable
//	WRITE_PERI_REG(RTC_GPIO_ENABLE,
//	        (READ_PERI_REG(RTC_GPIO_ENABLE) & (uint32)0xfffffffe) | (uint32)0x1);//out enable
}

void ICACHE_FLASH_ATTR gpio16_output_set (uint8 value)
{
	RTC->gpio_out_bits.gpio0 = value;
//	WRITE_PERI_REG(RTC_GPIO_OUT,
//	        (READ_PERI_REG(RTC_GPIO_OUT) & (uint32)0xfffffffe) | (uint32)(value & 1));
}

void ICACHE_FLASH_ATTR gpio16_input_conf (void)
{
	RTC->gpio3_conf_bits.fun_bit0 = 1; // mux configuration for XPD_DCDC to output rtc_gpio0
	RTC->gpio3_conf_bits.fun_bit1 = 0;
	RTC->gpio3_conf_bits.fun_bit2 = 0;
//	WRITE_PERI_REG(PAD_XPD_DCDC_CONF,
//	        (READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbc) | (uint32)0x1); // mux configuration for XPD_DCDC and rtc_gpio0 connection

	RTC->gpio_conf_bits.gpio0_mux = 0;	//mux configuration for out enable
//	WRITE_PERI_REG(RTC_GPIO_CONF,
//	        (READ_PERI_REG(RTC_GPIO_CONF) & (uint32)0xfffffffe) | (uint32)0x0);	//mux configuration for out enable

	RTC->gpio_enable_bits.gpio0_en = 0;		//out disable
//	WRITE_PERI_REG(RTC_GPIO_ENABLE,
//	        READ_PERI_REG(RTC_GPIO_ENABLE) & (uint32)0xfffffffe);//out disable
}

uint8 ICACHE_FLASH_ATTR gpio16_input_get (void)
{
	return (uint8)(RTC->gpio_in_bits.gpio0);	// (READ_PERI_REG(RTC_GPIO_IN_DATA) & 1);
}
