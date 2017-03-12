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
 * i.MX35 processor
 *
 */

#ifndef	__ARM_MX35_H_INCLUDED
#define	__ARM_MX35_H_INCLUDED


/*
 * Enhanced SDRAM Controller Base Address
 */
#define	MX35_ESDCTL_BASE		0xB8001000

/*
 * Wireless External Interface Module Base Address
 */
#define	MX35_WEIM_BASE			0xB8002000

/* 
 * WEIM registers : offset from WEIM base address
 */
#define	MX35_WEIM_CSCR0U		0x00
#define	MX35_WEIM_CSCR0L		0x04
#define	MX35_WEIM_CSCR0A		0x08
#define	MX35_WEIM_CSCR1U		0x10
#define	MX35_WEIM_CSCR1L		0x14
#define	MX35_WEIM_CSCR1A		0x18
#define	MX35_WEIM_CSCR2U		0x20
#define	MX35_WEIM_CSCR2L		0x24
#define	MX35_WEIM_CSCR2A		0x28
#define	MX35_WEIM_CSCR3U		0x30
#define	MX35_WEIM_CSCR3L		0x34
#define	MX35_WEIM_CSCR3A		0x38
#define	MX35_WEIM_CSCR4U		0x40
#define	MX35_WEIM_CSCR4L		0x44
#define	MX35_WEIM_CSCR4A		0x48
#define	MX35_WEIM_CSCR5U		0x50
#define	MX35_WEIM_CSCR5L		0x54
#define	MX35_WEIM_CSCR5A		0x58
#define	MX35_WEIM_WCR			0x60


/* Enhanced SDRAM Control Registers, offset from base address */
#define	MX35_ESDCTL0			0x00	/* Enhanced SDRAM Control Register 0 */
#define	MX35_ESDCFG0			0x04	/* Enhanced SDRAM Configuration Register 0 */
#define	MX35_ESDCTL1			0x08	/* Enhanced SDRAM Control Register 1 */
#define	MX35_ESDCFG1			0x0C	/* Enhanced SDRAM Configuration Register 1 */
#define	MX35_ESDMISC			0x10	/* Enhanced SDRAM Miscellaneous Register */
#define	MX35_ESDCDLY1			0x20	/* Enhanced MDDR Delay Line 1 Configuration Debug Register */
#define	MX35_ESDCDLY2			0x24	/* Enhanced MDDR Delay Line 2 Configuration Debug Register */
#define	MX35_ESDCDLY3			0x28	/* Enhanced MDDR Delay Line 3 Configuration Debug Register */
#define	MX35_ESDCDLY4			0x2C	/* Enhanced MDDR Delay Line 4 Configuration Debug Register */
#define	MX35_ESDCDLY5			0x30	/* Enhanced MDDR Delay Line 5 Configuration Debug Register */
#define	MX35_ESDCDLYL			0x30	/* Enhanced MDDR Delay Line Cycle Length Debug Register */

/*
 * L2 Cache Controller Base Address
 */
#define	MX35_L2CC_BASE			0x30000000

/*
 * I2Cs Base Address
 */
#define	MX35_I2C_BASE			0x43F80000
#define	MX35_I2C2_BASE			0x43F98000
#define	MX35_I2C3_BASE			0x43F84000

/*
 * USB Base Address
 */
#define	MX35_USB_BASE			0x53FF4000

/*
 * UARTs Base Address
 */
#define	MX35_UART1_BASE			0x43F90000
#define	MX35_UART2_BASE			0x43F94000
#define	MX35_UART3_BASE			0x5000C000

/*
 * IOMUXC Base Address
 */
#define	MX35_IOMUXC_BASE		0x43FAC000
#define	MX35_IOMUXC_SIZE		0x10000

/* IOMUX Registers, offset from base address */
#define	MX35_IOMUX_GPR			0x0
#define	MX35_IOMUX_SWMUX		0x04
#define	MX35_IOMUX_SWPAD		0x328
#define MX35_IOMUX_SWINPUT		0x7a8


/*
 * ATA DMA Port Base Address
 */
#define	MX35_ATACTL_BASE		0x50020000


/*
 * Clock Controller Module Base Address
 */
#define	MX35_CCM_BASE			0x53F80000

/* CCM Registers, offset from base address */
#define	MX35_CCM_CCMR			0x00
#define	MX35_CCM_PDR0			0x04
#define	MX35_CCM_PDR1			0x08
#define	MX35_CCM_PDR2			0x0C
#define	MX35_CCM_PDR3			0x10
#define	MX35_CCM_PDR4			0x14
#define	MX35_CCM_RCSR			0x18
#define	MX35_CCM_MPCTL			0x1C
#define	MX35_CCM_PPCTL			0x20
#define	MX35_CCM_ACMR			0x24
#define	MX35_CCM_COSR			0x28
	#define MX35_CCM_CLKOSEL(x)		(x & 0x1F)
		#define MX35_CLKOSEL_ARMCLOCK	0x07
		#define MX35_CLKOSEL_AUDIOCLOCK	0x02
			#define MX35_AUDIOCLOCK	24576000
	#define MX35_CCM_CLKOEN			(1 << 5)
	#define MX35_CCM_CLKO_DIV1(x)		((x&0x01) << 6)
	#define MX35_CCM_CLKO_DIV(x)		((x&0x3F) << 10)
#define	MX35_CCM_CGR0			0x2C
#define	MX35_CCM_CGR1			0x30
#define	MX35_CCM_CGR2			0x34
#define	MX35_CCM_CGR3			0x38
#define	MX35_CCM_DCVR0			0x40
#define	MX35_CCM_DCVR1			0x44
#define	MX35_CCM_DCVR2			0x48
#define	MX35_CCM_DCVR3			0x4C
#define	MX35_CCM_LTR0			0x50
#define	MX35_CCM_LTR1			0x54
#define	MX35_CCM_LTR2			0x58
#define	MX35_CCM_LTR3			0x5C
#define	MX35_CCM_LTBR0			0x60
#define	MX35_CCM_LTBR1			0x64
#define	MX35_CCM_PMCR0			0x68
#define	MX35_CCM_PMCR1			0x6C
#define	MX35_CCM_PMCR2			0x70

/*
 * GPT Base Address 
 */
#define	MX35_GPT1_BASE			0x53F90000

/* GPT Registers, offset from base address */
#define	MX35_GPTCR				0x00
#define	MX35_GPTPR				0x04
#define	MX35_GPTSR				0x08
#define	MX35_GPTIR				0x0C
#define	MX35_GPTOCR1			0x10
#define	MX35_GPTOCR2			0x14
#define	MX35_GPTOCR3			0x18
#define	MX35_GPTICR1			0x1C
#define MX35_GPTICR2			0x20
#define	MX35_GPTCNT				0x24

/*
 * Enhanced Periodic Interrupt Timer (EPIT) Base Address 
 */
#define	MX35_EPIT1_BASE			0x53F94000
#define	MX35_EPIT2_BASE			0x53F98000

/* EPIT Registers, offset from base address */
#define	MX35_EPIT_CR			0x00	/* Control Register */
#define	MX35_EPIT_SR			0x04	/* Status Register */
#define	MX35_EPIT_LR			0x08	/* Load Register */
#define	MX35_EPIT_CMPR			0x0C	/* Compare Register */
#define	MX35_EPIT_CNR			0x10	/* Counter Register */

/*
 * GPIOs Base Address 
 */
#define	MX35_GPIO1_BASE			0x53FCC000
#define	MX35_GPIO2_BASE			0x53FD0000
#define	MX35_GPIO3_BASE			0x53FA4000

/* GPIO Registers, offset from base address */
#define	MX35_GPIO_DR			0x00
#define	MX35_GPIO_GDIR			0x04
#define	MX35_GPIO_PSR			0x08
#define	MX35_GPIO_ICR1			0x0C
#define	MX35_GPIO_ICR2			0x10
#define	MX35_GPIO_IMR			0x14
#define	MX35_GPIO_ISR			0x18
#define	MX35_GPIO_EDR			0x1C

/*
 * SDMA Base Address
 */
#define	MX35_SDMA_BASE			0x53FD4000
#define MX35_SDMA_SIZE          0x4000

/* SDMA Registers, offset from base address */
#define	MX35_SDMA_MC0PTR		0x00	/* AP (MCU) Channel 0 Pointer */
#define	MX35_SDMA_INTR			0x04	/* Channel Interrupts */
#define	MX35_SDMA_STOP_STAT		0x08	/* Channel Stop / Channel Status */
#define	MX35_SDMA_HSTART		0x0C	/* Channel Start */
#define	MX35_SDMA_EVTOVR		0x10	/* Channel Event Override */
#define	MX35_SDMA_DSPOVR		0x14	/* Channel DSP (BP) Override */
#define	MX35_SDMA_HOSTOVR		0x18	/* Channel AP Override */
#define	MX35_SDMA_EVTPEND		0x1C	/* Channel Event Pending */
#define	MX35_SDMA_RESET			0x24	/* Reset Register */
#define	MX35_SDMA_EVTERR		0x28	/* DMA Request Error Register */
#define	MX35_SDMA_INTRMASK		0x2C	/* Channel AP Interrupt Mask */
#define	MX35_SDMA_PSW			0x30	/* Schedule Status */
#define	MX35_SDMA_EVTERRDBG		0x34	/* DMA Request Error Register */
#define	MX35_SDMA_CONFIG		0x38	/* Configuration Register */
#define	MX35_SDMA_ONCE_ENB		0x40	/* OnCE Enable */
#define	MX35_SDMA_ONCE_DATA		0x44	/* OnCE Data Register */
#define	MX35_SDMA_ONCE_INSTR	0x48	/* OnCE Instruction Register */
#define	MX35_SDMA_ONCE_STAT		0x4C	/* OnCE Status Register */
#define	MX35_SDMA_ONCE_CMD		0x50	/* OnCE Command Register */
#define	MX35_SDMA_EVT_MIRROT	0x54	/* DMA Request */
#define	MX35_SDMA_ILLINSTADDR	0x58	/* Illegal Instruction Trap Address */
#define	MX35_SDMA_CHN0ADDR		0x5C	/* Channel 0 Boot Address */
#define	MX35_SDMA_XTRIG_CONF1	0x70	/* Cross-Trigger Events Configuration Register 1 */
#define	MX35_SDMA_XTRIG_CONF2	0x74	/* Cross-Trigger Events Configuration Register 2 */
#define MX35_SDMA_CHNPRI(n)     (0x100 + ((n)<<2))   /* Channel Priority n = 0 to 31 */
#define MX35_SDMA_CHNENBL(n)    (0x200 + ((n)<<2))   /* Channel Enable n = 0 to 47 */


/* 
 * Digital Audio Multiplexer 
 */
#define MX35_AUDMUX_BASE		0x53FC4000

/* Port offset */
#define MX35_AUDMUX_PTCR1		0x00
#define MX35_AUDMUX_PDCR1		0x04
#define MX35_AUDMUX_PTCR2		0x08
#define MX35_AUDMUX_PDCR2		0x0C
#define MX35_AUDMUX_PTCR3		0x10
#define MX35_AUDMUX_PDCR3		0x14
#define MX35_AUDMUX_PTCR4		0x18
#define MX35_AUDMUX_PDCR4		0x1C
#define MX35_AUDMUX_PTCR5		0x20
#define MX35_AUDMUX_PDCR5		0x24
#define MX35_AUDMUX_PTCR6		0x28
#define MX35_AUDMUX_PDCR6		0x2C
#define MX35_AUDMUX_PTCR7		0x30
#define MX35_AUDMUX_PDCR7		0x34
#define MX35_AUDMUX_CNMCR		0x38
#define MX35_AUDMUX_CNMCR		0x38

/* Bit definition */
#define AUDMUX_SYNC			(1<<11)
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
 * RTC Base Address
 */
#define	MX35_RTC_BASE			0x53FD8000

/*
 * Watchdog Timer Base Address
 */
#define	MX35_WDOG_BASE			0x53FDC000

/* Watchdog Timer Registers, offset from base address */
#define	MX35_WDOG_CR			0x00
#define	MX35_WDOG_SR			0x02
#define	MX35_WDOG_RSR			0x04
#define	MX35_WDOG_ICR			0x06
#define	MX35_WDOG_MCR			0x08

/*
 * ARM11 Vector Interrupt Controller Base Address
 */
#define	MX35_AVIC_BASE			0x68000000


/*
 * NAND flash controller base address and size
 */
#define MX35_NAND_BASE			0xBB000000
#define MX35_NAND_SIZE			0x00002000

/* NAND flash controller register offsets */
#define MX35_NAND_MAIN_BUF_0		0x000
#define MX35_NAND_MAIN_BUF_1		0x200
#define MX35_NAND_MAIN_BUF_2		0x400
#define MX35_NAND_MAIN_BUF_3		0x600
#define MX35_NAND_MAIN_BUF_4		0x800
#define MX35_NAND_MAIN_BUF_5		0xA00
#define MX35_NAND_MAIN_BUF_6		0xC00
#define MX35_NAND_MAIN_BUF_7		0xE00

#define MX35_NAND_SPARE_BUF_0		0x1000
#define MX35_NAND_SPARE_BUF_1		0x1040
#define MX35_NAND_SPARE_BUF_2		0x1080
#define MX35_NAND_SPARE_BUF_3		0x10C0
#define MX35_NAND_SPARE_BUF_4		0x1100
#define MX35_NAND_SPARE_BUF_5		0x1140
#define MX35_NAND_SPARE_BUF_6		0x1180
#define MX35_NAND_SPARE_BUF_7		0x11C0

#define	MX35_NFC_REGISTER			0x1E00

#define MX35_RAM_BUFFER_ADDRESS		0x004
#define MX35_NAND_FLASH_ADD			0x006
#define MX35_NAND_FLASH_CMD			0x008
#define MX35_NFC_CONFIGURATION		0x00A
#define MX35_ECC_STATUS_RESULT1		0x00C
#define MX35_ECC_STATUS_RESULT2		0x00E
#define MX35_SPAS					0x010
#define MX35_NF_WR_PROT				0x012
#define MX35_NAND_FLASH_WR_PR_ST	0x018
#define MX35_NAND_FLASH_CONFIG1		0x01A
#define MX35_NAND_FLASH_CONFIG2		0x01C
#define MX35_UNLOCK_START_BLK_ADD0	0x020
#define MX35_UNLOCK_END_BLK_ADD0	0x022
#define MX35_UNLOCK_START_BLK_ADD1	0x024
#define MX35_UNLOCK_END_BLK_ADD1	0x026
#define MX35_UNLOCK_START_BLK_ADD2	0x028
#define MX35_UNLOCK_END_BLK_ADD2	0x02A
#define MX35_UNLOCK_START_BLK_ADD3	0x02C
#define MX35_UNLOCK_END_BLK_ADD3	0x02E

/* bit definitions for various NAND registers */
#define NAND_CONFIG1_FP_INT			0x800	// full Page interrupt
#define NAND_CONFIG1_PPB256			0x600	// 256 pages per block
#define NAND_CONFIG1_PPB128			0x400	// 128 pages per block
#define NAND_CONFIG1_PPB64			0x200	// 64 pages per block
#define NAND_CONFIG1_PPB32			0x000	// 32 pages per block
#define NAND_CONFIG1_SYM			0x100	// enable one flash clock cycle per access of RE# and WE#, (symmetric mode)
#define NAND_CONFIG1_NF_CE			0x80	// force CE# always
#define NAND_CONFIG1_NFC_RST		0x40	// reset NAND state machine
#define NAND_CONFIG1_NF_BIG			0x20	// enable big endian mode
#define NAND_CONFIG1_INT_MSK		0x10	// mask interrupt - 0 == interrupt enabled, 1 == interrupt disabled
#define NAND_CONFIG1_ECC_EN			0x08	// enable ECC operation
#define NAND_CONFIG1_SP_EN			0x04	// enable spare area only
#define NAND_CONFIG1_DMA_MODE		0x02	// dma_req is asserted only at the end of a page read
#define NAND_CONFIG1_ECC_MODE		0x01	// 4bit error correction

#define NAND_CONFIG2_INT			0x8000	// status; 1 == ready, 0 == busy

#define NAND_CONFIG2_FDO_PDO		0x08	// 1 page data out
#define NAND_CONFIG2_FDO_ID_OUT		0x10	// flash ID data out
#define NAND_CONFIG2_FDO_STAT_OUT	0x20	// flash status register data out

#define NAND_CONFIG2_FDI			0x04	// enable NAND data input
#define NAND_CONFIG2_FADD			0x02	// enable NAND address input
#define NAND_CONFIG2_FCMD			0x01	// enable NAND command input


#endif	/* __ARM_MX35_H_INCLUDED */

/* __SRCVERSION( "$URL$ $Rev$" ); */
