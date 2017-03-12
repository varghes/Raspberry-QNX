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


#ifndef	_DRA446_MMC_INCLUDED
#define	_DRA446_MMC_INCLUDED


#define	DRA446_MMCCTL		0x00	// MMC Control Register
	#define	MMCCTL_DATRST	(1 << 0)
	#define	MMCCTL_CMDRST	(1 << 1)
	#define	MMCCTL_WIDTH4	(1 << 2)
	#define	MMCCTL_WIDTH1	(0 << 2)
#define	DRA446_MMCCLK		0x04	// MMC Memory Clock Control Register
	#define	MMCCLK_CLKEN	(1 << 8)
#define	DRA446_MMCST0		0x08	// MMC Status Register 0
	#define	MMCST0_DATDNE	(1 << 0)
	#define	MMCST0_BSYDNE	(1 << 1)
	#define	MMCST0_RSPDNE	(1 << 2)
	#define	MMCST0_TOUTRD	(1 << 3)
	#define	MMCST0_TOUTRS	(1 << 4)
	#define	MMCST0_CRCWR	(1 << 5)
	#define	MMCST0_CRCRD	(1 << 6)
	#define	MMCST0_CRCRS	(1 << 7)
	#define	MMCST0_DXRDY	(1 << 9)
	#define	MMCST0_DRRDY	(1 << 10)
	#define	MMCST0_DATED	(1 << 11)
	#define	MMCST0_TRNDNE	(1 << 12)
	#define	DRA446_ST0_ERROR	(MMCST0_TOUTRD | MMCST0_TOUTRS | MMCST0_CRCWR | MMCST0_CRCRD | MMCST0_CRCRS)
#define	DRA446_MMCST1		0x0C	// MMC Status Register 1
	#define	MMCST1_BUSY		(1 << 0)
	#define	MMCST1_CLKSTP	(1 << 1)
	#define	MMCST1_DXEMP	(1 << 2)
	#define	MMCST1_DRFUL	(1 << 3)
	#define	MMCST1_DAT3ST	(1 << 4)
	#define	MMCST1_FIFOEMP	(1 << 5)
	#define	MMCST1_FIFOFUL	(1 << 6)
#define	DRA446_MMCIM		0x10	// MMC Interrupt Mask Register
	#define	MMCIM_EDATDNE	(1 << 0)
	#define	MMCIM_EBSYDNE	(1 << 1)
	#define	MMCIM_ERSPDNE	(1 << 2)
	#define	MMCIM_ETOUTRD	(1 << 3)
	#define	MMCIM_ETOUTRS	(1 << 4)
	#define	MMCIM_ECRCWR	(1 << 5)
	#define	MMCIM_ECRCRD	(1 << 6)
	#define	MMCIM_ECRCRS	(1 << 7)
	#define	MMCIM_EDXRDY	(1 << 9)
	#define	MMCIM_EDRRDY	(1 << 10)
	#define	MMCIM_EDATED	(1 << 11)
	#define	MMCIM_ETRNDNE	(1 << 12)
#define	DRA446_MMCTOR		0x14	// MMC Response Time-Out Register
#define	DRA446_MMCTOD		0x18	// MMC Data Read Time-Out Register
#define	DRA446_MMCBLEN		0x1C	// MMC Block Length Register
#define	DRA446_MMCNBLK		0x20	// MMC Number of Blocks Register
#define	DRA446_MMCNBLC		0x24	// MMC Number of Blocks Counter Register
#define	DRA446_MMCDRR		0x28	// MMC Data Receive Register
#define	DRA446_MMCDXR		0x2C	// MMC Data Transmit Register
#define	DRA446_MMCCMD		0x30	// MMC Command Register
	#define	MMCCMD_PPLEN	(1 << 7)
	#define	MMCCMD_BSYEXP	(1 << 8)
	#define	MMCCMD_RSPFMT_NONE	(0 << 9)
	#define	MMCCMD_RSPFMT_R1	(1 << 9)
	#define	MMCCMD_RSPFMT_R2	(2 << 9)
	#define	MMCCMD_RSPFMT_R3	(3 << 9)
	#define	MMCCMD_DTRW		(1 << 11)
	#define	MMCCMD_STRMTP	(1 << 12)
	#define	MMCCMD_WDATX	(1 << 13)
	#define	MMCCMD_INITCK	(1 << 14)
	#define	MMCCMD_DCLR		(1 << 15)
	#define	MMCCMD_DMATRIG	(1 << 16)
#define	DRA446_MMCARGHL		0x34	// MMC Argument Register
#define	DRA446_MMCRSP01		0x38	// MMC Response Register 0 and 1
#define	DRA446_MMCRSP23		0x3C	// MMC Response Register 2 and 3
#define	DRA446_MMCRSP45		0x40	// MMC Response Register 4 and 5
#define	DRA446_MMCRSP67		0x44	// MMC Response Register 6 and 7
#define	DRA446_MMCDRSP		0x48	// MMC Data Response Register
#define	DRA446_MMCCIDX		0x50	// MMC Command Index Register
#define	DRA446_SDIOCTL		0x64	// SDIO Control Register
#define	DRA446_SDIOST0		0x68	// SDIO Status Register 0
#define	DRA446_SDIOIEN		0x6C	// SDIO Interrupt Enable Register
#define	DRA446_SDIOIST		0x70	// SDIO Interrupt Status Register
#define	DRA446_MMCFIFOCTL	0x74	// MMC FIFO Control Register
	#define	MMCFIFOCTL_ACCWD4		(0 << 3)
	#define	MMCFIFOCTL_ACCWD1		(3 << 3)
	#define	MMCFIFOCTL_FIFO_128		(0 << 2)
	#define	MMCFIFOCTL_FIFO_256		(1 << 2)
	#define	MMCFIFOCTL_FIFO_READ	(0 << 1)
	#define	MMCFIFOCTL_FIFO_WRITE	(1 << 1)
	#define	MMCFIFOCTL_FIFO_RESET	(1 << 0)

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


#define	DRA446_SDC_BASE		0x01C50000
#define	DRA446_SDC_SIZE		0x100
#define	DRA446_SDC_IRQ		16
#define	DRA446_EDMA_BASE	0x01C00000
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

typedef	struct _dra446_mmc {
	unsigned		mmc_pbase;
	uint32_t		mmc_clock;

	uintptr_t		mmc_base;
	uintptr_t		dma_base;
	int				chrdma;
	int				chtdma;
	int				blksz;

	int				piomask;

	void			*hba;

	int				bshdl;
} dra446_ext_t;


extern int	dra446_attach(SIM_HBA *hba);

#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_dra446.h $ $Rev: 657836 $" )
