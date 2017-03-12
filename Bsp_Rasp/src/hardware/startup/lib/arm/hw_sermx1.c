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
 * polled serial operations for MC9328MX1/MC9328MX21/MCIMX31
 */

#include "startup.h"
#include <arm/mx1.h>

static void
parse_line(unsigned channel, const char *line, unsigned *baud, unsigned *clk)
{
	/*
	 * Get device base address and register stride
	 */
	if (*line != '.' && *line != '\0') {
		dbg_device[channel].base = strtoul(line, (char **)&line, 16);
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
init_mx1(unsigned channel, const char *init, const char *defaults)
{
	unsigned	baud, clk, base, rfdiv, tmp;

	/*
	 * Default perpherial clock rate is 96MHz 
	 */
	clk = 96000000;

	parse_line(channel, defaults, &baud, &clk);
	parse_line(channel, init, &baud, &clk);
	base = dbg_device[channel].base;

	if (baud == 0)
		return;

	/*
	 * Disable UART
	 */
	out32(base + MX1_UART_CR1, MX1_UCR1_UARTCLKEN);

	/* Reset FIFO */
	out32(base + MX1_UART_CR2, 0x00);

	/*
	 * 8-bit, no-parity, 1 stop bit
	 * ignore RTS, active CTS
	 */
	out32(base + MX1_UART_CR2, 0x5021);

	/* 
	 * Don't touch bit 2, they have different meaning
	 * in MX1 and MX21/MX31, the default value is good
	 */
	out32(base + MX1_UART_CR3, in32(base + MX1_UART_CR3) & 4);
	out32(base + MX1_UART_CR4, 0x41);

	rfdiv = clk / 16000000;
	/* We expect clock is <= 96MHz */
	if (rfdiv) {
		clk /= rfdiv;
		rfdiv = 6 - rfdiv;
	}
	else
		rfdiv = 5;

	out32(base + MX1_UART_FCR, 0x00000801 | (rfdiv << 7));

	tmp = baud * 16 / (clk / 10000);
	out32(base + MX1_UART_BIR, tmp - 1);
	out32(base + MX1_UART_BMR, 9999);

	/* Enable UART */
	out32(base + MX1_UART_CR1, MX1_UCR1_UARTCLKEN | MX1_UCR1_UARTEN);

	/* Enable Tx/Rx */
	out32(base + MX1_UART_CR2, 0x5027);
}

/*
 * Send a character
 */
void
put_mx1(int data)
{
	unsigned base = dbg_device[0].base;

	while (!(in32(base + MX1_UART_SR1) & MX1_USR1_TRDY))
		;
	out32(base + MX1_UART_TXDATA, (unsigned)data);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_sermx1.c $ $Rev: 657836 $" );
