/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. 
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

#ifdef		MMCSD_VENDOR_FREESCALE_MPC5125

#include	<sim_mpc5125.h>

#define		MAX_DMA_WRITE	512
#define		MMC_TIMER		6

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static	int	mpc5125_set_clock (mpc5125_ext_t *ext, int divisor)

{
mpc5125_mmc_reg	*mmc = (mpc5125_mmc_reg *) ext->mmc_base;
int				to;

	if (mmc->sd_status & 0x100) {
		mmc->sd_str_stp_clk = 1;

		for (to = MPC5125_MMC_TMOUT; to > 0; to--) {
			if ((mmc->sd_status & 0x100) == 0)
				break;
			nanospin_ns(100);;
			}

		if (to == 0) {
			slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: mpc5125_set_clock(): timeout to get SD clock stopped");
			return -1;
			}
		}

	if (divisor < 8) {
		divisor = 0x80;
		}
	mmc->sd_clk_rate = divisor;
	mmc->sd_str_stp_clk = 2;

	for (to = MPC5125_MMC_TMOUT; to > 0; to--) {
		if (mmc->sd_status & 0x100) {
			return (MMC_SUCCESS);
			}
		nanospin_ns(100);;
		}

	return (-1);
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static int mpc5125_clock (SIM_HBA *hba, int *clock, int high_speed)

{
SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *) hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *) ext->handle;
int				clk = *clock;
uint32_t		clk_rate;
int				clk_div = 100000000;		// FIXME!! should get from system page

	clk_rate = clk_div / clk;

	if (clk_rate <= 1) {
		clk_rate = 1;
		*clock = clk_div >> 1;
		}
	else
		if (clk_rate > 15) {
			clk_div >>= 1;
			for (clk_rate = 0x10; clk_rate <= 0x800; clk_rate *= 2) {
				if ((clk = clk_div / clk_rate / 2) < *clock) {
					*clock = clk;
					clk_rate = (clk_rate << 4) | 1;
					break;
					}
				}
			}
		else
			*clock = clk_div / (clk_rate + 1);

	mpc5125_set_clock (mpc5125, clk_rate);

	return (MMC_SUCCESS);
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static int mpc5125_powerdown (SIM_HBA *hba)

{
SIM_MMC_EXT		*ext = (SIM_MMC_EXT *) hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *)ext->handle;
mpc5125_mmc_reg	*mmc = (mpc5125_mmc_reg *) mpc5125->mmc_base;
int				i;

	mmc->sd_int_cntr = 0;			// clear interrupt control register
	mmc->sd_status = 0xC0007E2F;	// clear all status bits which are w1c

   	// reset the SDHC module
	mmc->sd_str_stp_clk = 8;
	mmc->sd_str_stp_clk = 9;
	for (i = 0; i < 8 ; ++i)
		mmc->sd_str_stp_clk = 1;
	delay (10);

	return (MMC_SUCCESS);
}

/***************************************************************************/
/* Reset host controller and card                                          */
/* The clock should be enabled and set to minimum (<400KHz)                */
/***************************************************************************/

static int mpc5125_powerup(SIM_HBA *hba)

{
SIM_MMC_EXT		*ext = (SIM_MMC_EXT *) hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *) ext->handle;
mpc5125_mmc_reg	*mmc = (mpc5125_mmc_reg *) mpc5125->mmc_base;

	mpc5125_powerdown (hba);

	// set the initialization mode clock
	if (mpc5125_set_clock (mpc5125, 0x401) != 0) /* 390Khz */
		return (MMC_FAILURE);

	mmc->sd_read_to = 0xffff;	// read timeout as given in mpc5125 spec.
	mmc->sd_res_to  = 0xFF;		// response timeout maximum

	return (MMC_SUCCESS);
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static int mpc5125_interrupt (SIM_HBA *hba, int irq, int resp_type, uint32_t *resp)

{
int				intr = 0;
uint32_t		sts, mask;
SIM_MMC_EXT		*ext = (SIM_MMC_EXT *) hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *) ext->handle;
mpc5125_mmc_reg	*mmc = (mpc5125_mmc_reg *) mpc5125->mmc_base;

	sts  = mmc->sd_status;
	mask = mmc->sd_int_cntr;

#if	0
	if (sts & (STATUS_CARDINSERTION | STATUS_CARDREMOVAL)) {
		mmc->sd_status = 0;
		mpc5125->card_pres = (sts & STATUS_CARDINSERTION) ? 1 : 0;
		if (mpc5125->card_pres) {
			uint8_t	ioc;
			ioc = in8 (mpc5125->ioc_base + 0x74);
			ioc |= (IOCTL_PUD | IOCTL_PUE);
			out8 (mpc5125->ioc_base +0x74, ioc);
			}
		return (MMC_INTR_CARD);
		}
#endif

	if (sts & STATUS_ENDCMDRES) {		// command response done?
		if (sts & STATUS_TIMEOUTRESP)	// Command Timeout ?
			intr |= MMC_ERR_CMD_TO | MMC_INTR_ERROR | MMC_INTR_COMMAND;

		if (sts & STATUS_RESPCRCERR)	// Command response crc error?
			intr |= MMC_ERR_CMD_CRC | MMC_INTR_ERROR | MMC_INTR_COMMAND;

		if (mask & INTCNTR_ENDCMDRES) 	// Only report command done if necessary
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
			}
		}

	/*
	 * Check for data related interrupts
	 */
	if (sts & STATUS_READOPDONE) {
		intr |= MMC_INTR_DATA;
		if (sts & STATUS_TIMEOUTREAD)
			intr |= MMC_ERR_DATA_TO | MMC_INTR_ERROR;
		if (sts & STATUS_READCRCERR)
			intr |= MMC_ERR_DATA_CRC | MMC_INTR_ERROR;

		mmc->sd_int_cntr = 0;
		}

	if (sts & STATUS_WRITEOPDONE) {
		intr |= MMC_INTR_DATA;
		if (sts & STATUS_WRITECRCERR)
			intr |= MMC_ERR_DATA_CRC | MMC_INTR_ERROR;

		mmc->sd_int_cntr = 0;
		}

	if (sts & STATUS_BUFREADREADY) {
		if (mask & INTCNTR_BUFREADEN) {
			intr |= MMC_INTR_RBRDY;
			mmc->sd_int_cntr = 0;
			}
		}

	if (sts & STATUS_BUFWRITEREADY)	{
		if (mask & INTCNTR_BUFWRITEEN) {
			intr |= MMC_INTR_WBRDY;
			mmc->sd_int_cntr = INTCNTR_WRITEOPDONE;
			}
		}

	// clear interrupt status
   	mmc->sd_status = sts;

	if (intr & MMC_INTR_ERROR) {
		slogf(_SLOGC_SIM_MMC, _SLOG_INFO, "mpc5125_interrupt: Error bits set in status:sts:0x%X for cmd: %d", sts, mmc->sd_cmd);

		mmc->sd_int_cntr = 0;

		// Abort pending DMA
		if (mpc5125->dma_pend) {
			mpc5125->sdmafuncs.xfer_abort (mpc5125->dma_chn);
			mpc5125->dma_pend = 0;
			slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "DMA Abort");
			}
		}

    return (intr);
}

/***************************************************************************/
/* setup PIO transfer                                                      */
/***************************************************************************/

static int mpc5125_setup_pio (SIM_HBA *hba, int len, int dir)

{
SIM_MMC_EXT		*ext = (SIM_MMC_EXT *) hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *) ext->handle;
mpc5125_mmc_reg	*mmc = (mpc5125_mmc_reg *) mpc5125->mmc_base;
uint32_t		blkcnt;
  
	if (dir == MMC_DIR_IN)
		mpc5125->dmask = INTCNTR_BUFREADEN;
	else
		mpc5125->dmask = INTCNTR_BUFWRITEEN;

	mmc->sd_blk_len = mpc5125->blksz;
	mmc->sd_nob = blkcnt = len / mpc5125->blksz;

	return (blkcnt * mpc5125->blksz);
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static int mpc5125_pio_done (SIM_HBA *hba, char *buf, int len, int dir)

{
uint32_t		cnt, i, *pbuf = (uint32_t *) buf;
SIM_MMC_EXT		*ext = (SIM_MMC_EXT *) hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *) ext->handle;
mpc5125_mmc_reg	*mmc = (mpc5125_mmc_reg *) mpc5125->mmc_base;

	cnt = mpc5125->bus_width == 0 ? SDDATA_BUFFER_SIZE / 4 : SDDATA_BUFFER_SIZE;
	if (len < cnt)
		cnt = len;

	if (dir == MMC_DIR_IN) {
		for (i = 0; (mmc->sd_status & STATUS_BUFREADREADY) == 0; i++) {
			if (i > 200000) {
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, " pio read: timedout for BUFREADREADY in present state register");
				return (0);
				}
			nanospin_ns(100);
			}

		for (i = 0; i < cnt; i += 4, pbuf++) {
			*pbuf = mmc->sd_buffer_access;
			*pbuf = ENDIAN_LE32 (*pbuf);
			}
	} else {
	    for (i = 0; (mmc->sd_status & STATUS_BUFWRITEREADY) == 0; i++) {
			if (i > 200000) {
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, " pio write: timedout for BUFWRITEREADY in present state register");
				return (0);
				}
			nanospin_ns(100);
			}

		for (i = 0; i < cnt; i += 4, pbuf++)
			mmc->sd_buffer_access = ENDIAN_LE32 (*pbuf);
		}

	return cnt;
}

/***************************************************************************/
/* Setup DMA transfer                                                      */
/***************************************************************************/

static int mpc5125_setup_dma(SIM_HBA *hba, paddr_t paddr, int len, int dir)

{
SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *) hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *) ext->handle;
//dma_transfer_t	tinfo;
//dma_addr_t		src_dma_addr, dst_dma_addr;
void			*chnl;
//int				ret;
int				i, xfer_len;
uint32_t		*p = (uint32_t *) (mpc5125->dma_base + 0x13c0);
uint32_t		*q;

	if ((mpc5125->vaddr = mmap (NULL, len, PROT_NOCACHE | PROT_READ | PROT_WRITE, MAP_PHYS, NOFD, paddr)) == MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_DEBUG1, "%s mmap failed", __FUNCTION__);
		return (-1);
		}
	mpc5125->paddr = paddr;
	if (mpc5125->len == 0)
		mpc5125->len = len;
	if (dir == MMC_DIR_OUT) {
		xfer_len = (len > mpc5125->dmasize) ? mpc5125->dmasize : len;
		for (i = 0, q = mpc5125->vaddr; i < (xfer_len / 4); i++, q++)
			*q = ENDIAN_LE32 (*q);
		}
	else {
		xfer_len = len;
		}
	if ((xfer_len = mpc5125_setup_pio (hba, xfer_len, dir)) <= 0)
		return (xfer_len);

//	memset ((char *) &tinfo, 0, sizeof (dma_transfer_t));
//	tinfo.xfer_unit_size = 4;
//	tinfo.xfer_unit_size = 32;
//	tinfo.xfer_bytes = len;
//	tinfo.mode_flags = DMA_MODE_FLAG_BURST;
	chnl = mpc5125->dma_chn;

	/* We setup the TCD ourselves, as there is a problem with the DMA library */

	if (dir == MMC_DIR_IN) {
		*p++ = mpc5125->mmc_pbase + DBA;
		*p++ = 0x02020000;
		*p++ = 0x00000040;
		*p++ = 0x00000000;
		*p++ = paddr;
		*p++ = ((xfer_len / 64) << 16) | 4;
		*p++ = ~xfer_len + 1;
		*p++ = ((xfer_len / 64) << 16) | 2;
		out8 (mpc5125->dma_base + 0x18, 30);

//		dst_dma_addr.paddr = paddr;
//		dst_dma_addr.len = len;
//		src_dma_addr.paddr = mpc5125->mmc_pbase + DBA;
//		src_dma_addr.len = 4;
//		tinfo.src_addrs = &src_dma_addr;
//		tinfo.dst_addrs = &dst_dma_addr;

//		tinfo.src_fragments = 1;
//		tinfo.dst_fragments = 8;

		mpc5125->dmask = INTCNTR_READOPDONE;
//		tinfo.src_flags = DMA_ADDR_FLAG_MEMORY | DMA_ADDR_FLAG_NO_INCREMENT;
//		tinfo.dst_flags = DMA_ADDR_FLAG_MEMORY | DMA_ADDR_FLAG_SEGMENTED;
		}
	else {
		*p++ = paddr;
		*p++ = 0x02020004;
		*p++ = 0x00000040;
		*p++ = ~xfer_len + 1;
		*p++ = mpc5125->mmc_pbase + DBA;
		*p++ = ((xfer_len / 64) << 16);
		*p++ = 0x00000000;
		*p++ = ((xfer_len / 64) << 16) | 2;
		out8 (mpc5125->dma_base + 0x18, 30);

//		src_dma_addr.paddr = paddr;
//		src_dma_addr.len = len;
//		dst_dma_addr.paddr = mpc5125->mmc_pbase + DBA;
//		dst_dma_addr.len = 4;
//		tinfo.src_addrs = &src_dma_addr;
//		tinfo.dst_addrs = &dst_dma_addr;

//		tinfo.dst_fragments = 1;
//		tinfo.src_fragments = 1;

		mpc5125->dmask = INTCNTR_WRITEOPDONE;
//		tinfo.dst_flags = DMA_ADDR_FLAG_DEVICE | DMA_ADDR_FLAG_NO_INCREMENT;
//		tinfo.src_flags = DMA_ADDR_FLAG_MEMORY;
		}

//	if ((ret = mpc5125->sdmafuncs.setup_xfer (chnl, &tinfo)) != 0) {
//		slogf (_SLOGC_SIM_MMC, _SLOG_DEBUG1, "dma setup_xfer failed %d - %s", ret, strerror (errno));
//		return (ret);
//		}
//	(dma_channel_t *) chnl->ctrl = DMA_CAP_DEVOCE_TO_MEMORY;
//	if ((ret = mpc5125->sdmafuncs.xfer_start (chnl)) != 0) {
//		slogf (_SLOGC_SIM_MMC, _SLOG_DEBUG1, "dma xfer_start failed %d", ret);
//		return (ret);
//		}
	mpc5125->dma_pend = chnl;

	return (xfer_len);
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static int mpc5125_dma_done (SIM_HBA *hba, int dir)

{
SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *)hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *)ext->handle;
void			*chnl;
int				i;

	chnl = mpc5125->dma_chn;
	if (dir == MMC_DIR_IN) {
		uint32_t	*p = mpc5125->vaddr;
		for (i = 0; i < (mpc5125->len / 4); i++, p++)
			*p = ENDIAN_LE32 (*p);
		mpc5125->len = 0;
		// Make sure the DMA transfer is done
		}
	else {
		if (mpc5125->len >= mpc5125->dmasize)
			mpc5125->len -= mpc5125->dmasize;
		else
			mpc5125->len = 0;
		if (mpc5125->len) {
			mpc5125->paddr += (mpc5125->len > mpc5125->dmasize) ? mpc5125->dmasize : mpc5125->len;
			}
		}

	mpc5125->sdmafuncs.xfer_complete (chnl);
	mpc5125->dma_pend = NULL;

	return (MMC_SUCCESS);
}


/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static int mpc5125_command (SIM_HBA *hba, mmc_cmd_t *cmd)

{
SIM_MMC_EXT		*ext = (SIM_MMC_EXT *)hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *)ext->handle;
mpc5125_mmc_reg	*mmc = (mpc5125_mmc_reg *) mpc5125->mmc_base;
uint32_t		cmddatcntl = mpc5125->bus_width;
uint32_t		mask;

   	if (cmd->resp & MMC_RSP_PRESENT) {
		if (cmd->resp & MMC_RSP_136)
			cmddatcntl |= CMDDATCONT_FORMATOFRESPONSE1;
		else
			cmddatcntl |= CMDDATCONT_FORMATOFRESPONSE;
		if (cmd->resp == MMC_RSP_R3)
			cmddatcntl |= CMDDATCONT_FORMATOFRESPONSE2;   	
		}

	mask = INTCNTR_ENDCMDRES;

	if (cmd->eflags & MMC_CMD_DATA) {
		mask |= mpc5125->dmask;

		cmddatcntl |= CMDDATCONT_DATAENABLE;

		if ((cmd->eflags & MMC_CMD_DATA_IN) == 0)
			cmddatcntl |= CMDDATCONT_WRITEREAD;
		}

	if (cmd->eflags & MMC_CMD_INIT)				// initialization clock sequence
		cmddatcntl |= CMDDATCONT_INIT;

	mmc->sd_arg = cmd->argument;
	mmc->sd_cmd = cmd->opcode;
	mmc->sd_status = 0xFFFFFFFF;
	mmc->sd_int_cntr = mask;
	mmc->sd_cmd_dat_cont = cmddatcntl;	 

    return (MMC_SUCCESS);
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static int mpc5125_cfg_bus (SIM_HBA *hba, int width, int mmc)

{
SIM_MMC_EXT		*ext;
mpc5125_ext_t	*mpc5125;

	ext  = (SIM_MMC_EXT *) hba->ext;
	mpc5125 = (mpc5125_ext_t *) ext->handle;

	if (width == 4)
		mpc5125->bus_width = 2 << 8;
	else
		mpc5125->bus_width = 0;

	return (MMC_SUCCESS);
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static int mpc5125_shutdown (SIM_HBA *hba)

{
SIM_MMC_EXT		*ext = (SIM_MMC_EXT *) hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *) ext->handle;

	mpc5125_powerdown (hba);

	if (ext->hccap & MMC_HCCAP_DMA) {
		mpc5125->sdmafuncs.channel_release (mpc5125->dma_chn);
		mpc5125->sdmafuncs.fini ();
		}

	munmap_device_io (mpc5125->mmc_base, 4096);
	free (mpc5125);

	return (MMC_SUCCESS);
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static int mpc5125_block_size(SIM_HBA *hba, int blksz)

{
SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *) hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *) ext->handle;

	if (blksz > 4095)
		return (MMC_FAILURE);

	mpc5125->blksz = blksz;

	return (MMC_SUCCESS);
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static int mpc5125_detect (SIM_HBA *hba)

{
SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *) hba->ext;
mpc5125_ext_t	*mpc5125 = (mpc5125_ext_t *) ext->handle;
uint8_t			cpld;

	cpld = in8 (mpc5125->cpld_base + CPLD_INT_STAT);
	mpc5125->card_pres = (cpld & CARD_NOT_PRESENT) ? 0 : 1;
	return (mpc5125->card_pres ? MMC_SUCCESS : MMC_FAILURE);
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

uint32_t	find_immr_info ()

{
struct		asinfo_entry  *asinfo;
int32_t		i, num, cnt;

	num = _syspage_ptr->asinfo.entry_size / sizeof(*asinfo);
	asinfo = SYSPAGE_ENTRY (asinfo);

	for (i = cnt = 0; i < num; ++i) {
		char *name;
		name = __hwi_find_string (asinfo->name);
		if (strcmp (name, "immr")) {
			asinfo++;
			continue;
			}
		return ((uint32_t) asinfo->start);
		}
	return (0);
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/

int		mpc5125_attach (SIM_HBA *hba, int dmasize)

{
CONFIG_INFO			*cfg;
SIM_MMC_EXT			*ext;
mpc5125_ext_t	    *mpc5125;
mpc5125_mmc_reg		*mmc;
int					channel;
uint32_t			iobase, iocbase = 0;
uint32_t			dmabase = 0, cpldbase = 0;
uintptr_t			ioc;

	cfg = (CONFIG_INFO *)&hba->cfg;
	ext = (SIM_MMC_EXT *)hba->ext;

	iobase = find_immr_info ();
	if (iobase == 0 && cfg->IOPort_Base [0] == 0) {
		return (MMC_FAILURE);
		}
	if (iobase) {
		dmabase = iobase + MPC5125_DMA_BASE;
		iocbase = iobase + MPC5125_IOCTL_BASE;
		cpldbase = iobase + MPC5125_CPLD_BASE;
		iobase += MPC5125_SD_BASE1;
		}

	if ((mpc5125 = calloc (1, sizeof (mpc5125_ext_t))) == NULL)
		return (MMC_FAILURE);

	mpc5125->dmasize = dmasize ? dmasize : MAX_DMA_WRITE;

	if (!cfg->NumIOPorts) {
		cfg->IOPort_Base[0] = iobase;
		cfg->IOPort_Length[0] = 4096;
		cfg->NumIOPorts = 1;
		}

	if (!cfg->NumIRQs) {
		cfg->IRQRegisters[0] = 8;
		cfg->NumIRQs = 1;
		}

	if (!cfg->NumDMAs) {
		cfg->DMALst[0] = 30;
		cfg->NumDMAs = 1;
		}

	mpc5125->mmc_pbase = cfg->IOPort_Base[0];
	if ((mpc5125->mmc_base =
			mmap_device_io (4096, mpc5125->mmc_pbase)) == (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io failed");
		goto fail1;
		}

	mmc = (mpc5125_mmc_reg *) mpc5125->mmc_base;

	if ((mpc5125->ioc_base = mmap_device_io (4096, iocbase)) == (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io1 failed");
		goto fail1;
		}
	ioc = mpc5125->ioc_base;
/* For 5121 */
//	out32 (ioc + 0xc4, 0x9b);
//	out32 (ioc + 0xc8, 0x83);
//	out32 (ioc + 0xc8, 0x83);
//	out32 (ioc + 0xd0, 0x9b);
//	out32 (ioc + 0xd4, 0x9b);
//	out32 (ioc + 0xd8, 0x9b);
/* For 5125 */
	out8 (ioc + 0x6f, IOCTL_SLEW3);
	out8 (ioc + 0x70, (IOCTL_PUD | IOCTL_PUE | IOCTL_SLEW3));
	out8 (ioc + 0x71, (IOCTL_PUD | IOCTL_PUE | IOCTL_SLEW3));
	out8 (ioc + 0x72, (IOCTL_PUD | IOCTL_PUE | IOCTL_SLEW3));
	out8 (ioc + 0x73, (IOCTL_PUD | IOCTL_PUE | IOCTL_SLEW3));
	out8 (ioc + 0x74, (IOCTL_PUD | IOCTL_PUE | IOCTL_SLEW3));

	if ((mpc5125->dma_base = mmap_device_io (0x20000, dmabase)) == (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io2 failed");
		goto fail1;
		}
	if ((mpc5125->cpld_base = mmap_device_io (0x20, cpldbase)) == (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io3 failed");
		goto fail1;
		}

	if (!cfg->Description[0])
		strncpy (cfg->Description, "FREESCALE ADS5125 SDMMC", sizeof(cfg->Description));

	// Capability
	ext->hccap |= MMC_HCCAP_BW1 | MMC_HCCAP_BW4;	// 1 bit / 4 bits bus supported
	ext->hccap |= MMC_HCCAP_33V;

	mpc5125->clock = 400000000;
 
	ext->hba       = hba;
	ext->handle    = mpc5125;
	ext->clock     = mpc5125->clock;		// Maximum clock
	ext->detect    = mpc5125_detect;
	ext->powerup   = mpc5125_powerup;
	ext->powerdown = mpc5125_powerdown;
	ext->command   = mpc5125_command;
	ext->cfg_bus   = mpc5125_cfg_bus;
	ext->set_clock = mpc5125_clock;
	ext->set_blksz = mpc5125_block_size;
	ext->interrupt = mpc5125_interrupt;
	ext->setup_pio = mpc5125_setup_pio;
	ext->pio_done  = mpc5125_pio_done;
	ext->shutdown  = mpc5125_shutdown;

	if (get_dmafuncs (&mpc5125->sdmafuncs, sizeof(dma_functions_t)) == -1) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "get_dmafuncs failure");
		return (MMC_FAILURE);
		}

	if (mpc5125->sdmafuncs.init (NULL) == -1) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "dmafuncs init failure");
		return (MMC_FAILURE);
		}

	SIGEV_PULSE_INIT (&mpc5125->event, hba->coid, getprio (0) + 1, SIM_DMA_INTERRUPT, NULL);

	channel = 30;
	if ((mpc5125->dma_chn = mpc5125->sdmafuncs.channel_attach (NULL, &mpc5125->event, &channel, 0,
		DMA_ATTACH_PRIORITY_HIGHEST | DMA_ATTACH_EVENT_ON_COMPLETE)) == NULL) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "channel_attach failed %s", strerror (errno));
		goto dma_fail1;
		}

	ext->hccap |= MMC_HCCAP_DMA;
	ext->setup_dma = mpc5125_setup_dma;
	ext->dma_done  = mpc5125_dma_done;

	out8 (mpc5125->cpld_base + CPLD_INT_MASK, ~CARD_NOT_PRESENT);

	return (MMC_SUCCESS);

dma_fail1:
	mpc5125->sdmafuncs.fini ();
	munmap_device_io (mpc5125->mmc_base, 4096);
fail1:	
	free (mpc5125);
	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, " exiting mpc5125_attach with failure");
	return (MMC_FAILURE);
}

#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_mpc5125.c $ $Rev: 657836 $" );
