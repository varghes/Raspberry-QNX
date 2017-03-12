/*
 * $QNXLicenseC: 
 * Copyright 2008, 2011, QNX Software Systems.  
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
#include	<sys/slogcodes.h>
#include	<sys/syspage.h>
#include	<hw/sysinfo.h>
#ifdef MMCSD_VENDOR_FREESCALE_MX35
#define IPG_PER_CLK 66500000
#include	<sim_mx35.h>

static int mx35_setup_pio(SIM_HBA *hba, int len, int dir);

static int mx35_reset_line(mx35_ext_t *mx35, uint32_t bits)
{
	uintptr_t	base = mx35->mmc_base;
	uint32_t	sts;
	uint32_t	protoctl = in32(base + MX35_MMCPROTOCTL);

	/* disable and clear interrupt*/	
	sts = in32(base + MX35_MMCINTRST);
	out32(base + MX35_MMCINTRSTEN, 0);
	if (!mx35->sdcken)
		out32(base + MX35_MMCINTRSIGEN, 0);
	out32(base + MX35_MMCINTRST, sts);		// clear interrupt status

	out32(base + MX35_MMCSYSCTL, in32(base + MX35_MMCSYSCTL) | bits);
	while (in32(base + MX35_MMCSYSCTL) & bits)
		delay(1);
	out32(base + MX35_MMCPROTOCTL, protoctl);	

	// set watermark register. Read/Write Water mark level: 128 words, Read/Write burst length: 8 words
	out32(base + MX35_MMCWATML, ((0x880 << MMCWATML_WRWMLSHIFT) | (0x880 << MMCWATML_RDWMLSHIFT)));

	// Enable all interrupt singals
	if (mx35->sdcken)
		out32(base + MX35_MMCINTRSIGEN, 0x117F01FF);

	return 0;
}

static int mx35_interrupt(SIM_HBA *hba, int irq, int resp_type, uint32_t *resp)
{
	SIM_MMC_EXT		*ext;
	mx35_ext_t	    *mx35;
	int				intr = 0;
	uint32_t		command, sts, mask, maskerrsts;
	uintptr_t		base;

	ext  = (SIM_MMC_EXT *)hba->ext;
	mx35 = (mx35_ext_t *)ext->handle;
	base = mx35->mmc_base;

	command = ((in32(base + MX35_MMCCMDXTYPE) & MMCCMDXTYPE_CMDINXMASK) >> MMCCMDXTYPE_CMDINXSHIFT);
	sts = in32(base + MX35_MMCINTRST);

#if defined(IMX_USDHC)
	/*
	 * Errata TKT070753, Rev 1.0, IMX6DQCE
	 * on mx6q, there is low possibility that DATA END interrupt comes earlier than DMA
	 * END interrupt which is conflict with standard host controller spec. In this case, read the
	 * status register again will workaround this issue.
	 */
	if ((sts & MMCINTRST_DATA_END_INT) && !(sts & MMCINTRST_DMA_END_INT))
		sts = in32(base + MX35_MMCINTRST);
#endif
	mask = in32(base + MX35_MMCINTRSTEN);
	maskerrsts = MMCINTR_CTOE | MMCINTR_CCE | MMCINTR_CEBE | MMCINTR_CIE;

	// Command without data
	if (mask & MMCINTR_CC) {		// Command complete interrupt enabled ?
		if (sts & MMCINTR_CTOE) {	// Command Timeout ?
			intr |= MMC_ERR_CMD_TO | MMC_INTR_ERROR | MMC_INTR_COMMAND;
			mx35_reset_line(mx35, MMCSYSCTL_RSTC | MMCSYSCTL_RSTD);
			slogf(_SLOGC_SIM_MMC, _SLOG_INFO, "mx35_interrupt: CMD error intrsts:0x%X currentsts 0x%X for cmd: %d", sts, in32(base + MX35_MMCPRSNTST), command);
		} else if (sts & MMCINTR_CC) {
			intr |= MMC_INTR_COMMAND;
			if ((sts & (MMCINTR_CCE | MMCINTR_CEBE | MMCINTR_CIE))) {
				intr |= MMC_INTR_ERROR;	// Other errors
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, "mx35_interrupt: CMD error intrsts:0x%X currentsts 0x%X for cmd: %d", sts, in32(base + MX35_MMCPRSNTST), command);
			}

			// Errata ENGcm03648
			if (resp_type & MMC_RSP_BUSY) {
				int timeout = 16 * 1024 * 1024;

				while (!(in32(base + MX35_MMCPRSNTST) & MMCPRSNTST_DL0SL) && timeout--)
					nanospin_ns(100);

				if (timeout <= 0) {
					intr |= MMC_ERR_CMD_TO | MMC_INTR_ERROR;
					mx35_reset_line(mx35, MMCSYSCTL_RSTC | MMCSYSCTL_RSTD);
					slogf(_SLOGC_SIM_MMC, _SLOG_INFO, "mx35_interrupt: busy done wait timeout for cmd: %d", command);
				}
			}

			if (resp_type & MMC_RSP_136) {
				resp[0] = in32(base + MX35_MMCCMDRSP0);
				resp[1] = in32(base + MX35_MMCCMDRSP1);
				resp[2] = in32(base + MX35_MMCCMDRSP2);
				resp[3] = in32(base + MX35_MMCCMDRSP3);

				/*
				 * CRC is not included in the response register,
				 * we have to left shift 8 bit to match the 128 CID/CSD structure
				 */
				resp[3] = (resp[3] << 8) | (resp[2] >> 24);
				resp[2] = (resp[2] << 8) | (resp[1] >> 24);
				resp[1] = (resp[1] << 8) | (resp[0] >> 24);
				resp[0] = (resp[0] << 8);
			} else if (resp_type & MMC_RSP_PRESENT) {
				resp[0] = in32(base + MX35_MMCCMDRSP0);
				resp[1] = in32(base + MX35_MMCCMDRSP1);
				resp[2] = in32(base + MX35_MMCCMDRSP2);
				resp[3] = in32(base + MX35_MMCCMDRSP3);
			}
		}
	}

    maskerrsts |= MMCINTR_DTOE | MMCINTR_DCE | MMCINTR_DEBE | MMCINTR_DMAE;

	if (mask & (MMCINTR_BRR | MMCINTR_BWR)) {	// PIO
		if (sts & MMCINTR_BRR)
			intr |= MMC_INTR_RBRDY;
		else if (sts & MMCINTR_BWR)
			intr |= MMC_INTR_WBRDY;
		else if (sts & (maskerrsts)) {			// errors
			intr |= MMC_INTR_ERROR;
			if (sts & MMCINTR_DCE)
		   		intr |= MMC_ERR_DATA_CRC;
			if (sts & MMCINTR_DTOE)
		   		intr |= MMC_ERR_DATA_TO;
			slogf(_SLOGC_SIM_MMC, _SLOG_INFO, "mx35_interrupt:PIO Error bits set in status:intrsts:0x%X currentsts 0x%X for cmd: %d", sts, in32(base + MX35_MMCPRSNTST), command);
			mx35_reset_line(mx35, MMCSYSCTL_RSTC | MMCSYSCTL_RSTD);
			if ((sts & (MMCINTR_DCE | MMCINTR_DMAE)) && (mask & MMCINTR_AC12E))
				ext->stop = MMCSD_STOP_PEND;
		}
	} else if (mask & (MMCINTR_DINT|MMCINTR_TC)) {			// DMA
		maskerrsts |= MMCINTR_DMAE;
		while (1) {
			if (sts & maskerrsts) {
		    	intr |= MMC_INTR_ERROR;
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, "mx35_interrupt: DMA Error bits set in status:intrsts:0x%X currentsts 0x%X for cmd: %d", sts, in32(base + MX35_MMCPRSNTST), command);
				if (sts & MMCINTR_DCE){
		    		intr |= MMC_ERR_DATA_CRC;
					if (1) {	//!mx35->sdcken){ //To1 need the workaround to re-indentify the card
						ext->state = MMCSD_STATE_IDENT;
						ext->ident = MMCSD_IDENT_START;
						break;
					}
				}
				if (sts & MMCINTR_DTOE)
		    		intr |= MMC_ERR_DATA_TO;
				mx35_reset_line(mx35, MMCSYSCTL_RSTC | MMCSYSCTL_RSTD);
				break;
			} else if ((sts & MMCINTR_DINT) == (mask & MMCINTR_DINT )) {
#if 0			// We are not using AC12, so move DAT0 checking to CMD12 command done interrupt
				/*
				 * There are the DATA END INT when
				 * writing is not complete. Double
				 * check on it. TO2 has been fixed it.
				 */
				if (!mx35->sdcken){
					int timeout = 20000000;
					while(!(in32(base + MX35_MMCPRSNTST) & MMCPRSNTST_DL0SL) && timeout--)
						nanospin_ns(100);
					if (timeout <= 0)
						slogf(_SLOGC_SIM_MMC, _SLOG_INFO, "mx35_interrupt: ERRATA ENGcm03648 timeout(%d) intrsts:0x%X currentsts 0x%X for cmd: %d", timeout, sts, in32(base + MX35_MMCPRSNTST), command);
				}
#endif
				if (sts & MMCINTR_TC) {
    	    		intr |= MMC_INTR_DATA;
					break;
				}
			}
			nanospin_ns(100);
			sts = in32(base + MX35_MMCINTRST);
		}
	}

	// clear interrupt source 
	sts = in32(base + MX35_MMCINTRST);
	out32(base + MX35_MMCINTRSTEN, 0);
	if (!mx35->sdcken)
		out32(base + MX35_MMCINTRSIGEN, 0);
	else
		out32(mx35->mmc_base + MX35_MMCSYSCTL, in32(mx35->mmc_base + MX35_MMCSYSCTL) & ~0x07);
	out32(base + MX35_MMCINTRST, sts);		// clear interrupt status

    return intr;
}

static int mx35_setup_dma(SIM_HBA *hba, paddr_t paddr, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	mx35_ext_t	    *mx35;
	int				xlen;
	
	ext  = (SIM_MMC_EXT *)hba->ext;
	mx35 = (mx35_ext_t *)ext->handle;

	if ((xlen = mx35_setup_pio(hba, len, dir)) > 0) {
		out32(mx35->mmc_base + MX35_MMCDMASYSADD, paddr);

#if defined(IMX_USDHC)
		mx35->mix_ctrl |= MMCCMDXTYPE_DMAEN;
#else
		mx35->cmdxtype |= MMCCMDXTYPE_DMAEN;
#endif
		mx35->mask &= ~(MMCINTR_BRR | MMCINTR_BWR);
		// T01 can't do this
		if (!mx35->sdcken || dir == MMC_DIR_IN)
			mx35->mask |= MMCINTR_DMAE | MMCINTR_DINT | MMCINTR_TC;
		else
			mx35->mask |= MMCINTR_DMAE | MMCINTR_TC;
	}

	return (xlen);
}

static int mx35_dma_done(SIM_HBA *hba, int dir)
{
	return MMC_SUCCESS;
}

static int mx35_command(SIM_HBA *hba, mmc_cmd_t *cmd)
{
	SIM_MMC_EXT		*ext;
	mx35_ext_t	    *mx35;
	uintptr_t		base;
	uint32_t        cmdxtype;
	uint32_t        intrsten;
	uint32_t        intrstien;
	uint32_t		command;
	uint32_t		intmask, i, maskinhibit;

	ext  = (SIM_MMC_EXT *)hba->ext;
	mx35 = (mx35_ext_t *)ext->handle;
	base = mx35->mmc_base;

	cmdxtype = 0;
    intrsten = 0;
    intrstien = 0;
    maskinhibit = MMCPRSNTST_CIHB;
    command = cmd->opcode;
	if (command == 12)
		cmdxtype |= MMCCMDXTYPE_CMDTYPABRT;

	cmdxtype |= (command << MMCCMDXTYPE_CMDINXSHIFT);

    if (cmd->resp & MMC_RSP_PRESENT) {
		if (cmd->resp & MMC_RSP_OPCODE)
			cmdxtype |= MMCCMDXTYPE_CICEN;
		if (cmd->resp & MMC_RSP_CRC)
			cmdxtype |= MMCCMDXTYPE_CCCEN;
		if (cmd->resp & MMC_RSP_BUSY) {			// must be R1b
			cmdxtype |= MMCCMDXTYPE_RSPTYPL48B;
			if (command != 12)					//command 12 can be asserted even if data lines are busy
				maskinhibit |= MMCPRSNTST_CDIHB;
		} else if (cmd->resp & MMC_RSP_136) 	// must be R2
			cmdxtype |= MMCCMDXTYPE_RSPTYPL136;
		else
			cmdxtype |= MMCCMDXTYPE_RSPTYPL48;
	}

	cmdxtype |= mx35->cmdxtype;
	intmask = MMCINTR_CTOE | MMCINTR_CCE | MMCINTR_CEBE | MMCINTR_CIE;
	if (!(cmd->eflags & MMC_CMD_DATA)) {
		// wait till card is ready to handle next command 
		for (i = 0; in32(base + MX35_MMCPRSNTST) & maskinhibit; i++) {
			if (i > 1000000) {
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, " timedout for cmd/data inhibit, currentsts %x", in32(base + MX35_MMCPRSNTST));
				return (MMC_FAILURE);
			}	
			nanospin_ns(100);
		}
		intmask = MMCINTR_CC | intmask;
	} else
		intmask |= mx35->mask | MMCINTR_DTOE | MMCINTR_DCE | MMCINTR_DEBE; 

	mx35->mask = 0;
	mx35->cmdxtype = 0;

	if ((cmd->eflags & MMC_CMD_INIT) || (command == 0)) {	// initialization clock sequence
		out32(base + MX35_MMCSYSCTL, in32(base + MX35_MMCSYSCTL) | MMCSYSCTL_INITA);
		for (i = 0; in32(base + MX35_MMCSYSCTL) & MMCSYSCTL_INITA; i++) {
			if (i > 100000)
				return (MMC_FAILURE);
			nanospin_ns(100);
	    }
	}
	out32(base + MX35_MMCINTRST, 0xFFFFFFFF);
	out32(base + MX35_MMCINTRSTEN, intmask);
	if (!mx35->sdcken)
		out32(base + MX35_MMCINTRSIGEN, intmask);
	else
		out32(mx35->mmc_base + MX35_MMCSYSCTL, in32(mx35->mmc_base + MX35_MMCSYSCTL) | 0x07);

#if defined(IMX_USDHC)
	mx35->mix_ctrl |= (in32(base + MX6_MMC_MIX_CTRL) & (0xf << 22));
	out32(base + MX6_MMC_MIX_CTRL, mx35->mix_ctrl);
	mx35->mix_ctrl = 0;

	// Rev 1.0 i.MX6x chips require the watermark register to be set prior to every SD command being sent.
	// If the watermark is not set only SD interface 3 works.
	out32(base + MX35_MMCWATML, ((0x880 << MMCWATML_WRWMLSHIFT) | (0x880 << MMCWATML_RDWMLSHIFT)));
#endif

	out32(base + MX35_MMCCMDARG, cmd->argument);
	out32(base + MX35_MMCCMDXTYPE, cmdxtype);

	return (MMC_SUCCESS);
}

/*
 * configure host controller 
 */
static int mx35_cfg_bus(SIM_HBA *hba, int width, int mmc)
{
	SIM_MMC_EXT		*ext;
	mx35_ext_t	    *mx35;
	uint32_t        protctl;

	ext  = (SIM_MMC_EXT *)hba->ext;
	mx35 = (mx35_ext_t *)ext->handle;
	
    if (width == 8)
    	protctl = MMCPROTOCTL_EMODLE | MMCPROTOCTL_DTW8BIT;
	else if (width == 4)
    	protctl = MMCPROTOCTL_EMODLE | MMCPROTOCTL_DTW4BIT;
	else
		protctl = MMCPROTOCTL_EMODLE; //DTW 1 bit is default
	
    out32(mx35->mmc_base + MX35_MMCPROTOCTL, protctl);

    return (MMC_SUCCESS);
}

static int mx35_block_size(SIM_HBA *hba, int blksz)
{
	SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *)hba->ext;
	mx35_ext_t		*mx35 = (mx35_ext_t *)ext->handle;

	if (blksz > 4096)
		return (MMC_FAILURE);

	mx35->blksz = blksz;
    out32(mx35->mmc_base + MX35_MMCBLATR, blksz << MMCBLATR_BLSIZESHIFT);

	return (MMC_SUCCESS);
}

static int mx35_clock(SIM_HBA *hba, int *clock, int high_speed)
{
	SIM_MMC_EXT		*ext  = (SIM_MMC_EXT *)hba->ext;
	mx35_ext_t		*mx35 = (mx35_ext_t *)ext->handle;
	uintptr_t		base = mx35->mmc_base;
	int			max_clk	= mx35->clock;
	int			sysctl, pre_div, div;

	if (*clock == 0)
		return (MMC_FAILURE);

	if (*clock > max_clk)
		*clock = max_clk;

    if(max_clk / 16 > *clock)
        for (pre_div = 1; pre_div < 256; pre_div *= 2) {
            if ((max_clk / pre_div) <= (*clock * 16))
                break;
        } else
		pre_div = 2;
    
	for (div = 1; div <= 16; div++)
		if ((max_clk / (div * pre_div)) <= *clock)
			break;

	*clock = max_clk / (pre_div * div);
	pre_div >>= 1;
	div -= 1;

	sysctl = ((0xE << MMCSYSCTL_DTOCVSHIFT) | (pre_div << MMCSYSCTL_SDCLKFSSHIFT) |
              (div << MMCSYSCTL_DVSSHIFT));
	if (!mx35->sdcken)
		sysctl |= 0x07;
	out32(base + MX35_MMCSYSCTL, sysctl);
	delay(1);
	out32(base + MX35_MMCSYSCTL, sysctl | mx35->sdcken);
	delay(10);

	return (MMC_SUCCESS);
}

/*
 * Reset host controller and card
 * The clock should be enabled and set to minimum (<400KHz)
 */
static int mx35_powerup(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	mx35_ext_t	    *mx35;
	uintptr_t		base;
	int				clk;

	ext  = (SIM_MMC_EXT *)hba->ext;
	mx35 = (mx35_ext_t *)ext->handle;
	base = mx35->mmc_base;

	// Apply a software reset
	out32(base + MX35_MMCSYSCTL, MMCSYSCTL_RSTA);
	delay(10);

	// Set 400K ident clock
	clk = 400 * 1000;
	mx35_clock(hba, &clk, 0);

	if (mx35->sdcken)
		out32(base + MX35_MMCINTRSIGEN, 0x117F01FF);

 	return (MMC_SUCCESS);
}

static int mx35_detect(SIM_HBA *hba)
{
	/* On MX353DS board, card insertion/removal detection is done by GPIO and not by SD host controller it self. Since this function is controller specific,
	 * nothing to do here*/
	return (MMC_SUCCESS);
}

static int mx35_powerdown(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	mx35_ext_t		*mx35;
	uintptr_t		base;

	ext  = (SIM_MMC_EXT *)hba->ext;
	mx35 = (mx35_ext_t *)ext->handle;
	base = mx35->mmc_base;

	// Disable all interrupt signals and status
	out32(base + MX35_MMCINTRSTEN, 0);
	out32(base + MX35_MMCINTRSIGEN, 0);

	return (MMC_SUCCESS);
}

static int mx35_shutdown(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	mx35_ext_t	    *mx35;
    uintptr_t		base;
    
	ext  = (SIM_MMC_EXT *)hba->ext;
	mx35 = (mx35_ext_t *)ext->handle;
   	base = mx35->mmc_base;

	// Apply a software reset
	out32(base + MX35_MMCSYSCTL, MMCSYSCTL_RSTA);
	delay(10);

	mx35_powerdown(hba);

	munmap_device_io(base, MX35_SDC_SIZE);

	free(mx35);

	return (MMC_SUCCESS);
}

/*
 * setup PIO transfer
 */
static int mx35_setup_pio(SIM_HBA *hba, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	mx35_ext_t		*mx35;
	uintptr_t		base;
	uint32_t		blkcnt, i, prsntst, maskinhibit;
      
	ext  = (SIM_MMC_EXT *)hba->ext;
	mx35 = (mx35_ext_t *)ext->handle;
	base = mx35->mmc_base;

	blkcnt = len / mx35->blksz;

  	if (blkcnt == 0)
		return 0;

	maskinhibit = MMCPRSNTST_CDIHB | MMCPRSNTST_CIHB;	
    prsntst = in32(base + MX35_MMCPRSNTST);	

    // Ensure that data lines are not busy before changing any register's value which can affect ongoing data transfer
	for (i = 0; ((prsntst & MMCPRSNTST_DL0SL) == 0) || ((prsntst & MMCPRSNTST_CLSL) == 0) || (prsntst & maskinhibit); i++) 
	{
		if (i > 40000) {
			slogf(_SLOGC_SIM_MMC, _SLOG_INFO, " timedout for data0 line.");
			return (0);
		}
		nanospin_ns(100000);
		prsntst = in32(base + MX35_MMCPRSNTST);	
	}

	out32(base + MX35_MMCBLATR, (mx35->blksz << MMCBLATR_BLSIZESHIFT) | (blkcnt << MMCBLATR_BLCNTSHIFT));	// only valid for multi-block transfer
	mx35->cmdxtype = MMCCMDXTYPE_DPSEL;		
	if (dir == MMC_DIR_IN) {
#if defined(IMX_USDHC)
		mx35->mix_ctrl |= MMCCMDXTYPE_DTDSEL;
#else
		mx35->cmdxtype |= MMCCMDXTYPE_DTDSEL;
#endif
		mx35->mask = MMCINTR_BRR;
	} else
		mx35->mask = MMCINTR_BWR;

	if (blkcnt > 1)
#if defined(IMX_USDHC)
		mx35->mix_ctrl |= MMCCMDXTYPE_BCEN | MMCCMDXTYPE_MSBSEL;
#else
		mx35->cmdxtype |= MMCCMDXTYPE_BCEN | MMCCMDXTYPE_MSBSEL;
#endif

    return (blkcnt * mx35->blksz);
}


static int mx35_pio_done(SIM_HBA *hba, char *buf, int len, int dir)
{
	SIM_MMC_EXT		*ext;
	mx35_ext_t		*mx35;
	uintptr_t		base;
	uint32_t		cnt, *pbuf = (uint32_t *)buf, i;
  
	ext  = (SIM_MMC_EXT *)hba->ext;
	mx35 = (mx35_ext_t *)ext->handle;
	base = mx35->mmc_base;

	if (len < (mx35->blksz))
		return 0;

	if (dir == MMC_DIR_IN) {
	    for (i = 0; (in32(base + MX35_MMCPRSNTST) & MMCPRSNTST_BREN) == 0; i++) {
			if (i > 100000000) {
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, " pio read: timedout for BREN in present state register");
				return (0);
			}
		}
		for (cnt = 0; cnt < mx35->blksz; cnt += 4)
			*pbuf++ = in32(base + MX35_MMCDATAPORT);
	} else {
	    for (i = 0; (in32(base + MX35_MMCPRSNTST) & MMCPRSNTST_BWEN) == 0; i++) {
			if (i > 100000000) {
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, " pio write: timedout for BWEN in present state register");
				return (0);
			}
		}
		for (cnt = 0; cnt < mx35->blksz; cnt += 4)
			out32(base + MX35_MMCDATAPORT, *pbuf++);
		nanospin_ns(100);	
	}

	return (mx35->blksz);
}

static unsigned mx35_get_syspage_clk(void)
{
	unsigned	item, offset, clock = 0;
	char		*name;
	hwi_tag		*tag;

	/* Find network hardware information. */
	item = hwi_find_item(HWI_NULL_OFF, HWI_ITEM_DEVCLASS_DISK, "eSDHC", NULL);
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

int	mx35_attach(SIM_HBA *hba)
{
	CONFIG_INFO		*cfg;
	SIM_MMC_EXT		*ext;
	mx35_ext_t	    *mx35;
	unsigned		reg;
	uintptr_t		mx35_rom_vbase;

	cfg = (CONFIG_INFO *)&hba->cfg;
	ext = (SIM_MMC_EXT *)hba->ext;

	if ((mx35 = calloc(1, sizeof(mx35_ext_t))) == NULL)
		return (NULL);

	// Check silicon revision
	if ((mx35_rom_vbase = mmap_device_io(16 * 1024, 0)) == (uintptr_t)MAP_FAILED)
		goto fail0;
	if ((in32(mx35_rom_vbase + 0x40) & 0x0F) >= 2)
		mx35->sdcken = 1 << 3;

	munmap_device_io(mx35_rom_vbase, 16 * 1024);

	mx35->mmc_pbase = cfg->IOPort_Base[0];
	if ((mx35->mmc_base =
			mmap_device_io(MX35_SDC_SIZE, mx35->mmc_pbase))
				== (uintptr_t)MAP_FAILED) {
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "SD: mmap_device_io failed");
		goto fail0;
	}
	
	reg = in32(mx35->mmc_base + MX35_MMCHCTLRCAP);

	// Capability
	ext->hccap |= MMC_HCCAP_BW8 | MMC_HCCAP_BW4 | MMC_HCCAP_BW1;	// 1, 4 and 8 bits bus supported
	ext->hccap |= MMC_HCCAP_33V;		// board supplies 3.3 V only to SD slot
	
	if (reg & MMCHCTLRCAP_DMAS)
		ext->hccap |= MMC_HCCAP_DMA;

	if (reg & MMCHCTLRCAP_HSS)
		ext->hccap |= MMC_HCCAP_HS;

	if ((mx35->clock = mx35_get_syspage_clk()) == 0)
		mx35->clock = MX35_CLOCK_DEFAULT;
 
	ext->hba       = hba;
	ext->handle    = mx35;
	ext->clock     = mx35->clock;		// Maximum clock
	ext->detect    = mx35_detect;
	ext->powerup   = mx35_powerup;
	ext->powerdown = mx35_powerdown;
	ext->command   = mx35_command;
	ext->cfg_bus   = mx35_cfg_bus;
	ext->set_clock = mx35_clock;
	ext->set_blksz = mx35_block_size;
	ext->interrupt = mx35_interrupt;
	ext->setup_pio = mx35_setup_pio;
	ext->pio_done  = mx35_pio_done;
	ext->setup_dma = mx35_setup_dma;
	ext->dma_done  = mx35_dma_done;
	ext->shutdown  = mx35_shutdown;

	if (!cfg->Description[0])
		strncpy(cfg->Description, "FREESCALE MX35 eSDHC", sizeof(cfg->Description));
	
	return (MMC_SUCCESS);

fail0:
    free(mx35);
	slogf(_SLOGC_SIM_MMC, _SLOG_INFO, " exiting mx35_attach with failure");
	return (MMC_FAILURE);
}

#endif


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_mx35.c $ $Rev: 657836 $" );
