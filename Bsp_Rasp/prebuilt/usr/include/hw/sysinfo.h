/*
 * $QNXLicenseC:
 * Copyright 2007, 2008, 2009, QNX Software Systems.
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

#ifndef __HWSYSINFO_H_INCLUDED

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

#include _NTO_HDR_(_pack64.h)

__BEGIN_DECLS

#define HWI_NULL_OFF			((_Uint16t)-1)
/* note that the HWI_ILLEGAL_VECTOR value is selected such that if it is passed
 * to InterruptAttach() (for example directly from a failed hwi_find_ivec() call),
 * it will fail with 'errno' == EINVAL */ 
#define HWI_ILLEGAL_VECTOR		((unsigned)-1)
#define MAX_SYNONYM_COUNT		10	// no particular reason for 10, just a choice

#define HWI_ITEM_ROOT_HW		"hw"

#define HWI_ITEM_BUS_PCI		"pci"
#define HWI_ITEM_BUS_ISA		"isa"
#define HWI_ITEM_BUS_EISA		"eisa"
#define HWI_ITEM_BUS_MCA		"mca"
#define HWI_ITEM_BUS_PCMCIA		"pcmcia"
#define HWI_ITEM_BUS_USB		"usb"
#define HWI_ITEM_BUS_I2C		"i2c"
#define HWI_ITEM_BUS_SPI		"spi"
#define HWI_ITEM_BUS_CAN		"can"
#define HWI_ITEM_BUS_SDIO		"sdio"
#define HWI_ITEM_BUS_I394		"I394"
#define HWI_ITEM_BUS_MOST		"most"
#define HWI_ITEM_BUS_BLUETOOTH		"bluetooth"
#define HWI_ITEM_BUS_UNKNOWN      	"unknown"
#define HWI_ITEM_BUS_INTERNAL     	"internal"

#define HWI_ITEM_DEVCLASS_SERIAL	"serial"
#define HWI_ITEM_DEVCLASS_PARALLEL	"parallel"
#define HWI_ITEM_DEVCLASS_DMA		"dma"
#define HWI_ITEM_DEVCLASS_RTC		"rtc"
#define HWI_ITEM_DEVCLASS_DISK		"disk"
#define HWI_ITEM_DEVCLASS_NETWORK	"network"
#define HWI_ITEM_DEVCLASS_AUDIO_CTRL	"audio_ctrl"
#define HWI_ITEM_DEVCLASS_AUDIO_MIXER	"audio_mixer"
#define HWI_ITEM_DEVCLASS_GRAPHICS	"graphics"
#define HWI_ITEM_DEVCLASS_INPUT		"input"
#define HWI_ITEM_DEVCLASS_NVRAM		"nvram"
#define HWI_ITEM_DEVCLASS_MISC		"misc"


struct hwi_prefix {
	_Uint16t			size;
	_Uint16t			name;
};

struct hwi_item {
	struct hwi_prefix	prefix;
	_Uint16t			itemsize;
	_Uint16t			itemname;
	_Uint16t			owner;
	_Uint16t			kids;
};

#define HWI_TAG_NAME_group	"Group"
#define HWI_TAG_ALIGN_group	(sizeof(_Uint32t))
struct hwi_group {
	struct hwi_item		item;
};

#define HWI_TAG_NAME_bus	"Bus"
#define HWI_TAG_ALIGN_bus	(sizeof(_Uint32t))
struct hwi_bus {
	struct hwi_item		item;
};


#define HWI_TAG_NAME_device		"Device"
#define HWI_TAG_ALIGN_device	(sizeof(_Uint32t))
struct hwi_device {
	struct hwi_item		item;
	_Uint32t			pnpid;
};

#define HWI_TAG_NAME_synonym	"Synonym"
#define HWI_TAG_ALIGN_synonym	(sizeof(_Uint32t))
struct hwi_synonym {
	struct hwi_item		item;
	_Uint32t			real;
};

#define HWI_TAG_NAME_location	"location"
#define HWI_TAG_ALIGN_location	(sizeof(_Uint64t))
struct hwi_location {
	struct hwi_prefix	prefix;
	_Uint32t			len;
	_Uint64t			base;
	_Uint16t			regshift;
	_Uint16t			addrspace;
};

#define HWI_TAG_NAME_irq		"irq"
#define HWI_TAG_ALIGN_irq		(sizeof(_Uint32t))
struct hwi_irq {
	struct hwi_prefix	prefix;
	_Uint32t			vector;
};

#define HWI_TAG_NAME_irqmapping		"irqmapping"
#define HWI_TAG_ALIGN_irqmapping	(sizeof(_Uint32t))
struct __hwi_specific_s {
	_Uint8t				devfunc;
	_Int8t				adjust;
};
struct hwi_irqmapping {
	struct hwi_prefix	prefix;
	_Uint32t			irq;
	_Uint16t			num;
	__FLEXARY(struct __hwi_specific_s, specific); /* struct __hwi_specific_s specific[] */
};

#define HWI_TAG_NAME_irqrange		"irqrange"
#define HWI_TAG_ALIGN_irqrange		(sizeof(_Uint32t))
struct hwi_irqrange {
	struct hwi_prefix	prefix;
	_Uint32t			irq;
	_Uint16t			num;
};

#define HWI_TAG_NAME_diskgeometry	"diskgeometry"
#define HWI_TAG_ALIGN_diskgeometry	(sizeof(_Uint32t))
struct hwi_diskgeometry {
	struct hwi_prefix	prefix;
	_Uint8t				disknumber;
	_Uint8t				sectorsize;	/* as a power of two */
	_Uint16t			heads;
	_Uint16t			cyls;
	_Uint16t			sectors;
	_Uint32t			nblocks;
};

#define HWI_TAG_NAME_regname	"regname"
#define HWI_TAG_ALIGN_regname	(sizeof(_Uint32t))
struct hwi_regname {
	struct hwi_prefix	prefix;
	_Uint16t			regname;
	_Uint16t			offset;
};

#define HWI_TAG_NAME_inputclk	"inputclk"
#define HWI_TAG_ALIGN_inputclk	(sizeof(_Uint32t))
struct hwi_inputclk {
	struct hwi_prefix	prefix;
	_Uint32t			clk;
	_Uint32t			div;
};

#define HWI_TAG_NAME_nicaddr	"nicaddr"
#define HWI_TAG_ALIGN_nicaddr	(sizeof(_Uint32t))
struct hwi_nicaddr {
	struct hwi_prefix	prefix;
	_Uint32t			len;
	__FLEXARY(_Uint8t, addr); /* _Uint8t addr[] */
};

#define HWI_TAG_NAME_nicphyaddr		"nicphyaddr"
#define HWI_TAG_ALIGN_nicphyaddr	(sizeof(_Uint32t))
struct hwi_nicphyaddr {
	struct hwi_prefix	prefix;
	_Uint32t			addr;
};

#define HWI_TAG_NAME_busattr		"busattr"
#define HWI_TAG_ALIGN_busattr		(sizeof(_Uint64t))
struct hwi_busattr {
	struct hwi_prefix	prefix;
	_Uint32t			flags;		/* see hwi_busattr_flags_e */
	_Uint64t			addr;		/* bus address */
	_Uint32t			speed;
	_Uint16t			width;		/* width of the bus */
	_Uint16t			spare;		/* pad out for size alignment */
	_Uint64t			size;		/* bus address size (intentionally at end of structure) */
};
/* some or all of the following may be applicable depending on the particular bus */
__extension__ typedef enum {
	hwi_busattr_MASTER		= (1U << 0),	/* otherwise slave */
	hwi_busattr_USE_ADDR	= (1U << 1),	/* addr field is valid (because 0 is a valid address) */
	
	/* the following apply to PCI only */
	hwi_busattr_PCI_memtype_mask = (7U << 28),	/* 3 bits for 8 possible types (see below) */
	hwi_busattr_isPCIe			 = (1U << 31)
	
	/* add as required */
} hwi_busattr_flags_e;

/* 3 currently defined PCI memory types (only applies to PCI buses */
#define hwi_busattr_PCI_memtype_NONE			(0U << 28)
#define hwi_busattr_PCI_memtype_IO				(1U << 28)
#define hwi_busattr_PCI_memtype_MEM				(2U << 28)
#define hwi_busattr_PCI_memtype_MEM_PREFETCH	(3U << 28)


#define HWI_TAG_NAME_optstr		"optstr"
#define HWI_TAG_ALIGN_optstr	(sizeof(_Uint32t))
struct hwi_optstr {
	struct hwi_prefix	prefix;
	_Uint32t			string;
};

#define HWI_TAG_NAME_dll		"drvr_dll"
#define HWI_TAG_ALIGN_dll		(sizeof(_Uint32t))
struct hwi_dll {
	struct hwi_prefix	prefix;
	_Uint32t			name;
};

#define HWI_TAG_NAME_errata		"errata"
#define HWI_TAG_ALIGN_errata	(sizeof(_Uint32t))
struct hwi_errata {
	struct hwi_prefix	prefix;
	_Uint32t			num;	/* errata number (as defined for the device in errata docs) */
};
#define HWI_TAG_NAME_nanospin     "nanospin"
#define HWI_TAG_ALIGN_nanospin    (sizeof(_Uint32t))

struct hwi_nanospin {
        struct hwi_prefix prefix;
        _Uint32t          hundred_loop_time;
        _Uint32t          overhead;
};

#define HWI_TAG_NAME_pad		"pad"
#define HWI_TAG_ALIGN_pad		(sizeof(_Uint32t))
struct hwi_pad {
	struct hwi_prefix	prefix;
};

typedef union {
	struct hwi_prefix			prefix;
	struct hwi_item				item;
	struct hwi_group			group;
	struct hwi_bus				bus;
	struct hwi_device			device;
	struct hwi_location 		location;
	struct hwi_irq		 		irq;
	struct hwi_irqmapping 		irqmapping;
	struct hwi_irqrange			irqrange;
	struct hwi_diskgeometry 	diskgeometry;
	struct hwi_regname			regname;
	struct hwi_inputclk			inputclk;
	struct hwi_nicaddr			nicaddr;
	struct hwi_nicphyaddr		nicphyaddr;
	struct hwi_pad				pad;
	struct hwi_synonym			synonym;
	struct hwi_busattr			busattr;
	struct hwi_dll				dll;
	struct hwi_optstr			optstr;
	struct hwi_errata			errata;
	struct hwi_nanospin			nanospin;
} hwi_tag;

void		*__hwi_base(void);
char		*__hwi_find_string(unsigned __off);
unsigned	hwi_find_tag(unsigned __start, int __curr_item, const char *__tagname);
unsigned 	hwi_find_item(unsigned __start, ...);
unsigned	hwi_next_tag(unsigned __off, int __curr_item);
unsigned	hwi_next_item(unsigned __off);
unsigned	hwi_tag2off(void *);
void	 	*hwi_off2tag(unsigned);

__END_DECLS

#include _NTO_HDR_(_packpop.h)

#define __HWSYSINFO_H_INCLUDED
#endif


__SRCVERSION( "$URL: http://svn/product/tags/internal/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/1.0.0/latest/services/system/public/hw/sysinfo.h $ $Rev: 510596 $" )
