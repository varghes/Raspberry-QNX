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



#ifndef _RESTORE_IFS_H_
#define _RESTORE_IFS_H_

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

/* Keep the flags as chars so they are endian neutral */
#define		RIFS_FLAG_NONE				0x00	/* Restore IFS disabled */
#define		RIFS_FLAG_ENABLE			0x01	/* Restore IFS enabled */
#define		RIFS_FLAG_CKSUM				0x02	/* Cksum verification enabled */
#define		RIFS_FLAG_IFS2				0x04	/* Enable secondary (non-bootable) IFS */
#define		RIFS_FLAG_IFS2_RESTORE		0x08	/* Restore secondary (non-bootable) IFS */
#define		RIFS_FLAG_IFS2_CKSUM		0x10	/* Cksum verfication enabled on IFS2 */
#define		RIFS_FLAG_IFS2_SRC			0x20	/* Manually specify IFS2 source */
#define		RIFS_FLAG_IFS2_DST			0x40	/* Manually specify IFS2 destination */

#include _NTO_HDR_(_pack64.h)

#define RIFS_SIGNATURE		"rifsboot"
#define RIFS_SIGNATURE_REV	"toobsfir"
#define RIFS_MAX_BOOTABLE	5			/* Max bootable executables supported in image */

/* Information about individual bootable executables */
struct restore_ifs_elf {
	unsigned long		offset;		/* Offset of writeable elf data region from the start of the IFS */
	unsigned long		size;		/* Size of writeable elf data */
	paddr32_t			data;		/* Location to store elf data for compressed images only (must be in 1-to-1 mapping region) */
};

/* IFS information stored persistently across boots */
struct restore_ifs_info {
	char					signature[8];	/* Signature of this data structure */
	unsigned long			cksum;			/* Checksum for this data structure */
	unsigned long			image_size;		/* Size of the IFS used for checksum */	
	struct restore_ifs_elf	elfinfo[RIFS_MAX_BOOTABLE];	/* ELF info for each bootable executable */
	unsigned long			numboot;		/* Number of bootable executables in IFS */	
};

/* IFS2 (non-bootable) information stored persistently across boots */
struct restore_ifs2_info {
	char					signature[8];	/* Signature of this data structure */
	unsigned long			cksum;			/* Checksum for this data structure */
	unsigned long			image_size;		/* Size of the IFS used for checksum */	
};

#include _NTO_HDR_(_packpop.h)

/* Function prototypes */
void rifs_set_cksum(struct image_header *ifs_hdr);
int rifs_save_elf32data(paddr32_t addr, union image_dirent *dir, int numboot);
int rifs_restore_ifs(paddr32_t ifs_paddr);
void load_ifs2_nonbootable(void);
int rifs_load_ifs2(void);
int rifs_restore_ifs2(void);

extern struct restore_ifs_info 		*rifs_info;
extern struct restore_ifs2_info 	*rifs2_info;
extern unsigned 					rifs_flag;
extern paddr32_t 					ifs2_paddr_src;
extern paddr32_t 					ifs2_paddr_dst;
extern unsigned 					ifs2_size;
extern unsigned 					mdriver_cksum_max;

#endif 

/* __SRCVERSION("restore_ifs.h $Rev: 657836 $"); */
