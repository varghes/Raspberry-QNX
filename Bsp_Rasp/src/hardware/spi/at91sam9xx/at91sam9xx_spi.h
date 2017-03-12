/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008, 2009 QNX Software Systems.  
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





#ifndef _AT91SAM9XX_SPI_H_INCLUDED
#define _AT91SAM9XX_SPI_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <hw/spi-master.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

#define	AT91_SPI_PRIORITY		21
#define	AT91_SPI_EVENT			1
#define	AT91_DMA_PULSE_PRIORITY		21
#define	AT91_DMA_PULSE_CODE		1

#define AT91_SPI_BASE         0xFFFCC000
#define AT91_SPI_SIZE         0x128

#define AT91_SPI_IRQ          13



#define AT91_SPI_CONREG		0x00	/* Control register */
#define AT91_SPI_MODEREG	0x04	/* Mode register */
#define AT91_SPI_RXDATA		0x08	/* Receive data register */
#define AT91_SPI_TXDATA		0x0C	/* Transmit data register */
#define AT91_SPI_STATREG    0x10    /* Status register */
#define AT91_SPI_INTENREG	0x14	/* Interrupt enable register */
#define AT91_SPI_INTDISREG	0x18	/* Interrupt disable register */
#define AT91_SPI_INTMASKREG	0x1C	/* Interrupt mask register */
#define AT91_SPI_CHIPSEL0  	0x30	/* Chip Select 0 register */
#define AT91_SPI_CHIPSEL1  	0x34	/* Chip Select 1 register */
#define AT91_SPI_CHIPSEL2  	0x38	/* Chip Select 2 register */
#define AT91_SPI_CHIPSEL3  	0x3C	/* Chip Select 3 register */
#define PDC_RPR  	0x100	/* Receive Pointer Register */
#define PDC_RCR  	0x104	/* Receive Counter Register*/
#define PDC_TPR  	0x108	/* Transmit Pointer Register */
#define PDC_TCR  	0x10C	/* Transmit Pointer Register */
#define PDC_RNPR  	0x110	/* Receive Next Pointer Register */
#define PDC_RNCR  	0x114	/* Receive Next Pointer Register */
#define PDC_TNPR  	0x118	/* Transmit Next Pointer Register */
#define PDC_TNCR  	0x11C	/* Transmit Next Counter Register */
#define PDC_PTCR  	0x120	/* Transfer Control Register */
#define PDC_PTSR  	0x124	/* Transfer Status Register */

// PDC Register Bit Definitions
#define PDC_PTCR	0x120   /* Transfer Control Register */
#define PTCR_TXTEN	0x00000100
#define PTCR_RXTEN	0x00000001
#define PTCR_TXTDIS	0x00000200
#define PTCR_RXTDIS	0x00000002

// Chip select a spi device 
#define SPI_MODEREG_PCS(val)         ((val & 0xf)  << 16)//for fixed peripheral

// CONREG BIT Definitions
#define SPI_CONREG_SPIEN          0x1
#define SPI_CONREG_SPIDIS         0x2
#define SPI_CONREG_SWRST          0x80

// MODEREG BIT Definitions
#define SPI_MODEREG_MSTR          0x1
#define SPI_MODEREG_PS            0x2
#define SPI_MODEREG_MODFDIS       0x10
#define SPI_MODEREG_LOOPBACK      0x80

// TX / RXDATA BIT Definitions
#define SPI_TXDATA_PCS(id)        ((((1 << id) - 1) & 0xf) << 16)

// CHIPSEL BIT Definitions
#define SPI_CHIPSEL_BITS_POS      0x4
#define SPI_CHIPSEL_SCBR_POS      0x8
#define SPI_CHIPSEL_NCPHA         0x2
#define SPI_CHIPSEL_CPOL          0x1
#define SPI_CHIPSEL_CSAAT         0x8
#define SPI_CHIPSEL_DLBCT_MASK    (0xFF<<24)
#define SPI_CHIPSEL_DLYBS_MASK    (0xFF<<16)

// INTENREG / INTDISREG / INTMASKREG BIT Definitions
#define SPI_INTREG_RDRF           0x1
#define SPI_INTREG_TRDE           0x2
#define SPI_INTREG_RXBUFF           0x40

#define DLYBS(mck, delay) \
    ((((delay * (mck / 1000000)) / 1000) << 16) & SPI_CHIPSEL_DLYBS_MASK)

#define DLYBCT(mck, delay) \
    ((((delay / 32 * (mck / 1000000)) / 1000) << 24) & SPI_CHIPSEL_DLBCT_MASK)

#define AT45_DLYBS                250
#define AT45_DLYBCT               250

typedef struct {
	SPIDEV		        spi;		/* This has to be the first element */

	unsigned	        pbase;
	uintptr_t	        vbase;
	int			        irq;
	int			        iid;
	int			        chid, coid;

	uint32_t	        bitrate;

	uint32_t	        clock;
	int                 curr_cs;
	uint8_t		        *pbuf;
	int			        xlen, tlen, rlen;
	int			        dlen;
	int			        dtime;		/* usec per data, for time out use */

    int                 loopback;
    
	struct sigevent	spievent;
} at91sam9xx_spi_t;

extern void *at91sam9xx_init(void *hdl, char *options);
extern void at91sam9xx_dinit(void *hdl);
extern void *at91sam9xx_xfer(void *hdl, uint32_t device, uint8_t *buf, int *len);
extern int at91sam9xx_setcfg(void *hdl, uint16_t device, spi_cfg_t *cfg);

extern int at91sam9xx_devinfo(void *hdl, uint32_t device, spi_devinfo_t *info);
extern int at91sam9xx_drvinfo(void *hdl, spi_drvinfo_t *info);

extern int at91sam9xx_xchange(at91sam9xx_spi_t *dev, uint8_t *buf, int len);
extern int at91sam9xx_read(at91sam9xx_spi_t *dev, uint8_t *buf, int len);
extern int at91sam9xx_write(at91sam9xx_spi_t *dev, uint8_t *buf, int len);

extern int at91sam9xx_attach_intr(at91sam9xx_spi_t *dev);
extern int at91sam9xx_wait(at91sam9xx_spi_t *dev, int len);

extern int at91sam9xx_cfg(void *hdl, spi_cfg_t *cfg);

extern int at91sam9xx_dma_init(at91sam9xx_spi_t *dev);
extern void at91sam9xx_dma_config_xfer(at91sam9xx_spi_t *dev,uint8_t *buf,int len,int xfer_width);
extern int at91sam9xx_dma_wait(at91sam9xx_spi_t *dev);
extern int at91sam9xx_dmaxfer( void *hdl, uint32_t device, spi_dma_paddr_t *paddr, int len );


#endif
