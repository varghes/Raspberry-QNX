/*
 * $QNXLicenseC:
 * Copyright 2007, 2008, QNX Software Systems. 
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


#ifndef	_SDHCI_INCLUDED
#define	_SDHCI_INCLUDED

#define SDHCI_ADMA_SUP

#define	__REG8(x)		(*((volatile uint8_t  *)(x)))
#define	__REG16(x)		(*((volatile uint16_t *)(x)))
#define	__REG32(x)		(*((volatile uint32_t *)(x)))

/*
 * SDIO/MMC Memory-mapped registers
 */
#define	SDHCI_DMAADR(base)		__REG32(base + 0x00)
#define	SDHCI_BLKSZ(base)		__REG16(base + 0x04)
#define	SDHCI_BLKCNT(base)		__REG16(base + 0x06)
#define	SDHCI_CMDARG(base)		__REG32(base + 0x08)
#define	SDHCI_XFRMODE(base)		__REG16(base + 0x0C)
#define	SDHCI_SDCMD(base)		__REG16(base + 0x0E)
#define	SDHCI_RESP0(base)		__REG32(base + 0x10)
#define	SDHCI_RESP1(base)		__REG32(base + 0x14)
#define	SDHCI_RESP2(base)		__REG32(base + 0x18)
#define	SDHCI_RESP3(base)		__REG32(base + 0x1C)
#define	SDHCI_BUFDATA(base)		__REG32(base + 0x20)
#define	SDHCI_PSTATE(base)		__REG32(base + 0x24)
#define	SDHCI_HOSTCTL(base)		__REG8 (base + 0x28)
#define	SDHCI_HOSTCTL2(base)	__REG16(base + 0x3e)
#define	SDHCI_PWRCTL(base)		__REG8 (base + 0x29)
#define	SDHCI_BLKGAPCTL(base)	__REG8 (base + 0x2A)
#define	SDHCI_WAKECTL(base)		__REG8 (base + 0x2B)
#define	SDHCI_CLKCTL(base)		__REG16(base + 0x2C)
#define	SDHCI_TOCTL(base)		__REG8 (base + 0x2E)
#define	SDHCI_SWRST(base)		__REG8 (base + 0x2F)
#define	SDHCI_NINTSTS(base)		__REG16(base + 0x30)
#define	SDHCI_ERINTSTS(base)	__REG16(base + 0x32)
#define	SDHCI_NINTEN(base)		__REG16(base + 0x34)
#define	SDHCI_ERINTEN(base)		__REG16(base + 0x36)
#define	SDHCI_NINTSIGEN(base)	__REG16(base + 0x38)
#define	SDHCI_ERINTSIGEN(base)	__REG16(base + 0x3A)
#define	SDHCI_AC12ERRSTS(base)	__REG16(base + 0x3C)
#define	SDHCI_CAP(base)			__REG32(base + 0x40)
#define	SDHCI_CAP2(base)		__REG32(base + 0x44)
#define	SDHCI_MCCAP(base)		__REG32(base + 0x48)
#define SDHCI_ADMA_ESTATE(base) __REG8(base + 0x54)
#define SDHCI_ADMA_ADDRL(base)  __REG32(base + 0x58)
#define SDHCI_ADMA_ADDRH(base)  __REG32(base + 0x5C)
#define	SDHCI_SLTINTSTS(base)	__REG16(base + 0xFC)
#define	SDHCI_CTRLRVER(base)	__REG16(base + 0xFE)
	#define SDHCI_SD_VER_1		0
	#define SDHCI_SD_VER_2		1
	#define SDHCI_SD_VER_3		2

/*
 * Transfer Mode Register (XFRMODE) bit defination
 */
#define	SDHCI_XFRMODE_DMAEN		(1 << 0)	// DMA enable
#define	SDHCI_XFRMODE_BCE		(1 << 1)	// Block Count Enable
#define	SDHCI_XFRMODE_AC12EN	(1 << 2)	// Auto CMD12 Enable
#define	SDHCI_XFRMODE_DATDIR	(1 << 4)	// Data Direction
#define	SDHCI_XFRMODE_MBS		(1 << 5)	// Multiple Block Select

/*
 * Command Register (CMD) bit defination
 */
#define	SDHCI_CMD_RSPLEN0		(0 << 0)	// No response
#define	SDHCI_CMD_RSPLEN136		(1 << 0)	// 136 bit response
#define	SDHCI_CMD_RSPLEN48		(2 << 0)	// 48 bit response
#define	SDHCI_CMD_RSPLEN48b		(3 << 0)	// 48 bit response with busy bit check
#define	SDHCI_CMD_CCCE			(1 << 3)	// Command CRC Check enable
#define	SDHCI_CMD_CICE			(1 << 4)	// Command Index Check enable
#define	SDHCI_CMD_DPS			(1 << 5)	// Data Present
#define	SDHCI_CMD_NORMAL		(0 << 6)	// Normal Command
#define	SDHCI_CMD_CMDIDX(x)		(((x) & 0x3F) << 8)


/*
 * Present State Register (PSTATE) bit defination
 */
#define	SDHCI_PSTATE_WP			(1 << 19)
#define	SDHCI_PSTATE_CD			(1 << 18)
#define	SDHCI_PSTATE_CSS		(1 << 17)
#define	SDHCI_PSTATE_CI			(1 << 16)
#define	SDHCI_PSTATE_BRE		(1 << 11)
#define	SDHCI_PSTATE_BUFWREN	(1 << 10)
#define	SDHCI_PSTATE_RTA		(1 <<  9)
#define	SDHCI_PSTATE_WTA		(1 <<  8)
#define	SDHCI_PSTATE_DLA		(1 <<  2)
#define	SDHCI_PSTATE_DCI		(1 <<  1)
#define	SDHCI_PSTATE_CINH		(1 <<  0)
#define	SDHCI_CARD_STABLE		(SDHCI_PSTATE_CD | SDHCI_PSTATE_CSS | SDHCI_PSTATE_CI)

/*
 * Normal Interrupt status/enable/signal enable bit defination
 */
#define	SDHCI_NINT_CC			(1 << 0)	// Command Complete
#define	SDHCI_NINT_TC			(1 << 1)	// Transfer Complete
#define	SDHCI_NINT_BGE			(1 << 2)	// Block Gap Event
#define	SDHCI_NINT_DMA			(1 << 3)	// DMA Interrupt
#define	SDHCI_NINT_BWR			(1 << 4)	// Buffer Write Ready
#define	SDHCI_NINT_BRR			(1 << 5)	// Buffer Read Ready
#define	SDHCI_NINT_CIN			(1 << 6)	// Card Insertion
#define	SDHCI_NINT_CRM			(1 << 7)	// Card Removal
#define	SDHCI_NINT_CI			(1 << 8)	// Card Interrupt
#define	SDHCI_NINT_EI			(1 << 15)	// Error Interrupt, for status register only

/*
 * Error Interrupt status/enable/signal enable bit defination
 */
#define	SDHCI_ERINT_CTE			(1 << 0)	// Command Timeout Error
#define	SDHCI_ERINT_CCE			(1 << 1)	// Command CRC Error
#define	SDHCI_ERINT_CEBE		(1 << 2)	// Command End Bit Error
#define	SDHCI_ERINT_CIE			(1 << 3)	// Command Index Error
#define	SDHCI_ERINT_DTE			(1 << 4)	// Data Timeout Error
#define	SDHCI_ERINT_DCE			(1 << 5)	// Data CRC Error
#define	SDHCI_ERINT_DEBE		(1 << 6)	// Data End Bit Error
#define	SDHCI_ERINT_CL			(1 << 7)	// Current Limit Error
#define	SDHCI_ERINT_AC12		(1 << 8)	// Auto CMD12 Error
#define	SDHCI_ERINT_ADMA		(1 << 9)	// ADMA Error

/*
 * Clock Control Register bit defination
 */
#define	SDHCI_CLKCTL_ICE		(1 << 0)	// Internal Clock Enable
#define	SDHCI_CLKCTL_ICS		(1 << 1)	// Internal Clock Stable
#define	SDHCI_CLKCTL_CLKEN		(1 << 2)	// Clock Enable

/*
 * Host Control Register bit defination
 */
#define	SDHCI_HOSTCTL_LEDCTL	(1 << 0)	// LED Control
#define	SDHCI_HOSTCTL_DTW1BIT	(0 << 1)	// Data Bus Width 1 bit
#define	SDHCI_HOSTCTL_DTW4BIT	(1 << 1)	// Data Bus Width 4 bit
#define	SDHCI_HOSTCTL_HSEN		(1 << 2)	// High Speed Enable
#define SDHCI_HOSTCTL_DMAMASK   (3 << 3)    // DMA select mask
#define SDHCI_HOSTCTL_SDMA      (0 << 3)    // SDMA select
#define SDHCI_HOSTCTL_ADMA32    (2 << 3)    // 32 bit ADMA select
#define SDHCI_HOSTCTL_ADMA64    (3 << 3)    // 64 bit ADMA select
#define	SDHCI_HOSTCTL_MMC8		(1 << 3)	// Data Bus Width 8 bit (Intel)
#define SDHCI_HOSTCTL_BW8		(1 << 5)	// 8 bit bus select

/*
 * Host Control 2 Register bit defination
 */
#define SDHCI_HOSTCTL2_SIG_1_8V		(1 << 3)
#define SDHCI_HOSTCTL2_UHS_DDR50	4
#define SDHCI_HOSTCTL2_UHS_SDR104	3
#define SDHCI_HOSTCTL2_UHS_SDR50	2
#define SDHCI_HOSTCTL2_UHS_SDR25	1
#define SDHCI_HOSTCTL2_UHS_MSK		0x07

/*
 * Power Control Register bit defination
 */
#define	SDHCI_PWRCTL_PWREN		(1 << 0)	// SD Bus Power Enable
#define	SDHCI_PWRCTL_V33		(7 << 1)	// 3.3V
#define SDHCI_PWRCTL_V30		(6 << 1)	// 3.0V
#define SDHCI_PWRCTL_V18		(5 << 1)	// 1.8V

/*
 * Software Reset Register bit defination
 */
#define	SDHCI_SWRST_ALL			(1 << 0)	// Reset All
#define	SDHCI_SWRST_RC			(1 << 1)	// Reset CMD
#define	SDHCI_SWRST_RD			(1 << 2)	// Reset Data

/*
 * Capability Registers bits 0 to 31
 */
#define	SDHCI_CAP_S18			(1 << 26)	// 1.8V support
#define	SDHCI_CAP_S30			(1 << 25)	// 3.0V support
#define	SDHCI_CAP_S33			(1 << 24)	// 3.3V support
#define	SDHCI_CAP_SRS			(1 << 23)	// Suspend/Resume support
#define	SDHCI_CAP_DMA			(1 << 22)	// DMA support
#define	SDHCI_CAP_HS			(1 << 21)	// High-Speed support
#define	SDHCI_CAP_ADMA1			(1 << 20)	// ADMA1 support
#define	SDHCI_CAP_ADMA2			(1 << 19)	// ADMA2 support
#define	SDHCI_CAP_EXTMBUS		(1 << 18)	// Extended media bus support
#define	SDHCI_CAP_MBL512		(0 << 16)	// Max block length 512
#define	SDHCI_CAP_MBL2048		(2 << 16)	// Max block length 2048

// bits 32 to 64
#define SDHCI_CAP_DRIVE_TYPE_D	(1 << 6)
#define SDHCI_CAP_DRIVE_TYPE_C	(1 << 5)
#define SDHCI_CAP_DRIVE_TYPE_A	(1 << 4)
#define SDHCI_CAP_DDR50			(1 << 2)
#define SDHCI_CAP_SDR104		(1 << 1)
#define SDHCI_CAP_SDR50			(1 << 0)

#ifdef SDHCI_ADMA_SUP 
// 32 bit ADMA descriptor defination
typedef struct _sdhci_adma32_t {
	uint16_t	attr;
	uint16_t	len;
	uint32_t	addr;
} sdhci_adma32_t;
#endif

#define SDHCI_ADMA2_VALID	(1 << 0)	// valid
#define SDHCI_ADMA2_END		(1 << 1)	// end of descriptor, transfer complete interrupt will be generated
#define SDHCI_ADMA2_INT		(1 << 2)	// generate DMA interrupt, will not be used
#define SDHCI_ADMA2_NOP		(0 << 4)	// no OP, go to the next desctiptor
#define SDHCI_ADMA2_TRAN	(2 << 4)	// transfer data
#define SDHCI_ADMA2_LINK	(3 << 4) 	// link to another descriptor

typedef	struct _sdhci_ext {
	void		*hba;
	void		*bshdl;
	int			pci_hdl;
	void		*pci_dev_hdl;
	uintptr_t	base;
	uint32_t	cap;
	uint32_t	clock;
	int			blksz;
	int 		pio_enable;
	int			pwrup_delay;
	uint8_t 	*tbuf;
	paddr_t		tbuf_phy;
#define	SDHCI_DMABUF_LEN	1024
} sdhci_ext_t;

extern int sdhci_init(SIM_HBA *hba);

#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_sdhci.h $ $Rev: 657836 $" )
