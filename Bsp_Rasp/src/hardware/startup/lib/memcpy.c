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






#include <inttypes.h>
#include <string.h>
#undef memcpy

void *memcpy(void *dst, const void *src, size_t nbytes) {
	void			*ret = dst;
	unsigned		n;

	/* Both addresses must be aligned to stuff in int size chunks */
	if(		nbytes >= sizeof(unsigned) &&
			((uintptr_t)src & (sizeof(unsigned) - 1)) == 0 &&
			((uintptr_t)dst & (sizeof(unsigned) - 1)) == 0) {
		unsigned			*d = (unsigned *)dst;
		unsigned			*s = (unsigned *)src;


		n = ((nbytes>>2) + 15)/16;
                 
		switch ((nbytes>>2)%16){
		case 0:        do {  *d++ = *s++;
		case 15:             *d++ = *s++;
		case 14:             *d++ = *s++;
		case 13:             *d++ = *s++;
		case 12:             *d++ = *s++;
		case 11:             *d++ = *s++;
		case 10:             *d++ = *s++;
		case 9:              *d++ = *s++;
		case 8:              *d++ = *s++;
		case 7:              *d++ = *s++;
		case 6:              *d++ = *s++;
		case 5:              *d++ = *s++;
		case 4:              *d++ = *s++;
		case 3:              *d++ = *s++;
		case 2:              *d++ = *s++;
		case 1:              *d++ = *s++;
   	                   } while (--n > 0);
		}
              
		// remainder
		nbytes = nbytes & 0x3;


		if(nbytes) {
			dst = (unsigned char *)d;
			src = (const unsigned char *)s;
		}
	}

	/* Get the unaligned bytes, or the remaining bytes */
	while(nbytes) {
		*(unsigned char *)dst = *(const unsigned char *)src;
		dst = (char *)dst + 1;
		src = (const char *)src + 1;
		--nbytes;
	}

	return ret;
}

__SRCVERSION("memcpy.c $Rev: 657836 $");
