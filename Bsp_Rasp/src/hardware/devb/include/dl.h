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

#include <dlfcn.h>

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/include/dl.h $ $Rev: 657836 $" )

struct dll_syms {
	char	*symname;
	void	*addr;
};

#ifdef DLL_cam
#include "dl_cam.h"
#endif

#ifdef DLL_cam_optical
#include "dl_optical.h"
#endif

#ifdef DLL_cam_disk
#include "dl_disk.h"
#endif

#ifdef DLL_cam_cdrom
#include "dl_cdrom.h"
#endif

#ifdef DLL_io_blk
#include "io-blk/dl_io_blk.h"
#endif

#ifdef DLL_qnx4
#include "fs/qnx4/dl_qnx4fs.h"
#endif

#ifdef DLL_cd
#include "fs/cd/dl_cdfs.h"
#endif

#ifdef DLL_dos
#include "fs/dos/dl_dosfs.h"
#endif

#ifdef DLL_ext2
#include "fs/ext2/dl_ext2fs.h"
#endif

#ifdef DLL_udf
#include "fs/udf/dl_udf.h"
#endif

#ifdef DLL_qnx6
#include "fs/qnx6/dl_qnx6fs.h"
#endif

#ifdef DLL_nt
#include "fs/nt/dl_ntfs.h"
#endif

#ifdef DLL_mac
#include "fs/mac/dl_macfs.h"
#endif

#ifdef DLL_exfat
#include "fs/exfat/dl_exfatfs.h"
#endif

struct dll_list {
	char					*fname;
	const struct dll_syms	*syms;
};

static const struct dll_list	dll_list[] = {
#ifdef DLL_IO_BLK_LIST
									{ DLL_IO_BLK_LIST },
#endif

#ifdef CAM_LIST
									{ CAM_LIST },
#endif

#ifdef CAM_OPTICAL_LIST
									{ CAM_OPTICAL_LIST },
#endif

#ifdef CAM_DISK_LIST
									{ CAM_DISK_LIST },
#endif

#ifdef CAM_CDROM_LIST
									{ CAM_CDROM_LIST },
#endif

#ifdef DLL_QNX4_LIST
									{ DLL_QNX4_LIST },
#endif

#ifdef DLL_CD_LIST
									{ DLL_CD_LIST },
#endif

#ifdef DLL_DOS_LIST
									{ DLL_DOS_LIST },
#endif

#ifdef DLL_EXT2_LIST
									{ DLL_EXT2_LIST },
#endif

#ifdef DLL_UDF_LIST
									{ DLL_UDF_LIST },
#endif

#ifdef DLL_QNX6_LIST
									{ DLL_QNX6_LIST },
#endif

#ifdef DLL_NTFS_LIST
									{ DLL_NTFS_LIST },
#endif

#ifdef DLL_MAC_LIST
									{ DLL_MAC_LIST },
#endif

#ifdef DLL_EXFAT_LIST
									{ DLL_EXFAT_LIST },
#endif

									{ NULL, NULL }
								};
