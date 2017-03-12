/*
 * $QNXLicenseC:
 * Copyright 2008, QNX Software Systems.
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
 * iMX3x L2 cache operations
 *
 * FIXME: for now we simply clean/invalidate all cache ways.
 *        Since the cache is physically indexed/tagged, the kernel code
 *        to flush the cache requires performing virtual to physical
 *        address lookups which are likely to be more expensive than just
 *        doing the whole 128K cache.
 *
 * unsigned control(paddr32_t base,
 *					unsigned num_lines,
 *					int flags,
 *					struct cacheattr_entry *cache,
 *					volatile struct syspage_entry * )
 */

#include "callout.ah"

patcher:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2				// address of callout routine

	mov		r1, #0x30000000			// L2 controller base address
	mov		r0, #0x1000				// 4K register size
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

CALLOUT_START(cache_imx3x_l2, 0, patcher)
	mov		ip,     #0x000000ff		// L2 controller base (patched)
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	tst		r2, #MS_INVALIDATE
#ifdef	FIXME
	bne		1f
	mov		r0, #0xff
	str		r0, [ip, #0x7bc]		// clean by way
0:	ldr		r0, [ip, #0x7bc]		// poll for completion
	teq		r0, #0
	bne		0b
	mov		pc, lr
#else
	/*
	 * FIXME: we assume cache is configured for write-through so we only
	 *        touch the cache if MS_INVALIDATE is set.
	 */
	moveq	r0, #0
	moveq	pc, lr
#endif

1:	mov		r0, #0xff				// affect all cache ways
	str		r0, [ip, #0x7fc]		// clean+inval by way
0:	ldr		r0, [ip, #0x7fc]		// poll for completion
	teq		r0, #0
	bne		0b
	mov		pc, lr
CALLOUT_END(cache_imx3x_l2)
