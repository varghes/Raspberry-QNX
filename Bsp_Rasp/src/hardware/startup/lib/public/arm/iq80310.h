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
 * Intel iq80310 Development Board
 *

 *
 */

#ifndef	ARM_IQ80310_H_INCLUDED
#define	ARM_IQ80310_H_INCLUDED

/*
 * 16550 UART register base.
 */
#define	IQ80310_UART1_BASE	0xfe800000
#define	IQ80310_UART2_BASE	0xfe810000

/*
 * Interrupt controller registers
 */
#define	IQ80310_INT_STAT	0xfe820000		/* status (read-only) */
#define	IQ80310_INT_MASK	0xfe860000		/* interrupt mask */

/*
 * IQ80310_INT_STAT/MASK bits
 */
#define	IQ80310_INT_TIMER	0x01			/* timer */
#define	IQ80310_INT_ETHER	0x02			/* 82559 ethernet */
#define	IQ80310_INT_UART1	0x04			/* uart1 */
#define	IQ80310_INT_UART2	0x08			/* uart2 */
#define	IQ80310_INT_PCI		0x10			/* PCI INTD */

/*
 * 7-Segment LED
 */
#define	IQ80310_LED_MSB		0xfe840000		/* MSB (write-only) */
#define	IQ80310_LED_LSB		0xfe850000		/* LSB (write-only) */

#define	IQ80310_LED_0		0xc0
#define	IQ80310_LED_1		0xf9
#define	IQ80310_LED_2		0xa4
#define	IQ80310_LED_3		0xb0
#define	IQ80310_LED_4		0x99
#define	IQ80310_LED_5		0x92
#define	IQ80310_LED_6		0x88
#define	IQ80310_LED_7		0xf8
#define	IQ80310_LED_8		0x80
#define	IQ80310_LED_9		0x90
#define	IQ80310_LED_a		0x88
#define	IQ80310_LED_b		0x83
#define	IQ80310_LED_c		0xa7
#define	IQ80310_LED_d		0xa1
#define	IQ80310_LED_e		0x86
#define	IQ80310_LED_f		0x8e

/*
 * Board configuration
 */
#define	IQ80310_BOARD_REV	0xfe830000		/* read-only */
#define	IQ80310_CPLD_REV	0xfe840000		/* read-only */
#define	IQ80310_BACKPLANE	0xfe870000		/* read-only */
#define	IQ80310_ROTARY_SW	0xfe8d0000		/* read-only */
#define	IQ80310_BATTERY		0xfe8f0000		/* read-only */

/*
 * Timer
 */
#define	IQ80310_TIMER_LA0	0xfe880000
#define	IQ80310_TIMER_LA1	0xfe890000
#define	IQ80310_TIMER_LA2	0xfe8a0000
#define	IQ80310_TIMER_LA3	0xfe8b0000
#define	IQ80310_TIMER_EN	0xfe8c0000

/*
 * IQ80310_TIMER_EN bits
 */
#define	IQ80310_TIMER_EN_CNT	1			/* enable counter */
#define	IQ80310_TIMER_EN_INT	2			/* enable interrupt */

#endif

/* __SRCVERSION("iq80310.h $Rev: 657836 $"); */
