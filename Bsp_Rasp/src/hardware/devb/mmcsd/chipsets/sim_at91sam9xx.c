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
 
#include <sim_at91sam9xx.h>
#include <sys/trace.h>

#define TIMEOUT_LOOPS 10000

static int stopdma (SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "stopdma()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	WRITE32(MCI_PTCR, MCI_TXTDIS | MCI_RXTDIS);
	WRITE32(MCI_RPR, 0);
	WRITE32(MCI_RCR, 0);
	WRITE32(MCI_RNPR, 0);
	WRITE32(MCI_RNCR, 0);
	WRITE32(MCI_TPR, 0);
	WRITE32(MCI_TCR, 0);
	WRITE32(MCI_TNPR, 0);
	WRITE32(MCI_TNCR, 0);

	return 0;
}

static int startdma (SIM_HBA *hba, int dir)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "startdma()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	if (dir == DATA_READ)
	{
		WRITE32(MCI_PTCR, MCI_RXTEN);
	}
	else if (dir == DATA_WRITE)
	{
		WRITE32 (MCI_PTCR, MCI_TXTEN);
	}
	else
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: DMA transfer neither READ or WRITE\n");
		return -1;
	}
	
	return 0;
}


static int _mci_detect (SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_detect()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	/* card detection pin is not available */
	
	return (MMC_SUCCESS);
}

static int _mci_interrupt (SIM_HBA *hba, int irq, int resp_type, uint32_t *resp)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;
	uint32_t	status, status2;
	int		intr;
	
	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_interrupt()");
	intr = MMC_INTR_NONE;

	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	status2 = READ32(MCI_SR);
	status = status2 & READ32(MCI_IMR);

	/* Disable incoming interrupt */
	WRITE32 (MCI_IDR, status);

	/*
	 * If there is error status and it is not a CRC error for R3, 
	 * since R3 command doesn't require CRC check.
	 */
	if ( (status2 & MCI_ERRORS) 
	&& (! ((resp_type == MMC_RSP_R3) && ((status2 & MCI_ERRORS) == RCRCE))))
	{
		intr |= MMC_INTR_ERROR;

		if (status2 & RINDE ) {
			intr |= MMC_ERR_CMD_IDX;
		}

		if (status2 & RCRCE ) {
			intr |= MMC_ERR_CMD_CRC;
		}

		if (status2 & RENDE ) {
			intr |= MMC_ERR_CMD_END;
		}

		if (status2 & RTOE ) {
			intr |= MMC_ERR_CMD_TO;
		}

		if (status2 & DCRCE ) {
			intr |= MMC_ERR_DATA_CRC;
		}

		if (status2 & DTOE ) {
			intr |= MMC_ERR_DATA_TO;
		}

		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: Error detected on MCI_SR 0x%x", status2);
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

		if (mci->cmd == MMC_WRITE_BLOCK)	/* For single block write */
		{
			WRITE32(MCI_IER, MCI_ERRORS | NOTBUSY);
			startdma(hba, DATA_WRITE);
		}
		else if (mci->cmd == MMC_WRITE_MULTIPLE_BLOCK)	/* For multiple block write */
		{
			WRITE32(MCI_IER, MCI_ERRORS | BLKE);
			startdma(hba, DATA_WRITE);
		}
	} 
	else if ((status & ENDRX) || (status & NOTBUSY) )
	{
		/* Read/Write data done for PDC */
		if (mci->cmd == MMC_STOP_TRANSMISSION)	/* For multiblock */
		{
			intr |= MMC_INTR_COMMAND;
			resp[0] = READ32(MCI_RSPR0);
		}
		else					/* For other case */
		{
			intr |= MMC_INTR_DATA;
		}
	}
	else if (status & BLKE)
	{
		/* Multiblock write done */
		intr |= MMC_INTR_DATA;
	}
	else
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: Nothing to do. MCI_SR 0x%x", status2);
	}


	return intr;
}

/*
 * setup DMA transfer
 */
static int _mci_setup_dma (SIM_HBA *hba, paddr_t paddr, int len, int dir)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;
	uint32_t	xlen = 0;
	uint32_t	mr;
	uint16_t	blkcnt;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_setup_dma()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	mci->pio_enable = 0;

	if (dir == DATA_READ)
	{
		blkcnt = len / mci->blksz;
		xlen = mci->blksz * blkcnt;

		if (blkcnt == 0)
		{
			return 0;
		}

		mr = READ32(MCI_MR) & 0xffff;
		WRITE32(MCI_MR, mr | (mci->blksz << 16));
		WRITE32(MCI_BLKR, blkcnt | (mci->blksz << 16));

		mci->blknum = blkcnt;

		WRITE32(MCI_RPR, paddr);
		WRITE32(MCI_RCR, xlen / 4);
	} 
	else if (dir == DATA_WRITE)
	{
		blkcnt = len / mci->blksz;
		xlen = mci->blksz * blkcnt;

		if (blkcnt == 0)
		{
			return 0;
		}

		mr = READ32(MCI_MR) & 0xffff;
		WRITE32(MCI_MR, mr | (mci->blksz << 16));
		WRITE32(MCI_BLKR, blkcnt | (mci->blksz << 16));

		mci->blknum = blkcnt;
		WRITE32(MCI_TPR, paddr);
		WRITE32(MCI_TCR, xlen / 4);
	}

	return (xlen);
}

static int _mci_dma_done (SIM_HBA *hba, int dir)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_dma_done()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	/* Disable DMA */
	stopdma (hba);
	
	/* Disable all incoming interrupt */
	WRITE32 (MCI_IDR, 0xffffffff);

	return MMC_SUCCESS;
}

/*
 * setup PIO transfer
 */
static int _mci_setup_pio(SIM_HBA *hba, int len, int dir)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;
	int		blkcnt, mr;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_setup_pio()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	blkcnt = len / mci->blksz;

	mci->pio_enable = 1;

	READ32(MCI_SR);

	mr = READ32(MCI_MR) & 0xffff;
	WRITE32(MCI_MR, mr | (mci->blksz << 16));
	WRITE32(MCI_BLKR, (mci->blksz << 16) | blkcnt);

	return len;
}

/* 
 * The real PIO transfer.
 * Note:
 * We only use PIO to read SCR and check/switch high speed mode, so only read operation is possible
 */
static int _mci_pio_done(SIM_HBA *hba, char *buf, int len, int dir)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;
	int		i;
	uint32_t 	*buf32 = (uint32_t *) buf;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_pio_done()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	delay(10);

	if (dir == DATA_READ)
	{
		for (i = 0; i < len; i+=4)
		{
			*buf32 = READ32(MCI_RDR);
			delay(1);
			buf32++;
		}
	}
	else 
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: We donot support PIO write\n");

		return -1;
	}

	mci->pio_enable = 0;

	delay(10);

	return len;
}


static int _mci_command (SIM_HBA *hba, mmc_cmd_t *cmd)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;
	uint32_t	command;
	uint32_t	mask = 0;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_command()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	command = cmd->opcode;
	mci->cmd = cmd->opcode;

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
	} else 
	{
		command |= RSPTYPE_NONE;
	}
	if (cmd->opcode == MMC_STOP_TRANSMISSION)
	{
		command |= (TRCMD_STOP | TRTYP_MB);
	}
	else if (cmd->eflags & MMC_CMD_DATA) 
	{
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
	} else 
	{
		if (command & TRCMD_START) 
		{
				if (command & TRDIR) 
				{
					mask |= ENDRX | NOTBUSY;
				}
				else 
				{
					mask |= CMDRDY | NOTBUSY;
				}
		}
	}

	/* If command is READ DATA then enable the DMA */
	if ((command & TRCMD_START) && (command & TRDIR) && (mci->pio_enable != 1))
	{
		startdma (hba, DATA_READ);
	}

	/* Send the command */
	WRITE32(MCI_ARGR, cmd->argument);
	WRITE32(MCI_CMDR, command);

	/* Enable selected interrupts */
	WRITE32(MCI_IER, MCI_ERRORS | mask);

	return (MMC_SUCCESS);
}

static int _mci_cfg_bus (SIM_HBA *hba, int width, int mmc)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;
	uint32_t	value;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_cfg_bus()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;
	
	value = READ32 (MCI_SDCR) & 0xFFFFFF7F;
	
	if (width == 4) 
	{
		WRITE32 (MCI_SDCR, value | SDCBUS4B);
	}
	else
	{
		WRITE32 (MCI_SDCR, value);
	}
	
	return (MMC_SUCCESS);
}

static int _mci_clock (SIM_HBA *hba, int *clock, int high_speed)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;
	uint32_t	div;
	uint32_t	value;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_clock()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	/* Make sure the *clock will not exceed the boundary */
	if (*clock > mci->clock/2 ) 
	{
		*clock = mci->clock/2;
	}
	
	if (*clock < (mci->clock/512))
	{
		*clock = mci->clock / 512;
	}
	
	/* Calculate divider */
	div = (mci->clock/ *clock) / 2 - 1;

	if (div > 255) 
	{
		div = 255;
	}

	/* Calculate the real clock we will use */
	*clock = mci->clock/(2*(div+1));

	/* Clean up and set the divider */
	value = READ32 (MCI_MR) & 0xffffff00;
	WRITE32 ( MCI_MR, value | div);

	return (MMC_SUCCESS);
}

static int _mci_block_size (SIM_HBA *hba, int blksz)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;
	uint32_t	value;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_block_size()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	if (blksz > BLK_LENGTH)
	{
		return (MMC_FAILURE);
	}
		
	mci->blksz = blksz;
	value = READ32 (MCI_MR) & 0xffff;
	WRITE32 (MCI_MR, value | blksz);
	
	return (MMC_SUCCESS);
}

/*
 * Reset host controller and card
 * The clock should be enabled and set to minimum (<400KHz)
 */
static int _mci_powerup (SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_powerup()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	return (MMC_SUCCESS);
}

static int _mci_powerdown (SIM_HBA *hba)
{
	CONFIG_INFO	*cfg;
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_powerdown()");
	ext = (SIM_MMC_EXT *)hba->ext;
	cfg = (CONFIG_INFO *)&hba->cfg;
	mci = (mci_ext_t *)ext->handle;

	return (MMC_SUCCESS);
}

static int _mci_shutdown (SIM_HBA *hba)
{
	CONFIG_INFO	*cfg;
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci;
	uintptr_t	base;

	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "_mci_shutdown()");
	ext = (SIM_MMC_EXT *)hba->ext;
	cfg = (CONFIG_INFO *)&hba->cfg;
	mci = (mci_ext_t *)ext->handle;
	base = mci->base;

	/* Reset and disable controller */
	WRITE32 (MCI_CR, SWRST|PWSDIS);
	delay (100);
	WRITE32 (MCI_CR, MCIDIS | PWSDIS);

	munmap_device_memory ((void *)mci->base, cfg->MemLength[0]);

	free(mci);

	return (MMC_SUCCESS);
}

/* MCI args */
static char *opts[] = 
{
	"slot",		// selection of mci slot: 0 or 1
	NULL
};

static int mci_args (SIM_HBA *hba, char *options)
{
	SIM_MMC_EXT *ext;
	mci_ext_t   *mci;
	char        *value;
	int         opt;
	int         val;
	int         idx;
	
	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "mci_args()");
	ext = (SIM_MMC_EXT *)hba->ext;
	mci = (mci_ext_t *)ext->handle;

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
						"MMC: wrong MCI slot option %d", val);
					slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, 
						"MMC: MCI slot must be 0 (A) or 1 (B)");
				}
				else {
					mci->slot = val;
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

int mci_init (SIM_HBA *hba)
{
	CONFIG_INFO	*cfg;
	SIM_MMC_EXT	*ext;
	mci_ext_t	*mci = NULL;
	uintptr_t	base;

	ext = (SIM_MMC_EXT *)hba->ext;
	cfg = (CONFIG_INFO *)&hba->cfg;
	hba->verbosity = 4;
	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "mci_init()");
	if (!ext->opts) 
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: missing board-specific options\n");
		goto ARGSERR;
	}
	
	if ((mci = calloc(1, sizeof(mci_ext_t))) == NULL)
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: alloc memory failed\n");
		goto ERR;
	}

	cfg->MemLength[0] = 0x1000;
	cfg->NumMemWindows = 1;
	cfg->MemBase[0] = cfg->IOPort_Base[0];

	//printf("\n membase :%x \n",cfg->MemBase[0]);
	slogf (_SLOGC_SIM_MMC, _SLOG_INFO, "MMC:  membase: %x \n",cfg->MemBase[0]);
	base = (uintptr_t)mmap_device_memory(NULL, cfg->MemLength[0],
		PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, cfg->MemBase[0]);

	if (base == (uintptr_t)MAP_FAILED) 
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: mmap_device_memory failed\n");
		goto ERR;
	}

	mci->clock     = 50000000;
	mci->base      = base;
	mci->hba       = hba;
	ext->handle    = mci;
	ext->clock     = mci->clock;
	ext->detect    = _mci_detect;
	ext->powerup   = _mci_powerup;
	ext->powerdown = _mci_powerdown;
	ext->cfg_bus   = _mci_cfg_bus;
	ext->set_clock = _mci_clock;
	ext->set_blksz = _mci_block_size;
	ext->interrupt = _mci_interrupt;
	ext->command   = _mci_command;
	ext->setup_dma = _mci_setup_dma;
	ext->dma_done  = _mci_dma_done;
	ext->setup_pio = _mci_setup_pio;
	ext->pio_done  = _mci_pio_done;
	ext->shutdown  = _mci_shutdown;

	/* Parse options */
	mci->slot = -1;
	mci->blksz = BLK_LENGTH;

	if (mci_args(hba, ext->opts) == -1)
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: improper options used");
		goto ARGSERR;
	}

	if (mci->slot == -1 ) 
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: no slot specified");
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: select slot 0 (A)");
		mci->slot = 0;
	}
	else 
	{
		slogf (_SLOGC_SIM_MMC, _SLOG_ERROR, "MMC: select slot %d", mci->slot);
	}

	/* Configure capacity of controller */
	/* It was found the MCI controller cannot properly hold the read/write 
	 * when the card is busy. We workaround this by seting flag MMC_HCCAP_NOCD_BUSY */
	ext->hccap |= MMC_HCCAP_BW1 | MMC_HCCAP_BW4 | MMC_HCCAP_DMA | MMC_HCCAP_NOCD_BUSY;

	/* Disable the controller */
	WRITE32(MCI_CR, SWRST|PWSDIS);
	delay (100);
	WRITE32(MCI_CR, MCIDIS | PWSDIS); 

	/* Enable the controller */
	WRITE32(MCI_CR, MCIEN | PWSDIS);
	WRITE32(MCI_IDR, 0xffffffff);

	/* Set Timeout to Max */
	WRITE32(MCI_DTOR, 0x7f);

	/* Use the lowest baudrate */
	WRITE32 ( MCI_MR, 0xff | PWSDIV(3) | PDCMODE | WRPROOF | RDPROOF);

	/* Select slot, set bus to 1 bit */
	WRITE32 (MCI_SDCR, mci->slot);

	if (!cfg->Description[0])
		strncpy(cfg->Description, "Atmel MCI ", sizeof(cfg->Description));

	return (MMC_SUCCESS);

ARGSERR:
	printf("\nImproper board-specific options used. Accepting args: \n");
	printf("    slot=#       The slot been used (0 or 1)\n");
	printf("NOTE:\n");
	printf("    1. The bs args are seperated by colon ':'\n");
	printf("Example:\n");
	printf("at91sam9263 port 1: devb-mmcsd-at91sam9xx mmcsd ioport=0xFFF80000,irq=10,clock=50000000,bs=slot=0\n");
	printf("at91sam9263 port 2: devb-mmcsd-at91sam9xx mmcsd ioport=0xFFF84000,irq=11,clock=50000000,bs=slot=0\n");

ERR:
	if (mci)
	{
		munmap_device_memory ((void *)mci->base, (uint32_t)cfg->MemLength[0]);
		free (mci);
	}
	return (MMC_FAILURE);
}

#endif

