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





/*
 * ARM PL011 specific polled serial driver
 *
 * This should be usable by any board that uses a PL011 UART
 */


#include "startup.h"
#include <arm/pl011.h>

static void
parse_line(unsigned channel, const char *line, unsigned *baud, unsigned *clk)
{
	/*
	 * Get device base address and register stride
	 */
	if (*line != '.' && *line != '\0') {
		dbg_device[channel].base = strtopaddr(line, (char **)&line, 16);
		if (*line == '^')
			dbg_device[channel].shift = strtoul(line+1, (char **)&line, 0);
	}

	/*
	 * Get baud rate
	 */
	if (*line == '.')
		++line;
	if (*line != '.' && *line != '\0')
		*baud = strtoul(line, (char **)&line, 0);

	/*
	 * Get clock rate
	 */
	if (*line == '.')
		++line;
	if (*line != '.' && *line != '\0')
		*clk = strtoul(line, (char **)&line, 0);
}

/*
 * Initialise one of the serial ports
 */
void
init_pl011(unsigned channel, const char *init, const char *defaults)
{
	paddr_t		base;
	unsigned	baud;
	unsigned	clk;
	unsigned	ibrd, fbrd;

	/*
	 * Default clock and baud rate
	 */
	clk  = 14745600;
	baud = 57600;

	parse_line(channel, defaults, &baud, &clk);
	parse_line(channel, init, &baud, &clk);
	base = dbg_device[channel].base;

	if (baud != 0) {
		clk /= 16;

		ibrd = clk / baud;
		fbrd = (((clk % baud) * 64) + ((baud + 1) >> 1)) / baud;

		while (in16(base + PL011_FR) & PL011_FR_BUSY)
			;
		out16(base + PL011_CR, 0);          /* Disable UART */

		out16(base + PL011_ICR, 0x7FF);     /* Clear all status */
		out16(base + PL011_IBRD, ibrd);
		out16(base + PL011_FBRD, fbrd);
		out16(base + PL011_LCR_H, PL011_LCR_H_WLEN8 | PL011_LCR_H_FEN);
	}

	out32(base + PL011_CR, PL011_CR_UARTEN|PL011_CR_TXE|PL011_CR_RXE);
}

/*
 * Send a character
 */
void
put_pl011(int c)
{
	paddr_t base = dbg_device[0].base;

	while ((in32(base + PL011_FR) & PL011_FR_TXFF) != 0)
		;
	out32(base + PL011_DR, c & 0xff);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_serpl011.c $ $Rev: 657836 $" );
