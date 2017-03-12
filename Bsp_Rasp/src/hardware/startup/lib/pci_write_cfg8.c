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
 * NOTE: Depends on pci_write_cfg32 (architecture specific)
 * 
 * This is architecture independant code expressed in terms of the architecture
 * specific pci_read_cfg32. In other words, to use this function, the target
 * architecture must have implemented pci_write_cfg32. For an example of this,
 * refer to x86/pci_write_cfg32.c.
 */
void pci_write_cfg8(uint8_t bus, uint8_t dev_number, uint8_t func_number, uint8_t reg_offset, uint8_t value)
{
	uint32_t temp;
	uint8_t shift = (8 * (reg_offset & 3));
	
	temp = pci_read_cfg32(bus, dev_number, func_number, (reg_offset & ~3));
	temp &= ~(0xff << shift);
	temp |= (((uint32_t)value) << shift);
	pci_write_cfg32(bus, dev_number, func_number, (reg_offset & ~3), temp);
}

__SRCVERSION("pci_write_cfg8.c $Rev$");
