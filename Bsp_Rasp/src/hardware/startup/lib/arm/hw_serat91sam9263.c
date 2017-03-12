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
#include <arm/at91sam9263.h>

/*
 * Drive an at91sam9263 style serial chip.
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
init_at91sam9263(unsigned channel, const char *init, const char *defaults) {
	unsigned long 	baud;
	unsigned long	div;
	unsigned long	clk;
	paddr_t			base;
	const unsigned	shift = 0;    /* We can only access this device in 32 bit */

	baud = 0;
	parse_line(channel, defaults, &baud, &clk, &div);
	parse_line(channel, init, &baud, &clk, &div);

	base = dbg_device[channel].base;
	chip_access(base, shift, 0, AT91SAM9263_DBGU_SIZE);
    
	if(baud != 0) {
		unsigned count = clk / (baud * div);
	
        // Reset Tx and RX
        chip_write32(AT91SAM9263_DBGU_CR, AT91SAM9263_DBGU_CR_RSTTX
                                        | AT91SAM9263_DBGU_CR_RSTRX
                                        | AT91SAM9263_DBGU_CR_RSTSTA);

		// Program divisor latch
		chip_write32( AT91SAM9263_DBGU_BRGR
                    , count & AT91SAM9263_DBGU_BRGR_CD_MASK);

        // 8 bits no parity
        chip_write32(AT91SAM9263_DBGU_MR, AT91SAM9263_DBGU_MR_PAR_NONE
                                        | AT91SAM9263_DBGU_MR_CHMODE_NORMAL);
        
        // Enable Tx and RX
        chip_write32(AT91SAM9263_DBGU_CR, AT91SAM9263_DBGU_CR_TXEN
                                        | AT91SAM9263_DBGU_CR_RXEN);
	}



	chip_done();
}

void
put_at91sam9263(int c) {
	paddr_t		base;
	const unsigned	shift = 0;    /* We can only access this device in 32 bit */

	base = dbg_device[0].base;

	chip_access(base, shift, 0, AT91SAM9263_DBGU_SIZE);
	do {
	} while((chip_read32(AT91SAM9263_DBGU_SR) & AT91SAM9263_DBGU_IE_SR_TXRDY) == 0);
    
	chip_write32(AT91SAM9263_DBGU_THR, c);
	chip_done();
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_serat91sam9263.c $ $Rev: 657836 $" );
