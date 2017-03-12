/*
 * $QNXLicenseC:
 * Copyright 2011, QNX Software Systems.
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
 * MPCore distributed interrupt controller callouts for OMAP4 WUGEN.
 *
 * This documents only cover mask and unmask callouts needed to support 
 * OMAP4 MPU inactive wakeup event. They are derived version of the normal
 * ARM GIC Callouts specified in the "callout_interrupt_mpcore.S" file.
 * 
 * ID, EOI & Config callouts are stil provided by the generic ARM GIC callouts.
 *
 * Here is the list of callouts you will need to include for OMAP4 if you want
 * to do power management and support MPU wakeup events. If you don't need to
 * support wakup event then you should use default MPCORE GIC callouts.
 *
 *          id    : interrupt_id_mpcore
 *          eoi   : interrupt_eoi_mpcore
 *          mask  : interrupt_mask_omap4_wugen_wugen
 *          unmask: interrupt_unmask_omap4_wugen_wugen
 *          config: interrupt_config_mpcore
 *
 * NOTE: the board specific startup is responsible for defining the
 *       mpcore_scu_base variable and assigning it to the physical
 *       address that the SCU is implemented on that board.
 *
 *       Since theses callouts were design for OMAP4430, the WUGEN base address
 *       and offset have been hardcoded (via #define). If you need to make it
 *       more generic, make use of the ".patch_data" to pass the address as 
 *       well as the SCU address.
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
#include "arm/mpcore.h"

#define WKUP_GEN_BASE       0x48281000
#define WKUP_GEN_SIZE       0x1000

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 *  [sp] - patch data specified by startup_intrinfo struct
 * -----------------------------------------------------------------------
 */

patch_intr:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map SCU base
	 */
	ldr		r1, [sp, #8]				// &mpcore_scu_base
	mov		r0, #MPCORE_GIC_DIST_BASE + MPCORE_GIC_DIST_SIZE
	ldr		r1, [r1]
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/*
	 * Map cortexa9 wkup generator controller registers
	 */
	mov		r0, #WKUP_GEN_SIZE
	ldr		r1, wkup_base
	bl		callout_io_map

	CALLOUT_PATCH	r4, r0, r1, r2, ip
    
	ldmia	sp!,{r4,pc}    
	
	
wkup_base:   .word   WKUP_GEN_BASE

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
CALLOUT_START(interrupt_mask_omap4_wugen, 0, patch_intr)
	mov		r0,     #0x000000ff		// SCU base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

    /*
	 * Get the WKUP controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	add		r0, r0, #MPCORE_GIC_DIST_BASE

	/* Get interrupt id bit position within the register */
	and		r2, r1, #0x1f
	mov		r3, #1
	mov		r3, r3, lsl r2							// 1 << (id % 32)
	
	/* Get register offset */
	mov		r2, r1, lsr #5
	mov		r2, r2, lsl #2
	
	/* Disable interrupt mask  */
	add		r0, r0, #MPCORE_GIC_DIST_ENABLE_CLR		// ENABLE_CLR[id / 32]
	str		r3, [r0, r2]

	/* Disable wkup event, we skip the first 32 interrupt that don't have 
	   associated WKUP events.
	*/
	teq     r1, #31
	ble     0f
	
	/* Fix the register offset since we ignore first 32 irq */
	add     r2, #-4
	
	/* Fix base address to point to WKG_ENB_A_0 */ 
	add     ip, ip,  #0x10
	
	/* Clear the WKUP event */
	mvn     r3, r3
	ldr     r0, [ip, r2]
	and		r0, r0, r3
	str		r0, [ip, r2]
0:
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_omap4_wugen)


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
CALLOUT_START(interrupt_unmask_omap4_wugen, 0, patch_intr)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		r0,     #0x000000ff
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

    /*
	 * Get the WKUP controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	add		r0, r0, #MPCORE_GIC_DIST_BASE

	/* Get interrupt id bit position within the register */
	and		r2, r1, #0x1f
	mov		r3, #1
	mov		r3, r3, lsl r2							// 1 << (id % 32)
	
	/* Get register offset */
	mov		r2, r1, lsr #5
	mov		r2, r2, lsl #2
	
	/* Enable interrupt mask  */
	add		r0, r0, #MPCORE_GIC_DIST_ENABLE_SET		// ENABLE_SET[id / 32]
	str		r3, [r0, r2]

	/* Enable wkup event, we skip the first 32 interrupt that don't have 
	   associated WKUP events.
	*/
	teq     r1, #31
	ble     0f

	/* Fix the register offset since we ignore first 32 irq */
	add     r2, #-4
	
	/* Fix base address to point to WKG_ENB_A_0 */ 
	add     ip, ip,  #0x10

	/* Set the WKUP event */
	ldr     r0, [ip, r2]
	orr		r0, r0, r3
	str		r0, [ip, r2]

0:
    mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_omap4_wugen)

