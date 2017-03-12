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
 * i.MX25 processor
 *
 */

#ifndef	__ARM_MX25_H_INCLUDED
#define	__ARM_MX25_H_INCLUDED


/*
 * -------------------------------------------------------------------------
 * AIPS registers
 * -------------------------------------------------------------------------
 */
#define	MX25_AIPI1_BASE			0x43F00000     // AIPS A control regs
#define	MX25_AIPI2_BASE			0x53F00000     // AIPS B control regs

#define	MX25_AIPS_MPR1			0x00
#define	MX25_AIPS_MPR2			0x04
#define	MX25_AIPI_PACR1			0x20
#define	MX25_AIPI_PACR2			0x24
#define	MX25_AIPI_PACR3			0x28
#define	MX25_AIPI_PACR4			0x2C
#define	MX25_AIPI_OPACR1		0x40
#define	MX25_AIPI_OPACR2		0x44
#define	MX25_AIPI_OPACR3		0x48
#define	MX25_AIPI_OPACR4		0x4C
#define	MX25_AIPI_OPACR5		0x50

/*
 * -------------------------------------------------------------------------
 * Direct Memory Access Controller (SDMA)
 * -------------------------------------------------------------------------
 */
#define	MX25_SDMA_BASE			0x53FD4000	/* DMA Base Address */

/*
 * -------------------------------------------------------------------------
 * Watch Dog Timer registers
 * -------------------------------------------------------------------------
 */
#define	MX25_WDOG_BASE			0x53FDC000	/* WDT Base Address */

/* Watchdog Timer Registers, offset from base address */
#define	MX35_WDOG_CR			0x00
#define	MX35_WDOG_SR			0x02
#define	MX35_WDOG_RSR			0x04
#define	MX35_WDOG_ICR			0x06
#define	MX35_WDOG_MCR			0x08

/*
 * -------------------------------------------------------------------------
 * General-Purpose Timer registers
 * -------------------------------------------------------------------------
 */
#define	MX25_GPT1_BASE			0x53F90000	/* General Purpose Timer 1 Base */
#define	MX25_GPT2_BASE			0x53F8C000	/* General Purpose Timer 2 Base */
#define	MX25_GPT3_BASE			0x53F88000	/* General Purpose Timer 3 Base */
#define	MX25_GPT4_BASE			0x53F84000	/* General Purpose Timer 3 Base */

#define	MX25_GPT_SIZE	        0x28		/* GPT register total size */
#define MX25_GPT1_IRQ	        54

/* GPT Registers, offset from base address */
#define	MX25_GPT_CTL			0x00		/* GPT Control Register */
#define	MX25_GPT_PRESCALE		0x04		/* GPT Prescaler Register */
#define MX25_GPT_STATUS			0x08		/* GPT Status Register */
#define MX25_GPT_INTR			0x0c		/* GPT Interrupt Register */
#define	MX25_GPT_CMP1			0x10		/* GPT Counter Compare Register 1 */
#define	MX25_GPT_CMP2			0x14		/* GPT Counter Compare Register 2 */
#define	MX25_GPT_CMP3			0x18		/* GPT Counter Compare Register 2 */
#define	MX25_GPT_CPT1			0x1c		/* GPT Capture Register 1 */
#define	MX25_GPT_CPT2			0x20		/* GPT Capture Register 2 */
#define	MX25_GPT_CNTR			0x24		/* GPT Counter Register */

/* GPT register bits */
#define MX25_GPT_CTL_ENABLE		0x01		/* CTL - Timer Enable Mode bit */
#define MX25_GPT_CTL_IPGCLK		0x40		/* CTL - ipg_clk select bit PERCLK1 */
#define MX25_GPT_CTL_NOTRESTART	0x200		/* CTL - free-run if set, RESTART IF CLEAR */
#define MX25_GPT_CTL_CMP1		0x01		/* INTR - enable cmp reg 1 */


/*
 * -------------------------------------------------------------------------
 * Enhanced Periodic Interrupt Timer (EPIT) registers
 * -------------------------------------------------------------------------
 */
#define	MX25_EPIT1_BASE			0x53F94000	/* Enhanced Periodic Interrupt Timer 1 Base */
#define	MX25_EPIT2_BASE			0x53F98000	/* Enhanced Periodic Interrupt Timer 2 Base */

#define	MX25_EPIT_SIZE	        0x14		/* EPIT register total size */
#define	MX25_EPIT1_IRQ			28			/* Enhanced Periodic Interrupt Timer 1 Base */

/* EPIT Registers, offset from base address */
#define	MX25_EPIT_CR			0x00		/* Control Register */
#define	MX25_EPIT_SR			0x04		/* Status Register */
#define	MX25_EPIT_LR			0x08		/* Load Register */
#define	MX25_EPIT_CMPR			0x0C		/* Compare Register */
#define	MX25_EPIT_CNR			0x10		/* Counter Register */


/*
 * -------------------------------------------------------------------------
 * Pulse-Width Modulator (PWM) Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_PWM1_BASE			0x53FE0000	/* PWM1 Base */
#define	MX25_PWM2_BASE			0x53FA0000	/* PWM2 Base */
#define	MX25_PWM3_BASE			0x53FA8000	/* PWM3 Base */
#define	MX25_PWM4_BASE			0x53FC8000	/* PWM4 Base */


/*
 * -------------------------------------------------------------------------
 * KeyPad Port (KPP) Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_KPP_BASE			0x43FA8000


/*
 * -------------------------------------------------------------------------
 * 1-Wire Interface (OWIRE) Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_OWIRE_BASE			0x43F9C000


/*
 * -------------------------------------------------------------------------
 * UARTs
 * -------------------------------------------------------------------------
 */
#define	MX25_UART1_BASE			0x43F90000	/* UART1 base address */
#define	MX25_UART2_BASE			0x43F94000	/* UART2 base address */
#define	MX25_UART3_BASE			0x5000C000	/* UART3 base address */
#define	MX25_UART4_BASE			0x50008000	/* UART4 base address */


/*
 * -------------------------------------------------------------------------
 * Configurable Serial Peripheral Interface (CSPI) Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_CSPI1_BASE			0x43FA4000	/* CSPI1 base address */
#define	MX25_CSPI2_BASE			0x50010000	/* CSPI2 base address */
#define	MX25_CSPI3_BASE			0x50004000	/* CSPI3 base address */


/*
 * -------------------------------------------------------------------------
 * Synchronous Serial Interface (SSI) Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_SSI1_BASE			0x50034000	/* SSI1 base address */
#define	MX25_SSI2_BASE			0x50014000	/* SSI2 base address */


/*
 * -------------------------------------------------------------------------
 * I2C  Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_I2C1_BASE			0x43F80000	/* I2C1 base address */
#define	MX25_I2C2_BASE			0x43F98000	/* I2C2 base address */
#define	MX25_I2C3_BASE			0x43F84000	/* I2C3 base address */

#define	MX25_I2C_ADR			0x00		/* I2C Address Register */
#define	MX25_I2C_FDR			0x04		/* I2C Frequency Divider Register */
#define	MX25_I2C_CR				0x08		/* I2C Control Register */
#define	MX25_I2C_SR				0x0C		/* I2C Status Register */
#define	MX25_I2C_DR				0x10		/* I2C Data I/O Registyer */

/* I2C Control Register bit defination */
#define	MX25_I2CR_RSTA			(1 << 2)	/* Repeat Start */
#define	MX25_I2CR_TXAK			(1 << 3)	/* Transmit Acknowledge Enable */
#define	MX25_I2CR_MTX			(1 << 4)	/* Transmit/Receive Mode Select */
#define	MX25_I2CR_MSTA			(1 << 5)	/* Master/Slave Mode Select */
#define	MX25_I2CR_IIEN			(1 << 6)	/* Interrupt Enable */
#define	MX25_I2CR_IEN			(1 << 7)	/* I2C Enable */

/* I2C Status Register bit defination */
#define	MX25_I2SR_RXAK			(1 << 0)	/* Received Acknowledge */
#define	MX25_I2SR_IIF			(1 << 1)	/* I2C Interrupt */
#define	MX25_I2SR_SRW			(1 << 2)	/* Slave Read/Write */
#define	MX25_I2SR_IAL			(1 << 4)	/* Arbitration Lost */
#define	MX25_I2SR_IBB			(1 << 5)	/* I2C Bus Busy */
#define	MX25_I2SR_IAAS			(1 << 6)	/* I2C Address as Slave */
#define	MX25_I2SR_ICF			(1 << 7)	/* Data Transferring */


/*
 * -------------------------------------------------------------------------
 * enhanced Secured Digital Host Controller Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_ESDHC1_BASE		0x53FB4000	/* SDHC1 base address */
#define	MX25_ESDHC2_BASE		0x53FB8000	/* SDHC2 base address */


/*
 * -------------------------------------------------------------------------
 * General-Purpose IO (GPIO) registers
 * -------------------------------------------------------------------------
 */
#define	MX25_GPIO1_BASE			0x53FCC000	/* GPIO1 base address */
#define	MX25_GPIO2_BASE			0x53FD0000	/* GPIO2 base address */
#define	MX25_GPIO3_BASE			0x53FA4000	/* GPIO3 base address */
#define	MX25_GPIO4_BASE			0x53F9C000	/* GPIO4 base address */

/* GPIO Registers, offset from base address */
#define	MX25_GPIO_DR			0x00
#define	MX25_GPIO_GDIR			0x04
#define	MX25_GPIO_PSR			0x08
#define	MX25_GPIO_ICR1			0x0C
#define	MX25_GPIO_ICR2			0x10
#define	MX25_GPIO_IMR			0x14
#define	MX25_GPIO_ISR			0x18
#define	MX25_GPIO_EDR			0x1C


/*
 * -------------------------------------------------------------------------
 * Digital Audio Mux (AUDMUX) Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_AUDMUX_BASE		0x43FB0000	/* AUDMUX base address */

/* Port offset */
#define MX25_AUDMUX_PTCR1		0x00
#define MX25_AUDMUX_PDCR1		0x04
#define MX25_AUDMUX_PTCR2		0x08
#define MX25_AUDMUX_PDCR2		0x0C
#define MX25_AUDMUX_PTCR3		0x10
#define MX25_AUDMUX_PDCR3		0x14
#define MX25_AUDMUX_PTCR4		0x18
#define MX25_AUDMUX_PDCR4		0x1C
#define MX25_AUDMUX_PTCR5		0x20
#define MX25_AUDMUX_PDCR5		0x24
#define MX25_AUDMUX_PTCR6		0x28
#define MX25_AUDMUX_PDCR6		0x2C
#define MX25_AUDMUX_PTCR7		0x30
#define MX25_AUDMUX_PDCR7		0x34
#define MX25_AUDMUX_CNMCR		0x38

/* Bit definition */
#define AUDMUX_SYNC				(1<<11)
#define AUDMUX_TFS_DIROUT		(1<<31)
#define AUDMUX_TFS_PORT4		(3<<27)
#define AUDMUX_TFS_PORT5		(4<<27)
#define AUDMUX_TCLK_DIROUT		(1<<26)
#define AUDMUX_TCLK_PORT4		(3<<22)
#define AUDMUX_TCLK_PORT5		(4<<22)
#define AUDMUX_RFS_DIROUT		(1<<21)
#define AUDMUX_RFS_PORT4		(3<<17)
#define AUDMUX_RFS_PORT5		(4<<17)
#define AUDMUX_RCLK_DIROUT		(1<<16)
#define AUDMUX_RCLK_PORT4		(3<<12)
#define AUDMUX_RCLK_PORT5		(4<<12)
#define AUDMUX_RXDSEL_PORT1		(0<<13)
#define AUDMUX_RXDSEL_PORT2		(1<<13)
#define AUDMUX_RXDSEL_PORT4		(3<<13)
#define AUDMUX_RXDSEL_PORT5		(4<<13)
#define AUDMUX_RXDSEL_MSK		(7<<13)


/*
 * -------------------------------------------------------------------------
 * LCD Controller Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_LCDC_BASE			0x53FBC000	/* LCDC base address */


/*
 * -------------------------------------------------------------------------
 * Smart LCD Controller Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_SLCDC_BASE			0x53FC0000	/* SLCDC base address */


/*
 * -------------------------------------------------------------------------
 * USB Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_USB_BASE			0x53FF4000	/* USB base address */
#define	MX25_USBOTG_BASE		0x53FF4000	/* USBOTG base address */
#define	MX25_USBHOST_BASE		0x53FF4400	/* USBHOST base address */


/*
 * -------------------------------------------------------------------------
 * Clock Reset Module (CRM) Registers
 * -------------------------------------------------------------------------
 */
#define	MX25_CRM_BASE			0x53F80000	/* CRM base address */  // REMAINDER unverified FOO

#define MX25_CRM_MPCTL			0x00		/* MCU PLL Control Register */
#define MX25_CRM_UPCTL			0x04		/* USB PLL Control Register */
#define MX25_CRM_CCTL			0x08		/* Clock Control Register */
#define MX25_CRM_CGR0			0x0C		/* Clock Gating Register 0 */
#define MX25_CRM_CGR1			0x10		/* Clock Gating Register 1 */
#define MX25_CRM_CGR2			0x14		/* Clock Gating Register 2 */
#define MX25_CRM_PDR0			0x18		/* perclk Divider Register 0 */
#define MX25_CRM_PDR1			0x1C		/* perclk Divider Register 1 */
#define MX25_CRM_PDR2			0x20		/* perclk Divider Register 2 */
#define MX25_CRM_PDR3			0x24		/* perclk Divider Register 3 */
#define MX25_CRM_CRSR			0x28		/* CRM Status Register */
#define MX25_CRM_CRDR			0x2C		/* CRM Debug Register */
#define MX25_CRM_DCVR0			0x30		/* DPTC Comparator Value Register 0 */
#define MX25_CRM_DCVR1			0x34		/* DPTC Comparator Value Register 1 */
#define MX25_CRM_DCVR2			0x38		/* DPTC Comparator Value Register 2 */
#define MX25_CRM_DCVR3			0x3C		/* DPTC Comparator Value Register 3 */
#define MX25_CRM_LTR0			0x40		/* Load Tracking Register 0 */
#define MX25_CRM_LTR1			0x44		/* Load Tracking Register 1 */
#define MX25_CRM_LTR2			0x48		/* Load Tracking Register 2 */
#define MX25_CRM_LTR3			0x4C		/* Load Tracking Register 3 */
#define MX25_CRM_LTBR0			0x50		/* Load Tracking Buffer Register 0 */
#define MX25_CRM_LTBR1			0x54		/* Load Tracking Buffer Register 1 */
#define MX25_CRM_PMCR0			0x58		/* Power Management Control Register 0 */
#define MX25_CRM_PMCR1			0x5C		/* Power Management Control Register 1 */
#define MX25_CRM_PMCR2			0x60		/* Power Management Control Register 2 */
#define MX25_CRM_MISC			0x64		/* CRM Misc Register */
#define MX25_CRM_LPIMR0			0x68		/* Low Power Interrupt Mask Registers 0 */
#define MX25_CRM_LPIMR1			0x6C		/* Low Power Interrupt Mask Registers 1 */

#define CCM_BASE_ADDR			MX25_CRM_BASE
#define CCM_MPCTL				(CCM_BASE_ADDR+0x00)       // MCU PLL Control Register
#define CCM_UPCTL				(CCM_BASE_ADDR+0x04)       // USB PLL Control Register
#define CCM_CCTL				(CCM_BASE_ADDR+0x08)       // Clock Control Register
#define CCM_CGR0				(CCM_BASE_ADDR+0x0C)       // Clock Gating Register 0
#define CCM_CGR1				(CCM_BASE_ADDR+0x10)       // Clock Gating Register 1
#define CCM_CGR2				(CCM_BASE_ADDR+0x14)       // Clock Gating Register 2
#define CCM_PDR0				(CCM_BASE_ADDR+0x18)       // perclk Divider Register 0
#define CCM_PDR1				(CCM_BASE_ADDR+0x1C)       // perclk Divider Register 1
#define CCM_PDR2				(CCM_BASE_ADDR+0x20)       // perclk Divider Register 2
#define CCM_PDR3				(CCM_BASE_ADDR+0x24)       // perclk Divider Register 3
#define CCM_CRSR				(CCM_BASE_ADDR+0x28)       // CRM Status Register
#define CCM_CRDR				(CCM_BASE_ADDR+0x2C)       // CRM Debug Register
#define CCM_DCVR0				(CCM_BASE_ADDR+0x30)       // DPTC Comparator Value Register 0
#define CCM_DCVR1				(CCM_BASE_ADDR+0x34)       // DPTC Comparator Value Register 1
#define CCM_DCVR2				(CCM_BASE_ADDR+0x38)       // DPTC Comparator Value Register 2
#define CCM_DCVR3				(CCM_BASE_ADDR+0x3C)       // DPTC Comparator Value Register 3
#define CCM_LTR0				(CCM_BASE_ADDR+0x40)       // Load Tracking Register 0
#define CCM_LTR1				(CCM_BASE_ADDR+0x44)       // Load Tracking Register 1
#define CCM_LTR2				(CCM_BASE_ADDR+0x48)       // Load Tracking Register 2
#define CCM_LTR3				(CCM_BASE_ADDR+0x4C)       // Load Tracking Register 3
#define CCM_LTBR0				(CCM_BASE_ADDR+0x50)       // Load Tracking Buffer Register 0
#define CCM_LTBR1				(CCM_BASE_ADDR+0x54)       // Load Tracking Buffer Register 1
#define CCM_PMCR0				(CCM_BASE_ADDR+0x58)       // Power Management Control Register 0
#define CCM_PMCR1				(CCM_BASE_ADDR+0x5C)       // Power Management Control Register 1
#define CCM_PMCR2				(CCM_BASE_ADDR+0x60)       // Power Management Control Register 2
#define CCM_MISC				(CCM_BASE_ADDR+0x64)       // CRM Misc Register
#define CCM_LPIMR0				(CCM_BASE_ADDR+0x68)       // Low Power Interrupt Mask Registers 0
#define CCM_LPIMR1				(CCM_BASE_ADDR+0x6C)       // Low Power Interrupt Mask Registers 1

/* MCR Misc Control Register Bit definition */
    #define MX25_CCM_CLKOSEL(x)             ((x&0x0f) << 20)
        #define MX25_CLKO_ARMCLOCK          2
        #define MX25_CLKO_USBCLKSRC         6
    #define MX25_CCM_CLKODIV(x)             ((x&0x3f) << 24)
    #define MX25_CCM_CLKOEN                 (1 << 30)


/*
 * -------------------------------------------------------------------------
 * Random Number Generator (RNGB)
 * -------------------------------------------------------------------------
 */
#define	MX25_RNGB_BASE			0x53FB0000	/* Random Number Generator base address */


/*
 * -------------------------------------------------------------------------
 * Run-Time Integrity Checker (RTIC)
 * -------------------------------------------------------------------------
 */
#define	MX25_RTIC_BASE			0x53FEC000	/* RTIC base address - rticv3 */

/*
 * -------------------------------------------------------------------------
 * Multi-Layer AHB Crossbar Switch (MAX)
 * -------------------------------------------------------------------------
 */
#define	MX25_MAX_BASE			0x43F04000	/* ARM platform MAX base address */


/*
 * -------------------------------------------------------------------------
 * Interrupt controller
 * -------------------------------------------------------------------------
 */
#define	MX25_ASIC_BASE			0x68000000	/* ARM platform ASIC base address */


/*
 * -------------------------------------------------------------------------
 * SDRAM Controller
 * -------------------------------------------------------------------------
 */
#define	MX25_SDRAMC_BASE		0xB8001000	/* SDRAM Controller base address */
#define	MX25_SDRAM0_BASE		0x80000000	/* SDRAM Bank 0 base address */
#define	MX25_SDRAM1_BASE		0x90000000	/* SDRAM Bank 1 base address */


/*
 * -------------------------------------------------------------------------
 * Wireless External Interface Module (WEIM)
 * -------------------------------------------------------------------------
 */
#define	MX25_EIM_BASE			0xB8002000	/* EIM base address - controller */

#define	MX25_EIM_CS0UCR			0x00		/* Chip Select 0 Upper Control Register */
#define	MX25_EIM_CS0LCR			0x04		/* Chip Select 0 Lower Control Register */
#define	MX25_EIM_CS0ACR			0x08		/* Chip Select 0 Addition Control Register */
#define	MX25_EIM_CS1UCR			0x10		/* Chip Select 1 Upper Control Register */
#define	MX25_EIM_CS1LCR			0x14		/* Chip Select 1 Lower Control Register */
#define	MX25_EIM_CS1ACR			0x18		/* Chip Select 1 Addition Control Register */
#define	MX25_EIM_CS2UCR			0x20		/* Chip Select 2 Upper Control Register */
#define	MX25_EIM_CS2LCR			0x24		/* Chip Select 2 Lower Control Register */
#define	MX25_EIM_CS2ACR			0x28		/* Chip Select 2 Addition Control Register */
#define	MX25_EIM_CS3UCR			0x30		/* Chip Select 3 Upper Control Register */
#define	MX25_EIM_CS3LCR			0x34		/* Chip Select 3 Lower Control Register */
#define	MX25_EIM_CS3ACR			0x38		/* Chip Select 3 Addition Control Register */
#define	MX25_EIM_CS4UCR			0x40		/* Chip Select 4 Upper Control Register */
#define	MX25_EIM_CS4LCR			0x44		/* Chip Select 4 Lower Control Register */
#define	MX25_EIM_CS4ACR			0x48		/* Chip Select 4 Addition Control Register */
#define	MX25_EIM_CS5UCR			0x50		/* Chip Select 5 Upper Control Register */
#define	MX25_EIM_CS5LCR			0x54		/* Chip Select 5 Lower Control Register */
#define	MX25_EIM_CS5ACR			0x58		/* Chip Select 5 Addition Control Register */
#define	MX25_EIM_WCR			0x60		/* WEIM Configuration Register */


/*
 * -------------------------------------------------------------------------
 * NAND Flash Controller
 * -------------------------------------------------------------------------
 */
#define	MX25_NAND_BASE			0xBB000000	/* NAND base address */

/*
 * -------------------------------------------------------------------------
 * DRYICE 
 * -------------------------------------------------------------------------
 */

#define MX25_DRYICE_BASE  0x53FFC000   /*DRYICE base address */
#define MX25_DRYICE_SIZE  0x80   				/*DRYICE size */
#define MX25_DRYICE_DTCMR     0x00           /* Time Counter MSB Reg */
#define MX25_DRYICE_DTCLR     0x04           /* Time Counter LSB Reg */

#define MX25_DRYICE_DCR       0x10           /* Control Reg */
#define DCR_TCE   (1 << 3)       /* Time Counter Enable */

#define MX25_DRYICE_DSR       0x14           /* Status Reg */
#define DSR_WBF   (1 << 10)      /* Write Busy Flag */
#define DSR_WNF   (1 << 9)       /* Write Next Flag */
#define DSR_WCF   (1 << 8)       /* Write Complete Flag */
#define DSR_WEF   (1 << 7)       /* Write Error Flag */
#define DSR_CAF   (1 << 4)       /* Clock Alarm Flag */
#define DSR_NVF   (1 << 1)       /* Non-Valid Flag */
#define DSR_SVF   (1 << 0)       /* Security Violation Flag */


#endif	/* __ARM_MX25_H_INCLUDED */

/* __SRCVERSION( "$URL$ $Rev$" ); */
