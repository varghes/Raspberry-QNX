/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. 
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
 * EP93xx specific polled serial driver
*/

#include "startup.h"
#include "ep93xx_private.h"


static void parse_line(unsigned channel, const char *line, unsigned *baud, unsigned *clk)
{
	if (line == NULL) return;
    if(*line != '.' && *line != '\0') {
		dbg_device[channel].base = strtopaddr(line, (char **)&line, 16);
		if(*line == '^') dbg_device[channel].shift = strtoul(line+1, (char **)&line, 0);
	}
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *baud = strtoul(line, (char **)&line, 0);
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *clk = strtoul(line, (char **)&line, 0);
}


/*******************************************************************************
 * init_ep93xx_uart
 * 
 * Initialise UART <channel>
 * 
 * note that updates to the LNCTRL registers MUST occur as 8 bit writes to
 * the LOW, MID and finally the HIGH register. This is true even if the
 * HIGH register value will not change (ie. you must read it out then write
 * it back).
 * The Low and Mid registers hold the LSB and MSB of the baudrate divisor.
 * 
 * The baud rate divisor is caclulated as follows (pg 14-21 of EP93xx Users Guide)
 * 		
 * 		BAUDDIV = (FUARTCLK / 16 * Baud rate)) – 1
 * 
*/
void init_ep93xx_uart(unsigned channel, const char *init, const char *defaults)
{
	unsigned baud = 0;
	unsigned clk = 0;
	uint16_t baudrate_div;
	const unsigned long fuart_clk = ep93xx_get_uartclk();
	
	/* get the baud rate from the defaults string */
	parse_line(channel, defaults, &baud, &clk);
	ASSERT(baud != 0);

	/* we don't require the 'clk' option but if its passed it must agree with get_fuartclk() */
	ASSERT((clk == 0) || (clk == fuart_clk));

	/* startup expects UART1 and UART2 for the debug devices (we don't care which is which) */
	ASSERT((dbg_device[channel].base == EP93xx_UART1_BASE) || (dbg_device[channel].base == EP93xx_UART2_BASE));

	chip_access(dbg_device[channel].base, 0, 1, EP93xx_UART1_SIZE);

	/* disable the UART */
	chip_write32(EP93xx_UART_CTRL, 0);

	/* figure out the baudrate divisor needed for the selected baud rate */
	baudrate_div = (fuart_clk / (16 * baud)) - 1;
	ASSERT(baudrate_div != 0);	// UART won't work if this is the case
	chip_write32(EP93xx_UART_LNCTRL_L, baudrate_div & 0xFFU);
	chip_write32(EP93xx_UART_LNCTRL_M, baudrate_div >> 8);

	/* select 8bit, no-parity, 1 stop bit and no FIFO */
	chip_write32(EP93xx_UART_LNCTRL_H, EP93xx_UART_LNCTRL_H_8N1);

	/* clear any pending interrupts (just in case) */
	chip_write32(EP93xx_UART_INTR, 0);

	/* Enable UART (no interrupts) */
	chip_write32(EP93xx_UART_CTRL, EP93xx_UART_CTRL_ENABLE);

	chip_done();
	
	/* make sure the UART is enabled in the EP93xx_SYSCTRL_DEVICECFG register */
	{
		uint32_t enable_mask = (dbg_device[channel].base == EP93xx_UART1_BASE) ? 0x00040000U : 0x00100000U;
		chip_access(EP93xx_SYSCTRL_BASE, 0, 1, EP93xx_SYSCTRL_SIZE);
		chip_write32(EP93xx_SYSCTRL_DEVICECFG, chip_read32(EP93xx_SYSCTRL_DEVICECFG) | enable_mask);
		chip_done();
	}
}


/*******************************************************************************
 * put_ep93xx_uart
 * 
 * Send a character out the debug_device[0] UART
 * 
 * Note that we wait for TXFF (Transmit Fifo Full) condition to clear to indicate
 * that we can write a character as this will work whether the transmit fifo is
 * enabled or not where as TXFE and BUSY will cause unneccesary delays when the
 * fifo is enabled
*/
void put_ep93xx_uart(int c)
{
	paddr_t base = dbg_device[0].base;

	ASSERT(base != NULL);

	chip_access(base, 0, 1, EP93xx_UART1_SIZE);
	while (chip_read32(EP93xx_UART_FLAGS) & EP93xx_UART_FLAGS_TXFF);
	chip_write32(EP93xx_UART_DATA, (unsigned)c & 0xFFU);
	chip_done();
}
