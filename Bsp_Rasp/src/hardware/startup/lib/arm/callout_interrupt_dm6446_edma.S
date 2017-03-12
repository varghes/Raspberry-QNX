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
#include <arm/dm6446.h>

/*
 * -----------------------------------------------------------------------
 * Routines to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */
#define IER_OFFSET		0x00
#define IERH_OFFSET		0x04
#define IECR_OFFSET		0x08
#define IECRH_OFFSET	0x0C
#define IESR_OFFSET		0x10
#define IESRH_OFFSET	0x14
#define IPR_OFFSET		0x18
#define IPRH_OFFSET		0x1C
#define ICR_OFFSET		0x20
#define ICRH_OFFSET		0x24
#define IEVAL_OFFSET	0x28

dma_interrupt_patch:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #0x30	// size to map
	ldr		r1, [sp, #8]
	ldr		r1, [r1]
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

/* -----------------------------------------------------------------------
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_dma_ints, 0, dma_interrupt_patch)

	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* Check for HIGH / LOW Channel */
	ldr		r5, [ip, #IPR_OFFSET]
	cmp		r5, #0
	beq		1f

	mov		r4, #32
	mov		r2, #1
0:	subs	r4, r4, #1
	blt 	2f
	tst		r5, r2, lsl r4
	beq		0b

	mov		r2, r2, lsl r4
	str		r2, [ip, #IECR_OFFSET]
	str		r2, [ip, #ICR_OFFSET]
	b		2f

1:
	ldr		r5, [ip, #IPRH_OFFSET]
    mov     r4, #32
    mov     r2, #1
0:  subs    r4, r4, #1
    blt     2f
    tst     r5, r2, lsl r4
    beq     0b

    mov     r2, r2, lsl r4
    str     r2, [ip, #IECRH_OFFSET]
    str     r2, [ip, #ICRH_OFFSET]

	add		r4, r4, #32

2:
CALLOUT_END(interrupt_id_dma_ints)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_dma_ints, 0, dma_interrupt_patch)

	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    /*
     * Only unmask interrupt if mask count is zero
     */

    teq     r7, #0
    bne     2f

	cmp		r4, #32
	bge		1f
	
	mov		r2, #1
    mov     r2, r2, lsl r4

	/*
	 * unmask interrupt
	 */

	str		r2, [ip, #IESR_OFFSET]
	b		2f

1:
	mov		r1, r4
	subs	r1, r1, #32

	mov		r2, #1
	mov		r2, r2, lsl r1

	str		r2, [ip, #IESRH_OFFSET]

2:
	mov 	r2, #1
	str		r2, [ip, #IEVAL_OFFSET]
CALLOUT_END(interrupt_eoi_dma_ints)


/*
 * -----------------------------------------------------------------------
 * Mask specified interrupt
 *
 * On entry:
 *	r0 - syspage_ptr
 *	r1 - interrupt number
 *
 * Returns:
 *	r0 - error status
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_mask_dma_ints, 0, dma_interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	cmp		r1, #32
	bge		1f

    mov     r2, #1
    mov     r2, r2, lsl r1

	/*
	 * mask interrupt
	 */

	str		r2, [ip, #IECR_OFFSET]
	b		2f

1:
	mov		r4, r1
	subs	r4, r4, #32

	mov		r2, #1
	mov		r2, r2, lsl r4

	str		r2, [ip, #IECRH_OFFSET]

2:
    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_dma_ints)


/*
 * -----------------------------------------------------------------------
 * Unmask specified interrupt
 *
 * On entry:
 *	r0 - syspage_ptr
 *	r1 - interrupt number
 *
 * Returns:
 *	r0 - error status
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_unmask_dma_ints, 0, dma_interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	cmp		r1, #32
	bge		1f

    mov     r2, #1
    mov     r2, r2, lsl r1

	/*
	 * unmask interrupt
	 */

	str		r2, [ip, #IESR_OFFSET]
	b		2f

1:
	mov		r4, r1
	subs	r4, r4, #32

	mov		r2, #1
	mov		r2, r2, lsl r4

	str		r2, [ip, #IESRH_OFFSET]

2:
    mov     r0, #0
    mov     pc, lr


CALLOUT_END(interrupt_unmask_dma_ints)

