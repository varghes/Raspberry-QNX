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

#ifndef _HWINFO_PRIVATE_H_
#define _HWINFO_PRIVATE_H_

#include <hw/sysinfo.h>
#include <sys/hwinfo.h>

/*
* This file contains the 'hwinfo' API's for common devices and their attributes.
* Processor architecture specific device API's are contained in a corresponding
* <cpu arch>/hwinfo.h file (ex. ppc/hwinfo.h) while processor/family specific
* device API's are contained in a hwinfo_<cpu>.h file (ex. hwinfo_85xx.h).
* (Note that the latter naming is used because we currently don't break down
* startup lib the files into 'cpu' specific directories, for example
* ppc/85xx/hwinfo.h although in the future we may want to) 
* 
* For each supported device, all of the common attribute tags are created as well
* a the tags specific to the device class. In order to remain portable and
* flexible without over complicating the interface, some of the created tags are
* not actually initialized to specific values. This is true for tags which may
* have multiple instances for the particular device but the number of instances
* would be device, board or cpu specific, for example the number of interrupt
* vectors for a given ethernet device.
* 
* To accomodate this, instead of trying to implement a variable sized data
* structure which passes in all of the attribute values for all of the instances
* of a particular attribute tag, instead the interface allows the caller to
* specify for such attributes the number of tags to be created. The caller can
* then initialize each of those tag instances with device, board or cpu specific
* and/or calculated values after the entry is created. In this way, we also only
* create the number of attribute tags we need for a given board.
* 
* Implementation Note
* 
* Note that this implementation (ie. placeholder tags) is a result of the fact
* that when tags are scanned starting from a specific device offset, it is
* assumed that the tags belonging to that device, follow that device. It's a bit
* clumsy but that's the way it was originally implemented. There is no way to
* validate that a tag belongs to a specific device (ie. item) since there is no
* owner field for tags and therefore the assumption is that the tags belonging
* to a device follow it and this is why all tags must be created at the time
* the device is created.
* 
*/

/*
 * hwiattr_common_t
*/
#define HWIATTR_COMMON_INITIALIZER \
		{ \
			.location = {.prefix = {0,0}, .base = 0, .len = 0, .regshift = 0, .addrspace = 0}, \
			.num_irq = 0, .num_errata = 0, .dll = NULL, .optstr = NULL \
		}

#define HWIATTR_SET_LOCATION(__hwitag_p__, __base__, __len__, __rs__, __as__) \
		do { \
			(__hwitag_p__)->location.base = (__base__); \
			(__hwitag_p__)->location.len = (__len__); \
			(__hwitag_p__)->location.regshift = (__rs__); \
			(__hwitag_p__)->location.addrspace = (__as__); \
		} while(0)
			
#define HWIATTR_SET_NUM_IRQ(__hwitag_p__, __num__) \
		do { \
			(__hwitag_p__)->num_irq = (__num__); \
		} while(0)

#define HWIATTR_SET_NUM_ERRATA(__hwitag_p__, __num__) \
		do { \
			(__hwitag_p__)->num_errata = (__num__); \
		} while(0)

#define HWIATTR_SET_DLL(__hwitag_p__, __dll__) \
		do { \
			(__hwitag_p__)->dll = (__dll__); \
		} while(0)

#define HWIATTR_SET_OPTSTR(__hwitag_p__, __os__) \
		do { \
			(__hwitag_p__)->optstr = (__os__); \
		} while(0)


/*
 * =============================================================================
 *                               D E V I C E S
 * =============================================================================
*/

/*******************************************************************************
* hwi_add_uart
* 
* Adds an 'hwinfo' entry for a UART device.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 	- input clock (1 per possible source)
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions. This includes the input clock information for each clock source
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
#define HWIATTR_UART_T_INITIALIZER \
		{ \
			.common = HWIATTR_COMMON_INITIALIZER, \
			.num_clks = 0 \
		}

#define HWIATTR_UART_SET_NUM_CLK(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_clks = (__num__); \
	} while(0)

#define HWIATTR_UART_SET_LOCATION(__hwitag_p__, ...)	HWIATTR_SET_LOCATION(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_UART_SET_NUM_IRQ(__hwitag_p__, ...)		HWIATTR_SET_NUM_IRQ(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_UART_SET_NUM_ERRATA(__hwitag_p__, ...)	HWIATTR_SET_NUM_ERRATA(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_UART_SET_DLL(__hwitag_p__, ...)			HWIATTR_SET_DLL(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_UART_SET_OPTSTR(__hwitag_p__, ...)		HWIATTR_SET_OPTSTR(&(__hwitag_p__)->common, __VA_ARGS__)

extern unsigned hwidev_add_uart(const char *name, hwiattr_uart_t *attr, unsigned bus_hwi_off);


/*******************************************************************************
* hwidev_add_enet
* 
* Adds an 'hwinfo' entry for a ethernet device.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 	- mac address
* 	- phy address (1 per phy)
* 	- input clock (1 per possible source)
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions.
* 
* Note that when the <num_phys> PHY attributes for the ethernet device are
* created, addresses are assigned to the PHY's in sequential order starting at
* 0. The board configuration is responsible for ensuring PHY addresses are 
* correct for the given NIC 
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
#define HWIATTR_ENET_T_INITIALIZER \
		{ \
			.common = HWIATTR_COMMON_INITIALIZER, \
			.num_clks = 0, .num_phys = 0, \
			.mac = {0, 0, 0, 0, 0, 0} \
		}

#define HWIATTR_ENET_SET_NUM_CLK(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_clks = (__num__); \
	} while(0)

#define HWIATTR_ENET_SET_NUM_PHY(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_phys = (__num__); \
	} while(0)

#define HWIATTR_ENET_SET_MAC(__hwitag_p__, __mac__) \
	do { \
		memcpy((__hwitag_p__)->mac, (__mac__), sizeof((__hwitag_p__)->mac)); \
	} while(0)

#define HWIATTR_ENET_SET_LOCATION(__hwitag_p__, ...)	HWIATTR_SET_LOCATION(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_ENET_SET_NUM_IRQ(__hwitag_p__, ...)		HWIATTR_SET_NUM_IRQ(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_ENET_SET_NUM_ERRATA(__hwitag_p__, ...)	HWIATTR_SET_NUM_ERRATA(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_ENET_SET_DLL(__hwitag_p__, ...)			HWIATTR_SET_DLL(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_ENET_SET_OPTSTR(__hwitag_p__, ...)		HWIATTR_SET_OPTSTR(&(__hwitag_p__)->common, __VA_ARGS__)

extern unsigned hwidev_add_enet(const char *name, hwiattr_enet_t *attr, unsigned bus_hwi_off);


/*******************************************************************************
* hwidev_add_rtc
* 
* Adds an 'hwinfo' entry for an RTC device.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 	- optional century register information
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
#define HWIATTR_RTC_T_INITIALIZER \
		{ \
			.common = HWIATTR_COMMON_INITIALIZER, \
			.centreg = {.name = NULL, .offset = 0} \
		}

#define HWIATTR_RTC_SET_CENTURY_REG(__hwitag_p__, __name__, __roff__) \
	do { \
		(__hwitag_p__)->centreg.name = (__name__); \
		(__hwitag_p__)->centreg.offset = (__roff__); \
	} while(0)

#define HWIATTR_RTC_SET_LOCATION(__hwitag_p__, ...)		HWIATTR_SET_LOCATION(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_RTC_SET_NUM_IRQ(__hwitag_p__, ...)		HWIATTR_SET_NUM_IRQ(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_RTC_SET_NUM_ERRATA(__hwitag_p__, ...)	HWIATTR_SET_NUM_ERRATA(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_RTC_SET_DLL(__hwitag_p__, ...)			HWIATTR_SET_DLL(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_RTC_SET_OPTSTR(__hwitag_p__, ...)		HWIATTR_SET_OPTSTR(&(__hwitag_p__)->common, __VA_ARGS__)

extern unsigned hwidev_add_rtc(const char *name, hwiattr_rtc_t *attr, unsigned bus_hwi_off);


/*******************************************************************************
* hwidev_add_timer
* 
* Adds an 'hwinfo' entry for a timer device.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 	- clock source
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
#define HWIATTR_TIMER_T_INITIALIZER \
		{ \
			.common = HWIATTR_COMMON_INITIALIZER, \
			.num_clks = 0, \
		}

#define HWIATTR_TIMER_SET_NUM_CLK(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_clks = (__num__); \
	} while(0)

#define HWIATTR_TIMER_SET_LOCATION(__hwitag_p__, ...)		HWIATTR_SET_LOCATION(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_TIMER_SET_NUM_IRQ(__hwitag_p__, ...)		HWIATTR_SET_NUM_IRQ(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_TIMER_SET_NUM_ERRATA(__hwitag_p__, ...)		HWIATTR_SET_NUM_ERRATA(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_TIMER_SET_DLL(__hwitag_p__, ...)			HWIATTR_SET_DLL(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_TIMER_SET_OPTSTR(__hwitag_p__, ...)			HWIATTR_SET_OPTSTR(&(__hwitag_p__)->common, __VA_ARGS__)

extern unsigned hwidev_add_timer(const char *name, hwiattr_timer_t *attr, unsigned bus_hwi_off);


/*******************************************************************************
* hwi_add_dma
* 
* Adds an 'hwinfo' entry for a DMA controller.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions.
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
#define HWIATTR_DMA_T_INITIALIZER \
		{ \
			.common = HWIATTR_COMMON_INITIALIZER, \
		}

#define HWIATTR_DMA_SET_LOCATION(__hwitag_p__, ...)		HWIATTR_SET_LOCATION(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_DMA_SET_NUM_IRQ(__hwitag_p__, ...)		HWIATTR_SET_NUM_IRQ(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_DMA_SET_NUM_ERRATA(__hwitag_p__, ...)	HWIATTR_SET_NUM_ERRATA(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_DMA_SET_DLL(__hwitag_p__, ...)			HWIATTR_SET_DLL(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_DMA_SET_OPTSTR(__hwitag_p__, ...)		HWIATTR_SET_OPTSTR(&(__hwitag_p__)->common, __VA_ARGS__)

extern unsigned hwidev_add_dma(const char *name, hwiattr_dma_t *attr, unsigned bus_hwi_off);


/*
 * =============================================================================
 *                                 B U S E S
 * =============================================================================
*/

/*******************************************************************************
* hwibus_add_i2c
* 
* Adds an 'hwinfo' entry for I2C devices.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 	- input clock (1 per possible source)
* 	- bus attributes (1 per possible supported speed/address/etc)
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions. This includes the input clock information for each clock source
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
#define HWIATTR_I2C_T_INITIALIZER \
		{ \
			.common = HWIATTR_COMMON_INITIALIZER, \
			.num_clks = 0, .num_busattr = 0, \
		}

#define HWIATTR_I2C_SET_NUM_CLK(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_clks = (__num__); \
	} while(0)

#define HWIATTR_I2C_SET_NUM_BUSATTR(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_busattr = (__num__); \
	} while(0)

#define HWIATTR_I2C_SET_LOCATION(__hwitag_p__, ...)		HWIATTR_SET_LOCATION(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_I2C_SET_NUM_IRQ(__hwitag_p__, ...)		HWIATTR_SET_NUM_IRQ(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_I2C_SET_NUM_ERRATA(__hwitag_p__, ...)	HWIATTR_SET_NUM_ERRATA(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_I2C_SET_DLL(__hwitag_p__, ...)			HWIATTR_SET_DLL(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_I2C_SET_OPTSTR(__hwitag_p__, ...)		HWIATTR_SET_OPTSTR(&(__hwitag_p__)->common, __VA_ARGS__)

extern unsigned hwibus_add_i2c(unsigned parent_hwi_off, hwiattr_i2c_t *attr);


/*******************************************************************************
* hwibus_add_spi
* 
* Adds an 'hwinfo' entry for SPI devices.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 	- input clock (1 per possible source)
* 	- bus attributes (1 per possible supported speed/address/etc)
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions. This includes the input clock information for each clock source and
* bus attributes
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
#define HWIATTR_SPI_T_INITIALIZER \
		{ \
			.common = HWIATTR_COMMON_INITIALIZER, \
			.num_clks = 0, .num_busattr = 0, \
		}

#define HWIATTR_SPI_SET_NUM_CLK(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_clks = (__num__); \
	} while(0)

#define HWIATTR_SPI_SET_NUM_BUSATTR(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_busattr = (__num__); \
	} while(0)

#define HWIATTR_SPI_SET_LOCATION(__hwitag_p__, ...)		HWIATTR_SET_LOCATION(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_SPI_SET_NUM_IRQ(__hwitag_p__, ...)		HWIATTR_SET_NUM_IRQ(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_SPI_SET_NUM_ERRATA(__hwitag_p__, ...)	HWIATTR_SET_NUM_ERRATA(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_SPI_SET_DLL(__hwitag_p__, ...)			HWIATTR_SET_DLL(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_SPI_SET_OPTSTR(__hwitag_p__, ...)		HWIATTR_SET_OPTSTR(&(__hwitag_p__)->common, __VA_ARGS__)

extern unsigned hwibus_add_spi(unsigned parent_hwi_off, hwiattr_spi_t *attr);


/*******************************************************************************
* hwibus_add_usb
* 
* Adds an 'hwinfo' entry for USB devices.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 	- input clock (1 per possible source)
* 	- bus attributes (1 per possible supported speed/address/etc)
* 	- dma attributes ???
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions.
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
#define HWIATTR_USB_T_INITIALIZER \
		{ \
			.common = HWIATTR_COMMON_INITIALIZER, \
		}

#define HWIATTR_USB_SET_LOCATION(__hwitag_p__, ...)		HWIATTR_SET_LOCATION(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_USB_SET_NUM_IRQ(__hwitag_p__, ...)		HWIATTR_SET_NUM_IRQ(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_USB_SET_NUM_ERRATA(__hwitag_p__, ...)	HWIATTR_SET_NUM_ERRATA(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_USB_SET_DLL(__hwitag_p__, ...)			HWIATTR_SET_DLL(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_USB_SET_OPTSTR(__hwitag_p__, ...)		HWIATTR_SET_OPTSTR(&(__hwitag_p__)->common, __VA_ARGS__)

extern unsigned hwibus_add_usb(unsigned parent_hwi_off, hwiattr_usb_t *attr);


/*******************************************************************************
* hwibus_add_pci
* 
* Adds an 'hwinfo' entry for PCI/PCIe devices.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 	- FIX ME - what else ?? bus attributes ?
* 				pex controller ?
* 				location tag for pci window ?
* 				number of PCI buses to scan ?
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions.
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
#define HWIATTR_PCI_T_INITIALIZER \
		{ \
			.common = HWIATTR_COMMON_INITIALIZER, \
			.num_irqblks = 0, .num_busattr = 0, \
		}

#define HWIATTR_PCI_SET_NUM_IRQBLKS(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_irqblks = (__num__); \
	} while(0)

#define HWIATTR_PCI_SET_NUM_BUSATTR(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_busattr = (__num__); \
	} while(0)

#define HWIATTR_PCI_SET_LOCATION(__hwitag_p__, ...)		HWIATTR_SET_LOCATION(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_PCI_SET_NUM_IRQ(__hwitag_p__, ...)		HWIATTR_SET_NUM_IRQ(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_PCI_SET_NUM_ERRATA(__hwitag_p__, ...)	HWIATTR_SET_NUM_ERRATA(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_PCI_SET_DLL(__hwitag_p__, ...)			HWIATTR_SET_DLL(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_PCI_SET_OPTSTR(__hwitag_p__, ...)		HWIATTR_SET_OPTSTR(&(__hwitag_p__)->common, __VA_ARGS__)

extern unsigned hwibus_add_pci(unsigned parent_hwi_off, hwiattr_pci_t *attr);


/*******************************************************************************
* hwibus_add_can
* 
* Adds an 'hwinfo' entry for CAN devices.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 	- input clock (1 per possible source)
* 	- memory address atribute (using location tag)
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions. This includes the input clock information for each clock source
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
#define HWIATTR_CAN_T_INITIALIZER \
		{ \
			.common = HWIATTR_COMMON_INITIALIZER, \
			.num_clks = 0, .num_memaddr = 0, \
		}

#define HWIATTR_CAN_SET_NUM_CLK(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_clks = (__num__); \
	} while(0)

#define HWIATTR_CAN_SET_NUM_MEMADDR(__hwitag_p__, __num__) \
	do { \
		(__hwitag_p__)->num_memaddr = (__num__); \
	} while(0)

#define HWIATTR_CAN_SET_LOCATION(__hwitag_p__, ...)		HWIATTR_SET_LOCATION(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_CAN_SET_NUM_IRQ(__hwitag_p__, ...)		HWIATTR_SET_NUM_IRQ(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_CAN_SET_NUM_ERRATA(__hwitag_p__, ...)	HWIATTR_SET_NUM_ERRATA(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_CAN_SET_DLL(__hwitag_p__, ...)			HWIATTR_SET_DLL(&(__hwitag_p__)->common, __VA_ARGS__)
#define HWIATTR_CAN_SET_OPTSTR(__hwitag_p__, ...)		HWIATTR_SET_OPTSTR(&(__hwitag_p__)->common, __VA_ARGS__)

extern unsigned hwibus_add_can(unsigned parent_hwi_off, hwiattr_can_t *attr);



#endif	/* _HWINFO_PRIVATE_H_ */
