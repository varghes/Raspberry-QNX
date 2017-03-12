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
 * i.MX31 processor
 *
 */

#ifndef	__ARM_MX31_H_INCLUDED
#define	__ARM_MX31_H_INCLUDED


/*
 * Enhanced SDRAM Controller Base Address
 */
#define	MX31_ESDCTL_BASE		0xB8001000

/*
 * Wireless External Interface Module Base Address
 */
#define	MX31_WEIM_BASE			0xB8002000

/* 
 * WEIM registers : offset from WEIM base address
 */
#define	MX31_WEIM_CSCR0U		0x00
#define	MX31_WEIM_CSCR0L		0x04
#define	MX31_WEIM_CSCR0A		0x08
#define	MX31_WEIM_CSCR1U		0x10
#define	MX31_WEIM_CSCR1L		0x14
#define	MX31_WEIM_CSCR1A		0x18
#define	MX31_WEIM_CSCR2U		0x20
#define	MX31_WEIM_CSCR2L		0x24
#define	MX31_WEIM_CSCR2A		0x28
#define	MX31_WEIM_CSCR3U		0x30
#define	MX31_WEIM_CSCR3L		0x34
#define	MX31_WEIM_CSCR3A		0x38
#define	MX31_WEIM_CSCR4U		0x40
#define	MX31_WEIM_CSCR4L		0x44
#define	MX31_WEIM_CSCR4A		0x48
#define	MX31_WEIM_CSCR5U		0x50
#define	MX31_WEIM_CSCR5L		0x54
#define	MX31_WEIM_CSCR5A		0x58
#define	MX31_WEIM_WCR			0x60


/* Enhanced SDRAM Control Registers, offset from base address */
#define	MX31_ESDCTL0			0x00	/* Enhanced SDRAM Control Register 0 */
#define	MX31_ESDCFG0			0x04	/* Enhanced SDRAM Configuration Register 0 */
#define	MX31_ESDCTL1			0x08	/* Enhanced SDRAM Control Register 1 */
#define	MX31_ESDCFG1			0x0C	/* Enhanced SDRAM Configuration Register 1 */
#define	MX31_ESDMISC			0x10	/* Enhanced SDRAM Miscellaneous Register */

/*
 * L2 Cache Controller Base Address
 */
#define	MX31_L2CC_BASE			0x30000000

/*
 * I2Cs Base Address
 */
#define	MX31_I2C_BASE			0x43F80000
#define	MX31_I2C2_BASE			0x43F98000
#define	MX31_I2C3_BASE			0x43F84000

/*
 * USBOTG Base Address
 */
#define	MX31_USBOTG_BASE		0x43F88000


/*
 * ATA Control Port Base Address
 */
#define	MX31_ATACTL_BASE		0x43F8C000

/*
 * UARTs Base Address
 */
#define	MX31_UART1_BASE			0x43F90000
#define	MX31_UART2_BASE			0x43F94000
#define	MX31_UART3_BASE			0x5000C000
#define	MX31_UART4_BASE			0x43FB0000
#define	MX31_UART5_BASE			0x43FB4000

/*
 * IOMUXC Base Address
 */
#define	MX31_IOMUXC_BASE		0x43FAC000
#define	MX31_IOMUXC_SIZE		0x310

/* IOMUX Registers, off set from base address */
#define	MX31_IOMUX_OBS1			0x00
#define	MX31_IOMUX_OBS2			0x04
#define	MX31_IOMUX_GPR			0x08
#define	MX31_IOMUX_SWMUX		0x0C
#define	MX31_IOMUX_SWPAD		0x154


/*
 * ATA DMA Port Base Address
 */
#define	MX31_ATADMA_BASE		0x50020000


/*
 * Clock Controller Module Base Address
 */
#define	MX31_CCM_BASE			0x53F80000

/* CCM Registers, off set from base address */
#define	MX31_CCM_CCMR			0x00
#define	MX31_CCM_PDR0			0x04
#define	MX31_CCM_PDR1			0x08
#define	MX31_CCM_RCSR			0x0C
#define	MX31_CCM_MPCTL			0x10
#define	MX31_CCM_UPCTL			0x14
#define	MX31_CCM_SPCTL			0x18
#define	MX31_CCM_COSR			0x1C
#define	MX31_CCM_CGR0			0x20
#define	MX31_CCM_CGR1			0x24
#define	MX31_CCM_CGR2			0x28
#define	MX31_CCM_WIMR0			0x2C
#define	MX31_CCM_LDC			0x30
#define	MX31_CCM_DCVR0			0x34
#define	MX31_CCM_DCVR1			0x38
#define	MX31_CCM_DCVR2			0x3C
#define	MX31_CCM_DCVR3			0x40
#define	MX31_CCM_LTR0			0x44
#define	MX31_CCM_LTR1			0x48
#define	MX31_CCM_LTR2			0x4C
#define	MX31_CCM_LTR3			0x50
#define	MX31_CCM_LTBR0			0x54
#define	MX31_CCM_LTBR1			0x58
#define	MX31_CCM_PMCR0			0x5C
#define	MX31_CCM_PMCR1			0x60
#define	MX31_CCM_PDR2			0x64

/*
 * GPT Base Address 
 */
#define	MX31_GPT1_BASE			0x53F90000

/* GPT Registers, off set from base address */
#define	MX31_GPTCR				0x00
#define	MX31_GPTPR				0x04
#define	MX31_GPTSR				0x08
#define	MX31_GPTIR				0x0C
#define	MX31_GPTOCR1			0x10
#define	MX31_GPTOCR2			0x14
#define	MX31_GPTOCR3			0x18
#define	MX31_GPTICR1			0x1C
#define MX31_GPTICR2			0x20
#define	MX31_GPTCNT				0x24

/*
 * Enhanced Periodic Interrupt Timer (EPIT) Base Address 
 */
#define	MX31_EPIT1_BASE			0x53F94000
#define	MX31_EPIT2_BASE			0x53F98000

/* EPIT Registers, off set from base address */
#define	MX31_EPIT_CR			0x00	/* Control Register */
#define	MX31_EPIT_SR			0x04	/* Status Register */
#define	MX31_EPIT_LR			0x08	/* Load Register */
#define	MX31_EPIT_CMPR			0x0C	/* Compare Register */
#define	MX31_EPIT_CNR			0x10	/* Counter Register */

/*
 * GPIOs Base Address 
 */
#define	MX31_GPIO1_BASE			0x53FCC000
#define	MX31_GPIO2_BASE			0x53FD0000
#define	MX31_GPIO3_BASE			0x53FA4000

/* GPIO Registers, offset from base address */
#define	MX31_GPIO_DR			0x00
#define	MX31_GPIO_GDIR			0x04
#define	MX31_GPIO_PSR			0x08
#define	MX31_GPIO_ICR1			0x0C
#define	MX31_GPIO_ICR2			0x10
#define	MX31_GPIO_IMR			0x14
#define	MX31_GPIO_ISR			0x18

/*
 * SDMA Base Address
 */
#define	MX31_SDMA_BASE			0x53FD4000
#define MX31_SDMA_SIZE          0x4000

/* SDMA Registers, off set from base address */
#define	MX31_SDMA_MC0PTR		0x00	/* AP (MCU) Channel 0 Pointer */
#define	MX31_SDMA_INTR			0x04	/* Channel Interrupts */
#define	MX31_SDMA_STOP_STAT		0x08	/* Channel Stop / Channel Status */
#define	MX31_SDMA_HSTART		0x0C	/* Channel Start */
#define	MX31_SDMA_EVTOVR		0x10	/* Channel Event Override */
#define	MX31_SDMA_DSPOVR		0x14	/* Channel DSP (BP) Override */
#define	MX31_SDMA_HOSTOVR		0x18	/* Channel AP Override */
#define	MX31_SDMA_EVTPEND		0x1C	/* Channel Event Pending */
#define	MX31_SDMA_RESET			0x24	/* Reset Register */
#define	MX31_SDMA_EVTERR		0x28	/* DMA Request Error Register */
#define	MX31_SDMA_INTRMASK		0x2C	/* Channel AP Interrupt Mask */
#define	MX31_SDMA_PSW			0x30	/* Schedule Status */
#define	MX31_SDMA_EVTERRDBG		0x34	/* DMA Request Error Register */
#define	MX31_SDMA_CONFIG		0x38	/* Configuration Register */
#define	MX31_SDMA_ONCE_ENB		0x40	/* OnCE Enable */
#define	MX31_SDMA_ONCE_DATA		0x44	/* OnCE Data Register */
#define	MX31_SDMA_ONCE_INSTR	0x48	/* OnCE Instruction Register */
#define	MX31_SDMA_ONCE_STAT		0x4C	/* OnCE Status Register */
#define	MX31_SDMA_ONCE_CMD		0x50	/* OnCE Command Register */
#define	MX31_SDMA_EVT_MIRROT	0x54	/* DMA Request */
#define	MX31_SDMA_ILLINSTADDR	0x58	/* Illegal Instruction Trap Address */
#define	MX31_SDMA_CHN0ADDR		0x5C	/* Channel 0 Boot Address */
#define	MX31_SDMA_XTRIG_CONF1	0x70	/* Cross-Trigger Events Configuration Register 1 */
#define	MX31_SDMA_XTRIG_CONF2	0x74	/* Cross-Trigger Events Configuration Register 2 */
#define MX31_SDMA_CHNENBL(n)    (0x80 + ((n)<<2))   /* Channel Enable n = 0 to 31 */
#define MX31_SDMA_CHNPRI(n)     (0x100 + ((n)<<2))   /* Channel Priority n = 0 to 31 */


/*
 * RTC Base Address
 */
#define	MX31_RTC_BASE			0x53FD8000

/*
 * Watchdog Timer Base Address
 */
#define	MX31_WDOG_BASE			0x53FDC000

/* Watchdog Timer Registers, offset from base address */
#define	MX31_WDOG_CR			0x00
#define	MX31_WDOG_SR			0x02
#define	MX31_WDOG_RSR			0x04

/*
 * ARM11 Vector Interrupt Controller Base Address
 */
#define	MX31_AVIC_BASE			0x68000000


/*
 * NAND flash controller base address and size
 */
#define MX31_NAND_BASE			0xB8000000
#define MX31_NAND_SIZE			0x00001000

/* NAND flash controller register offsets */
#define MX31_NAND_MAIN_BUF_0		0x000
#define MX31_NAND_MAIN_BUF_1		0x200
#define MX31_NAND_MAIN_BUF_2		0x400
#define MX31_NAND_MAIN_BUF_3		0x600

#define MX31_NAND_SPARE_BUF_0		0x800
#define MX31_NAND_SPARE_BUF_1		0x810
#define MX31_NAND_SPARE_BUF_2		0x820
#define MX31_NAND_SPARE_BUF_3		0x830

#define	MX31_NFC_REGISTER			0xE00

#define MX31_NFC_BUFSIZ				0x000
#define MX31_RAM_BUFFER_ADDRESS		0x004
#define MX31_NAND_FLASH_ADD			0x006
#define MX31_NAND_FLASH_CMD			0x008
#define MX31_NFC_CONFIGURATION		0x00A
#define MX31_ECC_STATUS_RESULT		0x00C
#define MX31_ECC_RSLT_MAIN_AREA		0x00E
#define MX31_ECC_RSLT_SPARE_AREA	0x010
#define MX31_NF_WR_PROT				0x012
#define MX31_UNLOCK_START_BLK_ADD	0x014
#define MX31_UNLOCK_END_BLK_ADD		0x016
#define MX31_NAND_FLASH_WR_PR_ST	0x018
#define MX31_NAND_FLASH_CONFIG1		0x01A
#define MX31_NAND_FLASH_CONFIG2		0x01C

/* bit definitions for various NAND registers */
#define NAND_CONFIG1_NF_CE			0x80	// force CE# always
#define NAND_CONFIG1_NFC_RST		0x40	// reset NAND state machine
#define NAND_CONFIG1_NF_BIG			0x20	// enable big endian mode
#define NAND_CONFIG1_INT_MSK		0x10	// mask interrupt - 0 == interrupt enabled, 1 == interrupt disabled
#define NAND_CONFIG1_ECC_EN			0x08	// enable ECC operation
#define NAND_CONFIG1_SP_EN			0x04	// enable spare area only

#define NAND_CONFIG2_INT			0x8000	// status; 1 == ready, 0 == busy

#define NAND_CONFIG2_FDO_PDO		0x08	// 1 page data out
#define NAND_CONFIG2_FDO_ID_OUT		0x10	// flash ID data out
#define NAND_CONFIG2_FDO_STAT_OUT	0x20	// flash status register data out

#define NAND_CONFIG2_FDI			0x04	// enable NAND data input
#define NAND_CONFIG2_FADD			0x02	// enable NAND address input
#define NAND_CONFIG2_FCMD			0x01	// enable NAND command input


#endif	/* __ARM_MX31_H_INCLUDED */

/* __SRCVERSION("mx31.h $Rev: 657836 $"); */
