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
/*
 * TI OMAP 1510 / 5910 / 1610 / 5912 specific interrupt callouts.
 *
 * interrupt_id_* and interrupt_eoi_* are copied and intermixed with other
 * kernel code during initialisation.
 *
 * They do not follow normal calling conventions, and must fall through
 * to the end, rather than attempting to perform a return instruction.
 *
 * The OMAP_INTR_GENFLAG_* bits in the intrinfo_entry defines which of the
 * following values can be loaded on entry to these code fragments:
 *
 *	r5 - holds the syspageptr				(OMAP_INTR_GENFLAG_SYSPAGE  set)
 *	r6 - holds the intrinfo_entry pointer	(OMAP_INTR_GENFLAG_INTRINFO set)
 *	r7 - holds the interrupt mask count		(OMAP_INTR_GENFLAG_INTRMASK set)
 *
 * The interrupt_id_* routine returns the (controller-relative) level in r4
 */

#include "callout.ah"
#include <arm/omap.h>

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

interrupt_patch_pri:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #OMAP_INTR_SIZE	// size of interrupt registers
	ldr		r1, Lintr1_base
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}



interrupt_patch_sec:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #OMAP_INTR_SIZE	// size of interrupt registers
	ldr		r1, Lintr2_base
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/*
	 * Patch the callout routine
	 */

	ldmia	sp!,{r4,pc}

/*
 * The primary and secondary OMAP interrupt controllers each have a control register 
 * which requires a bit (NEW_IRQ_AGR) to be written to, after each interrupt has been 
 * cleared and/or masked. Hitting this bit causes the interrupt controller to re-evaluate 
 * its inputs, and determine the next pending interrupt. That interrupt number is then 
 * placed into the INTR_SIR_IRQ_CODE register. If no further interrupts are pending, the 
 * controller will then (and only then) de-assert its output.
 *
 * A problem arises when this setup is combined with the QNX interrupt handling scheme;
 * When an interrupt comes in from a device which is not on the primary controller
 * (i.e. the secondary, or one of the GPIO interrupt controllers), the following 
 * situation occurs:
 *
 * -the core is interrupted
 * -the primary ID callout runs
 * -a cascade vector is identified
 * -because it's a cascade vector, the primary's EOI callout is run
 * -the NEW_IRQ_AGR bit is hit on the primary, and the cascade vector is unmasked;
 *  the secondary controller is still asserting at this point, so the primary
 *  sees another interrupt from the secondary, and puts it in the INTR_SIR_IRQ_CODE
 * -the secondary ID callout runs, and then identifies, clears, and masks the interrupt
 * -the secondary ID callout hits the secondary's NEW_IRQ_AGR bit
 * -the secondary controller de-asserts its output, but the primary has already
 *   got a pending interrupt from the secondary, and won't de-assert its output to
 *  the core until the primary's NEW_IRQ_AGR bit is set. 
 * -the primary ID callout runs again, and the cascade vector from the secondary is 
 *  identified again
 * -the secondary ID callout runs again, but this time, the pending IRQ (determined
 *  from the INTR_SIR_IRQ_CODE register) is already masked, so a glitch (-1) is returned
 *
 * This will have a performance impact, since three extra (and unnecessary) callouts are
 * run every time an interrupt comes in from any controller other than the primary.
 *
 * To work around this, it is necessary for every ID callout (besides the primary) to
 * hit the primary's NEW_IRQ_AGR bit once the interrupt into the secondary / tertiary
 * controller has been cleared and/or masked, and the output to the primary has been
 * de-asserted. This will cause the primary to re-evaluate its inputs _after_ the
 * cascade vectors have been de-asserted, and thus will not re-run the callouts 
 * unnecessarily. For GPIO controllers which cascade into the secondary, their ID
 * callouts will also need to hit NEW_IRQ_AGR on the secondary and primary.
 * None of this will cause interrupts to be missed, since actual interrupts will
 * remain asserted until they are explicitly cleared and/or masked.
 */

interrupt_patch_sec_pri:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #OMAP_INTR_SIZE	// size of interrupt registers
	ldr		r1, Lintr2_base
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	mov		r0, #OMAP_INTR_SIZE	// size of interrupt registers
	ldr		r1, Lintr1_base
	bl		callout_io_map

	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/*
	 * Patch the callout routine
	 */

	ldmia	sp!,{r4,pc}


Lintr1_base:	.word	OMAP_INTR1_BASE 
Lintr2_base:	.word	OMAP_INTR2_BASE 

/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_omap_pri, 0, interrupt_patch_pri)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * get the binary coded source interrupt - reading this register clears
	 * the corresponding bit in the ITR register if it's an edge triggered int
	 */

	ldr		r4, [ip, #OMAP_INTR_SIR_IRQ_CODE]

    mov     r2, #1
    /*
     * Mask the interrupt
     */
    ldr     r1, [ip, #OMAP_INTR_MIR]

	/*
	 * see if level is already masked; if so, return -1
	 * 
	 */
    and     r3, r1, r2, lsl r4
    teq     r3, #0
    beq     0f
    mov     r4, #-1  /* glitch */
    b       1f
0:
    orr     r1, r1, r2, lsl r4
    str     r1, [ip, #OMAP_INTR_MIR]

    /*
     * New IRQ Agreement 
     */

	ldr     r1,[ip, #OMAP_INTR_CONTROL_REG]
	orr     r1, r1, #0x01
	str     r1,[ip, #OMAP_INTR_CONTROL_REG]

1:
CALLOUT_END(interrupt_id_omap_pri)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_omap_pri, 0, interrupt_patch_pri)
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

	mov		r2, #1
	mov		r2, r2, lsl r4	

    ldr     r1, [ip, #OMAP_INTR_MIR]
    bic     r1, r1, r2
    str     r1, [ip, #OMAP_INTR_MIR]

0:
CALLOUT_END(interrupt_eoi_omap_pri)


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
CALLOUT_START(interrupt_mask_omap_pri, 0, interrupt_patch_pri)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #1
    /*
     * Mask the interrupt
     */
    ldr     r0, [ip, #OMAP_INTR_MIR]
    orr     r0, r0, r2, lsl r1
    str     r0, [ip, #OMAP_INTR_MIR]

    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_omap_pri)


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
CALLOUT_START(interrupt_unmask_omap_pri, 0, interrupt_patch_pri)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #1

    /*
     * Unmask the interrupt
     */
    ldr     r0, [ip, #OMAP_INTR_MIR]
    mov     r2, r2, lsl r1
    bic     r0, r0, r2
    str     r0, [ip, #OMAP_INTR_MIR]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_omap_pri)




/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_omap_sec, 0, interrupt_patch_sec_pri)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* patch in primary interrupt controller base */
	mov		r0,     #0x000000ff
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * There are 128 total interrupts in the secondary interrupt controller,
	 * comprised of 4 sections of 32 bits each. The SIR_IRQ register is not 
	 * duplicated, and will hold the value of the current active interrupt,
	 * from 0 to 127. Reading it will also clear the appropriate ITR bit for
	 * edge triggered interrupts. The 4 sections are duplicated at offsets of 0x100.
	 *
	 * r5 will be the section value (0, 1, 2, or 3) multiplied by 0x100, and added to ip.
	 * r3 will contain the interrupt value adjusted to be within the range 
	 * of 0 - 32, for masking, etc. So, for example, if the interrupt 
	 * value is 103, this corresponds to the seventh bit of the fourth 
	 * section, so r5 will be 0x300, and r3 will be 7.
	 * The section value (0,1,2, or 3) is: ((irq >> 5) & 0x03).
	 * The offset (0 to 31) is: irq - (section * 32)
	 */

	ldr		r4, [ip, #OMAP_INTR_SIR_IRQ_CODE]
	mov		r5, r4, lsr #5
	and 	r5, r5, #3	/* r5 now contains 0, 1, 2, or 3 */
	mov		r3, r5, lsl #5

	subs	r3, r4, r3	/* r3 now contains irq - (section *32) */
	mov		r5, r5, lsl #8

	add		r5, ip, r5

	mov		r2,#1

    /*
     * Mask the interrupt
     */
    ldr     r1, [r5, #OMAP_INTR_MIR]

	/*
	 * see if level is already masked; if so, return -1
	 * 
	 */
    and     r2, r1, r2, lsl r3
    teq     r2, #0
    beq     0f
    mov     r4, #-1  /* glitch */
    b       1f
0:
	mov		r2, #1
    orr     r1, r1, r2, lsl r3
    str     r1, [r5, #OMAP_INTR_MIR]


    /*
     * New IRQ Agreement on secondary
     */
    ldr     r1,[ip, #OMAP_INTR_CONTROL_REG]
    orr     r1, r1, #0x01
    str     r1,[ip, #OMAP_INTR_CONTROL_REG]

	/*
	 * New IRQ Agreement on primary
	 */
    ldr     r1,[r0, #OMAP_INTR_CONTROL_REG]
    orr     r1, r1, #0x01
    str     r1,[r0, #OMAP_INTR_CONTROL_REG]

1:
CALLOUT_END(interrupt_id_omap_sec)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_omap_sec, 0, interrupt_patch_sec)
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

	/*
	 * See id_omap_sec callout for an explanation of code
	 */
	mov		r5, r4, lsr #5
	and 	r5, r5, #3
	mov		r3, r5, lsl #5
	sub		r3, r4, r3
	mov		r5, r5, lsl #8
	add		ip, r5, ip

	mov		r2, #1
	mov		r2, r2, lsl r3	

    ldr     r1, [ip, #OMAP_INTR_MIR]
    bic     r1, r1, r2
    str     r1, [ip, #OMAP_INTR_MIR]
0:
CALLOUT_END(interrupt_eoi_omap_sec)


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
CALLOUT_START(interrupt_mask_omap_sec, 0, interrupt_patch_sec)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * See id_omap_sec callout for an explanation of code
	 */
	mov		r0, r1, lsr #5
	and 	r0, r0, #3
	mov		r3, r0, lsl #5
	sub		r3, r1, r3
	mov		r0, r0, lsl #8
	add		ip, r0, ip
    mov     r2, #1

    /*
     * Mask the interrupt
     */
    ldr     r0, [ip, #OMAP_INTR_MIR]
    orr     r0, r0, r2, lsl r3
    str     r0, [ip, #OMAP_INTR_MIR]

    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_omap_sec)


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
CALLOUT_START(interrupt_unmask_omap_sec, 0, interrupt_patch_sec)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * See id_omap_sec callout for an explanation of code
	 */
	mov		r0, r1, lsr #5
	and 	r0, r0, #3
	mov		r3, r0, lsl #5
	sub		r3, r1, r3
	mov		r0, r0, lsl #8
	add		ip, r0, ip
    mov     r2, #1

    /*
     * Unmask the interrupt
     */

    ldr     r0, [ip, #OMAP_INTR_MIR]
    mov     r2, r2, lsl r3
    bic     r0, r0, r2
    str     r0, [ip, #OMAP_INTR_MIR]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_omap_sec)

