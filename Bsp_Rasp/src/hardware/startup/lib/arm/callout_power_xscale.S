#
# Copyright 2007, 2008, QNX Software Systems. 
# 
# Licensed under the Apache License, Version 2.0 (the "License"). You 
# may not reproduce, modify or distribute this software except in 
# compliance with the License. You may obtain a copy of the License 
# at: http://www.apache.org/licenses/LICENSE-2.0 
# 
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OF ANY KIND, either express or implied.
#
# This file may contain contributions from others, either as 
# contributors under the License or as licensors under other terms.  
# Please review this entire file for other proprietary rights or license 
# notices, as well as the QNX Development Suite License Guide at 
# http://licensing.qnx.com/license-guide/ for other information.
#
/*
 * Xscale power callout.
 */

#include "callout.ah"

/*
 * --------------------------------------------------------------------------
 * Set IDLE mode in CP14 register 7.
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - flag (currently always 0)
 *
 * FIXME: currently, this callout is only called from the ARM specific halt()
 *        function in the kernel. If this becomes used for a general purpose
 *        power management facility, we will need to handle arguments.
 * --------------------------------------------------------------------------
 */
CALLOUT_START(power_xscale, 0, 0)

	teq		r1, #0
	bne		1f
	
	mov		r0, #1 
	mcr		p14, 0, r0, c7, c0, 0
	
1:
	mov		r0, #0
	mov		pc, lr

CALLOUT_END(power_xscale)

