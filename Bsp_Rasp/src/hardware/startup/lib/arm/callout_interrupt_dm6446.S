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

#define RINGIO_EVENT_CHART_SIZE 	0x10
#define DSP_SYSOFF_SIZE 			0x8
#define INTGEN_REG					0
#define RINGIO_INTERRUPT_CLEAR		0xefff
#define RINGIO_EVENT_CHART_PAYLOAD	0x4
#define RINGIO_EVENT_CHART_FLAG		0
#define DSP_SYSOFF_ADDRESS			0x01c40010
#define RINGIO_ID_MASK				0x0000ffff
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

interrupt_patch:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #0x20	// size to map
	ldr		r1, [sp, #8]
	ldr		r1, [r1]
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

interrupt_patch_ringio_ints:
	stmdb	sp!, {r4,lr}
	add		r4, r0, r2

	mov		r0, #RINGIO_EVENT_CHART_SIZE
    ldr     r1, [sp, #8]
    ldr     r1, [r1]
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	mov		r0, #DSP_SYSOFF_SIZE
	ldr		r1, ldsp_sysoff_address
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

ldsp_sysoff_address:	.word	DSP_SYSOFF_ADDRESS
/* -----------------------------------------------------------------------
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_dm6446, 0, interrupt_patch)
	/*
	 * Get the GPIO interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	ldr		r5, [ip, #DM6446_INTR_EINT1]
	ldr		r4, [ip, #DM6446_INTR_IRQ1]
	bic		r1, r5, r4

	cmp		r1, #0	
	beq		1f

	mov		r4, #32
    mov     r2, #1

0:  sub     r4, r4, #1
    tst     r1, r2, lsl r4
    beq     0b

    /*
     * Mask the interrupt
     */

    mov     r2, r2, lsl r4
	bic		r5, r5, r2

    str     r5, [ip, #DM6446_INTR_EINT1]

	/*
	 * clear the interrupt
	 */

	add		r4, r4, #DM6446_INTR_IRQ32
	str 	r2, [ip, #DM6446_INTR_IRQ1]
	b	3f

1:
	ldr     r5, [ip, #DM6446_INTR_EINT0]
	ldr     r4, [ip, #DM6446_INTR_IRQ0]
	bic     r1, r5, r4

	cmp     r1, #0
	bne		2f
	mov		r4, #-1
	b		3f

2:
	mov		r4, #32
    mov     r2, #1

0:  sub     r4, r4, #1
    tst     r1, r2, lsl r4
    beq     0b

    /*
     * Mask the interrupt
     */

    mov     r2, r2, lsl r4
	bic		r5, r5, r2

    str     r5, [ip, #DM6446_INTR_EINT0]

	/*
	 * clear the interrupt
	 */
	str 	r2, [ip, #DM6446_INTR_IRQ0]
3:
CALLOUT_END(interrupt_id_dm6446)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_dm6446, 0, interrupt_patch)
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
    bne     3f

	cmp		r4, #DM6446_INTR_IRQ32
	bge		2f

	mov		r2, #1

	/*
	 * unmask interrupt
	 */

    ldr     r1, [ip, #DM6446_INTR_EINT0]
    orr     r1, r1, r2, lsl r4

    str     r1, [ip, #DM6446_INTR_EINT0]
	b		3f

2:

	mov		r2, #1
	sub		r4, r4, #DM6446_INTR_IRQ32

	/*
	 * unmask interrupt
	 */

    ldr     r1, [ip, #DM6446_INTR_EINT1]
    orr     r1, r1, r2, lsl r4

    str     r1, [ip, #DM6446_INTR_EINT1]

3:
CALLOUT_END(interrupt_eoi_dm6446)


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
CALLOUT_START(interrupt_mask_dm6446, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000


	cmp		r1, #DM6446_INTR_IRQ32
	bge		2f

    mov     r2, #1
    mov     r2, r2, lsl r1
    /*
     * Mask the interrupt
     */
    ldr     r0, [ip, #DM6446_INTR_EINT0]
    bic     r0, r0, r2

    str     r0, [ip, #DM6446_INTR_EINT0]
	b		3f

2:
    mov     r2, #1
	sub		r1, r1, #DM6446_INTR_IRQ32
    mov     r2, r2, lsl r1
    /*
     * Mask the interrupt
     */
    ldr     r0, [ip, #DM6446_INTR_EINT1]
    bic     r0, r0, r2

    str     r0, [ip, #DM6446_INTR_EINT1]

3:
    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_dm6446)


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
CALLOUT_START(interrupt_unmask_dm6446, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000


	cmp		r1, #DM6446_INTR_IRQ32
	bge		2f

    mov     r2, #1

    /*
     * Unmask the interrupt
     */
    ldr     r0, [ip, #DM6446_INTR_EINT0]
    orr     r0, r0, r2, lsl r1

    str     r0, [ip, #DM6446_INTR_EINT0]
	b		3f

2:
    mov     r2, #1
	sub		r1, r1, #DM6446_INTR_IRQ32

    /*
     * Unmask the interrupt
     */
    ldr     r0, [ip, #DM6446_INTR_EINT1]
    orr     r0, r0, r2, lsl r1

    str     r0, [ip, #DM6446_INTR_EINT1]

3:
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_dm6446)

/* -----------------------------------------------------------------------
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_ringio_ints, 0, interrupt_patch_ringio_ints)
    /*
     * Get the Event Chart base address (patched)
     */
    mov     ip,     #0x000000ff
    orr     ip, ip, #0x0000ff00
    orr     ip, ip, #0x00ff0000
    orr     ip, ip, #0xff000000

	/*
	 * Get the DSP INTGEN Register
	 */
    mov     r0,     #0x000000ff
    orr     r0, r0, #0x0000ff00
    orr     r0, r0, #0x00ff0000
    orr     r0, r0, #0xff000000

	/*
	 * Clear DSP Interrupt Status
	 */
	mov		r5, #1
	mov		r1, #28
	mov		r5, r5, lsl r1
	ldr     r2, [r0, #INTGEN_REG]
	bic		r2, r2, r5

	str		r2, [r0, #INTGEN_REG]

	/* 
	 * Read event chart payload
	 */
	ldr     r2, [ip, #RINGIO_EVENT_CHART_PAYLOAD]
	and		r1, r2, #0xFF
	and		r3, r2, #0xFF00
	orr		r4, r1, r3
CALLOUT_END(interrupt_id_ringio_ints)

/* RINGIO INTERRUPTS simply used as NOTIFICATION from Dsplink to a user
 * application
 */
CALLOUT_START(interrupt_eoi_ringio_ints, 0, 0)
CALLOUT_END(interrupt_eoi_ringio_ints)

CALLOUT_START(interrupt_mask_ringio_ints, 0, 0)
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_ringio_ints)

CALLOUT_START(interrupt_unmask_ringio_ints, 0, 0)
    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_unmask_ringio_ints)

