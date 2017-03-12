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
 * ARM Integrator/AP Board
 *

 *
 */

#ifndef	__ARM_INTEGRATOR_H_INCLUDED
#define	__ARM_INTEGRATOR_H_INCLUDED

/*
 * ------------------------------------------------------------------------
 * Clock/Timer Controller
 * ------------------------------------------------------------------------
 */

#define	INTEGRATOR_TIMER_BASE		0x13000000	/* physical base address */
#define	INTEGRATOR_TIMER_SIZE		0x00000210

/*
 * Register offsets from INTEGRATOR_TIMER_BASE
 */
#define	INTEGRATOR_TIMER0_LOAD		0x000
#define	INTEGRATOR_TIMER0_VALUE		0x004
#define	INTEGRATOR_TIMER0_CTRL		0x008
#define	INTEGRATOR_TIMER0_CLR		0x00c
#define	INTEGRATOR_TIMER1_LOAD		0x100
#define	INTEGRATOR_TIMER1_VALUE		0x104
#define	INTEGRATOR_TIMER1_CTRL		0x108
#define	INTEGRATOR_TIMER1_CLR		0x10c
#define	INTEGRATOR_TIMER2_LOAD		0x200
#define	INTEGRATOR_TIMER2_VALUE		0x204
#define	INTEGRATOR_TIMER2_CTRL		0x208
#define	INTEGRATOR_TIMER2_CLR		0x20c

/*
 * TIMER_CTRL bits
 */
#define	INTEGRATOR_TIMER_CTRL_ENABLE		0x80	/* enable timer */
#define	INTEGRATOR_TIMER_CTRL_MODE			0x40	/* timer mode */
#define	INTEGRATOR_TIMER_CTRL_PRESCALE		0x0c	/* prescale divider */

#define	INTEGRATOR_TIMER_CTRL_PERIODIC		INTEGRATOR_TIMER_CTRL_MODE
#define	INTEGRATOR_TIMER_CTRL_PRESCALE_16	0x04	/* divide clock by 16 */
#define	INTEGRATOR_TIMER_CTRL_PRESCALE_256	0x08	/* divide clock by 256 */

/*
 * ------------------------------------------------------------------------
 * Interrupt Controller
 * ------------------------------------------------------------------------
 */

#define	INTEGRATOR_INTR_BASE		0x14000000	/* physical base address */
#define	INTEGRATOR_INTR_SIZE		0x000000f0

/*
 * Register offsets from INTEGRATOR_INTR_BASE
 */
#define	INTEGRATOR_IRQ0_STATUS		0x00
#define	INTEGRATOR_IRQ0_RAWSTAT		0x04
#define	INTEGRATOR_IRQ0_ENABLESET	0x08
#define	INTEGRATOR_IRQ0_ENABLECLR	0x0c
#define	INTEGRATOR_IRQ1_STATUS		0x40
#define	INTEGRATOR_IRQ1_RAWSTAT		0x44
#define	INTEGRATOR_IRQ1_ENABLESET	0x48
#define	INTEGRATOR_IRQ1_ENABLECLR	0x4c
#define	INTEGRATOR_IRQ2_STATUS		0x80
#define	INTEGRATOR_IRQ2_RAWSTAT		0x84
#define	INTEGRATOR_IRQ2_ENABLESET	0x88
#define	INTEGRATOR_IRQ2_ENABLECLR	0x8c
#define	INTEGRATOR_IRQ3_STATUS		0xc0
#define	INTEGRATOR_IRQ3_RAWSTAT		0xc4
#define	INTEGRATOR_IRQ3_ENABLESET	0xc8
#define	INTEGRATOR_IRQ3_ENABLECLR	0xcc
#define	INTEGRATOR_FIQ0_STATUS		0x20
#define	INTEGRATOR_FIQ0_RAWSTAT		0x24
#define	INTEGRATOR_FIQ0_ENABLESET	0x28
#define	INTEGRATOR_FIQ0_ENABLECLR	0x2c
#define	INTEGRATOR_FIQ1_STATUS		0x60
#define	INTEGRATOR_FIQ1_RAWSTAT		0x64
#define	INTEGRATOR_FIQ1_ENABLESET	0x68
#define	INTEGRATOR_FIQ1_ENABLECLR	0x6c
#define	INTEGRATOR_FIQ2_STATUS		0xa0
#define	INTEGRATOR_FIQ2_RAWSTAT		0xa4
#define	INTEGRATOR_FIQ2_ENABLESET	0xa8
#define	INTEGRATOR_FIQ2_ENABLECLR	0xac
#define	INTEGRATOR_FIQ3_STATUS		0xe0
#define	INTEGRATOR_FIQ3_RAWSTAT		0xe4
#define	INTEGRATOR_FIQ3_ENABLESET	0xe8
#define	INTEGRATOR_FIQ3_ENABLECLR	0xec
#define	INTEGRATOR_INT_SOFTSET		0x10
#define	INTEGRATOR_INT_SOFTCLEAR	0x14

/*
 * Interrupt vector numbers
 */
#define	INTEGRATOR_INTR_SOFTINT		0
#define	INTEGRATOR_INTR_UART0		1
#define	INTEGRATOR_INTR_UART1		2
#define	INTEGRATOR_INTR_KDB			3
#define	INTEGRATOR_INTR_MOUSE		4
#define	INTEGRATOR_INTR_TIMER0		5
#define	INTEGRATOR_INTR_TIMER1		6
#define	INTEGRATOR_INTR_TIMER2		7
#define	INTEGRATOR_INTR_RTC			8
#define	INTEGRATOR_INTR_EXP0		9
#define	INTEGRATOR_INTR_EXP1		10
#define	INTEGRATOR_INTR_EXP2		11
#define	INTEGRATOR_INTR_EXP3		12
#define	INTEGRATOR_INTR_PCIINT0		13
#define	INTEGRATOR_INTR_PCIINT1		14
#define	INTEGRATOR_INTR_PCIINT2		15
#define	INTEGRATOR_INTR_PCIINT3		16
#define	INTEGRATOR_INTR_LINT		17
#define	INTEGRATOR_INTR_DEG			18
#define	INTEGRATOR_INTR_ENUM		19
#define	INTEGRATOR_INTR_PCILB		20
#define	INTEGRATOR_INTR_APC			21

/*
 * ------------------------------------------------------------------------
 * PrimeCell Peripherals
 *
 * See arm/primecell.h for register definitions.
 * ------------------------------------------------------------------------
 */

#define	INTEGRATOR_RTC_BASE		0x15000000	/* physical base address */
#define	INTEGRATOR_UART0_BASE	0x16000000	/* physical base address */
#define	INTEGRATOR_UART1_BASE	0x17000000	/* physical base address */
#define	INTEGRATOR_KBD_BASE		0x18000000	/* physical base address */
#define	INTEGRATOR_MOUSE_BASE	0x19000000	/* physical base address */
#define	INTEGRATOR_GPIO_BASE	0x1b000000	/* physical base address */

/*
 * ------------------------------------------------------------------------
 * System Controller
 * ------------------------------------------------------------------------
 */

#define	INTEGRATOR_SC_BASE		0x11000000	/* physical base address */
#define	INTEGRATOR_SC_SIZE		0x00000028

/*
 * Register offsets from INTEGRATOR_SC_BASE
 */
#define	INTEGRATOR_SC_ID		0x00		/* system controller identification */
#define	INTEGRATOR_SC_OSC		0x04		/* oscillator dividers */
#define	INTEGRATOR_SC_CTRLS		0x08		/* control register set */
#define	INTEGRATOR_SC_CTRLC		0x0c		/* control register clear */
#define	INTEGRATOR_SC_DEC		0x10		/* decoder status */
#define	INTEGRATOR_SC_ARB		0x14		/* arbiter timeout values */
#define	INTEGRATOR_SC_PCI		0x18		/* PCI control */
#define	INTEGRATOR_SC_LOCK		0x1c		/* lock */
#define	INTEGRATOR_SC_LBFADDR	0x20		/* local bus fault addr */
#define	INTEGRATOR_SC_LBFCODE	0x24		/* local bus fault code */

/*
 * SC_OSC register bits
 */
#define	INTEGRATOR_SC_OSC_PCI_25MHz		0x100	/* 25MHz PCI bus */
#define	INTEGRATOR_SC_OSC_PCI_33MHz		0x000	/* 33MHz PCI bus */
#define	INTEGRATOR_SC_OSC_SYS_10MHz		0x020	/* 10MHz system bus */
#define	INTEGRATOR_SC_OSC_SYS_15MHz		0x034	/* 15MHz system bus */
#define	INTEGRATOR_SC_OSC_SYS_20MHz		0x048	/* 20MHz system bus */
#define	INTEGRATOR_SC_OSC_SYS_25MHz		0x05c	/* 25MHz system bus */
#define	INTEGRATOR_SC_OSC_SYS_33MHz		0x07c	/* 33MHz system bus */

/*
 * SC_CTRLS/SC_CTRLC register bits
 */
#define	INTEGRATOR_SC_CTRL_UART0RTS		0x80	/* uart0 rts line */
#define	INTEGRATOR_SC_CTRL_UART0DTR		0x40	/* uart0 dtr line */
#define	INTEGRATOR_SC_CTRL_UART1RTS		0x20	/* uart1 rts line */
#define	INTEGRATOR_SC_CTRL_UART1DTR		0x10	/* uart1 dtr line */
#define	INTEGRATOR_SC_CTRL_FLASHWP		0x04	/* flash write protect */
#define	INTEGRATOR_SC_CTRL_FLASHVPP		0x02	/* flash Vpp enable */
#define	INTEGRATOR_SC_CTRL_SOFTRESET	0x01	/* software reset */

/*
 * SC_PCI register bits
 */
#define	INTEGRATOR_SC_PCI_PCIEN			0x02	/* enable PCI interface */
#define	INTEGRATOR_SC_PCI_CLR			0x01	/* clear PCILBINT */

/*
 * SC_LOCK register values
 */
#define	INTEGRATOR_SC_LOCK_LCK			0x00010000	/* lock status */
#define	INTEGRATOR_SC_LOCK_LOCK			0x00000000	/* lock SC_OSC */
#define	INTEGRATOR_SC_LOCK_UNLOCK		0x0000a05f	/* unlock SC_OSC */

/*
 * ------------------------------------------------------------------------
 * Expansion Bus Interface Configuration Registers
 * ------------------------------------------------------------------------
 */

#define	INTEGRATOR_EBI_BASE			0x12000000	/* physical base address */
#define	INTEGRATOR_EBI_SIZE			0x00000024

/*
 * Register offsets from INTEGRATOR_EBI_BASE
 */
#define	INTEGRATOR_EBI_CSR0			0x00		/* chip select 0 (boot ROM) */
#define	INTEGRATOR_EBI_CSR1			0x04		/* chip select 1 (flash) */
#define	INTEGRATOR_EBI_CSR2			0x08		/* chip select 2 (SSRAM) */
#define	INTEGRATOR_EBI_CSR3			0x0c		/* chip select 3 (EXPM) */
#define	INTEGRATOR_EBI_LOCK			0x20		/* lock EBI_CSRx */

/*
 * EBI register bits
 */
#define	INTEGRATOR_EBI_8BIT			0x00		/*  8 bit memory */
#define	INTEGRATOR_EBI_16BIT		0x01		/* 16 bit memory */
#define	INTEGRATOR_EBI_32BIT		0x02		/* 32 bit memory */
#define	INTEGRATOR_EBI_WREN			0x04		/* write enable */
#define	INTEGRATOR_EBI_SSRAM		0x08		/* SSRAM */
#define	INTEGRATOR_EBI_WAIT_2		0x00		/*  2 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_3		0x01		/*  3 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_4		0x02		/*  4 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_5		0x03		/*  5 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_6		0x04		/*  6 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_7		0x05		/*  7 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_8		0x06		/*  8 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_9		0x07		/*  9 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_10		0x08		/* 10 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_11		0x09		/* 11 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_12		0x0a		/* 12 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_13		0x0b		/* 13 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_14		0x0c		/* 14 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_15		0x0d		/* 15 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_16		0x0e		/* 16 cycle wait state */
#define	INTEGRATOR_EBI_WAIT_17		0x0f		/* 17 cycle wait state */

#define	INTEGRATOR_EBI_LOCK_LOCK	0x0000		/* lock EBI_CSRx */
#define	INTEGRATOR_EBI_LOCK_UNLOCK	0xa05f		/* unlock EBI_CSRx */

/*
 * ------------------------------------------------------------------------
 * Core Module Registers
 * ------------------------------------------------------------------------
 */

#define	INTEGRATOR_CM_BASE			0x10000000	/* physical base address */
#define	INTEGRATOR_CM_SIZE			0x00000200

/*
 * Register offsets from INTEGRATOR_CM_BASE
 */
#define	INTEGRATOR_CM_ID			0x00		/* core module id */
#define	INTEGRATOR_CM_PROC			0x04		/* processor */
#define	INTEGRATOR_CM_OSC			0x08		/* oscillator control */
#define	INTEGRATOR_CM_CTRL			0x0c		/* control */
#define	INTEGRATOR_CM_STAT			0x10		/* status */
#define	INTEGRATOR_CM_LOCK			0x14		/* lock CM_OSC */
#define	INTEGRATOR_CM_SDRAM			0x20		/* SDRAM status and control */
#define	INTEGRATOR_CM_IRQ_STAT		0x40		/* core module IRQ status */
#define	INTEGRATOR_CM_IRQ_RSTAT		0x44		/* core module raw IRQ status */
#define	INTEGRATOR_CM_IRQ_ENSET		0x48		/* core module IRQ enable set */
#define	INTEGRATOR_CM_IRQ_ENCLR		0x4c		/* core module IRQ enable clear */
#define	INTEGRATOR_CM_SOFT_INTSET	0x50		/* software interrupt set */
#define	INTEGRATOR_CM_SOFT_INTCLR	0x54		/* software interrupt clear */
#define	INTEGRATOR_CM_FIQ_STAT		0x60		/* core module FIQ status */
#define	INTEGRATOR_CM_FIQ_RSTAT		0x64		/* core module FIQ raw status */
#define	INTEGRATOR_CM_FIQ_ENSET		0x68		/* core module FIQ enable set */
#define	INTEGRATOR_CM_FIQ_ENCLR		0x6c		/* core module FIQ enable clear */
#define	INTEGRATOR_CM_SPD			0x100		/* SDRAM SPD memory */

/*
 * CM_OSC register bits
 */
#define	INTEGRATOR_CM_OS_BMODE_MASK		0x01800000	/* bus mode */
#define	INTEGRATOR_CM_OSC_L_OD_2		0x00100000
#define	INTEGRATOR_CM_OSC_L_VDW(x)		(((2 * (x)) - 8) << 12)
#define	INTEGRATOR_CM_OSC_C_OD_2		0x00000100
#define	INTEGRATOR_CM_OSC_C_VDW(x)		((x) - 8)

/*
 * CM_CTRL register bits
 */
#define	INTEGRATOR_CM_CTRL_LED			0x00000001	/* MISC LED (1 = on) */
#define	INTEGRATOR_CM_CTRL_nMBDET		0x00000002	/* motherboard detect */
#define	INTEGRATOR_CM_CTRL_REMAP		0x00000004	/* remap memory at 0-256K */
#define	INTEGRATOR_CM_CTRL_RESET		0x00000008	/* reset all modules */
#define	INTEGRATOR_CM_CTRL_FASTBUS		0x00000010	/* 0=async bus, 1=fastbus */

/*
 * CM_LOCK register bits
 */
#define	INTEGRATOR_CM_LOCK_LCK			0x00010000	/* lock status */
#define	INTEGRATOR_CM_LOCK_LOCK			0x00000000	/* lock CM_OSC */
#define	INTEGRATOR_CM_LOCK_UNLOCK		0x0000a05f	/* unlock CM_OSC */

/*
 * CM_SDRAM register bits
 */
#define	INTEGRATOR_CM_SDRAM_CASLAT_2	0x00000002	/* 2 cycle latency */
#define	INTEGRATOR_CM_SDRAM_CASLAT_3	0x00000003	/* 2 cycle latency */
#define	INTEGRATOR_CM_SDRAM_SPDOK		0x00000020	/* SPD data ready */
#define	INTEGRATOR_CM_SDRAM_MSIZE_16M	0x00000000	/*  16MB SDRAM */
#define	INTEGRATOR_CM_SDRAM_MSIZE_32M	0x00000004	/*  32MB SDRAM */
#define	INTEGRATOR_CM_SDRAM_MSIZE_64M	0x00000008	/*  64MB SDRAM */
#define	INTEGRATOR_CM_SDRAM_MSIZE_128M	0x0000000c	/* 128MB SDRAM */
#define	INTEGRATOR_CM_SDRAM_MSIZE_256M	0x00000010	/* 256MB SDRAM */
#define	INTEGRATOR_CM_SDRAM_NROWS(x)	((x) << 8)	/* set NROWS */
#define	INTEGRATOR_CM_SDRAM_NCOLS(x)	((x) << 12)	/* set NCOLS */
#define	INTEGRATOR_CM_SDRAM_NBANKS(x)	((x) << 16)	/* set NBANKS */

/*
 * CM_IRQ and CM_FIQ registers
 */
#define	INTEGRATOR_CM_INT_SOFT			0x00000001	/* software interrupt */
#define	INTEGRATOR_CM_INT_COMMRx		0x00000002	/* debug comms Rx */
#define	INTEGRATOR_CM_INT_COMMTx		0x00000004	/* debug comms Tx */

#endif	/* __ARM_INTEGRATOR_H_INCLUDED */

/* __SRCVERSION("integrator.h $Rev: 657836 $"); */
