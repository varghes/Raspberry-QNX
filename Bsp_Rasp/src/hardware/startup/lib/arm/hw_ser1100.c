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
 * SA-1100 specific polled serial driver
 *
 * This should be usable by any board that uses an SA-1100.
 */

#include "startup.h"
#include "arm/sa1100.h"

/*
 * Syntax: base^shift.baud.clock.alt
 * Where:
 *
 *	base  = physical address (0x8005000 = UART3, 0x80010000 = UART1)
 *	shift = not used by driver code (typically set to 0)
 *	baud  = baud rate
 *	clock = clock frequency (typically 3686400, but may be different)
 *	alt   = 'A' if port uses alternate GPIO pins for Rx/Tx
 */
static void
parse_line(unsigned channel, const char *line, unsigned *baud, unsigned *clk, int *alt)
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

	/*
	 * Check if port uses alternate GPIO pins
	 */
	if (*line == '.')
		++line;
	if (*line != '\0')
		*alt = (*line == 'A');
}

/*
 * Initialise one of the serial ports
 */
void
init_sa1100(unsigned channel, const char *init, const char *defaults)
{
	paddr_t		base;
	unsigned	baud;
	unsigned	brd;
	unsigned	clk;
	int			alt;

	/*
	 * Default clock rate is 3.6864MHz
	 */
	clk = 3686400;
	alt = 0;

	parse_line(channel, defaults, &baud, &clk, &alt);
	parse_line(channel, init, &baud, &clk, &alt);
	base = dbg_device[channel].base;

	brd = (clk / (16 * baud)) - 1;

	if (base == SA1100_UART1_BASE) {
		if (alt) {
			unsigned	tmp;

			/*
			 * Re-assign UART1 to driver GPIO pins 14/15
			 */
			out32(SA1100_PPC_BASE + SA1100_PPAR, SA1100_PPAR_UPR);

			/*
			 * Configure GPIO pin 14 as an output, pin 15 as an input
			 */
			tmp = in32(SA1100_GPIO_BASE + SA1100_GPDR);
			tmp |= SA1100_GPIO_14;
			tmp &= ~SA1100_GPIO_15;
			out32(SA1100_GPIO_BASE + SA1100_GPDR, tmp);

			/*
			 * Configure GPIO pins 14/15 to use their alternate functions
			 */
			tmp = in32(SA1100_GPIO_BASE + SA1100_GAFR);
			tmp |= SA1100_GPIO_14 | SA1100_GPIO_15;
			out32(SA1100_GPIO_BASE + SA1100_GAFR, tmp);
		}
		else {
			/*
			 * Enable UART1 output onto serial port 1 pins
			 */
			out32(SA1100_SDLC_BASE + SA1100_SDCR0, SA1100_SDCR0_SUS);
		}
	}

	/*
	 * Disable UART while programming baud rate divisor
	 */
	while (in32(base + SA1100_UTSR1) & SA1100_UTSR1_TBY)
		;
	out32(base + SA1100_UTCR3, 0);
	out32(base + SA1100_UTBRD_HI, (brd >> 8) & 0xf);
	out32(base + SA1100_UTBRD_LO, brd & 0xff);

	/*
	 * Clear status bits
	 * Set 8bits, no parity, 1 stop bit
	 */
	out32(base + SA1100_UTSR0, 0xff);
	out32(base + SA1100_UTCR0, SA1100_UTCR0_DSS);

	/*
	 * Re-enable receive and transmit
	 */
	out32(base + SA1100_UTCR3, SA1100_UTCR3_RXE | SA1100_UTCR3_TXE);
}

/*
 * Send a character
 */
void
put_sa1100(int c)
{
	paddr_t	base = dbg_device[0].base;

	while ((in32(base + SA1100_UTSR1) & SA1100_UTSR1_TNF) == 0)
		;
	out32(base + SA1100_UTDR, c & 255);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_ser1100.c $ $Rev: 657836 $" );
