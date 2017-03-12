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
#include <hw/zscc.h>

/*
 * Drive an Zilog SCC style serial chip.
 */

unsigned reg_recovery = 200;

static void
parse_line(unsigned channel, const char *line, unsigned long *baud, unsigned long *clk, unsigned long *div) {
	unsigned	recovery;
	
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
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') {
		recovery = strtoul(line, (char **)&line, 16);
		if(recovery > reg_recovery) reg_recovery = recovery;
	}
}

static void
write_reg0(uint8_t data) {
	int i;

	chip_write8(0, data);
	for(i=reg_recovery; i > 0; --i) {}
}

static void
write_reg(uint8_t no, uint8_t data) {
	if(no < 8) {
		write_reg0(no);
	} else if(no < 16) {
		write_reg0((no-8) | 0x8);
	} else {
	    return;
	}
	write_reg0(data);
}

void
init_zscc(unsigned channel, const char *init, const char *defaults) {
	unsigned long 	baud;
	unsigned long	div;
	unsigned long	clk;
	paddr_t			base;
	unsigned		shift;
	uint16_t		value;

	parse_line(channel, defaults, &baud, &clk, &div);
	parse_line(channel, init, &baud, &clk, &div);

	shift = dbg_device[channel].shift;
	base = dbg_device[channel].base;
	chip_access(base, shift, 0, 2);

	if(div < 16) {
		div = 1;
		write_reg(4, SCC_WR4_STOP_1 | SCC_WR4_CLOCK_X1);
	} else if(div < 32) {
		div = 16;
		write_reg(4, SCC_WR4_STOP_1 | SCC_WR4_CLOCK_X16);
	} else if(div < 64) {
		div = 32;
		write_reg(4, SCC_WR4_STOP_1 | SCC_WR4_CLOCK_X32);
	} else {
		div = 64;
		write_reg(4, SCC_WR4_STOP_1 | SCC_WR4_CLOCK_X64);
	}

	// Set Baud rate
	if(baud != 0) {
		value = ((clk/(baud * div)) >> 1) - 2;
		write_reg(12, value & 0xff);
		write_reg(13, value >> 8);
	}

	// 8 bits no parity
	write_reg(3, 0xc1);
	write_reg(5, 0xea);
	write_reg(11, SCC_WR11_RXCLK_BRGEN | SCC_WR11_TXCLK_BRGEN);
	write_reg(14, SCC_WR14_BRGEN_ENABLE | SCC_WR14_BRGEN_PCLK);
	chip_done();
}

void
put_zscc(int c) {
	int			status;
	int			i;
	paddr_t		base;
	unsigned	shift;

	base = dbg_device[0].base;
	shift = dbg_device[0].shift;
	chip_access(base, shift, 0, 2);
	do {
		status = chip_read8(0);
		for(i=reg_recovery; i > 0; --i) {}
	} while(!(status & SCC_RR0_TX_EMPTY));
	chip_write8(1, c);
	for(i=reg_recovery; i > 0; --i) {}
	chip_done();
}

__SRCVERSION("hw_serzscc.c $Rev: 657836 $");
