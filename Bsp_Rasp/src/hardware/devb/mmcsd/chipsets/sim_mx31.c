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
 * In practice all cards are formatted for PC compatibility, with a partition 
 * table in the first block and a single FAT file system on the rest of the card.
 * Please ensure that the card is formatted this way.
*/ 

#include	<sim_mmc.h>

#ifdef MMCSD_VENDOR_FREESCALE_MX31

#include	<sim_mx31.h>

static int mx31_busy_done(mx31_ext_t *mx31)
{
	uint32_t	base, shift;
	int			to = IMX31_MMC_TMOUT * 100;

#define SWMUX_SD1_DATA0		16
#define	INPUTCONFIG_GPIO 	1
#define	INPUTCONFIG_FUNC	2
#define	OUTPUTCONFIG_GPIO	0
#define	OUTPUTCONFIG_FUNC	1

	base  = mx31->iomux_base + MX31_IOMUX_SWMUX + (SWMUX_SD1_DATA0 & ~0x03);
	shift = (SWMUX_SD1_DATA0 % 4) * 8;

	// Set SD_DATA0 pin in GPIO mode.
	out32(base, (in32(base) & ~(0xff << shift)) | ((INPUTCONFIG_GPIO | (OUTPUTCONFIG_GPIO << 4)) << shift));

	// Ensure GPIO pin direction is input
	out32(mx31->gpio2_base + MX31_GPIO_GDIR, in32(mx31->gpio2_base + MX31_GPIO_GDIR) & ~IMX31_SDDATA0_GPIO);

	// Wait till SD_DATA0 pin is pulled down by the card.
	while (((in32(mx31->gpio2_base) & IMX31_SDDATA0_GPIO) == 0) && to > 0) {
		nanospin_ns(100);
		--to;
	}

	out32(base, (in32(base) & ~(0xff << shift)) | ((INPUTCONFIG_FUNC | (OUTPUTCONFIG_FUNC << 4)) << shift));

	if (to == 0) {
		slogf(_SLOGC_SIM_MMC, _SLOG_ERROR, "mx31: Busy done timed out");
		return (MMC_FAILURE);
	}

	return (MMC_SUCCESS);
}

static int mx31_interrupt(SIM_HBA *hba, int irq, int resp_type, uint32_t *resp)
{
	int				intr = 0;
	uint32_t		sts, mask;
	SIM_MMC_EXT		*ext = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;
	imx31_mmc_reg	*mmc = (imx31_mmc_reg *)mx31->mmc_base;

	sts  = mmc->sd_status;
	mask = mmc->sd_int_cntr;

	if (sts & IMX31_SDSTATUS_ENDCMDRES) {		// command response done?
		if (sts & IMX31_SDSTATUS_TIMEOUTRESP)	// Command Timeout ?
			intr |= MMC_ERR_CMD_TO | MMC_INTR_ERROR | MMC_INTR_COMMAND;

		if (sts & IMX31_SDSTATUS_RESPCRCERR)	// Command response crc error?
			intr |= MMC_ERR_CMD_CRC | MMC_INTR_ERROR | MMC_INTR_COMMAND;

		if (mask & IMX31_SDINTCNTR_ENDCMDRES) 	// Only report command done if necessary
			intr |= MMC_INTR_COMMAND;

		if (!(intr & MMC_INTR_ERROR)) {			// Only grab the response if there is no error
			if (resp_type & MMC_RSP_136) {
				resp[3]  = mmc->sd_res_fifo << 16;
				resp[3] |= mmc->sd_res_fifo;
				resp[2]  = mmc->sd_res_fifo << 16;
				resp[2] |= mmc->sd_res_fifo;
				resp[1]  = mmc->sd_res_fifo << 16;
				resp[1] |= mmc->sd_res_fifo;
				resp[0]  = mmc->sd_res_fifo << 16;
				resp[0] |= mmc->sd_res_fifo;
			} else if (resp_type & MMC_RSP_PRESENT) {
				resp[0] = mmc->sd_res_fifo;
				resp[0] <<= 24;
				resp[1] = mmc->sd_res_fifo; 
				resp[1] <<= 8;
				resp[0] |= resp[1];
				resp[1] = mmc->sd_res_fifo;
				resp[1] >>= 8;
				resp[0] |= resp[1];
				resp[1] = 0;
			}

			// Busy check?
			if (resp_type & MMC_RSP_BUSY) {
				if (mx31_busy_done(mx31) != MMC_SUCCESS)
					intr |= MMC_ERR_CMD_TO | MMC_INTR_ERROR | MMC_INTR_COMMAND;
			}
		}
	}

	/*
	 * Check for data related interrupts
	 */
	if (sts & IMX31_SDSTATUS_READOPDONE) {
		intr |= MMC_INTR_DATA;
		if (sts & IMX31_SDSTATUS_TIMEOUTREAD)
			intr |= MMC_ERR_DATA_TO | MMC_INTR_ERROR;
		if (sts & IMX31_SDSTATUS_READCRCERR)
			intr |= MMC_ERR_DATA_CRC | MMC_INTR_ERROR;

		mmc->sd_int_cntr = 0;
	}

	if (sts & IMX31_SDSTATUS_WRITEOPDONE) {
		intr |= MMC_INTR_DATA;
		if (sts & IMX31_SDSTATUS_WRITECRCERR)
			intr |= MMC_ERR_DATA_CRC | MMC_INTR_ERROR;

		mmc->sd_int_cntr = 0;
	}

	if (sts & IMX31_SDSTATUS_BUFREADREADY) {
		if (mask & IMX31_SDINTCNTR_BUFREADEN) {
			intr |= MMC_INTR_RBRDY;
			mmc->sd_int_cntr = 0;
		}
	}

	if (sts & IMX31_SDSTATUS_BUFWRITEREADY)	{
		if (mask & IMX31_SDINTCNTR_BUFWRITEEN) {
			intr |= MMC_INTR_WBRDY;
			mmc->sd_int_cntr = IMX31_SDINTCNTR_WRITEOPDONE;
		}
	}

	// clear interrupt status
   	mmc->sd_status = sts;

	if (intr & MMC_INTR_ERROR) {
		slogf(_SLOGC_SIM_MMC, _SLOG_INFO, "mx31_interrupt: Error bits set in status:sts:0x%X for cmd: %d", sts, mmc->sd_cmd);

		mmc->sd_int_cntr = 0;

		// Abort pending DMA
		if (mx31->dma_pend != NULL) {
			mx31->sdmafuncs.xfer_abort(mx31->dma_pend);
			mx31->dma_pend = NULL;
		}
	}

    return intr;
}

static int mx31_command(SIM_HBA *hba, mmc_cmd_t *cmd)
{
	SIM_MMC_EXT		*ext = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;
	imx31_mmc_reg	*mmc = (imx31_mmc_reg *)mx31->mmc_base;
	uint32_t		cmddatcntl = mx31->bus_width;
	uint32_t		mask;

   	if (cmd->resp & MMC_RSP_PRESENT) {
		if (cmd->resp & MMC_RSP_136)
			cmddatcntl |= IMX31_SDCMDDATCONT_FORMATOFRESPONSE1;
		else
			cmddatcntl |= IMX31_SDCMDDATCONT_FORMATOFRESPONSE;
		if (cmd->resp == MMC_RSP_R3)
			cmddatcntl |= IMX31_SDCMDDATCONT_FORMATOFRESPONSE2;   	
	}

	mask = IMX31_SDINTCNTR_ENDCMDRES;

	if (cmd->eflags & MMC_CMD_DATA) {
		mask |= mx31->dmask;

		cmddatcntl |= IMX31_SDCMDDATCONT_DATAENABLE;

		if ((cmd->eflags & MMC_CMD_DATA_IN) == 0)
			cmddatcntl |= IMX31_SDCMDDATCONT_WRITEREAD;
	}

	if (cmd->eflags & MMC_CMD_INIT)				// initialization clock sequence
		cmddatcntl |= IMX31_SDCMDDATCONT_INIT;

	mmc->sd_cmd = cmd->opcode;
	mmc->sd_arg = cmd->argument;
	mmc->sd_status = 0xFFFFFFFF;
	mmc->sd_int_cntr = mask;
	mmc->sd_cmd_dat_cont = cmddatcntl;	 

    return (MMC_SUCCESS);
}

static int mx31_set_clock(mx31_ext_t *mx31, int divisor)
{
	imx31_mmc_reg	 *mmc = (imx31_mmc_reg *)mx31->mmc_base;
	int				to;

	if (mmc->sd_status & 0x100) {
		mmc->sd_str_stp_clk = 1;

		for (to = IMX31_MMC_TMOUT; to > 0; to--) {
			if ((mmc->sd_status & 0x100) == 0)
				break;
			nanospin_ns(100);;
		}

		if (to == 0) {
			slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: mx31_set_clock(): timeout to get SD clock stopped");	
			return -1;
		}
	}

	mmc->sd_clk_rate = divisor;
	mmc->sd_str_stp_clk = 2;

	for (to = IMX31_MMC_TMOUT; to > 0; to--) {
		if (mmc->sd_status & 0x100)
			return 0;
		nanospin_ns(100);;
	}

	return -1;
}

static int mx31_powerdown(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;
	imx31_mmc_reg	*mmc = (imx31_mmc_reg *)mx31->mmc_base;
	int				i;

	mmc->sd_int_cntr = 0;			// clear interrupt control register
	mmc->sd_status = 0xC0007E2F;	// clear all status bits which are w1c

   	// reset the SDHC module
	mmc->sd_str_stp_clk = 8;
	mmc->sd_str_stp_clk = 9;
	for (i = 0; i < 8 ; ++i)
		mmc->sd_str_stp_clk = 1;
	delay(10);

	return (MMC_SUCCESS);
}

/*
 * Reset host controller and card
 * The clock should be enabled and set to minimum (<400KHz)
 */
static int mx31_powerup(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;
	imx31_mmc_reg	*mmc = (imx31_mmc_reg *)mx31->mmc_base;

	mx31_powerdown(hba);

	// set the initialization mode clock
	if (mx31_set_clock(mx31, 0x40 << 4) != 0) 
		return (MMC_FAILURE);

	mmc->sd_read_to = 0x2DB4;	// read timeout as given in imx31 spec.
	mmc->sd_res_to  = 0xFF;		// response timeout maximum

	return (MMC_SUCCESS);
}

static int mx31_detect(SIM_HBA *hba)
{
	return (MMC_SUCCESS);
}

static int mx31_shutdown(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;

	mx31_powerdown(hba);

	if (ext->hccap & MMC_HCCAP_DMA) {
		mx31->sdmafuncs.channel_release(mx31->rdma_chn);
		mx31->sdmafuncs.channel_release(mx31->wdma_chn);
		mx31->sdmafuncs.fini();
	}

	munmap_device_io(mx31->gpio2_base, 0x4000);
	munmap_device_io(mx31->gpio3_base, 0x4000);
	munmap_device_io(mx31->iomux_base, 0x4000);
	munmap_device_io(mx31->mmc_base, 0x4000);
	free(mx31);

	return (MMC_SUCCESS);
}

/*
 * setup PIO transfer
 */
static int mx31_setup_pio(SIM_HBA *hba, int len, int dir)
{
	SIM_MMC_EXT		*ext = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;
	imx31_mmc_reg	*mmc = (imx31_mmc_reg *)mx31->mmc_base;
	uint32_t		blkcnt;
  
	if (dir == MMC_DIR_IN)
		mx31->dmask = IMX31_SDINTCNTR_BUFREADEN;
	else
		mx31->dmask = IMX31_SDINTCNTR_BUFWRITEEN;

	mmc->sd_blk_len = mx31->blksz;
	mmc->sd_nob = blkcnt = len / mx31->blksz;

	return (blkcnt * mx31->blksz);
}

static int mx31_pio_done(SIM_HBA *hba, char *buf, int len, int dir)
{
	uint32_t		cnt, i, *pbuf = (uint32_t *)buf;
	SIM_MMC_EXT		*ext = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;
	imx31_mmc_reg	*mmc = (imx31_mmc_reg *)mx31->mmc_base;

	cnt = mx31->bus_width == 0 ? IMX31_SDDATA_BUFFER_SIZE / 4 : IMX31_SDDATA_BUFFER_SIZE;
	if (len < cnt)
		cnt = len;

	if (dir == MMC_DIR_IN) {
		for (i = 0; (mmc->sd_status & IMX31_SDSTATUS_BUFREADREADY) == 0; i++) {
			if (i > 200000) {
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, " pio read: timedout for BUFREADREADY in present state register");
				return (0);
			}
			nanospin_ns(100);
		}

		for (i = 0; i < cnt; i += 4)
			*pbuf++ = mmc->sd_buffer_access;
	} else {
	    for (i = 0; (mmc->sd_status & IMX31_SDSTATUS_BUFWRITEREADY) == 0; i++) {
			if (i > 200000) {
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, " pio write: timedout for BUFWRITEREADY in present state register");
				return (0);
			}
			nanospin_ns(100);
		}

		for (i = 0; i < cnt; i += 4)
			mmc->sd_buffer_access = *pbuf++;
	}

	return cnt;
}

/*
 * setup DMA transfer
 */
static int mx31_setup_dma(SIM_HBA *hba, paddr_t paddr, int len, int dir)
{
	SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;
	dma_transfer_t	tinfo;
	dma_addr_t		dma_addr;
	void			*chnl;

	if ((len = mx31_setup_pio(hba, len, dir)) <= 0)
		return len;

	tinfo.xfer_unit_size = 32;
	tinfo.xfer_bytes = dma_addr.len = len;
	dma_addr.paddr = paddr;

	if (dir == MMC_DIR_IN) {
		tinfo.src_addrs = NULL;
		tinfo.dst_addrs = &dma_addr;

		tinfo.dst_fragments = 1;
		chnl = mx31->rdma_chn;

		mx31->dmask = IMX31_SDINTCNTR_READOPDONE;
	} else {
		tinfo.src_addrs = &dma_addr;
		tinfo.dst_addrs = NULL;

		tinfo.src_fragments = 1;
		chnl = mx31->wdma_chn;

		mx31->dmask = IMX31_SDINTCNTR_WRITEOPDONE;
	}

	mx31->sdmafuncs.setup_xfer(chnl, &tinfo);
	mx31->sdmafuncs.xfer_start(chnl);

	mx31->dma_pend = chnl;

	return len;
}

static int mx31_dma_done(SIM_HBA *hba, int dir)
{
	SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;
	void			*chnl;

	if (dir == MMC_DIR_IN) {
		chnl = mx31->rdma_chn;
		// Make sure the DMA transfer is done
		while (mx31->sdmafuncs.bytes_left(chnl))
			;
	} else
		chnl = mx31->wdma_chn;

	mx31->sdmafuncs.xfer_complete(chnl);
	mx31->dma_pend = NULL;

	return MMC_SUCCESS;
}

static int mx31_cfg_bus(SIM_HBA *hba, int width, int mmc)
{
	SIM_MMC_EXT	*ext;
	mx31_ext_t	*mx31;

	ext  = (SIM_MMC_EXT *)hba->ext;
	mx31 = (mx31_ext_t *)ext->handle;

	if (width == 4)
		mx31->bus_width = 2 << 8;
	else
		mx31->bus_width = 0;

	return (MMC_SUCCESS);
}

static int mx31_clock(SIM_HBA *hba, int *clock, int high_speed)
{
	SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;
	int				clk = *clock;
	uint32_t		clk_rate;
	int				clk_div = 66500000;		// FIXME!! should get from system page

	clk_rate = clk_div / clk;

	if (clk_rate <= 1) {
		clk_rate = 1;
		*clock = clk_div >> 1;
	} else if (clk_rate > 15) {
		clk_div >>= 1;
		for (clk_rate = 0x10; clk_rate <= 0x800; clk_rate *= 2) {
			if ((clk = clk_div / clk_rate / 2) < *clock) {
				*clock = clk;
				clk_rate = (clk_rate << 4) | 1;
				break;
			}
		}
	} else
		*clock = clk_div / (clk_rate + 1);

	mx31_set_clock(mx31, clk_rate);

	return (MMC_SUCCESS);
}

static int mx31_block_size(SIM_HBA *hba, int blksz)
{
	SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *)hba->ext;
	mx31_ext_t		*mx31 = (mx31_ext_t *)ext->handle;

	if (blksz > 4095)
		return (MMC_FAILURE);

	mx31->blksz = blksz;

	return (MMC_SUCCESS);
}

int	mx31_attach(SIM_HBA *hba)
{
	CONFIG_INFO		*cfg;
	SIM_MMC_EXT		*ext;
	mx31_ext_t		*mx31;
	unsigned 		channel;
	char			opt[64];

	cfg = (CONFIG_INFO *)&hba->cfg;
	ext = (SIM_MMC_EXT *)hba->ext;

	if ((mx31= calloc(1, sizeof(mx31_ext_t))) == NULL)
		return (NULL);

	if (!cfg->NumIOPorts) {
		cfg->IOPort_Base[0]   = 0x50004000;
		cfg->IOPort_Length[0] = 0x4000;
		cfg->NumIOPorts = 1;
	}

	if (!cfg->NumIRQs) {
		cfg->IRQRegisters[0] = 9;
		cfg->NumIRQs = 1;
	}

	if (!cfg->NumDMAs) {
		cfg->DMALst[0] = 20;
		cfg->NumDMAs = 1;
	}

	mx31->mmc_pbase = cfg->IOPort_Base[0];
	if ((mx31->mmc_base =
			mmap_device_io(0x4000, mx31->mmc_pbase)) == (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io failed");
		goto fail1;
	}

	if ((mx31->gpio2_base =
			mmap_device_io(0x4000, MX31_GPIO2_BASE)) == (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io failed");
		goto fail2;
	}

	if ((mx31->gpio3_base =
			mmap_device_io(0x4000, MX31_GPIO3_BASE)) == (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io failed");
		goto fail2;
	}

	if ((mx31->iomux_base =
			mmap_device_io(0x4000, MX31_IOMUXC_BASE)) == (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io failed");
		goto fail3;
	}

	if (!cfg->Description[0])
		strncpy(cfg->Description, "FREESCALE mx31 SDMMC", sizeof(cfg->Description));

	// Capability
	ext->hccap |= MMC_HCCAP_BW1 | MMC_HCCAP_BW4;	// 1 bit / 4 bits bus supported
	ext->hccap |= MMC_HCCAP_33V;

	mx31->clock = 20000000;
 
	ext->hba       = hba;
	ext->handle    = mx31;
	ext->clock     = mx31->clock;		// Maximum clock
	ext->detect    = mx31_detect;
	ext->powerup   = mx31_powerup;
	ext->powerdown = mx31_powerdown;
	ext->command   = mx31_command;
	ext->cfg_bus   = mx31_cfg_bus;
	ext->set_clock = mx31_clock;
	ext->set_blksz = mx31_block_size;
	ext->interrupt = mx31_interrupt;
	ext->setup_pio = mx31_setup_pio;
	ext->pio_done  = mx31_pio_done;
	ext->shutdown  = mx31_shutdown;

	if (get_dmafuncs(&mx31->sdmafuncs, sizeof(dma_functions_t)) == -1)
		return (MMC_SUCCESS);

	if (mx31->sdmafuncs.init(NULL) == -1)
		return (MMC_SUCCESS);

	sprintf(opt, "eventnum=%ld,watermark=64,fifopaddr=0x%x\n", cfg->DMALst[0], mx31->mmc_pbase + 0x38);

	channel = 4;	// SDMA_CHTYPE_SHP_2_MCU
	if ((mx31->rdma_chn = mx31->sdmafuncs.channel_attach(opt, NULL, &channel, 1, 0)) == NULL)
		goto dma_fail1;

	channel = 3;	// SDMA_CHTYPE_MCU_2_SHP
	if ((mx31->wdma_chn = mx31->sdmafuncs.channel_attach(opt, NULL, &channel, 1, 0)) == NULL)
		goto dma_fail2;

	ext->hccap |= MMC_HCCAP_DMA;
	ext->setup_dma = mx31_setup_dma;
	ext->dma_done  = mx31_dma_done;

	return (MMC_SUCCESS);

dma_fail2:
	mx31->sdmafuncs.channel_release(mx31->rdma_chn);
dma_fail1:
	mx31->sdmafuncs.fini();

	return (MMC_SUCCESS);

fail3:
	munmap_device_io(mx31->gpio2_base, 0x4000);
	munmap_device_io(mx31->gpio3_base, 0x4000);
fail2:
	munmap_device_io(mx31->mmc_base, 0x4000);
fail1:	
	free(mx31);
	slogf(_SLOGC_SIM_MMC, _SLOG_INFO, " exiting mx31_attach with filure");
	return (MMC_FAILURE);
}

#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_mx31.c $ $Rev: 657836 $" );

