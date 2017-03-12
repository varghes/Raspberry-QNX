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
 * Samsung S3C2410 external interrupt callouts.
 *
 * interrupt_id_* and interrupt_eoi_* are copied and intermixed with other
 * kernel code during initialisation.
 *
 * They do not follow normal calling conventions, and must fall through
 * to the end, rather than attempting to perform a return instruction.
 *
 * The INTR_GENFLAG_* bits in the intrinfo_entry defines which of the
 * following values can be loaded on entry to these code fragments:
 *
 *	r5 - holds the syspageptr				(INTR_GENFLAG_SYSPAGE  set)
 *	r6 - holds the intrinfo_entry pointer	(INTR_GENFLAG_INTRINFO set)
 *	r7 - holds the interrupt mask count		(INTR_GENFLAG_INTRMASK set)
 *
 * The interrupt_id_* routine returns the (controller-relative) level in r4
 */

#include "callout.ah"
#include <arm/s3c2410.h>

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */
interrupt_patch:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #8	// size of interrupt registers
	ldr		r1, Lpaddr
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

Lpaddr:	.word	S3C2410_EINTR_BASE

/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_s3c2410_ext_4_7, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000


	mov		r4, #-1

    /*
     * Read pending IRQ interrupts - we're only interested in external IRQs 4 - 7
     */
    ldr     r1, [ip, #S3C2410_EINTPND]
	ldr		r2,	[ip, #S3C2410_EINTMSK]
	bic		r1, r1, r2	/* first, clear any bits that are masked */
	and		r1, r1, #0xf0	/* now, clear all bits except 4 - 7 */

	cmp		r1, #0
	beq		1f

    mov     r4, #4
    mov     r2, #0x10

    /*
     * Scan for first set bit
     */
0:  subs    r4, r4, #1
    blt     1f
    tst     r1, r2, lsl r4
    beq     0b

    /*
     * Mask the interrupt
     */
    ldr     r1, [ip, #S3C2410_EINTMSK]
    orr     r1, r1, r2, lsl r4
    str     r1, [ip, #S3C2410_EINTMSK]

	/*
	 * Clear the interrupt
	 */
	mov		r2, r2, lsl r4
    str     r2, [ip, #S3C2410_EINTPND]
1:
CALLOUT_END(interrupt_id_s3c2410_ext_4_7)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_s3c2410_ext_4_7, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    /*
     * Only unmask interrupt if mask count is zero
     */
    teq     r7, #0
    bne     0f

    mov     r2, #0x10

    ldr     r1, [ip, #S3C2410_EINTMSK]
    mov     r2, r2, lsl r4
	bic		r1, r1, r2
    str     r1, [ip, #S3C2410_EINTMSK]

0:
CALLOUT_END(interrupt_eoi_s3c2410_ext_4_7)


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
CALLOUT_START(interrupt_mask_s3c2410_ext_4_7, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #0x10

    /*
     * Mask the interrupt
     */
    ldr     r3, [ip, #S3C2410_EINTMSK]
    orr     r3, r3, r2, lsl r1
    str     r3, [ip, #S3C2410_EINTMSK]
    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_s3c2410_ext_4_7)


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
CALLOUT_START(interrupt_unmask_s3c2410_ext_4_7, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #0x10

    /*
     * Unmask the interrupt
     */
    ldr     r3, [ip, #S3C2410_EINTMSK]
	mov		r2, r2, lsl r1
    bic     r3, r3, r2
    str     r3, [ip, #S3C2410_EINTMSK]
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_s3c2410_ext_4_7)

/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_s3c2410_ext_8_23, 0, interrupt_patch) 
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r4, #-1

    /*
     * Read pending IRQ interrupts - we're only interested in external IRQs 8 - 23
     */
    ldr     r1, [ip, #S3C2410_EINTPND]
	ldr		r2,	[ip, #S3C2410_EINTMSK]
	bic		r1, r1, r2	/* first, clear any bits that are masked */
	bic		r1, r1, #0xff	/* now, clear all bits except 8 - 23 */

	cmp		r1, #0
	beq		1f

    mov     r4, #16
    mov     r2, #0x100

    /*
     * Scan for first set bit
     */
0:  subs    r4, r4, #1
    blt     1f
    tst     r1, r2, lsl r4
    beq     0b

    /*
     * Mask the interrupt
     */
    ldr     r1, [ip, #S3C2410_EINTMSK]
    orr     r1, r1, r2, lsl r4
    str     r1, [ip, #S3C2410_EINTMSK]

	/*
	 * Clear the interrupt
	 */
	mov		r2, r2, lsl r4
    str     r2, [ip, #S3C2410_EINTPND]
1:
CALLOUT_END(interrupt_id_s3c2410_ext_8_23)

/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_s3c2410_ext_8_23, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    /*
     * Only unmask interrupt if mask count is zero
     */
    teq     r7, #0
    bne     0f

    mov     r2, #0x100

    ldr     r1, [ip, #S3C2410_EINTMSK]
    mov     r2, r2, lsl r4
	bic		r1, r1, r2
    str     r1, [ip, #S3C2410_EINTMSK]

0:
CALLOUT_END(interrupt_eoi_s3c2410_ext_8_23)


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
CALLOUT_START(interrupt_mask_s3c2410_ext_8_23, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #0x100

    /*
     * Mask the interrupt
     */
    ldr     r3, [ip, #S3C2410_EINTMSK]
    orr     r3, r3, r2, lsl r1
    str     r3, [ip, #S3C2410_EINTMSK]
    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_s3c2410_ext_8_23)


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
CALLOUT_START(interrupt_unmask_s3c2410_ext_8_23, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #0x100

    /*
     * Unmask the interrupt
     */
    ldr     r3, [ip, #S3C2410_EINTMSK]
	mov		r2, r2, lsl r1
    bic     r3, r3, r2
    str     r3, [ip, #S3C2410_EINTMSK]
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_s3c2410_ext_8_23)


