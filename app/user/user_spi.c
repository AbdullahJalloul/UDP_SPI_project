/*
 * user_spi.c
 *
 *  Created on: 29.05.2016
 *      Author: Ilya Petrukhin
 */

#include	<c_types.h>
#include	<gpio.h>
#include	<user_interface.h>
#include	<ESP8266_registers.h>
#include	"global.h"
#include	"user_spi.h"


static void	gpio5_setup			(void);
static void	pin_int_handler		(void *para);
static void	spi_int_handler		(void *para);
void		print_udata			(u8 *buf, u32 len);
void		write_udata			(void);
void		read_udata			(void);
void		print_spi_reg		(void);
void		spi_reg_mod			(void);

u8			tcp_compare			(u8 *x, u8 *y);
u8			port_compare		(int x, int y);
u8			len_compare			(unsigned short len);

/*******************************************************************************
* Обмен данными по SPI1 в slave режиме
* Подключение:
* GPIO12 - данные MISO и MOSI
* GPIO13 - REQ GPIO OUT флаг запроса от ESP8266 на передачу данных
* GPIO14 - CLK
* GPIO15 - CS
* GPIO4  - CS GPIO IN для разрешения конфликтов
* Для запуска теста служит GPIO5 и кнопка
*******************************************************************************/
void ICACHE_FLASH_ATTR spi_init (void)
{
	// gpio mux
	IOMUX->gpio_mux_cfg_bits.spi1_sys_clk = 0;
	IOMUX->gpio12_mux_spi1_miso &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio12_mux_spi1_miso |= GPIO12_FUNC_SPI1_Q_MISO;	// MISO
	IOMUX->gpio14_mux_spi1_clk &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio14_mux_spi1_clk |= GPIO14_FUNC_SPI1_CLK;		// CLK
	IOMUX->gpio15_mux_spi1_cs &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio15_mux_spi1_cs |= GPIO15_FUNC_SPI1_CS;		// CS
	IOMUX->gpio13_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio13_mux |= GPIO13_FUNC_GPIO;			// GPIO13
	GPIO->pin_13_bits.gpio_source = 0;
	GPIO->pin_13_bits.pin_driver = 0;				// 0-PP
	GPIO->pin_13_bits.pin_int = 0;	//GPIO_PIN_INTR_DISABLE = 0 нет прерываний
	GPIO->enable_w1ts = GPIO_ENABLE_W1TS_PIN_13;	// enable OUT GPIO13
	GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_13;			// GPIO13 = 0;
	
	IOMUX->gpio4_mux &= ~GPIO_MUX_FUNC_MASK;
	IOMUX->gpio4_mux |= GPIO4_FUNC_GPIO;			// GPIO4
	GPIO->pin_4_bits.gpio_source = GPIO_DATA;
	GPIO->pin_4_bits.pin_driver = PUSH_PULL;		// 0-PP
	GPIO->pin_4_bits.pin_int = 0;	// GPIO_PIN_INTR_DISABLE = 0 нет прерываний
	GPIO->enable_w1tc_bits.pin_4 = 1;				// Disable OUT: GPIO4-IN
	
	// SPI_CLOCK_10MHZ
//	SPI1->clock = 0;	// slave
	SPI1->clock_bits.pre_s = 0;
	SPI1->clock_bits.clkcnt_n = 7;
	SPI1->clock_bits.clkcnt_h = 3;
	SPI1->clock_bits.clkcnt_l = 7;
	SPI1->clock_bits.equ_sysclk = 0;
	
	SPI1->ctrl_bits.rd_bit_order = 1;	// LSB first
	SPI1->ctrl_bits.wr_bit_order = 1;
	SPI1->ctrl1 = 0;

	// SPI format and length
	SPI1->user = SPI_USER_MOSI;	// в slave режиме это приём!!!
/*	SPI1->user_bits.dout_din = 0;
	SPI1->user_bits.cs_setup = 0;
	SPI1->user_bits.cs_hold = 0;
	SPI1->user_bits.command = 0;
	SPI1->user_bits.flash_mode = 0;	//disable flash operation mode
	SPI1->user_bits.mosi = 0; 
	SPI1->user_bits.miso = 1;
	SPI1->user_bits.ck_in_edge = 1;
	SPI1->user_bits.ck_out_edge = 1;
	SPI1->user_bits.wr_byte_order = 0;	//порядок байт в слове
*/	
	SPI1->user1_bits.addr_bitlen = 0;
	SPI1->user1_bits.dummy_cyclelen = 0;
	SPI1->user1_bits.miso_bitlen = 511;
	SPI1->user1_bits.mosi_bitlen = 511;
	
	SPI1->slave1_bits.buf_bitlen = 0;	//511;
	SPI1->slave1_bits.status_bitlen = 0;
	SPI1->slave1_bits.wr_addr_bitlen = 0;
	SPI1->slave1_bits.rd_addr_bitlen = 0;
	SPI1->slave1_bits.status_readback = 0;	// CHOOSE ACTIVE STATUS REG ???

	SPI1->pin = SPI_PIN_MOSI_MISO | SPI_PIN_CLK1_CS0;	//
//	SPI1->pin_bits.clk1_cs0 = 1;	// BIT19 ???
//	SPI1->pin_bits.mosi_miso = 1;	// режим приёма
	
	// Disable the interrupt
	ets_isr_mask ((1 << ETS_SPI_INUM));	
	// стереть флаги прерываний SPI0
	SPI0->slave &= ~(SPI_SLAVE_INT_STT_MASK | SPI_SLAVE_INT_EN_MASK);
	// стереть флаги прерываний SPI1
	SPI1->slave = SPI_SLAVE_INT_EN_MASK 	// all interrupts
				| SPI_SLAVE_SLAVE_MODE; 
//				| SPI_SLAVE_WR_RD_BUF_EN;
	
	ets_isr_attach (ETS_SPI_INUM, (spi_int_handler), (void *)(NULL));	//register level2 isr function, which contains spi, hspi and i2s events
	ets_isr_unmask ((1 << ETS_SPI_INUM));	//enable level2 isr, which contains spi, hspi and i2s events
	SPI1->cmd = SPI_CMD_USR;				// enable slave transmission listen 

	SPI1->ctrl2_bits.mosi_delay_mode = 1;
	SPI1->ctrl2_bits.miso_delay_mode = 0;
	
	g.need_transmit = 0;	// сбросим флаг для ошибки #1
	g.spi_transmit_mode = 0;	// приём
	gpio5_setup ();
	set_udata ();
}

/*******************************************************************************
* инициализация прерывания, имитирующего приём данных по UDP
*******************************************************************************/
static void ICACHE_FLASH_ATTR gpio5_setup (void)
{
	ets_isr_mask ((1 << ETS_GPIO_INUM));
		
	IOMUX->gpio5_mux &= ~GPIO_MUX_FUNC_MASK;	// 
	IOMUX->gpio5_mux |= GPIO5_FUNC_GPIO;		// 
	IOMUX->gpio5_mux_bits.pullup = 1;
	
	GPIO->enable_w1tc_bits.pin_5 = 1;			// GPIO4 input
	
	GPIO->pin_5_bits.gpio_source = GPIO_DATA;
	GPIO->pin_5_bits.pin_driver = PUSH_PULL;
	GPIO->pin_5_bits.pin_int = 2;	// GPIO_PIN_INTR_NEGEDGE = 2 прерывание по спаду
	GPIO->pin_5_bits.config = 0;	// ???
	
	ets_isr_attach (ETS_GPIO_INUM, pin_int_handler, (void *)NULL);
	GPIO->int_st_w1tc = GPIO_INT_STATUS_MASK;	// стереть маски прерываний по пинам
	ets_isr_unmask ((1 << ETS_GPIO_INUM));
}


/*******************************************************************************
* тестовое прерывание, имитирующее приём данных по UDP
*******************************************************************************/
static void __attribute__((optimize("O2"))) pin_int_handler (void *para)
{
	gpio_int_st_t	status;
	status = GPIO->int_st_bits;
	GPIO->int_st_w1tc = GPIO->int_st;	// стёрли все флаги прерывания по пинам
	
	if (status.pin_5)	// прерывание по пину 5 - аналог запроса на передачу данных при приёме UDP
	{	
		write_udata ();					// записать данные в буфер
		if (GPIO->in_bits.pin_4)		// первая проверка на высокий уровень CS
		{
			GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_13;	// REQ = 1
			// подождать 100 нс для проверки на состояние мастера
			__asm__ __volatile__ ("nop");	// 1/80 MHZ = 12.5 ns
			__asm__ __volatile__ ("nop");
			__asm__ __volatile__ ("nop");
			__asm__ __volatile__ ("nop");
			__asm__ __volatile__ ("nop");
			__asm__ __volatile__ ("nop");
			__asm__ __volatile__ ("nop");
			// проверить CS заново
			if (GPIO->in_bits.pin_4)	// реакции нет - передача разрешена
			{
				g.spi_transmit_mode = 1;		// режим передачи
				g.need_transmit = 0;
				SPI1->user = SPI_USER_MISO;		// в slave режиме это передача!!!
				SPI1->pin = SPI_PIN_CLK1_CS0;	// режим передачи
				SPI1->cmd = SPI_CMD_USR;		// transmission start
				// дальше работаем в прерывании spi по чтению буфера
			}
			else	// конфликт №3: мастер передаёт данные одновременно со слейвом
			{
				g.need_transmit = 1;	// установим флаг для запуска отложенной передачи
			}	
			GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_13;	// REQ = 0
		}
		else	// конфликт №1: запрос на передачу во время приёма
		{
			g.need_transmit = 1;	// установим флаг для запуска отложенной передачи
		}
	}
}


/*******************************************************************************
* функция, вызываемая после приёма данных по UDP
* 
*******************************************************************************/
void __attribute__((optimize("O2"))) user_udp_receive_cb (void *arg, char *pdata, unsigned short len)
{
	struct espconn	*parg = arg;
	u32 *data = (u32 *)pdata;
	u8 num_err = 0;
	if (g.state == U_STATE_TRANSMIT)
	{
		// сравнить IP
		num_err += tcp_compare (parg->proto.udp->remote_ip, g.user_udp_struct.proto.udp->remote_ip);
		// сравнить порт
		num_err += port_compare (parg->proto.udp->remote_port, g.user_udp_struct.proto.udp->remote_port);
		// сравнить длину посылки == 64 байта
		num_err += len_compare (len);	
		
		if (num_err == 0)
		{			
			if (GPIO->in_bits.pin_4)		// первая проверка на высокий уровень CS
			{
				GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_13;	// REQ = 1
				// подождать 100 нс для проверки на состояние мастера
				
				__asm__ __volatile__ ("nop");	// 1/80 MHZ = 12.5 ns
				__asm__ __volatile__ ("nop");
				__asm__ __volatile__ ("nop");
				__asm__ __volatile__ ("nop");
				__asm__ __volatile__ ("nop");
				__asm__ __volatile__ ("nop");
				__asm__ __volatile__ ("nop");
				
				// проверить CS заново
				if (GPIO->in_bits.pin_4)	// реакции нет - передача разрешена
				{
					SPI1->w0 = *data++;			// записать данные в буфер SPI
					SPI1->w1 = *data++;
					SPI1->w2 = *data++;
					SPI1->w3 = *data++;
					SPI1->w4 = *data++;
					SPI1->w5 = *data++;
					SPI1->w6 = *data++;
					SPI1->w7 = *data++;
					SPI1->w8 = *data++;
					SPI1->w9 = *data++;
					SPI1->w10 = *data++;
					SPI1->w11 = *data++;
					SPI1->w12 = *data++;
					SPI1->w13 = *data++;
					SPI1->w14 = *data++;
					SPI1->w15 = *data;

					g.spi_transmit_mode = 1;		// режим передачи
					g.need_transmit = 0;
					SPI1->user = SPI_USER_MISO;		// в slave режиме это передача!!!
					SPI1->pin = SPI_PIN_CLK1_CS0;	// режим передачи
					SPI1->cmd = SPI_CMD_USR;		// transmission start
					// дальше работаем в прерывании spi по чтению буфера
				}
				else	// конфликт №3: мастер передаёт данные одновременно со слейвом
				{
					g.transmit_buff = (u32*)data;
					g.need_transmit = 1;	// установим флаг для запуска отложенной передачи
				}	
				GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_13;	// REQ = 0
			}
			else	// конфликт №1: запрос на передачу во время приёма
			{
				g.transmit_buff = (u32*)data;
				g.need_transmit = 1;	// установим флаг для запуска отложенной передачи
			}
		}
		else 
		{
			os_printf ("Error in receive UDP");
		}
	}
}

inline u8 ICACHE_FLASH_ATTR tcp_compare (u8 *x, u8 *y)
{
	if (*x++ == *y++)
	{
		if (*x++ == *y++)
		{
			if (*x++ == *y++)
			{
				if (*x == *y)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

inline u8 ICACHE_FLASH_ATTR port_compare (int x, int y)
{
	if (x == y)
	{
		return 0;
	}
	return 1;
}


inline u8 ICACHE_FLASH_ATTR len_compare (unsigned short len)
{
	if (len == (SIZE_BUFFERS * sizeof (b.receive[0])))
	{
		return 0;
	}
	return 1;
}

/*******************************************************************************
* Slave режим
* Прерывание после передачи 16 слов данных SPI1
* Может вызываться в двух режимах: приёма и передачи данных
* В режиме передачи выставляется мютекс spi
* и данные пересылаются из буфера передачи.
* 	По окончании передачи анализируется запрос на приём
* 	и при его наличии запускается приём пакета SPI
* В режиме приёма выставляется мютекс spi
* и данные пересылаются в буфер приёма.
* 	По окончании запускается процесс передачи wifi
* 	и при наличии запроса на передачу запускается процесс передачи SPI
*******************************************************************************/
static void __attribute__((optimize("O2"))) spi_int_handler (void *para)
{
	dport_isr_flag_t	dport_value = DPORT->isr_flag_bits;
	volatile u32		*buffer;
	__IO u32			*spi_reg;
	
	// прерывание от spi0
	if (dport_value.spi0_isr)
	{
		// стереть флаги разрешения прерывания и статуса прерывания
		SPI0->slave &= ~(SPI_SLAVE_INT_STT_MASK | SPI_SLAVE_INT_EN_MASK);
	}
	// прерывание от spi1
	if (dport_value.spi1_isr)
	{
		SPI1->slave &= ~(SPI_SLAVE_INT_STT_MASK);	// сотрём флаги прерываний
		if (!g.spi_transmit_mode)	// режим приёма
		{
			buffer = b.receive;
			spi_reg = &SPI1->w0;
			*buffer++ = *spi_reg++;	// записать данные из SPI в буфер b.receive
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer++ = *spi_reg++;
			*buffer = *spi_reg;

			// запустить передачу UDP с полученным массивом
			if (g.state == U_STATE_TRANSMIT)
			{
				//espconn_udp_sent (&g.user_udp_struct, b.receive, (SIZE_BUFFERS * sizeof (b.receive[0])));
				// это низкоуровневая функция - на потом
				//udp_sendto ();	// struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *dst_ip, u16_t dst_port
				espconn_send (&g.user_udp_struct, (u8 *)b.receive, 64);	// struct espconn *espconn, uint8 *psent, uint16 length
			}
			
			if (g.need_transmit)	// запуск отложенной передачи
			{
				GPIO->out_w1ts = GPIO_OUT_W1TS_PIN_13;	// REQ = 1
				// подождать 100 нс для проверки на состояние мастера - сейчас с помощью записи данных
				
				spi_reg = &SPI1->w0;	// запись в регистры данных из буфера
				buffer = b.receive;
				*spi_reg++ = *buffer++;	// как влияет запись на регистр чтения?
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg++ = *buffer++;
				*spi_reg = *buffer;
				
				// проверить CS
				if (GPIO->in_bits.pin_4)		// реакции CS на REQ нет - передача разрешена
				{
					g.spi_transmit_mode = 1;	// режим передачи
					g.need_transmit = 0;
					SPI1->user = SPI_USER_MISO;		// в slave режиме это передача!!!
					SPI1->pin = SPI_PIN_CLK1_CS0;	// режим передачи
//					SPI1->slave_bits.sync_reset = 1;
					SPI1->cmd = SPI_CMD_USR;		// transmission start
					// дальше работаем в прерывании spi по чтению буфера
				}
				else	// конфликт №3: мастер передаёт данные одновременно со слейвом
				{
					g.need_transmit = 1;	// установим флаг для запуска отложенной передачи
				}			
				GPIO->out_w1tc = GPIO_OUT_W1TC_PIN_13;	// REQ = 0
			}
		}
		else	// режим передачи
		{
			SPI1->user = SPI_USER_MOSI;		// в slave режиме это приём!!!
			SPI1->pin = SPI_PIN_MOSI_MISO | SPI_PIN_CLK1_CS0;	// режим приёма
//			SPI1->slave_bits.sync_reset = 1;
			SPI1->cmd = SPI_CMD_USR;		// ready to slave read
			g.spi_transmit_mode = 0;
			// передали - свободны
		}
	}
	if (dport_value.i2s_isr)
	{
		// прерывание от i2s
	}
}

/*******************************************************************************
* Тестовые данные в буфер
*******************************************************************************/
void ICACHE_FLASH_ATTR set_udata (void)
{
	volatile u32 *p_buf = b.send; 
	*p_buf++ = 0x05040302;
	*p_buf++ = 0x09080706;
	*p_buf++ = 0x0d0c0b0a;
	*p_buf++ = 0x11100f0e;
	*p_buf++ = 0x15141312;
	*p_buf++ = 0x19181716;
	*p_buf++ = 0x1d1c1b1a;
	*p_buf++ = 0x21201f1e;
	*p_buf++ = 0x25242322;
	*p_buf++ = 0x29282726;
	*p_buf++ = 0x2d2c2b2a;
	*p_buf++ = 0x31302f2e;
	*p_buf++ = 0x35343332;
	*p_buf++ = 0x39383736;
	*p_buf++ = 0x3d3c3b3a;
	*p_buf++ = 0x41404f4e;
}

void write_udata (void)
{
	SPI1->w0 = b.receive[0];
	SPI1->w1 = b.receive[1];
	SPI1->w2 = b.receive[2];
	SPI1->w3 = b.receive[3];
	SPI1->w4 = b.receive[4];
	SPI1->w5 = b.receive[5];
	SPI1->w6 = b.receive[6];
	SPI1->w7 = b.receive[7];
	SPI1->w8 = b.receive[8];
	SPI1->w9 = b.receive[9];
	SPI1->w10 = b.receive[10];
	SPI1->w11 = b.receive[11];
	SPI1->w12 = b.receive[12];
	SPI1->w13 = b.receive[13];
	SPI1->w14 = b.receive[14];
	SPI1->w15 = b.receive[15];
}

inline void read_udata (void)
{
	b.receive[0] = SPI1->w0;
	b.receive[1] = SPI1->w1;
	b.receive[2] = SPI1->w2;
	b.receive[3] = SPI1->w3;
	b.receive[4] = SPI1->w4;
	b.receive[5] = SPI1->w5;
	b.receive[6] = SPI1->w6;
	b.receive[7] = SPI1->w7;
	b.receive[8] = SPI1->w8;
	b.receive[9] = SPI1->w9;
	b.receive[10] = SPI1->w10;
	b.receive[11] = SPI1->w11;
	b.receive[12] = SPI1->w12;
	b.receive[13] = SPI1->w13;
	b.receive[14] = SPI1->w14;
	b.receive[15] = SPI1->w15;
	
//	print_udata ((u8 *)b.receive[0], 64);
//	print_spi_reg ();
}

void ICACHE_FLASH_ATTR print_udata (u8 *buf, u32 len)
{
	u8 addr = 0;
	while (len)
	{
		ets_uart_printf ("Receive address = %X, receive data = %X;\n\r", addr, *buf);
		addr++;
		buf++;
		len--;
	}
}

void ICACHE_FLASH_ATTR print_spi_reg (void)
{
	ets_uart_printf ("SPI1 cmd = %x\n\r", SPI1->cmd);
	ets_uart_printf ("SPI1 addr = %x\n\r", SPI1->addr);
	ets_uart_printf ("SPI1 ctrl = %x\n\r", SPI1->ctrl);
	ets_uart_printf ("SPI1 ctrl1 = %x\n\r", SPI1->ctrl1);
	ets_uart_printf ("SPI1 rd_status = %x\n\r", SPI1->rd_status);
	ets_uart_printf ("SPI1 ctrl2 = %x\n\r", SPI1->ctrl2);
	ets_uart_printf ("SPI1 clock = %x\n\r", SPI1->clock);
	ets_uart_printf ("SPI1 user = %x\n\r", SPI1->user);
	ets_uart_printf ("SPI1 user1 = %x\n\r", SPI1->user1);
	ets_uart_printf ("SPI1 user2 = %x\n\r", SPI1->user2);
	ets_uart_printf ("SPI1 wr_status = %x\n\r", SPI1->wr_status);
	ets_uart_printf ("SPI1 pin = %x\n\r", SPI1->pin);
	ets_uart_printf ("SPI1 slave = %x\n\r", SPI1->slave);
	ets_uart_printf ("SPI1 slave1 = %x\n\r", SPI1->slave1);
	ets_uart_printf ("SPI1 slave2 = %x\n\r", SPI1->slave2);
	ets_uart_printf ("SPI1 slave3 = %x\n\r", SPI1->slave3);
	ets_uart_printf ("SPI1 ext0 = %x\n\r", SPI1->ext0);
	ets_uart_printf ("SPI1 ext1 = %x\n\r", SPI1->ext1);
	ets_uart_printf ("SPI1 ext2 = %x\n\r", SPI1->ext2);
	ets_uart_printf ("SPI1 ext3 = %x\n\r", SPI1->ext3);
}

void ICACHE_FLASH_ATTR spi_reg_mod (void)
{
	// будем менять SPI->user
	static u32 reg = 0x00010000;
	volatile static u32 *adr = &SPI1->ctrl2;
	static u32 savereg;
	static u32 status = 0;
	
	switch (status)
	{
	case 0:	// запомнить регистр
		savereg = *adr;
		ets_uart_printf ("Adr = %X, bit = %X\n\r", adr, reg);
		status = 1;
		break;
		
	case 1:	// установить бит
		*adr |= reg;
		status = 2;
		break;
		
	case 2:	// обнулить бит
		*adr &= ~reg;
		status = 3;
		break;
		
	case 3:	// вернуть бит на место
		*adr = savereg;
		reg <<= 1;
		if (reg == 0x00000000)
		{
			reg = 0x00000001;
			adr++;
		}
		status = 0;
		break;
		
	default:
		status = 0;
		break;
	}
	
	
}

	

