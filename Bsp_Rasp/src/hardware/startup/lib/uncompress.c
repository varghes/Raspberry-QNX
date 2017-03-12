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





#include "startup.h"

#ifndef SUPPORT_CMP_ZLIB
	#define	SUPPORT_CMP_ZLIB 0
#endif

#ifndef SUPPORT_CMP_LZO
	#define	SUPPORT_CMP_LZO 0
#endif

#ifndef SUPPORT_CMP_UCL
	#define	SUPPORT_CMP_UCL 1
#endif

void
uncompress(int type, paddr32_t dst_paddr, paddr32_t src_paddr) {
	uint8_t		*dst;
	uint8_t		*src;

	if (debug_flag > 0) kprintf("decompressing...");

	// We assume that the destination is in the one-to-one mapping area.
	dst = MAKE_1TO1_PTR(dst_paddr);
	src = startup_memory_map(shdr->stored_size - shdr->startup_size, src_paddr, PROT_READ);
	switch(type) {
#if SUPPORT_CMP_ZLIB
	case STARTUP_HDR_FLAGS1_COMPRESS_ZLIB:
		uncompress_zlib(dst, 0, src, 0,
				(uint8_t *)shdr->image_paddr + shdr->startup_size + shdr->imagefs_size);
		break;
#endif
#if SUPPORT_CMP_LZO
	case STARTUP_HDR_FLAGS1_COMPRESS_LZO:
		uncompress_lzo(dst, src);
		break;
#endif
#if SUPPORT_CMP_UCL
	case STARTUP_HDR_FLAGS1_COMPRESS_UCL:
		uncompress_ucl(dst, src);
		break;
#endif
	default:
		crash("unsupported compression type");
	}
	startup_memory_unmap(src);
}

__SRCVERSION("uncompress.c $Rev: 657836 $");
