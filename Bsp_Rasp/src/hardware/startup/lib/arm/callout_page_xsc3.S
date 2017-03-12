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
 * XSC3 (Xscale core generation 3) specific page flush callouts
 */

#include "callout.ah"

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
CALLOUT_START(page_flush_xsc3, 0, 0)
	mov		r2, r0
	teq		r1, #0
	beq		1f									// only flush TLBs

	mov		r1, #4096							// page size
0:	mcr		p15, 0, r0, c7, c10, 1				// clean Dcache line
	mcr		p15, 1, r0, c7, c11, 1				// clean L2 cache line
	mcr		p15, 0, r0, c7, c5, 1				// inval Icache line
	mcr		p15, 0, r0, c7, c6, 1				// inval Dcache line
	mcr		p15, 1, r0, c7, c7, 1				// inval L2 cache line
	add		r0, r0, #32							// next cache line
	subs	r1, r1, #32
	bne		0b
	mcr		p15, 0, r0, c7, c5, 6				// flush BTB
	mcr		p15, 0, r1, c7, c10, 4				// drain write buffer

1:	mcr		p15, 0, r2, c8, c6, 1				// flush DTLB
	mcr		p15, 0, r2, c8, c5, 1				// flush DTLB

	mov		r0, #0								// flush complete
	mov		pc, lr
CALLOUT_END(page_flush_xsc3)

/*
 * void	page_flush_deferred(int cache)
 *
 * This routine performs any cache/TLB flushing that could not be done in
 * the page_flush() callout.
 *
 * cache is non-zero if the cache needs to be flushed
 */
CALLOUT_START(page_flush_deferred_xsc3, 0, 0)
	mov		pc, lr
CALLOUT_END(page_flush_deferred_xsc3)
