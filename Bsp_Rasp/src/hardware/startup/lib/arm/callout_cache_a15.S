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

/*
 * Cortex A15 MPCore specific cache operations
 *
 * unsigned control(paddr32_t base,
 *					unsigned num_lines,
 *					int flags,
 *					struct cacheattr_entry *cache,
 *					volatile struct syspage_entry * )
 */

#include "callout.ah"

#define	MAX_LINES	32
#define	ILINE_SIZE	32		// FIXME: should really check h/w configuration
#define ILINE_LIMIT	1024

CALLOUT_START(cache_a15_i, 0, 0)
	/*
	 * For large ranges, just flush the whole cache
	 */
	cmp		r1, #ILINE_LIMIT
	blo		0f
	mov		r0, #0
	mcr		p15, 0, r0, c7, c1, 0		// ICIALLUIS (broadcast)
	dsb	
	isb
	mcr		p15, 0, r1, c7, c1, 6		// BPIALLIS  (broadcast)
	dsb	
	isb
	mov		pc, lr

	/*
	 * Trim the address to a cache line boundary and stop at 32 lines
	 * to avoid having to re-issue the whole flush if we get preempted
	 *
	 * FIXME: in order to support big/little configurations, the cache type
	 *        register defines a minimum line size that can be either 32 or
	 *        64 bytes. For now just hardcode the minimum to 32 bytes.
	 */
0:	bic		r3, r0, #(ILINE_SIZE-1)
	cmp		r1, #MAX_LINES
	movhi	r1, #MAX_LINES
	mov		r0, r1
0:	mcr		p15, 0, r3, c7, c5, 1		// ICIMVAU (broadcast)
	add		r3, r3, #ILINE_SIZE
	subs	r1, r1, #1
	bne		0b
	dsb	
	isb
	mcr		p15, 0, r1, c7, c1, 6		// BPIALLIS  (broadcast)
	dsb	
	isb
	mov		pc, lr
CALLOUT_END(cache_a15_i)

#define	DLINE_SIZE	64

CALLOUT_START(cache_a15_d, 0, 0)
	/*
	 * Trim the address to a cache line boundary, and stop at 32 lines
	 * to avoid having to re-issue the whole flush if we get preempted
	 */
	bic		r3, r0, #(DLINE_SIZE-1)
	cmp		r1, #MAX_LINES
	movhi	r1, #MAX_LINES
	mov		r0, r1

	tst		r2, #MS_INVALIDATE
	bne		1f

	/*
	 * Clean lines by address
	 */
0:	mcr		p15, 0, r3, c7, c10, 1		// DCCMVAC (broadcast)
	add		r3, r3, #DLINE_SIZE
	subs	r1, r1, #1
	bne		0b
	dsb	
	isb
	mov		pc, lr

	/*
	 * Clean and invalidate lines by address
	 */
1:	mcr		p15, 0, r3, c7, c14, 1		// DCCIMVAC (broadcast)
	add		r3, r3, #DLINE_SIZE
	subs	r1, r1, #1
	bne		1b
	dsb
	isb
	mov		pc, lr
CALLOUT_END(cache_a15_d)
