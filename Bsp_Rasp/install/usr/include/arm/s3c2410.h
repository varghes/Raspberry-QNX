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
 * S2C2410 register definitions
 */

/* Watchdog timer */
#define 	WTCON			0x53000000

/* LCD controller */
#define 	LCDCON1			0x4d000000

/* Core Program Status Register */
#define CPSR_MASK       0x1f
#define CPSR_SVC_IF     0xd3

/* Clock Controller */
#define 	LOCKTIME		0x4C000000
#define 	MPLLCON			0x4C000004
#define 	UPLLCON			0x4C000008
#define		CLKCON			0x4C00000c
#define 	CLKDIVN			0x4C000014

/* Timers */
#define	S3C2410_TIMER_BASE	0x51000000
#define	S3C2410_TIMER_SIZE	0x44

/* offsets from TIMER_BASE */
#define	S3C2410_TCFG0		0x00
#define	S3C2410_TCFG1		0x04
#define	S3C2410_TCON		0x08
#define	S3C2410_TCNTB0		0x0C
#define	S3C2410_TCMPB0		0x10
#define	S3C2410_TCNTO0		0x14
#define	S3C2410_TCNTB1		0x18
#define	S3C2410_TCMPB1		0x1C
#define	S3C2410_TCNTO1		0x20
#define	S3C2410_TCNTB2		0x24
#define	S3C2410_TCMPB2		0x28
#define	S3C2410_TCNTO2		0x2C
#define	S3C2410_TCNTB3		0x30
#define	S3C2410_TCMPB3		0x34
#define	S3C2410_TCNTO3		0x38
#define	S3C2410_TCNTB4		0x3C
#define	S3C2410_TCNTO4		0x40


/* RTC */
#define S3C2410_RTC_BASE		0x57000040
#define	S3C2410_RTC_SIZE		0x50

/* offsets from RTC_BASE */
#define	S3C2410_RTCCON	0x00
#define	S3C2410_TICNT	0x04
#define	S3C2410_RTCALM	0x10
#define	S3C2410_ALMSEC	0x14
#define	S3C2410_ALMMIN	0x18
#define	S3C2410_ALMHOUR	0x1C
#define	S3C2410_ALMDAY	0x20
#define	S3C2410_ALMMON	0x24
#define	S3C2410_ALMYEAR	0x28
#define	S3C2410_RTCRST	0x2C
#define	S3C2410_BCDSEC	0x30
#define	S3C2410_BCDMIN	0x34
#define	S3C2410_BCDHOUR	0x38
#define	S3C2410_BCDDAY	0x3C
#define	S3C2410_BCDDATE	0x40
#define	S3C2410_BCDMON	0x44
#define	S3C2410_BCDYEAR	0x48


/* MMU */
#define MMU_I   (1 << 12)
#define MMU_nF  (1 << 30)
#define MMU_iA  (1 << 31)
    
/* Memory Controller */
#define		BWSCON			0x48000000
#define		BANKCON0		0x48000004
#define		BANKCON1		0x48000008
#define		BANKCON2		0x4800000C
#define		BANKCON3		0x48000010
#define		BANKCON4		0x48000014
#define		BANKCON5		0x48000018
#define		BANKCON6		0x4800001C
#define		BANKCON7		0x48000020
#define		REFRESH			0x48000024
#define		BANKSIZE		0x48000028
#define		MRSRB6			0x4800002c
#define		MRSRB7			0x48000030


#define		BIT_SELFREFRESH	(1<<22)

/* Pre-defined constants */
#define	USERMODE 	0x10
#define	FIQMODE	 	0x11
#define	IRQMODE	 	0x12
#define	SVCMODE	 	0x13
#define	ABORTMODE 	0x17
#define	UNDEFMODE	0x1b
#define	MODEMASK	0x1f
#define	NOINT		0xc0

/* GPIO port definitions */
#define		PACON	0x56000000
#define		PADAT	0x56000004
#define		PBCON	0x56000010
#define 	PBDAT	0x56000014
#define		PBUP	0x56000018
#define		PCCON	0x56000020
#define		PCDAT	0x56000024
#define		PCUP	0x56000028
#define		PDCON	0x56000030
#define 	PDDAT	0x56000034
#define		PDUP	0x56000038
#define		PECON	0x56000040
#define		PEDAT	0x56000044
#define		PEUP	0x56000048
#define		PFCON	0x56000050
#define		PFDAT	0x56000054
#define		PFUP	0x56000058
#define		PGCON	0x56000060
#define		PGDAT	0x56000064
#define		PGUP	0x56000068
#define		PHCON	0x56000070
#define		PHDAT	0x56000074
#define		PHUP	0x56000078

#define 	MISCCR	0x56000080

/* external interrupts */
#define 	EXTINT0	0x56000088
#define 	EXTINT1	0x5600008C
#define 	EXTINT2	0x56000090

#define		S3C2410_EINTR_BASE	0x560000A4
#define 	S3C2410_EINTMSK		0x00
#define		S3C2410_EINTPND		0x04

/* UART */
#define		S3C2410_UART0_BASE	0x50000000
#define		S3C2410_UART1_BASE	0x50004000
#define		S3C2410_UART_SIZE	0x30

/* offsets from UARTx_BASE */
#define 	S3C2410_ULCON		0x00
#define		S3C2410_UCON		0x04
#define		S3C2410_UFCON		0x08
#define		S3C2410_UMCON		0x0C
#define		S3C2410_UTRSTAT		0x10
#define		S3C2410_UERSTAT		0x14
#define		S3C2410_UFSTAT		0x18
#define		S3C2410_UMSTAT		0x1C
#define		S3C2410_UTXH		0x20
#define		S3C2410_URXH		0x24
#define		S3C2410_UBRDIV		0x28

/* ULCON bits */
#define		ULCON_WORD_LENGTH_MASK	0x03
#define		ULCON_STOP_BIT_MASK		0x04
#define		ULCON_PARITY_MASK		0x38
#define		ULCON_IR_MASK			0x40
#define		ULCON_WORD_LENGTH_5BIT	0x00
#define		ULCON_WORD_LENGTH_6BIT	0x01
#define		ULCON_WORD_LENGTH_7BIT	0x02
#define		ULCON_WORD_LENGTH_8BIT	0x03
#define		ULCON_STOP_BIT_1		0x00
#define		ULCON_STOP_BIT_2		0x04
#define		ULCON_PARITY_NONE		0x00
#define		ULCON_PARITY_ODD		0x20
#define		ULCON_PARITY_EVEN		0x28
#define		ULCON_PARITY_FORCE1		0x30
#define		ULCON_PARITY_FORCE0		0x38
#define		ULCON_IR_OFF			0x00
#define		ULCON_IR_MODE			0x40

/* UCON bits */
#define		UCON_RX_MODE_MASK		0x03
#define		UCON_RX_MODE_INT		0x01
#define		UCON_RX_MODE_DMA0		0x02
#define		UCON_RX_MODE_DMA1		0x03
#define		UCON_TX_MODE_MASK		0x0C
#define		UCON_TX_MODE_INT		0x04
#define		UCON_TX_MODE_DMA0		0x08
#define		UCON_TX_MODE_DMA1		0x0C
#define		UCON_SEND_BREAK			0x10
#define		UCON_LOOP_BACK			0x20
#define		UCON_RX_ERR_INT_EN		0x40
#define		UCON_RX_TIMEOUT_EN		0x80
#define		UCON_RX_INT_TYPE_LEVEL	0x100
#define		UCON_TX_INT_TYPE_LEVEL	0x200

/* UFCON bits */
#define		UFCON_FIFO_MODE_ENABLE	0x01
#define		UFCON_RX_FIFO_RESET		0x02
#define		UFCON_TX_FIFO_RESET		0x04
#define		UFCON_RX_FIFO_TRIG_MASK	0x30
#define		UFCON_RX_FIFO_TRIG_4	0x00
#define		UFCON_RX_FIFO_TRIG_8	0x10
#define		UFCON_RX_FIFO_TRIG_12	0x20
#define		UFCON_RX_FIFO_TRIG_16	0x30
#define		UFCON_TX_FIFO_TRIG_MASK	0xC0
#define		UFCON_TX_FIFO_TRIG_EMP	0x00
#define		UFCON_TX_FIFO_TRIG_4	0x40
#define		UFCON_TX_FIFO_TRIG_8	0x80
#define		UFCON_TX_FIFO_TRIG_12	0xC0

/* UMCON bits */
#define		UMCON_RTS				0x01
#define		UMCON_AFC_ENABLE		0x80

/* UTRSTAT bits */
#define		UTRSTAT_RX_BUF_READY	0x01
#define		UTRSTAT_TX_BUF_EMPTY	0x02
#define		UTRSTAT_TX_EMPTY		0x04

/* UERSTAT bits */
#define		UERSTAT_OVERRUN_ERR		0x01
#define		UERSTAT_PARITY_ERR		0x02
#define		UERSTAT_FRAME_ERR		0x04
#define		UERSTAT_BREAK_DETECT	0x08

/* FIFO status bits */
#define		UFSTAT_RX_FIFO_COUNT	0x0f
#define		UFSTAT_TX_FIFO_COUNT	0xf0
#define		UFSTAT_RX_FIFO_FULL		0x100
#define		UFSTAT_TX_FIFO_FULL		0x200

/* MODEM status bits */
#define		UMSTAT_CTS				0x01
#define		UMSTAT_DELTA_CTS		0x04


/* Interrupt Controller */
#define		S3C2410_INTR_BASE	0x4A000000
#define		S3C2410_INTR_SIZE	0x20

/* offsets from INTR_BASE */
#define		S3C2410_INTR_SRCPND	0x00
#define		S3C2410_INTMOD		0x04
#define		S3C2410_INTMSK		0x08
#define		S3C2410_PRIORITY	0x0C
#define		S3C2410_INTPND		0x10
#define		S3C2410_INTOFFSET	0x14
#define		S3C2410_SUBSRCPND	0x18
#define		S3C2410_INTSUBMSK	0x1c

/* ADC / Touchscreen Controller */
#define		S3C2410_ADC_TC_BASE		0x58000000
#define 	S3C2410_ADC_TC_SIZE		0x14
#define		S3C2410_ADCCON			0x00
#define 	S3C2410_ADCTSC			0x04
#define		S3C2410_ADCDLY			0x08
#define		S3C2410_ADCDAT0			0x0c
#define		S3C2410_ADCDAT1			0x10

/* ADCCON bits */
#define		ADCCON_ECFLG			0x8000
#define		ADCCON_PRSCEN			0x4000
#define		ADCCON_PRSCVL_SHIFT		6
#define		ADCCON_PRSCVL_MASK		0xFF
#define		ADCCON_SEL_MUX_SHIFT	3
#define		ADCCON_SEL_MUX_MASK		0x07
#define		ADCCON_STDBM			0x04
#define		ADCCON_READ_START		0x02
#define		ADCCON_ENABLE_START		0x01

/* ADCTSC bits */
#define		ADCTSC_YM_SEN			0x80
#define		ADCTSC_YP_SEN			0x40
#define		ADCTSC_XM_SEN			0x20
#define		ADCTSC_XP_SEN			0x10
#define		ADCTSC_PULL_UP			0x08
#define		ADCTSC_AUTO_PST			0x04
#define		ADCTSC_XY_PST_WAIT_INT	0x03
#define		ADCTSC_XY_PST_Y_POS		0x02
#define		ADCTSC_XY_PST_X_POS		0x01
#define		ADCTSC_XY_PST_NONE		0x00

/* ADCDAT0 bits */
#define		ADCDAT0_UPDOWN			0x8000
#define		ADCDAT0_AUTO_PST		0x4000
#define		ADCDAT0_XY_PST_SHIFT	12
#define		ADCDAT0_XY_PST_MASK		0x03
#define		ADCDAT0_XY_PST_WAIT_INT	0x03
#define		ADCDAT0_XY_PST_Y_POS	0x02
#define		ADCDAT0_XY_PST_X_POS	0x01
#define		ADCDAT0_XY_PST_NONE		0x00
#define		ADCDAT0_XPDATA_MASK		0x3ff

/* ADCDAT1 bits */
#define		ADCDAT1_UPDOWN			0x8000
#define		ADCDAT1_AUTO_PST		0x4000
#define		ADCDAT1_XY_PST_SHIFT	12
#define		ADCDAT1_XY_PST_MASK		0x03
#define		ADCDAT1_XY_PST_WAIT_INT	0x03
#define		ADCDAT1_XY_PST_Y_POS	0x02
#define		ADCDAT1_XY_PST_X_POS	0x01
#define		ADCDAT1_XY_PST_NONE		0x00
#define		ADCDAT1_YPDATA_MASK		0x3ff



















/* __SRCVERSION("s3c2410.h $Rev: 657836 $"); */
