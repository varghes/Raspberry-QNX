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
* PPC85xx SOC devices
*
* This file contains names for all of the devices that may exists on any 85xx
* processor variant. Specifically, these are the internal devices that are part
* of the MPC85xx SOC and not board specific devices. Board specific devices
* are defined in a similarly named board specific file
* 
* The names are added to the hwinfo section of the syspage (at the discretion of
* the startup writer) so that drivers can use hwi_find_device() to determine
* the existence of SOC device instances in a portable fashion and optionally
* retrieve device specific information like interrupt vectors, etc.
* 
* Not all of the devices are on every processor so hwi_find_device() may return
* HWI_NULL_OFF if a device, or device instance (ie. unit) is not found.
*
*/

#ifndef __HWINFO_85xx_H_INCLUDED
#define __HWINFO_85xx_H_INCLUDED

#include <hw/sysinfo.h>

/*
 * =============================================================================
 * 
 *                                B U S E S
 * 
 * =============================================================================
*/
/* PCIe - do we need Port specific PCIe ? */
#define PPC85xx_HWI_PCI			HWI_ITEM_BUS_PCI
#define PPC85xx_HWI_PCIe		HWI_ITEM_BUS_PCI

/* I2C - 1 interrupt */
#define PPC85xx_HWI_I2C			HWI_ITEM_BUS_I2C

/* USB */
#define PPC85xx_HWI_USB			HWI_ITEM_BUS_USB

/* SPI */
#define PPC85xx_HWI_SPI			HWI_ITEM_BUS_SPI

/* LBC - 1 interrupt */
#define PPC85xx_HWI_LBC			"lbc"

/*
 * =============================================================================
 * 
 *                              D E V I C E S
 * 
 * =============================================================================
*/
/* SDHC */
#define PPC85xx_HWI_SDHC		"sdhc"

/* SATA */
#define PPC85xx_HWI_SATA		"sata"

/* L2CACHE - 1 interrupt */
#define PPC85xx_HWI_L2CACHE		"l2cache"

/* ECM - 1 interrupt */
#define PPC85xx_HWI_ECM			"ecm"

/* DRAM - 1 interrupt */
#define PPC85xx_HWI_DRAM		"dram"

/* DMA - multiple interrupts are assigned per available channel for each device */
#define PPC85xx_HWI_DMA			"dma"

/* eTSEC - multiple interrupts are assigned in order of Tx, Rx, Error
 *
 * NOTE: we define PPC85xx_HWI_LEGACY_TSEC to be backwards compatible with
 * 		 previous 'devn' drivers which searched for the hardcoded string "tsec".
 * 		 Normally this legacy name would be added as a synonym and all would be
 * 		 well. However, the devn drivers not only search for the hardcoded name
 * 		 "tsec", they search using hwi_find_item() which does not resolve synonyms.
 * 		 Therefore, we need the primary name of the TSEC devices to be "tsec" and
 * 		 we will make an alias for the preferred name "eTSEC". In the future if
 * 		 we get the devn drivers moved over to using hwi_find_device() then either
 * 		 name will work.  
*/
#define PPC85xx_HWI_eTSEC		"tsec"	// "eTSEC"
#define PPC85xx_HWI_LEGACY_TSEC	"eTSEC"	// "tsec"

/* FEC (Fast Ethernet Controller) - 1 interrupt */
#define PPC85xx_HWI_FEC			"fec"

/* DUART - 1 interrupt */
#define	PPC85xx_HWI_UART		"uart"

/* PMON (performance monitor) - 1 interrupt */
#define PPC85xx_HWI_PMON		"pmon"

/* Security - 1 interrupt per device */
#define PPC85xx_HWI_SECURITY	"security"

/* GPIO - 1 interrupt per device */
#define PPC85xx_HWI_GPIO		"gpio"

/* SRIO - Error/Write Port device */
#define PPC85xx_HWI_SRIO_EWP	"srio_ewp"

/* SRIO - interrupts are assigned in order of Outbound, Inbound */
#define PPC85xx_HWI_SRIO_DOORBELL	"srio_dbell"
#define PPC85xx_HWI_SRIO_MSGUNIT	"srio_msgunit"

/* PME - multiple interrupts are assigned in order of general, chan1, chan2, etc */
#define PPC85xx_HWI_PME			"pme"

/* TLU - mutliple interrupts are assigned per available unit */
#define PPC85xx_HWI_TLU			"tlu"

/* IEEE588 TSEC - 1 interrupt per device */
#define PPC85xx_HWI_IEEE588_TSEC	"ieee588_tsec"

/* IPI - multiple interrupts are assigned in order */
#define PPC85xx_HWI_IPI				"ipi"

/* Global Timers - multiple interrupts are assigned per available timer for each group */
#define PPC85xx_HWI_GTIMER			"gtimer"

/* Message - mutliple interrupts are assigned in order */
#define PPC85xx_HWI_MSG				"msg"

/* Shared Message - mutliple interrupts are assigned in order */
#define PPC85xx_HWI_SMSG			"smsg"

/* External Interrupts - mutliple interrupts are assigned in order IRQ0 - IRQn */
#define PPC85xx_HWI_EXT_IRQ			"ext_irq"


#endif	/* __HWINFO_85xx_H_INCLUDED */

__SRCVERSION("hwinfo_85xx.h $Rev: 169799 $");

