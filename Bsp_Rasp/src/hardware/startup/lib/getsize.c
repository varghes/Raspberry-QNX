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
 * getsize.c
 *	User friendly data unit input
 */
#include "startup.h"

/*
 * getsize()
 *	Convert a string to a number
 *
 * In addition to straight numeric massaging, accepts suffixes to
 * scale the quantity.
 */
paddr_t
getsize(char *str, char **dst) {
    unsigned long v;
    
    v = strtopaddr(str, &str, 0);
    switch(*str) {
    case 'G':
    case 'g':
		v *= 1024L;
		/* fall through */
    case 'M':
    case 'm':
		v *= 1024L;
		/* fall through */
    case 'K':
    case 'k':
		v *= 1024L;
		++str;
		break;
    }
	if(dst != NULL) *dst = str;
    return(v);
}

__SRCVERSION("getsize.c $Rev: 657836 $");
