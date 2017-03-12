/*
 * $QNXLicenseC:
 * Copyright 2008,2009, QNX Software Systems. 
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


extern void	print_syspage_enable(const char *name);
extern void	print_syspage_sections(void);

void
print_syspage(void) {
	int			opt;

	if(debug_flag > 1) {
		//We enable things here so that this code doesn't get
		//hauled in unless the user calls print_syspage().
		optind = 0;
		while((opt = getopt(_argc, _argv, "S:")) != -1) {
			if(opt == 'S') {
				print_syspage_enable(optarg);
			}
		}
		print_syspage_sections();
	}
}

__SRCVERSION("debug.c $Rev: 657836 $");
