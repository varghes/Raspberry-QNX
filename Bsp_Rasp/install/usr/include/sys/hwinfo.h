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

#ifndef _HWINFO_H_
#define _HWINFO_H_

#include <hw/sysinfo.h>

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
typedef struct
{
	struct hwi_location		location;	// just use the sysinfo.h structure
	const char *			dll;		// specific device driver 'DLL' to load (NULL means none)
	const char *			optstr;		// command line option string for driver (NULL means none)
	/*
	 * note that for the following attributes, only the number of tags specified will
	 * be created. It is the responsibility of the caller to set the tags to some useful
	 * value for the driver
	*/
	unsigned				num_irq;	// number of 'irq' tags to create
	unsigned				num_errata;	// number of 'errata' tags to create
} hwiattr_common_t;


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
typedef struct
{
	hwiattr_common_t	common;
	unsigned			num_clks;
} hwiattr_uart_t;


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
* Also, if the mac[] field is all zeros, the NIC address attribute tag WILL NOT
* be added. This is the default (ie. no tag unless the BSP specifies a non zero
* mac[] value). This is desireable so that if the BSP cannot fill in a sensible
* value (from an EEPROM or some other source), the NIC address tag will not
* added and hence not found by the driver. The driver will then resort to its
* own scheme to set a NIC address, including the use of any command line
* arguments.
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
typedef struct
{
	hwiattr_common_t	common;
	uint8_t				mac[6];
	unsigned			num_clks;
	unsigned			num_phys;
} hwiattr_enet_t;


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
typedef struct
{
	hwiattr_common_t	common;
	struct {
		const char *	name;	// if NULL, centreg' will be ignored
		uint16_t		offset;	// register offset
	} centreg;
} hwiattr_rtc_t;


/*******************************************************************************
* hwidev_add_timer
* 
* Adds an 'hwinfo' entry for a timer device.
* Added attribute tags are as follows...
* 
* 	- common attributes
* 	- number of clock sources
* 
* Clock sources are selected by the "driver" itself. The clock source values
* are listed in numerical order such that a CLKSEL field of 2 bits for example
* would have 4 clksrc entries and the clk values for each entry (ie instance)
* would correspond directly to the CLKSEL values 0 thru 3 respectively 
* 
* Unset tag fields should be set in the BSP init_hwinfo() and/or init_intrinfo()
* functions
* 
* Returns: the offset in the hwinfo section of the added device or HWI_NULL_OFF
* 			on error
*/
typedef struct
{
	hwiattr_common_t	common;
	unsigned			num_clks;
} hwiattr_timer_t;


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
typedef struct
{
	hwiattr_common_t	common;
	// FIX ME - what else
} hwiattr_dma_t;


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
typedef struct
{
	hwiattr_common_t	common;
	unsigned			num_clks;
	unsigned			num_busattr;
} hwiattr_i2c_t;


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
typedef struct
{
	hwiattr_common_t	common;
	unsigned			num_clks;
	unsigned			num_busattr;
} hwiattr_spi_t;


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
typedef struct
{
	hwiattr_common_t	common;
	// FIX ME: -c option to io-usb allows a configuration to be specified.
	// 			What is this and does it need a tag?

	// FIX ME - what other common attributes are there? Some devu-xxxx implementations
	//			have the number of endpoint descriptors. Should this be a tag?
	//			What about DMA?
} hwiattr_usb_t;


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
typedef struct
{
	hwiattr_common_t	common;
	unsigned			num_irqblks;	// number of irqrange blocks
	unsigned			num_busattr;
} hwiattr_pci_t;


/*******************************************************************************
* hwibus_add_can
* 
* Adds an 'hwinfo' entry for CAN buses.
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
typedef struct
{
	hwiattr_common_t	common;
	unsigned			num_clks;
	unsigned			num_memaddr;
} hwiattr_can_t;



typedef enum
{
hwi_devclass_first = 1,

	hwi_devclass_NONE = 0,
	hwi_devclass_UART = hwi_devclass_first,
	hwi_devclass_ENET,
	hwi_devclass_RTC,
	hwi_devclass_DMA,
	hwi_devclass_DISK,
	hwi_devclass_GRAPHICS,
	hwi_devclass_TIMER,

hwi_devclass_last
} hwi_devclass_e;

typedef union
{
	hwiattr_common_t	common;
	/* devices */
	hwiattr_uart_t		uart;
	hwiattr_enet_t		enet;
	hwiattr_rtc_t		rtc;
	hwiattr_dma_t		dma;
	/* buses */
	hwiattr_i2c_t		i2c;
	hwiattr_spi_t		spi;
	hwiattr_usb_t		usb;
	hwiattr_pci_t		pci;
} hwiattr_t;


#endif	/* _HWINFO_H_ */
