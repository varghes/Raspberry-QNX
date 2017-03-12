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
#include "callout.ah"

/*
 * This code is used as the virtual entry point for secondary processors.
 *
 * As the kernel initialises each processor, it will store the address
 * of its secondary processor initialisation code into the syspage smp
 * entry's start_addr field.
 *
 * We atomically exchange this so that only one processor at a time will
 * pick up the address and jump to it.
 *
 * r0 contains the _syspage_ptr
 * r1 contains the cpu number
 */

CALLOUT_START(smp_spin, 0, 0)
	ldrh	r2, [r0, #SYSPAGE_SMP]	// smp.entry_off
	add		r2, r0, r2

	/*
	 * Set syspage->smp.pending to 0 to tell transfer_aps() to start the
	 * next processor
	 */
	add		r3, r2, #SMP_PENDING
	mov		r4, #0
	str		r4, [r3]	

	/*
	 * cpu0 is still running with MMU/caches disabled so we need to flush
	 * the data cache line for smp.pending so cpu0 sees the write.
	 *
	 * WARNING: assumes cpu supports clean-dcache-by-MVA and 32-bye lines.
	 */
	bic		r3, r3, #0x1f
	mcr		p15, 0, r3, c7, c10, 1	// clean dcache line by MVA
	mcr		p15, 0, r4, c7, c10, 4	// data synchronisation barrier

	/*
	 * Wait until syspage->smp.start_addr is non-zero then atomically
	 * exchange it with 0 so only this cpu comes out of the loop
	 */
	add		r3, r2, #SMP_START_ADDR
0:	ldr		ip, [r3]
	teq		ip, #0
	beq		0b
	swp		ip, r4, [r3]
	teq		ip, #0
	beq		0b
	mcr		p15, 0, r4, c7, c10, 5	// data memory barrier

	mvn		r2, #0
	teq		ip, r2
	movne	pc, ip					// jump to start_addr

	/*
	 * If start_addr is -1, kernel wants us to shut down
	 * Stuff the -1 back into start_addr to shut down the next processor
	 */
	str		ip, [r3]
	mov		r0, #0
0:	mcr		p15, 0, r0, c7, c10, 4	// data synchronisation barrier
	mcr		p15, 0, r0, c7, c0, 4	// wait for interrupt
	b		0b
CALLOUT_END(smp_spin)
