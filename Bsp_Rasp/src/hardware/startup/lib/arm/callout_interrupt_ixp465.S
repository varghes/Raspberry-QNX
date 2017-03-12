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
 * Intel IXP425 (IXC1100) specific interrupt callouts
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
   r5 - holds the syspageptr           (INTR_GENFLAG_SYSPAGE  set)
 * r6 - holds the intrinfo_entry pointer  (INTR_GENFLAG_INTRINFO set)
 * r7 - holds the interrupt mask count    (INTR_GENFLAG_INTRMASK set)
 * r8 - holds INTRLEVEL pointer
 *
 * The interrupt_id_* routine returns the (controller-relative) level in r4
 */

#include "callout.ah"


/*
 * --------------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 * r0 - physical address of syspage
 * r1 - virtual  address of syspage
 * r2 - offset from start of syspage to start of the callout routine
 * r3 - offset from start of syspage to read/write data used by callout
 * --------------------------------------------------------------------------
 */


/*
 * Patch callouts with IXP425_IRQ_BASE
 */
patch_intr:

   stmdb sp!,{r4,lr}
   add      r4, r0, r2              // address of callout routine

   /*
    * Map registers
    */

   mov      r0, #IXP465_IRQ_SIZE    // size of registers
   ldr      r1, Lintr_base
   bl    callout_io_map


   CALLOUT_PATCH  r4, r0, r1, r2, ip

   ldmia sp!,{r4,pc}

patch_intr_gpio:

   stmdb sp!,{r4,lr}
   add      r4, r0, r2              // address of callout routine

   mov      r0, #IXP465_IRQ_SIZE
   ldr      r1, Lintr_base
   bl    callout_io_map
   CALLOUT_PATCH  r4, r0, r1, r2, ip

   mov      r0, #IXP425_GPIO_SIZE
   ldr      r1, Lgpio_base
   bl    callout_io_map
   CALLOUT_PATCH  r4, r0, r1, r2, ip

   ldmia sp!,{r4,pc}

Lintr_base: .word IXP425_IRQ_BASE
Lgpio_base: .word IXP425_GPIO_BASE 


CALLOUT_START(interrupt_id_ixp465, 0, patch_intr)
   /*
    *  Get the address of the interrupt registers (patched)
    */
   mov      ip,     #0x000000ff
   orr      ip, ip, #0x0000ff00
   orr      ip, ip, #0x00ff0000
   orr      ip, ip, #0xff000000


   /*
    *  Scan for first set bit
    */
   ldr      r1, [ip, #IXP425_IRQ_STATUS]     // indicates any active interrupts
   mov      r4, #31                       // number of sources
   mov      r2, #1

   cmp      r1, #0
   beq      3f                           // Must be in STATUS2 register

0:
   subs     r4, r4, #1
   blt      1f                           // Check the other status register
   tst      r1, r2, lsl r4
   beq      0b

   // Found an interrupt, look for higher priority interrupts
   ldr      r5,[ip,#IXP425_IRQ_HIPRIO]           // check for a higher prio int
   and      r5,r5,#0xfc                          // bit2 2-7 only
   mov      r5,r5, lsr #2
   sub      r5,r5,#1
   cmp      r5,#0
   beq      1f
   mov      r4,r5
   b        1f

1:
   /*
    *  mask the interrupt source
    */
   mov      r2, r2, lsl r4
   ldr      r1, [ip, #IXP425_IRQ_ENABLE]        // get enabled levels
   bic      r1,r1,r2
   str      r1, [ip, #IXP425_IRQ_ENABLE]       // drop appropriate bit
   b        2f

   /*
    * Work the 465's second 32-bit registers
    */
3:
   ldr      r1, [ip, #IXP465_IRQ_STATUS2]     // indicates any active interrupts
   mov      r4, #31                       // number of sources
   mov      r2, #1
4:
   subs     r4, r4, #1
   blt      5f
   tst      r1, r2, lsl r4
   beq      4b

5:
   /*
    *  mask the interrupt source
    */
   mov      r2, r2, lsl r4
   ldr      r1, [ip, #IXP465_IRQ_ENABLE2]        // get enabled levels
   bic      r1,r1,r2
   str      r1, [ip, #IXP465_IRQ_ENABLE2]       // drop appropriate bit
   add      r4,r4,#32

2:
CALLOUT_END(interrupt_id_ixp465)


CALLOUT_START(interrupt_eoi_ixp465, 0, patch_intr)

   /*
    * Get the address of the interrupt registers (patched)
    */
   mov      ip,     #0x000000ff
   orr      ip, ip, #0x0000ff00
   orr      ip, ip, #0x00ff0000
   orr      ip, ip, #0xff000000

   // Check if we're working on the low or high register
   cmp      r4, #32
   blt      1f

   // -- Start of top 32 bits of interrupts
   /*
    * Get current INT_MASK value
    */
   ldr      r0, [ip, #IXP465_IRQ_ENABLE2]

   /*
    * Only unmask if mask count is zero
    */
   sub      r4, r4, #32
   teq      r7, #0
   bne      4f

   mov      r2, #1
   mov      r2, r2, lsl r4
   orr      r0, r0, r2

4:
   /*
    * set the enable 
    */
   str      r0, [ip, #IXP465_IRQ_ENABLE2]
   add      r4, r4, #32  // Restore r4
   b        3f


   // -- Start of lower 32 bits of interrupts
1:
   /*
    * Get current INT_MASK value
    */
   ldr      r0, [ip, #IXP425_IRQ_ENABLE]

   /*
    * Only unmask if mask count is zero
    */
   teq      r7, #0
   bne      0f

   mov      r2, #1
   mov      r2, r2, lsl r4
   orr      r0, r0, r2

0:
   /*
    * set the enable 
    */
   str      r0, [ip, #IXP425_IRQ_ENABLE]

3:
CALLOUT_END(interrupt_eoi_ixp465)


/*
 * error = interrupt_mask_becc(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_ixp465, 0, patch_intr)
   /*
    * Get the address of the interrupt registers (patched)
    */
   mov      ip,     #0x000000ff
   orr      ip, ip, #0x0000ff00
   orr      ip, ip, #0x00ff0000
   orr      ip, ip, #0xff000000

   // r1 has the interrupt level in it.
   // Check if it is for the top or bottom 32 bits
   cmp     r1, #32
   blt     1f

   sub     r1, r1, #32   // Bring it down to < 32 for now
   ldr     r0, [ip, #IXP465_IRQ_ENABLE2]
   mov     r2, #1
   mov     r2, r2, lsl r1
   bic     r0,r0,r2
   str     r0, [ip, #IXP465_IRQ_ENABLE2]
   add     r1,r1,#32  // Put it back

   mov      r0, #0
   mov      pc, lr
   b        2f

1:
   ldr     r0, [ip, #IXP425_IRQ_ENABLE]
   mov     r2, #1
   mov     r2, r2, lsl r1
   bic      r0,r0,r2
   str     r0, [ip, #IXP425_IRQ_ENABLE]

   mov      r0, #0
   mov      pc, lr

2:
CALLOUT_END(interrupt_mask_ixp465)


/*
 * error = interrupt_unmask_ixdp465(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_ixp465, 0, patch_intr_gpio)
   /*
    * Get the address of the interrupt registers (patched)
    */
   mov      ip,     #0x000000ff
   orr      ip, ip, #0x0000ff00
   orr      ip, ip, #0x00ff0000
   orr      ip, ip, #0xff000000

   mov      r3,     #0x000000ff
   orr      r3, r3, #0x0000ff00
   orr      r3, r3, #0x00ff0000
   orr      r3, r3, #0xff000000

   /*
    *  clear appropriate gpio for PCI interrupts
    */

   cmp     r1, #32
   blt     0f

   // Manage interrupts with id >= 32:
   sub     r1, r1, #32
   ldr     r0, [ip, #IXP465_IRQ_ENABLE2]
   mov     r2, #1
   mov     r2, r2, lsl r1
   orr     r0, r0, r2
   add     r1, r1, #32

   str     r0, [ip, #IXP465_IRQ_ENABLE2]

   mov     r0,#0
   mov     pc, lr
   b       2f


   // Manage interrupts with id < 32:
0:
   cmp     r1, #(IXP425_IRQ_GPIO8 - 1) 
   ble     1f
   cmp     r1, #IXP425_IRQ_GPIO11
   bgt     1f

   ldr     r2,[r3,#IXP425_GPIO_GPISR]
   str     r2,[r3,#IXP425_GPIO_GPISR]


1:
   ldr     r0, [ip, #IXP425_IRQ_ENABLE]
   mov     r2, #1
   mov     r2, r2, lsl r1
   orr     r0, r0, r2

   str     r0, [ip, #IXP425_IRQ_ENABLE]

   mov     r0,#0
   mov     pc, lr
2:
CALLOUT_END(interrupt_unmask_ixp465)

