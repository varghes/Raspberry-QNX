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
 * SA1100/1110 power callout.
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
	add		r0, r0, r2			// address of callout routine
	ands	r1, r0, #0x1f
	moveq	pc, lr				// code is already suitably aligned

	add		r0, r0, #0x1c		// address of instruction following the nops
	sub		r1, r0, r1			// address we want to copy it to

	/*
	 * Copy the 10 words of the callout code down
	 */
	ldr		r2, [r0], #4
	str		r2, [r1], #4

	ldr		r2, [r0], #4
	str		r2, [r1], #4

	ldr		r2, [r0], #4
	str		r2, [r1], #4

	ldr		r2, [r0], #4
	str		r2, [r1], #4

	ldr		r2, [r0], #4
	str		r2, [r1], #4

	ldr		r2, [r0], #4
	str		r2, [r1], #4

	ldr		r2, [r0], #4
	str		r2, [r1], #4

	ldr		r2, [r0], #4
	str		r2, [r1], #4

	ldr		r2, [r0], #4
	str		r2, [r1], #4

	ldr		r2, [r0], #4
	str		r2, [r1], #4

	mov		pc, lr

/*
 * --------------------------------------------------------------------------
 * Use the CP15 register 15 "Wait for Interrupt"
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
CALLOUT_START(power_sa1100, 0, patch_power)

	/*
	 * Insert 7 nop instructions so the patcher code can shift the code
	 * down to ensure the clock switch disable mcr is 8-word aligned.
	 */
	mov		r0, r0
	mov		r0, r0
	mov		r0, r0
	mov		r0, r0
	mov		r0, r0
	mov		r0, r0
	mov		r0, r0
	
	/*
	 * Generate an uncached virtual address.
	 * WARNING: this value is ARM_STARTUP_BASE and we assume that the code in
	 *          init_mmu.c will have mapped the L1 table here.
	 */
	ldr		r0, Luncached_addr

	teq		r1, #0
	beq		1f
	
	/*
	 * Disable clock switching and load from an uncached address to
	 * force cpu clock to memory clock.
	 */
	mcr		p15, 0, r0, c15, c2, 2
	ldr		r0, [r0]

	/*
	 * Wait for interrupt and re-enable clock switching once out of idle
	 */
	mcr		p15, 0, r0, c15, c8, 2
	mcr		p15, 0, r0, c15, c1, 2

1:
	mov		r0, #0
	mov		pc, lr

Luncached_addr:	.word	0xfc400000

CALLOUT_END(power_sa1100)
