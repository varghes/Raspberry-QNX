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

/*
 * Cortex A9 specific page flush callouts
 */

#include "callout.ah"

#define LINE_SIZE	32

/*
 * int	page_flush(unsigned vaddr, int cache)
 *
 * This routine is called to flush the caches and TLBs for a single page.
 * vaddr. If the processor cannot flush cache lines or TLB entries by address,
 * those caches and TLBs can be flushed in page_flush_deferred()
 *
 * vaddr is the virtual address of the page
 * cache is non-zero if the cache needs to be flushed
 *
 * Returns 0 if all caches/TLBs were flushed.
 *         1 if the page_flush_deferred() callout needs to be called.
 */
CALLOUT_START(page_flush_a9mp, 0, 0)
	mov		r2, r0
	teq		r1, #0
	beq		1f								// only flush TLBs

	bic		r0, r0, #0xff					// kernel sets ASID in vaddr
	mov		r1, #4096						// page size
0:	mcr		p15, 0, r0, c7, c14, 1			// DCCIMVAC (broadcast)
	mcr		p15, 0, r0, c7, c5, 1			// ICIMVAU  (broadcast)
	add		r0, r0, #LINE_SIZE				// next cache line
	subs	r1, r1, #LINE_SIZE
	bne		0b
	dsb
	isb

1:	// Note: kernel sets ASID in bottom 7 bits of address
	mcr		p15, 0, r2, c8, c3, 1			// TLBIMVAIS (broadcast)
	mcr		p15, 0, r1, c7, c1, 6			// BPIALLIS  (broadcast)
	dsb
	isb

	mov		r0, #0							// flush complete
	mov		pc, lr
CALLOUT_END(page_flush_a9mp)

/*
 * void	page_flush_deferred(int cache)
 *
 * This routine performs any cache/TLB flushing that could not be done in
 * the page_flush() callout.
 *
 * cache is non-zero if the cache needs to be flushed
 */
CALLOUT_START(page_flush_deferred_a9mp, 0, 0)
	mov		pc, lr
CALLOUT_END(page_flush_deferred_a9mp)
