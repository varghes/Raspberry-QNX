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

static char	*debug_opt[2];

static void
get_freq(unsigned long *fp) {
	unsigned long	freq;
	char			*end;

	freq = strtoul(optarg, &end, 10);
	if(optarg != end) {
		switch(*end) {
		case 'm': case 'M':
			freq *= 1000;
			/* fall through */
		case 'k': case 'K':
			freq *= 1000;
			/* fall through */
		case 'h': case 'H':
			++end;
			break;
		default:
			if(freq < 1000) freq *= 1000000;
			break;
		}
		*fp = freq;
		optarg = end;
	}
	if(*optarg == ',') ++optarg;
}

void
handle_common_option(int opt) {
	unsigned				num;
	char 					*cp;
	unsigned long				hundred_loop_time, overhead;
	
   	if(!cpu_handle_common_option(opt)) {
		switch(opt) {
		case 'D':
			/* kprintf output channel */
			debug_opt[0] = optarg;
			break;
		case 'f':
			/* cpu frequency, clock cycles frequency, timer chip frequency */
			get_freq(&cpu_freq);
			get_freq(&cycles_freq);
			get_freq(&timer_freq);
			break;
		case 'K':
			/* kdebug remote debug protocol channel */
			debug_opt[1] = optarg;
			break;
		case 'N':
			add_typed_string(_CS_HOSTNAME, optarg);
			break;
		case 'R':
			reserved_size = getsize(optarg, &cp);
			if(*cp == ',') {
				reserved_align = getsize(cp + 1, &cp);
			}
			break;
		case 'S':
			//This gets handled later so that debug code isn't brought in all
			//the time.
			break;
		case 'T':	
			misc_flags |= MISC_FLAG_SUPPRESS_BOOTTIME;
			break;
		case 'P':
			num = strtoul(optarg, &optarg, 10);
			if(num > 0) max_cpus = num;
			break;
		case 'v':
			debug_flag++;
			break;
		case 'F':	
		case 'A':
		case 'M':
		case 'r':
		case 'j':
		case 'Z':
			// All handled later, see init_system_private...
			break;
		case 'I':
			// Fastboot/Restore IFS
			rifs_flag |= RIFS_FLAG_ENABLE;

			num = strtoul(optarg, &optarg, 10);
			if(num) {
				rifs_flag |= RIFS_FLAG_CKSUM;
			}
			break;
		case 'i':
			// Fastboot/Secondary IFS
			rifs_flag |= RIFS_FLAG_IFS2;
			ifs2_size = strtoul(optarg, &optarg, 0);

			// Valid flags are: 'R', 'RK', or 'K' (equivalent to 'RK')
			if((cp = strchr(optarg, ',')) != NULL) {
				if(*(cp + 1) == 'R') {
					rifs_flag |= RIFS_FLAG_IFS2_RESTORE;
					cp++;
				}
				if(*(cp + 1) == 'K') {
					rifs_flag |= RIFS_FLAG_IFS2_RESTORE;
					rifs_flag |= RIFS_FLAG_IFS2_CKSUM;
					cp++;
				}
			}
			if((cp = strchr(cp, ',')) != NULL) {
				rifs_flag |= RIFS_FLAG_IFS2_SRC;
				ifs2_paddr_src = strtoul(cp + 1, &cp, 0);
			}
			if((cp = strchr(cp, ',')) != NULL) {
				rifs_flag |= RIFS_FLAG_IFS2_DST;
				// Get destination value and make sure it is at a 4K page boundary
				ifs2_paddr_dst = 0xFFFFF000 & strtoul(cp + 1, &cp, 0);
			}
			break;
		case 'o':
			hundred_loop_time = (unsigned long) strtoul(optarg, &cp, 0);
			if(*cp == ',') {
				overhead = (unsigned long) strtoul(cp + 1, &cp, 0);
				hwi_add_nanospin(hundred_loop_time, overhead);
			}
			break;
		}
	}
}

void
set_debug(unsigned channel, const struct debug_device *dev, const char *options) {
	unsigned	base;
	unsigned	i;

	dev->init(channel, options, dev->defaults[channel]);
	if(channel == 0) set_print_char(dev->put);
	base = offsetof(struct callout_entry, debug);
	base += channel * sizeof(struct debug_callout);
	for(i = 0; i < NUM_ELTS(dev->callouts); ++i) {
		const struct callout_rtn	*rtn = dev->callouts[i];

		if(rtn != NULL) {
			add_callout(base + i*sizeof(callout_fp_t), rtn);
		}
	}
}

static void
select_one(const struct debug_device *dev, unsigned size, unsigned channel) {
	unsigned					len;
	const struct debug_device	*selected;
	const struct debug_device	*end;
	char						*arg;

	end = (void *)((uint8_t *) dev + size);
	arg = debug_opt[channel];
	if(arg == NULL) {
		for( ;; ) {
			if(dev >= end) return;
			if(dev->defaults[channel] != NULL) break;
			++dev;
		}
		set_debug(channel, dev, "");
		return;
	}
	selected = NULL;
	for(;;) {
		const char	*defaults;

		if(dev >= end) break;
		defaults = dev->defaults[channel];
		if(defaults != NULL) {
			char	ch;

			len = strlen(dev->name);
			ch = arg[len];
			if((memcmp(arg, dev->name, len) == 0)
			 &&(ch==' ' || ch=='\t' || ch=='\0' || ch=='.')) {
				selected = dev;
				arg += len;
				if(*arg == '.') ++arg;
				break;
			}
			if((selected == NULL) && (defaults[0] != '\0')) {
				//If we don't match any of the names, the first device
				//in the list with actual options the default device.
				selected = dev;
			}
		}
		++dev;
	}
	if(selected != NULL) set_debug(channel, selected, arg);
}

void
select_debug(const struct debug_device *dev, unsigned size) {
	select_one(dev, size, 0);
	if(debug_opt[1] != NULL) select_one(dev, size, 1);
}

__SRCVERSION("common_options.c $Rev: 657836 $");
