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
 * Entry point for secondary processors.
 *
 * The board-specific board_smp_start() arranges for each secondary
 * processor to begin execution here.
 */

	.text
	.globl	smp_start

smp_start:
	/*
	 * Set up stack
	 */
	ldr		sp, =stack

	/*
	 * Get our cpu number and save in r4 for later
	 */
	ldr		ip, =cpu_starting
	ldr		r0, [ip]
	sub		r0, r0, #1
	bl		board_smp_adjust_num
	mov		r4, r0

	/*
	 * Perform any custom cpu startup operations
	 */
	bl		cpu_startup

	/*
	 * Initialise cpuinfo
	 */
	mov		r0, r4
	bl		init_one_cpuinfo

	/*
	 * Tell bootstrap cpu start_aps() that it can start the next cpu
	 */
	ldr		ip, =cpu_starting
	mov		r0, #0
	str		r0, [ip]

	/*
	 * Wait for bootstrap cpu to finish initialising the system page
	 */
	ldr		ip, =syspage_available
1:	ldr		r0, [ip]
	teq		r0, r4
	bne		1b

	ldr		ip, =smp_spin_vaddr
	ldr		r0, [ip]
	mov		r1, r4
	bl		cpu_startnext

	/*
	 * We should not return from cpu_startnext
	 */
0:	b		0b

.data
	.space	512
stack:
