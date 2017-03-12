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
 * hw_ser2681.c
 *	Routines for yacking at the console port during early bootup
 */
#include "startup.h"
#include <hw/s2681.h>

static const unsigned uart_baudtable[] = {
  75, 110, 134, 150, 300, 600, 1200, 2000, 2400, 4800, 1800, 9600, 19200, 38400
};

static int
uart_baud2code(int baud) {
    int code;
    
    for (code = 0; code < sizeof(uart_baudtable)/sizeof(uart_baudtable[0]); code++) {
		if (uart_baudtable[code] == baud) return(code);
    }
    return(-1);
}

static void
parse_line(unsigned channel, const char *line, paddr_t *uart, unsigned *code, unsigned *duart_select) {
    if(*line != '.' && *line != '\0') {
		*uart = strtopaddr(line, (char **)&line, 16);
		if(*line == '^') dbg_device[channel].shift = strtoul(line+1, (char **)&line, 0);
	}
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *code = uart_baud2code(strtoul(line, (char **)&line, 0));
    if(*line == '.') ++line;
	switch(*line) {
	case 'a':
	case 'A':
		*duart_select = 0;
		break;
	case 'b':
	case 'B':
		*duart_select = 1;
		break;
	}
}

/*
 * init_2681()
 *  Initialize one of the serial ports on a Signetics 2681 chip
 */
void
init_2681(unsigned channel, const char *init, const char *defaults) {
	paddr_t			chip = 0;
	paddr_t			uart;
	unsigned		code = 0;
	unsigned		b_channel = 0;
	unsigned		shift;

	parse_line(channel, defaults, &chip, &code, &b_channel);
	parse_line(channel, init, &chip, &code, &b_channel);

	shift = dbg_device[channel].shift;
	uart = chip;
	if(b_channel) {
		uart += (S2681_REG_MR1B - S2681_REG_MR1A) << shift;
	}
	dbg_device[channel].base = uart;
    
    chip_access(uart, shift, 0, S2681_MEM_SIZE);
    chip_write8(S2681_REG_CRA, S2681_CR_RESETMRPTR);
    chip_write8(S2681_REG_MR1A, S2681_MR1_PRTYOFF | S2681_MR1_DBITS8);
    chip_write8(S2681_REG_MR2A, S2681_MR2_MODENORMAL | S2681_MR2_STOP16X16);

	if(code != -1) {
		chip_write8(S2681_REG_CSRA, (code << 4) | code);
	}
    chip_write8(S2681_REG_CRA, S2681_CR_STP_TXENABLE | S2681_CR_STP_RXENABLE);
	if(b_channel) {
		chip_write8(S2681_SETPORT_CMD, S2681_DTRB | S2681_RTSB);
	} else {
		chip_write8(S2681_SETPORT_CMD, S2681_DTRA);
	}
	chip_done();
}

/*
 * put_2681()
 *	Busy-wait and then send a character
 */
void
put_2681(int c) {
	paddr_t		port;
	unsigned	shift;
	unsigned	sr;

	port = dbg_device[0].base;
	shift = dbg_device[0].shift;
	chip_access(port, shift, 0, S2681_MEM_SIZE);
	do {
		sr = chip_read8(S2681_REG_SRA);
	} while((sr & S2681_SR_TXREADY) == 0);
	chip_write8(S2681_REG_THRA, c);
	chip_done();
}

__SRCVERSION("hw_ser2681.c $Rev: 657836 $");
