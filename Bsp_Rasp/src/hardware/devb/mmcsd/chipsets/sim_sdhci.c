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

#ifdef MMCSD_VENDOR_SDHCI

#include	<sim_sdhci.h>

//#define SDHCI_DEBUG

static int _sdhci_detect(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;
	uint32_t		i;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	if (!(SDHCI_PSTATE(base) & SDHCI_PSTATE_CI))
		return (MMC_FAILURE);

	i = 0;
	while ((SDHCI_PSTATE(base) & SDHCI_CARD_STABLE) != SDHCI_CARD_STABLE) {
		if (++i > 1000)
			return (MMC_FAILURE);
		delay(1);
	}

	return (MMC_SUCCESS);
}

static int _sdhci_reset( SIM_HBA *hba, uint32_t rst )
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;
	uint32_t		cnt;

	ext		= (SIM_MMC_EXT *)hba->ext;
	sdhci	= (sdhci_ext_t *)ext->handle;
	base	= sdhci->base;

		// wait up to 100 ms for reset to complete
	SDHCI_SWRST(base) = rst;
	for( cnt = 100; cnt && ( SDHCI_SWRST(base) & rst ); cnt-- ) {
		delay( 1 );
	}

	return( cnt ? MMC_SUCCESS : MMC_FAILURE );
}

static int _sdhci_interrupt(SIM_HBA *hba, int irq, int resp_type, uint32_t *resp)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;
	uint16_t		nsts, ersts, nmask, ermask;
	int				intr = 0;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

#if !defined(__ARM__)
	// might be a shared interrupt
	if (!(SDHCI_SLTINTSTS(base) & 1))
		return 0;
#endif

	nsts   = SDHCI_NINTSTS(base);
	ersts  = SDHCI_ERINTSTS(base);
	nmask  = SDHCI_NINTEN(base);
	ermask = SDHCI_ERINTEN(base);

	if (nsts & SDHCI_NINT_DMA) {		// DMA interrupt
		if (!(nsts & SDHCI_NINT_TC)) 	// Data not completed ?
			SDHCI_DMAADR(base) = SDHCI_DMAADR(base);
	}

	if (nsts & SDHCI_NINT_EI)
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: error 0x%04x, prestate %x", __FUNCTION__, ersts, SDHCI_PSTATE(base));

	if (nmask & SDHCI_NINT_CC) {		// Command complete interrupt enabled ?
		if (ersts & SDHCI_ERINT_CTE) {	// Command Timeout ?
			intr |= MMC_ERR_CMD_TO | MMC_INTR_ERROR | MMC_INTR_COMMAND;
			if (_sdhci_reset( hba, SDHCI_SWRST_RC | SDHCI_SWRST_RD)) {
				mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: SDHCI_SWRST_RC/SDHCI_SWRST_RD timeout", __FUNCTION__ );
			}
		} else if (nsts & SDHCI_NINT_CC) {
			intr |= MMC_INTR_COMMAND;
			if (ersts & (SDHCI_ERINT_CCE | SDHCI_ERINT_CEBE | SDHCI_ERINT_CIE))
				intr |= MMC_INTR_ERROR;	// Other errors
			if (resp_type & MMC_RSP_136) {
				resp[0] = SDHCI_RESP0(base);
				resp[1] = SDHCI_RESP1(base);
				resp[2] = SDHCI_RESP2(base);
				resp[3] = SDHCI_RESP3(base);

				/*
				 * CRC is not included in the response register,
				 * we have to left shift 8 bit to match the 128 CID/CSD structure
				 */
				resp[3] = (resp[3] << 8) | (resp[2] >> 24);
				resp[2] = (resp[2] << 8) | (resp[1] >> 24);
				resp[1] = (resp[1] << 8) | (resp[0] >> 24);
				resp[0] = (resp[0] << 8);
			} else if (resp_type & MMC_RSP_PRESENT) {
				resp[0] = SDHCI_RESP0(base);
				resp[1] = SDHCI_RESP1(base);
				resp[2] = SDHCI_RESP2(base);
				resp[3] = SDHCI_RESP3(base);
			}
		}
	}

	if (ersts & SDHCI_ERINT_ADMA) {
		intr |= MMC_INTR_ERROR;
		SDHCI_HOSTCTL(base) &= ~SDHCI_HOSTCTL_DMAMASK;
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: ADMA Error %x", __FUNCTION__, SDHCI_ADMA_ESTATE(base) );
		if (_sdhci_reset( hba, SDHCI_SWRST_RC | SDHCI_SWRST_RD)) {
			mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: SDHCI_SWRST_RC/SDHCI_SWRST_RD timeout", __FUNCTION__ );
		}
	}

	/*
	 * Check for data related interrupts
	 * FIXME!!! ACMD12 error ?
	 */
	if (nmask & (SDHCI_NINT_TC | SDHCI_NINT_BRR | SDHCI_NINT_BWR)) {
		if (nsts & SDHCI_NINT_BRR)
			intr |= MMC_INTR_RBRDY;
		else if (nsts & SDHCI_NINT_BWR)
			intr |= MMC_INTR_WBRDY;
		if (nsts & SDHCI_NINT_TC) {		// data done
			intr |= MMC_INTR_DATA;
		} else if (ersts & 0x1FF) {		// errors
			if (ersts & SDHCI_ERINT_AC12)
				mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: AC12 Error %x", __FUNCTION__, SDHCI_AC12ERRSTS(base));
			intr |= MMC_ERR_DATA_TO | MMC_INTR_ERROR;
			if (_sdhci_reset( hba, SDHCI_SWRST_RC | SDHCI_SWRST_RD)) {
				mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: SDHCI_SWRST_RC/SDHCI_SWRST_RD timeout", __FUNCTION__ );
			}
		}
	}

	SDHCI_ERINTSTS(base) = ersts;

	if (nsts & SDHCI_NINT_CIN)
		intr |= MMC_INTR_CARDINS;
	if (nsts & SDHCI_NINT_CRM)
		intr |= MMC_INTR_CARDRMV;

	SDHCI_NINTSTS(base) = nsts;		// clear interrupt status

	if (intr)
		SDHCI_HOSTCTL(base) &= ~SDHCI_HOSTCTL_LEDCTL;	// Light off

	return intr;
}

/*
 * setup DMA transfer
 */
static int _sdhci_setup_dma(SIM_HBA *hba, paddr_t paddr, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;
	uint32_t		xlen;
	uint16_t		blkcnt;
	uint16_t		xmode;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	xlen = len;
	blkcnt = xlen / sdhci->blksz;
	xlen   = sdhci->blksz * blkcnt;

	if (blkcnt == 0)
		return 0;

	xmode = SDHCI_XFRMODE_DMAEN;

	if (blkcnt > 1) {
		xmode |= SDHCI_XFRMODE_BCE | SDHCI_XFRMODE_MBS;
		if (ext->hccap & MMC_HCCAP_ACMD12)
			xmode |= SDHCI_XFRMODE_AC12EN;
	}

	if (dir == MMC_DIR_IN)
		xmode |= SDHCI_XFRMODE_DATDIR;

	SDHCI_BLKCNT(base)  = blkcnt;	// only valid for multi-block transfer
	SDHCI_XFRMODE(base) = xmode;
	SDHCI_DMAADR(base)  = paddr;

	return (xlen);
}

/*
 * setup PIO transfer
 */
static int _sdhci_setup_pio(SIM_HBA *hba, int len, int dir)
{
	SIM_MMC_EXT 	*ext;
	sdhci_ext_t 	*sdhci;
	uintptr_t		base;
	uint16_t		blkcnt, xmode = 0;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	blkcnt = len / sdhci->blksz;

	if (blkcnt == 0)
		return 0;

	xmode = SDHCI_XFRMODE_DMAEN;
	if (blkcnt > 1) {
		xmode |= SDHCI_XFRMODE_BCE | SDHCI_XFRMODE_MBS;
		if (ext->hccap & MMC_HCCAP_ACMD12)
			xmode |= SDHCI_XFRMODE_AC12EN;
	}

	if (dir == MMC_DIR_IN)
		xmode |= SDHCI_XFRMODE_DATDIR;

	SDHCI_BLKCNT(base)	= blkcnt;	// only valid for multi-block transfer
	SDHCI_XFRMODE(base) = xmode;
	SDHCI_DMAADR(base)	= sdhci->tbuf_phy;
	sdhci->pio_enable = 1;
	return (blkcnt * sdhci->blksz);
}

#ifdef SDHCI_ADMA_SUP
static int _sdhci_setup_sgdma(SIM_HBA *hba, int dir)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;
	uint32_t		xlen;
	uint16_t		blkcnt;
	uint16_t		xmode;
	sdhci_adma32_t	*adma;
	CCB_SCSIIO		*ccb;

	ext   = (SIM_MMC_EXT *)hba->ext;
	ccb	  = ext->nexus;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	adma = (sdhci_adma32_t *)sdhci->tbuf;

#ifdef SDHCI_DEBUG
	mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: ", __FUNCTION__ );
#endif

	if (ccb->cam_ch.cam_flags & CAM_SCATTER_VALID) {
		SG_ELEM		*sg;
		int			sgi;

		sg = ccb->cam_data.cam_sg_ptr;
		for (xlen = 0, sgi = 0; sgi < ccb->cam_sglist_cnt; sgi++, adma++, sg++) {
			adma->addr = mmc_mphys(ccb, sg->cam_sg_address, sgi);
			adma->attr = SDHCI_ADMA2_VALID | SDHCI_ADMA2_TRAN;
			adma->len  = sg->cam_sg_count;

			xlen += sg->cam_sg_count;
		}
		adma--;
	} else {
		adma->addr = mmc_mphys(ccb, ccb->cam_data.cam_data_ptr, 0);
		adma->attr = SDHCI_ADMA2_VALID | SDHCI_ADMA2_TRAN;
		adma->len  = xlen = ccb->cam_dxfer_len;
	}
	adma->attr |= SDHCI_ADMA2_END;

	blkcnt = xlen / sdhci->blksz;
	xlen   = sdhci->blksz * blkcnt;

	if (blkcnt == 0)
		return 0;

	xmode = SDHCI_XFRMODE_DMAEN;

	if (blkcnt > 1) {
		xmode |= SDHCI_XFRMODE_BCE | SDHCI_XFRMODE_MBS;
		if (ext->hccap & MMC_HCCAP_ACMD12)
			xmode |= SDHCI_XFRMODE_AC12EN;
	}

	if (dir == MMC_DIR_IN)
		xmode |= SDHCI_XFRMODE_DATDIR;

	SDHCI_HOSTCTL(base) = (SDHCI_HOSTCTL(base) & ~SDHCI_HOSTCTL_DMAMASK) | SDHCI_HOSTCTL_ADMA32;
	SDHCI_BLKCNT(base)  = blkcnt;   // only valid for multi-block transfer
	SDHCI_XFRMODE(base) = xmode;
	SDHCI_ADMA_ADDRL(base) = sdhci->tbuf_phy;

	return xlen;
}
#endif

static int _sdhci_dma_done(SIM_HBA *hba, int dir)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	SDHCI_HOSTCTL(base) &= ~SDHCI_HOSTCTL_DMAMASK;

	return MMC_SUCCESS;
}

static int _sdhci_pio_done(SIM_HBA *hba, char *buf, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;
	uint32_t		cnt, *pbuf = (uint32_t *)buf;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	if (len < sdhci->blksz)
		return 0;

	if (dir == MMC_DIR_IN) {
		uint32_t	*tbuf = (uint32_t *)sdhci->tbuf;
		for (cnt = 0; cnt < sdhci->blksz; cnt += 4)
			*pbuf++ = *tbuf++;

		sdhci->pio_enable = 0;
	} else {
		if (!(SDHCI_PSTATE(base) & SDHCI_PSTATE_BUFWREN))
			return 0;
		for (cnt = 0; cnt < sdhci->blksz; cnt += 4)
			SDHCI_BUFDATA(base) = *pbuf++;
	}
	return sdhci->blksz;
}

static int _sdhci_command(SIM_HBA *hba, mmc_cmd_t *cmd)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;
	uint16_t		command;
	uint32_t		i;
	uint16_t		mask = SDHCI_PSTATE_CINH;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	command = SDHCI_CMD_CMDIDX(cmd->opcode);

	if (cmd->resp & MMC_RSP_PRESENT) {
		if (cmd->resp & MMC_RSP_OPCODE)
			command |= SDHCI_CMD_CICE;

		if (cmd->resp & MMC_RSP_CRC)
			command |= SDHCI_CMD_CCCE;

		if (cmd->resp & MMC_RSP_BUSY) {			// must be R1b
			command |= SDHCI_CMD_RSPLEN48b;
			mask |= SDHCI_PSTATE_DCI;
		} else if (cmd->resp & MMC_RSP_136) 	// must be R2
			command |= SDHCI_CMD_RSPLEN136;
		else
			command |= SDHCI_CMD_RSPLEN48;
	}

	if (cmd->eflags & MMC_CMD_DATA) {
		mask |= SDHCI_PSTATE_DCI;
		command |= SDHCI_CMD_DPS;
	}

	for (i = 0; SDHCI_PSTATE(base) & mask; i++) {
		if (i > 1000000)
			return (MMC_FAILURE);

		nanospin_ns(100);
	}

	/*
	 * Only enable the interrupts we want
	 */
	mask = SDHCI_NINT_CIN | SDHCI_NINT_CRM;
	if (cmd->eflags & MMC_CMD_DATA) {
		SDHCI_ERINTEN(base) = 0x017F;
		if ((cmd->eflags & MMC_CMD_DATA_DMA) || sdhci->pio_enable) {
			mask |= SDHCI_NINT_DMA;
			mask |= SDHCI_NINT_TC;
		}
		else if (cmd->eflags & MMC_CMD_DATA_IN)
			mask |= SDHCI_NINT_BRR;
		else
			mask |= SDHCI_NINT_BWR | SDHCI_NINT_TC;
	} else {
		SDHCI_ERINTEN(base) = 0x017F;
		mask |= SDHCI_NINT_CC;
	}

	SDHCI_NINTEN(base) = mask;
	SDHCI_CMDARG(base) = cmd->argument;
	SDHCI_SDCMD(base)  = command;

	SDHCI_HOSTCTL(base) |= SDHCI_HOSTCTL_LEDCTL;	// Light up

	return (MMC_SUCCESS);
}

static int _sdhci_cfg_bus(SIM_HBA *hba, int width, int mmc)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;
	uint8_t			hostctl;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	hostctl = SDHCI_HOSTCTL(base) & ~SDHCI_HOSTCTL_DTW4BIT;

	if (sdhci->cap & (SDHCI_CAP_ADMA2 | SDHCI_CAP_ADMA1))
		hostctl &= ~SDHCI_HOSTCTL_BW8;
	else
		hostctl &= ~SDHCI_HOSTCTL_MMC8;		// old Intel implementation (no ADMA)
	
	if (width == 8) {
		if (sdhci->cap & (SDHCI_CAP_ADMA2 | SDHCI_CAP_ADMA1))
			hostctl |= SDHCI_HOSTCTL_BW8;
		else
			hostctl |= SDHCI_HOSTCTL_MMC8;
	}
	else if (width == 4) {
		hostctl |= SDHCI_HOSTCTL_DTW4BIT;
	}

	SDHCI_HOSTCTL(base) = hostctl;

	return (MMC_SUCCESS);
}

static int _sdhci_clock(SIM_HBA *hba, int *clock, int high_speed)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;
	uint16_t		clkctl;
	uint32_t		clk;
	uint8_t			hostctl;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	clk = sdhci->clock;

	// Disable SD clock first
	SDHCI_CLKCTL(base) &= ~SDHCI_CLKCTL_CLKEN;

	if (*clock > clk)
		*clock = clk;

	if (*clock < (clk / 256))
		*clock = clk / 256;

	for (clkctl = 0; clkctl <= 8; clkctl++) {
		if ((clk / (1 << clkctl)) <= *clock) {
			*clock = clk / (1 << clkctl);
			break;
		}
	}

	hostctl = SDHCI_HOSTCTL(base) & ~SDHCI_HOSTCTL_HSEN;
	if (high_speed)
		hostctl |= SDHCI_HOSTCTL_HSEN;

	if (clkctl)
		clkctl = 1 << (clkctl - 1);
	SDHCI_CLKCTL(base)  = (clkctl << 8) | SDHCI_CLKCTL_ICE;
	SDHCI_CLKCTL(base)  = (clkctl << 8) | SDHCI_CLKCTL_ICE | SDHCI_CLKCTL_CLKEN;
	SDHCI_HOSTCTL(base) = hostctl;

	return (MMC_SUCCESS);
}

static int _sdhci_block_size(SIM_HBA *hba, int blksz)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	if (blksz > 4095)
		return (MMC_FAILURE);

	sdhci->blksz = blksz;

	SDHCI_BLKSZ(base) = blksz | (7 << 12);

	return (MMC_SUCCESS);
}

/*
 * Reset host controller and card
 * The clock should be enabled and set to minimum (<400KHz)
 */
static int _sdhci_powerup(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	sdhci_ext_t		*sdhci;
	uintptr_t		base;
	uint32_t		clk;
	uint16_t		clkctl;
	int				timeout;

	ext   = (SIM_MMC_EXT *)hba->ext;
	sdhci = (sdhci_ext_t *)ext->handle;
	base  = sdhci->base;

	/*
	 * Card is in
	 */
	if (SDHCI_PSTATE(base) & SDHCI_PSTATE_WP)
		ext->eflags &= ~MMC_EFLAG_WP;
	else
		ext->eflags |= MMC_EFLAG_WP;

	if (_sdhci_reset( hba,  SDHCI_SWRST_ALL)) {
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: SDHCI_SWRST_ALL timeout", __FUNCTION__ );
		return (MMC_FAILURE);
	}

	/*
	 * Enable internal clock
	 * Set clock to 400KHz for ident
	 */
	clk = sdhci->clock;
	for (clkctl = 0; clkctl <= 8; clkctl++) {
		if ((clk / (1 << clkctl)) <= 400 * 1000) {
			clkctl = (1 << (clkctl - 1)) << 8;
			break;
		}
	}

	/*
	 * Enable internal clock first
	 */
	SDHCI_CLKCTL(base) = clkctl;
	SDHCI_CLKCTL(base) = clkctl | SDHCI_CLKCTL_ICE;
	timeout = 1024 * 1024;
	while (!(SDHCI_CLKCTL(base) & SDHCI_CLKCTL_ICS)) {
		if (--timeout == 0) {
			SDHCI_SWRST(base) = SDHCI_SWRST_ALL;
			return (MMC_FAILURE);
		}
		nanospin_ns(100);
	}

	/*
	 * Powerup the bus, to 3.3V
	 */
	SDHCI_PWRCTL(base) = SDHCI_PWRCTL_PWREN | SDHCI_PWRCTL_V33;
	delay(sdhci->pwrup_delay);

	/*
	 * Enable SD clock
	 */
	SDHCI_CLKCTL(base) |= SDHCI_CLKCTL_CLKEN;

	/*
	 * Currently set the timeout to maximum
	 */
	SDHCI_TOCTL(base) = 0x0E;

	/*
	 * Enable interrupt signals
	 */
	SDHCI_NINTSIGEN(base)  = 0x00FF;
	SDHCI_ERINTSIGEN(base) = 0x01FF;

	/*
	 * disable all interrupts except card insertion and removal
	 */
	SDHCI_ERINTEN(base) = 0x0000;
	SDHCI_NINTEN(base)  = SDHCI_NINT_CRM | SDHCI_NINT_CIN;

	return (MMC_SUCCESS);
}

static int _sdhci_powerdown(SIM_HBA *hba)
{
	CONFIG_INFO			*cfg;
	SIM_MMC_EXT			*ext;
	sdhci_ext_t			*sdhci;

	ext = (SIM_MMC_EXT *)hba->ext;
	cfg = (CONFIG_INFO *)&hba->cfg;
	sdhci = (sdhci_ext_t *)ext->handle;

	/* Power off the bus */
	SDHCI_PWRCTL(sdhci->base) = 0x00;

	/* Disable clocks */
	SDHCI_CLKCTL(sdhci->base) = 0x00;

	return (MMC_SUCCESS);
}

static int _sdhci_shutdown(SIM_HBA *hba)
{
	CONFIG_INFO			*cfg;
	SIM_MMC_EXT			*ext;
	sdhci_ext_t			*sdhci;

	ext = (SIM_MMC_EXT *)hba->ext;
	cfg = (CONFIG_INFO *)&hba->cfg;
	sdhci = (sdhci_ext_t *)ext->handle;

	if (_sdhci_reset( hba,  SDHCI_SWRST_ALL)) {
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: SDHCI_SWRST_ALL timeout", __FUNCTION__ );
		return (MMC_FAILURE);
	}

	_sdhci_powerdown(hba);

	munmap(sdhci->tbuf, SDHCI_DMABUF_LEN);
	munmap_device_memory((void *)sdhci->base, cfg->MemLength[0]);
	pci_detach_device(sdhci->pci_dev_hdl);
	pci_detach(sdhci->pci_hdl);
	free(sdhci);

	return (MMC_SUCCESS);
}

int	sdhci_init(SIM_HBA *hba)
{
	CONFIG_INFO			*cfg;
	SIM_MMC_EXT			*ext;
	sdhci_ext_t			*sdhci;
	struct pci_dev_info	pci_info;
	uintptr_t			base;
	uint32_t			cap;

	ext = (SIM_MMC_EXT *)hba->ext;
	cfg = (CONFIG_INFO *)&hba->cfg;

	if ((sdhci = calloc(1, sizeof(sdhci_ext_t))) == NULL)
		return (MMC_FAILURE);

	if (cfg->Device_ID.DevID != 0) {
		if ((sdhci->pci_hdl = pci_attach(0)) == -1) {
			mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: pci_attach failed", __FUNCTION__ );
			goto fail1;
		}

		pci_info.VendorId = cfg->Device_ID.DevID & 0xFFFF;
		pci_info.DeviceId = cfg->Device_ID.DevID >> 16;
		sdhci->pci_dev_hdl = pci_attach_device(NULL,
				PCI_MASTER_ENABLE | PCI_INIT_ALL, 0, &pci_info);
		if (sdhci->pci_dev_hdl == NULL)
			goto fail2;

		if (pci_info.Class != 0x080501)
			goto fail3;

		if (cfg->NumIRQs == 0) {
			cfg->IRQRegisters[0] = pci_info.Irq;
			cfg->NumIRQs = 1;
		}

		if (cfg->NumMemWindows == 0) {
			cfg->MemBase[0] = PCI_MEM_ADDR(pci_info.CpuBaseAddress[0]);
			cfg->MemLength[0] = pci_info.BaseAddressSize[0];
			cfg->NumMemWindows = 1;
		}
	} else if (cfg->NumMemWindows == 0) {
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: invalid commandline args", __FUNCTION__ );
		return (MMC_FAILURE);
	}

	base = (uintptr_t)mmap_device_memory(NULL, cfg->MemLength[0],
				PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, cfg->MemBase[0]);
	if (base == (uintptr_t)MAP_FAILED) {
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: mmap_device_memory failed", __FUNCTION__ );
		return (MMC_FAILURE);
	}

	ext->hccap |= MMC_HCCAP_ACMD12 | MMC_HCCAP_BW1 | MMC_HCCAP_BW4 | MMC_HCCAP_BW8 | MMC_HCCAP_CD_INTR ;

	cap = SDHCI_CAP(base);

	if (cap & SDHCI_CAP_S18)
		ext->hccap |= MMC_HCCAP_18V;

	if (cap & SDHCI_CAP_S30)
		ext->hccap |= MMC_HCCAP_30V;

	if (cap & SDHCI_CAP_S33)
		ext->hccap |= MMC_HCCAP_33V;

	if (cap & SDHCI_CAP_DMA)
		ext->hccap |= MMC_HCCAP_DMA;

	if (cap & SDHCI_CAP_HS)
		ext->hccap |= MMC_HCCAP_HS;

	sdhci->cap   = cap;
	sdhci->clock = ((SDHCI_CAP(base) >> 8) & 0x3F) * 1000 * 1000;
	sdhci->base  = base;
	sdhci->hba   = hba;
	sdhci->pwrup_delay = 30; // set default pwr_up delay

	/* Allocate 1 K local DMA Buffer memory to workaround PIO mode bug*/
	if ((sdhci->tbuf = mmap(NULL, SDHCI_DMABUF_LEN, PROT_READ | PROT_WRITE | PROT_NOCACHE, 
		MAP_PRIVATE | MAP_ANON | MAP_PHYS, NOFD, 0)) == MAP_FAILED) {
		return (-1);
	}
	sdhci->tbuf_phy		= mphys(sdhci->tbuf);

	ext->handle			= sdhci;
	ext->clock			= sdhci->clock;
	ext->detect			= ext->detect ? ext->detect : _sdhci_detect;
	ext->powerup		= ext->powerup ? ext->powerup : _sdhci_powerup;
	ext->powerdown		= _sdhci_powerdown;
	ext->cfg_bus		= _sdhci_cfg_bus;
	ext->set_clock		= _sdhci_clock;
	ext->set_blksz		= _sdhci_block_size;
	ext->interrupt		= _sdhci_interrupt;
	ext->command		= _sdhci_command;
	ext->setup_dma		= _sdhci_setup_dma;
	ext->dma_done		= _sdhci_dma_done;
	ext->setup_pio		= _sdhci_setup_pio;
	ext->pio_done		= _sdhci_pio_done;
	ext->shutdown		= _sdhci_shutdown;
#ifdef SDHCI_ADMA_SUP
	if (sdhci->cap & SDHCI_CAP_ADMA2) {
		ext->setup_sgdma	= _sdhci_setup_sgdma;
		ext->max_sg			= SDHCI_DMABUF_LEN / sizeof(sdhci_adma32_t);
	}
#endif

	/*
	 * Enable interrupt signals
	 */
	SDHCI_NINTSIGEN(base)  = 0x00FF;
	SDHCI_ERINTSIGEN(base) = 0x01FF;

	/*
	 * disable all interrupts except card insertion / removal
	 */
	SDHCI_ERINTEN(base) = 0;
	SDHCI_NINTEN(base)  = SDHCI_NINT_CRM | SDHCI_NINT_CIN;

	if (!cfg->Description[0])
		strncpy(cfg->Description, "Generic SDHCI", sizeof(cfg->Description));

	return (MMC_SUCCESS);

fail3:
	pci_detach_device(sdhci->pci_dev_hdl);
fail2:
	pci_detach(sdhci->pci_hdl);
fail1:
	free(sdhci);
	return (MMC_FAILURE);
}

#endif


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_sdhci.c $ $Rev: 657836 $" );
