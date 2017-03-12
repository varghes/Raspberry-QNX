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

//
// Make sure the ethernet isn't generating any interrupts or receiving
// data. Note that even if we turn the chip off, there could be a packet
// in the middle of being received so memory still might be modified following
// the return of this function. However, this is called from the near
// the begining of startup and we're unlikely to be using the memory
// until the OS itself gets going. By that time, we should have finished
// receiving any in-progress packet.
//

#define	DEC_CSR0			0x00	/* Bus Mode */
	#define	CSR0_SW_RESET	(1<<0)
#define	DEC_CSR6	0x30	/* Operation Mode */
	#define	CSR6_START_TX	(1<<13)
	#define	CSR6_START_RX	(1<<1)
#define	DEC_CSR7			0x38	/* Interrupt Enable */

void
tulip_reset(paddr_t base, int mem_mapped) {
	uintptr_t IoBase;

	IoBase = startup_io_map(0x100, base);

	/* Stop Board */
	out32(IoBase + DEC_CSR6, in32(IoBase + DEC_CSR6) &
	    ENDIAN_LE32(~(CSR6_START_TX | CSR6_START_RX)));

	/* Disable Interrupts + Reset Board */
	out32(IoBase + DEC_CSR7, ENDIAN_LE32(0));
	out32(IoBase + DEC_CSR0, ENDIAN_LE32(CSR0_SW_RESET));

	startup_io_unmap(IoBase);
}

__SRCVERSION("hw_tulip_reset.c $Rev: 657836 $");
