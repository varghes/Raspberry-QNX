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
#include <ucl/ucl.h>

void
uncompress_ucl(uint8_t *dst, uint8_t *src) {
	unsigned	len;
	ucl_uint	out_len;
	int			status;

	for(;;) {
		len = (*src++ << 8);
		len += (*src++);
		if(len == 0) break;
		status = ucl_nrv2b_decompress_8(src, len, dst, &out_len, NULL);
		if(status != 0) {
			crash("fail");
		}
		dst += out_len;
		src += len;
	}
}

__SRCVERSION("uncompress_ucl.c $Rev: 657836 $");
