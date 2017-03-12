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
#include "arm/ixp2400.h"
#include <hw/8250.h>

/*
 * Drive IXP2400 serial channel (similar to 8250)
 */

static void
parse_line(unsigned channel, const char *line, unsigned long *baud, unsigned long *clk, unsigned long *div) {
	
    if(*line != '.' && *line != '\0') {
		dbg_device[channel].base = strtopaddr(line, (char **)&line, 16);
		if(*line == '^') dbg_device[channel].shift = strtoul(line+1, (char **)&line, 0);
	}
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *baud = strtoul(line, (char **)&line, 0);
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *clk = strtoul(line, (char **)&line, 0);
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *div = strtoul(line, (char **)&line, 0);
}

void
init_ixp2400(unsigned channel, const char *init, const char *defaults) {
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
	chip_access(base, shift, 0, REG_MS);
	chip_write32(REG_IE,IXP2400_UART_IER_UUE);  //turn serial port on
	if(baud != 0) {
		unsigned count = clk / (baud * div);
	
		// Program divisor latch
		chip_write32(REG_LC, LCR_DLAB);
		chip_write32(REG_DL0, count & 0xff);
		chip_write32(REG_DL1, count >> 8);
		chip_write32(REG_LC, 0x03);
	}
	chip_done();
}

void
put_ixp2400(int c) {
	paddr_t			base;
	unsigned		shift;

	base = dbg_device[0].base;
	shift = dbg_device[0].shift;
	chip_access(base, shift, 0, REG_MS);
	do {
	} while( !(chip_read32(REG_LS) & LSR_TXRDY));
	chip_write32(REG_TX, c);
	chip_done();
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_serixp2400.c $ $Rev: 657836 $" );
