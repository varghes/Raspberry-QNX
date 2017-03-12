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

// This is for all the transport layer prototypes and defines

#ifndef __XPT_H_INCLUDED
#define __XPT_H_INCLUDED

#include <cam.h>
#include <module.h>
#include <sys/cache.h>
#include <sys/resmgr.h>

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/include/xpt.h $ $Rev: 657836 $" )

struct _sim_hba;

#define XPT_XFLAG_BOUNCE				0x0001
#define XPT_XFLAG_CACHE					0x0002
#define XPT_XFLAG_VERIFY				0x0004
#define XPT_XFLAG_VERIFY_BYTCHK			0x0008
#define XPT_XFLAG_PNP					0x0010
#define XPT_XFLAG_ENUMERATING			0x0020
#define XPT_XFLAG_MATTR_SNOOPED			0x0040
#define XPT_XFLAG_RESMGR				0x0080
#define XPT_XFLAG_DISP_CCB_INTERNAL		0x0100
#define XPT_XFLAG_DISP_CCB_EXTERNAL		0x0200
#define XPT_XFLAG_IO_DONE				0x0400
#define XPT_XFLAG_COALESCE				0x0800

#define XPT_MAX_20BIT_ADDR				0xfffff
#define XPT_MAX_24BIT_ADDR				0xffffff
#define XPT_MAX_32BIT_ADDR				0xffffffff

#define XPT_MEM_FREE_SIZE_THRES			8192
#define XPT_MEM_POOL_HIWATER			4

typedef struct _xpt_mlist	XPT_MEM_LIST;

TAILQ_HEAD(_xpt_mlist,_xpt_mem_hdr);

typedef struct _xpt_mem_hdr {
	TAILQ_ENTRY(_xpt_mem_hdr)	hlink;
	int							mfd;
	int							size;
	int							mapf;
	int							protf;
	void						*vaddr;
	paddr64_t					paddr;
} XPT_MEM_HDR;

typedef struct _xpt_mem {
	int							vfd;			// virtual alloc fd
	int							cfd;			// contig alloc fd
	int							flags;
	int							thres;
	int							hiwater;
	int							flist_cnt;
	XPT_MEM_LIST				flist;
	XPT_MEM_LIST				ulist;
	pthread_mutex_t				mutex;
	uint64_t					min_paddr;
	uint64_t					max_paddr;
	uint64_t					align_paddr;
} XPT_MEM;

typedef struct _xpt {
	uint32_t			xflags;
	uint32_t			verbosity;
	uint32_t			vflags;
	uint32_t			lun_scan;
	uint32_t			xpt_max_ccb;
	uint32_t			xpt_max_edt;
	uint32_t			xpt_max_path;
	uint32_t			xpt_last_path;
	pthread_mutex_t		mutex;
	CCB_HEADER			*xpt_ccb_list;
	CAM_SIM_ENTRY		**cam_conftbl;
	CAM_EDT_ENTRY		*cam_edt;
	CAM_EDT_ENTRY		**cam_map;
	ASYNC_INFO			*async;			// XPT async callback list
	uint32_t			argc;
	char				**argv;
	MODULE_ENTRY		*sim_module;
	uint32_t			nsims;
	void				**sim_data;
	struct cache_ctrl	cachectl;
	int					pagesize;
	int					bounce_size;
	paddr_t				max_paddr;
	uint32_t			retries;
	dev_t				devno; 
	uint32_t			rsmgr_lo_water;
	uint32_t			rsmgr_hi_water;
	uint32_t			rsmgr_maximum;
	int					g1_timeout;		// default group 1 timeout (in seconds)
	int					g2_timeout;		// default group 2 timeout (in seconds)
	int					g3_timeout;		// default group 3 timeout (in seconds)
	int					rw_timeout;		// default read/write timeout (in seconds)
	XPT_MEM				mem;
	uid_t				uid;
	gid_t				gid;
	int					rsvd[17];
} XPT;

// QNX specific cam_vu_flags in CCB_SCSIIO
#define CAM_VUF_RW                  0x01    /* flag says read/write ccb */
#define CAM_VUF_DIO					0x02    /* flag says direct I/O ccb */
#define CAM_VUF_PTHRU               0x04    /* flag says pass through ccb */
#define CAM_VUF_OTHER               0x08    /* flag says internal ccb */
#define CAM_VUF_RETRY			    0x80

// QNX specific cam_vuhba_flags in CCB_PATHINQ
#define CAM_VUHBA_FLAGS				0		/* offset for flags */
#define CAM_VUHBA_FLAG_PHYS			0x01	/* flag says physical address supported */
#define CAM_VUHBA_FLAG_PTR			0x02	/* flag says ptr address is suported */
#define CAM_VUHBA_FLAG_DMA			0x04	/* flag says HBA uses DMA for transfering */
#define CAM_VUHBA_FLAG_DMA_LOW		0x08	/* flag says HBA uses ISA DMA for transfering */
#define CAM_VUHBA_FLAG_DMA_NOX64K	0x10	/* flag says HBA can't handle X 64K buffers */
#define CAM_VUHBA_FLAG_ATAPI		0x20	/* flag says HBA is an ATAPI device ( use 10 byte commands ) */
#define CAM_VUHBA_FLAG_BMSTR		0x40	/* flag says HBA busmasters */
#define CAM_VUHBA_FLAG_MLUN         0x80    /* flag says HBA can handle multi-lun devices */
#define CAM_VUHBA_EFLAGS			1		/* offset for extended flags */
#define CAM_VUHBA_EFLAG_PNP			0x01	/* flag says plug and play driver */
#define CAM_VUHBA_EFLAG_DMA_20		0x02	/* flag says HBA supports 20 Bit physical addresses */
#define CAM_VUHBA_EFLAG_DMA_24		0x04	/* flag says HBA supports 24 Bit physical addresses */
#define CAM_VUHBA_EFLAG_DMA_32		0x08	/* flag says HBA supports 32 Bit physical addresses */
#define CAM_VUHBA_EFLAG_DMA_64		0x10	/* flag says HBA supports 64 Bit physical addresses */
#define CAM_VUHBA_EFLAG_DEVNAME		0x20	/* flag says set device name using INQ vend_spc */
#define CAM_VUHBA_MAX_SG			2		/* Maximun # of sg elements on this path */
#define CAM_VUHBA_MAX_LINKED		3		/* Maximun # of linked in a chain 0=unlimited */
#define CAM_VUHBA_HEADS				4		/* Number of Heads BIOS presents for fdisk disk 0, 0=default */
#define CAM_VUHBA_HEADS1			5		/* Number of Heads BIOS presents for fdisk disk 1, 0=default */
#define CAM_VUHBA_SECTS				6		/* Number of Sectors/Trk BIOS presents for fdisk disk 0, 0=default */
#define CAM_VUHBA_SECTS1			7		/* Number of Sectors/Trk BIOS presents for fdisk disk 1, 0=default */
#define CAM_VUHBA_MAX_IO			8
#define CAM_VUHBA_MAX_IO1			9

/*
 * The SIM calls this routine to inform the XPT that an async event has occured 
 * and that there may be peripheral drivers which need to be informed. 
 * 
 *  - The opcode, path_id, target_id, lun, and data_cnt arguments are long 32-bit 
 *    values. 
 *  - The path_id, target_id, and lun define a nexus for the Async Callback.
 *  - The opcode contains the value for what has happened. 
 *  - The buffer_ptr and data_cnt are used to inform the XPT where and how much 
 *    data is associated with the opcode. 
 */
void xpt_async(long opcode, long path_id, long target_id, long lun,
                            void *buffer_ptr, long data_cnt);


/*
 * The SIM shall call the XPT once for each supported bus in order to obtain the 
 * Path ID for that bus. 
 * 
 * The argument is the pointer for the data structure defining the entry points 
 * for the SIM. The value returned is the assigned Path ID; a value of -1 
 * indicates that registration was not successful. 
 */
long xpt_bus_register(CAM_SIM_ENTRY *sim_entry, struct _sim_hba *sim_data);


/*
 * The argument is the Path ID for the bus being de-registered. A return value of 
 * zero indicates the bus is no longer registered, any other value indicates the 
 * call was unsuccessful.
 */
long xpt_bus_deregister(long path_id);


/*
 * This routine is called by the peripheral driver to request that the XPT and 
 * sub-layers be initialized. Once the sub-layers are initialized any subsequent 
 * calls by other peripheral drivers shall quickly return. 
 * 
 * There are no arguments and the CAM Status is either CAM_SUCCESS of CAM_FAILURE.
 */
long xpt_init(void);


/*
 * This routine is used whenever a peripheral driver needs a CCB (the common data 
 * structure for processing SCSI requests). It returns a pointer to the allocated 
 * CCB which the peripheral driver can now use as the CCB for it's SCSI/XPT 
 * requests. The returned CCB shall be properly initialized for use as a SCSI I/O 
 * Request CCB. The SIM Private Data area shall have been already set up to be 
 * used by the XPT and SIM, and shall not be modified by the peripheral driver. 
 * 
 * There are no arguments and the return value is a pointer to an initialized 
 * CCB. 
 */
CCB *xpt_ccb_alloc(void);


/*
 * This routine takes a pointer to the CCB that the peripheral driver has 
 * finished with so it can be returned to the CAM subsystem CCB pool. 
 * 
 * The argument is the pointer to the CCB to be freed, there is no CAM Status. 
 */
void xpt_ccb_free(CCB *ccb);


/*
 * All CAM/SCSI CCB requests to the XPT/SIM are placed through this function 
 * call. All returned CAM status information is obtained at the callback point 
 * via the CAM and SCSI status fields. 
 * 
 * The argument is a pointer to the CCB, and the CAM Status is either Success or 
 * Failure. 
 */
long xpt_action(CCB *ccb);

long xpt_setasync_cb( ASYNC_INFO **alist, CCB_SETASYNC *ccb );

int xpt_scan_bus(int path);

long xpt_scan_devs(int type, int (*func)( CCB_PATHINQ *, CCB_GETDEV *));

CAM_EDT_ENTRY *xpt_nexus( int path, int target, int lun, int create );

int xpt_smart_cache( );

/* virtual to physical address translation routines */
CAM_PM_OFFSET xpt_vtop( CAM_VM_OFFSET addr, CAM_VM_OFFSET cam_map );
int xpt_vtop_sg( SG_ELEM *vsg, SG_ELEM *psg, int nsg, CAM_VM_OFFSET cam_map );

#define XPT_ALLOC_CONTIG		0x01
#define XPT_ALLOC_NOCACHE		0x02
void *xpt_alloc( int alfg, size_t size, paddr64_t *paddr );
int xpt_free( CAM_VM_OFFSET addr, size_t size );

#endif
