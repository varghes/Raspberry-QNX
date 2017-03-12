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
 * Samsung S3C2410 polled serial operations
 */

#include <arm/s3c2410.h>
#include "startup.h"

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
init_s3c2410(unsigned channel, const char *init, const char *defaults)
{
	paddr_t		base;
	unsigned	baud;
	unsigned	clk;
	unsigned	brd;
	int i;

	baud=0;

	parse_line(channel, defaults, &baud, &clk);
	parse_line(channel, init, &baud, &clk);
	base = dbg_device[channel].base;

	if(baud) {
		brd = (clk / (16 * baud)) - 1;
	    /*
	     * Configure UART and program baud rate divisor
	     */
	    out32(base + S3C2410_UFCON, 0);
	    out32(base + S3C2410_UMCON, 0);
	    out32(base + S3C2410_ULCON, (ULCON_WORD_LENGTH_8BIT | ULCON_PARITY_NONE | ULCON_STOP_BIT_1));
		out32(base + S3C2410_UCON, (UCON_TX_INT_TYPE_LEVEL | UCON_RX_ERR_INT_EN | UCON_TX_MODE_INT | UCON_RX_MODE_INT));
	    out32(base + S3C2410_UBRDIV, brd);
	    for(i=0;i<100;i++); // pause
	}
}

/*
 * Send a character
 */
void
put_s3c2410(int c)
{
	paddr_t base = dbg_device[0].base;

	while ((in32(base + S3C2410_UTRSTAT) & UTRSTAT_TX_BUF_EMPTY) == 0)
		;
	out32(base + S3C2410_UTXH, c & 0xff);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_sers3c2410.c $ $Rev: 657836 $" );
