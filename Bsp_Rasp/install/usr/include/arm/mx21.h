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
 * i.MX21 processor
 *
 */

#ifndef	__ARM_MX21_H_INCLUDED
#define	__ARM_MX21_H_INCLUDED

/*
 * AIPI
 */
#define	MX21_AIPI1_BASE			0x10000000
#define	MX21_AIPI2_BASE			0x10020000

#define	MX21_AIPI_PSR0			0x00
#define	MX21_AIPI_PSR1			0x04
#define	MX21_AIPI_PAR			0x08

/*
 * -------------------------------------------------------------------------
 * Direct Memory Access Controller (DMAC)
 * -------------------------------------------------------------------------
 */
#define	MX21_DMA_BASE			0x10001000	/* DMA Base Address */

/*
 * -------------------------------------------------------------------------
 * Watch Dog Timer registers
 * -------------------------------------------------------------------------
 */
#define	MX21_WDOG_BASE			0x10002000	/* WDT Base Address */

/*
 * -------------------------------------------------------------------------
 * General-Purpose Timer registers
 * -------------------------------------------------------------------------
 */
#define	MX21_GPT1_BASE			0x10003000	/* General Purpose Timer 1 Base */
#define	MX21_GPT2_BASE			0x10004000	/* General Purpose Timer 2 Base */
#define	MX21_GPT3_BASE			0x10005000	/* General Purpose Timer 3 Base */

/*
 * -------------------------------------------------------------------------
 * Pulse-Width Modulator (PWM) Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_PWM_BASE			0x10006000

/*
 * -------------------------------------------------------------------------
 * Real Time Clock (RTC) Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_RTC_BASE			0x10007000

/*
 * -------------------------------------------------------------------------
 * KeyPad Port (KPP) Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_KPP_BASE			0x10008000

/*
 * -------------------------------------------------------------------------
 * 1-Wire Interface (OWIRE) Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_OWIRE_BASE			0x10009000

/*
 * -------------------------------------------------------------------------
 * UARTs
 * -------------------------------------------------------------------------
 */
#define	MX21_UART1_BASE			0x1000A000	/* UART1 base address */
#define	MX21_UART2_BASE			0x1000B000	/* UART2 base address */
#define	MX21_UART3_BASE			0x1000C000	/* UART3 base address */
#define	MX21_UART4_BASE			0x1000D000	/* UART4 base address */

/*
 * -------------------------------------------------------------------------
 * Configurable Serial Peripheral Interface (CSPI) Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_CSPI1_BASE			0x1000E000	/* CSPI1 base address */
#define	MX21_CSPI2_BASE			0x1000F000	/* CSPI2 base address */
#define	MX21_CSPI3_BASE			0x10017000	/* CSPI3 base address */

/*
 * -------------------------------------------------------------------------
 * Synchronous Serial Interface (SSI) Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_SSI1_BASE			0x10010000	/* SSI1 base address */
#define	MX21_SSI2_BASE			0x10011000	/* SSI2 base address */

/*
 * -------------------------------------------------------------------------
 * I2C  Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_I2C_BASE			0x10012000	/* I2C base address */

#define	MX21_I2C_ADR			0x00		/* I2C Address Register */
#define	MX21_I2C_FDR			0x04		/* I2C Frequency Divider Register */
#define	MX21_I2C_CR				0x08		/* I2C Control Register */
#define	MX21_I2C_SR				0x0C		/* I2C Status Register */
#define	MX21_I2C_DR				0x10		/* I2C Data I/O Registyer */

/* I2C Control Register bit defination */
#define	MX21_I2CR_RSTA			(1 << 2)	/* Repeat Start */
#define	MX21_I2CR_TXAK			(1 << 3)	/* Transmit Acknowledge Enable */
#define	MX21_I2CR_MTX			(1 << 4)	/* Transmit/Receive Mode Select */
#define	MX21_I2CR_MSTA			(1 << 5)	/* Master/Slave Mode Select */
#define	MX21_I2CR_IEN			(1 << 6)	/* Interrupt Enable */
#define	MX21_I2CR_EN			(1 << 7)	/* I2C Enable */

/* I2C Status Register bit defination */
#define	MX21_I2SR_RXAK			(1 << 0)	/* Received Acknowledge */
#define	MX21_I2SR_IIF			(1 << 1)	/* I2C Interrupt */
#define	MX21_I2SR_SRW			(1 << 2)	/* Slave Read/Write */
#define	MX21_I2SR_IAL			(1 << 4)	/* Arbitration Lost */
#define	MX21_I2SR_IBB			(1 << 5)	/* I2C Bus Busy */
#define	MX21_I2SR_IAAS			(1 << 6)	/* I2C Address as Slave */
#define	MX21_I2SR_ICF			(1 << 7)	/* Data Transferring */

/*
 * -------------------------------------------------------------------------
 * SD Host Controller Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_SDHC1_BASE			0x10013000	/* SDHC1 base address */
#define	MX21_SDHC2_BASE			0x10014000	/* SDHC2 base address */

/*
 * -------------------------------------------------------------------------
 * General-Purpose IO (GPIO) registers
 * -------------------------------------------------------------------------
 */
#define	MX21_GPIO_BASE			0x10015000	/* GPIO base address */
#define	MX21_PORTA_BASE			0x10015000	/* Port A base address */
#define	MX21_PORTB_BASE			0x10015100	/* Port B base address */
#define	MX21_PORTC_BASE			0x10015200	/* Port C base address */
#define	MX21_PORTD_BASE			0x10015300	/* Port D base address */
#define	MX21_PORTE_BASE			0x10015400	/* Port E base address */
#define	MX21_PORTF_BASE			0x10015500	/* Port F base address */
#define	MX21_PORT_MASK			0x10015600	/* Port Interrupt Mask */


/*
 * -------------------------------------------------------------------------
 * Digital Audio Mux (AUDMUX) Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_AUDMUX_BASE		0x10016000	/* AUDMUX base address */

/*
 * -------------------------------------------------------------------------
 * LCD Controller Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_LCDC_BASE			0x10021000	/* LCDC base address */

/*
 * -------------------------------------------------------------------------
 * Smart LCD Controller Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_SLCDC_BASE			0x10022000	/* SLCDC base address */

/*
 * -------------------------------------------------------------------------
 * USB On-TheGo Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_USBOTG_BASE		0x10024000	/* USBOTG base address */

/*
 * -------------------------------------------------------------------------
 * enhanced Multimedia Accelerator (eMMA) Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_eMMA_BASE			0x10026000	/* eMMA base address */

/*
 * -------------------------------------------------------------------------
 * Clock Reset Module (CRM) Registers
 * -------------------------------------------------------------------------
 */
#define	MX21_CRM_BASE			0x10027000	/* CRM base address */

#define	MX21_CRM_CSCR			0x00		/* Clock Source Control Register */
#define	MX21_CRM_MPCTL0			0x04		/* MPLL Control Register 0 */
#define	MX21_CRM_MPCTL1			0x08		/* MPLL Control Register 1 */
#define	MX21_CRM_SPCTL0			0x0C		/* SPLL Control Register 0 */
#define	MX21_CRM_SPCTL1			0x10		/* SPLL Control Register 1 */
#define	MX21_CRM_OSC26MCTL		0x14		/* Oscillator Register */
#define	MX21_CRM_PCDR0			0x18		/* Perpherial Clock Divider Register 0 */
#define	MX21_CRM_PCDR1			0x1C		/* Perpherial Clock Divider Register 1 */
#define	MX21_CRM_PCCR0			0x20		/* Perpherial Clock Control Register 0 */
#define	MX21_CRM_PCCR1			0x24		/* Perpherial Clock Control Register 1 */


/*
 * -------------------------------------------------------------------------
 * System Control Module
 * -------------------------------------------------------------------------
 */
#define	MX21_SYSCTL_BASE		0x10027800	/* System control base address */


/*
 * -------------------------------------------------------------------------
 * Fast InfraRed Interface (FIRI) Module
 * -------------------------------------------------------------------------
 */
#define	MX21_FIRI_BASE			0x10028000	/* FIRI base address */

/*
 * -------------------------------------------------------------------------
 * Random Number Generator Accelerator (RNGA)
 * -------------------------------------------------------------------------
 */
#define	MX21_RNGA_BASE			0x10029000	/* RNGA base address */

/*
 * -------------------------------------------------------------------------
 * Run-Time Integrity Checker (RTIC)
 * -------------------------------------------------------------------------
 */
#define	MX21_RTIC_BASE			0x1002A000	/* RTIC base address */

/*
 * -------------------------------------------------------------------------
 * MAX
 * -------------------------------------------------------------------------
 */
#define	MX21_MAX_BASE			0x1003F000	/* MAX base address */

/*
 * -------------------------------------------------------------------------
 * Interrupt controller
 * -------------------------------------------------------------------------
 */
#define	MX21_AITC_BASE			0x10040000	/* AITC base address */

/*
 * -------------------------------------------------------------------------
 * SDRAM Controller
 * -------------------------------------------------------------------------
 */
#define	MX21_SDRAMC_BASE		0xDF000000	/* SDRAM Controller base address */

/*
 * -------------------------------------------------------------------------
 * External Interface Module (EIM)
 * -------------------------------------------------------------------------
 */
#define	MX21_EIM_BASE			0xDF001000	/* EIM base address */

#define	MX21_EIM_CS0U			0x00		/* Chip Select 0 Upper Control Register */
#define	MX21_EIM_CS0L			0x04		/* Chip Select 0 Lower Control Register */
#define	MX21_EIM_CS1U			0x08		/* Chip Select 1 Upper Control Register */
#define	MX21_EIM_CS1L			0x0C		/* Chip Select 1 Lower Control Register */
#define	MX21_EIM_CS2U			0x10		/* Chip Select 2 Upper Control Register */
#define	MX21_EIM_CS2L			0x14		/* Chip Select 2 Lower Control Register */
#define	MX21_EIM_CS3U			0x18		/* Chip Select 3 Upper Control Register */
#define	MX21_EIM_CS3L			0x1C		/* Chip Select 3 Lower Control Register */
#define	MX21_EIM_CS4U			0x20		/* Chip Select 4 Upper Control Register */
#define	MX21_EIM_CS4L			0x24		/* Chip Select 4 Lower Control Register */
#define	MX21_EIM_CS5U			0x28		/* Chip Select 5 Upper Control Register */
#define	MX21_EIM_CS5L			0x2C		/* Chip Select 5 Lower Control Register */

/*
 * -------------------------------------------------------------------------
 * PCMCIA/CF Interface
 * -------------------------------------------------------------------------
 */
#define	MX21_PCMCIA_BASE		0xDF002000	/* PCMCIA/CF base address */

/*
 * -------------------------------------------------------------------------
 * NAND Flash Controller
 * -------------------------------------------------------------------------
 */
#define	MX21_NANDFC_BASE		0xDF003000	/* NANDFC base address */

#endif	/* __ARM_MX21_H_INCLUDED */

/* __SRCVERSION("mx21.h $Rev: 657836 $"); */
