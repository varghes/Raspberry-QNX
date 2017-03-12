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
#include <drvr/hwinfo.h>


/*******************************************************************************
 * hwi_set_nicaddr
 * 
 * Unconditionally set the NIC (mac) address for the ethernet device
 * corresponding to <hwi_off> to <nic_addr>.
 * The API allows for the setting of a specific NIC instance in the case where
 * there could be more than 1 NIC address per ethernet device. Typically this
 * would not be the case and <nic_idx> will be 0 indicating the first instance
 * 
 * Returns: true if the specified tag was successfully set, otherwise false 
 * 
*/
int hwitag_set_nicaddr(unsigned hwi_off, unsigned nic_idx, uint8_t *nic_addr)
{
	hwi_tag *tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_nicaddr, &nic_idx);
	if (tag != NULL) memcpy(tag->nicaddr.addr, nic_addr, tag->nicaddr.len);
	return (tag == NULL) ? 0 : 1;
}

__SRCVERSION("$URL$ $Rev$");
