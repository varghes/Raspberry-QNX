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

const void *
find_startup_info( const void *start, unsigned type ) {
	const struct	startup_info_hdr	*p;

	if( start == NULL ) {
		p = (const void *)&shdr->info[0];
	} else {
		p = start;
		p = (const void *)((const char *)p + p->size);
	}
	for( ;; ) {
		if( p->size == 0 ) return 0;
		if( p->type == type ) return p;
		p = (const void *)((const char *)p + p->size);
	}
}

__SRCVERSION("startup_info.c $Rev: 657836 $");
