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
 * IXP1200 specific polled serial driver
 *
 * This should be usable by any board that uses an IXP1200.
 */

#include "startup.h"
#include "arm/ixp1200.h"

/*
 * Syntax: base^shift.baud.clock
 * Where:
 *
 *	base  = physical address (0x90003400)
 *	shift = not used by driver code (typically set to 0)
 *	baud  = baud rate
 *	clock = clock frequency (typically 3686400, but may be different)
 */
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
init_ixp1200(unsigned channel, const char *init, const char *defaults)
{
	paddr_t		base;
	unsigned	baud;
	unsigned	brd;
	unsigned	clk;

	/*
	 * Default clock rate is 3.6864MHz
	 */
	clk = 3686400;

	parse_line(channel, defaults, &baud, &clk);
	parse_line(channel, init, &baud, &clk);
	base = dbg_device[channel].base;

	brd = (clk / (16 * baud)) - 1;

	/*
	 * Set baud rate divisor and 8-bit data, 1-stop, no parity
	 */
	brd = (brd << 16) | IXP1200_UART_CR_UE | IXP1200_UART_CR_DSS_8;
	out32(base + IXP1200_UART_CR, brd);
}

/*
 * Send a character
 */
void
put_ixp1200(int c)
{
	paddr_t base = dbg_device[0].base;

	while ((in32(base + IXP1200_UART_SR) & IXP1200_UART_SR_TXR) == 0)
		;
	out32(base + IXP1200_UART_DR, c & 255);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_serixp1200.c $ $Rev: 657836 $" );
