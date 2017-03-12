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
#include "hwinfo_private.h"

static void hwitag_add_errata(unsigned hwi_off, unsigned errata_num);
static void hwitag_add_dll(unsigned hwi_off, const char *dll);
static void hwitag_add_optstr(unsigned hwi_off, const char *optstr);

#define ADD_IRQ_TAGS(h, n) \
		do { \
			unsigned i = (n); \
			while (i--) hwitag_add_irq((h), HWI_ILLEGAL_VECTOR); \
		} while(0)

#define ADD_ERRATA_TAGS(h, n) \
		do { \
			unsigned i = (n); \
			while (i--) hwitag_add_errata((h), 0); \
		} while(0)

/*******************************************************************************
* hwitag_add_common
* 
* add all of the common tags to device <hwi_off> and and set the tag fields as
* specified in <attr>.
*  
* Returns: -1 on error or the vector tag index that was set (base 0)
* 
*/ 
void hwitag_add_common(unsigned hwi_off, void *_attr)
{
	hwiattr_common_t *attr = (hwiattr_common_t *)_attr;

	hwitag_add_location(hwi_off, attr->location.base, attr->location.len, attr->location.regshift, attr->location.addrspace);
	ADD_IRQ_TAGS(hwi_off, attr->num_irq);
	if (attr->dll != NULL) hwitag_add_dll(hwi_off, attr->dll);
	if (attr->optstr != NULL) hwitag_add_optstr(hwi_off, attr->optstr);
	ADD_ERRATA_TAGS(hwi_off, attr->num_errata);
}


static void hwitag_add_errata(unsigned hwi_off, unsigned errata_num)
{
	hwi_tag		*tag;

	tag = hwi_alloc_tag(HWI_TAG_INFO(errata));
	tag->errata.num = errata_num;
}
static void hwitag_add_dll(unsigned hwi_off, const char *dll)
{
	hwi_tag		*tag;
	unsigned	string_idx = add_string(dll);

	tag = hwi_alloc_tag(HWI_TAG_INFO(dll));
	tag->dll.name = string_idx;
}
static void hwitag_add_optstr(unsigned hwi_off, const char *string)
{
	hwi_tag		*tag;
	unsigned	string_idx = add_string(string);

	tag = hwi_alloc_tag(HWI_TAG_INFO(optstr));
	tag->optstr.string = string_idx;
}


__SRCVERSION("$URL$ $Rev$");
