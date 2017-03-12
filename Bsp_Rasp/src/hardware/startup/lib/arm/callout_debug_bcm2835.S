/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. 
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

#include "callout.ah"
#include <arm/bcm2835.h>
/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *  r0     - physical address of syspage
 *  r1     - virtual  address of syspage
 *  r2     - offset from start of syspage to start of the callout routine
 *  r3     - offset from start of syspage to read/write data used by callout
 *  [sp]   - data registered with callout via callout_register_data()
 *  [sp+4] - address of callout definition (CALLOUT_START)
 * -----------------------------------------------------------------------
 */
patch_debug:
    stmdb       sp!,{r4,lr}
    add         r4, r0, r2                  // address of callout routine

    /*
     * Map DBUG control registers
     */
	ldr		r1, [sp, #8]				// debug_devices[...]
	ldr		r1, [r1, #DDI_BASE]
	mov		r0, #BCM2835_UART_SIZE
	bl		callout_io_map

    /* Patch the callout routine */
    CALLOUT_PATCH   r4, r0, r1, r2, ip

    ldmia       sp!,{r4,pc}


 Lpaddr: .word   BCM2835_UART0_BASE

/*
 * -----------------------------------------------------------------------
 * void display_char_at91sam9xx(struct sypage_entry *, char)
 * -----------------------------------------------------------------------
 */
CALLOUT_START(display_char_bcm2835, 0, patch_debug)
    mov         ip,     #0x000000ff      // DGBU base address
    orr         ip, ip, #0x0000ff00
    orr         ip, ip, #0x00ff0000
    orr         ip, ip, #0xff000000

    /*
     * Wait for TXRDY
     */
0:  ldr         r0, [ip, #BCM2835_UART0_FR] //ldr
    tst         r0, #BCM2835_DBGU_IE_SR_TXRDY
    bne         0b

    /*
     * Output character
     */
    str         r1, [ip, #BCM2835_UART0_DR]

    mov         pc, lr
CALLOUT_END(display_char_bcm2835)



/*
 * -----------------------------------------------------------------------
 * char display_char_at91sam9xx(struct sypage_entry *)
 * -----------------------------------------------------------------------
 */
CALLOUT_START(poll_key_bcm2835, 0, patch_debug)
    mov         ip,     #0x000000ff      // DGBU base address
    orr         ip, ip, #0x0000ff00
    orr         ip, ip, #0x00ff0000
    orr         ip, ip, #0xff000000

    /*
     * If RXRDY, read character, otherwise return -1
     */
0:  ldrb         r3, [ip, #BCM2835_UART0_FR]
    tst         r3, #BCM2835_DBGU_IE_SR_RXEMTY
    ldreqb       r0, [ip, #BCM2835_UART0_DR]
    mvnne       r0, #1
    mov         pc, lr
CALLOUT_END(poll_key_bcm2835)


/*
 * -----------------------------------------------------------------------
 * int break_detect(struct syspage_entry *)
 *
 * Return 1 if break detected, 0 otherwise
 * -----------------------------------------------------------------------
 */
CALLOUT_START(break_detect_bcm2835, 0, patch_debug)
    mov         ip,     #0x000000ff     // DGBU base address
    orr         ip, ip, #0x0000ff00
    orr         ip, ip, #0x00ff0000
    orr         ip, ip, #0xff000000

    /*
     * Not capable of detecting break character, always return 0.
     */
    mov         r0, #0
    mov         pc, lr
CALLOUT_END(break_detect_bcm2835)
