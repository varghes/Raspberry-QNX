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





char *optarg;
int  optind = 0;

int getopt (int argc, char **argv, char *opts) {
    static	char	*cp;
    static	int		last_optind;
    int 			opt;

	if (optind == 0) {
		optind = 1;
		last_optind = 0;
	}
	if (optind >= argc) {
		return -1;
	}
	if (last_optind != optind) {
		cp = argv [optind];
		last_optind = optind;
		if (*cp++ != '-' || *cp == 0) {
			return -1;
		}
	}
	if (*cp == '-') {
		++optind;
		return -1;
	}
	for (; *opts; ++opts) {
		if (*opts == ':') continue;
		if (*cp == *opts) break;
	}
	optarg = cp;
	++optind;
	opt = *opts++;
	if(opt) {
		if (*++cp) {
			if (*opts == ':') {
				optarg = cp;
			} else {
				--optind;
			}
		} else {
			if (*opts == ':') {
				if (optind < argc) {
				    optarg = argv [optind];
					optind++;
				} else {
					opt = '?';
				}
			}
		}
	} else {
		opt = '?';
	}
	return opt;
}

#include <sys/platform.h>
__SRCVERSION("getopt.c $Rev: 657836 $");
