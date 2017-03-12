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
 * MX1 processor
 *

 *
 */

#ifndef	__ARM_MX1_H_INCLUDED
#define	__ARM_MX1_H_INCLUDED

/*
 * Embedded SRAM base
 */
#define	MX1_ESRAM_BASE			0x00300000

/*
 * -------------------------------------------------------------------------
 * WatchDog Timer registers
 * -------------------------------------------------------------------------
 */
#define	MX1_WDT_BASE			0x00201000	/* Base Physical Address */
#define	MX1_WDT_SIZE			0x0C

/*
 * Register offsets from MX1_WDT_BASE
 */
#define	MX1_WDT_WCR				0x00		/* WatchDog Control Register */
#define	MX1_WDT_WSR				0x04		/* WatchDog Service Register */
#define	MX1_WDT_WSTR			0x08		/* WatchDog Status Register */


/*
 * -------------------------------------------------------------------------
 * General-Purpose Timer registers
 * -------------------------------------------------------------------------
 */

#define	MX1_TIMER1_BASE			0x00202000	/* Timer 1 BASE */
#define	MX1_TIMER2_BASE			0x00203000	/* Timer 2 BASE */
#define	MX1_TIMER_SIZE			0x00000018

/*
 * Register offsets from MX1_TIMER_BASE
 */
#define	MX1_TIMER_CTL			0x00		/* Timer Control Register */
#define	MX1_TIMER_PRER			0x04		/* Timer Prescaler Register */
#define	MX1_TIMER_CMP			0x08		/* Timer Compare Register */
#define	MX1_TIMER_CR			0x0C		/* Timer Capture Register */
#define	MX1_TIMER_CN			0x10		/* Timer Counter Register */
#define	MX1_TIMER_STAT			0x14		/* Timer Status Register */

/*
 * -------------------------------------------------------------------------
 * RTC Registers
 * -------------------------------------------------------------------------
 */
#define	MX1_RTC_BASE			0x00204000
#define	MX1_RTC_SIZE			0x28

/* 
 * RTC Registers, offset from base address 
 */
#define	MX1_RTC_HOURMIN			0x00		/* RTC Hours and Minutes Counter Register */
#define	MX1_RTC_SECONDS			0x04		/* RTC Seconds Counter Register */
#define	MX1_RTC_ALRM_HM			0x08		/* RTC Hours and Minutes Alarm Register */
#define	MX1_RTC_ALRM_SEC		0x0C		/* RTC Seconds Alarm Register */
#define	MX1_RTC_RCCTL			0x10		/* RTC Control Register */
#define	MX1_RTC_ISR				0x14		/* RTC Interrupt Status Register */
#define	MX1_RTC_IENR			0x18		/* RTC Interrupt Enable Register */
#define	MX1_RTC_STPWCH			0x1C		/* Stopwatch Minutes Register */
#define	MX1_RTC_DAYR			0x20		/* RTC Days Counter Register */
#define	MX1_RTC_DAYALARM		0x24		/* RTC Day Alarm Register */

/*
 * -------------------------------------------------------------------------
 * Serial ports
 * -------------------------------------------------------------------------
 */

/* UART1 base address */
#define	MX1_UART1_BASE			0x00206000

/* UART2 base address */
#define	MX1_UART2_BASE			0x00207000

#define	MX1_UART_SIZE			0xE0

/* UART registers, offset from base address */
#define	MX1_UART_RXDATA			0x00		/* Receiver Register */
#define	MX1_UART_TXDATA			0x40		/* Transmitter Register */
#define	MX1_UART_CR1			0x80		/* Control Register 1 */
#define	MX1_UART_CR2			0x84		/* Control Register 2 */
#define	MX1_UART_CR3			0x88		/* Control Register 3 */
#define	MX1_UART_CR4			0x8C		/* Control Register 4 */
#define	MX1_UART_FCR			0x90		/* FIFO Control Register */
#define	MX1_UART_SR1			0x94		/* Status Register 1 */
#define	MX1_UART_SR2			0x98		/* Status Register 2 */
#define	MX1_UART_ESC			0x9C		/* Escape Character Register */
#define	MX1_UART_TIM			0xA0		/* Escape Timer Register */
#define	MX1_UART_BIR			0xA4		/* BRM Incremental Register */
#define	MX1_UART_BMR			0xA8		/* BRM Modulator Register */
#define	MX1_UART_BRC			0xAC		/* Baud Rate Count Register */
#define	MX1_UART_BIPR1			0xB0		/* BRM Incremental Preset Register 1 */
#define	MX1_UART_BIPR2			0xB4		/* BRM Incremental Preset Register 2 */
#define	MX1_UART_BIPR3			0xB8		/* BRM Incremental Preset Register 3 */
#define	MX1_UART_BIPR4			0xBC		/* BRM Incremental Preset Register 4 */
#define	MX1_UART_BMPR1			0xC0		/* BRM Modulator Preset Register */
#define	MX1_UART_BMPR2			0xC4		/* BRM Modulator Preset Register */
#define	MX1_UART_BMPR3			0xC8		/* BRM Modulator Preset Register */
#define	MX1_UART_BMPR4			0xCC		/* BRM Modulator Preset Register */
#define	MX1_UART_TS				0xD0		/* Test Register */

/* 
 * Receiver Register bits
 */
#define	MX1_URXD_CHARRDY		(1<<15)		/* Character Ready */
#define	MX1_URXD_ERR			(1<<14)		/* Error Detect */
#define	MX1_URXD_OVERRUN		(1<<13)		/* Receiver Overrun */
#define	MX1_URXD_FRMERR			(1<<12)		/* Frame Error */
#define	MX1_URXD_BRK			(1<<11)		/* BREAK detect */
#define	MX1_URXD_PRERR			(1<<10)		/* Parity Error */

/* 
 * Control Register 1 bits
 */
#define	MX1_UCR1_ADEN			(1<<15)		/* Automatic Baud Rate Detection Interrupt Enable */
#define	MX1_UCR1_ADBR			(1<<14)		/* Automatic Detection of Baud Rate */
#define	MX1_UCR1_TRDYEN			(1<<13)		/* Transmitter Ready Interrupt Enable */
#define	MX1_UCR1_IDEN			(1<<12)		/* Idle Condition Detected Interrupt */
#define	MX1_UCR1_ICD_MASK		(3<<10)		/* Idle Condition Detect Mask */
#define	MX1_UCR1_RRDYEN			(1<<9)		/* Receiver Ready Interrupt Enable */
#define	MX1_UCR1_RDMAEN			(1<<8)		/* Receive Ready DMA Enable */
#define	MX1_UCR1_IREN			(1<<7)		/* Infrared Interface Enable */
#define	MX1_UCR1_TXMPTYEN		(1<<6)		/* Transmitter Empty Interrupt Enable */
#define	MX1_UCR1_RTSDEN			(1<<5)		/* RTS Delta Interrupt Enable */
#define	MX1_UCR1_SNDBRK			(1<<4)		/* Send BREAK */
#define	MX1_UCR1_TDMAEN			(1<<3)		/* Transmitter Ready DMA Enable */
#define	MX1_UCR1_UARTCLKEN		(1<<2)		/* UART Clock Enable */
#define	MX1_UCR1_DOZE			(1<<1)		/* UART DOZE State Control */
#define	MX1_UCR1_UARTEN			(1<<0)		/* UART Enable */

/* 
 * Control Register 2 bits
 */
#define	MX1_UCR2_ESCI			(1<<15)		/* Escape Sequence Interrupt Enable */
#define	MX1_UCR2_IRTS			(1<<14)		/* Ignore UART RTS pin */
#define	MX1_UCR2_CTSC			(1<<13)		/* UART CTS pin Control */
#define	MX1_UCR2_CTS			(1<<12)		/* Clear To Send */
#define	MX1_UCR2_ESCEN			(1<<11)		/* Escape Enable */
#define	MX1_UCR2_RTEC_MASK		(3<<9)		/* Request to Send Edge Control Mask */
#define	MX1_UCR2_PREN			(1<<8)		/* Parity Enable */
#define	MX1_UCR2_PROE			(1<<7)		/* Parity Odd/Even */
#define	MX1_UCR2_STPB			(1<<6)		/* Stop Bit */
#define	MX1_UCR2_WS				(1<<5)		/* Word Size */
#define	MX1_UCR2_RTSEN			(1<<4)		/* Request to Send Interrupt Enable */
#define	MX1_UCR2_TXEN			(1<<2)		/* Transmitter Enable */
#define	MX1_UCR2_RXEN			(1<<1)		/* Receiver Enable */
#define	MX1_UCR2_SRST			(1<<0)		/* Software Reset */

/* 
 * Control Register 3 bits
 */
#define	MX1_UCR3_DPEC_MASK		(3<<14)		/* DTR Interrupt Edge Control */
#define	MX1_UCR3_DTREN			(1<<13)		/* Data Terminal Ready Interrupt Enable */
#define	MX1_UCR3_PARERREN		(1<<12)		/* Parity Error Interrupt Enable */
#define	MX1_UCR3_FRAERREN		(1<<11)		/* Frame Error Interrupt Enable */
#define	MX1_UCR3_DSR			(1<<10)		/* Data Set Ready */
#define	MX1_UCR3_DCD			(1<<9)		/* Data Carrier Detect */
#define	MX1_UCR3_RI				(1<<8)		/* Ring Indicator */
#define	MX1_UCR3_RXDSEN			(1<<6)		/* Receive Status Interrupt Enable */
#define	MX1_UCR3_AIRINTEN		(1<<5)		/* Asynchronous IR WAKE Interrupt Enable */
#define	MX1_UCR3_AWAKEN			(1<<4)		/* Asynchronous WAKE Interrupt Enable */
#define	MX1_UCR3_REF25			(1<<3)		/* Reference Frequency 25 MHz */
#define	MX1_UCR3_REF20			(1<<2)		/* Reference Frequency 30 MHz */
#define	MX1_UCR3_INVT			(1<<1)		/* Inverted Infrared Transmission */
#define	MX1_UCR3_BPEN			(1<<0)		/* Preset Registers Enable */

/* 
 * Control Register 4 bits
 */
#define	MX1_UCR4_CTSTL_MASK		(0x3F<<10)	/* CTS Trigger Level (0-32)*/
#define	MX1_UCR4_INVR			(1<<9)		/* Inverted Infrared Reception */
#define	MX1_UCR4_ENIRI			(1<<8)		/* Serial Infrared Interrupt Enable */
#define	MX1_UCR4_WKEN			(1<<7)		/* WAKE Interrupt Enable */
#define	MX1_UCR4_REF16			(1<<6)		/* Reference Frequency 16 MHz */
#define	MX1_UCR4_IRSC			(1<<5)		/* IR Special Case */
#define	MX1_UCR4_TCEN			(1<<3)		/* Transmit Complete Interrupt Enable */
#define	MX1_UCR4_BKEN			(1<<2)		/* BREAK Condition Detected Interrupt Enable */
#define	MX1_UCR4_OREN			(1<<1)		/* Receive Overrun Interrupt Enable */
#define	MX1_UCR4_DREN			(1<<0)		/* Receive Data Ready Interrupt Enable */

/* 
 * Status Register 1 bits
 */
#define	MX1_USR1_PARITYERR		(1<<15)		/* Parity Error Interrupt Flag */
#define	MX1_USR1_RTSS			(1<<14)		/* RTS Pin Status */
#define	MX1_USR1_TRDY			(1<<13)		/* Transmitter Ready Interrupt/DMA Flag */
#define	MX1_USR1_RTSD			(1<<12)		/* RTS Delta */
#define	MX1_USR1_ESCF			(1<<11)		/* Escape Sequence Interrupt Flag */
#define	MX1_USR1_FRAMERR		(1<<10)		/* Frame Error Interrupt Flag */
#define	MX1_USR1_RRDY			(1<<9)		/* Receiver Ready Interrupt/DMA Flag */
#define	MX1_USR1_RXDS			(1<<6)		/* Receiver IDLE Interrupt Flag */
#define	MX1_USR1_AIRINT			(1<<5)		/* Asynchronous IR WAKE Interrupt Flag */
#define	MX1_USR1_AWAKE			(1<<4)		/* Asynchronous WAKE Interrupt Flag */

/* 
 * Status Register 2 bits
 */
#define	MX1_USR2_ADET			(1<<15)		/* Automatic Baud Rate Detect Complete */
#define	MX1_USR2_TXFE			(1<<14)		/* Transmit Buffer FIFO Empty */
#define	MX1_USR2_DTRF			(1<<13)		/* DTR Edge Triggered Interrupt Flag */
#define	MX1_USR2_IDLE			(1<<12)		/* IDLE Condition */
#define	MX1_USR2_IRINT			(1<<8)		/* Serial Infrared Interrupt Flag */
#define	MX1_USR2_WAKE			(1<<7)		/* WAKE */
#define	MX1_USR2_DCDDELT		(1<<6)		/* Data Carrier Detect delta */
#define	MX1_USR2_DCDIN			(1<<5)		/* Data Carrier Detect input */
#define	MX1_USR2_RTSF			(1<<4)		/* RTS Edge Triggered Interrupt Flag */
#define	MX1_USR2_TXDC			(1<<3)		/* Transmitter Complete */
#define	MX1_USR2_BRCD			(1<<2)		/* BREAK Condition Detected */
#define	MX1_USR2_ORE			(1<<1)		/* Overrun Error */
#define	MX1_USR2_RDR			(1<<0)		/* Receive Data Ready */


/* 
 * PLL Controller
 */

/* Register base address */
#define	MX1_PLL_BASE			0x0021B000	/* Base Physical address */
#define	MX1_PLL_SIZE			0x00000810

/* PCC control registers, offset from base address */
#define	MX1_PLL_CSCR			0x00		/* Clock Source Control Register */
#define	MX1_PLL_MPCTL0			0x04		/* MCU PLL Control Register 0 */
#define	MX1_PLL_MPCTL1			0x08		/* MCU PLL Control Register 1 */
#define	MX1_PLL_SPCTL0			0x0C		/* System PLL Control Register 0 */
#define	MX1_PLL_SPCTL1			0x10		/* System PLL Control Register 1 */
#define	MX1_PLL_PCDR			0x20		/* Peripherial Clock Divider Register */
#define	MX1_PLL_RSR				0x800		/* Reset Source Register */
#define	MX1_PLL_SIDR			0x804		/* Silicon ID Register */
#define	MX1_PLL_FMCR			0x808		/* Function Muxing Control Register */
#define	MX1_PLL_GPCR			0x80C		/* Global Peripherial Control Regiser */


/*
 * -------------------------------------------------------------------------
 * GPIO controller
 * -------------------------------------------------------------------------
 */
/* Register base address */
#define	MX1_GPIOA_BASE			0x0021C000	/* GPIO A base physical address */
#define	MX1_GPIOB_BASE			0x0021C100	/* GPIO B base physical address */
#define	MX1_GPIOC_BASE			0x0021C200	/* GPIO C base physical address */
#define	MX1_GPIOD_BASE			0x0021C300	/* GPIO D base physical address */
#define	MX1_GPIO_SIZE			0x00000044

/*
 * Register offsets from port base address
 */
#define	MX1_GPIO_DDIR			0x00		/* Data Direction Register */
#define	MX1_GPIO_OCR1			0x04		/* Output Configuration Register 1 */
#define	MX1_GPIO_OCR2			0x08		/* Output Configuration Register 2 */
#define	MX1_GPIO_ICRA1			0x0C		/* Input Configuration Register A1 */
#define	MX1_GPIO_ICRA2			0x10		/* Input Configuration Register A2 */
#define	MX1_GPIO_ICRB1			0x14		/* Input Configuration Register B1 */
#define	MX1_GPIO_ICRB2			0x18		/* Input Configuration Register B2 */
#define	MX1_GPIO_DR				0x1C		/* Data Register */
#define	MX1_GPIO_GIUS			0x20		/* GPIO In Use Register */
#define	MX1_GPIO_SSR			0x24		/* Sample Status Register */
#define	MX1_GPIO_INTCR1			0x28		/* Interrupt Configuration Register 1 */
#define	MX1_GPIO_INTCR2			0x2C		/* Interrupt Configuration Register 2 */
#define	MX1_GPIO_INTMR			0x30		/* Interrupt Mask Register */
#define	MX1_GPIO_INTSR			0x34		/* Interrupt Status Register */
#define	MX1_GPIO_GPR			0x38		/* General Purpose Register */
#define	MX1_GPIO_SWR			0x3C		/* Software Reset Register */
#define	MX1_GPIO_PUEN			0x40		/* Pull-up Enable Register */

/*
 * -------------------------------------------------------------------------
 * External Interface Module (EIM)
 * -------------------------------------------------------------------------
 */
#define	MX1_EIM_BASE			0x00220000	/* base physical address */
#define	MX1_EIM_SIZE			0x00000034

/*
 * Register offsets from MX1_EIM_BASE
 */
#define	MX1_EIM_CS0U			0x00		/* Chip Select 0 Upper Control Register */
#define	MX1_EIM_CS0L			0x04		/* Chip Select 0 Lower Control Register */
#define	MX1_EIM_CS1U			0x08		/* Chip Select 1 Upper Control Register */
#define	MX1_EIM_CS1L			0x0C		/* Chip Select 1 Lower Control Register */
#define	MX1_EIM_CS2U			0x10		/* Chip Select 2 Upper Control Register */
#define	MX1_EIM_CS2L			0x14		/* Chip Select 2 Lower Control Register */
#define	MX1_EIM_CS3U			0x18		/* Chip Select 3 Upper Control Register */
#define	MX1_EIM_CS3L			0x1C		/* Chip Select 3 Lower Control Register */
#define	MX1_EIM_CS4U			0x20		/* Chip Select 4 Upper Control Register */
#define	MX1_EIM_CS4L			0x24		/* Chip Select 4 Lower Control Register */
#define	MX1_EIM_CS5U			0x28		/* Chip Select 5 Upper Control Register */
#define	MX1_EIM_CS5L			0x2C		/* Chip Select 5 Lower Control Register */
#define	MX1_EIM_EIM				0x30		/* EIM Configuration Register */

/*
 * -------------------------------------------------------------------------
 * SDRAM Controller (SDRAMC)
 * -------------------------------------------------------------------------
 */
#define	MX1_SDRAMC_BASE			0x00221000	/* base physical address */
#define	MX1_SDRAMC_SIZE			0x00000020

/*
 * Register offsets from MX1_SDRAMC_BASE
 */
#define	MX1_SDRAMC_CTL0			0x00		/* SDRAM0 Controll Register */
#define	MX1_SDRAMC_CTL1			0x04		/* SDRAM1 Controll Register */
#define	MX1_SDRAMC_MISC			0x14		/* Miscellaneous Register */
#define	MX1_SDRAMC_RESET		0x18		/* SDRAM Reset Register */

/*
 * Bits defination for SDCTL register
 */
#define	MX1_SDCTL_SDE			(1 << 31)	/* SDRAM Controller Enable */
#define	MX1_SDCTL_SMODE_MASK	(7 << 28)	/* SDRAM Controller Operating Mode */
 #define	MX1_SDMODE_NORMAL	(0 << 28)	/* Mode : Normal Read/Write */
 #define	MX1_SDMODE_PREC		(1 << 28)	/* Mode : Precharge Command */
 #define	MX1_SDMODE_ARF		(2 << 28)	/* Mode : Auto-Refresh Command */
 #define	MX1_SDMODE_SMR		(3 << 28)	/* Mode : Set Mode Register Command */
 #define	MX1_SDMODE_LCR		(6 << 28)	/* Mode : SyncFlash Load Command Register */
 #define	MX1_SDMODE_PRG		(7 << 28)	/* Mode : SyncFlash Program Read/Write */
#define	MX1_SDCTL_SP			(1 << 27)	/* Supervisor Protect */
#define	MX1_SDCTL_ROW(x)		((x - 11) << 24)	/* Row Address Width (11 - 13) */
#define	MX1_SDCTL_COL(x)		((x - 8) << 20)	/* Column Address Width (8 - 11) */
#define	MX1_SDCTL_IAM			(1 << 19)	/* Interleaved Address Mode */
#define	MX1_SDCTL_DSIZ			(3 << 16)	/* SDRAM Memory Data Width */
#define	MX1_SDCTL_SREFR			(3 << 14)	/* SDRAM Refresh Rate */
#define	MX1_SDCTL_CLKST			(3 << 12)	/* Clock Suspend Time-Out */
#define	MX1_SDCTL_SCL			(3 << 8)	/* SDRAM CAS Latency */
#define	MX1_SDCTL_SRP			(1 << 6)	/* SDRAM Row Precharge Delay */
#define	MX1_SDCTL_SRCD			(3 << 4)	/* SDRAM Row-to-Column Delay */
#define	MX1_SDCTL_SRC			(7 << 0)	/* SDRAM Row Cycle Delay */


/*
 * -------------------------------------------------------------------------
 * Interrupt controller
 * -------------------------------------------------------------------------
 */

#define	MX1_AITC_BASE			0x00223000	/* base physical address */
#define	MX1_AITC_SIZE			0x00000068

/*
 * Register offsets from MX1_AITC_BASE
 */
#define MX1_AITC_INTCNTL   		0x00		/* Interrupt Control Register */
#define	MX1_AITC_NIMASK			0x04		/* Normal Interrupt Mask Register */
#define	MX1_AITC_INTENNUM		0x08		/* Interrupt Enable Number Register */
#define	MX1_AITC_INTDISNUM		0x0C		/* Interrupt Disable Number Register */
#define	MX1_AITC_INTENABLEH		0x10		/* Interrupt Enable Register High */
#define	MX1_AITC_INTENABLEL		0x14		/* Interrupt Enable Register Low */
#define	MX1_AITC_INTTYPEH		0x18		/* Interrupt Type Register High */
#define	MX1_AITC_INTTYPEL		0x1C		/* Interrupt Type Register Low */
#define	MX1_AITC_NIPRIORITY7	0x20		/* Normal Interrupt Priority Level Register 7 */
#define	MX1_AITC_NIPRIORITY6	0x24		/* Normal Interrupt Priority Level Register 6 */
#define	MX1_AITC_NIPRIORITY5	0x28		/* Normal Interrupt Priority Level Register 5 */
#define	MX1_AITC_NIPRIORITY4	0x2C		/* Normal Interrupt Priority Level Register 4 */
#define	MX1_AITC_NIPRIORITY3	0x30		/* Normal Interrupt Priority Level Register 3 */
#define	MX1_AITC_NIPRIORITY2	0x34		/* Normal Interrupt Priority Level Register 2 */
#define	MX1_AITC_NIPRIORITY1	0x38		/* Normal Interrupt Priority Level Register 1 */
#define	MX1_AITC_NIPRIORITY0	0x3c		/* Normal Interrupt Priority Level Register 0 */
#define	MX1_AITC_NIVECSR		0x40		/* Normal Interrupt Vector and Status Register */
#define	MX1_AITC_FIVECSR		0x44		/* Fast Interrupt Vector and Status Register */
#define	MX1_AITC_INTSRCH		0x48		/* Interrupt Source Register High */
#define	MX1_AITC_INTSRCL		0x4C		/* Interrupt Source Register Low */
#define	MX1_AITC_INTFRCH		0x50		/* Interrupt Force Register High */
#define	MX1_AITC_INTFRCL		0x54		/* Interrupt Force Register Low */
#define	MX1_AITC_NIPNDH			0x58		/* Normal Interrupt Pending Register High */
#define	MX1_AITC_NIPNDL			0x5C		/* Normal Interrupt Pending Register Low */
#define	MX1_AITC_FIPNDH			0x60		/* Fast Interrupt Pending Register High */
#define	MX1_AITC_FIPNDL			0x64		/* Fast Interrupt Pending Register Low */


#endif	/* __ARM_MX1_H_INCLUDED */

/* __SRCVERSION("mx1.h $Rev: 657836 $"); */
