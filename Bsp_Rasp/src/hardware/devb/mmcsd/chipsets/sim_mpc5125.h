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

#ifndef	MPC5125_H_INCLUDED
#define	MPC5125_H_INCLUDED

#include	<hw/dma.h>
#include	<hw/sysinfo.h>

#define	MPC5125_SD_BASE1		0x1500
#define	MPC5125_SD_BASE2		0x5100
#define	MPC5125_IOCTL_BASE		0xa000
#define	MPC5125_DMA_BASE		0x14000
#define	MPC5125_CPLD_BASE		0x2000000

#define SDDATA_BUFFER_SIZE		64		/* Data buffer(X/Y) size is 64 bytes */
#define	MPC5125_MMC_TMOUT		100000

#define	IOCTL_SLEW0				0
#define	IOCTL_SLEW1				1
#define	IOCTL_SLEW2				2
#define	IOCTL_SLEW3				3
#define	IOCTL_PUE				(1 << 3)
#define	IOCTL_PUD				(1 << 4)
#define	IOCTL_FUNCMUX1			0
#define	IOCTL_FUNCMUX2			(1 << 5)
#define	IOCTL_FUNCMUX3			(2 << 5)
#define	IOCTL_FUNCMUX4			(3 << 5)

#define	CPLD_INT_MASK			13
#define	CPLD_INT_STAT			14
#define	CARD_NOT_PRESENT		0x40

/* Register definitions */

#define	STR_STP_CLK			0x00		/* SDHC Clock Control Register */
	#define	STOP_CLK		(1 << 0)
	#define	START_CLK		(1 << 1)
	#define	SDHC_RESET		(1 << 3)
#define	STATUS				0x04		/* SDHC Status Register */
	#define STATUS_CARDINSERTION      (1<<31)	/* Card insertion status */
	#define STATUS_CARDREMOVAL        (1<<30)	/* Card removal status */
	#define STATUS_YBUFEMPTY          (1<<29)	/* Data buffer Y empty status */
	#define STATUS_XBUFEMPTY          (1<<28)	/* Data buffer X empty status */
	#define STATUS_YBUFFULL           (1<<27)	/* Data buffer Y full status */
	#define STATUS_XBUFFULL           (1<<26)	/* Data buffer X full status */
	#define STATUS_BUFUNDRUN          (1<<25)	/* Data buffer underrun status */
	#define STATUS_BUFOVFL            (1<<24)	/* Data buffer overflow status */
	#define STATUS_SDIOINTACTIVE      (1<<14)	/* SDIO interrupt active status */
	#define STATUS_ENDCMDRES          (1<<13)	/* End of command response status */
	#define STATUS_WRITEOPDONE        (1<<12)	/* Write operation done status */
	#define STATUS_READOPDONE         (1<<11)	/* Read operation done status */
	#define STATUS_WRCRCERRORCODE     (3<<9)	/* Write operation CRC error code */
	#define STATUS_CARDBUSCLKRUN      (1<<8)	/* SD clock run status */
	#define STATUS_BUFREADREADY       (1<<7)	/* Data buffer(X/Y) ready for read status */
	#define STATUS_BUFWRITEREADY      (1<<6)	/* Data buffer(X/Y) ready to be written status */
	#define STATUS_RESPCRCERR         (1<<5)	/* Command response CRC error status */
	#define STATUS_READCRCERR         (1<<3)	/* Read operation CRC error status */
	#define STATUS_WRITECRCERR        (1<<2)	/* Write operation CRC error status */
	#define STATUS_TIMEOUTRESP        (1<<1)	/* Command response timeout status */
	#define STATUS_TIMEOUTREAD		  (1<<0)	/* Read operation timeout status */
#define	CLK_RATE			0x08		/* SDHC Clock Rate Register */
#define	CMD_DAT_CONT		0x0c		/* SDHC Command and Data Control Register */
	#define CMDDATCONT_CMDRESUME            (1<<15)	/* Issue command to card control*/
	#define CMDDATCONT_CMDRESPLONGOFF       (1<<12)	/* Command response long off control */
	#define CMDDATCONT_STOPREADWAIT         (1<<11)	/* stop read/wait control (used in SDIO card interface) */
	#define CMDDATCONT_STARTREADWAIT        (1<<10)	/* start read/wait  (used in SDIO card interface) */
	#define CMDDATCONT_BUSWIDTH             (2<<8)	/* Bus width control */
	#define CMDDATCONT_INIT                 (1<<7)	/* enable/disable 80 initialization clock prefix to each command */
	#define CMDDATCONT_WRITEREAD            (1<<4)	/* Write/read control */
	#define CMDDATCONT_DATAENABLE           (1<<3)	/* Data enable/disable control */
	#define CMDDATCONT_FORMATOFRESPONSE     (1<<0)	/* Command response type: 48 bit response with crc check (R1/R1b/R5/R6) */
	#define CMDDATCONT_FORMATOFRESPONSE1    (1<<1)	/* Command response type: 136 bit response (R2) */
	#define CMDDATCONT_FORMATOFRESPONSE2    (3<<0)	/* Command response type: 48 bit response without CRC check (R3/R4) */
#define	RES_TO				0x10		/* SDHC Responce Time Out Register */
#define	READ_TO				0x14		/* SDHC Read Time Out Register */
#define	BLK_LEN				0x18		/* SDHC Block Length Register */
#define	NOB					0x1c		/* SDHC Number of Blocks Regsiter */
#define	REV_NO				0x20		/* SDHC Revision Number Register */
#define	INT_CNTR			0x24		/* SDHC Interrupt Control Register */
	#define INTCNTR_CARDINSERTIONEN         (1<<15)	/* card insertion interrupt enable */
	#define INTCNTR_CARDREMOVALEN           (1<<14)	/* card removal interrupt enable */
	#define INTCNTR_SDIOINTEN               (1<<13)	/* SDIO interrrupt enable */
	#define INTCNTR_DAT0EN                  (1<<12)	/* Data enable (used in SDIO card interface) */
	#define INTCNTR_BUFREADEN               (1<<4)	/* Data buffer(X/Y) ready for read interrupt enable */
	#define INTCNTR_BUFWRITEEN              (1<<3)	/* Data buffer(X/Y) ready to be written interrupt enable */
	#define INTCNTR_ENDCMDRES               (1<<2)	/* End of command response interrupt enable */
	#define INTCNTR_WRITEOPDONE             (1<<1)	/* Write operation done interrupt enable */
	#define INTCNTR_READOPDONE              (1<<0)	/* Read operation done interrupt enable */
#define	CMD					0x2c		/* SDHC CMD Argument Register */
#define	RES_FIFO			0x34		/* SDHC Response FIFO Access Register */
#define	DBA					0x38		/* SDHC Data Buffer Access Register */

typedef struct {
	volatile uint32_t	sd_str_stp_clk;	 /* 0x00 */
	volatile uint32_t	sd_status;
	volatile uint32_t	sd_clk_rate;
	volatile uint32_t	sd_cmd_dat_cont;

	volatile uint32_t	sd_res_to;		 /* 0x10 */
	volatile uint32_t	sd_read_to;
	volatile uint32_t	sd_blk_len;
	volatile uint32_t	sd_nob;
	
	volatile uint32_t	sd_rev_no;		 /* 0x20 */
	volatile uint32_t	sd_int_cntr;		
	volatile uint32_t	sd_cmd;		
	volatile uint32_t	sd_arg;
		
    volatile uint32_t	sd_reserved;     /* 0x30 */
	volatile uint32_t	sd_res_fifo;   
	volatile uint32_t	sd_buffer_access;
} mpc5125_mmc_reg;

typedef	struct {
	uint32_t		mmc_pbase;
	uintptr_t		mmc_base;
	uintptr_t		ioc_base;
	uintptr_t		dma_base;
	uintptr_t		cpld_base;
	int				blksz;
	uint32_t		bus_width;
	uint32_t		clock;
    void			*hba;
	dma_functions_t	sdmafuncs;
	struct sigevent	event;
	timer_t			timer;
	void			*dma_chn;
	void			*dma_pend;
	uint32_t		dmask;		// Interrupt mask for data transfer
	int				card_pres;
	int				dmasize;
	paddr_t			paddr;
	uint32_t		len;
	uint32_t		*vaddr;
} mpc5125_ext_t;

int		mpc5125_attach (SIM_HBA *hba, int dmasize);

#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_mpc5125.h $ $Rev: 657836 $" )
