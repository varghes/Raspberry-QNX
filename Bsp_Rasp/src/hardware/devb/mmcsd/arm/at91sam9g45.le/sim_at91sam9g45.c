/*
 * $QNXLicenseC:
 * Copyright 2010-2011 QNX Software Systems. 
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

#include <sim_mmc.h>

#ifdef MMCSD_VENDOR_MCI
 
#include <sim_at91sam9g45.h>
#include <sys/trace.h>

#define TIMEOUT_LOOPS 1000000

#define MAX_DMABUF_SIZE 65536
#define MAX_LLI_NUM	128

#define DMAC_IRQ	21

#ifdef AT91SAM9G45_R1BWORKAROUND
struct timespec uswait = {0, 10000};

static uint32_t * pioa_pdsr = (uint32_t *)MAP_FAILED;
#endif

static uint32_t lli_mphy;
static uint32_t lli_size;

/******************************
 *  DMAC functions START
 *****************************/
static dmac_dev_t * dmac_init (hsmci_ext_t *hsmci)
{
	rsrc_request_t	req;
	dmac_dev_t	*dmac_dev;
	int		timeout;

	/* Allocated memory for channel */
	dmac_dev = (dmac_dev_t*)calloc(1, sizeof (dmac_dev_t));
	if (dmac_dev == NULL)
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: DMAC calloc failed\n");
		return NULL;
	}

	/* Map DMAC controller */
	dmac_dev->dmac = (at91sam9xx_dmac_t *)mmap_device_memory(NULL,0x200,
		PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, (uint32_t)(hsmci->dbase));

	if (dmac_dev->dmac == MAP_FAILED)
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: DMAC MAP_FAILED\n");
		free (dmac_dev);
		return NULL;
	}

	/* Apply DMAC channel */
	memset(&req, 0, sizeof(req));
	req.length = 1;
	req.flags = RSRCDBMGR_DMA_CHANNEL;

	if (rsrcdbmgr_attach( &req, 1) == -1) 
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: Cannot acquire DMAC channel\n");
		munmap_device_memory((void *)dmac_dev->dmac, sizeof(at91sam9xx_dmac_t));
		free (dmac_dev);
		return NULL;
	}
	else
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: Use DMAC channel %lld\n", req.start);
		dmac_dev->chid = req.start;
	}

	/* Stop channel in case it is running */
	dmac_dev->dmac->chdr = 1 << (dmac_dev->chid);
	
	timeout = 0;
	while (dmac_dev->dmac->chsr & (1 <<dmac_dev->chid))
	{
		delay (1);
		if (timeout++ > 2000)
		{
			slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "%s: release DMAC channel time out\n", __func__);
			break;
		}
	}

	/* Reset HW LLI table*/
	memset(&(dmac_dev->dmac->lli[dmac_dev->chid]), 0, sizeof(at91sam9xx_dmac_lli_t));

	/* Alloc buffer link list memory */
	dmac_dev->lli = mmap (0, (MAX_LLI_NUM) * sizeof(at91sam9xx_dmac_bd_t),
		PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, NOFD, 0);

	if (dmac_dev->lli == MAP_FAILED)
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: DMA setup_xfer map failed\n");
		return NULL;
	}

	/* Initilize the link list */
	memset(dmac_dev->lli, 0, MAX_LLI_NUM * sizeof(at91sam9xx_dmac_bd_t));

	/* Get link list physical address here and assign them in setup_xfer to save CPU time */
	lli_mphy = (uint32_t) mphys(&dmac_dev->lli[0]);
	lli_size = sizeof(at91sam9xx_dmac_bd_t);

	/* 
	 * Associate Channel chid Src/Dst Request peripheral identifier 
	 * SRC_PER/DST_PER handshaking interface 
	 */
	dmac_dev->dmac->lli[dmac_dev->chid].cfg  = DMAC_SOD_DISABLE
			| (hsmci->dintf << 0) | (hsmci->dintf << 4);

	dmac_dev->dmac->en = DMAC_ENABLE;

	return dmac_dev;
}


static void dmac_fini (dmac_dev_t * dmac_dev)
{
	rsrc_request_t req;
	int timeout = 0;
    
	munmap (dmac_dev->lli, (MAX_LLI_NUM) * sizeof(at91sam9xx_dmac_bd_t) ); 

	/* Disable channel */
	dmac_dev->dmac->chdr |= (1 << dmac_dev->chid);

	while (dmac_dev->dmac->chsr & (1 <<dmac_dev->chid))
	{
		delay (1);
		if (timeout++ > TIMEOUT_LOOPS)
		{
			slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "%s: release DMAC channel time out\n", __func__);
			break;
		}
	}

	/* release DMAC mmap resources */
	munmap (dmac_dev->lli, (MAX_LLI_NUM) * sizeof(at91sam9xx_dmac_bd_t) ); 

	/* Release DMAC channel */	
	memset(&req, 0, sizeof(req));
	req.length = 1;
	req.flags = RSRCDBMGR_DMA_CHANNEL;
	req.start = dmac_dev->chid;
	req.end = dmac_dev->chid;

	/* Return the resource to the database: */
	rsrcdbmgr_detach( &req, 1);
	
	if (dmac_dev != NULL)
	{
		if ( dmac_dev->dmac!= MAP_FAILED)
		{
			munmap_device_memory((void *)dmac_dev->dmac, sizeof(at91sam9xx_dmac_t));
		}
		
		free (dmac_dev);
	}
}

static int dmac_xfer_start (dmac_dev_t * dmac_dev)
{
	/* Enable DMA request */
	/* Copy current TCD image into DMA controller */
	dmac_dev->dmac->lli[dmac_dev->chid].saddr = 0;
	dmac_dev->dmac->lli[dmac_dev->chid].daddr = 0;
	dmac_dev->dmac->lli[dmac_dev->chid].ctrlb = 0;
	dmac_dev->dmac->lli[dmac_dev->chid].ctrla = 0;
	dmac_dev->dmac->lli[dmac_dev->chid].dscr = lli_mphy;

	/* Trigger DMA transfer */
	dmac_dev->dmac->cher = (1 << dmac_dev->chid);

	return 0;
}

static int dmac_xfer_stop (dmac_dev_t * dmac_dev)
{
	/* Suspend DMA transfer */
	dmac_dev->dmac->cher = (1 << (dmac_dev->chid + 8));

	return 0;
}

static int dmac_setup_xfer (dmac_dev_t * dmac_dev, paddr_t paddr, int len, int dir)
{
	int i, blknum = len/dmac_dev->blksz;

	dmac_dev->blknum = blknum;

	memset(dmac_dev->lli, 0, blknum * sizeof(at91sam9xx_dmac_bd_t));

	/* Resume Channel */
	dmac_dev->dmac->chdr = 1 << (dmac_dev->chid + 8);

	if (dir == DATA_READ)		/* Block READ */
	{
		/* Setup DMA direction */
		dmac_dev->dmac->lli[dmac_dev->chid].cfg &= ~(DMAC_DST_H2SEL_SW | DMAC_SRC_H2SEL_HW);
		dmac_dev->dmac->lli[dmac_dev->chid].cfg |= DMAC_DST_H2SEL_SW | DMAC_SRC_H2SEL_HW;

		/* Setup link list */
		for (i = 0; i < blknum; i++)
		{
			/* Set Source and address transfer size and modulo operation */
			dmac_dev->lli[i].ctrla = ((dmac_dev->blksz/4) & 0xffff) 
				| SRC_WIDTH(2) | DST_WIDTH(2) | SCSIZE;
			dmac_dev->lli[i].ctrlb = DMAC_FC_PER2MEM | FLAG_SRC_INCREMENT
				| FLAG_DST_INCREMENT | SET_DST_DSCR;
			dmac_dev->lli[i].saddr = dmac_dev->io_addr;
			dmac_dev->lli[i].daddr = paddr + dmac_dev->blksz * i;

			if (i == blknum - 1)
			{
				/* Set last lli descriptor to 0 to mark NULL */
				dmac_dev->lli[i].dscr = 0;
			}
			else
			{
				/* Set next lli descriptor */
				dmac_dev->lli[i].dscr = lli_mphy + (i + 1) * lli_size;
			}
		}
	} 
	else if (dir == DATA_WRITE)	/* Block WRITE */
	{
		/* Setup DMA direction */
		dmac_dev->dmac->lli[dmac_dev->chid].cfg &= ~(DMAC_DST_H2SEL_HW | DMAC_SRC_H2SEL_SW);
		dmac_dev->dmac->lli[dmac_dev->chid].cfg |= DMAC_DST_H2SEL_HW | DMAC_SRC_H2SEL_SW;

		for (i = 0; i < blknum; i++)
		{
			/* Set Source and address transfer size and modulo operation */
			dmac_dev->lli[i].ctrla = ((dmac_dev->blksz/4) & 0xffff) 
				| SRC_WIDTH(2) | DST_WIDTH(2) | DCSIZE;
			dmac_dev->lli[i].ctrlb = DMAC_FC_MEM2PER | FLAG_SRC_INCREMENT 
				| FLAG_DST_INCREMENT | SET_SRC_DSCR;
			dmac_dev->lli[i].saddr = paddr + dmac_dev->blksz * i;
			dmac_dev->lli[i].daddr = dmac_dev->io_addr;
			if (i == blknum - 1)
			{
				/* Set last lli descriptor to 0 to mark NULL */
				dmac_dev->lli[i].dscr = 0;
			}
			else
			{
				/* Set next lli descriptor */
				dmac_dev->lli[i].dscr = lli_mphy + (i + 1) * lli_size;
			}
		}
	}
	else
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: DMA setup_xfer neither READ or WRITE\n");
		return -1;
	}

	return 0;
}
/******************************
 *  DMAC functions END
 *****************************/

/******************************
 * HSMCI functions
 *****************************/

static int _hsmci_detect (SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
    uint32_t  cd_wp, cd, wp;
	uintptr_t	base;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

    cd_wp = in32( hsmci->pio_base + AT91SAM9G45_PIO_PDSR );

    if ( hsmci->port == 1 ) {
        cd = AT91SAM9G45_MCI_PIO_CD1;
        wp = AT91SAM9G45_MCI_PIO_WP1;
    }
    else {
        cd = AT91SAM9G45_MCI_PIO_CD0;
        wp = AT91SAM9G45_MCI_PIO_WP0;
    }

    if ( ~ cd_wp & cd ) {       /* Card detect (negative logic)? */

        if ( cd_wp & wp ) {     /*  Write protected? */
            ext->eflags |= MMC_EFLAG_WP;
        }
        else {
            ext->eflags &= ~MMC_EFLAG_WP;
        }

        return MMC_SUCCESS;
    }

    return MMC_FAILURE;
}

static int _hsmci_interrupt (SIM_HBA *hba, int irq, int resp_type, uint32_t *resp)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;
	uint32_t	status, status2;
	int		intr;
	volatile int	sts;

	intr = MMC_INTR_NONE;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

	status2 = READ32(MCI_SR);
	status = status2 & READ32(MCI_IMR);

	/* Disable incoming interrupt */
	WRITE32 (MCI_IDR, status);

	/*
	 * If there is error status and it is not a CRC error for R3, 
	 * since R3 command doesn't require CRC check.
	 */
	if ( (status & MCI_ERRORS) 
	&& (! ((resp_type == MMC_RSP_R3) && ((status & MCI_ERRORS) == RCRCE))))
	{
		intr |= MMC_INTR_ERROR;

		if (status & RINDE ) {
			intr |= MMC_ERR_CMD_IDX;
		}

		if (status & RCRCE ) {
			intr |= MMC_ERR_CMD_CRC;
		}

		if (status & RENDE ) {
			intr |= MMC_ERR_CMD_END;
		}

		if (status & RTOE ) {
			intr |= MMC_ERR_CMD_TO;
		}

		if (status & DCRCE ) {
			intr |= MMC_ERR_DATA_CRC;
		}

		if (status & DTOE ) {
			intr |= MMC_ERR_DATA_TO;
		}
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: MCI_SR 0x%x cmd %x", status, hsmci->cmd);
	}

	else if (status & CMDRDY) {			/* Send Command done */
		intr |= MMC_INTR_COMMAND;

		/* Handle a normal command that has been completed */
		if (resp_type & MMC_RSP_PRESENT) {
			if (resp_type & MMC_RSP_136)	/* 136 bit response */
			{
				resp[3] = READ32(MCI_RSPR0);
				resp[2] = READ32(MCI_RSPR1);
				resp[1] = READ32(MCI_RSPR2);
				resp[0] = READ32(MCI_RSPR3);
			} else				/* 48 bit response */
			{
				resp[0] = READ32(MCI_RSPR0);
			}
		}
		if ((hsmci->cmd == MMC_WRITE_BLOCK) || (hsmci->cmd == MMC_WRITE_MULTIPLE_BLOCK)
			|| (hsmci->cmd == MMC_READ_SINGLE_BLOCK) || (hsmci->cmd == MMC_READ_MULTIPLE_BLOCK))
		{
			/* Clean up pending CBTC flags */
			sts = hsmci->dmac_dev->dmac->ebcisr;

			dmac_xfer_start(hsmci->dmac_dev);

			/* For single block, just wait for DMA DONE */
			if ((hsmci->cmd == MMC_WRITE_BLOCK) || (hsmci->cmd == MMC_READ_SINGLE_BLOCK))
			{
				WRITE32(MCI_IER, MCI_ERRORS | XFRDONE);
			}
			else	/* For multiple block write, must poll CBTC bit */
			{
				int iid, irqerr;
				uint64_t ntime;
				struct sigevent event;

				/* Wait for DMA done using an interrupt from CBTC bit of EBCISR register */
				/* Initialize interrupt handler */
				SIGEV_INTR_INIT(&event);

				/* Attach to DMAC IRQ */
				iid = InterruptAttachEvent(DMAC_IRQ, &event,
						_NTO_INTR_FLAGS_TRK_MSK);
				if (iid == -1)
				{
					slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: DMAC interrupt attach error");
					intr |= MMC_INTR_ERROR;
					return intr;
				}

				/* Enable CBTC interrupt */
				hsmci->dmac_dev->dmac->ebcier = (1<<(hsmci->dmac_dev->chid + 8));
				InterruptUnmask(DMAC_IRQ, iid);

				/* Setup Time out */
				ntime = (uint64_t) ( (uint64_t)(1000000) * (uint64_t)(1000));
				TimerTimeout(CLOCK_MONOTONIC, _NTO_TIMEOUT_INTR, NULL, &ntime, NULL);

				/* Now wait for CBTC interrupt */
				irqerr = InterruptWait(0, NULL);

				if (irqerr == -1) {
					slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: wait for DMAC done time out\n");
					intr |= MMC_INTR_ERROR;
				}

				/* Disable CBTC interrupt */
				hsmci->dmac_dev->dmac->ebcidr = (1<<(hsmci->dmac_dev->chid + 8)); 
				InterruptDetach(iid);

				/* DMA done, wait for FIFO empty */
				WRITE32(MCI_IER, MCI_ERRORS | FIFOEMPTY);
			}
		}
	} 

	else if (status & FIFOEMPTY)
	{
		/* Write done for DMAC */
		intr |= MMC_INTR_DATA;
	}
	else if ((status & XFRDONE) || (status & NOTBUSY))
	{
		/* Read/Write data done for DMAC */
		if (hsmci->cmd == MMC_STOP_TRANSMISSION)	/* For multiblock stop */
		{

#ifdef AT91SAM9G45_R1BWORKAROUND
			int timeout = 0;
			/* 
			 * R1B busy timing workaround:
			 * According to AT91Sam9G45 Errata, we have to use pio mode to poll the mci_d0 line
			 * to check the busy status of D0 line.
			 * According to datasheet Table 35-2, the I/O line of mci0_da0 is pa2.
			 * According to datasheet Table 35-2, the I/O line of mci1_da0 is pa23.
			 */
			timeout = 0;
			while (!((*pioa_pdsr)&hsmci->da0_mask))
			{
				nanosleep( &uswait, NULL);
				timeout++;
			}

			if (timeout > TIMEOUT_LOOPS)
				intr |= MMC_INTR_ERROR;
#endif

			intr |= MMC_INTR_COMMAND;
			resp[0] = READ32(MCI_RSPR0);
		}
		else									/* For other cases */
		{
			intr |= MMC_INTR_DATA;
		}
	}

	return intr;
}

void clearFIFO (SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;
	int		i;
	int		mcisr, mcififo;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

	/* Keep read the FIFO until the FIFO is empty */
	for (i=0;i<256;i++)
	{
		mcisr = READ32(MCI_SR);
		mcififo = READ32(MCI_FIFO);
		if (mcisr & FIFOEMPTY) break;
	}
}

/*
 * setup DMA transfer
 */
static int _hsmci_setup_dma (SIM_HBA *hba, paddr_t paddr, int len, int dir)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;
	uint32_t	xlen;
	uint32_t	mr;
	uint16_t	blkcnt;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

	hsmci->dmac_dev->dir = dir;

	blkcnt = len / hsmci->blksz;
	xlen = hsmci->blksz * blkcnt;

	if (blkcnt == 0)
	{
		return 0;
	}

	hsmci->blknum = blkcnt;

	mr = READ32(MCI_MR) & 0xffff;
	WRITE32(MCI_MR, mr | (hsmci->blksz << 16));
	WRITE32(MCI_BLKR, (hsmci->blksz << 16) | blkcnt);

	WRITE32(MCI_DMA, (READ32(MCI_DMA) | DMAEN));

	if (dmac_setup_xfer (hsmci->dmac_dev, paddr, xlen, dir) == -1)
	{
		return -1;
	}

	return (xlen);
}

static int _hsmci_dma_done (SIM_HBA *hba, int dir)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

	/* Disable DMA */
	dmac_xfer_stop(hsmci->dmac_dev);
	WRITE32(MCI_DMA, (READ32(MCI_DMA) & (~(DMAEN))));

	/* Disable all incoming interrupt */
	WRITE32 (MCI_IDR, 0xffffffff);

	return MMC_SUCCESS;
}

/*
 * setup PIO transfer
 */
static int _hsmci_setup_pio(SIM_HBA *hba, int len, int dir)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;
	int blkcnt, mr, sts;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

	blkcnt = len / hsmci->blksz;

	READ32(MCI_SR);
	sts = hsmci->dmac_dev->dmac->ebcisr;

	clearFIFO (hba);

	mr = READ32(MCI_MR) & 0xffff;
	WRITE32(MCI_MR, mr | (hsmci->blksz << 16));
	WRITE32(MCI_BLKR, (hsmci->blksz << 16) | blkcnt);

	return len;
}

/* 
 * The real PIO transfer.
 * Note:
 * We only use PIO to read SCR and check/switch high speed mode, so only read operation is possible
 */

static int _hsmci_pio_done(SIM_HBA *hba, char *buf, int len, int dir)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;
	int		i;
    int timeout = TIMEOUT_LOOPS;
	uint32_t 	*buf32 = (uint32_t *) buf;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;
	
    // Make sure the data is ready
    while (!(READ32(MCI_SR) & RXRDY) && timeout--);

	if (dir == DATA_READ)
	{
		for (i = 0; i < len; i+=4)
		{
			*buf32++ = READ32(MCI_RDR);
			delay(5);
		}
	}
	else 
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "%s: We only support PIO read\n", __func__);

		return -1;
	}

	clearFIFO (hba);

	return len;
}

static int _hsmci_command (SIM_HBA *hba, mmc_cmd_t *cmd)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;
	uint32_t	command;
	uint32_t	mask = 0;
	int		mcisr;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

	command = cmd->opcode;
	hsmci->cmd = cmd->opcode;

	if (cmd->resp & MMC_RSP_PRESENT) {	/* If need response */
		command |= MAXLAT;		/* Wait maximum time */
		if (cmd->resp & MMC_RSP_BUSY)
		{
			command |= RSPTYPE_R1B;
		}
		else if (cmd->resp & MMC_RSP_136) 
		{
			command |= RSPTYPE_136;
		}
		else 
		{
			command |= RSPTYPE_48;
		}
	} 
	else 
	{
		command |= RSPTYPE_NONE;
	}

	if (cmd->opcode == MMC_STOP_TRANSMISSION)
	{
		command |= (TRCMD_STOP | TRTYP_MB);
	}
	else if (cmd->eflags & MMC_CMD_DATA) 
	{
		WRITE32(MCI_MR, ((READ32(MCI_MR) | WRPROOF| RDPROOF | BLK_LENGTH << 16)));

		if (cmd->eflags & MMC_CMD_DATA_IN) 
		{
			command |= (TRDIR | TRCMD_START);       
		}
		else 
		{
			command |= TRCMD_START;
		}

		if(cmd->eflags & MMC_CMD_DATA_MULTI) 
		{
			command |= TRTYP_MB; 
		}
		else {
			command |= TRTYP_SB;
		}
	} 
        
	/* Set the arguments and send the command */
	if (cmd->resp & MMC_RSP_BUSY) 
	{
		mask |= NOTBUSY;
	}
	else if (!(cmd->eflags & MMC_CMD_DATA)) 
	{
		mask |= CMDRDY; 
	}
	else
	{
		if (command & TRCMD_START) 
		{
			mask |= CMDRDY;
		}
	}

	/* Send the command */
	WRITE32(MCI_ARGR, cmd->argument);
	WRITE32(MCI_CMDR, command);

	mcisr = READ32(MCI_SR);

	/* Enable selected interrupts */
	WRITE32(MCI_IER, MCI_ERRORS | mask);

	return (MMC_SUCCESS);
}

static int _hsmci_cfg_bus (SIM_HBA *hba, int width, int mmc)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;
	uint32_t	value;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;
	
	/* Clean Bit 6 and 7 SDCBUS flag in SDCR register */
	value = READ32 (MCI_SDCR) & 0xFFFFFF3F;
	
	if (width == 8) 
	{
		WRITE32 (MCI_SDCR, value | SDCBUS8B);
	}
	else if (width == 4) 
	{
		WRITE32 (MCI_SDCR, value | SDCBUS4B);
	}
	else
	{
		WRITE32 (MCI_SDCR, value);
	}
	
	return (MMC_SUCCESS);
}

static int _hsmci_clock (SIM_HBA *hba, int *clock, int high_speed)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;
	uint32_t	div;
	uint32_t	value;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

	/* Make sure the *clock will not exceed the boundary */
	if (*clock > hsmci->clock ) 
	{
		*clock = hsmci->clock;
	}
	
	if (*clock < (hsmci->clock/512))
	{
		*clock = hsmci->clock / 512;
	}
	
	/* Calculate divider */
	div = (hsmci->clock/ *clock) / 2 - 1;

	if (div > 255) 
	{
		div = 255;
	}

	/* Calculate the real clock we will use */
	*clock = hsmci->clock/(2*(div+1));

	/* Clean up and set the divider */
	value = READ32 (MCI_MR) & 0xffffff00;
	WRITE32 ( MCI_MR, value | div);

	delay (10);
	if (high_speed)
	{
		WRITE32 ( MCI_CFG, READ32 (MCI_CFG) | HSMODE);
	}
	delay (10);

	return (MMC_SUCCESS);
}

static int _hsmci_block_size (SIM_HBA *hba, int blksz)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;
	uint32_t	value;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

	if (blksz > BLK_LENGTH)
	{
		return (MMC_FAILURE);
	}
		
	hsmci->blksz = blksz;
	hsmci->dmac_dev->blksz = blksz;

	value = READ32 (MCI_MR) & 0xffff;
	WRITE32 (MCI_MR, value | (blksz << 16));
	
	return (MMC_SUCCESS);
}

/*
 * Reset host controller and card
 * The clock should be enabled and set to minimum (<400KHz)
 */
static int _hsmci_powerup (SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;

	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

    /* Enable the pheripheral clock             */
    out32 (hsmci->pmc_base + PMC_PCER, 1 << hba->cfg.IRQRegisters[0]);

    /* Set the slow clock for initialization         */
    WRITE32 (MCI_MR, 0xff | WRPROOF| RDPROOF);

    /* Minimal delay 1 ms and 74 clock cycles   */
    delay(2);

    /* Set Timeout to Max */
    WRITE32(MCI_DTOR, 0x7f);

	return (MMC_SUCCESS);
}

static int _hsmci_powerdown (SIM_HBA *hba)
{
	CONFIG_INFO	*cfg;
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;

	ext = (SIM_MMC_EXT *)hba->ext;
	cfg = (CONFIG_INFO *)&hba->cfg;
	hsmci = (hsmci_ext_t *)ext->handle;

    /*  Disable peripheral clock    */
    out32 (hsmci->pmc_base + PMC_PCDR, 1 << hba->cfg.IRQRegisters[0]);

	return (MMC_SUCCESS);
}

static int _hsmci_shutdown (SIM_HBA *hba)
{
	CONFIG_INFO	*cfg;
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci;
	uintptr_t	base;

	ext = (SIM_MMC_EXT *)hba->ext;
	cfg = (CONFIG_INFO *)&hba->cfg;
	hsmci = (hsmci_ext_t *)ext->handle;
	base = hsmci->base;

	/* Reset and disable controller */
	WRITE32 (MCI_CR, SWRST|PWSDIS);
	delay (100);
	WRITE32 (MCI_CR, MCIDIS | PWSDIS);

	/* Indicate we are done with DMA lib */
	dmac_fini(hsmci->dmac_dev);

#ifdef AT91SAM9G45_R1BWORKAROUND
	munmap_device_memory ((void *)pioa_pdsr, 4);
#endif

	munmap_device_memory ((void *)hsmci->base, cfg->MemLength[0]);
    munmap_device_io (hsmci->pio_base, AT91SAM9G45_PIO_SIZE);
    munmap_device_io (hsmci->pmc_base, PMC_SIZE);

	free(hsmci);

	return (MMC_SUCCESS);
}

/* MCI args */
static char *opts[] = 
{
	"port",		// The HSMCI port # to use, can be 0 for MCI0, or 1 for MCI1
	NULL
};

static int hsmci_args (SIM_HBA *hba, char *options)
{
	SIM_MMC_EXT *ext;
	hsmci_ext_t   *hsmci;
	char        *value;
	int         opt;
	int         val;
	int         idx;
	
	ext = (SIM_MMC_EXT *)hba->ext;
	hsmci = (hsmci_ext_t *)ext->handle;

	for (idx = 0; idx < strlen(options); idx++)
	{
		if (':'==options[idx]) 
		{
			options[idx] = ',';
		}
	}

	strlwr(options);

	if (*options == '\0') {	
		return (0);
	}

	while (*options != '\0') {
		if ((opt = getsubopt(&options, opts, &value)) == -1) {
			continue;
		}
		switch (opt) {
			case 0:
				val = strtoull(value, 0, 0);
				if ((val > 1) || (val < 0)) {
					slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, 
						"MMC: wrong MCI port option %d", val);
					slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, 
						"MMC: G45 MCI port must be 0 or 1");
				}
				else {
					hsmci->port = val;
				}
				break;
			default:
				slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, 
					"MMC: Unrecognized options %s\n", value);
				return (-1);
		}
	}

	return (0);
}

int hsmci_init (SIM_HBA *hba)
{
	CONFIG_INFO	*cfg;
	SIM_MMC_EXT	*ext;
	hsmci_ext_t	*hsmci = NULL;
	uintptr_t	base;

	ext = (SIM_MMC_EXT *)hba->ext;
	cfg = (CONFIG_INFO *)&hba->cfg;
	hba->verbosity = 4;

	if (!ext->opts) 
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: missing board-specific options\n");
		goto ARGSERR;
	}
	
	if ((hsmci = calloc(1, sizeof(hsmci_ext_t))) == NULL)
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: alloc memory failed\n");
		goto ERR;
	}

	cfg->MemLength[0] = 0x1000;
	cfg->NumMemWindows = 1;
	cfg->MemBase[0] = cfg->IOPort_Base[0];

	base = (uintptr_t)mmap_device_memory(NULL, cfg->MemLength[0],
		PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, cfg->MemBase[0]);

	if (base == (uintptr_t)MAP_FAILED) 
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: mmap_device_memory failed\n");
		goto ERR;
	}

	hsmci->clock     = 133000000;
	hsmci->base      = base;
	hsmci->hba       = hba;
	ext->handle    = hsmci;
	ext->clock     = hsmci->clock;
	ext->detect    = _hsmci_detect;
	ext->powerup   = _hsmci_powerup;
	ext->powerdown = _hsmci_powerdown;
	ext->cfg_bus   = _hsmci_cfg_bus;
	ext->set_clock = _hsmci_clock;
	ext->set_blksz = _hsmci_block_size;
	ext->interrupt = _hsmci_interrupt;
	ext->command   = _hsmci_command;
	ext->setup_dma = _hsmci_setup_dma;
	ext->dma_done  = _hsmci_dma_done;
	ext->setup_pio = _hsmci_setup_pio;
	ext->pio_done  = _hsmci_pio_done;
	ext->shutdown  = _hsmci_shutdown;

	/* Parse options */
	hsmci->port = -1;
	hsmci->blksz = BLK_LENGTH;
	hsmci->slot = 0;

	/* Hardcode DMAC controller base address according to G45 datasheet
	 * since this driver is specifically for G45 SoC */
	hsmci->dbase = DMAC_BASE;

	if (!ext->opts)
	{
		goto ARGSERR;
	}

	if (hsmci_args(hba, ext->opts) == -1)
	{
		goto ARGSERR;
	}

	/* 
	 * Set Src/Dst Request peripheral identifier SRC_PER/DST_PER 
	 * handshaking interface # according to Table 41-1 DMA Channel Definition
	 * According to datasheet Table 35-2, the I/O line of mci0_da0 is pa2.
	 * According to datasheet Table 35-2, the I/O line of mci1_da0 is pa23.
	 */
	if (hsmci->port == 0 ) 
	{
		hsmci->dintf = 0;

#ifdef AT91SAM9G45_R1BWORKAROUND
		hsmci->da0_mask = (1<<2);
#endif

	}
	else 
	{
		hsmci->dintf=13;

#ifdef AT91SAM9G45_R1BWORKAROUND
		hsmci->da0_mask = (1<<23);
#endif

	}

#ifdef AT91SAM9G45_R1BWORKAROUND 
	/* Map G45 PIOA for polling busy signal */
	pioa_pdsr = (uint32_t *)mmap_device_memory(NULL, 4,
		PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, PIOA_PDSR);	

	if (pioa_pdsr == (uint32_t *)MAP_FAILED)
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: mmap_device_memory failed\n");
		goto ERR;
	}
#endif

   if ( (uintptr_t) MAP_FAILED == ( hsmci->pio_base = mmap_device_io( AT91SAM9G45_PIO_SIZE, AT91SAM9G45_PIOD_BASE ) ) ) {
        slogf( _SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: mmap_device_io for PIOD_PDSR failed" );
        goto ERR;
    }

    /* Configure CD and WP PIO */
    InterruptDisable();
#define CFG_PIO(reg)    out32( hsmci->pio_base + (reg), AT91SAM9G45_MCI_PIO_BITS )
    CFG_PIO( AT91SAM9G45_PIO_PER );     /* Ensable PIO */
    CFG_PIO( AT91SAM9G45_PIO_ODR );     /* Disable output */
    CFG_PIO( AT91SAM9G45_PIO_IFDR );        /* Disable glitch input filter */
    CFG_PIO( AT91SAM9G45_PIO_CODR );        /* Clear output data */
    CFG_PIO( AT91SAM9G45_PIO_IDR );     /* Disable interrupt */
    CFG_PIO( AT91SAM9G45_PIO_MDDR );        /* Disable multi-driver */
    CFG_PIO( AT91SAM9G45_PIO_PUER );        /* Enable pull-up */
    CFG_PIO( AT91SAM9G45_PIO_OWDR );        /* Output write disable */
#undef CFG_PIO
    InterruptEnable();

    /* Configure capacity of controller */
#ifdef AT91SAM9G45_R1BWORKAROUND
	ext->hccap |= MMC_HCCAP_BW1 | MMC_HCCAP_BW4 | MMC_HCCAP_DMA | MMC_HCCAP_BW8 | MMC_HCCAP_HS;
#else	// Use the flag MMC_HCCAP_NOCD_BUSY to inform the MMCSD stack that this hardware has R1B bug
	ext->hccap |= MMC_HCCAP_BW1 | MMC_HCCAP_BW4 | MMC_HCCAP_DMA | MMC_HCCAP_BW8 | MMC_HCCAP_HS | MMC_HCCAP_NOCD_BUSY;
#endif

	/* Disable the controller and soft reset */
	WRITE32(MCI_CR, SWRST | PWSDIS);
	delay (100);
	WRITE32(MCI_CR, MCIDIS | PWSDIS); 

	/* Disable DMA */
	WRITE32(MCI_DMA, (READ32(MCI_DMA) & (~(DMAEN))));

	/* Enable the controller */
	WRITE32(MCI_CR, MCIEN | PWSDIS);
	WRITE32(MCI_IDR, 0xffffffff);

	/* Set Timeout to Max */
	WRITE32(MCI_DTOR, 0x7f);

	/* Use the lowest baudrate */
	WRITE32 (MCI_MR, 0xff | WRPROOF| RDPROOF);
		
	hsmci->dmac_dev = dmac_init(hsmci);

	if (hsmci->dmac_dev == NULL)
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: dmafuncs init FAILED\n");
		goto ERR;
	}

	hsmci->dmac_dev->io_addr = cfg->MemBase[0] + MCI_FIFO;
	hsmci->dmac_dev->blksz = hsmci->blksz;

	/* Select slot, set bus to 1 bit */
	WRITE32 (MCI_SDCR, hsmci->slot);

	if (!cfg->Description[0])
		strncpy(cfg->Description, "Atmel HSMCI ", sizeof(cfg->Description));

   if ( (uintptr_t) MAP_FAILED == ( hsmci->pmc_base = mmap_device_io(PMC_SIZE, PMC_BASE) ) ) {
        slogf( _SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: mmap_device_io for PMC failed" );
        goto ERR;
    }

	return (MMC_SUCCESS);

ARGSERR:
	printf("\nImproper board-specific options used. Accepting args: \n");
	printf("    port=#       The MCI port been used (0 or 1)\n");
	printf("NOTE:\n");
	printf("    1. The args are seperated by colon ':'\n");
	printf("Example:\n");
	printf("at91sam9g45 port 0: devb-mmcsd-at91sam9g45 mmcsd ioport=0xFFF80000,irq=11,bs=port=0\n");
	printf("at91sam9g45 port 1: devb-mmcsd-at91sam9g45 mmcsd ioport=0xFFFD0000,irq=29,bs=port=1\n");

ERR:
	if (hsmci)
	{
		munmap_device_memory ((void *)hsmci->base, (uint32_t)cfg->MemLength[0]);

		if (hsmci->pio_base)
    		munmap_device_io (hsmci->pio_base, AT91SAM9G45_PIO_SIZE);

		free (hsmci);
	}

	if (pioa_pdsr != (uint32_t *)MAP_FAILED)
		munmap_device_memory ((void *)pioa_pdsr, 4);

	return (MMC_FAILURE);
}

#endif

