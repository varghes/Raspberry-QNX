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

/**
 * NOTE: Depends on pci_read_cfg32 (architecture specific)
 * 
 * This is architecture independant code expressed in terms of the architecture
 * specific pci_read_cfg32. In other words, to use this function, the target
 * architecture must have implemented pci_read_cfg32. For an example of this,
 * refer to x86/pci_read_cfg32.c.
 */
uint16_t pci_read_cfg16(uint8_t bus, uint8_t dev_number, uint8_t func_number, uint8_t reg_offset)
{
	uint32_t temp;
	
	if (reg_offset & 1) {
		/**
		 * We don't support unaligned 16bit reads.
		 */
		crash("Unaligned 16bit PCI read.\n");
	}
	
	temp = pci_read_cfg32(bus, dev_number, func_number, (reg_offset & ~3));
	temp >>= (8 * (reg_offset & 3));
	return (uint16_t)(temp);
}

__SRCVERSION("pci_read_cfg16.c $Rev$");
