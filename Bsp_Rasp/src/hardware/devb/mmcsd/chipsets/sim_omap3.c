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


#include	<sim_mmc.h>

#ifdef MMCSD_VENDOR_TI_OMAP3

#include	<sim_omap3.h>
#include	<sys/syspage.h>
#include	<hw/sysinfo.h>


static int omap3_interrupt(SIM_HBA *hba, int irq, int resp_type, uint32_t *resp)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;
	uint32_t		sts;
	int				intr = 0;
	uintptr_t		base;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;
	base  = omap3->mmc_base;

	sts = in32(base + OMAP3_MMCHS_STAT);
	sts &= in32(base + OMAP3_MMCHS_IE) | INTR_ERRI;
	out32(base + OMAP3_MMCHS_STAT, sts);

	if (sts & INTR_ERRI) {	// Any errors ?
		slogf(_SLOGC_SIM_MMC, _SLOG_ERROR, "OMAP3 interrupt error = %x", sts);
		intr |= MMC_INTR_ERROR | MMC_INTR_COMMAND;
		if (sts & INTR_DTO) {
			intr |= MMC_ERR_DATA_TO;
			out32(base + OMAP3_MMCHS_SYSCTL, in32(base + OMAP3_MMCHS_SYSCTL) | SYSCTL_SRD);
			while (in32(base + OMAP3_MMCHS_SYSCTL) & SYSCTL_SRD);
		}
		if (sts & INTR_DCRC)
			intr |= MMC_ERR_DATA_CRC;
		if (sts & INTR_DEB)
			intr |= MMC_ERR_DATA_END;

		if (sts & INTR_CTO) {
			intr |= MMC_ERR_CMD_TO;
			out32(base + OMAP3_MMCHS_SYSCTL, in32(base + OMAP3_MMCHS_SYSCTL) | SYSCTL_SRC);
			while (in32(base + OMAP3_MMCHS_SYSCTL) & SYSCTL_SRC);
		}
		if (sts & INTR_CCRC)
			intr |= MMC_ERR_CMD_CRC;
		if (sts & INTR_CEB)
			intr |= MMC_ERR_CMD_END;
		if (sts & INTR_CIE)
			intr |= MMC_ERR_CMD_IDX;
	} else {
		if (sts & INTR_CC) {
			intr |= MMC_INTR_COMMAND;
			if (resp) {
				if (resp_type & MMC_RSP_136) {
					resp[3] = in32(base + OMAP3_MMCHS_RSP76);
					resp[2] = in32(base + OMAP3_MMCHS_RSP54);
					resp[1] = in32(base + OMAP3_MMCHS_RSP32);
					resp[0] = in32(base + OMAP3_MMCHS_RSP10);
				} else if (resp_type & MMC_RSP_PRESENT)
					resp[0] = in32(base + OMAP3_MMCHS_RSP10);
			}

			// Busy check?
			if (resp_type & MMC_RSP_BUSY) {
				int		i;

				for (i = 5 * 1024 + 3 * 1024; i > 0; i--) {
					if (in32(base + OMAP3_MMCHS_PSTATE) & PSTATE_DLA) {
                        if (i > (3 * 1024))
						    nanospin_ns(1024);
                        else
                            delay(1);
						continue;
					}
					break;
				}
				if (i <= 0) {
					intr |= MMC_ERR_CMD_TO | MMC_INTR_ERROR;
					slogf(_SLOGC_SIM_MMC, _SLOG_ERROR, "OMAP3 busy check time out, pstate = %x", in32(base + OMAP3_MMCHS_PSTATE));
				}
			}
		}

		if (sts & (INTR_TC | INTR_BWR | INTR_BRR)) {
			if (sts & INTR_TC)
				intr |= MMC_INTR_DATA;
			if (sts & INTR_BRR)
				intr |= MMC_INTR_RBRDY;
			if (sts & INTR_BWR)
				intr |= MMC_INTR_WBRDY;
		}
	}

	if (intr)
		out32(base + OMAP3_MMCHS_IE, 0);

	return intr;
}

static int omap3_setup_pio(SIM_HBA *hba, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;
	int				nblk;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;

	nblk = len / omap3->blksz;
	len  = nblk * omap3->blksz;

	omap3->dcmd = CMD_DP;
	if (nblk > 1) {
		omap3->dcmd |= CMD_MBS | CMD_BCE;
		if (ext->hccap & MMC_HCCAP_ACMD12)
			omap3->dcmd |= CMD_ACEN;
	}
	if (dir == MMC_DIR_IN) {
		omap3->dcmd |= CMD_DDIR;
		omap3->dmask = INTR_BRR;
	} else
		omap3->dmask = INTR_BWR;

	out32(omap3->mmc_base + OMAP3_MMCHS_BLK, (nblk << 16) | omap3->blksz);

	return (len);
}

static int omap3_pio_done(SIM_HBA *hba, char *buf, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;
	uintptr_t		base;
	int				nbytes, cnt;
	uint32_t		*pbuf = (uint32_t *)buf;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;
	base  = omap3->mmc_base;

	cnt = nbytes = len < MMCHS_FIFO_SIZE ? len : MMCHS_FIFO_SIZE;

	if (dir == MMC_DIR_IN) {
		if (!(in32(base + OMAP3_MMCHS_PSTATE) & PSTATE_BRE)) {
			out32(base + OMAP3_MMCHS_IE, INTR_BRR);
			return 0;
		}

		for (; nbytes > 0; nbytes -= 4)
			*pbuf++ = in32(base + OMAP3_MMCHS_DATA);

		if ( len < MMCHS_FIFO_SIZE ) {
			int	cnt;
			for ( cnt = 2000; cnt; cnt-- ) {
				if ( !( in32( base + OMAP3_MMCHS_PSTATE ) & PSTATE_RTA ) ) {
					break;
				}
				nanospin_ns( 1000 );
			}
		}
	} else {
		if (!(in32(base + OMAP3_MMCHS_PSTATE) & PSTATE_BWE)) {
			out32(base + OMAP3_MMCHS_IE, INTR_BWR);
			return 0;
		}

		for (; nbytes > 0; nbytes -= 4)
			out32(base + OMAP3_MMCHS_DATA, *pbuf++);

		if (len == cnt)
			out32(base + OMAP3_MMCHS_IE, INTR_TC);
	}

	return cnt;
}

#ifdef USE_EDMA
static void omap3_edma_bit(omap3_ext_t	*omap3, int reg, int channel)
{
	uintptr_t   base = omap3->dma_base + DRA446_SHADOW0_OFF + reg;

	if (channel > 31) {
		base    += 4;
		channel -= 32;
	}

	out32(base, 1 << channel);
}

static void omap3_edma_done(omap3_ext_t	*omap3, int channel)
{
	uintptr_t	base = omap3->dma_base + DRA446_SHADOW0_OFF;
	dra446_param	*param = (dra446_param *)(omap3->dma_base + DRA446_PARAM(channel));

	if (channel > 31) {
		base    += 4;
		channel -= 32;
	}

	if (in32(base + DRA446_EDMA_ER) & in32(base + DRA446_EDMA_EER)) {
		int i=100;
		while (param->ccnt != 0 && i--){
			printf("%s(%d): %d %x \n", __func__, __LINE__, channel, param->ccnt);
			delay(1);
		}
	}

	/* Disable this EDMA event */
	omap3_edma_bit(omap3, DRA446_EDMA_EECR, channel);
}


static void omap3_setup_rx_edma(omap3_ext_t *omap3, paddr_t addr, int len)
{
	dra446_param	*param;
	int				chnl = omap3->dma_rreq;

	/*
	 * In case there is a pending event
	 */
	omap3_edma_bit(omap3, DRA446_EDMA_ECR, chnl);

	/*
	 * Initialize Rx DMA channel
	 */
	param = (dra446_param *)(omap3->dma_base + DRA446_PARAM(chnl));
	param->opt =  (0 << 23)		/* ITCCHEN = 0 */
				| (0 << 22)		/* TCCHEN = 0 */
				| (0 << 21)		/* */
				| (0 << 20)		/* */
				| (chnl << 12)		/* TCC */
				| (0 << 11)		/* Normal completion */
				| (0 << 3)		/* PaRAM set is not static */
				| (1 << 2)		/* AB-synchronizad */
				| (0 << 1)		/* Destination address increment mode */
				| (0 << 0);		/* Source address increment mode */

	param->src          = omap3->mmc_pbase + OMAP3_MMCHS_DATA;
	param->abcnt        = (128 << 16) | 4;
	param->dst          = addr;
	param->srcdstbidx   = (4 << 16) | 0;
	param->linkbcntrld  = 0xFFFF;
	param->srcdstcidx   = (512<< 16) | 0;
	param->ccnt         = len / 512;

	/*
	 * Enable event
	 */
	omap3_edma_bit(omap3, DRA446_EDMA_EESR, chnl);
}

static void omap3_setup_tx_edma(omap3_ext_t *omap3, paddr_t addr, int len)
{
	dra446_param	*param;
	int				chnl = omap3->dma_treq;

	/*
	 * In case there is a pending event
	 */
	omap3_edma_bit(omap3, DRA446_EDMA_ECR, chnl);

	/*
	 * Initialize Tx DMA channel
	 */
	param = (dra446_param *)(omap3->dma_base + DRA446_PARAM(chnl));
	param->opt =  (0<< 23)		/* ITCCHEN = 0 */
				| (0 << 22)		/* TCCHEN = 0 */
				| (0 << 21)		/* */
				| (0 << 20)		/* */
				| (chnl << 12)	/* TCC */
				| (0 << 11)		/* Normal completion */
				| (0 << 3)		/* PaRAM set is not static */
				| (1 << 2)		/* AB-synchronizad */
				| (0 << 1)		/* Destination address increment mode */
				| (0 << 0);		/* Source address increment mode */

	param->src          = addr;
	param->abcnt        = (128 << 16) | 4;
	param->dst          = omap3->mmc_pbase + OMAP3_MMCHS_DATA;
	param->srcdstbidx   = (0 << 16) | 4;
	param->linkbcntrld  = 0xFFFF;
	param->srcdstcidx   = (0 << 16) | 512;
	param->ccnt         = len / 512;

	/*
	 * Enable event
	 */
	omap3_edma_bit(omap3, DRA446_EDMA_EESR, chnl);
}

static int omap3_setup_dma(SIM_HBA *hba, paddr_t paddr, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap;
	uintptr_t		base;

	ext    = (SIM_MMC_EXT *)hba->ext;
	omap = (omap3_ext_t *)ext->handle;
	base   = omap->mmc_base;

	len = ext->setup_pio(hba, len, dir);
	
	if(len>0){
		if (dir == MMC_DIR_IN) {	// read
			/* setup receive EDMA channel */
			omap3_setup_rx_edma(omap, paddr, len);
		} else {
			/* setup transmit EDMA channel */
			omap3_setup_tx_edma(omap, paddr, len);
		}
		omap->dcmd |= CMD_DE;
		omap->dmask = INTR_TC; 	// Use transfer complete interrupt
		omap->dsize = len;
	}
	return (len);
}

static int omap3_dma_done(SIM_HBA *hba, int dir)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap;

	ext    = (SIM_MMC_EXT *)hba->ext;
	omap = (omap3_ext_t *)ext->handle;

	if (dir == MMC_DIR_IN)
		omap3_edma_done(omap, omap->dma_rreq);
	else
		omap3_edma_done(omap, omap->dma_treq);

	return MMC_SUCCESS;
}

#else
static int omap3_setup_dma(SIM_HBA *hba, paddr_t paddr, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;
	dma4_param		*param;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;

	//
	// Initialize Tx DMA channel
	//
	param = (dma4_param *)(omap3->dma_base + DMA4_CCR(omap3->dma_chnl));

	len = omap3_setup_pio(hba, len, dir);

	if (len > 0) {
		// Clear all status bits
		param->csr  = 0x1FFE;
		param->cen  = len >> 2;
		param->cfn  = 1;			// Number of frames
		param->cse  = 1;
		param->cde  = 1;
		param->cicr = 0;			// We don't want any interrupts

		if (dir == MMC_DIR_IN) {
			// setup receive SDMA channel
			param->csdp = (2 <<  0)		// DATA_TYPE = 0x2:  32 bit element
						| (0 <<  2)		// RD_ADD_TRSLT = 0: Not used
						| (0 <<  6)		// SRC_PACKED = 0x0: Cannot pack source data
						| (0 <<  7)		// SRC_BURST_EN = 0x0: Cannot burst source
						| (0 <<  9)		// WR_ADD_TRSLT = 0: Undefined
						| (0 << 13)		// DST_PACKED = 0x0: No packing
						| (3 << 14)		// DST_BURST_EN = 0x3: Burst at 16x32 bits
						| (1 << 16)		// WRITE_MODE = 0x1: Write posted
						| (0 << 18)		// DST_ENDIAN_LOCK = 0x0: Endianness adapt
						| (0 << 19)		// DST_ENDIAN = 0x0: Little Endian type at destination
						| (0 << 20)		// SRC_ENDIAN_LOCK = 0x0: Endianness adapt
						| (0 << 21);	// SRC_ENDIAN = 0x0: Little endian type at source

			param->ccr  = DMA4_CCR_SYNCHRO_CONTROL(omap3->dma_rreq)	// Synchro control bits
						| (1 <<  5)		// FS = 1: Packet mode with BS = 0x1
						| (0 <<  6)		// READ_PRIORITY = 0x0: Low priority on read side
						| (0 <<  7)		// ENABLE = 0x0: The logical channel is disabled.
						| (0 <<  8)		// DMA4_CCRi[8] SUSPEND_SENSITIVE = 0
						| (0 << 12)		// DMA4_CCRi[13:12] SRC_AMODE = 0x0: Constant address mode
						| (1 << 14)		// DMA4_CCRi[15:14] DST_AMODE = 0x1: Post-incremented address mode
						| (1 << 18)		// DMA4_CCRi[18] BS = 0x1: Packet mode with FS = 0x1
						| (1 << 24)		// DMA4_CCRi[24] SEL_SRC_DST_SYNC = 0x1: Transfer is triggered by the source. The packet element number is specified in the DMA4_CSFI register.
						| (0 << 25);	// DMA4_CCRi[25] BUFFERING_DISABLE = 0x0

			param->cssa = omap3->mmc_pbase + OMAP3_MMCHS_DATA;
			param->cdsa = paddr;
			param->csf  = omap3->blksz >> 2;
		} else {
			// setup transmit SDMA channel
			param->csdp = (2 <<  0)		// DATA_TYPE = 0x2:  32 bit element
						| (0 <<  2)		// RD_ADD_TRSLT = 0: Not used
						| (0 <<  6)		// SRC_PACKED = 0x0: Cannot pack source data
						| (3 <<  7)		// SRC_BURST_EN = 0x3: Burst at 16x32 bits
						| (0 <<  9)		// WR_ADD_TRSLT = 0: Undefined
						| (0 << 13)		// DST_PACKED = 0x0: No packing
						| (0 << 14)		// DST_BURST_EN = 0x0: Cannot Burst
						| (0 << 16)		// WRITE_MODE = 0x0: Write not posted
						| (0 << 18)		// DST_ENDIAN_LOCK = 0x0: Endianness adapt
						| (0 << 19)		// DST_ENDIAN = 0x0: Little Endian type at destination
						| (0 << 20)		// SRC_ENDIAN_LOCK = 0x0: Endianness adapt
						| (0 << 21);	// SRC_ENDIAN = 0x0: Little endian type at source

			param->ccr  = DMA4_CCR_SYNCHRO_CONTROL(omap3->dma_treq)
						| (1 <<  5)		// FS = 1: Packet mode with BS = 0x1
						| (0 <<  6)		// READ_PRIORITY = 0x0: Low priority on read side
						| (0 <<  7)		// ENABLE = 0x0: The logical channel is disabled.
						| (0 <<  8)		// DMA4_CCRi[8] SUSPEND_SENSITIVE = 0
						| (1 << 12)		// DMA4_CCRi[13:12] SRC_AMODE = 0x1: Post-incremented address mode
						| (0 << 14)		// DMA4_CCRi[15:14] DST_AMODE = 0x0: Constant address mode
						| (1 << 18)		// DMA4_CCRi[18] BS = 0x1: Packet mode with FS = 0x1
						| (0 << 24)		// DMA4_CCRi[24] SEL_SRC_DST_SYNC = 0x0: Transfer is triggered by the source. The packet element number is specified in the DMA4_CSFI register.
						| (0 << 25);	// DMA4_CCRi[25] BUFFERING_DISABLE = 0x0

			param->cssa = paddr;
			param->cdsa = omap3->mmc_pbase + OMAP3_MMCHS_DATA;
			param->cdf  = omap3->blksz >> 2;
		}

		// Enable DMA event
		param->ccr |= 1 << 7;

		omap3->dcmd |= CMD_DE;
		omap3->dmask = INTR_TC;		// Use transfer complete interrupt
		omap3->dsize = len;
	}

	return (len);
}

static int omap3_dma_done(SIM_HBA *hba, int dir)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;
	dma4_param		*param;
	int				ret = MMC_SUCCESS;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;

	param = (dma4_param *)(omap3->dma_base + DMA4_CCR(omap3->dma_chnl));

#define	OMAP3_SDMA_ERROR	((1 << 11) | (1 << 10) | (1 << 9) | (1 << 8))
	while (1) {
		// transfer complete?
		if (param->ccen == (omap3->dsize >> 2))
			break;
		// Check DMA errors
		if (param->csr & OMAP3_SDMA_ERROR)
			break;
	}
	if (param->csr & OMAP3_SDMA_ERROR)
		ret = MMC_FAILURE;

	// Disable this DMA event
	param->ccr = 0;

	// Clear all status bits
	param->csr = 0x1FFE;

	return ret;
}
#endif

static int omap3_command(SIM_HBA *hba, mmc_cmd_t *cmd)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;
	uintptr_t		base;
	uint32_t		command;
	uint32_t		imask;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;
	base  = omap3->mmc_base;

	if (cmd->eflags & MMC_CMD_INIT) {
		uint32_t	tmp = in32(base + OMAP3_MMCHS_CON);
		out32(base + OMAP3_MMCHS_CON, tmp | CON_INIT);
		delay(10);
		out32(base + OMAP3_MMCHS_CON, tmp);
	}

	/* Clear Status */
	out32(base + OMAP3_MMCHS_STAT, 0x117F8033);

	// MMC support?
//	if (cmd->eflags & MMC_CMD_PPL)

	imask = 0x110f8000;

	command = (cmd->opcode) << 24;
	if (cmd->opcode == 12)
		command |= CMD_TYPE_CMD12;

	if (cmd->eflags & MMC_CMD_DATA) {
		command |= omap3->dcmd;
		imask |= INTR_DTO | INTR_DCRC | INTR_DEB;	// Enable all data error interrupts
		imask |= omap3->dmask;	// Data complete interrupt or data ready interrupt
	} else
		imask |= INTR_CC;		// Enable command complete interrupt

	if (cmd->resp & MMC_RSP_PRESENT) {
		if (cmd->resp & MMC_RSP_136)
			command |= CMD_RSP_TYPE_136;
		else if (cmd->resp & MMC_RSP_BUSY)	// Response with busy check
			command |= CMD_RSP_TYPE_48b;
		else
			command |= CMD_RSP_TYPE_48;

		if (cmd->resp & MMC_RSP_OPCODE)		// Index check
			command |= CMD_CICE;
		if (cmd->resp & MMC_RSP_CRC)		// CRC check
			command |= CMD_CCCE;
	}

	/* Setup the Argument Register and send CMD */
	out32(base + OMAP3_MMCHS_IE,  imask);
	out32(base + OMAP3_MMCHS_ARG, cmd->argument);
	out32(base + OMAP3_MMCHS_CMD, command);

	return (MMC_SUCCESS);
}

static int omap3_cfg_bus(SIM_HBA *hba, int width, int mmc)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;
	uintptr_t		base;
	uint32_t		tmp;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;
	base  = omap3->mmc_base;

	tmp = in32(base + OMAP3_MMCHS_CON);
	if(width==8){
		out32(base + OMAP3_MMCHS_CON, tmp | CON_DW8 );
	}else{
		out32(base + OMAP3_MMCHS_CON, tmp & ~CON_DW8 );
		tmp = in32(base + OMAP3_MMCHS_HCTL);
		if (width == 4)
			tmp |= HCTL_DTW4;
		else
			tmp &= ~HCTL_DTW4;
		out32(base + OMAP3_MMCHS_HCTL, tmp);
	}

	return (MMC_SUCCESS);
}

static int omap3_clock(SIM_HBA *hba, int *clock, int high_speed)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;
	uintptr_t		base;
	uint32_t		sysctl;
	int				clkd;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;
	base  = omap3->mmc_base;

	clkd = omap3->mmc_clock / (*clock);
	if (omap3->mmc_clock != (*clock) * clkd)
		clkd++;
	*clock = omap3->mmc_clock / clkd;

	sysctl = in32(base + OMAP3_MMCHS_SYSCTL);

	// Stop clock
	sysctl &= ~(SYSCTL_ICE | SYSCTL_CEN | SYSCTL_DTO_MASK);
	sysctl |= SYSCTL_DTO_MAX | SYSCTL_SRC | SYSCTL_SRD;
	out32(base + OMAP3_MMCHS_SYSCTL, sysctl);

	// Enable internal clock
	sysctl &= ~(0x3FF << 6);
	sysctl |= (clkd << 6) | SYSCTL_ICE;
	out32(base + OMAP3_MMCHS_SYSCTL, sysctl);

	// Wait for the clock to be stable
	while ((in32(base + OMAP3_MMCHS_SYSCTL) & SYSCTL_ICS) == 0)
		;

	// Enable clock to the card
	out32(base + OMAP3_MMCHS_SYSCTL, sysctl | SYSCTL_CEN);

	return (MMC_SUCCESS);
}

static int omap3_block_size(SIM_HBA *hba, int blksz)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;

	if (blksz > 1024)
		return (MMC_FAILURE);

	omap3->blksz = blksz;
	out32(omap3->mmc_base + OMAP3_MMCHS_BLK, omap3->blksz);
	delay(10);

	return (MMC_SUCCESS);
}

/*
 * Reset host controller and card
 * The clock should be enabled and set to minimum (<400KHz)
 */
static int omap3_powerup(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;
	uintptr_t		base;
	int				clock;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;
	base  = omap3->mmc_base;

	// Disable All interrupts
	out32(base + OMAP3_MMCHS_IE, 0);

	// Software reset
	out32(base + OMAP3_MMCHS_SYSCONFIG, SYSCONFIG_SOFTRESET);
	while ((in32(base + OMAP3_MMCHS_SYSSTATUS) & SYSSTATUS_RESETDONE) == 0)
		;

	out32(base + OMAP3_MMCHS_SYSCTL, SYSCTL_SRA);
	while ((in32(base + OMAP3_MMCHS_SYSCTL) & SYSCTL_SRA) != 0)
		;

	out32(base + OMAP3_MMCHS_HCTL, HCTL_SDVS3V0);

	out32(base + OMAP3_MMCHS_CAPA, 
			in32(base + OMAP3_MMCHS_CAPA) | CAPA_VS3V3 | CAPA_VS3V0 | CAPA_VS1V8);

	out32(base + OMAP3_MMCHS_CON, (3 << 9));

	clock = 400 * 1000;		// 400KHz clock

	omap3_clock(hba, &clock, 0);

	out32(base + OMAP3_MMCHS_HCTL, HCTL_SDVS3V0 | HCTL_SDBP);
	out32(base + OMAP3_MMCHS_ISE, 0x117F8033);	// Enable the interrupt signals which we use

	delay(10);

	return (MMC_SUCCESS);
}

static int omap3_detect(SIM_HBA *hba)
{
	// Need external logic to detect card
	return (MMC_SUCCESS);
}

static int omap3_powerdown(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;
	uintptr_t		base;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;
	base  = omap3->mmc_base;

	// Disable all MMCHS interrupt signals
	out32(base + OMAP3_MMCHS_ISE, 0);

	// Disable All interrupts
	out32(base + OMAP3_MMCHS_IE, 0);
	out32(base + OMAP3_MMCHS_ISE, 0);

	// Software reset
	out32(base + OMAP3_MMCHS_SYSCONFIG, SYSCONFIG_SOFTRESET);
	while ((in32(base + OMAP3_MMCHS_SYSSTATUS) & SYSSTATUS_RESETDONE) == 0)
		;

	out32(base + OMAP3_MMCHS_SYSCTL, SYSCTL_SRA);
	while ((in32(base + OMAP3_MMCHS_SYSCTL) & SYSCTL_SRA) != 0)
		;

	return (MMC_SUCCESS);
}

static int omap3_shutdown(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;

	ext   = (SIM_MMC_EXT *)hba->ext;
	omap3 = (omap3_ext_t *)ext->handle;

	omap3_powerdown(hba);

	munmap_device_io(omap3->mmc_base, OMAP3_MMCHS_SIZE);
	if (omap3->dma_base) {
#ifdef USE_EDMA
		omap3_edma_done(omap3, omap3->dma_rreq);
		omap3_edma_done(omap3, omap3->dma_treq);
		// Unmap the DMA registers
		munmap_device_io(omap3->dma_base, DRA446_EDMA_SIZE);
#else
		dma4_param	*param = (dma4_param *)(omap3->dma_base + DMA4_CCR(omap3->dma_chnl));

		// Disable this DMA event
		param->ccr = 0;

		// Clear all status bits
		param->csr = 0x1FFE;

		// Unmap the DMA registers
		munmap_device_io(omap3->dma_base, OMAP3_DMA4_SIZE);
#endif
	}

	free(omap3);

	return (0);
}

static unsigned omap3_get_syspage_clk(void)
{
	unsigned	item, offset, clock = 0;
	char		*name;
	hwi_tag		*tag;

	item = hwi_find_item(HWI_NULL_OFF, HWI_ITEM_DEVCLASS_DISK, "mmc", NULL);
	if (item == HWI_NULL_OFF)
		return 0;

	offset = item;

	while ((offset = hwi_next_tag(offset, 1)) != HWI_NULL_OFF) {
		tag = hwi_off2tag(offset);
		name = __hwi_find_string(((hwi_tag *)tag)->prefix.name);

		if (strcmp(name, HWI_TAG_NAME_inputclk) == 0)
			clock = ((struct hwi_inputclk *)tag)->clk / ((struct hwi_inputclk *)tag)->div;
	}

	return clock;
}

int	omap3_attach(SIM_HBA *hba)
{
	CONFIG_INFO		*cfg;
	SIM_MMC_EXT		*ext;
	omap3_ext_t		*omap3;

	cfg = (CONFIG_INFO *)&hba->cfg;
	ext = (SIM_MMC_EXT *)hba->ext;

	if ((omap3 = calloc(1, sizeof(omap3_ext_t))) == NULL)
		return (NULL);

	omap3->mmc_pbase = cfg->IOPort_Base[0];

	if ((omap3->mmc_base =
			mmap_device_io(OMAP3_MMCHS_SIZE, omap3->mmc_pbase))
				== (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "OMAP3 MMCSD: mmap_device_io failed");
		goto fail0;
	}

	// Capability
	ext->hccap |= MMC_HCCAP_ACMD12 | MMC_HCCAP_BW1 | MMC_HCCAP_BW4 | MMC_HCCAP_BW8;	// 1 bit / 4 bits bus supported
	ext->hccap |= MMC_HCCAP_33V | MMC_HCCAP_30V | MMC_HCCAP_18V;
#ifdef USE_EDMA
	if (cfg->NumDMAs > 1 && cfg->NumIOPorts > 1) {
		omap3->dma_treq = cfg->DMALst[0];
		omap3->dma_rreq = cfg->DMALst[1];
		if ((omap3->dma_base = 
			mmap_device_io(DRA446_EDMA_SIZE, cfg->IOPort_Base[1])) == (uintptr_t)MAP_FAILED) {
			slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "OMAP3 MMCSD: mmap_device_io failed");
			goto fail1;
		}
		ext->hccap |= MMC_HCCAP_DMA;
	}
#else
	if (cfg->NumDMAs > 2 && cfg->NumIOPorts > 1) {
		omap3->dma_chnl = cfg->DMALst[0];
		omap3->dma_treq = cfg->DMALst[1];
		omap3->dma_rreq = cfg->DMALst[2];

		if ((omap3->dma_base = 
			mmap_device_io(OMAP3_DMA4_SIZE, cfg->IOPort_Base[1])) == (uintptr_t)MAP_FAILED) {
			slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "OMAP3 MMCSD: mmap_device_io failed");
			goto fail1;
		}
		ext->hccap |= MMC_HCCAP_DMA;
	}
#endif

	if ((omap3->mmc_clock = omap3_get_syspage_clk()) == 0)
		omap3->mmc_clock = 96000000;

	ext->hba       = hba;
	ext->handle    = omap3;
	ext->clock     = omap3->mmc_clock;
	ext->detect    = omap3_detect;
	ext->powerup   = omap3_powerup;
	ext->powerdown = omap3_powerdown;
	ext->command   = omap3_command;
	ext->cfg_bus   = omap3_cfg_bus;
	ext->set_clock = omap3_clock;
	ext->set_blksz = omap3_block_size;
	ext->interrupt = omap3_interrupt;
	ext->setup_dma = omap3_setup_dma;
	ext->dma_done  = omap3_dma_done;
	ext->setup_pio = omap3_setup_pio;
	ext->pio_done  = omap3_pio_done;
	ext->shutdown  = omap3_shutdown;

	if (!cfg->Description[0])
		strncpy((void *)cfg->Description, "TI OMAP3 MMCHS", sizeof(cfg->Description));

	return (MMC_SUCCESS);

fail0:
	free(omap3);
fail1:
	munmap_device_io(omap3->mmc_base, OMAP3_MMCHS_SIZE);

	return (MMC_FAILURE);
}

#endif


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_omap3.c $ $Rev: 657836 $" );
