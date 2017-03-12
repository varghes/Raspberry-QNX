#
# Copyright 2011, QNX Software Systems. 
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

	.text
	.align 2
	.globl vstart_v7

/*
 * void	vstart_v7(uintptr_t syspageptr, unsigned entry_point, unsigned cpunum)
 *
 * Enable the mmu and jump to the next program's entry point
 * The next program is responsible for tearing down the 1-1 section
 * mapping of the startup program set up by init_mmu().
 */
vstart_v7:
	mov		r4, r1
	mov		r5, r2

	/*
	 * Set the translation table base
	 */
	ldr		ip, =L1_paddr
	ldr		ip, [ip]
	add		ip, ip, r5, lsl #14			// L1_paddr * (cpu * ARM_L1_SIZE)
	mcr		p15, 0, ip, c2, c0, 0		// TTBR0
	mcr		p15, 0, ip, c2, c0, 1		// TTBR1
	mov		ip, #1
	mcr		p15, 0, ip, c2, c0, 2		// TTBCR = 1

	/*
	 * Enable MMU domain 0
	 */
	mov		ip, #1
	mcr		p15, 0, ip, c3, c0, 0

	/*
	 * Enable the MMU, using read-modify-write to preserve reserved bits.
	 */
	ldr		r2, =mmu_cr_clr
	ldr		r3, =mmu_cr_set
	ldr		r2, [r2]
	ldr		r3, [r3]
	mrc		p15, 0, lr, c1, c0, 0
	bic		ip, lr, r2
	orr		ip, ip, r3
	dsb
	isb
	mcr		p15, 0, ip, c1, c0, 0
	dsb
	isb
	mov		ip, #0

	/*
	 * Invalidate the caches and TLBs
	 */
	.align 5
	mcr		p15, 0, ip, c7, c5, 0	// invalidate instruction caches
	mcr		p15, 0, ip, c8, c7, 0	// invalidate TLBs
	dsb
	isb

	/*
	 * Call entry_point(_syspage_ptr, cpunum)
	 */
	mov		r1, r5
	mov		pc, r4

