/*
 * $QNXLicenseC:
 * Copyright 2008-2009, QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */



#include "externs.h"
#define	USART_RXERR		(AT91USART_INT_OVRE | AT91USART_INT_FRAME | AT91USART_INT_PARE)
#define	USART_RXEVENT	(USART_RXERR | AT91USART_INT_RXRDY)

static inline int tx_interrupt(DEV_USART *dev)
{
	dev->tty.un.s.tx_tmr = 0;	/* clear Timeout */
	return tto(&dev->tty, TTO_DATA, 0);
}

static inline int rx_interrupt(DEV_USART *dev)
{
	int	 status = 0;
	uintptr_t	base = dev->base;
	unsigned    key = 0;

	while (!(in32(base + BCM2835_UART0_FR) & BCM2835_DBGU_IE_SR_RXEMTY ))
	{
		key = in32(base + BCM2835_UART0_DR);
		status |= tti(&dev->tty, key);
	}
	out32(base+ BCM2835_UART0_ICR, 0x7FF);

	return status;
}

static inline int do_interrupt(DEV_USART *dev, int id)
{
	int	sts;
	sts = rx_interrupt(dev);
	return sts;
}

/*
 * Serial interrupt handler
 */
static const struct sigevent * ser_intr(void *area, int id)
{
	DEV_USART	*dev = area;

	if (do_interrupt(dev, id) && (dev->tty.flags & EVENT_QUEUED) == 0) {
		dev_lock(&ttyctrl);
		ttyctrl.event_queue[ttyctrl.num_events++] = &dev->tty;
		atomic_set(&dev->tty.flags, EVENT_QUEUED);
		dev_unlock(&ttyctrl);
		return &ttyctrl.event;
	}

	return 0;
}

void
ser_attach_intr(DEV_USART *dev)
{
	uintptr_t	base = dev->base;
	dev->intr=BCM2835_PHYSIRQ_UART0;
	out32(base+BCM2835_UART0_IMSC, (1<<1) | (1<<4)| (1<<6) |
			                      (1<<7) | (1<<8) | (1<<9) | (1<<10));

	dev->iid = InterruptAttach(dev->intr, ser_intr, dev, 0, 0);
}

