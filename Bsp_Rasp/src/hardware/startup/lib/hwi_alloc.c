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

//
// Override __hwi*() definitions in libc
//
char *
__hwi_find_string(unsigned off) {
	char	*p = lsp.strings.p->data;
	return(p + off);
}

void *
__hwi_base() {
	return(lsp.hwinfo.p->data);
}


static unsigned offset = 0;
static unsigned last_item = 0;

static void
align_tag(unsigned align) {
	unsigned	grow;

	grow = ROUND(offset, align) - offset;
	if(grow != 0) {
		hwi_alloc_tag(HWI_TAG_INFO(pad));
	}
}

void *
hwi_alloc_tag(const char *name, unsigned size, unsigned align) {
	uint8_t		*p;
	hwi_tag		*tag;
	unsigned	name_off;

	align_tag(align);
	name_off = add_string(name);
	p = grow_syspage_section(&lsp.hwinfo, size);
	tag = (hwi_tag *)(p + last_item);
	tag->item.itemsize += size / 4;
	tag = (hwi_tag *)(p + offset);
	tag->prefix.size = size / 4;
	tag->prefix.name = name_off;
	offset += size;
	return(tag);
}

void *
hwi_alloc_item(const char *name, unsigned size, unsigned align,
				const char *itemname, unsigned owner) {
	hwi_tag		*tag;
	unsigned	saved_in;

	//Do add_string() up front so that the tag pointer doesn't move due
	//to shifting section sizes.
	saved_in = add_string(itemname);
	align_tag(align);
	last_item = offset;
	tag = hwi_alloc_tag(name, size, align);
	tag->item.itemname = saved_in;
	tag->item.owner = owner;
	if(owner != HWI_NULL_OFF) {
		hwi_tag		*owner_tag;

		owner_tag = hwi_off2tag(owner);
		owner_tag->item.kids++;
	}
	return(tag);
}

void *
hwi_grow_tag(unsigned offset, unsigned size) {
	uint8_t		*p;
	hwi_tag		*tag;

	p = grow_syspage_section(&lsp.hwinfo, size);
	tag = (hwi_tag *)(p + last_item);
	tag->item.itemsize += size / 4;
	tag = (hwi_tag *)(p + offset);
	tag->prefix.size += size / 4;
	offset += size;
	return(tag);
}

__SRCVERSION("hwi_alloc.c $Rev: 657836 $");
