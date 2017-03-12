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


#ifndef	_OMAP3_MMC_INCLUDED
#define	_OMAP3_MMC_INCLUDED

//
// MMCHS base address, irq
//
#define	OMAP3_MMCHS1_BASE		0x4809C000
#define	OMAP3_MMCHS2_BASE		0x480B4000
#define	OMAP3_MMCHS3_BASE		0x480AD000
#define	OMAP3_MMCHS_SIZE		512
#define	OMAP3_MMCHS1_IRQ		83
#define	OMAP3_MMCHS2_IRQ		86
#define	OMAP3_MMCHS3_IRQ		94

#define	MMCHS_FIFO_SIZE			512

// Register Descriptions
#define	OMAP3_MMCHS_SYSCONFIG	0x010	// System Configuration Register
	#define	SYSCONFIG_AUTOIDLE		(1 << 0)	// Clock auto idle
	#define	SYSCONFIG_SOFTRESET		(1 << 1)	// Software reset

#define	OMAP3_MMCHS_SYSSTATUS	0x014	// System Status Register
	#define	SYSSTATUS_RESETDONE		(1 << 0)	// Reset done

#define	OMAP3_MMCHS_CSRE		0x024	// Card status response error
#define	OMAP3_MMCHS_SYSTEST		0x028	// System Test Register
#define	OMAP3_MMCHS_CON			0x02C	// Configuration register
	#define	CON_OD					(1 <<  0)	// Card Open Drain Mode
	#define	CON_INIT				(1 <<  1)	// Send initialization stream
	#define	CON_DW8				(1 <<  5)	// MMC 8-bit width
	#define	CON_PADEN				(1 << 15)	// Control Power for MMC Lines
	#define	CON_CLKEXTFREE			(1 << 16)	// External clock free running

#define	OMAP3_MMCHS_PWCNT		0x030	// Power counter register
#define	OMAP3_MMCHS_BLK			0x104	// Transfer Length Configuration register
#define	OMAP3_MMCHS_ARG			0x108	// Command argument Register
#define	OMAP3_MMCHS_CMD			0x10C	// Command and transfer mode register
	#define	CMD_DE					(1 <<  0)	// DMA Enable
	#define	CMD_BCE					(1 <<  1)	// Block Count Enable
	#define	CMD_ACEN				(1 <<  2)	// Auto CMD12 Enable
	#define	CMD_DDIR				(1 <<  4)	// Data Transfer Direction Read
	#define	CMD_MBS					(1 <<  5)	// Multi Block Select
	#define	CMD_RSP_TYPE_136		(1 << 16)	// Response length 136 bit
	#define	CMD_RSP_TYPE_48			(2 << 16)	// Response length 48 bit
	#define	CMD_RSP_TYPE_48b		(3 << 16)	// Response length 48 bit with busy after response
	#define	CMD_CCCE				(1 << 19)	// Comamnd CRC Check Enable
	#define	CMD_CICE				(1 << 20)	// Comamnd Index Check Enable
	#define	CMD_DP					(1 << 21)	// Data Present
	#define	CMD_TYPE_CMD12			(3 << 22)	// CMD12 or CMD52 "I/O Abort"

#define	OMAP3_MMCHS_RSP10		0x110	// Command response[31:0] Register
#define	OMAP3_MMCHS_RSP32		0x114	// Command response[63:32] Register
#define	OMAP3_MMCHS_RSP54		0x118	// Command response[95:64] Register
#define	OMAP3_MMCHS_RSP76		0x11C	// Command response[127:96] Register
#define	OMAP3_MMCHS_DATA		0x120	// Data Register
#define	OMAP3_MMCHS_PSTATE		0x124	// Present state register
	#define	PSTATE_CMDI				(1 <<  0)	// Command inhibit (mmci_cmd).
	#define	PSTATE_DATI				(1 <<  1)	// Command inhibit (mmci_dat).
	#define	PSTATE_DLA				(1 <<  2)	// mmci_dat line active
	#define	PSTATE_WTA				(1 <<  8)	// Write Transfer Active
	#define	PSTATE_RTA				(1 <<  9)	// Read Transfer Active
	#define	PSTATE_BWE				(1 << 10)	// Buffer Write Ready
	#define	PSTATE_BRE				(1 << 11)	// Buffer Read Ready
	#define PSTATE_CINS				(1<<16) //card insertion
	#define PSTATE_WP				(1<<19) //card write protection

#define	OMAP3_MMCHS_HCTL			0x128	// Host Control register
	#define	HCTL_DTW4				(1 << 1)	// Data transfer width 4 bit
	#define HCTL_CDTL				(1 << 6)	// Card Detection Test Level
	#define	HCTL_CDSS				(1 << 7)	// Card Detection Signal Selection
	#define	HCTL_SDBP				(1 << 8)	// SD bus power
	#define	HCTL_SDVS1V8			(5 << 9)	// SD bus voltage 1.8V
	#define	HCTL_SDVS3V0			(6 << 9)	// SD bus voltage 3.0V
	#define	HCTL_SDVS3V3			(7 << 9)	// SD bus voltage 3.3V

#define	OMAP3_MMCHS_SYSCTL		0x12C	// SD system control register
	#define	SYSCTL_ICE				(1 << 0)	// Internal Clock Enable
	#define	SYSCTL_ICS				(1 << 1)	// Internal Clock Stable
	#define	SYSCTL_CEN				(1 << 2)	// Clock Enable
	#define	SYSCTL_DTO_MASK			(0xF << 16)	// Data timeout counter value and busy timeout
	#define	SYSCTL_DTO_MAX			(0xE << 16)	// Timeout = TCF x 2^27
	#define	SYSCTL_SRA				(1 << 24)	// Software reset for all
	#define	SYSCTL_SRC				(1 << 25)	// Software reset for mmci_cmd line
	#define	SYSCTL_SRD				(1 << 26)	// Software reset for mmci_dat line

#define	OMAP3_MMCHS_STAT		0x130	// Interrupt status register
#define	OMAP3_MMCHS_IE			0x134	// Interrupt SD enable register
#define	OMAP3_MMCHS_ISE			0x138	// Interrupt signal enable register
	#define	INTR_CC					(1 <<  0)	// Command Complete
	#define	INTR_TC					(1 <<  1)	// Transfer Complete
	#define	INTR_BGE				(1 <<  2)	// Block Gap Event
	#define	INTR_BWR				(1 <<  4)	// Buffer Write Ready interrupt
	#define	INTR_BRR				(1 <<  5)	// Buffer Read Ready interrupt
	#define	INTR_CIRQ				(1 <<  8)	// Card interrupt
	#define	INTR_OBI				(1 <<  9)	// Out-Of-Band interrupt
	#define	INTR_ERRI				(1 << 15)	// Error interrupt
	#define	INTR_CTO				(1 << 16)	// Command Timeout error
	#define	INTR_CCRC				(1 << 17)	// Command CRC error
	#define	INTR_CEB				(1 << 18)	// Command End Bit error
	#define	INTR_CIE				(1 << 19)	// Command Index error
	#define	INTR_DTO				(1 << 20)	// Data Timeout error
	#define	INTR_DCRC				(1 << 21)	// Data CRC error
	#define	INTR_DEB				(1 << 22)	// Data End Bit error
	#define	INTR_ACE				(1 << 24)	// ACMD12 error
	#define	INTR_CERR				(1 << 28)	// Card error
	#define	INTR_BADA				(1 << 29)	// Bad Access to Data Space

#define	OMAP3_MMCHS_AC12			0x13C	// Auto CMD12 Error Status Register
#define	OMAP3_MMCHS_CAPA			0x140	// Capabilities register
	#define	CAPA_VS3V3				(1 << 24)	// 3.3V
	#define	CAPA_VS3V0				(1 << 25)	// 3.0V
	#define	CAPA_VS1V8				(1 << 26)	// 1.8V

#define	OMAP3_MMCHS_CUR_CAPA		0x148	// Maximum current capabilities Register
#define	OMAP3_MMCHS_REV				0x1FC	// Versions Register

#ifdef USE_EDMA

typedef struct {
	volatile uint32_t	opt;
	volatile uint32_t	src;
	volatile uint32_t	abcnt;
	volatile uint32_t	dst;
	volatile uint32_t	srcdstbidx;
	volatile uint32_t	linkbcntrld;
	volatile uint32_t	srcdstcidx;
	volatile uint32_t	ccnt;
} dra446_param;


#define	DRA446_EDMA_BASE	0x49000000
#define	DRA446_EDMA_SIZE	0x5000

#define	DRA446_SHADOW0_OFF	0x2000
#define	DRA446_PARAM_OFF	0x4000
#define	DRA446_PARAM_SIZE	0x20
#define	DRA446_PARAM(ch)	(DRA446_PARAM_OFF + (DRA446_PARAM_SIZE * ch))

/*
 * EDMA registers, offset from EDMA register base
 */
#define	DRA446_EDMA_ER		0x0000		/* ER Event Register */
#define	DRA446_EDMA_ERH		0x0004		/* ER Event Register High */
#define	DRA446_EDMA_ECR		0x0008		/* ER Event Clear Register */
#define	DRA446_EDMA_ECRH	0x000C		/* ER Event Clear Register High */
#define	DRA446_EDMA_ESR		0x0010		/* ER Event Set Register */
#define	DRA446_EDMA_ESRH	0x0014		/* ER Event Set Register High */
#define	DRA446_EDMA_EER		0x0020		/* ER Event Enable Register */
#define	DRA446_EDMA_EERH	0x0024		/* ER Event Enable Register High */
#define	DRA446_EDMA_EECR	0x0028		/* ER Event Enable Clear Register */
#define	DRA446_EDMA_EECRH	0x002C		/* ER Event Enable Clear Register High */
#define	DRA446_EDMA_EESR	0x0030		/* ER Event Enable Set Register */
#define	DRA446_EDMA_EESRH	0x0034		/* ER Event Enable Set Register High */
#else
//
// SDMA registers
// Be better to move to omap3530.h
//

#define	OMAP3_DMA4_BASE			0x48056000
#define	OMAP3_DMA4_SIZE			0x1000

#define	DMA4_IRQSTATUS(j)		(0x08 + (j) * 4)	// j = 0 - 3
#define	DMA4_IRQENABLE(j)		(0x18 + (j) * 4)	// j = 0 - 3
#define	DMA4_SYSSTATUS			(0x28)
#define	DMA4_OCP_SYSCONFIG		(0x2C)
#define	DMA4_CAPS_0				(0x64)
#define	DMA4_CAPS_2				(0x6C)
#define	DMA4_CAPS_3				(0x70)
#define	DMA4_CAPS_4				(0x74)
#define	DMA4_GCR				(0x78)
#define	DMA4_CCR(i)				(0x80 + (i) * 0x60)	// i = 0 - 31
#define	DMA4_CLNK_CTRL(i)		(0x84 + (i) * 0x60)
#define	DMA4_CICR(i)			(0x88 + (i) * 0x60)
#define	DMA4_CSR(i)				(0x8C + (i) * 0x60)
#define	DMA4_CSDP(i)			(0x90 + (i) * 0x60)
#define	DMA4_CEN(i)				(0x94 + (i) * 0x60)
#define	DMA4_CFN(i)				(0x98 + (i) * 0x60)
#define	DMA4_CSSA(i)			(0x9C + (i) * 0x60)
#define	DMA4_CDSA(i)			(0xA0 + (i) * 0x60)
#define	DMA4_CSE(i)				(0xA4 + (i) * 0x60)
#define	DMA4_CSF(i)				(0xA8 + (i) * 0x60)
#define	DMA4_CDE(i)				(0xAC + (i) * 0x60)
#define	DMA4_CDF(i)				(0xB0 + (i) * 0x60)
#define	DMA4_CSAC(i)			(0xB4 + (i) * 0x60)
#define	DMA4_CDAC(i)			(0xB8 + (i) * 0x60)
#define	DMA4_CCEN(i)			(0xBC + (i) * 0x60)
#define	DMA4_CCFN(i)			(0xC0 + (i) * 0x60)
#define	DMA4_COLOR(i)			(0xC4 + (i) * 0x60)

#define	DMA4_MMC1_TX		61
#define	DMA4_MMC1_RX		62
#define	DMA4_MMC2_TX		47
#define	DMA4_MMC2_RX		48
#define	DMA4_MMC3_TX		77
#define	DMA4_MMC3_RX		78

#define	DMA4_CCR_SYNCHRO_CONTROL(s)		(((s) & 0x1F) | (((s) >> 5) << 19))

typedef struct {
	volatile uint32_t	ccr;		// 0x00
	volatile uint32_t	clnk_ctrl;	// 0x04
	volatile uint32_t	cicr;		// 0x08
	volatile uint32_t	csr;		// 0x0C
	volatile uint32_t	csdp;		// 0x10
	volatile uint32_t	cen;		// 0x14
	volatile uint32_t	cfn;		// 0x18
	volatile uint32_t	cssa;		// 0x1C
	volatile uint32_t	cdsa;		// 0x20
	volatile uint32_t	cse;		// 0x24
	volatile uint32_t	csf;		// 0x28
	volatile uint32_t	cde;		// 0x2C
	volatile uint32_t	cdf;		// 0x30
	volatile uint32_t	csac;		// 0x34
	volatile uint32_t	cdac;		// 0x38
	volatile uint32_t	ccen;		// 0x3C
	volatile uint32_t	ccfn;		// 0x40
	volatile uint32_t	color;		// 0x44
} dma4_param;
#endif


typedef	struct _omap3_mmc {
	unsigned		mmc_pbase;
	uint32_t		mmc_clock;

	uintptr_t		mmc_base;

	int				mmc;

	// Flags for data transfer
	uint32_t		dcmd;
	uint32_t		dmask;
	int				dsize;

	uintptr_t		dma_base;
	int				dma_chnl;
	int				dma_rreq;	// DMA request line for Rx
	int				dma_treq;	// DMA request line for Tx

	int				blksz;

	void			*hba;
	void			*bshdl;
} omap3_ext_t;


extern int	omap3_attach(SIM_HBA *hba);

#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_omap3.h $ $Rev: 657836 $" )
