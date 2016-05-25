/*
 *  Copyright (C) 2014 -2016  Espressif System
 *
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include "c_types.h"
#include "ets_sys.h"
#include "gpio_registers.h"


//#define PERIPHS_IO_MUX_FUNC             0x13
//define PERIPHS_IO_MUX_FUNC_S           4
#define PERIPHS_IO_MUX_PULLUP           BIT7
#define PERIPHS_IO_MUX_PULLUP2          BIT6
#define PERIPHS_IO_MUX_SLEEP_PULLUP     BIT3
#define PERIPHS_IO_MUX_SLEEP_PULLUP2    BIT2
#define PERIPHS_IO_MUX_SLEEP_OE         BIT1
#define PERIPHS_IO_MUX_OE               BIT0

#define PERIPHS_IO_MUX_CONF_U           ((volatile uint32 *)0x60000800)
#define SPI0_CLK_EQU_SYS_CLK            BIT8
#define SPI1_CLK_EQU_SYS_CLK            BIT9
#define PERIPHS_IO_MUX_MTDI_U           ((volatile uint32 *)0x60000804)
#define FUNC_GPIO12                     3
#define PERIPHS_IO_MUX_MTCK_U           ((volatile uint32 *)0x60000808)
#define FUNC_GPIO13                     3
#define PERIPHS_IO_MUX_MTMS_U           ((volatile uint32 *)0x6000080C)
#define FUNC_GPIO14                     3
#define PERIPHS_IO_MUX_MTDO_U           ((volatile uint32 *)0x60000810)
#define FUNC_GPIO15                     3
#define FUNC_U0RTS                      4
#define PERIPHS_IO_MUX_U0RXD_U          ((volatile uint32 *)0x60000814)
#define FUNC_GPIO3                      3
#define PERIPHS_IO_MUX_U0TXD_U          ((volatile uint32 *)0x60000818)
#define FUNC_U0TXD                      0
#define FUNC_GPIO1                      3
#define PERIPHS_IO_MUX_SD_CLK_U         ((volatile uint32 *)0x6000081c)
#define FUNC_SDCLK                      0
#define FUNC_SPICLK                     1
#define PERIPHS_IO_MUX_SD_DATA0_U       ((volatile uint32 *)0x60000820)
#define FUNC_SDDATA0                    0
#define FUNC_SPIQ                       1
#define FUNC_U1TXD                      4
#define PERIPHS_IO_MUX_SD_DATA1_U       ((volatile uint32 *)0x60000824)
#define FUNC_SDDATA1                    0
#define FUNC_SPID                       1
#define FUNC_U1RXD                      4
#define FUNC_SDDATA1_U1RXD              7
#define PERIPHS_IO_MUX_SD_DATA2_U       ((volatile uint32 *)0x60000828)
#define FUNC_SDDATA2                    0
#define FUNC_SPIHD                      1
#define FUNC_GPIO9                      3
#define PERIPHS_IO_MUX_SD_DATA3_U       ((volatile uint32 *)0x6000082c)
#define FUNC_SDDATA3                    0
#define FUNC_SPIWP                      1
#define FUNC_GPIO10                     3
#define PERIPHS_IO_MUX_SD_CMD_U         ((volatile uint32 *)0x60000830)
#define FUNC_SDCMD                      0
#define FUNC_SPICS0                     1
#define PERIPHS_IO_MUX_GPIO0_U          ((volatile uint32 *)0x60000834)
#define FUNC_GPIO0                      0
#define PERIPHS_IO_MUX_GPIO2_U          ((volatile uint32 *)0x60000838)
#define FUNC_GPIO2                      0
#define FUNC_U1TXD_BK                   2
#define FUNC_U0TXD_BK                   4
#define PERIPHS_IO_MUX_GPIO4_U          ((volatile uint32 *)0x6000083C)
#define FUNC_GPIO4                      0
#define PERIPHS_IO_MUX_GPIO5_U          ((volatile uint32 *)0x60000840)
#define FUNC_GPIO5                      0

#define PIN_PULLUP_DIS(PIN_NAME)		CLEAR_PERI_REG_MASK (PIN_NAME, PERIPHS_IO_MUX_PULLUP)
#define PIN_PULLUP_EN(PIN_NAME)			SET_PERI_REG_MASK (PIN_NAME, PERIPHS_IO_MUX_PULLUP)


#define GPIO_Pin_0              (BIT(0))  /* Pin 0 selected */
#define GPIO_Pin_1              (BIT(1))  /* Pin 1 selected */
#define GPIO_Pin_2              (BIT(2))  /* Pin 2 selected */
#define GPIO_Pin_3              (BIT(3))  /* Pin 3 selected */
#define GPIO_Pin_4              (BIT(4))  /* Pin 4 selected */
#define GPIO_Pin_5              (BIT(5))  /* Pin 5 selected */
#define GPIO_Pin_6              (BIT(6))  /* Pin 6 selected */
#define GPIO_Pin_7              (BIT(7))  /* Pin 7 selected */
#define GPIO_Pin_8              (BIT(8))  /* Pin 8 selected */
#define GPIO_Pin_9              (BIT(9))  /* Pin 9 selected */
#define GPIO_Pin_10             (BIT(10)) /* Pin 10 selected */
#define GPIO_Pin_11             (BIT(11)) /* Pin 11 selected */
#define GPIO_Pin_12             (BIT(12)) /* Pin 12 selected */
#define GPIO_Pin_13             (BIT(13)) /* Pin 13 selected */
#define GPIO_Pin_14             (BIT(14)) /* Pin 14 selected */
#define GPIO_Pin_15             (BIT(15)) /* Pin 15 selected */
#define GPIO_Pin_All            (0xFFFF)  /* All pins selected */

#define GPIO_PIN_REG_0          PERIPHS_IO_MUX_GPIO0_U
#define GPIO_PIN_REG_1          PERIPHS_IO_MUX_U0TXD_U
#define GPIO_PIN_REG_2          PERIPHS_IO_MUX_GPIO2_U
#define GPIO_PIN_REG_3          PERIPHS_IO_MUX_U0RXD_U
#define GPIO_PIN_REG_4          PERIPHS_IO_MUX_GPIO4_U
#define GPIO_PIN_REG_5          PERIPHS_IO_MUX_GPIO5_U
#define GPIO_PIN_REG_6          PERIPHS_IO_MUX_SD_CLK_U
#define GPIO_PIN_REG_7          PERIPHS_IO_MUX_SD_DATA0_U
#define GPIO_PIN_REG_8          PERIPHS_IO_MUX_SD_DATA1_U
#define GPIO_PIN_REG_9          PERIPHS_IO_MUX_SD_DATA2_U
#define GPIO_PIN_REG_10         PERIPHS_IO_MUX_SD_DATA3_U
#define GPIO_PIN_REG_11         PERIPHS_IO_MUX_SD_CMD_U
#define GPIO_PIN_REG_12         PERIPHS_IO_MUX_MTDI_U
#define GPIO_PIN_REG_13         PERIPHS_IO_MUX_MTCK_U
#define GPIO_PIN_REG_14         PERIPHS_IO_MUX_MTMS_U
#define GPIO_PIN_REG_15         PERIPHS_IO_MUX_MTDO_U

#define GPIO_PIN_REG(i) \
    (i==0) ? GPIO_PIN_REG_0 :  \
    (i==1) ? GPIO_PIN_REG_1 :  \
    (i==2) ? GPIO_PIN_REG_2 :  \
    (i==3) ? GPIO_PIN_REG_3 :  \
    (i==4) ? GPIO_PIN_REG_4 :  \
    (i==5) ? GPIO_PIN_REG_5 :  \
    (i==6) ? GPIO_PIN_REG_6 :  \
    (i==7) ? GPIO_PIN_REG_7 :  \
    (i==8) ? GPIO_PIN_REG_8 :  \
    (i==9) ? GPIO_PIN_REG_9 :  \
    (i==10)? GPIO_PIN_REG_10 : \
    (i==11)? GPIO_PIN_REG_11 : \
    (i==12)? GPIO_PIN_REG_12 : \
    (i==13)? GPIO_PIN_REG_13 : \
    (i==14)? GPIO_PIN_REG_14 : \
    GPIO_PIN_REG_15

#define GPIO_PIN_ADDR(i)        (GPIO_PIN0_ADDRESS + i*4)

#define GPIO_ID_IS_PIN_REGISTER(reg_id) \
    ((reg_id >= GPIO_ID_PIN0) && (reg_id <= GPIO_ID_PIN(GPIO_PIN_COUNT-1)))

#define GPIO_REGID_TO_PINIDX(reg_id) ((reg_id) - GPIO_ID_PIN0)

typedef enum
{
	GPIO_PIN_INTR_DISABLE = 0,
	GPIO_PIN_INTR_POSEDGE = 1,
	GPIO_PIN_INTR_NEGEDGE = 2,
	GPIO_PIN_INTR_ANYEGDE = 3,
	GPIO_PIN_INTR_LOLEVEL = 4,
	GPIO_PIN_INTR_HILEVEL = 5
} GPIO_INT_TYPE;

typedef enum
{
	GPIO_Mode_Input = 0x0,
	GPIO_Mode_Out_OD,
	GPIO_Mode_Output,
	GPIO_Mode_Sigma_Delta,
} GPIOMode_TypeDef;

typedef enum
{
	GPIO_PullUp_DIS = 0x0, GPIO_PullUp_EN = 0x1,
} GPIO_Pullup_IF;

typedef struct
{
	uint16 GPIO_Pin;
	GPIOMode_TypeDef GPIO_Mode;
	GPIO_Pullup_IF GPIO_Pullup;
	GPIO_INT_TYPE GPIO_IntrType;
} GPIO_ConfigTypeDef;

/*#define GPIO_OUTPUT_SET (gpio_no, bit_value)	do {\
	GPIO->out_w1ts = bit_value << gpio_no; \
	GPIO->out_w1tc = ((~bit_value) & 0x01) << gpio_no; \
	GPIO->enable_w1ts = 1 << gpio_no; \
	} while (0)
*/
//    gpio_output_conf (bit_value << gpio_no, ((~bit_value) & 0x01) << gpio_no, 1 << gpio_no, 0)

#define GPIO_OUTPUT(gpio_bits, bit_value) \
    if(bit_value) gpio_output_conf (gpio_bits, 0, gpio_bits, 0);\
    else gpio_output_conf (0, gpio_bits, gpio_bits, 0)

#define GPIO_DIS_OUTPUT(gpio_no)    (GPIO->enable_w1tc = 1<<gpio_no)
#define GPIO_AS_INPUT(gpio_bits)    (GPIO->enable_w1tc = gpio_bits)
#define GPIO_AS_OUTPUT(gpio_bits)   (GPIO->enable_w1ts = gpio_bits)
#define GPIO_INPUT_GET(gpio_no)     ((GPIO->in >> gpio_no) & BIT0)

void gpio_config1		(gpio_config_t *config);
void gpio16_output_conf	(void);
void gpio16_output_set	(uint8 value);
void gpio16_input_conf	(void);
uint8 gpio16_input_get	(void);

void gpio_output_conf (uint32 set_mask, uint32 clear_mask, uint32 enable_mask,
        uint32 disable_mask);
void gpio_intr_handler_register (void *fn);
void gpio_pin_wakeup_enable (uint32 i, gpio_pin_int_t intr_state);
void gpio_pin_wakeup_disable ();
void gpio_pin_intr_state_set (uint32 i, gpio_pin_int_t intr_state);
uint32 gpio_input_get (void);

#endif
