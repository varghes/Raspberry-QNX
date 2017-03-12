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
#include <arm/pxa250.h>

/*
 * PXA250 serial ports
 */

static void
parse_line(unsigned channel, const char *line, unsigned long *baud, unsigned long *clk)
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

void
init_pxa250(unsigned channel, const char *init, const char *defaults) {
	unsigned long 	baud;
	unsigned long	clk;
	paddr_t			base;
	unsigned		brd;
	unsigned		tmp;

	/*
	 * Default clock rate is 14.7456MHz
	 */
	clk = 14745600;

	parse_line(channel, defaults, &baud, &clk);
	parse_line(channel, init, &baud, &clk);
	base = dbg_device[channel].base;

	brd = clk / (16 * baud);

	/*
	 * Set baud rate
	 */
	tmp = in32(base + PXA250_LCR);
	out32(base + PXA250_LCR, tmp | PXA250_LCR_DLAB);
	out32(base + PXA250_DLL, brd & 0xff);
	out32(base + PXA250_DLH, (brd >> 8) & 0xff);

	/*
	 * Set 8-bit, 1-stop, no-parity (also clear DLAB bit)
	 */
	out32(base + PXA250_LCR, PXA250_LCR_WLS_8);

	// eat extraneous characters
	while( (in32(base + PXA250_LSR) & PXA250_LSR_DR) != 0)
		in32(base + PXA250_RBR);

	/*
	 * Enable UART, NRZ encoding, no interrupts
	 */
//	out32(base + PXA250_IER, PXA250_IER_UUE | PXA250_IER_NRZE);
	out32(base + PXA250_IER, PXA250_IER_UUE );

}

void
put_pxa250(int c)
{
	paddr_t base = dbg_device[0].base;

	while ((in32(base + PXA250_LSR) & PXA250_LSR_TDRQ) == 0)
		;
	out32(base + PXA250_THR, c & 255);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_serpxa250.c $ $Rev: 657836 $" );
