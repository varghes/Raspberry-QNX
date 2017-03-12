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

int	cycles_per_loop;

static void inline
loop(unsigned count) {
	volatile unsigned	tmp = count;

	__asm__ __volatile__(
		"1:	subs	%0, %0, #1;"
		"	bgt		1b"
		:
		: "r" (tmp)
	);
}

unsigned long
arm_cpuspeed()
{
	unsigned	speed;
	unsigned	adjust;
	int			change;
	unsigned	start;
	unsigned	diff;
	unsigned	one_ms;
	unsigned	upwards;

	upwards = 0;
	adjust = 1000;
	speed = 1000;
	change = 1000;
	one_ms = timer_ns2tick(1000000000UL/1000);
	for( ;; ) {
		start = timer_start();
		loop(speed);
		diff = timer_diff(start);
		if(diff <= one_ms) {
			if(change < 0) adjust /= 2;
			change = adjust;
			if(diff < one_ms && change == 0) change = 1;
			++upwards;
		} else {
			if(change > 0) adjust /= 2;
			change = -adjust;
			upwards = 0;
		}
		if(change == 0) break;
		if((diff == 0) && (upwards > 100)) {
			crash("Timer not running. Possible configuration problem?\n");
		}
		speed += change;
	}

	/*
	 * Multiply by 3.01 and divide by 1000 to get MHz:
	 * 	- 3.00 => cycles per loop
	 *	- 0.01 => error correction
	 */
	speed = ((speed * cycles_per_loop) + (speed/100)) / 1000;
	return speed;
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/arm_cpuspeed.c $ $Rev: 657836 $" );
