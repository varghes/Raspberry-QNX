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
#
# $QNXLicenseA$
#

/*
 * Marvell Sheeva specific cache operations
 *
 * unsigned control(paddr32_t base,
 *					unsigned num_lines,
 *					int flags,
 *					struct cacheattr_entry *cache,
 *					volatile struct syspage_entry * )
 */

#include "callout.ah"

#define	MAX_LINES	32
#define	LINE_SIZE	32
#define LINE_LIMIT	1024

CALLOUT_START(cache_sheeva_i_v6, 0, 0)
	/*
	 * For large range just flush the whole cache
	 */
	cmp		r1, #LINE_LIMIT
	movhs	r0, #0
	mcrhs	p15, 0, r0, c7, c5, 0			// invalidate whole icache
	movhs	pc, lr

	/*
	 * Trim the address to a cache line boundary, and stop at 32 lines
	 * to avoid having to re-issue the whole flush if we get preempted
	 */
	bic		r3, r0, #0x1f
	cmp		r1, #MAX_LINES
	movhi	r1, #MAX_LINES
	mov		r0, r1

	/*
	 * Invalidate lines by address
	 */
0:	mcr		p15, 0, r3, c7, c5, 1			// inval L1 line
	add		r3, r3, #LINE_SIZE
	subs	r1, r1, #1
	bne		0b

	/*
	 * Flush BTB
	 */
	mcr		p15, 0, r0, c7, c5, 6
	mov		pc, lr
CALLOUT_END(cache_sheeva_i_v6)

CALLOUT_START(cache_sheeva_d_v6, 0, 0)
	/*
	 * For large ranges just operate on the whole cache
	 */
	cmp		r1, #LINE_LIMIT
	blo		0f
	tst		r2, #MS_INVALIDATE
	mov		r0, #0
	mcreq	p15, 0, r0, c7, c10, 0		// clean entire dcache
	mcrne	p15, 0, r0, c7, c14, 0		// clean/inval entire dcache
	mcr		p15, 1, r0, c7, c11, 0		// clean L2
	mcrne	p15, 1, r0, c7, c7, 0		// inval L2
	mcr		p15, 0, r0, c7, c10, 4		// DSB/drain write buffer
	mov		pc, lr

0:
	/*
	 * Trim the address to a cache line boundary, and stop at 32 lines
	 * to avoid having to re-issue the whole flush if we get preempted
	 */
	bic		r3, r0, #0x1f
	cmp		r1, #MAX_LINES
	movhi	r1, #MAX_LINES
	mov		r0, r1

	tst		r2, #MS_INVALIDATE
	bne		1f

	/*
	 * Clean lines by address
	 */
0:	mcr		p15, 0, r3, c7, c10, 1			// clean L1 line
	mcr		p15, 1, r3, c7, c11, 1			// clean L2 line
	add		r3, r3, #LINE_SIZE
	subs	r1, r1, #1
	bne		0b
	mcr		p15, 0, r1, c7, c10, 4			// DSB/drain write buffer
	mov		pc, lr

	/*
	 * Clean and invalidate lines by address
	 */
1:	mcr		p15, 0, r3, c7, c14, 1			// clean/inval L1 line
	mcr		p15, 1, r3, c7, c15, 1			// clean/inval L2 line
	add		r3, r3, #LINE_SIZE
	subs	r1, r1, #1
	bne		1b
	mcr		p15, 0, r1, c7, c10, 4			// DSB/drain write buffer
	mov		pc, lr
CALLOUT_END(cache_sheeva_d_v6)
