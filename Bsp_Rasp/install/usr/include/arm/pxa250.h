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
 * Intel PXA250 Processor
 *

 *
 */

#ifndef	ARM_PXA250_H_INCLUDED
#define	ARM_PXA250_H_INCLUDED

/*
 * -------------------------------------------------------------------------
 * Interrupt controller
 * -------------------------------------------------------------------------
 */
#define	PXA250_INTR_BASE	0x40d00000		/* base physical address */
#define	PXA250_INTR_SIZE	0x0000001c

/*
 * Register offsets from PXA250_INTR_BASE
 */
#define	PXA250_ICIP			0x00			/* IRQ pending register */
#define	PXA250_ICMR			0x04			/* mask register */
#define	PXA250_ICLR			0x08			/* level register */
#define	PXA250_ICFP			0x0c			/* FIQ pending register */
#define	PXA250_ICPR			0x10			/* pending register */
#define	PXA250_ICCR			0x14			/* control register */

/*
 * PXA250_ICCR register bits
 */
#define	PXA250_ICCR_DIM		0x00000001		/* disable IDLE mask */

/*
 * -------------------------------------------------------------------------
 * GPIO controller
 * -------------------------------------------------------------------------
 */
#define	PXA250_GPIO_BASE	0x40e00000		/* base physical address */
#define	PXA250_GPIO_SIZE	0x0000006c

/*
 * Register offsets from PXA250_GPIO_BASE
 */
#define	PXA250_GPLR0		0x00			/* pin level GPIO 0-31  */
#define	PXA250_GPLR1		0x04			/* pin level GPIO 31-63 */
#define	PXA250_GPLR2		0x08			/* pin level GPIO 64-80 */
#define	PXA250_GPDR0		0x0c			/* pin direction GPIO 0-31  */
#define	PXA250_GPDR1		0x10			/* pin direction GPIO 31-63 */
#define	PXA250_GPDR2		0x14			/* pin direction GPIO 64-80 */
#define	PXA250_GPSR0		0x18			/* pin set GPIO 0-31  */
#define	PXA250_GPSR1		0x1c			/* pin set GPIO 31-63 */
#define	PXA250_GPSR2		0x20			/* pin set GPIO 64-80 */
#define	PXA250_GPCR0		0x24			/* pin set GPIO 0-31  */
#define	PXA250_GPCR1		0x28			/* pin set GPIO 31-63 */
#define	PXA250_GPCR2		0x2c			/* pin set GPIO 64-80 */
#define	PXA250_GRER0		0x30			/* rising edge detect GPIO 0-31  */
#define	PXA250_GRER1		0x34			/* rising edge detect GPIO 31-63 */
#define	PXA250_GRER2		0x38			/* rising edge detect GPIO 64-80 */
#define	PXA250_GFER0		0x3c			/* falling edge detect GPIO 0-31  */
#define	PXA250_GFER1		0x40			/* falling edge detect GPIO 31-63 */
#define	PXA250_GFER2		0x44			/* falling edge detect GPIO 64-80 */
#define	PXA250_GEDR0		0x48			/* edge detect status GPIO 0-31  */
#define	PXA250_GEDR1		0x4c			/* edge detect status GPIO 31-63 */
#define	PXA250_GEDR2		0x50			/* edge detect status GPIO 64-80 */
#define	PXA250_GAFR0_L		0x54			/* alternate function GPIO 0-15 */
#define	PXA250_GAFR0_U		0x58			/* alternate function GPIO 16-31 */
#define	PXA250_GAFR1_L		0x5c			/* alternate function GPIO 32-47 */
#define	PXA250_GAFR1_U		0x60			/* alternate function GPIO 48-63 */
#define	PXA250_GAFR2_L		0x64			/* alternate function GPIO 64-79 */
#define	PXA250_GAFR2_U		0x68			/* alternate function GPIO 80    */

/*
 * -------------------------------------------------------------------------
 * Serial UARTs
 * -------------------------------------------------------------------------
 */
#define	PXA250_FFUART_BASE	0x40100000		/* base physical address */
#define	PXA250_BTUART_BASE	0x40200000		/* base physical address */
#define	PXA250_STUART_BASE	0x40700000		/* base physical address */
#define	PXA250_UART_SIZE	0x24

/*
 * Register offsets from register base
 */
#define	PXA250_RBR			0x00			/* Rx data */
#define	PXA250_THR			0x00			/* Tx data */
#define	PXA250_IER			0x04			/* interrupt enable */
#define	PXA250_IIR			0x08			/* interrupt identification */
#define	PXA250_FCR			0x08			/* fifo control */
#define	PXA250_LCR			0x0c			/* line control */
#define	PXA250_MCR			0x10			/* modem control */
#define	PXA250_LSR			0x14			/* line status */
#define	PXA250_MSR			0x18			/* modem status */
#define	PXA250_ISR			0x20			/* infrared selection */
#define	PXA250_DLL			0x00			/* divisor latch low  */
#define	PXA250_DLH			0x04			/* divisor latch high */

/*
 * IER bits
 */
#define	PXA250_IER_RAVIE	0x01			/* Rx data available */
#define	PXA250_IER_TIE		0x02			/* Tx request */
#define	PXA250_IER_RLSE		0x04			/* Rx line status */
#define	PXA250_IER_MIE		0x08			/* modem status */
#define	PXA250_IER_RTOIE	0x10			/* Rx timeout */
#define	PXA250_IER_NRZE		0x20			/* NRZ encoding */
#define	PXA250_IER_UUE		0x40			/* uart enable */
#define	PXA250_IER_DMAE		0x80			/* DMA request */

/*
 * IIR bits
 */
#define	PXA250_IIR_IP		0x01			/* interrupt pending */
#define	PXA250_IIR_IID		0x06			/* interrupt id mask */
#define	PXA250_IIR_IID_MS	0x00			/* modem status */
#define	PXA250_IIR_IID_TX	0x02			/* Tx fifo request */
#define	PXA250_IIR_IID_RX	0x04			/* Rx data available */
#define	PXA250_IIR_IID_RE	0x06			/* RX error */
#define	PXA250_IIR_TOD		0x08			/* timeout pending */
#define	PXA250_IIR_FIFOES	0xc0			/* fifo mode mask */
#define	PXA250_IIR_FIFO_EN	0xc0			/* fifo mode enabled */

/*
 * FCR bits
 */
#define	PXA250_FCR_TRFIFOE	0x01			/* Tx/Tx fifo enable */
#define	PXA250_FCR_RESETRF	0x02			/* reset Rx fifo */
#define	PXA250_FCR_RESETTF	0x04			/* reset Tx fifo */
#define	PXA250_FCR_ITL		0xc0			/* interrupt trigger level mask */
#define	PXA250_FCR_ITL_1	0x00			/* 1 byte trigger */
#define	PXA250_FCR_ITL_8	0x40			/* 8 byte trigger */
#define	PXA250_FCR_ITL_16	0x80			/* 16 byte trigger */
#define	PXA250_FCR_ITL_32	0xc0			/* 32 byte trigger */

/*
 * LCR bits
 */
#define	PXA250_LCR_WLS_5	0x00			/* 5-bit characters */
#define	PXA250_LCR_WLS_6	0x01			/* 6-bit characters */
#define	PXA250_LCR_WLS_7	0x02			/* 7-bit characters */
#define	PXA250_LCR_WLS_8	0x03			/* 8-bit characters */
#define	PXA250_LCR_STB		0x04			/* 2 stop bits */
#define	PXA250_LCR_PEN		0x08			/* parity enable */
#define	PXA250_LCR_EPS		0x10			/* even parity */
#define	PXA250_LCR_STKYP	0x20			/* sticky parity */
#define	PXA250_LCR_SB		0x40			/* set break */
#define	PXA250_LCR_DLAB		0x80			/* divisor latch access */

/*
 * LSR bits
 */
#define	PXA250_LSR_DR		0x01			/* data ready */
#define	PXA250_LSR_OE		0x02			/* overrun error */
#define	PXA250_LSR_PE		0x04			/* parity error */
#define	PXA250_LSR_FE		0x08			/* framing error */
#define	PXA250_LSR_BI		0x10			/* break interrupt */
#define	PXA250_LSR_TDRQ		0x20			/* Tx data request */
#define	PXA250_LSR_TEMT		0x40			/* transmitter empty */
#define	PXA250_LSR_FIFOE	0x80			/* fifo error status */

/*
 * MCR bits
 */
#define	PXA250_MCR_DTR		0x01			/* 0 = nDTR set to 1 */
#define	PXA250_MCR_RTS		0x02			/* 0 = nCTS set to 1 */
#define	PXA250_MCR_OUT1		0x04			/* test bit */
#define	PXA250_MCR_OUT2		0x08			/* 0 = disable UART interrupt */

/*
 * MSR bits
 */
#define	PXA250_MSR_DCTS		0x01			/* CTS delta */
#define	PXA250_MSR_DDSR		0x02			/* DSR delta */
#define	PXA250_MSR_TERI		0x04			/* RI trailing edge */
#define	PXA250_MSR_DDCD		0x08			/* DCD delta */
#define	PXA250_MSR_CTS		0x10			/* 0 = nCTS is 1 */
#define	PXA250_MSR_DSR		0x20			/* 0 = nDSR is 1 */
#define	PXA250_MSR_RI		0x40			/* 0 = nRI is 1 */
#define	PXA250_MSR_DCD		0x80			/* 0 = nDCD is 1 */

/*
 * ISR bits
 */
#define	PXA250_ISR_XMITIR	0x01			/* Tx SIR enable */
#define	PXA250_ISR_RCVEIR	0x02			/* Rx SIR enable */
#define	PXA250_ISR_XMODE	0x04			/* Tx pulse width */
#define	PXA250_ISR_TXPL		0x08			/* Tx polarity */
#define	PXA250_ISR_RXPL		0x10			/* Rx polarity */

/*
 * -------------------------------------------------------------------------
 * OS Timer
 * -------------------------------------------------------------------------
 */
#define	PXA250_TIMER_BASE	0x40a00000	/* physical base address */
#define	PXA250_TIMER_SIZE	0x00000020

/*
 * Register offsets from PXA250_TIMER_BASE
 */
#define	PXA250_OSMR0		0x00		/* match register 0 */
#define	PXA250_OSMR1		0x04		/* match register 1 */
#define	PXA250_OSMR2		0x08		/* match register 2 */
#define	PXA250_OSMR3		0x0c		/* match register 3 */
#define	PXA250_OSCR			0x10		/* count register */
#define	PXA250_OSSR			0x14		/* status register */
#define	PXA250_OWER			0x18		/* watchdog enable register */
#define	PXA250_OIER			0x1c		/* interrupt enable register */

/*
 * OWER bits
 */
#define	PXA250_OWER_WME		0x00000001	/* enable watchdog reset on match 3 */

/*
 * OSSR bits
 */
#define	PXA250_OSSR_M0		0x00000001	/* channel 0 has matched */
#define	PXA250_OSSR_M1		0x00000002	/* channel 1 has matched */
#define	PXA250_OSSR_M2		0x00000004	/* channel 1 has matched */
#define	PXA250_OSSR_M4		0x00000008	/* channel 1 has matched */

/*
 * OIER bits
 */
#define	PXA250_OIER_E0		0x00000001	/* enable interrupt for match 0 */
#define	PXA250_OIER_E1		0x00000002	/* enable interrupt for match 1 */
#define	PXA250_OIER_E2		0x00000004	/* enable interrupt for match 2 */
#define	PXA250_OIER_E4		0x00000008	/* enable interrupt for match 3 */

/*
 * -------------------------------------------------------------------------
 * LCD Controller
 * -------------------------------------------------------------------------
 */
#define	PXA250_LCD_BASE		0x44000000	/* physical base address */
#define	PXA250_LCD_SIZE		0x00000220	/* size of registers */

/*
 * Register offsets from PXA250_LCD_BASE
 */
#define	PXA250_LCCR0		0x00		/* control register 0 */
#define	PXA250_LCCR1		0x04		/* control register 1 */
#define	PXA250_LCCR2		0x08		/* control register 2 */
#define	PXA250_LCCR3		0x0c		/* control register 3 */
#define	PXA250_FBR0			0x20		/* DMA 0 frame branch */
#define	PXA250_FBR1			0x24		/* DMA 1 frame branch */
#define	PXA250_LCSR			0x38		/* status */
#define	PXA250_LIIDR		0x3c		/* interrupt ID */
#define	PXA250_TRGBR		0x40		/* TMED RGB seed */
#define	PXA250_TCR			0x44		/* TMED control */
#define	PXA250_FDADR0		0x200		/* DMA 0 frame descriptor address */
#define	PXA250_FSADR0		0x204		/* DMA 0 frame source address */
#define	PXA250_FIDR0		0x208		/* DMA 0 frame ID */
#define	PXA250_LDCMD0		0x20c		/* DMA 0 command */
#define	PXA250_FDADR1		0x210		/* DMA 1 frame descriptor address */
#define	PXA250_FSADR1		0x214		/* DMA 1 frame source address */
#define	PXA250_FIDR1		0x218		/* DMA 1 frame ID */
#define	PXA250_LDCMD1		0x21c		/* DMA 1 command */

/*
 * LCCR0 bits
 */
#define	PXA250_LCCR0_ENB	0x00000001	/* LCD enable */
#define	PXA250_LCCR0_CMS	0x00000002	/* color/monochrome select */
#define	PXA250_LCCR0_SDS	0x00000004	/* single/dual panel select */
#define	PXA250_LCCR0_LDM	0x00000008	/* mask LCD disable done */
#define	PXA250_LCCR0_SFM	0x00000010	/* mask start of frame interrupt */
#define	PXA250_LCCR0_IUM	0x00000020	/* mask input fifo underrun */
#define	PXA250_LCCR0_EFM	0x00000040	/* mask end of frame interrupt */
#define	PXA250_LCCR0_PAS	0x00000080	/* active/passive select */
#define	PXA250_LCCR0_BLE	0x00000100	/* big-endian select */
#define	PXA250_LCCR0_DPD	0x00000200	/* double-pixel data */
#define	PXA250_LCCR0_DIS	0x00000400	/* LCD disable */
#define	PXA250_LCCR0_QDM	0x00000800	/* mask quick disable interrupt */
#define	PXA250_LCCR0_BM		0x00100000	/* mask branch interrupt */
#define	PXA250_LCCR0_OUM	0x00200000	/* mask output fifo underrun */
#define	PXA250_LCCR0_PDD(x)	(((x) & 0xff) << 12)

/*
 * LCCR1 bits
 */
#define	PXA250_LCCR1_PPL(x)	((x) & 0x3ff)
#define	PXA250_LCCR1_HSW(x)	(((x) & 0x3f) << 10)
#define	PXA250_LCCR1_ELW(x)	(((x) & 0xff) << 16)
#define	PXA250_LCCR1_BLW(x)	(((x) & 0xff) << 24)

/*
 * LCCR2 bits
 */
#define	PXA250_LCCR2_LPP(x)	((x) & 0x3ff)
#define	PXA250_LCCR2_VSW(x)	(((x) & 0x3f) << 10)
#define	PXA250_LCCR2_EFW(x)	(((x) & 0xff) << 16)
#define	PXA250_LCCR2_BFW(x)	(((x) & 0xff) << 24)

/*
 * LCCR3 bits
 */
#define	PXA250_LCCR3_PCD(x)	((x) & 0xff)
#define	PXA250_LCCR3_ACB(x)	(((x) & 0xff) << 8)
#define	PXA250_LCCR3_API(x)	(((x) & 0xf) << 16)
#define	PXA250_LCCR3_VSP	0x00100000
#define	PXA250_LCCR3_HSP	0x00200000
#define	PXA250_LCCR3_PCP	0x00400000
#define	PXA250_LCCR3_OEP	0x00800000
#define	PXA250_LCCR3_1BPP	0x00000000
#define	PXA250_LCCR3_2BPP	0x01000000
#define	PXA250_LCCR3_4BPP	0x02000000
#define	PXA250_LCCR3_8BPP	0x03000000
#define	PXA250_LCCR3_16BPP	0x04000000
#define	PXA250_LCCR3_DPC	0x08000000


/*
 * -------------------------------------------------------------------------
 * Clock Manager 
 * -------------------------------------------------------------------------
 */

#define PXA250_CCCR			0x41300000		/* Core Clock Configuration Register */
#define PXA250_CKEN			0x41300004		/* Clock Enable Register */
#define PXA250_OSCC			0x41300008		/* Oscillator Control Register */

/*
 * -------------------------------------------------------------------------
 * Memory Controller 
 * -------------------------------------------------------------------------
 */

#define	PXA250_MEM_CTLR_BASE	0x48000000
#define PXA250_MEM_CTLR_SIZE	0x00000048

/*
 * Register offsets from PXA250_MEM_CTLR_BASE
 */

#define PXA250_MDCNFG			0x00	/* SDRAM Configuration Register */
#define PXA250_MDREFR			0x04	/* SDRAM Refresh Control Register */
#define PXA250_MSC0				0x08	/* Static Memory Control Register 0 */
#define PXA250_MSC1				0x0c	/* Static Memory Control Register 1 */
#define PXA250_MSC2				0x10	/* Static Memory Control Register 2 */
#define PXA250_MECR				0x14	/* Expansion Memory Configuration Register */
#define PXA250_SXLCR			0x18	/* LCR Value Register */
#define PXA250_SXCNFG			0x1c	/* Synchronous Static Memory Control Register */
#define PXA250_SXMRS			0x24	/* MRS Value Register */ 
#define PXA250_MCMEM0			0x28	/* Card Interface Common Memory Space 0 Timing */
#define PXA250_MCMEM1			0x2c	/* Card Interface Common Memory Space 1 Timing */
#define PXA250_MCATT0			0x30	/* Card Interface Attribute Space 0 Timing */
#define PXA250_MCATT1			0x34	/* Card Interface Attribute Space 1 Timing */
#define PXA250_MCIO0			0x38	/* Card Interface I/O Space 0 Timing */
#define PXA250_MCIO1			0x3c	/* Card Interface I/O Space 1 Timing */
#define PXA250_MDMRS			0x40	/* MRS Value Register */

/*
 * -------------------------------------------------------------------------
 * RTC Controller 
 * -------------------------------------------------------------------------
 */

#define	PXA250_RTC_BASE			0x40900000
#define	PXA250_RTC_SIZE			0x00000010

/*
 * Register offsets from PXA250_RTC_BASE
 */

#define	PXA250_RCNR				0x00	/* Counter register */
#define	PXA250_RTAR				0x04	/* Alarm register */
#define	PXA250_RTSR				0x08	/* Status register */
#define	PXA250_RTTR				0x0c	/* Trim register */

#endif

/* __SRCVERSION("pxa250.h $Rev: 657836 $"); */
