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
 * ARM PrimeCell specific polled serial driver
 *
 * This should be usable by any board that uses a PrimeCell UART
 */

#include "arm/primecell.h"
#include "startup.h"

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
init_primecell(unsigned channel, const char *init, const char *defaults)
{
	paddr_t		base;
	unsigned	baud;
	unsigned	clk;
	unsigned	brd;

	/*
	 * Default clock and baud rate
	 */
	clk  = 14745600;
	baud = 57600;

	parse_line(channel, defaults, &baud, &clk);
	parse_line(channel, init, &baud, &clk);
	base = dbg_device[channel].base;

	/*
	 * Disable UART and set baud rate, 8bits, 1stop, no parity
	 */
	brd = (clk / (16 * baud)) - 1;
	out32(base + PRIMECELL_UART_CR, 0);
	out32(base + PRIMECELL_UART_LCRL, brd & 0xff);
	out32(base + PRIMECELL_UART_LCRM, brd >> 8);
	out32(base + PRIMECELL_UART_LCRH, PRIMECELL_UART_LCRH_8BIT |
									  PRIMECELL_UART_LCRH_FEN);

	/*
	 * Clear pending interrupts and errors, and enable UART
	 */
	out32(base + PRIMECELL_UART_ICR, 0);
	out32(base + PRIMECELL_UART_ECR, 0);
	out32(base + PRIMECELL_UART_CR, PRIMECELL_UART_CR_UARTEN);
}

/*
 * Send a character
 */
void
put_primecell(int c)
{
	paddr_t base = dbg_device[0].base;

	while ((in32(base + PRIMECELL_UART_FR) & PRIMECELL_UART_FR_TXFF) != 0)
		;
	out32(base + PRIMECELL_UART_DR, c & 0xff);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_serprimecell.c $ $Rev: 657836 $" );
