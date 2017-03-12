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

/*
 *  dcmd_sim_mmcsd.h   Non-portable low-level devctl definitions
 *
*/

#ifndef __DCMD_SIM_MMCSD_H_INCLUDED
#define __DCMD_SIM_MMCSD_H_INCLUDED

#ifndef _DEVCTL_H_INCLUDED
 #include <devctl.h>
#endif

#include <_pack64.h>

#define MMCSD_MAX_SLOTNAME_LEN	32
/* CID reg values of the card */
typedef struct _mmcsd_cid {
	_Uint32t		flags;	/*set MMCSD_FULL_CID flag t0 return raw/full cid contents */
	_Uint8t         rsvd[4];
	union{
		struct{
			_Uint32t            cid[4];
		}full_cid;
		union{
			struct{
				uint8_t 	mid;		// Manufacture ID
				uint8_t 	oid[3]; 	// OEM/Application ID
				uint8_t 	pnm[6]; 	// Product name
				uint8_t 	prv;		// Product revision
				uint32_t	psn;		// Product serial number
				uint16_t	mdt;		// Manufacture date
			}sd_cid;
			struct{
				uint32_t	mid;		// Manufacture ID
				uint16_t	oid;		// OEM ID
				uint8_t 	pnm[8]; 	// Product name
				uint8_t 	hwr;		// HW revision
				uint8_t 	fwr;		// FW revision
				uint32_t	psn;		// Product serial number
				uint8_t 	mcd;		// Month code
				uint16_t	ycd;		// Year code
			}mmc_cid;
		}parsed_cid;
	}cid;
	pid_t		pid;    			/* Store PID of driver process for this device */
	_Uint32t	speed; 				/*  Card speed currently working at */
	_Uint32t	media_change; 		/* Media change counter */
	_Uint8t		hwspec_version;		/* physical layer spec */
	_Uint8t		csd_version;		/* CSD structure version */
	_Uint8t		mmcprot_version;	/* MMC proto version */
	_Uint8t	type;				/* card type, MMC or SD for now */
	char		slotname[MMCSD_MAX_SLOTNAME_LEN];		/* slot name */
} MMCSD_CID;

typedef struct _mmc_csd_ext {
	uint32_t	hs_max_dtr;
	uint32_t	sectors;
	uint8_t		erase_grp_def;
	uint8_t		hc_erase_group_size;
	uint8_t		hc_wp_grp_size;
	uint8_t		user_wp;
} MMC_CSD_EXT;

typedef struct _mmcsd_csd {
	_Uint32t	flags;  /*set MMCSD_FULL_CID flag t0 return raw/full cid contents */
	_Uint8t		rsvd[4];
	union {
		struct {
			uint8_t		csd_structure;  // CSD structure
			uint8_t		taac;
			uint8_t		nsac;
			uint8_t		tran_speed;
			uint16_t	ccc;
			uint8_t		read_bl_len;
			uint8_t		read_bl_partial;
			uint8_t		write_blk_misalign;
			uint8_t		read_blk_misalign;
			uint8_t		dsr_imp;
			union {
				struct {
					uint16_t	c_size;
					uint8_t		vdd_r_curr_min;
					uint8_t		vdd_r_curr_max;
					uint8_t		vdd_w_curr_min;
					uint8_t		vdd_w_curr_max;
					uint8_t		c_size_mult;
				} csd_ver1;
				struct {
					uint32_t	c_size;
				} csd_ver2;
			}csd;
			uint8_t		erase_blk_en;
			uint8_t		sector_size;
			uint8_t		wp_grp_size;
			uint8_t		wp_grp_enable;
			uint8_t		r2w_factor;
			uint8_t		write_bl_len;
			uint8_t		write_bl_partial;
			uint8_t		file_format_grp;
			uint8_t		copy;
			uint8_t		perm_write_protect;
			uint8_t		tmp_write_protect;
			uint8_t		file_format;
		} sd_csd;
		struct {
			uint8_t		csd_structure;  // CSD structure
			uint8_t		mmc_prot;
			uint8_t		taac;
			uint8_t		nsac;
			uint8_t		tran_speed;
			uint16_t	ccc;
			uint8_t		read_bl_len;
			uint8_t		read_bl_partial;
			uint8_t		write_blk_misalign;
			uint8_t		read_blk_misalign;
			uint8_t		dsr_imp;
			uint16_t	c_size;
			uint8_t		vdd_r_curr_min;
			uint8_t		vdd_r_curr_max;
			uint8_t		vdd_w_curr_min;
			uint8_t		vdd_w_curr_max;
			uint8_t		c_size_mult;
			union {
				struct {	/* MMC system specification version 3.1 */
					uint8_t erase_grp_size;
					uint8_t erase_grp_mult;
				} mmc_v31;
				struct {	/* MMC system specification version 2.2 */
					uint8_t sector_size;
					uint8_t erase_grp_size;
				} mmc_v22;
			} erase;
			MMC_CSD_EXT	ext_csd;
			uint8_t		wp_grp_size;
			uint8_t		wp_grp_enable;
			uint8_t		r2w_factor;
			uint8_t		write_bl_len;
			uint8_t		write_bl_partial;
	//      uint8_t		file_format_grp;
			uint8_t		copy;
			uint8_t		perm_write_protect;
			uint8_t		tmp_write_protect;
			uint8_t		ecc;
		} mmc_csd;
	} csd;
	pid_t		pid;				/* Store PID of driver process for this device */
	_Uint8t		csd_version;		/* CSD structure version */
	_Uint8t		mmcprot_version;	/* MMC proto version */
	_Uint8t		type;				/* card type, MMC or SD for now */
} MMCSD_CSD;

/*flags definition */
#define MMCSD_FULL_CID			0x80000000	/* The client request a raw/full CID instead of parsed CID*/
#define MMCSD_ECC_INFO          0x40000000 /* The client request ECC error information */
#define MMCSD_DEV_RDONLY        0x00000001 /* write protected */
#define MMCSD_DEV_NO_MEDIA      0x00000002 /* no media inserted */
#define MMCSD_DEV_RDY      		0x00000004 /* Media is ready to accept IO */
#define MMCSD_DEV_PRELOAD      	0x00000008 /* device is in the slot before the driver started */
#define MMCSD_DEV_LOCKED        0x00000010 /* device is locked */
#define MMCSD_DEV_MEDIA_ERROR   0x00000020 /* device inserted but there are error when identify it */
#define MMCSD_DEV_ECC           0x00000040 /* device has ECC error */

#define MMCSD_CARD_TYPE_UNKNOWN	0x0
#define MMCSD_CARD_TYPE_MMC		0x1
#define MMCSD_CARD_TYPE_SD		0x2

#define MMCSD_WP_ACTION_CLR		0x00
#define MMCSD_WP_ACTION_SET		0x01
typedef struct _mmcsd_write_protect {
	_Uint32t		action;
	_Uint32t		rsvd;
	_Uint64t		lba;
	_Uint64t		nlba;
	_Uint64t		rsvd2;
} MMCSD_WRITE_PROTECT;

#define DCMD_MMCSD_GET_CID       	__DIOTF(_DCMD_CAM, _SIM_MMCSD + 0, struct _mmcsd_cid)
#define DCMD_MMCSD_WRITE_PROTECT   	__DIOTF(_DCMD_CAM, _SIM_MMCSD + 1, struct _mmcsd_write_protect)
#define DCMD_MMCSD_GET_CSD      	__DIOTF(_DCMD_CAM, _SIM_MMCSD + 2, struct _mmcsd_csd)
#define DCMD_MMCSD_GET_ECCERR_ADDR 	__DIOTF(_DCMD_CAM, _SIM_MMCSD + 3, int32_t)

#include <_packpop.h>

#endif
