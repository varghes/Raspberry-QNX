/*
 * $QNXLicenseC:
 * Copyright 2007, QNX Software Systems. All Rights Reserved.
 * 
 * You must obtain a written license from and pay applicable license fees to QNX 
 * Software Systems before you may reproduce, modify or distribute this software, 
 * or any work that includes all or part of this software.   Free development 
 * licenses are available for evaluation and non-commercial purposes.  For more 
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *  
 * This file may contain contributions from others.  Please review this entire 
 * file for other proprietary rights or license notices, as well as the QNX 
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
 * for other information.
 * $
 */

#ifndef _DRVR_HWINFO_H_INCLUDED
#define _DRVR_HWINFO_H_INCLUDED

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif
#include <hw/sysinfo.h>		// needed for hwi_tag type
#include <sys/hwinfo.h>		// needed for the hwiattr_* types

/*
 * HWI support routines
 * 
 * Note that these functions were intentionally placed in libdrvr instead of
 * libc (where come companion functions are) so that we are not dependent on
 * libc release schedules
*/
unsigned	hwi_find(const char *device_name, int unit, unsigned hwi_off);
int			hwi_find_unit(unsigned hwi_off);
int			hwi_find_num_units(const char *name);
unsigned	hwi_find_bus(const char *bus_name, unsigned unit);
unsigned	hwi_find_device(const char *device_name, unsigned unit);
unsigned	hwi_find_devicebus(unsigned device_hwi_off);

hwi_tag *	hwi_tag_find(unsigned hwi_off, const char *tag_name, unsigned *tag_idx);
unsigned	hwitag_find_ivec(unsigned hwi_off, unsigned *irq);
int			hwitag_find_nicaddr(unsigned hwi_off, unsigned *nicaddr_idx, _Uint8t *nicaddr);
int			hwitag_find_phyaddr(unsigned hwi_off, unsigned *phy_idx);
long		hwitag_find_clkfreq(unsigned hwi_off, unsigned *clk_idx);
int			hwitag_find_errata(unsigned hwi_off, unsigned *errata_idx);
int			hwitag_find_busattr(unsigned hwi_off, unsigned *busattr_idx, struct hwi_busattr *busattr);

int			hwiattr_get(unsigned hwi_off, hwiattr_t *hwiattr);
/* bus attribute get functions */
int			hwiattr_get_spi(unsigned hwi_off, hwiattr_spi_t *hwiattr);
int			hwiattr_get_i2c(unsigned hwi_off, hwiattr_i2c_t *hwiattr);
int			hwiattr_get_pci(unsigned hwi_off, hwiattr_pci_t *hwiattr);
int			hwiattr_get_usb(unsigned hwi_off, hwiattr_usb_t *hwiattr);
int			hwiattr_get_can(unsigned hwi_off, hwiattr_can_t *hwiattr);
/* device attribute get functions */
int			hwiattr_get_uart(unsigned hwi_off, hwiattr_uart_t *hwiattr);
int			hwiattr_get_enet(unsigned hwi_off, hwiattr_enet_t *hwiattr);
int			hwiattr_get_rtc(unsigned hwi_off, hwiattr_rtc_t *hwiattr);
int			hwiattr_get_dma(unsigned hwi_off, hwiattr_dma_t *hwiattr);
int			hwiattr_get_timer(unsigned hwi_off, hwiattr_timer_t *hwiattr);

#endif	/* _DRVR_HWINFO_H_INCLUDED */

__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/trunk/lib/drvr/public/drvr/hwinfo.h $ $Rev: 218911 $" )
