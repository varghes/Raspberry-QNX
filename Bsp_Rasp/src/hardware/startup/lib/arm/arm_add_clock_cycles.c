/*
 * $QNXLicenseC: 
 * Copyright 2010, QNX Software Systems.  
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

/*
 * FIXME:
 *  This is already defined in a libc.  We are temporarily defining it here
 *  in order to not break bsp builds because they pick up the libc.a from the
 *  installed sdp rather than the one that matches the startup source.
 */
unsigned __clockcycles_incr_bit;

static output_callout_t		*prev_output_rtn;

static output_callout_t *
callout_output_clock(int sizing)
{
	struct arm_cpu_entry	*cpu = lsp.cpu.arm_cpu.p;

	callout_output_one(sizing, &cpu->clock_cycles);

	return prev_output_rtn;
}

void
arm_add_clock_cycles(struct callout_rtn *callout, int incr_bit)
{
	struct arm_cpu_entry	*cpu = lsp.cpu.arm_cpu.p;

	if (prev_output_rtn == 0) {
		prev_output_rtn = callout_output_rtn;
		callout_output_rtn = (output_callout_t *)callout_output_clock;
	}
	cpu->clock_cycles = (void *)callout;

	if (incr_bit == 32) {
		__clockcycles_incr_bit = 32;
	} else {
		__clockcycles_incr_bit = 0;
	}
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/arm_add_clock_cycles.c $ $Rev: 657836 $" );
