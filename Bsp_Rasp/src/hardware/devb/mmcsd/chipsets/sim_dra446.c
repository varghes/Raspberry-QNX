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

#ifdef MMCSD_VENDOR_TI_DRA446

#include	<sim_dra446.h>
#include	<sys/syspage.h>
#include	<hw/sysinfo.h>


static int dra446_interrupt(SIM_HBA *hba, int irq, int resp_type, uint32_t *resp)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;
	_uint32			sts;
	int				intr = 0;
	uintptr_t		base;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;
	base   = dra446->mmc_base;

	sts = in32(base + DRA446_MMCST0);
	sts &= in32(base + DRA446_MMCIM);

	if (sts & (MMCST0_RSPDNE | MMCST0_BSYDNE)) { 	// response/busy done
		intr |= MMC_INTR_COMMAND;
		if (resp_type & MMC_RSP_136) {
			resp[3] = in32(base + DRA446_MMCRSP67);
			resp[2] = in32(base + DRA446_MMCRSP45);
			resp[1] = in32(base + DRA446_MMCRSP23);
			resp[0] = in32(base + DRA446_MMCRSP01);
		} else if (resp_type & MMC_RSP_PRESENT)
			resp[0] = in32(base + DRA446_MMCRSP67);
	}

	if (sts & DRA446_ST0_ERROR) {
		intr |= MMC_INTR_ERROR | MMC_INTR_COMMAND;
		if (sts & MMCST0_TOUTRS)	// Timeout
			intr |= MMC_ERR_CMD_TO;
	}

	if (sts & (MMCST0_DATDNE | MMCST0_DRRDY)) 		// data done
		intr |= MMC_INTR_DATA;

	if (intr)
		out32(base + DRA446_MMCIM, 0);

	return intr;
}

static void dra446_edma_bit(dra446_ext_t *dra446, int reg, int channel)
{
	uintptr_t   base = dra446->dma_base + DRA446_SHADOW0_OFF + reg;

	if (channel > 31) {
		base    += 4;
		channel -= 32;
	}

	out32(base, 1 << channel);
}

static void dra446_edma_done(dra446_ext_t *dra446, int channel)
{
	uintptr_t	base = dra446->dma_base + DRA446_SHADOW0_OFF;
	uint32_t	bit;

	if (channel > 31) {
		base    += 4;
		channel -= 32;
	}

	bit = 1 << channel;

	if (in32(base + DRA446_EDMA_ER) & in32(base + DRA446_EDMA_EER)) {
		dra446_param	*param = (dra446_param *)(dra446->dma_base + DRA446_PARAM(channel));

		while (param->ccnt != 0)
			;
	}

	/* Disable this EDMA event */
	dra446_edma_bit(dra446, DRA446_EDMA_EECR, channel);
}


static void dra446_setup_rx_edma(dra446_ext_t *dra446, paddr_t addr, int len)
{
	dra446_param	*param;
	int				chnl = dra446->chrdma;

	/*
	 * In case there is a pending event
	 */
	dra446_edma_bit(dra446, DRA446_EDMA_ECR, chnl);

	/*
	 * Initialize Rx DMA channel
	 */
	param = (dra446_param *)(dra446->dma_base + DRA446_PARAM(chnl));
	param->opt =  (0 << 23)		/* ITCCHEN = 0 */
				| (0 << 22)		/* TCCHEN = 0 */
				| (0 << 21)		/* */
				| (0 << 20)		/* */
				| (26 << 12)	/* TCC */
				| (0 << 11)		/* Normal completion */
				| (0 << 3)		/* PaRAM set is not static */
				| (1 << 2)		/* AB-synchronizad */
				| (0 << 1)		/* Destination address increment mode */
				| (0 << 0);		/* Source address increment mode */

	param->src          = dra446->mmc_pbase + DRA446_MMCDRR;
	param->abcnt        = (8 << 16) | 4;
	param->dst          = addr;
	param->srcdstbidx   = (4 << 16) | 0;
	param->linkbcntrld  = 0xFFFF;
	param->srcdstcidx   = (32 << 16) | 0;
	param->ccnt         = len / 32;

	/*
	 * Enable event
	 */
	dra446_edma_bit(dra446, DRA446_EDMA_EESR, chnl);
}

static void dra446_setup_tx_edma(dra446_ext_t *dra446, paddr_t addr, int len)
{
	dra446_param	*param;
	int				chnl = dra446->chtdma;

	/*
	 * In case there is a pending event
	 */
	dra446_edma_bit(dra446, DRA446_EDMA_ECR, chnl);

	/*
	 * Initialize Tx DMA channel
	 */
	param = (dra446_param *)(dra446->dma_base + DRA446_PARAM(chnl));
	param->opt =  (0 << 23)		/* ITCCHEN = 0 */
				| (0 << 22)		/* TCCHEN = 0 */
				| (0 << 21)		/* */
				| (0 << 20)		/* */
				| (27 << 12)	/* TCC */
				| (0 << 11)		/* Normal completion */
				| (0 << 3)		/* PaRAM set is not static */
				| (1 << 2)		/* AB-synchronizad */
				| (0 << 1)		/* Destination address increment mode */
				| (0 << 0);		/* Source address increment mode */

	param->src          = addr;
	param->abcnt        = (8 << 16) | 4;
	param->dst          = dra446->mmc_pbase + DRA446_MMCDXR;
	param->srcdstbidx   = (0 << 16) | 4;
	param->linkbcntrld  = 0xFFFF;
	param->srcdstcidx   = (0 << 16) | 32;
	param->ccnt         = len / 32;

	/*
	 * Enable event
	 */
	dra446_edma_bit(dra446, DRA446_EDMA_EESR, chnl);
}

// Note:
// We only use PIO to read SCR and check/switch high speed mode, so only read operation is possible
static int dra446_setup_pio(SIM_HBA *hba, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;
	uintptr_t		base;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;
	base   = dra446->mmc_base;

	len &= ~(dra446->blksz - 1);

	out32(base + DRA446_MMCFIFOCTL, MMCFIFOCTL_FIFO_RESET);    /* Reset FIFO */
	out32(base + DRA446_MMCNBLK, len / dra446->blksz);
	if (dir == MMC_DIR_IN) {
		out32(base + DRA446_MMCFIFOCTL, MMCFIFOCTL_ACCWD4 | MMCFIFOCTL_FIFO_256);

		if (len <= 32)
			dra446->piomask = MMCIM_EDATDNE;
		else
			dra446->piomask = MMCIM_EDRRDY;
	} else {
		// TODO
		out32(base + DRA446_MMCFIFOCTL, MMCFIFOCTL_ACCWD4 | MMCFIFOCTL_FIFO_256 | MMCFIFOCTL_FIFO_WRITE);
	}


	return (len);
}

static int dra446_pio_done(SIM_HBA *hba, char *buf, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;
	uintptr_t		base;
	int				cnt, nbytes = len;
	uint32_t		pos;
	uint32_t		*pbuf = (uint32_t *)buf;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;
	base   = dra446->mmc_base;

	if (dir == MMC_DIR_IN) {
		while (nbytes) {
			cnt = nbytes <= 32 ? nbytes : 32;
			while (cnt) {
				*pbuf++ = in32(base + DRA446_MMCDRR);
				nbytes -= 4;
				cnt -= 4;
			}

			if (nbytes > 32) {
				while (!(in32(base + DRA446_MMCST0) & MMCST0_DRRDY))
					nanospin_ns(1000);
			} else if (nbytes > 0) {
				while (!(in32(base + DRA446_MMCST0) & MMCST0_DATDNE))
					nanospin_ns(1000);
			} 
		}

		pos = dra446->chrdma;
	} else {
		// TODO
		pos = dra446->chtdma;
	}

	/* Clear missed event */
	base = dra446->dma_base + 0x300;

	if (pos > 31)
		base += 4, pos -= 32;

	out32(base, 1 << pos);

	return len;
}

static int dra446_setup_dma(SIM_HBA *hba, paddr_t paddr, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;
	uintptr_t		base;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;
	base   = dra446->mmc_base;

	len &= ~(dra446->blksz - 1);

	out32(base + DRA446_MMCFIFOCTL, MMCFIFOCTL_FIFO_RESET);    /* Reset FIFO */
	out32(base + DRA446_MMCNBLK, len / dra446->blksz);

	if (dir == MMC_DIR_IN) {	// read
		out32(base + DRA446_MMCFIFOCTL, MMCFIFOCTL_ACCWD4 | MMCFIFOCTL_FIFO_256);
		/* setup receive EDMA channel */
		dra446_setup_rx_edma(dra446, paddr, len);
	} else {
		out32(base + DRA446_MMCFIFOCTL, MMCFIFOCTL_ACCWD4 | MMCFIFOCTL_FIFO_256 | MMCFIFOCTL_FIFO_WRITE);
		/* setup transmit EDMA channel */
		dra446_setup_tx_edma(dra446, paddr, len);
	}

	return (len);
}

static int dra446_dma_done(SIM_HBA *hba, int dir)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;

	if (dir == MMC_DIR_IN)
		dra446_edma_done(dra446, dra446->chrdma);
	else
		dra446_edma_done(dra446, dra446->chtdma);

	return MMC_SUCCESS;
}

static int dra446_command(SIM_HBA *hba, mmc_cmd_t *cmd)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;
	uintptr_t		base;
	_uint32			status;
	_uint32			command;
	_uint32			mask;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;
	base   = dra446->mmc_base;

	/* wait for card not busy */
	while (in32(base + DRA446_MMCST1) & MMCST1_BUSY)
		nanospin_ns(100);

	/* Clear Status */
	status = in32(base + DRA446_MMCST0);

#if 0
	/* Clear Response Registers*/
	out32(base + DRA446_MMCRSP01, 0);
	out32(base + DRA446_MMCRSP23, 0);
	out32(base + DRA446_MMCRSP45, 0);
	out32(base + DRA446_MMCRSP67, 0);
	out32(base + DRA446_MMCCIDX,  0);
#endif

	command = cmd->opcode;
	if (cmd->eflags & MMC_CMD_PPL)
		command |= MMCCMD_PPLEN;

	if (cmd->eflags & MMC_CMD_INIT)
		command |= MMCCMD_INITCK;			// initialization clock sequence

	mask = MMCIM_ETOUTRS | MMCIM_ECRCRS;
	if (cmd->eflags & MMC_CMD_DATA) {
		command |= MMCCMD_DMATRIG | MMCCMD_WDATX;	// data transfer, DMA enable
		if (dra446->piomask == 0) {
			mask |= MMCIM_EDATDNE;				// Data done interrupt enable
			if (!(cmd->eflags & MMC_CMD_DATA_IN)) {
				command |= MMCCMD_DTRW;			// data write
				mask |= MMCIM_ECRCWR;
			} else
				mask |= MMCIM_ETOUTRD | MMCIM_ECRCRD;
		} else {
			mask |= dra446->piomask;			// response done interrupt enable
			dra446->piomask = 0;
		}
	} else if (cmd->resp == MMC_RSP_R1B)
		mask |= MMCIM_EBSYDNE;				// busy done interrupt enable
	else
		mask |= MMCIM_ERSPDNE;				// response done interrupt enable

	switch (cmd->resp) {
		case MMC_RSP_NONE:
			break;
		case MMC_RSP_R2:
			command |= MMCCMD_RSPFMT_R2;
			break;
		case MMC_RSP_R3:
			command |= MMCCMD_RSPFMT_R3;
			break;
		case MMC_RSP_R1B:
			command |= MMCCMD_BSYEXP;	// busy expected
		default:
			command |= MMCCMD_RSPFMT_R1;
			break;
	}

	/* Setup the Argument Register and send CMD */
	out32(base + DRA446_MMCARGHL, cmd->argument);
	out32(base + DRA446_MMCCMD, command);
	out32(base + DRA446_MMCIM, mask);

	return (MMC_SUCCESS);
}

static int dra446_cfg_bus(SIM_HBA *hba, int width, int mmc)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;
	uintptr_t		base;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;
	base   = dra446->mmc_base;

	if (width == 4)
		out32(base + DRA446_MMCCTL, MMCCTL_WIDTH4);
	else
		out32(base + DRA446_MMCCTL, MMCCTL_WIDTH1);

	return (MMC_SUCCESS);
}

static int dra446_clock(SIM_HBA *hba, int *clock, int high_speed)
{
	SIM_MMC_EXT		*ext = (SIM_MMC_EXT *)hba->ext;
	dra446_ext_t	*dra446 = (dra446_ext_t *)ext->handle;
	int				clkrt, pclk = dra446->mmc_clock / 2;

	for (clkrt = 1; clkrt < 256; clkrt++) {
		if ((pclk / clkrt) <= *clock)
			break;
	}

	*clock = pclk / clkrt;

	out32(dra446->mmc_base + DRA446_MMCCLK, MMCCLK_CLKEN | (clkrt - 1));
	delay(10);

	return (MMC_SUCCESS);
}

static int dra446_block_size(SIM_HBA *hba, int blksz)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;

	if (blksz > 4095)
		return (MMC_FAILURE);

	dra446->blksz = blksz;

	out32(dra446->mmc_base + DRA446_MMCBLEN, blksz);

	return (MMC_SUCCESS);
}

/*
 * Reset host controller and card
 * The clock should be enabled and set to minimum (<400KHz)
 */
static int dra446_powerup(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;
	uintptr_t		base;
	int				clock;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;
	base   = dra446->mmc_base;

	out32(base + DRA446_MMCIM, 0);

	/* Enable the Clock and set the clock <= 400KHz */
	clock = 400 * 1000;
	dra446_clock(hba, &clock, 0);

	/* Set FIFO level */
	out32(base + DRA446_MMCFIFOCTL, MMCFIFOCTL_ACCWD4);

	/* put the CMD and Data Logic in RESET state */
	out32(base + DRA446_MMCCTL, MMCCTL_DATRST | MMCCTL_CMDRST | MMCCTL_WIDTH1);
	out32(base + DRA446_MMCTOR, 0x1FFF);   /* Response time out 255 clcok cycles */
	out32(base + DRA446_MMCTOD, 0xFFFF);   /* Data read time out 2097151 clcok cycles */
	out32(base + DRA446_MMCCTL, MMCCTL_WIDTH1);

	return (MMC_SUCCESS);
}

static int dra446_detect(SIM_HBA *hba)
{
	// The MMC controller on DRA446 has no ability to detect the card
	return (MMC_SUCCESS);
}

static int dra446_powerdown(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;

	out32(dra446->mmc_base + DRA446_MMCIM, 0);

	/*
	 * Make sure EDMA is stopped
	 * We can't actually stop EDMA once it started, so we wait for it to finish its work
	 */
	dra446_edma_done(dra446, dra446->chrdma);
	dra446_edma_done(dra446, dra446->chtdma);

	out32(dra446->mmc_base + DRA446_MMCCLK, 0);
	
	return (MMC_SUCCESS);
}

static int dra446_shutdown(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;

	ext    = (SIM_MMC_EXT *)hba->ext;
	dra446 = (dra446_ext_t *)ext->handle;

	dra446_powerdown(hba);

	munmap_device_io(dra446->mmc_base, DRA446_SDC_SIZE);
	munmap_device_io(dra446->dma_base, DRA446_EDMA_SIZE);
	free(dra446);

	return (0);
}

static unsigned dra446_get_syspage_clk(void)
{
	unsigned	item, offset, clock = 0;
	char		*name;
	hwi_tag		*tag;

	/* Find network hardware information. */
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

int	dra446_attach(SIM_HBA *hba)
{
	CONFIG_INFO		*cfg;
	SIM_MMC_EXT		*ext;
	dra446_ext_t	*dra446;
	unsigned		reg, mask;

	cfg = (CONFIG_INFO *)&hba->cfg;
	ext = (SIM_MMC_EXT *)hba->ext;

	if ((dra446= calloc(1, sizeof(dra446_ext_t))) == NULL)
		return (NULL);

	dra446->mmc_pbase = cfg->IOPort_Base[0];
	dra446->chrdma    = cfg->DMALst[0];
	dra446->chtdma    = cfg->DMALst[1];

	if ((dra446->mmc_base =
			mmap_device_io(DRA446_SDC_SIZE, dra446->mmc_pbase))
				== (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io failed");
		goto fail0;
	}

	if ((dra446->dma_base = 
			mmap_device_io(DRA446_EDMA_SIZE, cfg->IOPort_Base[1])) 
				== (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io failed");
		goto fail1;
	}

	/*
	 * Make sure EDMA region 0 is enabled for MMC
	 * FIXME!!! This should be moved to startup
	 */
	reg = in32(dra446->dma_base + 0x340);
	mask = (1 << dra446->chrdma) | (1 << dra446->chtdma);
	if ((reg & mask) != mask) {
		InterruptDisable();
		out32(dra446->dma_base + 0x340, in32(dra446->dma_base + 0x340) | mask);
		InterruptEnable();
	}
	
	// Capability
	ext->hccap |= MMC_HCCAP_BW1 | MMC_HCCAP_BW4;	// 1 bit / 4 bits bus supported
	ext->hccap |= MMC_HCCAP_DMA | MMC_HCCAP_33V;
//	ext->hccap |= MMC_HCCAP_HS;

	if ((dra446->mmc_clock = dra446_get_syspage_clk()) == 0)
		dra446->mmc_clock = 567000000 / 6;

	ext->hba       = hba;
	ext->handle    = dra446;
	ext->clock     = dra446->mmc_clock / 2;	// Maximum clock rate
	ext->detect    = dra446_detect;
	ext->powerup   = dra446_powerup;
	ext->powerdown = dra446_powerdown;
	ext->command   = dra446_command;
	ext->cfg_bus   = dra446_cfg_bus;
	ext->set_clock = dra446_clock;
	ext->set_blksz = dra446_block_size;
	ext->interrupt = dra446_interrupt;
	ext->setup_dma = dra446_setup_dma;
	ext->dma_done  = dra446_dma_done;
	ext->setup_pio = dra446_setup_pio;
	ext->pio_done  = dra446_pio_done;
	ext->shutdown  = dra446_shutdown;

	if (!cfg->Description[0])
		strncpy(cfg->Description, "TI DRA446 SDMMC", sizeof(cfg->Description));

	return (MMC_SUCCESS);

fail0:
	free(dra446);
fail1:
	munmap_device_io(dra446->mmc_base, DRA446_SDC_SIZE);

	return (MMC_FAILURE);
}

#endif


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_dra446.c $ $Rev: 657836 $" );
