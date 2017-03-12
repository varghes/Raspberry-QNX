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


#ifndef	_STA2X11_INCLUDED
#define	_STA2X11_INCLUDED

#include	<hw/dma.h>
#include	<hw/sysinfo.h>
#include 	<hw/pci.h>
#include 	<hw/pci_devices.h>

#define	__REG32(x)		(*((volatile uint32_t *)(x)))

/*
 * SDIO/MMC Memory-mapped registers
 */
#define	SDI_PWR(base)		__REG32(base + 0x00)
#define	SDI_CLKCR(base)		__REG32(base + 0x04)
#define	SDI_ARG(base)		__REG32(base + 0x08)
#define	SDI_CMD(base)		__REG32(base + 0x0c)
#define	SDI_RESPCMD(base)	__REG32(base + 0x10)
#define	SDI_RESP0(base)		__REG32(base + 0x14)
#define	SDI_RESP1(base)		__REG32(base + 0x18)
#define	SDI_RESP2(base)		__REG32(base + 0x1c)
#define	SDI_RESP3(base)		__REG32(base + 0x20)
#define	SDI_DTIMER(base)	__REG32(base + 0x24)
#define	SDI_DLEN(base)		__REG32(base + 0x28)
#define	SDI_DCTRL(base)		__REG32(base + 0x2c)
#define	SDI_DCOUNT(base)	__REG32(base + 0x30)
#define	SDI_STA(base)		__REG32(base + 0x34)
#define	SDI_ICR(base)		__REG32(base + 0x38)
#define	SDI_MASK0(base)		__REG32(base + 0x3C)
#define	SDI_MASK1(base)		__REG32(base + 0x40)
#define	SDI_CSEL(base)		__REG32(base + 0x44)
#define SDI_FIFOCNT(base)	__REG32(base + 0x48)
#define	SDI_FIFO(base)		__REG32(base + 0x80)
#define SDI_DBTIMER(base)	__REG32(base + 0xC0)
#define SDI_TRSCA(base)     __REG32(base + 0xC4)

/* Power control Register bit definition*/
#define PWR_PWRCTRL_SHIFT		0
#define PWR_PWRCTRL_MSK			(0x3)
#define PWR_DAT2DIRPWRVEN		(1<<2)
#define PWR_CMDDIREN			(1<<3)
#define PWR_DAT0DIREN			(1<<4)
#define PWR_DAT31DIREN			(1<<5)
#define PWR_OPT					(1<<6)
#define PWR_FBCLKEN				(1<<7)
#define PWR_DAT74DIREN			(1<<8)

/* Clock Control Register bit definition */
#define CLKCR_CLKDIV_SHIFT		0
#define CLKCR_CLKDIV_MSK		(0xff)
#define CLKCR_CLKEN				(1<<8)
#define CLKCR_CLKENV2			(1<<16)
#define	CLKCR_PWRSAV			(1<<9)
#define CLKCR_BYPASS			(1<<10)
#define CLKCR_WIDBUS_SHIFT		11
#define	CLKCR_WIDBUS_MSK		(0x3<<CLKCR_WIDBUS_SHIFT)
#define CLKCR_NEGEDGE			(1<<13)
#define CLKCR_HWFC_EN			(1<<14)
#define CLKCR_CLKOUT_INV		(1<<15)

/* Argument Register bit definition */

/* SDI Command Register (SDI_CMD) bit definition */
#define CMD_INDEX_MSK			0x3F
#define CMD_WAITRESP			(1<<6)
#define CMD_LONGRESP			(1<<7)
#define CMD_WAITINT				(1<<8)
#define CMD_WAITPEND			(1<<9)
#define CMD_CPSMEN				(1<<10)
#define CMD_SDIO_Suspend		(1<<1)
#define CMD_ENCMD				(1<<12)
#define CMD_nIEN				(1<<13)
#define CMD_CEATA_CMD			(1<<14)
#define CMD_CBOOTMODEEN			(1<<15)

/* SDI Command Response Register (SDI_RESPCMD) bit definition */
#define RESPCMD_INDEX_MASK		0x3F


/* SDI Data Control Register (SDI_DCTRL) bit definition */

#define DCTRL_DTEN				(1<<0)
#define DCTRL_DTDIR				(1<<1)
#define DCTRL_DTMODE			(1<<2)
#define DCTRL_DMAEN				(1<<3)
#define DCTRL_DBLOCKSIZE_SHIFT	4
#define DCTRL_DBLOCKSIZE_MSK	0xF
#define DCTRL_DBLOCKSIZE_SHIFTV2	16
#define DCTRL_DBLOCKSIZE_MSKV2	0x7FFF

#define DCTRL_RWSTART			(1<<8)
#define DCTRL_RWSTOP			(1<<9)
#define DCTRL_RWMOD				(1<<10)
#define DCTRL_SDIOEN			(1<<11)
#define DCTRL_DMAREQCTL			(1<<12)
#define DCTRL_DBOOTMODEEN		(1<<13)
#define DCTRL_BUSYMODE			(1<<14)

/*SDI Status Register (SDI_STA) bit definition */
#define STA_CCRCFAIL			(1<<0)	
#define STA_DCRCFAIL			(1<<1)	
#define STA_CTIMEOUT			(1<<2)	
#define STA_DTIMEOUT			(1<<3)	
#define STA_TXUNDERR			(1<<4)	
#define STA_RXOVERR				(1<<5)	
#define STA_CMDREND				(1<<6)	
#define STA_CMDSENT				(1<<7)	
#define STA_DATAEND				(1<<8)	
#define STA_STBITERR			(1<<9)	
#define STA_DBCKEND				(1<<10)	
#define STA_CMDACT				(1<<11)	
#define STA_TXACT				(1<<12)	
#define STA_RXACT				(1<<13)	
#define STA_TXFIFOBW			(1<<14)	
#define STA_RXFIFOBR			(1<<15)	
#define STA_TXFIFOF				(1<<16)	
#define STA_RXFIFOF				(1<<17)	
#define STA_TXFIFOE				(1<<18)	
#define STA_RXFIFOE				(1<<19)	
#define STA_TXDAVL				(1<<20)	
#define STA_RXDAVL				(1<<21)	
#define STA_SDIOIT				(1<<22)	
#define STA_CEATAEND			(1<<23)	
#define STA_CARDBUSY			(1<<24)	
#define STA_BOOTMODE			(1<<25)	
#define STA_BOOTACKERR			(1<<26)	
#define STA_BOOTACKTIMEOUT		(1<<27)	

/* SDI Interrupt Clear Register (SDI_ICR) bit definition */
#define ICR_CCRCFAILC			(1<<0)	
#define ICR_DCRCFAILC			(1<<1)	
#define ICR_CTIMEOUTC			(1<<2)	
#define ICR_DTIMEOUTC			(1<<3)	
#define ICR_TXUNDERRC			(1<<4)	
#define ICR_RXOVERRC			(1<<5)	
#define ICR_CMDRENDC			(1<<6)	
#define ICR_CMDSENTC			(1<<7)	
#define ICR_DATAENDC			(1<<8)	
#define ICR_STBITERRC			(1<<9)	
#define ICR_DBCKENDC			(1<<10)	
#define ICR_SDIOITC				(1<<22)	
#define ICR_CEATAENDC			(1<<23)	
#define ICR_CARDBUSYC			(1<<24)	
#define ICR_BOOTACKERRC			(1<<26)	
#define ICR_BOOTACKTIMEOUTC		(1<<27)	

#define ICR_CLR_ALL				0x0DC007FF

/* SDI Mask Registers 0...1 (SDI_MASK0...1) bit definition */
/* Same as SDI_STA and only SDI_MASK0 is valid for this device for now*/
#define ERR_MASK			(STA_DCRCFAIL | STA_CTIMEOUT \
							| STA_DTIMEOUT | ICR_TXUNDERRC | ICR_RXOVERRC | STA_STBITERR) 

/* SDI Card Select Register (SDI_CSEL) bit definition */
#define CSEL_SDCARD_MSK			0xf

/*SDI FIFO Counter Register (SDI_FIFOCNT) bit definition */
#define FIFOCNT_DATACOUNT_MSK	0xffffff
#define FIFO_OFF				0x80
#define SDIO0_FIFO_BASE 		(0x54001000+FIFO_OFF)
#define SDIO1_FIFO_BASE 		(0x54000000+FIFO_OFF)
//#define SDIO2_FIFO_BASE 		(0x54004000+FIFO_OFF)
#define SDIO2_FIFO_BASE 		(0x58004000+FIFO_OFF)
#define SDIO3_FIFO_BASE 		(0x5C004000+FIFO_OFF)
#define DMA_REQ_SDIO0 0
#define DMA_REQ_SDIO1 1
#define DMA_REQ_SDIO2 2
#define DMA_REQ_SDIO3 3
#define GPIO_DAT	0x00
#define GPIO_DATS	0x04
#define GPIO_DATC	0x08
#define GPIO_PDIS	0x0c
#define GPIO_DIR	0x10
#define GPIO_DIRS	0x14
#define GPIO_DIRC	0x18
#define GPIO_AFSELA	0x20
#define GPIO_RIMSC	0x40
#define GPIO_FIMSC	0x44
#define GPIO_IS		0x48
#define GPIO_IC		0x4c


#define SDI_FIFOSIZE	(16*4)
#define SDI_FIFOHALFSIZE (SDI_FIFOSIZE / 2)
#define NR_SG			(16)
#define SDI_MAX_CLK				52000000
#define SDI_MAX_CLK1			48000000
#define SDI_MAX_CLK2			100000000

#define SDI_STATE_PIO_IN	0x1
#define SDI_STATE_PIO_OUT	0x2
#define SDI_STATE_DMA_IN	0x4
#define SDI_STATE_DMA_OUT	0x8
#define SDI_STATE_PIO		0x3
#define SDI_STATE_DMA		0xC
#define SDI_STATE_CMD		0x10
#define SDI_STATE_DMA_STARTED 0x20

#define POLL_MODE	1
#define NR_DESCS_PER_CHANNEL	64
#define POLL_TIMEOUT	1000000
typedef	struct _sta2x11_ext {
	int			pci_hdl;
	void		*pci_dev_hdl;
	uintptr_t	base;
	uint32_t	pbase;
	int 		version;
	int 		translen;
	int 		currlen;
	int			blksz;
	int			blksz_bits;
	uint16_t	BusNumber;
	uint16_t	DevFunc;
	uint32_t	clock;
	uint32_t	state;
	uint32_t	fifo_reg;
	uintptr_t	gpio_base; //card detect and write protect gpio base
	uintptr_t	sctl_base; //sctl base
	int 		gpio_cd; //card detect gpio pin
	int 		gpio_wp; //wirte protect gpio pin
	int 		gpio_pwr; //powerenable pin
	uint32_t	gpio_mask0; //pin pull up/down 
	uint32_t	gpio_mask1; //pin pull up/down 
	uint32_t	gpio_mask_clk; //clk pin
	int			gpio_size;
	void		*gpio_dev_hdl;
	int			port;
	intrspin_t 	spinlock;
	uint8_t		*buf;
	dma_transfer_t	tinfo;
	dma_addr_t	src_addr[NR_DESCS_PER_CHANNEL];
	dma_addr_t	dst_addr[NR_DESCS_PER_CHANNEL];
	void   		*dma_chn;
	dma_functions_t dmafuncs;
#ifdef POLL_MODE
	int			timer_initialized;
	timer_t		timer_id;
	struct itimerspec	itime;
	struct itimerspec	zerotime;
	struct sigevent timer;
#endif
	volatile unsigned command; //current command
	void		*hba;

} sta2x11_ext_t;

extern int sta2x11_init(SIM_HBA *hba);

#endif

__SRCVERSION( "$URL$ $Rev$" );

