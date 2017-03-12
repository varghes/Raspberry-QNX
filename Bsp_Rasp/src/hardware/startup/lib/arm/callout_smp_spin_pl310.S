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

patcher:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2				// address of callout routine

	ldr		r1, [sp, #8]			// L2 base address in patch data param
	mov		r0, #0x1000				// 4K register size
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

CALLOUT_START(smp_spin_pl310, 0, patcher)
	mov		ip,     #0x000000ff		// L2 controller base (patched)
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

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
	dsb
	isb

	/*
	 * Clean L2 line to memory so cpu0 sees the write
	 */
	mov		r4, r3, asl #20
	mov		r4, r4, lsr #20				// page offset
	mcr		p15, 0, r3, c7, c8, 0		// VA2PA for privileged read access
	mrc		p15, 0, r3, c7, c4, 0		// get physical address/attributes
	bic		r3, r3, #0xff				// clear attribute bits
	bic		r3, r3, #0xf00
	orr		r3, r3, r4					// add page offset
	str		r3, [ip, #0x7b0]			// clean line by PA
	mov		r3, #0
	str		r3, [ip, #0x730]		// cache sync
0:	ldr		r3, [ip, #0x730]
	teq		r3, #0
	bne		0b

	/*
	 * Wait until syspage->smp.start_addr is non-zero then atomically
	 * exchange it with 0 so only this cpu comes out of the loop
	 */
	mov		r4, #0
	add		r3, r2, #SMP_START_ADDR
0:	ldr		ip, [r3]
	teq		ip, #0
	beq		0b
	ldrex	ip, [r3]
	strex	r2, r4, [r3]
	teq		r2, #1
	teqne	ip, #0
	beq		0b						// strex failed or start_addr == 0
	dsb
	isb

	mvn		r2, #0
	teq		ip, r2
	movne	pc, ip					// jump to start_addr

	/*
	 * If start_addr is -1, kernel wants us to shut down
	 * Stuff the -1 back into start_addr to shut down the next processor
	 */
	str		ip, [r3]
	mov		r0, #0
0:	dsb
	wfi	
	b		0b
CALLOUT_END(smp_spin_pl310)
