/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. 
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
* EP93xx SOC devices
*
* This file contains names for all of the devices that may exists on any Cirrus
* Logic EP93xx processor variant. Specifically, these are the internal devices
* that are part of the EP93xx SOC and not board specific devices. Board specific
* devices are defined in a similarly named board specific file
* 
* The names are added to the hwinfo section of the syspage (at the discretion of
* the startup writer) so that drivers can use hwi_find_xxx() to determine
* the existence of SOC bus/device instances in a portable fashion and optionally
* retrieve device specific information like interrupt vectors, etc.
* 
* Not all of the devices are on every processor so hwi_find_xxx() may return
* HWI_NULL_OFF if a bus/device, or bus/device instance (ie. unit) is not found.
*
*/

#ifndef _HWINFO_EP93xx_PROTO_H_
#define _HWINFO_EP93xx_PROTO_H_


extern void hwidev_add_ep93xx_gpio(const char *device_name, unsigned bus_hwi_off, paddr_t base_addr, unsigned num);
extern void hwidev_add_ep93xx_timer_group(const char *device_name, unsigned bus_hwi_off, paddr_t base_addr, unsigned num);
extern void hwidev_add_ep93xx_ext_irq(const char *device_name, unsigned bus_hwi_off, unsigned num);

typedef enum
{
	hwiep93xx_flags_t_NONE		= 0,
	hwiep93xx_flags_t_USE_MAC	= (1UL << 0),		// create a NIC address tag for ENET devices (must be set in BSP)
	hwiep93xx_flags_t_SSPisSPI	= (0UL << 1),		// configure the SSP as a Motorola SPI bus (default)
	hwiep93xx_flags_t_SSPisSSI	= (1UL << 1),		// configure the SSP as a TI SSI bus
	hwiep93xx_flags_t_SSPisNSS	= (2UL << 1),		// configure the SSP as a National Semi serial bus
	hwiep93xx_flags_t_SSPisI2S	= (3UL << 1),		// configure the SSP pins for I2S
	hwiep93xx_flags_t_SSP_mask	= (3UL << 1),		// mask for SSPisxxx
	
} hwiep93xx_flags_t;

extern void hwi_ep9301(hwiep93xx_flags_t flags);



#endif	/* _HWINFO_EP93xx_PROTO_H_ */

__SRCVERSION("hwinfo_ep93xx_proto.h $Rev: 169799 $");

