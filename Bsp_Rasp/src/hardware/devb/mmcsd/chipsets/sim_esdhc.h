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

#ifndef	_ESDHC_INCLUDED
#define	_ESDHC_INCLUDED

#define ESDHC_MAXIO							1

// CCB clock should be pulled from syspage
#define MPC8536_CCB_CLOCK					500000000	// complex bus AKA platform clock

#define MPC8536_ESDHC_BASE					0x2e000
#define MPC8536_ESDHC_LEN					0x1000

#define ESDHC_PRSSTAT_CMD_TIMEOUT			1000000
#define ESDHC_PRSSTAT_DATA_TIMEOUT			200000
#define ESDHC_PRSSTAT_DETECT_TIMEOUT		200000
#define ESDHC_DATPORT_DELAY					1000		// nanospin delay between accesses

// Registers offsets

#define ESDHC_DSADDR				0x00	// DMA system address

#define ESDHC_BLKATTR				0x04	// Block attributes
	#define ESDHC_BLKATTR_BLKSIZE_MASK		0x00000fff
	#define ESDHC_BLKATTR_BLKCNT_MASK		0xffff0000
	#define ESDHC_BLKATTR_BLKCNT_SHIFT		16

#define ESDHC_CMDARG				0x08	// Command argument

#define ESDHC_XFERTYP				0x0C	// Command transfer type
	#define ESDHC_XFERTYP_CMDINX_MASK		0x3f000000
	#define ESDHC_XFERTYP_CMDINX(_c)		( (_c) << 24 )
	#define ESDHC_XFERTYP_CMDTYP_MASK		0x00c00000
	#define ESDHC_XFERTYP_CMDTYP_ABORT		0x00c00000
	#define ESDHC_XFERTYP_CMDTYP_RESUME 	0x00800000
	#define ESDHC_XFERTYP_CMDTYP_SUSPEND	0x00400000
	#define ESDHC_XFERTYP_CMDTYP_NORMAL		0x00000000
	#define ESDHC_XFERTYP_DPSEL				0x00200000
	#define ESDHC_XFERTYP_CIEN				0x00100000
	#define ESDHC_XFERTYP_CCCEN				0x00080000
	#define ESDHC_XFERTYP_RSPTYP_MASK		0x00070000
	#define ESDHC_XFERTYP_RSPLEN48BSY		0x00030000
	#define ESDHC_XFERTYP_RSPLEN48			0x00020000
	#define ESDHC_XFERTYP_RSPLEN136			0x00010000
	#define ESDHC_XFERTYP_MSBSEL			0x00000020
	#define ESDHC_XFERTYP_DTD_RD			0x00000010
	#define ESDHC_XFERTYP_AC12EN			0x00000004
	#define ESDHC_XFERTYP_BCEN				0x00000002
	#define ESDHC_XFERTYP_DMAEN				0x00000001

#define ESDHC_CMDRSP0				0x10	// Command response 0
#define ESDHC_CMDRSP1				0x14	// Command response 1
#define ESDHC_CMDRSP2				0x18	// Command response 2
#define ESDHC_CMDRSP3				0x1c	// Command response 3

#define ESDHC_DATPORT				0x20	// Data buffer access port

#define ESDHC_PRSSTAT				0x24	// Present state
	#define ESDHC_PRSSTAT_DSL_MASK		0xff000000
	#define ESDHC_PRSSTAT_DSL0			0x01000000
	#define ESDHC_PRSSTAT_CLSL			0x00800000
	#define ESDHC_PRSSTAT_WPSPL			0x00080000
	#define ESDHC_PRSSTAT_CDPL			0x00040000
	#define ESDHC_PRSSTAT_CINS			0x00010000
	#define ESDHC_PRSSTAT_BREN			0x00000800
	#define ESDHC_PRSSTAT_BWEN			0x00000400
	#define ESDHC_PRSSTAT_RTA			0x00000200
	#define ESDHC_PRSSTAT_WTA			0x00000100
	#define ESDHC_PRSSTAT_SDOFF			0x00000080
	#define ESDHC_PRSSTAT_PEROFF		0x00000040
	#define ESDHC_PRSSTAT_HCKOFF		0x00000020
	#define ESDHC_PRSSTAT_IPGOFF		0x00000010
	#define ESDHC_PRSSTAT_DLA			0x00000004
	#define ESDHC_PRSSTAT_CDIHB			0x00000002
	#define ESDHC_PRSSTAT_CIHB			0x00000001

#define ESDHC_PROCTL				0x28	// Protocol control
	#define ESDHC_PROCTL_WECRM			0x04000000
	#define ESDHC_PROCTL_WECINS			0x02000000
	#define ESDHC_PROCTL_WECINT			0x01000000
	#define ESDHC_PROCTL_IABG			0x00080000
	#define ESDHC_PROCTL_RWCTL			0x00040000
	#define ESDHC_PROCTL_CREQ			0x00020000
	#define ESDHC_PROCTL_SABGREQ		0x00010000
	#define ESDHC_PROCTL_CDSS_EN		0x00000080
	#define ESDHC_PROCTL_CDTL			0x00000040
	#define ESDHC_PROCTL_EMODE_MASK		0x00000030
	#define ESDHC_PROCTL_EMODE_AI		0x00000020
	#define ESDHC_PROCTL_D3CD			0x00000008
	#define ESDHC_PROCTL_DTW_MASK		0x00000006
	#define ESDHC_PROCTL_DTW_8			0x00000004
	#define ESDHC_PROCTL_DTW_4			0x00000002
	#define ESDHC_PROCTL_DTW_1			0x00000000

#define ESDHC_SYSCTL				0x2C	// System Control
	#define ESDHC_SYSCTL_INITA			0x08000000
	#define ESDHC_SYSCTL_RSTD			0x04000000
	#define ESDHC_SYSCTL_RSTC			0x02000000
	#define ESDHC_SYSCTL_RSTA			0x01000000
	#define ESDHC_SYSCTL_DTOCV_MAX		0x000e0000
	#define ESDHC_SYSCTL_DTOCV_MASK		0x000f0000
	#define ESDHC_SYSCTL_DTOCV_SHIFT	16
	#define ESDHC_SYSCTL_SDCLKFS_MASK	0x0000ff00
	#define ESDHC_SYSCTL_SDCLKFS_SHIFT	8
	#define ESDHC_SYSCTL_DVS_MASK		0x000000f0
	#define ESDHC_SYSCTL_DVS_SHIFT		4
	#define ESDHC_SYSCTL_PEREN			0x00000004
	#define ESDHC_SYSCTL_HCKEN			0x00000002
	#define ESDHC_SYSCTL_IPGEN			0x00000001

#define ESDHC_IRQSTAT				0x30	// Interrupt status
#define ESDHC_IRQSTATEN				0x34	// Interrupt status enable
#define ESDHC_IRQSIGEN				0x38	// Interrupt signal enable
	#define ESDHC_IRQ_CC				0x00000001
	#define ESDHC_IRQ_TC				0x00000002
	#define ESDHC_IRQ_BGE				0x00000004
	#define ESDHC_IRQ_DINT				0x00000008
	#define ESDHC_IRQ_BWR				0x00000010
	#define ESDHC_IRQ_BRR				0x00000020
	#define ESDHC_IRQ_CINS				0x00000040
	#define ESDHC_IRQ_CRM				0x00000080
	#define ESDHC_IRQ_CINT				0x00000100
	#define ESDHC_IRQ_CTOE				0x00010000
	#define ESDHC_IRQ_CCE				0x00020000
	#define ESDHC_IRQ_CEBE				0x00040000
	#define ESDHC_IRQ_CIE				0x00080000
	#define ESDHC_IRQ_DTOE				0x00100000
	#define ESDHC_IRQ_DCE				0x00200000
	#define ESDHC_IRQ_DEBE				0x00400000
	#define ESDHC_IRQ_AC12E				0x01000000
	#define ESDHC_IRQ_DMAE				0x10000000
	#define ESDHC_IRQ_ERROR_MASK		0x117F0000
	#define ESDHC_IRQ_MASK				0x117F013F

#define ESDHC_AUTOC12ERR			0x3C			// Auto CMD12 status
	#define ESDHC_AUTOC12ERR_CNIBAC12E	0x00000800
	#define ESDHC_AUTOC12ERR_AC12IE		0x00000010
	#define ESDHC_AUTOC12ERR_AC12CE		0x00000008
	#define ESDHC_AUTOC12ERR_AC12EBE	0x00000004
	#define ESDHC_AUTOC12ERR_AC12TOE	0x00000002
	#define ESDHC_AUTOC12ERR_AC12NE		0x00000001

#define ESDHC_HOSTCAPBLT			0x40			// Host controller capabilities
	#define ESDHC_HOSTCAPBLT_VS18		0x04000000
	#define ESDHC_HOSTCAPBLT_VS30		0x02000000
	#define ESDHC_HOSTCAPBLT_VS33		0x01000000
	#define ESDHC_HOSTCAPBLT_SRS		0x00800000
	#define ESDHC_HOSTCAPBLT_DMAS		0x00400000
	#define ESDHC_HOSTCAPBLT_HSS		0x00200000

#define ESDHC_WML					0x44			// Watermark level
	#define ESDHC_WML_WR_MASK			0x00ff0000
	#define ESDHC_WML_RD_MASK			0x000000ff
	#define ESDHC_WML_RD_SHIFT			0
	#define ESDHC_WML_WR_SHIFT			16

#define ESDHC_FEVT					0x50		// Force event

#define ESDHC_HOSTVER				0xFC		// Host controller version

#define ESDHC_SCR					0x40C
	#define ESDHC_SCR_SNOOP				0x00000040

// MUX Control
#define MPC_PMUXCR							0xe0060
	#define MPC_PMUXCR_SDHC_CD				( 1 << 30 )
	#define MPC_PMUXCR_SDHC_WP				( 1 << 29 )

typedef	struct _esdhc_ext {
	uintptr_t	base;
	uint32_t	blksz;
	uint32_t	xmode;
	uint32_t	clock;
} esdhc_ext_t;

extern int esdhc_init( SIM_HBA *hba );

#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_esdhc.h $ $Rev: 657836 $" )
