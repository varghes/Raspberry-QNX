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
#
# $QNXLicenseA$
#

/*
 * XSC3 (Xscale core generation 3) specific cache operations
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

CALLOUT_START(cache_xsc3_i, 0, 0)
	/*
	 * Trim the address to a cache line boundary, and stop at 32 lines
	 * to avoid having to re-issue the whole flush if we get preempted
	 */
	bic		r3, r0, #0x1f
	cmp		r1, #MAX_LINES
	movhi	r1, #MAX_LINES
	mov		r0, r1

	/*
	 * Generate MVA for CP15 operation if necessary
	 */
	ands	ip, r3, #0xfe000000
	mrceq	p15, 0, ip, c13, c0, 0
	orreq	r3, r3, ip

	/*
	 * Invalidate lines by address
	 */
0:	mcr		p15, 0, r3, c7, c5, 1			// inval L1 line
	mcr		p15, 1, r3, c7, c7, 1			// inval L2 line
	add		r3, r3, #LINE_SIZE
	subs	r1, r1, #1
	bne		0b

	/*
	 * Flush BTB
	 */
	mcr		p15, 0, r0, c7, c5, 6
	mov		pc, lr
CALLOUT_END(cache_xsc3_i)

CALLOUT_START(cache_xsc3_d, 0, 0)
	/*
	 * Trim the address to a cache line boundary, and stop at 32 lines
	 * to avoid having to re-issue the whole flush if we get preempted
	 */
	bic		r3, r0, #0x1f
	cmp		r1, #MAX_LINES
	movhi	r1, #MAX_LINES
	mov		r0, r1

	/*
	 * Generate MVA for CP15 operation if necessary
	 */
	ands	ip, r3, #0xfe000000
	mrceq	p15, 0, ip, c13, c0, 0
	orreq	r3, r3, ip

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
	/*
	 * Drain write buffer
	 */
	mcr		p15, 0, r1, c7, c10, 4
	mov		pc, lr

	/*
	 * Clean and invalidate lines by address
	 */
1:	mcr		p15, 0, r3, c7, c10, 1			// clean L1 line
	mcr		p15, 1, r3, c7, c11, 1			// clean L2 line
	mcr		p15, 0, r3, c7, c6, 1			// inval L1 line
	mcr		p15, 1, r3, c7, c7, 1			// inval L2 line
	add		r3, r3, #LINE_SIZE
	subs	r1, r1, #1
	bne		1b
	/*
	 * Drain write buffer
	 */
	mcr		p15, 0, r1, c7, c10, 4
	mov		pc, lr
CALLOUT_END(cache_xsc3_d)
