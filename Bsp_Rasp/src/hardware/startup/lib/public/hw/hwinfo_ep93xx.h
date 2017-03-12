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
* Cirrus Logic EP93xx SOC devices
*
* This file contains names for all of the devices that may exists on any EP93xx
* processor variant. Specifically, these are the internal devices that are part
* of the EP93xx SOC and not board specific devices. Board specific devices
* are defined in a similarly named board specific file
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

#ifndef __HWINFO_EP93xx_H_INCLUDED
#define __HWINFO_EP93xx_H_INCLUDED

#include <hw/sysinfo.h>

/*
 * =============================================================================
 * 
 *                                B U S E S
 * 
 * =============================================================================
*/
#define EP93xx_HWI_AHB			"ahb"		// high speed host bus
#define EP93xx_HWI_APB			"apb"		// peripheral bus (bridged from AHB)

/* USB */
#define EP93xx_HWI_USB			HWI_ITEM_BUS_USB

/* SPI */
#define EP93xx_HWI_SPI			HWI_ITEM_BUS_SPI

/* I2S - id this a bus ? */
#define EP93xx_HWI_I2S			"i2s"

/*
 * =============================================================================
 * 
 *                              D E V I C E S
 * 
 * =============================================================================
*/

/*--- devices attached to the AHB ---*/

/* SDRAM */
#define EP93xx_HWI_SDRAM	"sdram"

/* SRAM (Flash/Rom) */
#define EP93xx_HWI_SRAM		"sram"

/* DMA - multiple interrupts are assigned per available channel for each device */
#define EP93xx_HWI_DMA		"dma"

/* Ethernet Controller */
#define EP93xx_HWI_ENET		"enet"

/* IDE Controller */
#define EP93xx_HWI_IDE		"ide"

/*--- devices attached to the APB ---*/

/* UART - 2 devices */
#define	EP93xx_HWI_UART		"uart"

/* KEYPAD */
#define	EP93xx_HWI_KEYPAD	"keypad"

/* TOUCHSCREEN */
#define	EP93xx_HWI_TOUCH	"touchscreen"

/* IrDA */
#define	EP93xx_HWI_IrDA		"irda"

/* GPIO - 1 interrupt per device */
#define EP93xx_HWI_GPIO		"gpio"

/* AC97 Audio - 1 device */
#define	EP93xx_HWI_AUDIO	"audio_ctrl"

/* RTC - 1 device (3 interrupts - RTC match, 64KHz and 1 KHz) */
#define	EP93xx_HWI_RTC		"rtc"

/* Timers - 3 devices (onlt 2 usable as 1 dedicated to qtimer) */
#define	EP93xx_HWI_TIMER	"timer"

/* Watchdog */
#define	EP93xx_HWI_WDOG		"wdog"

/* PWM (no interrupts) */
#define EP93xx_HWI_PWM		"pwm"

/* RASTER graphics device (3 interrupts - INT_PROG, V_SYNC, INT_VIDEO_FIFO) */
#define EP93xx_HWI_RASTER	"raster"

/* add pseudo device for INT_DSP */
#define EP93xx_HWI_CORE		"core"

/* add pseudo device for 3 external interrupt pins */
#define EP93xx_HWI_EXT_IRQ	"extirq"

/* SSP (can be configured for SPI, etc, so this name will provide an aka) */
#define EP93xx_HWI_SSP		"ssp"

/* I2S */
#define EP93xx_HWI_I2S		"i2s"


#endif	/* __HWINFO_EP93xx_H_INCLUDED */

__SRCVERSION("hwinfo_ep93xx.h $Rev: 169799 $");

