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


/*
* IMX51 SOC devices
*
* This file contains names for all of the devices that may exists on IMX51
* processor. Specifically, these are the internal devices that are part
* of the IMX51 SOC and not board specific devices. Board specific devices
* are defined in a similarly named board specific file
* the startup writer) so that drivers can use hwi_find_device() to determine
* the existence of SOC device instances in a portable fashion and optionally
* retrieve device specific information like interrupt vectors, etc.
* 
* Not all of the devices are on every processor so hwi_find_device() may return
* HWI_NULL_OFF if a device, or device instance (ie. unit) is not found.
*
*/

#ifndef __HWINFO_IMX51_PROTO_H_INCLUDED
#define __HWINFO_IMX51_PROTO_H_INCLUDED


/* FIX ME - consider a single second parameter as a pointer to a structure containing relevant attributes */
/* FIX ME - some of these go away as common devices are in sys/hwinfo.h */

extern void hwidev_add_imx51_sdhc(const char *device_name, unsigned bus_hwi_off, paddr_t base_addr);

/* FIX ME - I think what follows will be considered imx51 specific devices */
extern void hwidev_add_imx51_gpio(const char *device_name, unsigned bus_hwi_off, paddr_t base_addr, unsigned num);

typedef enum
{
        hwi_imx51_flags_t_NONE           = 0,
        hwi_imx51_flags_t_USE_MAC        = (1UL << 0),           // create a NIC address tag for ENET devices (must be set in BSP)
        hwi_imx51_flags_t_I2C_ONLY       = (1UL << 1),           // only create I2C devices (this is for EEPROM reads on Freescale boards)
	hwi_imx51_flags_t_I2C_NONE       = (1UL << 2),           // don't create I2C devices (so that hwi_p2020() can be called twice without duplicates)
        hwi_imx51_flags_t_SEC_NONE       = (1UL << 3),           // don't create SECURITY devices

} hwi_imx51_flags_t;

extern void hwi_imx51();

#endif  /* __HWINFO_IMX51_PROTO_H_INCLUDED */

__SRCVERSION("hwinfo_imx51_proto.h $Rev: 169799 $");
