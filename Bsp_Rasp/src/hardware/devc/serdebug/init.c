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
#include <sys/mman.h>
#include <pthread.h>

#define BCM2835_UART_SIZE    0x90

#if 0
/*
 * Specify parameters for default devices.
 */
void *
query_default_device(TTYINIT_USART *dip, void *link)
{
	/*
	 * No default device, the base address and irq have to be specified
	 */

	return NULL;
}
#endif

#define GPIO_BASE       0x20200000
#define LED_GPFSEL      0x10
#define LED_GPFBIT      21
#define LED_GPSET       0x20
#define LED_GPCLR       0x2c
#define LED_GPIO_BIT    15
uintptr_t gpio;


void
create_device(TTYINIT_USART *dip, unsigned unit)
{

    DEV_USART	*dev;
	/*
	 * Get a device entry and the input/output buffers for it.
	 */
	dev = (void *) _smalloc(sizeof(*dev));
	memset(dev, 0, sizeof(*dev));
	/*
	 * Get buffers.
	 */
	dev->tty.ibuf.head = dev->tty.ibuf.tail = dev->tty.ibuf.buff = _smalloc(dev->tty.ibuf.size = dip->tty.isize);
	dev->tty.obuf.head = dev->tty.obuf.tail = dev->tty.obuf.buff = _smalloc(dev->tty.obuf.size = dip->tty.osize);
	dev->tty.cbuf.head = dev->tty.cbuf.tail = dev->tty.cbuf.buff = _smalloc(dev->tty.cbuf.size = dip->tty.csize);
	dev->tty.highwater = dev->tty.ibuf.size - (dev->tty.ibuf.size < 128 ? dev->tty.ibuf.size/4 : 100);

	strcpy(dev->tty.name, dip->tty.name);

	dev->tty.baud    = dip->tty.baud;
	dev->tty.flags   = EDIT_INSERT | LOSES_TX_INTR;
	dev->tty.c_cflag = dip->tty.c_cflag;
	dev->tty.c_iflag = dip->tty.c_iflag;
	dev->tty.c_lflag = dip->tty.c_lflag;
	dev->tty.c_oflag = dip->tty.c_oflag;
	dev->tty.fifo    = dip->tty.fifo;

	dev->intr        = dip->tty.intr;
	dev->clk         = dip->tty.clk;

	/*
	 * Map device registers
	 */
	dev->base = mmap_device_io(BCM2835_UART_SIZE, BCM2835_UART0_BASE);
	if (dev->base == (uintptr_t)MAP_FAILED) {
		perror("USART : MAP_FAILED\n");
		exit(1);
	}

	/*
	 * Initialize termios cc codes to an ANSI terminal.
	 */
	ttc(TTC_INIT_CC, &dev->tty, 0);

	/* 
	 * Initialize the device's name.
	 * Assume that the basename is set in device name.  This will attach
	 * to the path assigned by the unit number/minor number combination
	 */
	unit = SET_NAME_NUMBER(unit) | NUMBER_DEV_FROM_USER;
	ttc(TTC_INIT_TTYNAME, &dev->tty, unit);

	/*
	 * Initialize power management structures before attaching ISR
	 */
	ttc(TTC_INIT_POWER, &dev->tty, 0);

	/*
	 * Only setup IRQ handler for non-pcmcia devices.
	 * Pcmcia devices will have this done later when card is inserted.
	 */

	if (dip->tty.port != 0 && dev->intr != -1) {
		ser_stty(dev);
		ser_attach_intr(dev);
	}

	/*
	 * Attach the resource manager
	 */
  //   sleep(1);// test timer
	ttc(TTC_INIT_ATTACH, &dev->tty, 0);

}
