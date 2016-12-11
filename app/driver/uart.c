/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: uart.c
 *
 * Description: Two UART mode configration and interrupt handler.
 *              Check your hardware connection while use this mode.
 *
 * Modification history:
 *     2014/3/12, v1.0 create this file.
 *******************************************************************************/
#include	"c_types.h"
#include	"ets_sys.h"
#include	"osapi.h"
#include	"bits.h"
#include	"ESP8266_registers.h"
//#include	"uart_register.h"
#include	"driver/uart.h"

#define UART0   0
#define UART1   1

// UartDev is defined and initialized in rom code.
extern UartDevice UartDev;

static void uart0_rx_intr_handler (void *para);

/******************************************************************************
 * FunctionName : uart_config
 * Description  : Internal used function
 *                UART0 used for data TX/RX, RX buffer size is 0x100, interrupt enabled
 *                UART1 just used for debug output
 * Parameters   : uart_no, use UART0 or UART1 defined ahead
 * Returns      : NONE
 *******************************************************************************/
static void ICACHE_FLASH_ATTR uart_config (uint8 uart_no)
{
	UART_TypeDef *uart_p;
	
	if (uart_no == UART1)
	{
		uart_p = UART_1;
		IOMUX->gpio2_mux &= ~GPIO_MUX_FUNC_MASK;
		IOMUX->gpio2_mux |= GPIO2_FUNC_U1TXD;
	}
	else
	{
		uart_p = UART_0;
		/* rcv_buff size if 0x100 */
		ets_isr_attach (5, uart0_rx_intr_handler, (void *)&UartDev.rcv_buff);
		IOMUX->gpio1_mux_bits.pullup = 0;	

		IOMUX->gpio1_mux_u0_txd &= ~GPIO_MUX_FUNC_MASK;
		IOMUX->gpio1_mux_u0_txd |= GPIO1_FUNC_U0TXD;

		IOMUX->gpio15_mux_mtdo &= ~GPIO_MUX_FUNC_MASK;
		IOMUX->gpio15_mux_mtdo |= GPIO15_FUNC_U0RTS;
	}
	
	uart_div_modify (uart_no, UART_CLK_FREQ / (UartDev.baut_rate));
	
	uart_p->conf0 = (UartDev.exist_parity
	        | UartDev.parity
	        | (UartDev.stop_bits << 4)	//UART_STOP_BIT_NUM_S)
	        | (UartDev.data_bits << 2));	//UART_BIT_NUM_S));
	
	//clear rx and tx fifo,not ready
	uart_p->conf0 |= UART_CONF0_RXFIFO_RST | UART_CONF0_TXFIFO_RST;	
	uart_p->conf0 &= ~(UART_CONF0_RXFIFO_RST | UART_CONF0_TXFIFO_RST);	
	
	if (uart_no == UART0)
	{
		//set rx fifo trigger
		uart_p->conf1_bits.rx_fifo_full_trhd = 0x10;
		uart_p->conf1_bits.rx_flow_trhd = 0x10;
		uart_p->conf1_bits.rx_flow_en = 1;
		uart_p->conf1_bits.rx_tout_trhd = 0x02;
		uart_p->conf1_bits.rx_tout_en = 1;
		
		uart_p->int_ena |= UART_INT_ENA_RXFIFO_TOUT | UART_INT_ENA_FRM_ERR;
	}
	else
	{
		uart_p->conf1_bits.rx_fifo_full_trhd = UartDev.rcv_buff.TrigLvl;
	}
	
	//clear all interrupt
	uart_p->int_clr = UART_INT_CLR_MASK;
	//enable rx_interrupt
	uart_p->int_ena_bits.rx_fifo_full = 1;
}

/******************************************************************************
 * FunctionName : uart1_tx_one_char
 * Description  : Internal used function
 *                Use uart1 interface to transfer one char
 * Parameters   : uint8 TxChar - character to tx
 * Returns      : OK
 *******************************************************************************/
static STATUS ICACHE_FLASH_ATTR uart1_tx_one_char (uint8 TxChar)
{
	while (UART_1->status_bits.tx_fifo_cnt < 126);

	UART_1->fifo = TxChar;
	return OK;
}

/******************************************************************************
 * FunctionName : uart0_tx_one_char
 * Description  : Internal used function
 *                Use uart1 interface to transfer one char
 * Parameters   : uint8 TxChar - character to tx
 * Returns      : OK
 *******************************************************************************/
STATUS uart0_tx_one_char (uint8 TxChar)
{
	while (true)
	{
		uint32 fifo_cnt = UART_0->status_bits.tx_fifo_cnt; 
		if (fifo_cnt < 126)
		{
			break;
		}
	}
	UART_0->fifo = TxChar;
	return OK;
}

/******************************************************************************
 * FunctionName : uart1_write_char
 * Description  : Internal used function
 *                Do some special deal while tx char is '\r' or '\n'
 * Parameters   : char c - character to tx
 * Returns      : NONE
 *******************************************************************************/
static void ICACHE_FLASH_ATTR uart1_write_char (char c)
{
	if (c == '\n')
	{
		uart1_tx_one_char ('\r');
		uart1_tx_one_char ('\n');
	}
	else if (c == '\r')
	{
	}
	else
	{
		uart1_tx_one_char (c);
	}
}

/******************************************************************************
 * FunctionName : uart0_sendStr
 * Description  : use uart0 to transfer buffer
 * Parameters   : uint8 *buf - point to send buffer
 *                uint16 len - buffer len
 * Returns      :
 *******************************************************************************/
/*void ICACHE_FLASH_ATTR uart0_sendStr(const char *str)
{
	while(*str)
	{
		uart_tx_one_char(UART0, *str++);
	}
}

void at_port_print(const char *str) __attribute__((alias("uart0_sendStr")));
 */

/******************************************************************************
 * FunctionName : uart0_rx_intr_handler
 * Description  : Internal used function
 *                UART0 interrupt handler, add self handle code inside
 * Parameters   : void *para - point to ETS_UART_INTR_ATTACH's arg
 * Returns      : NONE
 *******************************************************************************/
static void uart0_rx_intr_handler (void *para)
{
	/* uart0 and uart1 intr combine togther, when interrupt occur, see reg 0x3ff20020, bit2, bit0 represents
	 * uart1 and uart0 respectively
	 */
	RcvMsgBuff	*pRxBuff = (RcvMsgBuff *)para;
	uint8		RcvChar;

	if (UART_0->int_st_bits.rx_fifo_full == 0)
	{
		return;
	}

	UART_0->int_clr_bits.rx_fifo_full = 1;
//	WRITE_PERI_REG(UART_INT_CLR (UART0), UART_RXFIFO_FULL_INT_CLR);

	while (UART_0->status_bits.rx_fifo_cnt)
	{
		RcvChar = (u8)UART_0->fifo;
//		READ_PERI_REG (UART_FIFO (UART0)) & 0xFF;

		/* you can add your handle code below.*/
		*(pRxBuff->pWritePos) = RcvChar;

		// insert here for get one command line from uart
		if (RcvChar == '\r')
		{
			pRxBuff->BuffState = WRITE_OVER;
		}

		pRxBuff->pWritePos++;

		if (pRxBuff->pWritePos == (pRxBuff->pRcvMsgBuff + RX_BUFF_SIZE))
		{
			// overflow ...we may need more error handle here.
			pRxBuff->pWritePos = pRxBuff->pRcvMsgBuff;
		}
	}
}

/******************************************************************************
 * FunctionName : uart0_tx_buffer
 * Description  : use uart0 to transfer buffer
 * Parameters   : uint8 *buf - point to send buffer
 *                uint16 len - buffer len
 * Returns      :
 *******************************************************************************/
void ICACHE_FLASH_ATTR uart0_tx_buffer (uint8 *buf, uint16 len)
{
	while (len)
	{
		uart_tx_one_char (*buf);
		buf++;
		len--;
	}
}

/******************************************************************************
 * FunctionName : uart_init
 * Description  : user interface for init uart
 * Parameters   : UartBautRate uart0_br - uart0 bautrate
 *                UartBautRate uart1_br - uart1 bautrate
 * Returns      : NONE
 *******************************************************************************/
void ICACHE_FLASH_ATTR uart_init (UartBautRate uart0_br, UartBautRate uart1_br)
{
	// rom use 74880 baut_rate, here reinitialize
	UartDev.baut_rate = uart0_br;
	uart_config (UART0);
	UartDev.baut_rate = uart1_br;
	uart_config (UART1);
	ETS_UART_INTR_ENABLE();

	// install uart1 putc callback
	os_install_putc1 ((void *)uart1_write_char);
}


//=============================================================================
// uart_wait_tx_fifo_empty()
//-----------------------------------------------------------------------------
void ICACHE_FLASH_ATTR uart_wait_tx_fifo_empty (void)
{
	while (UART_0->status_bits.tx_fifo_cnt);
	while (UART_1->status_bits.tx_fifo_cnt);
}
//=============================================================================
// user_uart_wait_tx_fifo_empty()
// Use SDK Espressif
//-----------------------------------------------------------------------------
void ICACHE_FLASH_ATTR user_uart_wait_tx_fifo_empty(uint32 uart_num, uint32 x)
{
	if (uart_num)
	{
		while (UART_1->status_bits.tx_fifo_cnt);
	}
	else 
	{
		while (UART_0->status_bits.tx_fifo_cnt && (x--));
	}
}

