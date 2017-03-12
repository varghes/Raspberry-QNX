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
 * SA-1100 processor
 *

 *
 */

#ifndef	__ARM_SA1100_H_INCLUDED
#define	__ARM_SA1100_H_INCLUDED

/*
 * -------------------------------------------------------------------------
 * GPIO controller
 * -------------------------------------------------------------------------
 */
#define	SA1100_GPIO_BASE	0x90040000		/* base physical address */
#define	SA1100_GPIO_SIZE	0x00000020

/*
 * Register offsets from SA1100_GPIO_BASE
 */
#define	SA1100_GPLR			0x00			/* pin level register */
#define	SA1100_GPDR			0x04			/* pin direction register */
#define	SA1100_GPSR			0x08			/* pin output set register */
#define	SA1100_GPCR			0x0c			/* pin output clear register */
#define	SA1100_GRER			0x10			/* rising edge detect register */
#define	SA1100_GFER			0x14			/* falling edge detect register */
#define	SA1100_GEDR			0x18			/* edge detect status register */
#define	SA1100_GAFR			0x1c			/* alternate function register */

/*
 * GPIO pins
 */
#define	SA1100_GPIO_0		(1 <<  0)		/* GPIO Pin 0 */
#define	SA1100_GPIO_1		(1 <<  1)		/* GPIO Pin 1 */
#define	SA1100_GPIO_2		(1 <<  2)		/* GPIO Pin 2 */
#define	SA1100_GPIO_3		(1 <<  3)		/* GPIO Pin 3 */
#define	SA1100_GPIO_4		(1 <<  4)		/* GPIO Pin 4 */
#define	SA1100_GPIO_5		(1 <<  5)		/* GPIO Pin 5 */
#define	SA1100_GPIO_6		(1 <<  6)		/* GPIO Pin 6 */
#define	SA1100_GPIO_7		(1 <<  7)		/* GPIO Pin 7 */
#define	SA1100_GPIO_8		(1 <<  8)		/* GPIO Pin 8 */
#define	SA1100_GPIO_9		(1 <<  9)		/* GPIO Pin 9 */
#define	SA1100_GPIO_10		(1 << 10)		/* GPIO Pin 10 */
#define	SA1100_GPIO_11		(1 << 11)		/* GPIO Pin 11 */
#define	SA1100_GPIO_12		(1 << 12)		/* GPIO Pin 12 */
#define	SA1100_GPIO_13		(1 << 13)		/* GPIO Pin 13 */
#define	SA1100_GPIO_14		(1 << 14)		/* GPIO Pin 14 */
#define	SA1100_GPIO_15		(1 << 15)		/* GPIO Pin 15 */
#define	SA1100_GPIO_16		(1 << 16)		/* GPIO Pin 16 */
#define	SA1100_GPIO_17		(1 << 17)		/* GPIO Pin 17 */
#define	SA1100_GPIO_18		(1 << 18)		/* GPIO Pin 18 */
#define	SA1100_GPIO_19		(1 << 19)		/* GPIO Pin 19 */
#define	SA1100_GPIO_20		(1 << 20)		/* GPIO Pin 20 */
#define	SA1100_GPIO_21		(1 << 21)		/* GPIO Pin 21 */
#define	SA1100_GPIO_22		(1 << 22)		/* GPIO Pin 22 */
#define	SA1100_GPIO_23		(1 << 23)		/* GPIO Pin 23 */
#define	SA1100_GPIO_24		(1 << 24)		/* GPIO Pin 24 */
#define	SA1100_GPIO_25		(1 << 25)		/* GPIO Pin 25 */
#define	SA1100_GPIO_26		(1 << 26)		/* GPIO Pin 26 */
#define	SA1100_GPIO_27		(1 << 27)		/* GPIO Pin 27 */

/*
 * -------------------------------------------------------------------------
 * Peripheral Pin controller
 * -------------------------------------------------------------------------
 */

#define	SA1100_PPC_BASE		0x90060000		/* base physical address */
#define	SA1100_PPC_SIZE		0x00000014

/*
 * Register offsets from SA1100_PPC_BASE
 */
#define	SA1100_PPDR			0x00			/* pin direction register */
#define	SA1100_PPSR			0x04			/* pin status register */
#define	SA1100_PPAR			0x08			/* pin assignment register */
#define	SA1100_PSDR			0x0c			/* sleep mode direction register */
#define	SA1100_PPFR			0x10			/* pin flag register */

/*
 * PPDR/PPSR/PSDR bits
 *
 * For PPDR:
 *	- setting a bit to 0 configures pin for input
 *	- setting a bit to 1 configures pin as output
 *
 * For PPSR:
 *	- reads return current state of the peripheral pin
 *	- writes set/clear pins configured as outputs
 *
 * For PSDR:
 *	- setting a bit to 0 configures pin as output during sleep (driven low)
 *	- setting a bit to 1 configures pin as input during sleep
 */
#define	SA1100_PPC_LDD0		(1 <<  0)		/* LCD data pin 0 */
#define	SA1100_PPC_LDD1		(1 <<  1)		/* LCD data pin 1 */
#define	SA1100_PPC_LDD2		(1 <<  2)		/* LCD data pin 2 */
#define	SA1100_PPC_LDD3		(1 <<  3)		/* LCD data pin 3 */
#define	SA1100_PPC_LDD4		(1 <<  4)		/* LCD data pin 4 */
#define	SA1100_PPC_LDD5		(1 <<  5)		/* LCD data pin 5 */
#define	SA1100_PPC_LDD6		(1 <<  6)		/* LCD data pin 6 */
#define	SA1100_PPC_LDD7		(1 <<  7)		/* LCD data pin 7 */
#define	SA1100_PPC_L_PCLK	(1 <<  8)		/* LCD pixel clock pin */
#define	SA1100_PPC_L_LCLK	(1 <<  9)		/* LCD line clock pin */
#define	SA1100_PPC_L_FCLK	(1 << 10)		/* LCD frame clock pin */
#define	SA1100_PPC_L_BIAS	(1 << 11)		/* LCD AC bias pin */
#define	SA1100_PPC_TXD1		(1 << 12)		/* Serial Port 1 Tx pin */
#define	SA1100_PPC_RXD1		(1 << 13)		/* Serial Port 1 Rx pin */
#define	SA1100_PPC_TXD2		(1 << 14)		/* Serial Port 2 Tx pin	*/
#define	SA1100_PPC_RXD2		(1 << 15)		/* Serial Port 2 Rx pin	*/
#define	SA1100_PPC_TXD3		(1 << 16)		/* Serial Port 3 Tx pin */
#define	SA1100_PPC_RXD3		(1 << 17)		/* Serial Port 3 Rx pin */
#define	SA1100_PPC_TXD4		(1 << 18)		/* Serial Port 4 Tx pin */
#define	SA1100_PPC_RXD4		(1 << 19)		/* Serial Port 4 Rx pin */
#define	SA1100_PPC_SCLK		(1 << 20)		/* Serial Port 4 serial clock pin */
#define	SA1100_PPC_SFRM		(1 << 21)		/* Serial Port 4 serial frame pin */

/*
 * PPAR register bits
 */
#define	SA1100_PPAR_UPR		(1 << 12)		/* Reassign Serial Port 1 UART pins */
#define	SA1100_PPAR_SPR		(1 << 18)		/* Reassign Serial Port 4 SSP pins */

/*
 * PPFR register bits
 */
#define	SA1100_PPFR_LCD		(1 <<  0)		/* LCD controller flag */
#define	SA1100_PPFR_SP1_TX	(1 << 12)		/* Serial Port 1 Tx flag */
#define	SA1100_PPFR_SP1_RX	(1 << 13)		/* Serial Port 1 Rx flag */
#define	SA1100_PPFR_SP2_TX	(1 << 14)		/* Serial Port 2 Tx flag */
#define	SA1100_PPFR_SP2_RX	(1 << 15)		/* Serial Port 2 Rx flag */
#define	SA1100_PPFR_SP3_TX	(1 << 16)		/* Serial Port 3 Tx flag */
#define	SA1100_PPFR_SP3_RX	(1 << 17)		/* Serial Port 3 Rx flag */
#define	SA1100_PPFR_SP4		(1 << 18)		/* Serial Port 4 flag */

/*
 * -------------------------------------------------------------------------
 * Interrupt controller
 * -------------------------------------------------------------------------
 */

#define	SA1100_INTR_BASE	0x90050000	/* base physical address */
#define	SA1100_INTR_SIZE	0x00000024

/*
 * Register offsets from SA1100_INTR_BASE
 */
#define	SA1100_ICIP			0x00		/* IRQ pending register */
#define	SA1100_ICMR			0x04		/* mask register */
#define	SA1100_ICLR			0x08		/* level register */
#define	SA1100_ICCR			0x0c		/* control register */
#define	SA1100_ICFP			0x10		/* FIQ pending register */
#define	SA1100_ICPR			0x20		/* pending registers */

/*
 * ICCR bits
 */
#define	SA1100_ICCR_DIM		0x000000001	/* disable idle mask */

/*
 * Interrupt vectors
 */
#define	SA1100_INTR_GPIO_0		0		/* GPIO edge detect, pin 0 */
#define	SA1100_INTR_GPIO_1		1		/* GPIO edge detect, pin 1 */
#define	SA1100_INTR_GPIO_2		2		/* GPIO edge detect, pin 2 */
#define	SA1100_INTR_GPIO_3		3		/* GPIO edge detect, pin 3 */
#define	SA1100_INTR_GPIO_4		4		/* GPIO edge detect, pin 4 */
#define	SA1100_INTR_GPIO_5		5		/* GPIO edge detect, pin 5 */
#define	SA1100_INTR_GPIO_6		6		/* GPIO edge detect, pin 6 */
#define	SA1100_INTR_GPIO_7		7		/* GPIO edge detect, pin 7 */
#define	SA1100_INTR_GPIO_8		8		/* GPIO edge detect, pin 8 */
#define	SA1100_INTR_GPIO_9		9		/* GPIO edge detect, pin 9 */
#define	SA1100_INTR_GPIO_10		10		/* GPIO edge detect, pin 10 */
#define	SA1100_INTR_GPIO_11_27	11		/* GPIO edge detect, pin 11-27 */
#define	SA1100_INTR_LCD			12		/* LCD service request */
#define	SA1100_INTR_UDC			13		/* UDC service request */
#define	SA1100_INTR_SDLC		14		/* SDLC service request */
#define	SA1100_INTR_UART_1		15		/* UART 1 service request */
#define	SA1100_INTR_UART_2		16		/* UART 2 service request */
#define	SA1100_INTR_HSSP		16		/* HSSP service request */
#define	SA1100_INTR_UART_3		17		/* UART 3 service request */
#define	SA1100_INTR_MCP			18		/* MCP service request */
#define	SA1100_INTR_SSP			19		/* SSP service request */
#define	SA1100_INTR_DMA_0		20		/* DMA channel 0 service request */
#define	SA1100_INTR_DMA_1		21		/* DMA channel 1 service request */
#define	SA1100_INTR_DMA_2		22		/* DMA channel 2 service request */
#define	SA1100_INTR_DMA_3		23		/* DMA channel 3 service request */
#define	SA1100_INTR_DMA_4		24		/* DMA channel 4 service request */
#define	SA1100_INTR_DMA_5		25		/* DMA channel 5 service request */
#define	SA1100_INTR_TIMER_0		26		/* Timer match register 0 */
#define	SA1100_INTR_TIMER_1		27		/* Timer match register 1 */
#define	SA1100_INTR_TIMER_2		28		/* Timer match register 2 */
#define	SA1100_INTR_TIMER_3		29		/* Timer match register 3 */
#define	SA1100_INTR_RTC_TICK	30		/* RTC timer tick */
#define	SA1100_INTR_RTC_ALARM	31		/* RTC alarm */


/*
 * -------------------------------------------------------------------------
 * OS Timer registers
 * -------------------------------------------------------------------------
 */

#define	SA1100_TIMER_BASE	0x90000000	/* physical base address */
#define	SA1100_TIMER_SIZE	0x00000020

/*
 * Register offsets from SA1100_TIMER_BASE
 */
#define	SA1100_OSMR0		0x00		/* match register 0 */
#define	SA1100_OSMR1		0x04		/* match register 1 */
#define	SA1100_OSMR2		0x08		/* match register 2 */
#define	SA1100_OSMR3		0x0c		/* match register 3 */
#define	SA1100_OSCR			0x10		/* count register */
#define	SA1100_OSSR			0x14		/* status register */
#define	SA1100_OWER			0x18		/* watchdog enable register */
#define	SA1100_OIER			0x1c		/* interrupt enable register */

/*
 * OWER bits
 */
#define	SA1100_OWER_WME		0x00000001	/* enable watchdog reset on match 3 */

/*
 * OSSR bits
 */
#define	SA1100_OSSR_M0		0x00000001	/* channel 0 has matched */
#define	SA1100_OSSR_M1		0x00000002	/* channel 1 has matched */
#define	SA1100_OSSR_M2		0x00000004	/* channel 1 has matched */
#define	SA1100_OSSR_M4		0x00000008	/* channel 1 has matched */

/*
 * OIER bits
 */
#define	SA1100_OIER_E0		0x00000001	/* enable interrupt for match 0 */
#define	SA1100_OIER_E1		0x00000002	/* enable interrupt for match 1 */
#define	SA1100_OIER_E2		0x00000004	/* enable interrupt for match 2 */
#define	SA1100_OIER_E4		0x00000008	/* enable interrupt for match 3 */

/*
 * -------------------------------------------------------------------------
 * Serial ports
 * -------------------------------------------------------------------------
 */

#define	SA1100_UART1_BASE	0x80010000		/* base physical address */
#define	SA1100_UART2_BASE	0x80030000		/* base physical address */
#define	SA1100_UART3_BASE	0x80050000		/* base physical address */
#define	SA1100_UART_SIZE	0x00000024

#define	SA1100_UDC_BASE		0x80000000		/* base physical address */
#define	SA1100_HSSP_BASE	0x80040060		/* base physical address */

/*
 * Byte offsets from UART base address.
 */
#define	SA1100_UTCR0		0x00			/* control 0 */
#define	SA1100_UTCR1		0x04			/* control 1 */
#define	SA1100_UTCR2		0x08			/* control 2 */
#define	SA1100_UTCR3		0x0c			/* control 3 */
#define	SA1100_UTDR			0x14			/* data */
#define	SA1100_UTSR0		0x1c			/* status 0 */
#define	SA1100_UTSR1		0x20			/* status 1 */

/*
 * UTCR0 bits
 */
#define	SA1100_UTCR0_PE		0x01			/* parity enable */
#define	SA1100_UTCR0_OES	0x02			/* odd/even parity select */
#define	SA1100_UTCR0_SBS	0x04			/* stop bit select */
#define	SA1100_UTCR0_DSS	0x08			/* data size select */
#define	SA1100_UTCR0_SCE	0x10			/* sample clock enable */
#define	SA1100_UTCR0_RCE	0x20			/* receive clock edge select */
#define	SA1100_UTCR0_TCE	0x40			/* transmit clock edge select */

/*
 * Aliases for baud rate divisor registers
 */
#define	SA1100_UTBRD_HI		SA1100_UTCR1	/* baud rate divisor bits 8-11 */
#define	SA1100_UTBRD_LO		SA1100_UTCR2	/* baud rate divisor bits 0-7 */

/*
 * UTCR3 bits
 */
#define	SA1100_UTCR3_RXE	0x01			/* receiver enable */
#define	SA1100_UTCR3_TXE	0x02			/* transmitter enable */
#define	SA1100_UTCR3_BRK	0x04			/* break */
#define	SA1100_UTCR3_RIE	0x08			/* receive FIFO interrupt enable */
#define	SA1100_UTCR3_TIE	0x10			/* transmit FIFO interrupt enable */
#define	SA1100_UTCR3_LBM	0x20			/* loopback mode */

/*
 * UTSR0 bits
 */
#define	SA1100_UTSR0_TFS	0x01			/* transmit FIFO service request */
#define	SA1100_UTSR0_RFS	0x02			/* receive FIFO service request */
#define	SA1100_UTSR0_RID	0x04			/* receiver idle */
#define	SA1100_UTSR0_RBB	0x08			/* receiver begin of break */
#define	SA1100_UTSR0_REB	0x10			/* receiver end of break */
#define	SA1100_UTSR0_EIF	0x20			/* error in FIFO */

/*
 * UTSR1 bits
 */
#define	SA1100_UTSR1_TBY	0x01			/* transmitter busy */
#define	SA1100_UTSR1_RNE	0x02			/* receive FIFO not empty */
#define	SA1100_UTSR1_TNF	0x04			/* transmit FIFO not full */
#define	SA1100_UTSR1_PRE	0x08			/* parity error */
#define	SA1100_UTSR1_FRE	0x10			/* framing error */
#define	SA1100_UTSR1_ROR	0x20			/* receive FIFO overrun */

/*
 * -------------------------------------------------------------------------
 * MCP port
 * -------------------------------------------------------------------------
 */
#define	SA1100_MCP_BASE		0x80060000		/* base physical address */
#define	SA1100_MCP_SIZE		0x1c

/*
 * Byte offsets from SA1100_MCP_BASE
 */
#define	SA1100_MCCR0		0x00			/* control register 0 */
#define	SA1100_MCDR0		0x08			/* data register 0 */
#define	SA1100_MCDR1		0x0c			/* data register 1 */
#define	SA1100_MCDR2		0x10			/* data register 2 */
#define	SA1100_MCSR			0x18			/* status register */

/*
 * MCCR0 bits
 */
#define	SA1100_MCCR0_ASD(x)	(x)				/* audio sample rate divisor */
#define	SA1100_MCCR0_TSD(x)	((x) << 8)		/* telecom sample rate divisor */
#define	SA1100_MCCR0_MCE	0x00010000		/* mcp enable */
#define	SA1100_MCCR0_ECS	0x00020000		/* external clock select */
#define	SA1100_MCCR0_ADM	0x00040000		/* A/D sampling mode */
#define	SA1100_MCCR0_TTE	0x00080000		/* telecom tx fifo interrupt enable */
#define	SA1100_MCCR0_TRE	0x00100000		/* telecom rx fifo interrupt enable */
#define	SA1100_MCCR0_ATE	0x00200000		/* audio tx fifo interrupt enable */
#define	SA1100_MCCR0_ARE	0x00400000		/* audio rx fifo interrupt enable */
#define	SA1100_MCCR0_LBM	0x00800000		/* loopback mode */
#define	SA1100_MCCR0_ECP(x)	((x) << 24)		/* external clock prescaler */

/*
 * MCDR2 bits
 */
#define	SA1100_MCDR2_WR		0x00010000		/* write to register */
#define	SA1100_MCDR2_REG(x)	((x) << 17)

/*
 * MCSR bits
 */
#define	SA1100_MCSR_ATS		0x00000001		/* audio tx fifo request */
#define	SA1100_MCSR_ARS		0x00000002		/* audio rx fifo request */
#define	SA1100_MCSR_TTS		0x00000004		/* telecom tx fifo request */
#define	SA1100_MCSR_TRS		0x00000008		/* telecom rx fifo request */
#define	SA1100_MCSR_ATU		0x00000010		/* audio tx fifo underrun */
#define	SA1100_MCSR_ARO		0x00000020		/* audio rx fifo overrun */
#define	SA1100_MCSR_TTU		0x00000040		/* telecom tx fifo underrun */
#define	SA1100_MCSR_TRO		0x00000080		/* telecom rx fifo overrun */
#define	SA1100_MCSR_ANF		0x00000100		/* audio tx fifo not full */
#define	SA1100_MCSR_ANE		0x00000200		/* audio rx fifo not empty */
#define	SA1100_MCSR_TNF		0x00000400		/* telecom tx fifo not full */
#define	SA1100_MCSR_TNE		0x00000800		/* telecom rx fifo not empty */
#define	SA1100_MCSR_CWC		0x00001000		/* codec write completed */
#define	SA1100_MCSR_CRC		0x00002000		/* codec read completed */
#define	SA1100_MCSR_ACE		0x00004000		/* audio codec enabled */
#define	SA1100_MCSR_TCE		0x00008000		/* telecom codec enabled */

/*
 * -------------------------------------------------------------------------
 * SSP port
 * -------------------------------------------------------------------------
 */
#define	SA1100_SSP_BASE		0x80070060		/* base physical address */
#define	SA1100_SSP_SIZE		0x18			/* size of register block */

/*
 * Register offsets from SA1100_SSP_BASE
 */
#define	SA1100_SSCR0		0x00			/* control register 0 */
#define	SA1100_SSCR1		0x04			/* control register 1 */
#define	SA1100_SSDR			0x0c			/* data register */
#define	SA1100_SSSR			0x14			/* status register */

/*
 * SSCR0 bits
 */
#define	SA1100_SSCR0_DSS_MASK		0x0f
#define	SA1100_SSCR0_DSS_4			0x03	/*  4 bit data */
#define	SA1100_SSCR0_DSS_5			0x04	/*  5 bit data */
#define	SA1100_SSCR0_DSS_6			0x05	/*  6 bit data */
#define	SA1100_SSCR0_DSS_7			0x06	/*  7 bit data */
#define	SA1100_SSCR0_DSS_8			0x07	/*  8 bit data */
#define	SA1100_SSCR0_DSS_9			0x08	/*  9 bit data */
#define	SA1100_SSCR0_DSS_10			0x09	/* 10 bit data */
#define	SA1100_SSCR0_DSS_11			0x0a	/* 11 bit data */
#define	SA1100_SSCR0_DSS_12			0x0b	/* 12 bit data */
#define	SA1100_SSCR0_DSS_13			0x0c	/* 13 bit data */
#define	SA1100_SSCR0_DSS_14			0x0d	/* 14 bit data */
#define	SA1100_SSCR0_DSS_15			0x0e	/* 15 bit data */
#define	SA1100_SSCR0_DSS_16			0x0f	/* 16 bit data */
#define	SA1100_SSCR0_FRF_MASK		0x30
#define	SA1100_SSCR0_FRF_MOTOROLA	0x00	/* Motorola frame format */
#define	SA1100_SSCR0_FRF_TEXAS		0x01	/* Texas Instruments format */
#define	SA1100_SSCR0_FRF_NATIONAL	0x02	/* National Microwire format */
#define	SA1100_SSCR0_SSE			0x80	/* SSP enable */
#define	SA1100_SSCR0_SCR(x)		((x) << 8)	/* Set serial clock rate */

/*
 * SSCR1 bits
 */
#define	SA1100_SSCR1_RIE			0x01	/* Rx fifo interrupt enable */
#define	SA1100_SSCR1_TIE			0x02	/* Tx fifo interrupt enable */
#define	SA1100_SSCR1_LBM			0x04	/* loopback mode */
#define	SA1100_SSCR1_SPO			0x08	/* serial clock polarity */
#define	SA1100_SSCR1_SP				0x10	/* serial clock phase */
#define	SA1100_SSCR1_ECS			0x20	/* external clock select */

/*
 * SSSR register bits
 */
#define	SA1100_SSSR_TNF				0x02	/* Tx fifo not full */
#define	SA1100_SSSR_RNE				0x04	/* Rx fifo not empty */
#define	SA1100_SSSR_BSY				0x08	/* SSP busy */
#define	SA1100_SSSR_TFS				0x10	/* Tx fifo service request */
#define	SA1100_SSSR_RFS				0x20	/* Rx fifo service request */
#define	SA1100_SSSR_ROR				0x40	/* Rx fifo overrun */

/*
 * -------------------------------------------------------------------------
 * SDLC port
 * -------------------------------------------------------------------------
 */
#define	SA1100_SDLC_BASE	0x80020060		/* base physical address */
#define	SA1100_SDLC_SIZE	0x28			/* size of register block */

/*
 * Register offsets from SDLC_BASE
 */
#define	SA1100_SDCR0		0x00			/* control register 0 */
#define	SA1100_SDCR1		0x04			/* control register 1 */
#define	SA1100_SDCR2		0x08			/* control register 2 */
#define	SA1100_SDCR3		0x0c			/* control register 3 */
#define	SA1100_SDCR4		0x10			/* control register 4 */
#define	SA1100_SDDR			0x18			/* data register */
#define	SA1100_SDSR0		0x20			/* status register 0 */
#define	SA1100_SDSR1		0x24			/* status register 1 */

/*
 * SDCR0 bits
 */
#define	SA1100_SDCR0_SUS	0x01			/* select UART mode */
#define	SA1100_SDCR0_SDF	0x02			/* double flag select */
#define	SA1100_SDCR0_LBM	0x04			/* loopback mode */
#define	SA1100_SDCR0_BMS	0x08			/* NRZ encoding */
#define	SA1100_SDCR0_SCE	0x10			/* sample clock enable */
#define	SA1100_SDCR0_SCD	0x20			/* sample clock direction */
#define	SA1100_SDCR0_RCE	0x40			/* receive clock edge select */
#define	SA1100_SDCR0_TCE	0x80			/* transmit clock edge select */

/*
 * SDCR1 bits
 */
#define	SA1100_SDCR1_AAF	0x01			/* abort after frame */
#define	SA1100_SDCR1_TXE	0x02			/* transmit enable */
#define	SA1100_SDCR1_RXE	0x04			/* receive enable */
#define	SA1100_SDCR1_RIE	0x08			/* rx fifo interrupt enable */
#define	SA1100_SDCR1_TIE	0x10			/* tx fifo interrupt enable */
#define	SA1100_SDCR1_AME	0x20			/* address match enable */
#define	SA1100_SDCR1_TUS	0x40			/* tx fifo underrun select */
#define	SA1100_SDCR1_RAE	0x80			/* rx abort interrupt enable */

/*
 * SDSR0 bits
 */
#define	SA1100_SDSR0_EIF	0x01			/* error in fifo */
#define	SA1100_SDSR0_TUR	0x02			/* tx fifo underrun */
#define	SA1100_SDSR0_RAB	0x04			/* receiver abort */
#define	SA1100_SDSR0_TFS	0x08			/* tx fifo service request */
#define	SA1100_SDSR0_RFS	0x10			/* rx fifo service request */

/*
 * SDSR1 bits
 */
#define	SA1100_SDSR1_RSY	0x01			/* receiver synchronised */
#define	SA1100_SDSR1_TBY	0x02			/* tranmitter busy */
#define	SA1100_SDSR1_RNE	0x04			/* rx fifo not empty */
#define	SA1100_SDSR1_TNF	0x08			/* tx fifo not full */
#define	SA1100_SDSR1_RTD	0x10			/* receive transition detected */
#define	SA1100_SDSR1_EOF	0x20			/* end of frame */
#define	SA1100_SDSR1_CRE	0x40			/* crc error */
#define	SA1100_SDSR1_ROR	0x80			/* rx fifo overrun */

/*
 * -------------------------------------------------------------------------
 * Reset Controller
 * -------------------------------------------------------------------------
 */

#define	SA1100_RESET_BASE		0x90030000		/* base physical address */
#define	SA1100_RESET_SIZE		0x0000000c		/* size of register block */

/*
 * Offsets from SA1100_RESET_BASE
 */
#define	SA1100_RSRR				0x00			/* software reset register */
#define	SA1100_RCSR				0x04			/* reset status register */
#define	SA1100_TUCR				0x08			/* test control register */

/*
 * RSRR bits
 */
#define	SA1100_RSRR_SWR			0x01			/* software reset */

/*
 * RCSR bits
 */
#define	SA1100_RCSR_HWR			0x01			/* hardware reset */
#define	SA1100_RCSR_SWR			0x02			/* software reset */
#define	SA1100_RCSR_WDR			0x04			/* watchdog reset */
#define	SA1100_RCRR_SMR			0x08			/* sleep mode reset */

/*
 * TUCR bits
 */
#define	SA1100_TUCR_PMD			0x00000200		/* power management disable */
#define	SA1100_TUCR_MR			0x00000400		/* memory request mode */
#define	SA1100_TUCR_TSEL_xxx	0x00000000
#define	SA1100_TUCR_TSEL_xx0	0x20000000
#define	SA1100_TUCR_TSEL_x1x	0x40000000
#define	SA1100_TUCR_TSEL_x10	0x80000000
#define	SA1100_TUCR_TSEL_2xx	0x80000000
#define	SA1100_TUCR_TSEL_2x0	0x80000000
#define	SA1100_TUCR_TSEL_21x	0x80000000
#define	SA1100_TUCR_TSEL_210	0x80000000

/*
 * -------------------------------------------------------------------------
 * Power Manager
 * -------------------------------------------------------------------------
 */
#define	SA1100_POWER_BASE		0x90020000		/* base physical address */
#define	SA1100_POWER_SIZE		0x00000020		/* size of register block */

/*
 * Offsets from SA1100_POWER_BASE
 */
#define	SA1100_PMCR				0x00			/* control */
#define	SA1100_PSSR				0x04			/* sleep status */
#define	SA1100_PSPR				0x08			/* scratch pad */
#define	SA1100_PWER				0x0c			/* wakeup enable */
#define	SA1100_PCFR				0x10			/* general configuration */
#define	SA1100_PPCR				0x14			/* PLL configuration */
#define	SA1100_PGSR				0x18			/* GPIO sleep state */
#define	SA1100_POSR				0x1c			/* oscillator status */

/*
 * PMCR bits
 */
#define	SA1100_PMCR_SF			0x00000001		/* force sleep mode */

/*
 * PSSR bits
 */
#define	SA1100_PSSR_SS			0x00000001		/* forced sleep via PMCR_SF */
#define	SA1100_PSSR_BFS			0x00000002		/* BATT_FAULT pin asserted */
#define	SA1100_PSSR_VFS			0x00000004		/* VDD_FAULT pin asserted */
#define	SA1100_PSSR_DH			0x00000008		/* DRAM control hold */
#define	SA1100_PSSR_PH			0x00000010		/* peripheral control hold */

/*
 * PWER bits
 */
#define	SA1100_PWER_WE31		0x80000000		/* enable RTC wakeup */

/*
 * PCFR bits
 */
#define	SA1100_PCFR_OPDE		0x00000001		/* stop 3.6864MHz oscillator */
#define	SA1100_PCFR_FP			0x00000002		/* float PCMCIA control signals */
#define	SA1100_PCFR_FS			0x00000004		/* float static chip selects */
#define	SA1100_PCFR_FO			0x00000008		/* force 32kHz oscillator on */

/*
 * PPCR bits (clock speed for 3.6864MHz oscillator)
 */
#define	SA1100_PPCR_59_0		0x00000000		/*  59.0MHz core clock */
#define	SA1100_PPCR_73_7		0x00000001		/*  73.7MHz core clock */
#define	SA1100_PPCR_88_5		0x00000002		/*  88.5MHz core clock */
#define	SA1100_PPCR_103_2		0x00000003		/* 103.2MHz core clock */
#define	SA1100_PPCR_118_0		0x00000004		/* 118.0MHz core clock */
#define	SA1100_PPCR_132_7		0x00000005		/* 132.7MHz core clock */
#define	SA1100_PPCR_147_5		0x00000006		/* 147.5MHz core clock */
#define	SA1100_PPCR_162_2		0x00000007		/* 162.2MHz core clock */
#define	SA1100_PPCR_176_9		0x00000008		/* 176.9MHz core clock */
#define	SA1100_PPCR_191_7		0x00000009		/* 191.7MHz core clock */
#define	SA1100_PPCR_206_4		0x0000000a		/* 206.4MHz core clock */
 
/*
 * POSR bits
 */
#define	SA1100_POSR_OOK			0x00000001		/* oscillator stabilised */

/*
 * -------------------------------------------------------------------------
 * LCD Controller
 * -------------------------------------------------------------------------
 */

#define	SA1100_LCD_BASE			0xb0100000		/* base physical address */
#define	SA1100_LCD_SIZE			0x0000002c		/* size of register block */

/*
 * Offsets from SA1100_LCD_BASE
 */
#define	SA1100_LCCR0			0x00			/* control register 0 */
#define	SA1100_LCSR				0x04			/* status register */
#define	SA1100_DBAR1			0x10			/* DMA channel 1 base address */
#define	SA1100_DCAR1			0x14			/* DMA channel 1 current address */
#define	SA1100_DBAR2			0x18			/* DMA channel 2 base address */
#define	SA1100_DCAR2			0x1c			/* DMA channel 2 current address */
#define	SA1100_LCCR1			0x20			/* control register 1 */
#define	SA1100_LCCR2			0x24			/* control register 2 */
#define	SA1100_LCCR3			0x28			/* control register 3 */

/*
 * LCCR0 bits
 */
#define	SA1100_LCCR0_LEN		0x00000001		/* LCD controller enable */
#define SA1100_LCCR0_CMS		0x00000002		/* colour/monochrome select */
#define	SA1100_LCCR0_SDS		0x00000004		/* single/dual-panel select */
#define	SA1100_LCCR0_LDM		0x00000008		/* LCD disable done mask */
#define	SA1100_LCCR0_BAM		0x00000010		/* base address update mask */
#define	SA1100_LCCR0_ERM		0x00000020		/* error mask */
#define	SA1100_LCCR0_PAS		0x00000080		/* passive/active display select */
#define	SA1100_LCCR0_BLE		0x00000100		/* big/little endian select */
#define	SA1100_LCCR0_DPD		0x00000200		/* double pixel data pin mode */
#define	SA1100_LCCR0_PDD(x)		((x) << 12)		/* PDD field */

/*
 * LCSR bits
 */
#define	SA1100_LCSR_LDD			0x00000001		/* LCD disable done */
#define	SA1100_LCSR_BAU			0x00000002		/* base address update */
#define	SA1100_LCSR_BER			0x00000004		/* bus error */
#define	SA1100_LCSR_ABC			0x00000008		/* AC bias count */
#define	SA1100_LCSR_IOL			0x00000010		/* input FIFO overrun  (lower panel) */
#define	SA1100_LCSR_IUL			0x00000020		/* input FIFO underrun (lower panel) */
#define	SA1100_LCSR_IOU			0x00000040		/* input FIFO overrun  (upper panel) */
#define	SA1100_LCSR_IUU			0x00000080		/* input FIFO underrun (upper panel) */
#define	SA1100_LCSR_OOL			0x00000100		/* output FIFO overrun  (lower panel) */
#define	SA1100_LCSR_OUL			0x00000200		/* output FIFO underrun (lower panel) */
#define	SA1100_LCSR_OOU			0x00000400		/* output FIFO overrun  (upper panel) */
#define	SA1100_LCSR_OUU			0x00000800		/* output FIFO underrun (upper panel) */

/*
 * LCCR1 bits
 */
#define	SA1100_LCCR1_PPL(x)		(x)				/* pixels per line */
#define	SA1100_LCCR1_HSW(x)		((x) << 10)		/* horizontal sync pulse width */
#define	SA1100_LCCR1_ELW(x)		((x) << 16)		/* end-of-line pixel clock wait */
#define	SA1100_LCCR1_BLW(x)		((x) << 24)		/* beginning-of-line pixel clock wait */

/*
 * LCCR2 bits
 */
#define	SA1100_LCCR2_LPP(x)		(x)				/* lines per panel */
#define	SA1100_LCCR2_VSW(x)		((x) << 10)		/* vertical sync pulse width */
#define	SA1100_LCCR2_EFW(x)		((x) << 16)		/* end-of-frame pixel clock wait */
#define	SA1100_LCCR2_BFW(x)		((x) << 24)		/* beginning-of-frame pixel clock wait */

/*
 * LCCR3 bits
 */
#define	SA1100_LCCR3_PCD(x)		(x)				/* pixel clock divisor */
#define	SA1100_LCCR3_ACB(x)		((x) << 8)		/* AC bias pin frequency */
#define	SA1100_LCCR3_API(x)		((x) << 16)		/* AC bias pin transitions per interrupt */
#define	SA1100_LCCR3_VSP		0x00100000		/* vertical sync polarity */
#define	SA1100_LCCR3_HSP		0x00200000		/* horizontal sync polarity */
#define	SA1100_LCCR3_PCP		0x00400000		/* pixel clock polarity */
#define	SA1100_LCCR3_OEP		0x00800000		/* output enable polarity */


/*
 * -------------------------------------------------------------------------
 * Memory Controller
 * -------------------------------------------------------------------------
 */

#define	SA1100_MEMCTL_BASE		0xa0000000		/* base physical address */
#define	SA1100_MEMCTL_SIZE		0x00000034		/* size of register block */

/* 
 * Register Offsets from MEMCTL_BASE
 */
#define SA1100_MDCNFG           0x01
#define SA1100_MDCAS00          0x04
#define SA1100_MDCAS01          0x08
#define SA1100_MDCAS02          0x0C
#define SA1100_MSC0             0x10
#define SA1100_MSC1             0x14
#define SA1100_MECR             0x18
#define SA1100_MDREFR           0x1C
#define SA1100_MDCAS20          0x20
#define SA1100_MDCAS21          0x24
#define SA1100_MSC2             0x2C
#define SA1100_SMCNFG           0x30

/*
 * MSC Bits (Static Memory, nCS[0..5])
 */
#define SA1100_MSC_RT(a)        (a & 0x3)
#define SA1100_MSC_RBW          (1<<2)
#define    SA1100_MSC_RBW16     (SA1100_MSC_RBW*1)
#define    SA1100_MSC_RBW32     (SA1100_MSC_RBW*0) 
#define SA1100_MSC_RDF(a)       ( ( a & 0x1f ) << 3 )
#define SA1100_MSC_RDN(a)       ( ( a & 0x1f ) << 8 )
#define SA1100_MSC_RRR(a)       ( ( a & 0x7 ) << 13 )

#define SA1100_MSC0_nCS0( a )    ( a )
#define SA1100_MSC0_nCS1( a )    ( a << 16 )
#define SA1100_MSC1_nCS2( a )    ( a )
#define SA1100_MSC1_nCS3( a )    ( a << 16 )
#define SA1100_MSC2_nCS4( a )    ( a )
#define SA1100_MSC2_nCS5( a )    ( a << 16 )


/*
 * -------------------------------------------------------------------------
 * RTC Controller
 * -------------------------------------------------------------------------
 */

#define	SA1100_RTC_BASE			0x90010000		/* base physical address */
#define	SA1100_RTC_SIZE			0x00000014		/* size of register block */

/*
 * Rgister offsets from RTC_BASE
 */
#define	SA1100_RTAR				0x00
#define	SA1100_RCNR				0x04
#define	SA1100_RTTR				0x08
#define	SA1100_RTSR				0x10

/*
 * RTSR bits
 */
#define	SA1100_RTSR_AL			0x00000001
#define	SA1100_RTSR_HZ			0x00000002
#define	SA1100_RTSR_ALE			0x00000004
#define	SA1100_RTSR_HZE			0x00000008

#endif	/* __ARM_SA1100_H_INCLUDED */

/* __SRCVERSION("sa1100.h $Rev: 657836 $"); */
