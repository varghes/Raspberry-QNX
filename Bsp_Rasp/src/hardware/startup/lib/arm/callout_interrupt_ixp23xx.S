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
 * Intel IXP23XX specific interrupt callouts
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
 *	r8 - holds INTRLEVEL pointer
 *
 * The interrupt_id_* routine returns the (controller-relative) level in r4
 */

#include "callout.ah"
#include <arm/ixp23xx.h>

/*
 * --------------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * --------------------------------------------------------------------------
 */


/*
 * Patch callouts with IXP23XX_IRQ_BASE
 */
patch_intr:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map registers
	 */

	mov		r0, #IXP23XX_INTR_SIZE + IXP23XX_GPIO_SIZE
	ldr		r1, Lintr_base
	bl		callout_io_map


	CALLOUT_PATCH	r4, r0, r1, r2, ip


	ldmia	sp!,{r4,pc}

Lintr_base:	.word	IXP23XX_INTR_BASE

CALLOUT_START(interrupt_id_ixp23xx, 0, patch_intr)
	/*
	 *  Get the address of the interrupt registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * read encoded status register 
	 */

	mov		r4, #-1

	ldr		r1, [ip, #IXP23XX_INTR_IRQ_ENC_ST]
	teq		r1, #0
	beq		0f		/* glitch */

	mov		r2, #2	/* encoded value is shifted right by 2 bits */
	mov		r4, r1, lsr r2
	subs	r4, r4, #1	/* encoded value is one higher then actual; 0 == glitch */

	/*
	 * Mask the interrupt. r4 contains the normalized vector. 
	 */

	mov		r2, #1
	cmp		r4, #31
	bgt		3f

	ldr		r1, [ip, #IXP23XX_INTR_EN1]
	bic		r1, r1, r2, lsl r4
	str		r1, [ip, #IXP23XX_INTR_EN1]
	b		0f
3:
	cmp		r4, #55
	bgt		2f

	sub		r3, r4, #32
	ldr		r1, [ip, #IXP23XX_INTR_EN2]
	bic		r1, r1, r2, lsl r3
	str		r1, [ip, #IXP23XX_INTR_EN2]
	b		0f
2:
	cmp		r4, #87
	bgt		1f

	sub		r3, r4, #56
	ldr		r1, [ip, #IXP23XX_INTR_EN3]
	bic		r1, r1, r2, lsl r3
	str		r1, [ip, #IXP23XX_INTR_EN3]
	/* Thread A interrupts must be cleared */
	mov		r2, r2, lsl r3
	str		r2, [ip, #IXP23XX_INTR_ST3]
	b		0f
1:
	sub		r3, r4, #88
	ldr		r1, [ip, #IXP23XX_INTR_EN4]
	bic		r1, r1, r2, lsl r3
	str		r1, [ip, #IXP23XX_INTR_EN4]
	/* Thread B interrupts must be cleared */
	mov		r2, r2, lsl r3
	str		r2, [ip, #IXP23XX_INTR_ST4]
0:
CALLOUT_END(interrupt_id_ixp23xx)


CALLOUT_START(interrupt_eoi_ixp23xx, 0, patch_intr)

	/*
	 * Get the address of the interrupt registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * the following interrupts must be cleared; gas_int_0 (int 20), GPIO
	 * interrupts 6 - 15 (ints 34 - 43), and Thread A and B interrupts (ints
	 * 56 - 119). The GPIO interrupts are not cleared via the interrupt controller
	 * registers, but rather, within the GPIO Interrupt status register. 
	 */

	mov		r2, #1

	/* check for gas_int_0 */
	cmp		r4, #20
	bne		3f

	mov		r1, r2, lsl r4
	str		r1, [ip, #IXP23XX_INTR_ST1]
	b		0f

3:
	/* check for GPIO 6 - 15 interrupt */
	cmp		r4, #43
	bgt		2f
	cmp		r4, #34
	blt		0f

	/* clear GPIO interrupt via GPISR register */
	sub		r3, r4, #28
	mov		r1, r2, lsl r3
	str		r1, [ip, #IXP23XX_GPISR]
	b		0f

2:
	cmp		r4, #56
	blt		0f
	cmp		r4, #87
	bgt		1f

	sub		r3, r4, #56
	mov		r1, r2, lsl r3
	str		r1, [ip, #IXP23XX_INTR_ST3]
	b		0f
1:
	sub		r3, r4, #88
	mov		r1, r2, lsl r3
	str		r1, [ip, #IXP23XX_INTR_ST4]
0:

	/*
	 * Only unmask if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	mov		r2, #1
	cmp		r4, #31
	bgt		3f

	ldr		r1, [ip, #IXP23XX_INTR_EN1]
	orr		r1, r1, r2, lsl r4
	str		r1, [ip, #IXP23XX_INTR_EN1]
	b		0f
3:
	cmp		r4, #55
	bgt		2f

	sub		r3, r4, #32
	ldr		r1, [ip, #IXP23XX_INTR_EN2]
	orr		r1, r1, r2, lsl r3
	str		r1, [ip, #IXP23XX_INTR_EN2]
	b		0f
2:
	cmp		r4, #87
	bgt		1f

	sub		r3, r4, #56
	ldr		r1, [ip, #IXP23XX_INTR_EN3]
	orr		r1, r1, r2, lsl r3
	str		r1, [ip, #IXP23XX_INTR_EN3]
	b		0f
1:
	sub		r3, r4, #88
	ldr		r1, [ip, #IXP23XX_INTR_EN4]
	orr		r1, r1, r2, lsl r3
	str		r1, [ip, #IXP23XX_INTR_EN4]
0:
CALLOUT_END(interrupt_eoi_ixp23xx)


/*
 * error = interrupt_mask_ixp23xx(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_ixp23xx, 0, patch_intr)
	/*
	 * Get the address of the interrupt registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r2, #1
	cmp		r1, #31
	bgt		3f

	ldr		r0, [ip, #IXP23XX_INTR_EN1]
	bic		r0, r0, r2, lsl r1
	str		r0, [ip, #IXP23XX_INTR_EN1]
	b		0f
3:
	cmp		r1, #55
	bgt		2f

	sub		r3, r1, #32
	ldr		r0, [ip, #IXP23XX_INTR_EN2]
	bic		r0, r0, r2, lsl r3
	str		r0, [ip, #IXP23XX_INTR_EN2]
	b		0f
2:
	cmp		r1, #87
	bgt		1f

	sub		r3, r1, #56
	ldr		r0, [ip, #IXP23XX_INTR_EN3]
	bic		r0, r0, r2, lsl r3
	str		r0, [ip, #IXP23XX_INTR_EN3]
	b		0f
1:
	sub		r3, r1, #88
	ldr		r0, [ip, #IXP23XX_INTR_EN4]
	bic		r0, r0, r2, lsl r3
	str		r0, [ip, #IXP23XX_INTR_EN4]
0:

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_ixp23xx)


/*
 * error = interrupt_unmask_ixp23xx(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_ixp23xx, 0, patch_intr)
	/*
	 * Get the address of the interrupt registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r2, #1
	cmp		r1, #31
	bgt		3f

	ldr		r0, [ip, #IXP23XX_INTR_EN1]
	orr		r0, r0, r2, lsl r1
	str		r0, [ip, #IXP23XX_INTR_EN1]
	b		0f
3:
	cmp		r1, #55
	bgt		2f

	sub		r3, r1, #32
	ldr		r0, [ip, #IXP23XX_INTR_EN2]
	orr		r0, r0, r2, lsl r3
	str		r0, [ip, #IXP23XX_INTR_EN2]
	b		0f
2:
	cmp		r1, #87
	bgt		1f

	sub		r3, r1, #56
	ldr		r0, [ip, #IXP23XX_INTR_EN3]
	orr		r0, r0, r2, lsl r3
	str		r0, [ip, #IXP23XX_INTR_EN3]
	b		0f
1:
	sub		r3, r1, #88
	ldr		r0, [ip, #IXP23XX_INTR_EN4]
	orr		r0, r0, r2, lsl r3
	str		r0, [ip, #IXP23XX_INTR_EN4]
0:
	
	mov		r0,#0
	mov		pc, lr

CALLOUT_END(interrupt_unmask_ixp23xx)

