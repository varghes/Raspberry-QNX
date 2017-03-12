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
 * cstart.S
 *
 *	Initial code to get Neutrino started
 *
 * This code runs as the first instructions after control has transferred
 * from any bootup monitor.  This module defines a minimal bootup stack,
 * and sets SP to this stack, and then starts running C code in _main().
 */

	.data
	.globl	stack
	.globl	boot_args
	.globl	vstart
	
boot_args:
	.ascii	"ddpvbskr"	/* signature for mkifs */
stack_end:
	.space	3*1024
stack:
	.text

	.extern	_main
	.globl	_start

_start:
	/*
	 * Turn off interrupts and make sure we are in SVC mode
	 */
	mrs		lr, cpsr
	bic		lr, lr, #0x1f
	orr		lr, lr, #0xd3
	msr		cpsr, lr

	/*
	 * Turn off MMU and data cache if necessary.
	 * WARNING: assumes we are running with a 1-1 mapping if MMU is enabled.
	 */
	mrc		p15, 0, lr, c1, c0, 0
	bic		lr, lr, #0x0000000f			// WCAM bits
	bic		lr, lr, #0x00000300			// RS   bits
	mcr		p15, 0, lr, c1, c0, 0
	mov		r0, #0
	mov		r0, r0
	mov		r0, r0

	/*
	 * Invalidate instruction cache and TLBs.
	 * WARNING: assumes data caches are clean.
	 */
	mcr		p15, 0, r0, c7, c5, 0		// invalidate instruction caches
	mcr		p15, 0, r0, c8, c7, 0		// invalidate TLBs

	ldr		sp, =stack
	bl		_main

oops:
	b		oops

