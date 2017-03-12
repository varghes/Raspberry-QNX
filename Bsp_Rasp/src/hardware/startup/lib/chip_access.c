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

static union {
		uint8_t		*p;
		uintptr_t	port;
}				chip_base;
static unsigned	chip_shift;
static int		chip_mmap;

void
chip_access(paddr_t base, unsigned reg_shift, unsigned mem_mapped, unsigned size) {
	chip_shift = reg_shift;
	chip_mmap = mem_mapped;
	if(mem_mapped) {
		chip_base.p = startup_memory_map(size << reg_shift, base,
							PROT_READ|PROT_WRITE|PROT_NOCACHE);
	} else {
		chip_base.port = startup_io_map(size << reg_shift, base);
	}
}

void
chip_done() {
	if(chip_mmap) {
		startup_memory_unmap(chip_base.p);
	} else {
		startup_io_unmap(chip_base.port);
	}
}

unsigned
chip_read8(unsigned off) {
	unsigned	disp = (off << chip_shift);
	uint8_t		val;

	if(chip_mmap) {
		val = chip_base.p[disp];
	} else {
		val = in8(chip_base.port + disp);
	}
	return(val);
}

void
chip_write8(unsigned off, unsigned val) {
	unsigned	disp = (off << chip_shift);

	if(chip_mmap) {
		chip_base.p[disp] = val;
	} else {
		out8(chip_base.port + disp, val);
	}
}

unsigned
chip_read16(unsigned off) {
	unsigned	disp = (off << chip_shift);
	uint16_t	val;

	if(chip_mmap) {
		val = *((uint16_t*) (&chip_base.p[disp]));
	} else {
		val = in16(chip_base.port + disp);
	}
	return(val);
}

void
chip_write16(unsigned off, unsigned val) {
	unsigned	disp = (off << chip_shift);

	if(chip_mmap) {
		*((uint16_t*) (&chip_base.p[disp])) = val;
	} else {
		out16(chip_base.port + disp, val);
	}
}

unsigned
chip_read32(unsigned off) {
	unsigned	disp = (off << chip_shift);
	uint32_t	val;

	if(chip_mmap) {
		val = *((uint32_t*) (&chip_base.p[disp]));
	} else {
		val = in32(chip_base.port + disp);
	}
	return(val);
}

void
chip_write32(unsigned off, unsigned val) {
	unsigned	disp = (off << chip_shift);

	if(chip_mmap) {
		*((uint32_t*) (&chip_base.p[disp])) = val;
	} else {
		out32(chip_base.port + disp, val);
	}
}


__SRCVERSION("chip_access.c $Rev: 657836 $");
