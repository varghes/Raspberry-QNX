/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. All Rights Reserved.
 * 
 * You must obtain a written license from and pay applicable license fees to QNX 
 * Software Systems before you may reproduce, modify or distribute this software, 
 * or any work that includes all or part of this software.   Free development 
 * licenses are available for evaluation and non-commercial purposes.  For more 
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *  
 * This file may contain contributions from others.  Please review this entire 
 * file for other proprietary rights or license notices, as well as the QNX 
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
 * for other information.
 * $
 */




#include "at91sam9xx.h"


//
// this is the rx interrupt handler which directly masks off 
// the hardware interrupt at the nic, and should be a bit faster
//
const struct sigevent *
at91sam9xx_isr_rx(void *arg, int iid)
{
	at91sam9xx_dev_t		*at91sam9xx;
	struct _iopkt_inter	*ient;
	volatile uint32_t	*base;


	at91sam9xx	= arg;
	ient = &at91sam9xx->inter_rx;
	base = at91sam9xx->reg;


	/*
	 * We have to make sure the interrupt is masked regardless
	 * of our on_list status.  This is because of a window where
	 * a shared (spurious) interrupt comes after on_list
	 * is knocked down but before the enable() callout is made.
	 * If enable() then happened to run after we masked, we
	 * could end up on the list without the interrupt masked
	 * which would cause the kernel more than a little grief
	 * if one of our real interrupts then came in.
	 *
	 * This window doesn't exist when using kermask since the
	 * interrupt isn't unmasked until all the enable()s run
	 * (mask count is tracked by kernel).
	 */

	*(base + EMAC_IDR) = ~0;

	return interrupt_queue(at91sam9xx->iopkt, ient);
}

//
// convert virtual to physical address
//
static paddr_t
vtophys (void *addr)
{
    off64_t  offset;

    if (mem_offset64 (addr, NOFD, 1, &offset, 0) == -1) {
        return (-1);
    }
    return (offset);
}

// 
// this is rx interrupt handler when using the kermask method
//
const struct sigevent *
at91sam9xx_isr_rx_kermask(void *arg, int iid)
{
	at91sam9xx_dev_t		*at91sam9xx;
	struct _iopkt_inter	*ient;

	at91sam9xx	= arg;
	ient = &at91sam9xx->inter_rx;


	at91sam9xx->iid_rx = iid;

	InterruptMask(at91sam9xx->cfg.irq[AT91SAM9xx_RX_INT], iid);

	return interrupt_queue(at91sam9xx->iopkt, ient);
}


int
at91sam9xx_enable_rx_kermask(void *arg)
{
	at91sam9xx_dev_t *at91sam9xx = arg;

	InterruptUnmask(at91sam9xx->cfg.irq[AT91SAM9xx_RX_INT], at91sam9xx->iid_rx);

	return 0;
}



int
at91sam9xx_enable_rx(void *arg)
{
	at91sam9xx_dev_t  		*at91sam9xx= arg;
	volatile uint32_t  	*base	= at91sam9xx->reg;

	*(base + EMAC_IER) = AT91SAM9xx_ENABLED_IMASK;

	return 0;
}

// Transmit Under run H/w limitation
void reset_tx(void *arg)
{
	at91sam9xx_dev_t        *at91sam9xx     = arg;
	uint32_t        	*base           = at91sam9xx->reg;
	unsigned        	ctl;
	int 			i;	
	mpc_bd_t		*bd;
	struct mbuf             *m;

	 /* Disable all interrupts */
	*(base + EMAC_IDR) = 0x00003CFF;

	/* Disable Tx */
	ctl = *(base + EMAC_NCR);
	*(base + EMAC_NCR) &= ~EMAC_NCR_TE;

	/* Clear Tx Status */
	*(base + EMAC_TSR) = 0; //*(base + EMAC_TSR);

	at91sam9xx->tx_pidx = at91sam9xx->tx_cidx = at91sam9xx->tx_descr_inuse = 0;

	for(i=0; i < at91sam9xx->num_tx_descriptors; i++) {
		bd = &at91sam9xx->tx_bd[i];
		
		bd->status = EMAC_TBD_FREE;

		if(at91sam9xx->sram_buffers)
			bd->buffer = vtophys ((char *)at91sam9xx->tx_buffers +  (i * TX_BUFFER_SIZE));

		// free any previously txd mbufs
                if ((m = at91sam9xx->tx_pkts[i]) != NULL) {
                        m_freem(m);
                        at91sam9xx->tx_pkts[i] = NULL;
                } 
		if (i == (at91sam9xx->num_tx_descriptors - 1)) {
                	bd->status |= EMAC_TBD_WRAP;
		}	
	}

	/* Enable all interrupts */
	*(base + EMAC_IER) |= AT91SAM9xx_ENABLED_IMASK;

	/* Enable TX */
	*(base + EMAC_NCR) |= (EMAC_NCR_TE | EMAC_NCR_RE);	
}

	

//
// all three hardware interrupt sources are handled here
//
int
at91sam9xx_process_interrupt(void *arg, struct nw_work_thread *wtp)
{
	at91sam9xx_dev_t	*at91sam9xx	= arg;
	uint32_t    	*base 		= at91sam9xx->reg;
	uint32_t    	ievent,ctl;

    for (;;) {

	// read interrupt cause bits
        ievent = *(base + EMAC_ISR);  

		if (at91sam9xx->cfg.verbose > 6) {
			log(LOG_ERR, "%s(): %d - ievent 0x%X\n", __FUNCTION__,__LINE__, ievent);
		}

	if (!ievent) {
            break;
        }
	
	if(ievent & EMAC_RCOMP) {
		at91sam9xx_receive(at91sam9xx, wtp);
	}

#if	0	/* Uncomment this code to use transmit interrupts */
	if(ievent & EMAC_TUN) {
		if(at91sam9xx->cfg.verbose > 6) {
			log(LOG_ERR, "%s(): Transmit Underrun issue\n",__FUNCTION__);
		}
	
	//reset nic
	reset_tx(at91sam9xx);
	}
	else if(ievent & EMAC_TCOMP){ 
		at91sam9xx_transmit_complete(at91sam9xx);
	}
#endif
	if(ievent & EMAC_RXUBR) {
		ctl = *(base + EMAC_RSR);

		if(ctl & EMAC_RSR_BNA) {
			ctl = *(base + EMAC_NCR);

			*(base + EMAC_NCR) = ctl & ~EMAC_NCR_RE;
			*(base + EMAC_NCR) = ctl | EMAC_NCR_RE;
		}
	}

	at91sam9xx_update_stats(at91sam9xx);
    }

	return 1;
}


void	at91sam9xx_hk_callout(void *arg)

{
at91sam9xx_dev_t		*at91sam9xx = arg;
struct ifnet			*ifp = &at91sam9xx->ecom.ec_if;
struct _iopkt_self		*iopkt = at91sam9xx->iopkt;
struct nw_work_thread	*wtp = WTP;

	// if the transmit side is quiet, process txd descriptors now
	if (!at91sam9xx->start_running) {
		NW_SIGLOCK_P (&ifp->if_snd_ex, iopkt, wtp);
		at91sam9xx->hk = 1;
		at91sam9xx_transmit_complete (at91sam9xx);
		NW_SIGUNLOCK_P (&ifp->if_snd_ex, iopkt, wtp);
		at91sam9xx->hk = 0;
		}
	callout_msec (&at91sam9xx->hk_callout, 1 * 1000, at91sam9xx_hk_callout, at91sam9xx);
}

__SRCVERSION( "$URL$ $REV$" )
