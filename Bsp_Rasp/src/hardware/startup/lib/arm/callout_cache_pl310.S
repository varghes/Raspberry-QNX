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
 * PL310 L2 cache operations
 *
 * unsigned control(paddr32_t base,
 *					unsigned num_lines,
 *					int flags,
 *					struct cacheattr_entry *cache,
 *					volatile struct syspage_entry * )
 */

#include "callout.ah"

rw_data:	.word	8
#define OFF_LOCK	0
#define OFF_L2CC	4

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0     - physical address of syspage
 *	r1     - virtual  address of syspage
 *	r2     - offset from start of syspage to start of the callout routine
 *	r3     - offset from start of syspage to read/write data used by callout
 *  [sp]   - data registered with callout via callout_register_data()
 *  [sp+4] - address of callout definition (CALLOUT_START)
 * -----------------------------------------------------------------------
 */
patcher:
	stmdb	sp!,{r4,lr}
	add		r2, r0, r2				// address of callout routine
	add		r4, r0, r3				// address of rw data

	/*
	 * Patch callout with rw offset
	 */
	ldr		r1, [r2]
	and		ip, r3, #0xff
	bic		r1, r1, #0xff
	orr		r1, r1, ip
	str		r1, [r2]				// patched 1st instruction

	ldr		r1, [r2, #4]
	mov		ip, r3, lsr #8
	and		ip, ip, #0xff
	bic		r1, r1, #0xff
	orr		r1, r1, ip
	str		r1, [r2, #4]			// patched 2nd instruction

	ldr		r1, [sp, #8]			// patch_data = L2 controller base address
	mov		r0, #0x1000				// 4K register size
	bl		callout_io_map
	str		r0, [r4, #OFF_L2CC]

	ldmia	sp!,{r4,pc}

/*
 * Parameters:
 *	r0   - paddr
 *	r1   - number of lines
 *	r2   - flags
 *	r3   - cacheattr_entry pointer
 *	[sp] - syspage pointer
 */
CALLOUT_START(cache_pl310, rw_data, patcher)
	mov		ip,     #0x000000ff		// RW offset (patched)
	orr		ip, ip, #0x0000ff00
	ldr		r3, [sp]
	add		ip, r3, ip				// address of RW data	

	/*
	 * Trim the address to a cache line boundary
	 */
	bic		r0, r0, #(32 - 1)

	tst		r2, #MS_INVALIDATE
	bne		2f

	/*
	 * Trim number of lines to end of page containing addr
	 */
	mov		r2, r0
	mov		r0, r0, asl #20
	mov		r3, r0, lsr #20			// addr & 0xfff
	rsb		r0, r3, #4096			// number of bytes to end of page
	mov		r0, r0, lsr #5			// number of lines to end of page
	cmp		r0, r1
	movgt	r0, r1

	cpsid	i
	mcr		p15, 0, r2, c7, c8, 0	// VA2PA for privileged read access
	mrc		p15, 0, r2, c7, c4, 0	// get physical address/attributes
	tst		r2, #1
	bne		1f						// lookup failed - bail out

	bic		r2, r2, #0xff0
	bic		r2, r2, #0x00f			// mask off attribute bits
	orr		r2, r2, r3				// add page offset of addr

	/*
	 * Acquire spin lock to access L2CC
	 */
	mov		r3, #1
0:	ldrex	r1, [ip]
	teq		r1, #0
	wfene
	strexeq	r1, r3, [ip]
	teqeq	r1, #0
	bne		0b
	dmb

	mov		r1, r0
	ldr		r3, [ip, #OFF_L2CC]		// L2CC register base
0:	str		r2, [r3, #0x7b0]		// clean line by PA
	add		r2, r2, #32
	subs	r1, r1, #1
	bne		0b

	str		r1, [r3, #0x730]		// cache sync
0:	ldr		r1, [r3, #0x730]
	teq		r1, #0
	bne		0b

	/*
	 * Clear spin lock
	 */
	dmb
	str		r1, [ip]
	dsb
	sev
1:	cpsie	i
	mov		pc, lr

2:
	/*
	 * Trim number of lines to end of page containing addr
	 */
	stmdb	sp!,{r4,lr}
	mov		r2, r0
	mov		r4, r0
	mov		r0, r0, asl #20
	mov		r3, r0, lsr #20			// addr & 0xfff
	rsb		r0, r3, #4096			// number of bytes to end of page
	mov		r0, r0, lsr #5			// number of lines to end of page
	cmp		r0, r1
	movgt	r0, r1

	cpsid	i
	mcr		p15, 0, r2, c7, c8, 0	// VA2PA for privileged read access
	mrc		p15, 0, r2, c7, c4, 0	// get physical address/attributes
	tst		r2, #1
	bne		1f						// lookup failed - bail out

	bic		r2, r2, #0xff0
	bic		r2, r2, #0x00f			// mask off attribute bits
	orr		r2, r2, r3				// add page offset of addr

	/*
	 * Acquire spin lock to access L2CC
	 */
	mov		r3, #1
0:	ldrex	r1, [ip]
	teq		r1, #0
	wfene
	strexeq	r1, r3, [ip]
	teqeq	r1, #0
	bne		0b
	dmb

	/*
	 * Clean and invalidate lines:
	 * r0 contains number of lines to flush
	 * r2 contains paddr
	 * r4 contains vaddr
	 */
	mov		r1, r0
	ldr		r3, [ip, #OFF_L2CC]		// L2CC register base
0:	str		r2, [r3, #0x7f0]		// clean/inval line by PA
	mcr		p15, 0, r4, c7, c14, 1	// DCCIMVAC (broadcast)
	add		r2, r2, #32
	add		r4, r4, #32
	subs	r1, r1, #1
	bne		0b

	str		r1, [r3, #0x730]		// cache sync
0:	ldr		r1, [r3, #0x730]
	teq		r1, #0
	bne		0b

	/*
	 * Clear spin lock
	 */
	dmb
	str		r1, [ip]
	dsb
	sev
1:	cpsie	i
	ldmia	sp!,{r4,pc}	
CALLOUT_END(cache_pl310)
