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

unsigned long
timer_tick2ns(unsigned long ticks) {
	int power = (-9) - lsp.qtime.p->timer_scale;
	unsigned long rate = lsp.qtime.p->timer_rate;
	unsigned long ns = 0;

	//
	//Tricky bit. Try to maintain maximum accuracy without overflowing
	//
	for( ;; ) {
		if(ns == 0) {
			ns = rate * ticks;
			if((ns / rate) != ticks) {
				//Overflow
				ns = 0;
			}
		}
		if(power <= 0) break;
		ns /= 10;
		rate /= 10;
		--power;
	}
	while(power < 0) {
		ns *= 10;
		++power;
	}
	return(ns);
}

__SRCVERSION("timer_tick2ns.c $Rev: 657836 $");
