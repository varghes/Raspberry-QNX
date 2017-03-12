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

/*
	Override the C library's definition because we don't need all of
	it's fluff.
*/

#define hexstr(p) (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))

static int
radix_value( char c ) {
	if(c >= '0'  &&  c <= '9') return(c - '0');
	if(c >= 'a'  &&  c <= 'f') return(c - 'a' + 10);
	if(c >= 'A'  &&  c <= 'F') return(c - 'A' + 10);
	return( 37 );
}

paddr_t
strtopaddr(const char *nptr, char **endptr, int base) {
	const char	*p;
	const char	*startp;
	int 		digit;
	paddr_t		value;

	if(endptr != NULL) *endptr = (char *)nptr;
	p = nptr;
	if(base == 0) {
	    if(hexstr(p))	 base = 16;
	    else if(*p == '0') base = 8;
	    else		 base = 10;
	}
	if(base == 16) {
	    if(hexstr(p))  p += 2;	/* skip over '0x' */
	}
	startp = p;
	value = 0;
	for(;;) {
	    digit = radix_value(*p);
	    if(digit >= base) break;
	    value = value * base + digit;
	    ++p;
	}
	if(p == startp)  p = nptr;
	if(endptr != NULL) *endptr = (char *)p;
	return value;
}

unsigned long
strtoul(const char *nptr, char **endptr, int base) {
	//Assuming that sizeof(paddr_t) >= sizeof(unsigned long)
	return strtopaddr(nptr, endptr, base);
}

__SRCVERSION("strtoul.c $Rev: 657836 $");
