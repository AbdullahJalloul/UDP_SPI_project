/*
 * udp_setup.c
 *
 *  Created on: 18 окт. 2016 г.
 *      Author: Ilya
 */
/*------------------------------------------------------------------------------
 * udp_setup.c
 * Программы управления состоянием модуля в режиме UDP
 *  Created on: 29.05.2016
 *      Author: Ilya Petrukhin
 ------------------------------------------------------------------------------*/
#include	<c_types.h>
#include	<bits.h>
#include	<user_interface.h>
#include	"global.h"
#include	"at.h"
#include	"udp_setup.h"
#include	"user_spi.h"
#include	"ets.h"
#include	"lwip/err.h"
#include	"lwip/app/espconn.h"



int ICACHE_FLASH_ATTR user_udp_init (void)
{
	g.user_pcbv = udp_new ();	// (struct udp_pcb *)
	g.user_pcbv->local_ip.addr = g.local_ip.ip.addr;
	g.user_pcbv->local_port = g.local_port;

	udp_connect (g.user_pcbv, &g.remote_ip, (u16)g.remote_port);// соединиться

	/*	if (user_pcbv != NULL)
	 {
	 udp_disconnect (user_pcbv);
	 udp_remove (user_pcbv);
	 user_pcbv = NULL;
	 }

	 user_pcbv = udp_new ();	// (struct udp_pcb *)
	 if (user_pcbv == NULL
	 || (udp_bind (user_pcbv, &g.local_ip.ip, g.local_port) != ERR_OK))
	 {
	 udp_disconnect (user_pcbv);	// error
	 udp_remove (user_pcbv);
	 user_pcbv = NULL;
	 return -1;
	 }
	 */
//	udp_recv (g.user_pcbv, user_udp_receive_cb, g.user_pcbv);	// зарегистрировать функцию приёма
	return 1;
}

/******************************************************************************
 *
 ******************************************************************************/
void ICACHE_FLASH_ATTR user_udp_stop (void)
{
	/*	ets_isr_mask (BIT(ETS_FRC_TIMER0_INUM));
	 DPORT->reg_1_bits.timer0_edge_int_en = 0;
	 //	INTC_EDGE_EN &= ~BIT(1);

	 if (out_wave_pbuf != NULL)
	 {
	 os_free (out_wave_pbuf);
	 out_wave_pbuf = NULL;
	 sar_off (); // отключить SAR
	 }
	 */
}



/******************************************************************************
 * Передача данных из буфера b.spi_receive по UDP
 ******************************************************************************/
void ICACHE_FLASH_ATTR user_tx (void)
{
	struct pbuf *z;

	z = pbuf_alloc (PBUF_TRANSPORT, sizeof (b.spi_receive), PBUF_RAM);
	if (z != NULL)
	{
		err_t err = pbuf_take (z, b.spi_receive, sizeof (b.spi_receive));
		if (err == ERR_OK)
		{
			udp_sendto (g.user_pcbv, z, &g.remote_ip, g.remote_port);
		}
		pbuf_free (z);
	}
}


