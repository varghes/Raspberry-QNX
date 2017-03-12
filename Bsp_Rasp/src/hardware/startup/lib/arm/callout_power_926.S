#
# Copyright 2009, QNX Software Systems. 
# 
# Licensed under the Apache License, Version 2.0 (the "License"). You 
# may not reproduce, modify or distribute this software except in 
# compliance with the License. You may obtain a copy of the License 
# at: http://www.apache.org/licenses/LICENSE-2.0 
# 
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OF ANY KIND, either express or implied.
#
# This file may contain contributions from others, either as 
# contributors under the License or as licensors under other terms.  
# Please review this entire file for other proprietary rights or license 
# notices, as well as the QNX Development Suite License Guide at 
# http://licensing.qnx.com/license-guide/ for other information.
#
#include "callout.ah"

/*
 * --------------------------------------------------------------------------
 * Use the CP15 register 7 "Wait for Interrupt" to halt the processor.
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - flag (currently always 0)
 *
 * FIXME: currently, this callout is only called from the ARM specific halt()
 *        function in the kernel. If this becomes used for a general purpose
 *        power management facility, we will need to handle arguments.
 * --------------------------------------------------------------------------
 */
CALLOUT_START(power_926, 0, 0)

	teq		r1, #0
	bne		1f
	
	mrs		r3, CPSR
	orr		r2, r3, #(1 << 7)		// set CPSR_I bit
	msr		CPSR, r2
	mrc		p15, 0, r0, c1, c0, 0	// read control register
	mcr		p15, 0, r1, c7, c10, 4	// drain write buffer - r1 is 0
	bic		r2, r0, #(1 << 12)		// clear ARM_MMU_CR_I to disable Icache
	mcr		p15, 0, r2, c1, c0, 0
	mcr		p15, 0, r1, c7, c0, 4	// wait for interrupt - r1 is 0
	mcr		p15, 0, r0, c1, c0, 0	// re-enable Icache
	msr		CPSR, r3				// re-enable interrupts
	
1:
	mov		r0, #0
	mov		pc, lr

CALLOUT_END(power_926)

