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
 * S2C2400 register definitions
 *

 *
 */

/* Interrupt control register */
#define 	INTOFFSET		0x14400014
#define		INTMSK			0x14400008

/* Watchdog timer */
#define 	WTCON			0x15300000

/* LCD controller */
#define 	LCDCON1			0x14a00000

/* Core Program Status Register */
#define CPSR_MASK       0x1f
#define CPSR_SVC_IF     0xd3

/* Clock Controller */
#define 	LOCKTIME		0x14800000
#define 	MPLLCON			0x14800004
#define 	UPLLCON			0x14800008
#define		CLKCON			0x1480000c
#define 	CLKDIVN			0x14800014

/* Timers */
#define	S3C2400_TIMER_BASE	0x15100000
#define	S3C2400_TIMER_SIZE	0x44

/* offsets from TIMER_BASE */
#define	S3C2400_TCFG0		0x00
#define	S3C2400_TCFG1		0x04
#define	S3C2400_TCON		0x08
#define	S3C2400_TCNTB0		0x0C
#define	S3C2400_TCMPB0		0x10
#define	S3C2400_TCNTO0		0x14
#define	S3C2400_TCNTB1		0x18
#define	S3C2400_TCMPB1		0x1C
#define	S3C2400_TCNTO1		0x20
#define	S3C2400_TCNTB2		0x24
#define	S3C2400_TCMPB2		0x28
#define	S3C2400_TCNTO2		0x2C
#define	S3C2400_TCNTB3		0x30
#define	S3C2400_TCMPB3		0x34
#define	S3C2400_TCNTO3		0x38
#define	S3C2400_TCNTB4		0x3C
#define	S3C2400_TCNTO4		0x40


/* RTC */
#define S3C2400_RTC_BASE		0x15700040
#define	S3C2400_RTC_SIZE		0x50

/* offsets from RTC_BASE */
#define	S3C2400_RTCCON	0x00
#define	S3C2400_TICNT	0x04
#define	S3C2400_RTCALM	0x10
#define	S3C2400_ALMSEC	0x14
#define	S3C2400_ALMMIN	0x18
#define	S3C2400_ALMHOUR	0x1C
#define	S3C2400_ALMDAY	0x20
#define	S3C2400_ALMMON	0x24
#define	S3C2400_ALMYEAR	0x28
#define	S3C2400_RTCRST	0x2C
#define	S3C2400_BCDSEC	0x30
#define	S3C2400_BCDMIN	0x34
#define	S3C2400_BCDHOUR	0x38
#define	S3C2400_BCDDAY	0x3C
#define	S3C2400_BCDDATE	0x40
#define	S3C2400_BCDMON	0x44
#define	S3C2400_BCDYEAR	0x48


/* MMU */
#define MMU_I   (1 << 12)
#define MMU_nF  (1 << 30)
#define MMU_iA  (1 << 31)

/* FCLK = 50000000 */
#define 	M_MDIV			0x5c    /* Fin=12.0 MHz, Fout=50.0 MHz */
#define		M_PDIV			0x4
#define 	M_SDIV			0x2
    
/* Memory Controller */
#define		BWSCON			0x14000000
#define		BANKCON0		0x14000004
#define		BANKCON1		0x14000008
#define		BANKCON2		0x1400000C
#define		BANKCON3		0x14000010
#define		BANKCON4		0x14000014
#define		BANKCON5		0x14000018
#define		BANKCON6		0x1400001C
#define		BANKCON7		0x14000020
#define		REFRESH			0x14000024
#define		BANKSIZE		0x14000028
#define		MRSRB6			0x1400002c
#define		MRSRB7			0x14000030

#define B1_BWSCON_32   2
#define B1_BWSCON_16   1
#define B1_BWSCON_8    0
#define B2_BWSCON   1
#define B3_BWSCON   9
#define B4_BWSCON   1
#define B5_BWSCON   1
#define B6_BWSCON   2
#define B7_BWSCON   2

/* bank 0 parameters */
#define B0_Tacs 0x0 /* 0clk */
#define B0_Tcos 0x0 /* 0clk */
#define B0_Tacc 0x7 /* 14clk */
#define B0_Tcoh 0x0 /* 0clk */
#define B0_Tah  0x0 /* 0clk */
#define B0_Tacp 0x0
#define B0_PMC  0x0 /* normal */

/* bank 1 parameters */
#define B1_Tacs 0x0 /* 0clk */
#define B1_Tcos 0x0 /* 0clk */
#define B1_Tacc 0x7 /* 14clk */
#define B1_Tcoh 0x0 /* 0clk */
#define B1_Tah  0x0 /* 0clk */
#define B1_Tacp 0x0
#define B1_PMC  0x0 /* normal */

/* Bank 2 parameters */
#define B2_Tacs 0x0 /* 0clk */
#define B2_Tcos 0x0 /* 0clk */
#define B2_Tacc 0x7 /* 14clk */
#define B2_Tcoh 0x0 /* 0clk */
#define B2_Tah  0x0 /* 0clk */
#define B2_Tacp 0x0
#define B2_PMC  0x0 /* normal */

/* Bank 3 parameters */
#define B3_Tacs 0x0 /* 0clk */
#define B3_Tcos 0x0 /* 0clk */
#define B3_Tacc 0x7 /* 14clk */
#define B3_Tcoh 0x0 /* 0clk */
#define B3_Tah  0x0 /* 0clk */
#define B3_Tacp 0x0
#define B3_PMC  0x0 /* normal */

/* Bank 4 parameters */
#define B4_Tacs 0x0 /* 0clk */
#define B4_Tcos 0x0 /* 0clk */
#define B4_Tacc 0x7 /* 14clk */
#define B4_Tcoh 0x0 /* 0clk */
#define B4_Tah  0x0 /* 0clk */
#define B4_Tacp 0x0
#define B4_PMC  0x0 /* normal */

/* Bank 5 parameters */
#define B5_Tacs 0x0 /* 0clk */
#define B5_Tcos 0x0 /* 0clk */
#define B5_Tacc 0x7 /* 14clk */
#define B5_Tcoh 0x0 /* 0clk */
#define B5_Tah  0x0 /* 0clk */
#define B5_Tacp 0x0
#define B5_PMC  0x0 /* normal */

/* Bank 6 parameters */
#define B6_MT   0x3 /* SDRAM */
#define B6_Trcd 0x1 /* 3clk */
#define B6_SCAN 0x1 /* 9bit */

/* Bank 7 parameters */
#define B7_MT   0x3 /* SDRAM */
#define B7_Trcd 0x1 /* 3clk */
#define B7_SCAN 0x1 /* 9bit */

/* REFRESH parameters */
#define REFEN   0x1 /* Refresh enable */
#define TREFMD  0x0 /* CBR(CAS before RAS)/Auto refresh */
#define Trp     0x0 /* 2clk */
#define Trc     0x3 /* 7clk */

#define Tchr    0x2 /* 3clk */
#define REFCNT  1113 /* period=15.6us, HCLK=60Mhz, (2048+1-15.6*60) */


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
#define		PACON	0x15600000
#define		PADAT	0x15600004
#define		PBCON	0x15600008
#define 	PBDAT	0x1560000C
#define		PBUP	0x15600010
#define		PCCON	0x15600014
#define		PCDAT	0x15600018
#define		PCUP	0x1560001C
#define		PDCON	0x15600020
#define 	PDDAT	0x15600024
#define		PDUP	0x15600028
#define		PECON	0x1560002C
#define		PEDAT	0x15600030
#define		PEUP	0x15600034
#define		PFCON	0x15600038
#define		PFDAT	0x1560003C
#define		PFUP	0x15600040
#define		PGCON	0x15600044
#define		PGDAT	0x15600048
#define		PGUP	0x1560004C
#define 	OPENCR	0x15600050
#define 	EXTINT	0x15600058

/* UART */
#define		S3C2400_UART0_BASE	0x15000000
#define		S3C2400_UART1_BASE	0x15004000
#define		S3C2400_UART_SIZE	0x30

/* offsets from UARTx_BASE */
#define 	S3C2400_ULCON		0x00
#define		S3C2400_UCON		0x04
#define		S3C2400_UFCON		0x08
#define		S3C2400_UMCON		0x0C
#define		S3C2400_UTRSTAT		0x10
#define		S3C2400_UERSTAT		0x14
#define		S3C2400_UFSTAT		0x18
#define		S3C2400_UMSTAT		0x1C
#define		S3C2400_UTXH		0x20
#define		S3C2400_URXH		0x24
#define		S3C2400_UBRDIV		0x28

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
#define		S3C2400_INTR_BASE	0x14400000
#define		S3C2400_INTR_SIZE	0x18

/* offsets from INTR_BASE */
#define		S3C2400_INTR_SRCPND	0x00
#define		S3C2400_INTMOD		0x04
#define		S3C2400_INTMSK		0x08
#define		S3C2400_PRIORITY	0x0C
#define		S3C2400_INTPND		0x10
#define		S3C2400_INTOFFSET	0x14

/* __SRCVERSION("s3c2400.h $Rev: 657836 $"); */
