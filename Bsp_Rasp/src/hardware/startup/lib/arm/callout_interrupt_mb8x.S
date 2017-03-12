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
 * Fujitsu MB8X specific interrupt callouts.
 *
 * interrupt_id_* and interrupt_eoi_* are copied and intermixed with other
 * kernel code during initialisation.
 *
 * They do not follow normal calling conventions, and must fall through
 * to the end, rather than attempting to perform a return instruction.
 *
 * The MB8X_INTR_GENFLAG_* bits in the intrinfo_entry defines which of the
 * following values can be loaded on entry to these code fragments:
 *
 *	r5 - holds the syspageptr				(MB8X_INTR_GENFLAG_SYSPAGE  set)
 *	r6 - holds the intrinfo_entry pointer	(MB8X_INTR_GENFLAG_INTRINFO set)
 *	r7 - holds the interrupt mask count		(MB8X_INTR_GENFLAG_INTRMASK set)
 *
 * The interrupt_id_* routine returns the (controller-relative) level in r4
 */

#include "callout.ah"
#include <arm/mb8x.h>

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code for IRC
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */
interrupt_patch_irc:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #MB8X_INTR_SIZE	// size of IRC registers
	ldr		r1, [sp, #8]
	ldr		r1, [r1]
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}
    
    
/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code for EIRC with IRQ ID
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */
interrupt_patch_irc_eirc:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

    /* Capture physical address */
    mov     r3, r0, lsr #8
	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #MB8X_INTR_SIZE	// size of IRC registers
	ldr		r1, [sp, #8]
	ldr		r1, [r1]
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #MB8X_EIRC_SIZE	// size of IRC registers
	ldr		r1, Leintr1_base
	bl		callout_io_map

    /* If this is not the IRC0, pass 0 as EXIRC address */
    and     r3, r3, #0xff
    cmp     r3, #0xfe
    beq     1f
    eor     r1, r1, r1      // Load zero EXIRC address.
1:

    /*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
    
	ldmia	sp!,{r4,pc}


/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code for EIRC with IRQ ID
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */
interrupt_patch_irc_eirc_id:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

    /* Capture physical address */
    mov     r3, r0, lsr #8
	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #MB8X_INTR_SIZE	// size of IRC registers
	ldr		r1, [sp, #8]
	ldr		r1, [r1]
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #MB8X_EIRC_SIZE	// size of IRC registers
	ldr		r1, Leintr1_base
	bl		callout_io_map

    /* If this is not the IRC0, pass 0 as EXIRC address */
    and     r3, r3, #0xff
    cmp     r3, #0xfe
    beq     1f
    eor     r1, r1, r1      // Load zero EXIRC address.
1:

    /*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
    
    /*
     *  Always map IRC1
     */
	mov		r0, #MB8X_INTR_SIZE	// size of IRC registers
	ldr		r1, Lirc1_base
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
    
	ldmia	sp!,{r4,pc}    

Leintr1_base:   .word   MB8X_EIRC_BASE
Lirc1_base:     .word   MB8X_INTR2_BASE



/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_mb8x_irc, 0, interrupt_patch_irc_eirc_id)
    /*
     * Get the IRC interrupt controller base address (patched)
     */
    mov		ip,     #0x000000ff
    orr		ip, ip, #0x0000ff00
    orr		ip, ip, #0x00ff0000
    orr		ip, ip, #0xff000000

   	/*
	 * Get the external interrupt controller (EXIRC) base address (patched)
	 */
	mov		r1,     #0x000000ff
	orr		r1, r1, #0x0000ff00
	orr		r1, r1, #0x00ff0000
	orr		r1, r1, #0xff000000

   	/*
	 * Always get address of IRC1
	 */
	mov		r2,     #0x000000ff
	orr		r2, r2, #0x0000ff00
	orr		r2, r2, #0x00ff0000
	orr		r2, r2, #0xff000000

    /*
     * First disable IRC from generating interrupt.
     */ 
    mov     r0, #0
    str     r0, [ip, #MB8X_INTR_IRQM]
    
    /*
     * Get the current interrupt been serviced by reading the VECTOR. From that 
     * value we only care about the least significant byte. This value is an 
     * index (*4) within TBR with an offset of 0x20. 
     */
    ldr    r4, [ip, #MB8X_INTR_VCT]
    sub    r4, r4, #0x20
    mov    r4, r4, lsr #2
    and    r4, r4, #0x1f        /* There is only 32 interrupt source */

    /*
     * Now we need to mask the source of the interrupt
     */
    mov     r0, #MB8X_INTR_DISABLE
    add     r3, ip, #MB8X_INTR_ICR00
    str     r0, [r3, r4, lsl #2] 

    /*
     * Determine which IRC we are using and if this is IRC0, validate if we 
     * need to perform special action for IRC1 or EIRC
     */
    teq     r1, #0
    beq     2f
    
    /*
     * If source is from IRC1, disable interrupt from IRC1 to avoid double
     * interrupt.
     */
    teq     r4, #6
    bne     1f
    
    mov     r0, #0
    str     r0, [r2, #MB8X_INTR_IRQM]

    /* EXIRC range from IRC(0) 10 to 13 */
1:
    sub     r3, r4, #10
    cmp     r3, #3
    bhi     2f

    /*
     * Mask EXIRC interrupt
     */
    mov     r0, #1
    mvn     r0, r0, lsl r3
    ldr     r2, [r1, #MB8X_EIRC_EIENB]
    and     r2, r2, r0
    str     r2, [r1, #MB8X_EIRC_EIENB]  

    /*
     * Verify if EXIRC interrupt is edge trigger
     */
    mov     r2, #2
    mov     r0, r3, lsl #2
    mov     r0, r2, lsl r0
    ldr     r2, [r1, #MB8X_EIRC_EILVL]
    and     r0, r0, r2
    teq     r0, #0
    beq     2f
    
    /* Acknowledge edge source */
    mov     r0, #1
    mvn     r0, r0, lsl r3
    str     r0, [r1, #MB8X_EIRC_EIREQ] /* Writing one to it has no effect */

    /*
     * Now we have to clear the general interrupt flag
     */
2:
    mov     r0, #0
    str     r0, [ip, #MB8X_INTR_IRQF]
    /*
     * First disable IRC from generating interrupt. It is also used as dummy
     * instruction to give time to IRC to propagate the clearing flag 
     * instruction as stated in documentation.
     */ 
    mov     r0, #1
    str     r0, [ip, #MB8X_INTR_IRQM]

    CALLOUT_END(interrupt_id_mb8x_irc)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_mb8x_irc, 0, interrupt_patch_irc)
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
    bne     1f
    
    /*
     * Now we need to unmask the source for the specified interrupt source
     */
    mov    r2, #MB8X_INTR_ENABLE
    add    r3, ip, #MB8X_INTR_ICR00
    str    r2, [r3, r4, lsl #2] 

1:
CALLOUT_END(interrupt_eoi_mb8x_irc)


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
CALLOUT_START(interrupt_mask_mb8x_irc, 0, interrupt_patch_irc)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    /*
     * Mask the interrupt
     */
    mov    r0, #MB8X_INTR_DISABLE
    add    r2, ip, #MB8X_INTR_ICR00
    str    r0, [r2, r1, lsl #2] 

    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_mb8x_irc)


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
CALLOUT_START(interrupt_unmask_mb8x_irc, 0, interrupt_patch_irc_eirc)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000
    
	/*
	 * Get the external interrupt controller (EXIRC) base address (patched)
	 */
	mov		r2,     #0x000000ff
	orr		r2, r2, #0x0000ff00
	orr		r2, r2, #0x00ff0000
	orr		r2, r2, #0xff000000

    /*
     * Determine which device we are using and if this is the primary, do ack
     * interrupt comming from EIRC
     */
    teq     r2, #0
    beq     1f

    /* EXIRC range from IRC(0) 10 to 13 */
    sub     r3, r1, #10
    cmp     r3, #3
    bhi     1f

    /*
     * Verify if we have level trigger interrupt
     */
	stmdb	sp!,{r4}        /* Need to save r4 to avoid corruption of register */
    mov     r4, #2
    mov     r0, r3, lsl #2
    mov     r0, r4, lsl r0
    ldr     r4, [r2, #MB8X_EIRC_EILVL]
    and     r0, r4, r0
	ldmia	sp!,{r4}
    teq     r0, #0
    bne     2f
    
    /* Acknowledge level trigger source */
    mov     r0, #1
    mvn     r0, r0, lsl r3
    str     r0, [r2, #MB8X_EIRC_EIREQ] /* Writing one to it has no effect */

    /*  Unmask interrupt   */
2:    
    mov     r0, #1
    mov     r0, r0, lsl r3
    ldr     r3, [r2, #MB8X_EIRC_EIENB]
    orr     r0, r3, r0
    str     r0, [r2, #MB8X_EIRC_EIENB]  
1:
    /*
     * Unmask the interrupt
     */
    mov    r0, #MB8X_INTR_ENABLE
    add    r2, ip, #MB8X_INTR_ICR00
    str    r0, [r2, r1, lsl #2] 

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_mb8x_irc)

