#
# Copyright 2007, 2008, QNX Software Systems. 
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
/*
 * HY7201 power callout
 */

#include "callout.ah"

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */
patch_power:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map PMU registers
	 */
	mov		r0, #4						// mapping just one register
	ldr		r1, Lpmumode
	bl		callout_io_map

	ldmia	sp!,{r4,pc}

Lpmumode:	.word	HY7201_PMU_BASE + HY7201_PMU_PMUMODE

/*
 * --------------------------------------------------------------------------
 * Set idle mode in the PMUMODE register
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
CALLOUT_START(power_hy7201, 0, patch_power)
	mov		ip,     #0x000000ff		// PMUMODE address (patched)
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	teq		r1, #0
	bne		1f
	
	mov		r0, #HY7201_PMU_PMUMODE_IDLE
	str		r0, [ip]

	/*
	 * Make sure we are back in RUN mode
	 */
	mov		r0, #HY7201_PMU_PMUMODE_RUN
	str		r0, [ip]

1:
	mov		r0, #0
	mov	pc, lr

CALLOUT_END(power_hy7201)

