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

static inline unsigned
arm_v7_ccsidr(int level, int icache)
{
	unsigned	ccsidr;

	// Set CSSELR for this level and cache type
	__asm__ __volatile__("mcr	p15, 2, %0, c0, c0, 0" : : "r"((level << 1) | icache));

	// Get CSSIDR value
	__asm__ __volatile__("mrc	p15, 1, %0, c0, c0, 0" : "=r"(ccsidr));
	return ccsidr;
}

static inline void
arm_v7_cache_size(unsigned cssidr, unsigned *nline, unsigned *lsize)
{
	unsigned	nsets = ((cssidr >> 13) & 0x7fff) + 1;
	unsigned	assoc = ((cssidr >> 3) & 0x3ff) + 1;

	*nline = nsets * assoc;
	*lsize = 1 << ((cssidr & 7) + 4);
}

void
arm_v7_add_cache(unsigned cpunum, struct cpuinfo_entry *cpu, const struct armv_cache *cache)
{
	unsigned	clidr;
	int			i;

	/*
	 * Read CLIDR to figure out how many cache levels are implemented
	 */
	__asm__ __volatile__("mrc	p15, 1, %0, c0, c0, 1" : "=r"(clidr));

	for (i = 0; i < 8; i++, clidr >>= 3) {
		unsigned	type = clidr & 7;
		unsigned	ccsidr;
		unsigned	nline = 0;
		unsigned	lsize = 0;
		unsigned	flags;

		if (type == 0) {
			break;
		}
		if (type > 4) {
			crash("CLIDR reports reserved value");
		}

		flags = 0;
		if (type & 1) {
			ccsidr = arm_v7_ccsidr(i, 1);
			arm_v7_cache_size(ccsidr, &nline, &lsize);

			if (debug_flag) {
				kprintf("CPU%d: L%d Icache: %dx%d\n", cpunum, i+1, nline, lsize);
			}

			/*
			 * FIXME: do we need to check CTR to set CACHE_FLAG_VIRTUAL?
			 */
			flags = CACHE_FLAG_INSTR;

			/*
			 * CP15 cache operations work on all levels so only call add_cache()
			 * for L1 caches.
			 */
			if (i == 0) {
				cpu->ins_cache = add_cache(cpu->ins_cache,
											flags,
											lsize,
											nline,
											cache->icache_rtn);
			}
		}
		if ((type & 2) || type == 4) {
			ccsidr = arm_v7_ccsidr(i, 0);
			arm_v7_cache_size(ccsidr, &nline, &lsize);

			flags = (type == 4) ? CACHE_FLAG_UNIFIED : CACHE_FLAG_DATA;
			if (ccsidr & (1 << 30)) {
				flags |= CACHE_FLAG_WRITEBACK;
			}
			if (debug_flag) {
				kprintf("CPU%d: L%d Dcache: %dx%d %s\n", cpunum, i+1, nline, lsize, (flags & CACHE_FLAG_WRITEBACK) ? "WB" : "WT");
			}
			/*
			 * CP15 cache operations work on all levels so only call add_cache()
			 * for L1 caches.
			 */
			if (i == 0) {
				cpu->data_cache = add_cache(cpu->data_cache,
											flags,
											lsize,
											nline,
											cache->dcache_rtn);
			}
		}
	}
}

/*
 * Parse the CP15 cache type register
 */
static void
arm_cache_size(unsigned csize, unsigned *nline, unsigned *lsize)
{
	unsigned	S = (csize >> 6) & 15;
	unsigned	A = (csize >> 3) & 7;
	unsigned	M = (csize >> 2) & 1;
	unsigned	L = (csize & 3) + 3;

	/*
	 * Check if cache is absent
	 */
	if (A == 0 && M == 1) {
		*nline = 0;
		*lsize = 0;
	}
	else {
		*nline = (M+2) << (S + 8 - L);
		*lsize = 1 << L;
	}
}

void
arm_add_cache(struct cpuinfo_entry *cpu, const struct armv_cache *cache)
{
	unsigned	ctype;
	unsigned	nline;
	unsigned	lsize;
	unsigned	flags;
	unsigned	cpunum = cpu - lsp.cpuinfo.p;

	/*
	 * Use hardcoded values if there is no cache type register
	 */
	if (cache->dcache_config || cache->icache_config) {
		const struct arm_cache_config	*config;
		config=cache->dcache_config;
		if (config != 0) {
			cpu->data_cache = add_cache(cpu->data_cache,
										config->flags,
										config->line_size,
										config->num_lines,
										cache->dcache_rtn);
			if (debug_flag) {
				kprintf("CPU%d: %s: %dx%d %s\n", cpunum, (config->flags & CACHE_FLAG_INSTR) ? "Cache" : "Dcache", config->num_lines, config->line_size, (config->flags & CACHE_FLAG_WRITEBACK) ? "WB" : "WT");
			}
		}
		config=cache->icache_config;	
		if (config !=0) {
			cpu->ins_cache = add_cache(cpu->ins_cache,
										config->flags,
										config->line_size,
										config->num_lines,
										cache->icache_rtn);
			if (debug_flag) {
				kprintf("CPU%d: Icache: %dx%d\n", cpunum, config->num_lines, config->line_size);
			}
		}
		return;
	}


	/*
	 * Read CP15 cache type register
	 */
	ctype = arm_mmu_cache_type();

	if (ctype & (1 << 31)) {
		/*
		 * ARMv7 cache handling is different
		 */
		arm_v7_add_cache(cpunum, cpu, cache);
		return;
	}
	
	/*
	 * Get Data/Unified cache size
	 */
	arm_cache_size(ctype >> 12, &nline, &lsize);

#ifndef	FIXME
	if (nline == 0)
		crash("No Dcache in cache type register?");
#endif

	/*
	 * FIXME: currently all ARM caches are virtual - this may change
	 */
	flags = CACHE_FLAG_VIRTUAL;
	if (((ctype >> 25) & 15) != 0)
		flags |= CACHE_FLAG_WRITEBACK;

	if ((ctype & (1 << 24)) == 0) {
		flags |= CACHE_FLAG_UNIFIED;
	}
	else {
		flags |= CACHE_FLAG_DATA;
	}
	if (debug_flag) {
		kprintf("CPU%d: %s: %dx%d %s\n", cpunum, (flags & CACHE_FLAG_INSTR) ? "Cache" : "Dcache", nline, lsize, (flags & CACHE_FLAG_WRITEBACK) ? "WB" : "WT");
	}

	cpu->data_cache = add_cache(cpu->data_cache, flags, lsize, nline, cache->dcache_rtn);

	if ((flags & CACHE_FLAG_INSTR) == 0) {
		/*
		 * Get Instruction cache size
		 */
		arm_cache_size(ctype, &nline, &lsize);

#ifndef	FIXME
		if (nline == 0)
			crash("No Icache in cache type register?");
#endif

		if (debug_flag) {
			kprintf("CPU%d: Icache: %dx%d\n", cpunum, nline, lsize);
		}

		/*
		 * FIXME: currently all ARM caches are virtual - this may change
		 */
		flags = CACHE_FLAG_VIRTUAL | CACHE_FLAG_INSTR;

		cpu->ins_cache = add_cache(cpu->ins_cache, flags, lsize, nline, cache->icache_rtn);
	}
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/arm_cache.c $ $Rev: 657836 $" );
