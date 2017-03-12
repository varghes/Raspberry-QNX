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

#ifndef	_IMX31_MMC_INCLUDED
#define	_IMX31_MMC_INCLUDED

#include	<arm/mx31.h>
#include	<hw/dma.h>


#define IMX31_SDDATA_BUFFER_SIZE	64			/* Data buffer(X/Y) size is 64 bytes */
#define	IMX31_MMC_TMOUT				100000

/* card detect GPIO pin's status bit in GPIO1 register */
#define IMX31_CD_STATUS				(1 << 30)	/* Bit to be checked in GPIO1 data register for card detect*/ 								// Comments: This should be at devb/mmcsd/arm since it is board-specific code
#define IMX31_WP_STATUS				(1 << 31)	/* Bit to be checked in GPIO1 data register for card's write protect status */ 				// Comments: This should be at devb/mmcsd/arm since it is board-specific code


#define IMX31_SDDATA0_GPIO			(1 << 28)	/* GPIO2 group's 28th pin (SD data0 in SD mode) used to check card status in GPIO mode*/ 	// Comments: I am not sure if we have to use GPIO mode to check card status

#define IMX31_SD_STR_STP_CLK        (0x00)      /* SDHC1 start/stop clock register */
#define IMX31_SD_STATUS             (0x04)      /* SDHC1 status register */
#define IMX31_SD_CLK_RATE           (0x08)      /* SDHC1 clock rate register */
#define IMX31_SD_CMD_DAT_CONT       (0x0C)      /* SDHC1 command/data control register */
#define IMX31_SD_RES_TO             (0x10)      /* SDHC1 response timeout register */
#define IMX31_SD_READ_TO            (0x14)      /* SDHC1 read timeout register */
#define IMX31_SD_BLK_LEN            (0x18)      /* SDHC1 block length register */
#define IMX31_SD_NOB                (0x1C)      /* SDHC1 number of blocks register */
#define IMX31_SD_REV_NO             (0x20)      /* SDHC1 revision number register */
#define IMX31_SD_INT_CNTR           (0x24)      /* SDHC1 interrupt control register */
#define IMX31_SD_CMD                (0x28)      /* SDHC1 command number register */
#define IMX31_SD_ARG                (0x2C)      /* SDHC1 command argument register */
#define IMX31_SD_RES_FIFO           (0x34)      /* SDHC1 response FIFO access register */
#define IMX31_SD_BUFFER_ACCESS      (0x38)      /* SDHC1 data buffer access register */

/* SDHC status register bit description */
#define IMX31_SDSTATUS_CARDINSERTION      (1<<31)	/* Card insertion status */
#define IMX31_SDSTATUS_CARDREMOVAL        (1<<30)	/* Card removal status */
#define IMX31_SDSTATUS_YBUFEMPTY          (1<<29)	/* Data buffer Y empty status */
#define IMX31_SDSTATUS_XBUFEMPTY          (1<<28)	/* Data buffer X empty status */
#define IMX31_SDSTATUS_YBUFFULL           (1<<27)	/* Data buffer Y full status */
#define IMX31_SDSTATUS_XBUFFULL           (1<<26)	/* Data buffer X full status */
#define IMX31_SDSTATUS_BUFUNDRUN          (1<<25)	/* Data buffer underrun status */
#define IMX31_SDSTATUS_BUFOVFL            (1<<24)	/* Data buffer overflow status */
#define IMX31_SDSTATUS_SDIOINTACTIVE      (1<<14)	/* SDIO interrupt active status */
#define IMX31_SDSTATUS_ENDCMDRES          (1<<13)	/* End of command response status */
#define IMX31_SDSTATUS_WRITEOPDONE        (1<<12)	/* Write operation done status */
#define IMX31_SDSTATUS_READOPDONE         (1<<11)	/* Read operation done status */
#define IMX31_SDSTATUS_WRCRCERRORCODE     (3<<9)	/* Write operation CRC error code */
#define IMX31_SDSTATUS_CARDBUSCLKRUN      (1<<8)	/* SD clock run status */
#define IMX31_SDSTATUS_BUFREADREADY       (1<<7)	/* Data buffer(X/Y) ready for read status */
#define IMX31_SDSTATUS_BUFWRITEREADY      (1<<6)	/* Data buffer(X/Y) ready to be written status */
#define IMX31_SDSTATUS_RESPCRCERR         (1<<5)	/* Command response CRC error status */
#define IMX31_SDSTATUS_READCRCERR         (1<<3)	/* Read operation CRC error status */
#define IMX31_SDSTATUS_WRITECRCERR        (1<<2)	/* Write operation CRC error status */
#define IMX31_SDSTATUS_TIMEOUTRESP        (1<<1)	/* Command response timeout status */
#define IMX31_SDSTATUS_TIMEOUTREAD		  (1<<0)	/* Read operation timeout status */

/* interrupt controller register bit description */
#define IMX31_SDINTCNTR_SDIOINTWKPEN            (1<<18)	/* SDIO interrupt wake-up enable */
#define IMX31_SDINTCNTR_CARDINSERTIONWKPEN      (1<<17)	/* Card insertion interrupt wake-up enable */
#define IMX31_SDINTCNTR_CARDREMOVALWKPEN        (1<<16)	/* Card removal interrupt wake-up enable */
#define IMX31_SDINTCNTR_CARDINSERTIONEN         (1<<15)	/* card insertion interrupt enable */
#define IMX31_SDINTCNTR_CARDREMOVALEN           (1<<14)	/* card removal interrupt enable */
#define IMX31_SDINTCNTR_SDIOINTEN               (1<<13)	/* SDIO interrrupt enable */
#define IMX31_SDINTCNTR_DAT0EN                  (1<<12)	/* Data enable (used in SDIO card interface) */
#define IMX31_SDINTCNTR_BUFREADEN               (1<<4)	/* Data buffer(X/Y) ready for read interrupt enable */
#define IMX31_SDINTCNTR_BUFWRITEEN              (1<<3)	/* Data buffer(X/Y) ready to be written interrupt enable */
#define IMX31_SDINTCNTR_ENDCMDRES               (1<<2)	/* End of command response interrupt enable */
#define IMX31_SDINTCNTR_WRITEOPDONE             (1<<1)	/* Write operation done interrupt enable */
#define IMX31_SDINTCNTR_READOPDONE              (1<<0)	/* Read operation done interrupt enable */

/*command data control register bit description */
#define IMX31_SDCMDDATCONT_CMDRESUME            (1<<15)	/* Issue command to card control*/
#define IMX31_SDCMDDATCONT_CMDRESPLONGOFF       (1<<12)	/* Command response long off control */
#define IMX31_SDCMDDATCONT_STOPREADWAIT         (1<<11)	/* stop read/wait control (used in SDIO card interface) */
#define IMX31_SDCMDDATCONT_STARTREADWAIT        (1<<10)	/* start read/wait  (used in SDIO card interface) */
#define IMX31_SDCMDDATCONT_BUSWIDTH             (2<<8)	/* Bus width control */
#define IMX31_SDCMDDATCONT_INIT                 (1<<7)	/* enable/disable 80 initialization clock prefix to each command */
#define IMX31_SDCMDDATCONT_WRITEREAD            (1<<4)	/* Write/read control */
#define IMX31_SDCMDDATCONT_DATAENABLE           (1<<3)	/* Data enable/disable control */
#define IMX31_SDCMDDATCONT_FORMATOFRESPONSE     (1<<0)	/* Command response type: 48 bit response with crc check (R1/R1b/R5/R6) */
#define IMX31_SDCMDDATCONT_FORMATOFRESPONSE1    (1<<1)	/* Command response type: 136 bit response (R2) */
#define IMX31_SDCMDDATCONT_FORMATOFRESPONSE2    (3<<0)	/* Command response type: 48 bit response without CRC check (R3/R4) */

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
} imx31_mmc_reg;


typedef	struct _mx31_mmc {
	unsigned		mmc_pbase;
	uintptr_t		mmc_base;
	unsigned        gpio2_base;
	unsigned        gpio3_base;
	uintptr_t		iomux_base;
	int				blksz;
	uint32_t		bus_width;
	uint32_t		clock;
    void			*hba;

	void			*bshdl;

	uint32_t		dmask;		// Interrupt mask for data transfer
	void			*dma_pend;	// DMA transfer pending channel

	dma_functions_t	sdmafuncs;
	void			*rdma_chn;
	void			*wdma_chn;
} mx31_ext_t;


extern int	mx31_attach(SIM_HBA *hba);

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_mx31.h $ $Rev: 657836 $" )
#endif
