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


#ifndef	_MCI_INCLUDED
#define	_MCI_INCLUDED

#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>


 uintptr_t mb_addr;
#define MB_ADDR 0x40007000

#define MBOX_BASE 0x2000b880
 uintptr_t	mbox_base;

 uintptr_t	base;
#define SD_CARD_INTERRUPT       (1 << 8)

#define MBOX_PEEK 0x10
#define MBOX_READ 0x00
#define MBOX_WRITE 0x20
#define MBOX_STATUS 0x18
#define MBOX_SENDER 0x14
#define MBOX_CONFIG 0x1c

#define MBOX_FB		1
#define MBOX_PROP	8

#define MBOX_SUCCESS	0x80000000
#define MBOX_FULL		0x80000000
#define	MBOX_EMPTY		0x40000000

//BCM2835
#define EMMC_BASE		0x20300000
#define	EMMC_ARG2		0
#define EMMC_BLKSIZECNT		4
#define EMMC_ARG1		8
#define EMMC_CMDTM		0xC
#define EMMC_RESP0		0x10
#define EMMC_RESP1		0x14
#define EMMC_RESP2		0x18
#define EMMC_RESP3		0x1C
#define EMMC_DATA		0x20
#define EMMC_STATUS		0x24
#define EMMC_CONTROL0		0x28
#define EMMC_CONTROL1		0x2C
#define EMMC_INTERRUPT		0x30
#define EMMC_IRPT_MASK		0x34
#define EMMC_IRPT_EN		0x38
#define EMMC_CONTROL2		0x3C
#define EMMC_CAPABILITIES_0	0x40
#define EMMC_CAPABILITIES_1	0x44
#define EMMC_FORCE_IRPT		0x50
#define EMMC_BOOT_TIMEOUT	0x70
#define EMMC_DBG_SEL		0x74
#define EMMC_EXRDFIFO_CFG	0x80
#define EMMC_EXRDFIFO_EN	0x84
#define EMMC_TUNE_STEP		0x88
#define EMMC_TUNE_STEPS_STD	0x8C
#define EMMC_TUNE_STEPS_DDR	0x90
#define EMMC_SPI_INT_SPT	0xF0
#define EMMC_SLOTISR_VER	0xFC
#define xDMA_DEBUG


 //clock
  uint32_t hci_ver = 0;
#define SD_GET_CLOCK_DIVIDER_FAIL	0xffffffff
  // SD Clock Frequencies (in Hz)
#define SD_CLOCK_ID         400000
#define SD_CLOCK_NORMAL     25000000
#define SD_CLOCK_HIGH       50000000
#define SD_CLOCK_100        100000000
#define SD_CLOCK_208        208000000

/*********** BCM2835 End ***************/





#ifdef DMA_DEBUG
   #define DMA_SHOW_FUNC()         fprintf(stderr,"%s()\n", __func__);
   #define DMA_SHOW_FUNC_LINE()    fprintf(stderr,"%s()@%d\n", __func__, __LINE__);
#else
   #define DMA_SHOW_FUNC()
   #define DMA_SHOW_FUNC_LINE()
#endif
   
#define	READ32(offset) \
	*((volatile uint32_t *)(base + offset))
#define	WRITE32(offset, value) \
	(*((volatile uint32_t *)(base + offset)) = (value))

/* DMA tranfer block size */
#define BLK_LENGTH 512

/* Data xfer direction */
#define DATA_READ   1
#define DATA_WRITE  2

#define DMAC_BASE 0xffffec00
#define PIOA_PDSR 0xfffff23c

/*
	Literature:
	[1] Atmel, "AT91SAM ARM-based Embedded MPU SAM9G45" (6438G–ATARM–19-Apr-11)
	[2] Atmel, "AT91SAM9G45-EKES User Guide" (6481B–ATARM–27-Nov-09)
*/

/* Power Management Controller              */
#define PMC_BASE                0xfffffc00
#define PMC_SIZE                0x100 

#define PMC_PCER                0x10            /* Peripheral Clock Enable Register */
#define PMC_PCDR                0x14            /* Peripheral Clock Disable Register */

/* [1] p.15, Figure 6-1: SAM9G45 Memory Mapping */
#define AT91SAM9G45_PIOD_BASE		0xfffff800

/* [1] pp.456,457, 30.6: Parallel Input/Output Controller (PIO) User Interface */
#define AT91SAM9G45_PIO_SIZE		0x00000100
#define AT91SAM9G45_PIO_PER		0x0000			/* PIO Enable Register */
#define AT91SAM9G45_PIO_PDR		0x0004			/* PIO Disable Register */
#define AT91SAM9G45_PIO_PSR		0x0008			/* PIO Status Register */
#define AT91SAM9G45_PIO_OER		0x0010			/* Output Enable Register */
#define AT91SAM9G45_PIO_ODR		0x0014			/* Output Disable Register */
#define AT91SAM9G45_PIO_OSR		0x0018			/* Output Status Register */
#define AT91SAM9G45_PIO_IFER		0x0020			/* Glitch Input Filter Enable Register */
#define AT91SAM9G45_PIO_IFDR		0x0024			/* Glitch Input Filter Disable Register */
#define AT91SAM9G45_PIO_IFSR		0x0028			/* Glitch Input Filter Status Register */
#define AT91SAM9G45_PIO_SODR		0x0030			/* Set Output Data Register */
#define AT91SAM9G45_PIO_CODR		0x0034			/* Clear Output Data Register */
#define AT91SAM9G45_PIO_ODSR		0x0038			/* Output Data Status Register */
#define AT91SAM9G45_PIO_PDSR		0x003C			/* Pin Data Status Register */
#define AT91SAM9G45_PIO_IER		0x0040			/* Interrupt Enable Register */
#define AT91SAM9G45_PIO_IDR		0x0044			/* Interrupt Disable Register */
#define AT91SAM9G45_PIO_IMR		0x0048			/* Interrupt Mask Register */
#define AT91SAM9G45_PIO_ISR		0x004C			/* Interrupt Status Register */
#define AT91SAM9G45_PIO_MDER		0x0050			/* Multi-driver Enable Register */
#define AT91SAM9G45_PIO_MDDR		0x0054			/* Multi-driver Disable Register */
#define AT91SAM9G45_PIO_MDSR		0x0058			/* Multi-driver Status Register */
#define AT91SAM9G45_PIO_PUDR		0x0060			/* Pull-up Disable Register */
#define AT91SAM9G45_PIO_PUER		0x0064			/* Pull-up Enable Register */
#define AT91SAM9G45_PIO_PUSR		0x0068			/* Pad Pull-up Status Register */
#define AT91SAM9G45_PIO_ASR		0x0070			/* Peripheral A Select Register */
#define AT91SAM9G45_PIO_BSR		0x0074			/* Peripheral B Select Register */
#define AT91SAM9G45_PIO_ABSR		0x0078			/* AB Status Register */
#define AT91SAM9G45_PIO_OWER		0x00A0			/* Output Write Enable */
#define AT91SAM9G45_PIO_OWDR		0x00A4			/* Output Write Disable */
#define AT91SAM9G45_PIO_OWSR		0x00A8			/* Output Write Status Register */
//#define AT91SAM9G45_PIO_DELAY0R	0x00C0			/* I/O Delay Register */
//#define AT91SAM9G45_PIO_DELAY1R	0x00C4			/* I/O Delay Register */
//#define AT91SAM9G45_PIO_DELAY2R	0x00C8			/* I/O Delay Register */
//#define AT91SAM9G45_PIO_DELAY3R	0x00CC			/* I/O Delay Register */
//#define AT91SAM9G45_PIO_WPMR		0x00E4			/* Write Protect Mode Register */
//#define AT91SAM9G45_PIO_WPSR		0x00E8			/* Write Protect Status Register */

/* [2] p.39, 5.5.5: Multiplexing on PIO Controller D (PIOD) */
#define AT91SAM9G45_MCI_PIO_CD0		0x00000400		/* CD0: bit 11 */
#define AT91SAM9G45_MCI_PIO_CD1		0x00000800		/* CD1: bit 10 */
#define AT91SAM9G45_MCI_PIO_WP0		0x00000000		/* WP0 is not available on dev board */
#define AT91SAM9G45_MCI_PIO_WP1		0x20000000		/* WP1: bit 29 */
#define AT91SAM9G45_MCI_PIO_BITS		0x20000c00		/* WP1:29, WP0:n.a., CD1:11, CD0:10 */

/* MCI Interface Registers */
#define	MCI_CR		0x00		/* Control Register */
	#define	MCIEN			(1 << 0)	/* Interface Enable */
	#define	MCIDIS			(1 << 1)	/* Interface Disable */
	#define	PWSEN			(1 << 2)	/* Power Save Mode Enable */
	#define	PWSDIS			(1 << 3)	/* Power Save Mode Disable */
	#define	SWRST			(1 << 7)	/* Software Reset */
#define	MCI_MR		0x04		/* Mode Register */
	#define	RDPROOF			(1 << 11)	/* Read Proof Enable */
	#define	WRPROOF			(1 << 12)	/* Write Proof Enable */
	#define	FBYTE			(1 << 13)	/* Force Byte Transfer */
	#define	PADV			(1 << 14)	/* Padding Value */
	#define	PDCMODE			(1 << 15)	/* PDC-oriented Mode */
	#define PWSDIV(x)		(((x)&7) << 8)	/* Power Saveing divider */ 
#define	MCI_DTOR	0x08		/* Data Timeout Register */
#define	MCI_SDCR	0x0c		/* SD/SDIO Card Register */
	#define	SDCSEL0			0x00		/* Slot A selected */
	#define	SDCSEL1			0x01		/* Slot B selected */
	#define	SDCBUS1B		(0 << 6)	/* 0 = 1-bit */
	#define	SDCBUS4B		(2 << 6)	/* 2 = 4-bit bus */
	#define	SDCBUS8B		(3 << 6)	/* 3 = 8-bit bus */

#define	MCI_ARGR	0x10		/* Argument Register */
#define	MCI_CMDR	0x14		/* Command Register */
	#define	RSPTYPE_NONE		(0x0 << 6)	/* No response */
	#define	RSPTYPE_48		(0x1 << 6)	/* 48-bit response */
	#define	RSPTYPE_136		(0x2 << 6)	/* 136-bit response */
	#define RSPTYPE_R1B		(0x3 << 6)	/* R1b response */
	#define	SPCMD_INIT		(0x1 << 8)	/* Initialize Command */
	#define	SPCMD_SYNCH		(0x2 << 8)	/* Synchronize Command */
	#define	SPCMD_INT		(0x4 << 8)	/* Interrupt Command */
	#define	SPCMD_INTR		(0x5 << 8)	/* Interrupt Response */
	#define	OPDCMD			(1 << 11)	/* Open Drain Command */
	#define	MAXLAT			(1 << 12)	/* 0 = 5-cycle, 1 = 64-cycle max latency */
	#define	TRCMD_NO		(0x0 << 16)	/* No Data Transfer */
	#define	TRCMD_START		(0x1 << 16)	/* Start Data Transfer */
	#define	TRCMD_STOP		(0x2 << 16)	/* Stop Data Transfer */
	#define	TRDIR			(1 << 18)	/* Transfer Direction 0 = Write, 1 = Read */
	#define	TRTYP_SB		(0x0 << 19)	/* MMC Single Block */
	#define	TRTYP_MB		(0x1 << 19)	/* MMC Multiple Block */
	#define	TRTYP_ST		(0x2 << 19)	/* MMC Stream */
#define	MCI_BLKR	0x18		/* Block Register */
#define MCI_CSTOR	0x1c
#define	MCI_RSPR0	0x20		/* Response Register 1 */
#define	MCI_RSPR1	0x24		/* Response Register 2 */
#define	MCI_RSPR2	0x28		/* Response Register 3 */
#define	MCI_RSPR3	0x2c		/* Response Register 4 */
#define	MCI_RDR		0x30		/* Receive Data Register */
#define	MCI_TDR		0x34		/* Transmit Data Register */
#define	MCI_SR		0x40		/* Status Register */
	#define	CMDRDY			(1 << 0)	/* Command Ready */
	#define	RXRDY			(1 << 1)	/* Receiver Ready */
	#define	TXRDY			(1 << 2)	/* Transmit Ready */
	#define	BLKE			(1 << 3)	/* Data Block Ended */
	#define	DTIP			(1 << 4)	/* Data Transfer in Progress */
	#define	NOTBUSY			(1 << 5)	/* MCI Not Busy */
	#define	ENDRX			(1 << 6)	/* End of RX Buffer */
	#define	ENDTX			(1 << 7)	/*End of TX Buffer */
	#define SDIOIRQA		(1 << 8)	/* SDIO interrupt for Slot A */
	#define SDIOIRQB		(1 << 9)	/* SDIO interrupt for Slot B */
	#define SDIOIRQC		(1 << 10)	/* SDIO interrupt for Slot C */
	#define SDIOIRQD		(1 << 11)	/* SDIO interrupt for Slot D */
	#define CSRCV			(1 << 13)	/* Completion Signal Received */
	#define	RXBUFF			(1 << 14)	/* Rx Buffer Full */
	#define	TXBUFE			(1 << 15)	/* Tx Buffer Empty */
	#define	RINDE			(1 << 16)	/* Response Index Error */
	#define	RDIRE			(1 << 17)	/* Response Direction Error */
	#define	RCRCE			(1 << 18)	/* Response CRC Error */
	#define	RENDE			(1 << 19)	/* Response End Bit Error */
	#define	RTOE			(1 << 20)	/* Response Timeout Error */
	#define	DCRCE			(1 << 21)	/* Data CRC Error */
	#define	DTOE			(1 << 22)	/* Data Timeout Error */
	#define CSTOE			(1 << 23)	/* Completion Error Timeout */
	#define BLKOVRE			(1 << 24)
	#define DMADONE 		(1 << 25)
	#define FIFOEMPTY		(1 << 26)
	#define XFRDONE			(1 << 27)
	#define	OVRE			(1 << 30)	/* Overrun */
	#define	UNRE			(1 << 31)	/* Underrun */
#define	MCI_IER			0x44			/* Interrupt Enable Register */
#define	MCI_IDR			0x48			/* Interrupt Disable Register */
#define	MCI_IMR			0x4c			/* Interrupt Mask Register */

/* Register related to DMA Controller (DMAC) */
#define MCI_DMA		0x50				/* DMA configuration Register */	
	#define DMAEN		(1 << 8)		/* DMA enable*/
#define MCI_CFG		0x54				/* Configuration Register */
	#define FIFOMODE	(1 << 0)		/* FIFO control Mode*/
	#define FERRCTRL	(1 << 4)		/* Error flag reset control mode*/
	#define HSMODE		(1 << 8)		/* High Speed Mode*/
	#define LSYNC		(1 << 12)		/* Synchronise on last block */
#define MCI_WPMR	0xE4				/* Write Protection mode register */
	#define WP_EN		(1 << 1)
#define MCI_WPSR	0xE8				/* Write Protection status register */

/* Register related to Peripheral DMA Controller (PDC) */

#define	MCI_RPR		0x100		/* Receive Pointer Register */
#define	MCI_RCR		0x104		/* Receive Counter Register */
#define	MCI_TPR		0x108		/* Transmit Pointer Register */
#define	MCI_TCR		0x10c		/* Transmit Counter Register */
#define	MCI_RNPR	0x110		/* Receive Next Pointer Register */
#define	MCI_RNCR	0x114		/* Receive Next Counter Register */
#define	MCI_TNPR	0x118		/* Transmit Next Pointer Register */
#define	MCI_TNCR	0x11c		/* Transmit Next Counter Register */
#define	MCI_PTCR	0x120		/* Transfer Control Register */
	#define	MCI_RXTEN		(1 << 0)	/* Receive Transfer Enable */
	#define	MCI_RXTDIS		(1 << 1)	/* Receive Transfer Disable */
	#define	MCI_TXTEN		(1 << 8)	/* Transmit Transfer Enable */
	#define	MCI_TXTDIS		(1 << 9)	/* Transmit Transfer Disable */
#define	MCI_PTSR	0x124		/* Transfer Status Register */
	#define	MCI_RXTENE		(1 << 0)	/* Receiver Transfer Enabled */
	#define	MCI_TXTENE		(1 << 8)	/* Transmitter Transfer Enabled */
#define MCI_FIFO	0x200

#define MCI_ERRORS (RINDE | RDIRE | RCRCE	\
		| RENDE | RTOE | DCRCE		\
		| DTOE | OVRE | UNRE | BLKOVRE \
		| CSTOE )

/*******************************************
 *  DMA Controller registers (DMAC) 
 ******************************************/

#define DMA_EN          0x004
#define DMA_SADDR0      0x03c
#define DMA_CHER        0x028
#define DMA_CHDR        0x02c
#define DMA_CHSR        0x030
#define DMA_EBCIER      0x018
#define DMA_EBCIDR      0x01C
#define DMA_EBCIMR      0x020
#define DMA_EBCISR      0x024
#define DMA_CTRLA0      0x048
#define DMA_CTRLB0      0x04C
#define DMA_DADDR0      0x040

/******************************
 *  DMAC headers START
 *****************************/
#define DMAC_ENABLE			(0x1 << 0)

#define DMAC_SOD_DISABLE		(0x0 << 16)
#define DMAC_DST_H2SEL_SW		(0x0 << 13)
#define DMAC_SRC_H2SEL_HW		(0x1 <<  9)
#define DMAC_SRC_H2SEL_SW		(0x0 <<  9)
#define DMAC_DST_H2SEL_HW		(0x1 << 13)
#define DMAC_FC_PER2MEM			(0x2 << 21)
#define DMAC_FC_MEM2PER			(0x1 << 21)

#define DCSIZE				(0x0 << 20)
#define SCSIZE				(0x0 << 16)
 
#define DMAC_NUM_CHANNEL		8   // Maximum DMA channel in Atmel SOCs
#define DMAC_FIRST_CHAN			0
#define DMAC_LAST_CHAN			(DMAC_NUM_CHANNEL - 1)

#define BTC(x)				(0x1 << ((x)))
#define CBTC(x)				(0x1 << (8 + (x)))
#define ERR(x)				(0x1 << (16 + (x)))



#define SRC_WIDTH(x)			(((x)&3) << 24)
#define DST_WIDTH(x)			(((x)&3) << 28)

#define FLAG_SRC_NO_INCREMENT           (0x10 << 24)
#define FLAG_SRC_DECREMENT              (0x01 << 24) 
#define FLAG_SRC_INCREMENT              (0x00 << 24)
        
#define FLAG_DST_NO_INCREMENT           (0x10 << 28)
#define FLAG_DST_DECREMENT              (0x01 << 28)
#define FLAG_DST_INCREMENT              (0x00 << 28)
        
#define SET_SIF_DIF                     (0x3 & (0)) | ((0x3 & (1)) <<  4);
#define SET_SRC_DSCR                    (1<<16)
#define SET_DST_DSCR                    (1<<20)
#define SET_DSCR                        (1<<20)|(1<<16)
#define UNSET_DSCR                      (0<<20)|(0<<16)

/* Define DMAC Buffer Descriptor */
typedef struct {
        /* values that are not changed by hardware */
        uint32_t    saddr;
        uint32_t    daddr;
        uint32_t    ctrla;
        uint32_t    ctrlb;
        uint32_t    dscr;        /* Pointer to next or first lli */
}at91sam9xx_dmac_bd_t;

/* Define DMAC channel control structure */
typedef struct {
    uint32_t    saddr;           /* Source Address */
    uint32_t    daddr;           /* Destination address */
    uint32_t    dscr;            /* Descriptor Address Register */
    uint32_t    ctrla;           /* Control A Register */
    uint32_t    ctrlb;           /* Control B Register */
    uint32_t    cfg;             /* Configuration Register */
    uint32_t    spip;            /* Source Picture in Picture 
                                    Configuration Register */
    uint32_t    dpip;            /* Destination Picture in Picture 
                                    Configuration Register */
    uint32_t    resv[2];         /* Reserved block */
}at91sam9xx_dmac_lli_t;

/* DMA Controller (DMAC) User Interface */
typedef struct {            /* Offset */
    uint32_t     gcfg;      /* DMAC Global Configuration Register    (0x0000) */
    uint32_t     en;        /* DMAC Enable Register                  (0x0004) */
    uint32_t     sreq;      /* DMAC s/w single request register      (0x0008) */
    uint32_t     creq;      /* DMAC s/w chunk transfer request reg   (0x000c) */
    uint32_t     last;      /* DMAC s/w last transfer flag register  (0x0010) */
    uint32_t     resv1;     /* Reserved block 1                      (0x0014) */
    uint32_t     ebcier;    /* DMAC Error, Chained Buffer transfer completed and
                               Buffer transfer completed Interrupt Enable register.
                                                                     (0x0018) */
    uint32_t     ebcidr;    /* DMAC Error, Chained Buffer transfer completed and
                               Buffer transfer completed Interrupt Disable register.
                                                                     (0x001c) */
    uint32_t     ebcimr;    /* DMAC Error, Chained Buffer transfer completed and
                               Buffer transfer completed Mask register.
                                                                     (0x0020) */
    uint32_t     ebcisr;    /* DMAC Error, Chained Buffer transfer completed and
                               Buffer transfer completed Status register.
                                                                     (0x0024) */
    uint32_t     cher;      /* DMAC Channel Handler Enable Register  (0x0028) */
    uint32_t     chdr;      /* DMAC Channel Handler Disable Register (0x002c) */
    uint32_t     chsr;      /* DMAC Channel Handler Status Registeri (0x0030) */
    
    uint32_t    resv[2];         /* Reserved block */
    at91sam9xx_dmac_lli_t   lli[DMAC_NUM_CHANNEL]; /* Channel LLI    (0x1000) */
} at91sam9xx_dmac_t;

typedef struct {
    unsigned    chid;       /* Channel ID */
    uint32_t    io_addr;    /* DMAC input/output address of device */

    at91sam9xx_dmac_bd_t * lli;
    int          blksz;
    int          blknum;
    int          dir;

    at91sam9xx_dmac_t   *dmac;
} dmac_dev_t;

/******************************
 *  DMAC headers END
 *****************************/

typedef	struct _hsmci_ext {
    uintptr_t   base;
    int         blksz;
    int         blknum;

    uint32_t    clock;
    void        *hba;

    /* The last command issued */
    int         cmd;

    /* Specify the port # of HSMCI */
    int		port;

    /* Specify the slot # of the MCI */
    int         slot;

#ifdef AT91SAM9G45_R1BWORKAROUND
    /* Specify the bit position of mcix_da0 for polling busy signal */
    int		da0_mask;
#endif

    /* For DMAC controller */
    dmac_dev_t  *dmac_dev;

    /* Base address of DMAC controller */
    unsigned    dbase;

    /* DMAC Channel HW interface Number */
    unsigned    dintf;

    /* Base address for PIO */
    uintptr_t    pio_base;

    /* Base address for PMC */
    uintptr_t    pmc_base;

} hsmci_ext_t;

extern int hsmci_init(SIM_HBA *hba);

#endif
