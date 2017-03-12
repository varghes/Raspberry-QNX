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

// Module Description:  MMC header file

#ifndef _MMC_H_INCLUDED
#define _MMC_H_INCLUDED

#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <malloc.h>
#include <string.h>
#include <signal.h>
#include <atomic.h>
#include <stdarg.h>
#include <pthread.h>
#include <gulliver.h>
#include <hw/pci.h>
#include <hw/inout.h>
#include <sys/mman.h>
#include <sys/disk.h>
#include <sys/types.h>
#include <sys/resmgr.h>
#include <sys/syspage.h>
#include <sys/trace.h>

// CAM specific includes
#include <module.h>
#include <ntocam.h>
#include <sim.h>
#include <sys/dcmd_cam.h>
#include <hw/dcmd_sim_mmcsd.h>

#include <mmcsd.h>
#include <sim_bs.h>

#define SIM_MMC_VERSION		100
#define SIM_MMC_LETTER		'A'

#define MMC_PRIORITY		21
#define MMC_IRQ_PRIORITY	21
#define MMC_STACK_SIZE		16384	// 8192

#define MMC_MAX_TARGET		1
#define MMC_MAX_HBA			1
#define MMC_MAX_SG			128

#define MMC_DFLT_BLKSIZE	512
#define MMC_TRIM_MAX_LBA	0xffffffff

#define MMC_SANDISK_INAND	1

#define MMC_CFLAG_SCAN		0x01		// auto detect interfaces

typedef struct _mmc_ctrl {
	TAILQ_HEAD(,_sim_hba)	hlist;			// linked list of hba's
	uint32_t				cflags;
	uint32_t				verbosity;
	uint32_t				nhba;			// number of hba's
	uint32_t				pathid_max;		// max path id
	int						chid;
	int						coid;
	int						tid;
	struct sigevent			event;
	int						state;
#define	MMCSD_CFLAGS_INITIALIZED	1
#define	MMCSD_CFLAGS_MEDIA_PRESENT	2
} MMC_CTRL;

#define MMC_PARTITION_MAX		8
#define MMC_GP_PART_MAX			4
#define MMC_ERASE_GRP_512K		0x80000
#define MMC_BOOT_SIZE_MULT		0x20000
#define MMC_RPMB_SIZE_MULT		0x20000
typedef struct _mmc_partition {
#define MMC_PFLAG_WP				0x01
#define MMC_PFLAG_PART_RSV			0x02
#define MMC_PFLAG_PART_RSV_REMAP	0x04
	uint32_t		pflags;
	uint32_t		config;
	uint32_t		slba;		// starting lba
	uint32_t		nlba;		// num lba
} MMC_PARTITION;

typedef struct _sim_mmc_ext {
	SIM_HBA				*hba;
	uint32_t			hccap;
#define	MMC_HCCAP_HS		(1 << 0)		// Host support high speed
#define	MMC_HCCAP_DMA		(1 << 1)		// Host support DMA
#define	MMC_HCCAP_18V		(1 << 2)		// Host support 1.8V
#define	MMC_HCCAP_30V		(1 << 3)		// Host support 3.0V
#define	MMC_HCCAP_33V		(1 << 4)		// Host support 3.3V
#define	MMC_HCCAP_BW1		(1 << 5)		// Host support 1 bit bus (mandatory)
#define	MMC_HCCAP_BW4		(1 << 6)		// Host support 4 bit bus (mandatory)
#define	MMC_HCCAP_BW8		(1 << 7)		// Host support 8 bit bus
#define	MMC_HCCAP_ACMD12	(1 << 8)		// Host support auto-stop command(ACMD12)
#define	MMC_HCCAP_CD_INTR	(1 << 9)		// Host support card detect interrupt
#define	MMC_HCCAP_NOCD_BUSY	(1 << 10)		// Host has card busy detect bug

	uint32_t			eflags;
#define MMC_EFLAG_INITIALIZED		(1 << 0)
#define MMC_EFLAG_VALID				(1 << 1)
#define MMC_EFLAG_DRIVER_READY		(1 << 2)
#define MMC_EFLAG_MONITOR_READY		(1 << 3)
#define	MMC_EFLAG_INSERTED			(1 << 4)
#define	MMC_EFLAG_READY				(1 << 5)
#define	MMC_EFLAG_INVALID_CARD		(1 << 6)
#define	MMC_EFLAG_CARD_PRELOAD		(1 << 7)

#define	MMC_EFLAG_HS				(1 << 8)	// high speed card
#define	MMC_EFLAG_HC				(1 << 9)	// high capacity card
#define	MMC_EFLAG_WP				(1 << 12)	// card is write protected
#define MMC_EFLAG_TRIM				(1 << 13)	// card support TRIM

#define	MMC_EFLAG_FORCECLK			(1 << 14)	// force clock
#define	MMC_EFLAG_FORCEBW			(1 << 15)	// force bus width
#define	MMC_EFLAG_NOAC12			(1 << 16)	// disable auto command 12
#define	MMC_EFLAG_NOHS				(1 << 17)	// no high speed support

#define	MMC_EFLAG_PPL				(1 << 27)	// Push-Pull enable
#define	MMC_EFLAG_PIO				(1 << 28)
#define	MMC_EFLAG_BUSY				(1 << 29)

#define MMC_EFLAG_MEDIA_CHANGED     (1 << 31)

	CCB_SCSIIO			*nexus;

	uint8_t				state;
#define	MMCSD_STATE_IDLE			0
#define	MMCSD_STATE_IDENT			1
#define	MMCSD_STATE_DATA			2

	uint8_t				ident;		// ident type
#define	MMCSD_IDENT_NONE			0
#define	MMCSD_IDENT_START			1
#define	MMCSD_IDENT_GO_IDLE			2
#define	MMCSD_IDENT_CARD_VERSION	3
#define	MMCSD_IDENT_SD_POWERUP		4
#define	MMCSD_IDENT_MMC_POWERUP		6
#define	MMCSD_IDENT_ALL_SEND_CID	10
#define	MMCSD_IDENT_SET_ADDRESS		11
#define	MMCSD_IDENT_SEND_CID		12
#define	MMCSD_IDENT_SEND_CSD		13
#define	MMCSD_IDENT_SEND_SCR		14
#define	MMCSD_IDENT_PARSE_SCR		15
#define	MMCSD_IDENT_CHECK_MODE		16
#define	MMCSD_IDENT_SWITCH_MODE		17
#define	MMCSD_IDENT_SEL_DES_CARD	18
#define	MMCSD_IDENT_CHECK_STS		19
#define	MMCSD_IDENT_SET_BW_APP		20
#define	MMCSD_IDENT_SET_BW			21
#define	MMCSD_IDENT_EXT_CSD			22
#define	MMCSD_IDENT_MMC_SWITCH_MODE_HS 23
#define	MMCSD_IDENT_MMC_SWITCH_MODE_BUSWIDTH 24
#define	MMCSD_IDENT_FINAL			30
	int16_t				istage;		// ident stage

	uint32_t			maxio;		// Max I/O size
	int32_t				max_sg;
	int32_t				cur_sg;		// Current SG
	int32_t				offset;		// Offset of current buffer, -1 all done
	int32_t				nbytes;		// Total length of current buffer
	paddr_t				paddr;		// Physical address of current buffer
	uint32_t			blkno;		// Current block number 
	uint8_t				dir;		// Direction
	uint8_t				stop;
#define	MMCSD_STOP_NONE		0
#define	MMCSD_STOP_PEND		1
#define	MMCSD_STOP_ISSUED	2
#define	MMCSD_STOP_AUTO		3

	uint8_t				version;	// Physical Layer Version
#define	SD_VERSION_1	0x01
#define	SD_VERSION_2	0x02
#define	MMC_VERSION_1	0x11
#define	MMC_VERSION_2	0x12

	uint8_t				bus_width;	// Bus width
	uint16_t			pwr_delay;	// power delay in ms
	uint16_t			pwr_retry;
	uint32_t			busy_retry;
	uint32_t			cmdlog;

	uint32_t			devid;
	uint32_t			rca;
	uint32_t			nluns;
	uint32_t			pactive;
	MMC_PARTITION		partitions[MMC_PARTITION_MAX];
	uint32_t			raw_cid[4];
	uint32_t			raw_csd[4];
	uint8_t				raw_scr[SD_SCR_SIZE];

	union {
		sd_cid_t		sd_cid;
		mmc_cid_t		mmc_cid;
	} cid;

	union {
		sd_csd_t		sd_csd;
		mmc_csd_t		mmc_csd;
	} csd;

	uint8_t				extended_csd[EXT_CSD_SIZE];

	sd_scr_t			sd_scr;

	uint32_t			mmc_blksize;
	uint32_t			mmc_capacity;

	uint32_t			resp_type;
	uint32_t			normval;
	uint32_t			mmc_resp[4];
	uint32_t			media_change;
	char				slotname[MMCSD_MAX_SLOTNAME_LEN];		/* slot name */
	
	int					iid;			// DMA interrupt ID

// Hardware specifics follows
	uint32_t			clock;			// host controller base clock
	uint32_t			nclock;			// operation clock in case of forcing clock
	uint32_t			speed;			//current operating clock
	uint32_t			max_dtr;
	void				*bs;			// hardware specific pointer
	char				*opts;

// Hardware callouts	
	void				*handle;							// assigned by attach() call
	int					(*detect)(SIM_HBA *);				// card status
	int					(*powerup)(SIM_HBA *);				// powerup MMC/SD HC
	int					(*powerdown)(SIM_HBA *);			// powerdown MMC/SD HC
//	int					(*set_power)(SIM_HBA *, int);		// set power, TODO
	int					(*cfg_bus)(SIM_HBA *, int, int);	// set bus width and card type
	int					(*set_clock)(SIM_HBA *, int *, int);// set bus clock
	int					(*set_blksz)(SIM_HBA *, int);		// set data block size
	int					(*command)(SIM_HBA *, mmc_cmd_t *);	// send command
//	int					(*command_done)(SIM_HBA *);			// command complete call back, TODO
	int					(*setup_dma)(SIM_HBA *, paddr_t, int, int);	// DMA read/write setup
	int					(*setup_sgdma)(SIM_HBA *, int dir);	// Scatter-gather DMA read/write setup
	int					(*dma_done)(SIM_HBA *, int dir);	// DMA xfer complete
	int					(*setup_pio)(SIM_HBA *, int len, int dir);	// PIO read/write setup
	int					(*pio_done)(SIM_HBA *, char *buf, int len, int dir);	// complete
	int					(*interrupt)(SIM_HBA *, int irq, int resp_type, uint32_t *resp);	// validate interrupt
	int					(*shutdown)(SIM_HBA *);				// Shutdown

	int					mmc_priority;
	int					mmc_irq_priority;
	int					sim_priority;
	int					sim_timer_priority;
} SIM_MMC_EXT;



/*
 * Interrupt status
 */
#define	MMC_INTR_NONE			0
#define	MMC_INTR_COMMAND		(1 << 0)		// Command complete
#define	MMC_INTR_DATA			(1 << 1)		// Data complete
#define	MMC_INTR_RBRDY			(1 << 2)		// Read buffer ready, for PIO only
#define	MMC_INTR_WBRDY			(1 << 3)		// Write buffer ready, for PIO only
#define	MMC_INTR_CARDINS		(1 << 4)		// Card insertion detected
#define	MMC_INTR_CARDRMV		(1 << 5)		// Card removal detected
#define	MMC_INTR_OVERCURRENT	(1 << 6)		// Over current detected
#define	MMC_INTR_CARD			(MMC_INTR_CARDINS | MMC_INTR_CARDRMV | MMC_INTR_OVERCURRENT)
#define	MMC_INTR_ERROR			(1 << 15)		// Error detected
#define	MMC_ERR_DATA_END		(1 << 16)		// Data End Bit Error
#define	MMC_ERR_DATA_CRC		(1 << 17)		// Data CRC Error
#define	MMC_ERR_DATA_TO			(1 << 18)		// Data Timeout Error
#define	MMC_ERR_CMD_IDX			(1 << 19)		// Command Index Rrror
#define	MMC_ERR_CMD_END			(1 << 20)		// Command End Bit Rrror
#define	MMC_ERR_CMD_CRC			(1 << 21)		// Command CRC Rrror
#define	MMC_ERR_CMD_TO			(1 << 22)		// Command Timeout Rrror

/*
 * MMCSD pulse code
 */
#define	MMCSD_PULSE_CARD_INSERTION		5
#define	MMCSD_PULSE_CARD_REMOVAL		6
#define	MMCSD_PULSE_CARD_READY			7


extern int mmc_sim_args(char *options);
extern int mmc_sim_detach(void);
extern int mmc_sim_attach(CAM_ENTRY *centry);
extern long mmc_sim_init(SIM_HBA *hba, long path);
extern long mmc_sim_action(SIM_HBA *hba, CCB *ccb_ptr);
extern void *mmc_driver_thread(void *data);
extern void mmc_io_cmds(SIM_HBA *hba);
extern ssize_t mmc_slogf(SIM_HBA *hba, int opcode, int severity, int vlevel, const char *fmt, ...);

extern int	mmc_card_ident(SIM_HBA *hba, uint32_t intr);
extern int mmc_write_protect( SIM_HBA *hba, int op, uint32_t lba, uint32_t nlba );

#define	mmc_sendcmd(h, c, r, f, a)	_mmc_sendcmd(h, (c) | ((r) << 8) | ((f) << 16), a)
extern int _mmc_sendcmd(SIM_HBA *hba, uint32_t cmdflg, uint32_t argument);
extern int mmc_switch( SIM_HBA *hba, int flgs, uint32_t cmdset, uint32_t mode, uint32_t index, uint32_t value );
extern int mmc_send_rwcmd(SIM_HBA *hba, int nbytes, int flag);
extern int mmc_wait_command(SIM_HBA *hba);
extern int mmc_wait_card_ready(SIM_HBA *hba);
extern int mmc_erase( SIM_HBA *hba, int flgs, uint64_t lba, int nlba );
extern paddr_t mmc_mphys(CCB_SCSIIO *ccb, paddr_t dptr, int sgi);


extern int bs_init(SIM_HBA *hba);

struct _mmcsd_product {
	uint16_t	vid;			// vendor id
	uint16_t	did;            // device id
	uint32_t	pflags;
	char		*name;
	int			(*init)(SIM_HBA *);
};

#ifndef EXTERN
#define EXTERN_ADDED
#define EXTERN extern
#define	VALUE(x)
#else
#define VALUE(x) = { x }
#endif

EXTERN MMC_CTRL mmc_ctrl
#ifndef EXTERN_ADDED
= { { 0 },0 }
#endif
;

#ifdef EXTERN_ADDED
#undef EXTERN_ADDED
#undef EXTERN
#endif
#undef VALUE

#endif


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/sim_mmc.h $ $Rev: 657836 $" )
