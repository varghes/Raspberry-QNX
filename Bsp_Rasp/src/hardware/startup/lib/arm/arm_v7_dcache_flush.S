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

/*
 * Flush entire D-cache and drain write buffer
 */

	.text
	.align 2
	.globl	arm_v7_dcache_flush

arm_v7_dcache_flush:
	stmfd	r13!, {r0-r5, r7, r9-r12, r14}

	mrc		p15, 1, r0, c0, c0, 1
	ands	r3, r0, #0x7000000
	mov		r3, r3, lsr #23
	beq		finished
	mov		r10, #0
loop1:
	add		r2, r10, r10, lsr #1
	mov		r1, r0, lsr r2
	and		r1, r1, #7
	cmp		r1, #2
	blt		skip
	mcr		p15, 2, r10, c0, c0, 0
	isb
	mrc		p15, 1, r1, c0, c0, 0
	and		r2, r1, #7
	add		r2, r2, #4
	ldr		r4, =0x3ff
	ands	r4, r4, r1, lsr #3
	clz		r5, r4
	ldr		r7, =0x7fff
	ands	r7, r7, r1, lsr #13
loop2:
	mov		r9, r4
loop3:
	orr		r11, r10, r9, lsl r5
	orr		r11, r11, r7, lsl r2
	mcr		p15, 0, r11, c7, c14, 2
	subs	r9, r9, #1
	bge		loop3
	subs	r7, r7, #1
	bge		loop2
skip:
	add		r10, r10, #2
	cmp		r3, r10
	bgt		loop1
finished:
	mov		r10, #0	
	mcr		p15, 2, r10, c0, c0, 0
    isb
	ldmfd	r13!, {r0-r5, r7, r9-r12, pc}
