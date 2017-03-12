/*
 * $QNXLicenseC:
 * Copyright 2008, QNX Software Systems. 
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





#include "startup.h"
#include <arm/omap.h>

/*
 * Drive OMAP serial channel (similar to 8250)
 */

static void
parse_line(unsigned channel, const char *line, unsigned long *baud, unsigned long *clk, unsigned long *div) {
	
    if(*line != '.' && *line != '\0') {
		dbg_device[channel].base = strtoul(line, (char **)&line, 16);
		if(*line == '^') dbg_device[channel].shift = strtoul(line+1, (char **)&line, 0);
	}
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *baud = strtoul(line, (char **)&line, 0);
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *clk = strtoul(line, (char **)&line, 0);
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *div = strtoul(line, (char **)&line, 0);
}

static void set_port(unsigned reg, unsigned shift, unsigned mask, unsigned data) {
	unsigned char c;

	c = chip_read8(reg >> shift);
	chip_write8(reg >> shift, (c & ~mask) | (data & mask));
}

void
init_omap(unsigned channel, const char *init, const char *defaults) {
	unsigned long 	baud;
	unsigned long	div;
	unsigned long	clk;
	paddr_t			base;
	unsigned		shift;

	baud = 0;
	parse_line(channel, defaults, &baud, &clk, &div);
	parse_line(channel, init, &baud, &clk, &div);

	base = dbg_device[channel].base;
	shift = dbg_device[channel].shift;

	chip_access(base, shift, 0, OMAP_UART_SIZE);

	if (baud != 0) {
		unsigned count = clk / (baud * div);

		/* Mask all interrupts causes and disable sleep mode and low power mode. */
		set_port(OMAP_UART_IER, shift, 0xff, 0x0);

		/* Disable UART (default state) */
		set_port(OMAP_UART_MDR1, shift, 0xff, 0x07);

		/* Turn FCR into EFR access */
		set_port(OMAP_UART_LCR, shift, 0xff, 0xbf);

		/* This is EFR since LCR = 0xBF */
		set_port(OMAP_UART_FCR, shift, 0x50, 0x50);

		/* Divisor latch enable, 8N1 */
		set_port(OMAP_UART_LCR, shift, 0xff, 0x83);

		/* Enable access to TCR and TLR registers */
		set_port(OMAP_UART_MCR, shift, 0x40, 0x40);

		/*
		 * Reg#6 ?? only when LCR = 0xBF, else it is TCR
		 * TCR set to default (halt RX at 52 bytes, start RX at 0 bytes)
		 */
		set_port(OMAP_UART_XOFF1, shift, 0xff, 0x0d);

		/*
		 * Reg#7, XOFF2 when LCR = 0xBF, else it is SPR/TLR
		 * TLR set to 48bytes on RX FIFO, 60 on TX FIFO
		 */
		set_port(OMAP_UART_TLR, shift, 0xff, 0xcf);

		/* Disable access to TCR and TLR */
		set_port(OMAP_UART_MCR, shift, 0x40, 0x0);

		/* FCR set to 60/32/TXclear/RXclear/Enable */
		set_port(OMAP_UART_FCR, shift, 0xff, 0xe7);

		set_port(OMAP_UART_DLL, shift, 0xff, count);		/* baud rate. lobyte */
		set_port(OMAP_UART_DLH, shift, 0xff, (count >> 8));	/* baud rate. hibyte */

		set_port(OMAP_UART_LCR, shift, 0xff, 0x03);			/* Divisor latch disable, 8N1 */
		set_port(OMAP_UART_MDR1, shift, 0xff, 0x00);		/* UART mode */
	}

	chip_done();
}

void
put_omap(int c) {
	paddr_t		base;
	unsigned	shift;

	base = dbg_device[0].base;
	shift = dbg_device[0].shift;

	chip_access(base, shift, 0, OMAP_UART_SIZE);

	while (!(chip_read8(OMAP_UART_LSR >> shift) & OMAP_LSR_TXRDY))
		;

	chip_write8(OMAP_UART_THR >> shift, c);

	chip_done();
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_seromap.c $ $Rev: 657836 $" );
