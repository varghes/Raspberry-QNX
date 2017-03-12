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

/* QNX raspberry Pi BCM2835
 * by Varghese Alphonse
 */



#include "startup.h"

unsigned					paddr_bits = 32;
int							debug_flag = 0;
unsigned					reserved_size;
unsigned					reserved_align;
unsigned long				cpu_freq;
unsigned long				cycles_freq;
unsigned long				timer_freq;
chip_info					dbg_device[2];
unsigned					patch_channel;
struct startup_header		*shdr;
char						**_argv;
int							_argc;
unsigned 					max_cpus = ~0;
unsigned					system_icache_idx = CACHE_LIST_END;
unsigned					system_dcache_idx = CACHE_LIST_END;
chip_info					timer_chip;
unsigned 					(*timer_start)(void);
unsigned 					(*timer_diff)(unsigned start);
struct syspage_entry		*_syspage_ptr;
unsigned					misc_flags;

extern struct bootargs_entry	boot_args;	//filled in by mkifs

extern int	main(int argc,char **argv,char **envv);

static char				*argv[20], *envv[20];

static void
setup_cmdline(void) {
	char		*args;
	int			i, argc, envc;

	//
	// Find and construct argument and environment vectors for ourselves
	//

	tweak_cmdline(&boot_args, "startup");

	argc = envc = 0;
	args = boot_args.args;
	for(i = 0; i < boot_args.argc; ++i) {
		if(i < sizeof argv / sizeof *argv - 1) argv[argc++] = args;
		while(*args++) ;
	}
	argv[argc] = 0;

	for(i = 0; i < boot_args.envc; ++i) {
		if (i < sizeof envv / sizeof *envv - 1) envv[envc++] = args;
		while(*args++) ;
	}
	envv[envc] = 0;
	_argc = argc;
	_argv = argv;

}


size_t strlen(const char* str)
{
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}


/** Simple loop variable */
volatile unsigned int tim;
void
_main(void) {
	
	shdr = (struct startup_header *)boot_args.shdr_addr;

	board_init();

	setup_cmdline();

	cpu_startup();

	#define INIT_SYSPAGE_SIZE 0x600
	init_syspage_memory(ws_alloc(INIT_SYSPAGE_SIZE), INIT_SYSPAGE_SIZE);

	if(shdr->imagefs_paddr != 0) {
		avoid_ram(shdr->imagefs_paddr, shdr->stored_size);
	}
//done
	main(_argc, _argv, envv);

	//
	// Tell the mini-drivers that the next time they're called, they're
	// going to be in the kernel. Also flip the handler & data pointers
	// to the proper values for that environment.
	//
	mdriver_hook();

	//
	// Copy the local version of the system page we've built to the real
	// system page location we allocated in init_system_private().
	//
	write_syspage_memory();

	//
	// Tell the AP's that that the syspage is now present.
	//
	smp_hook_rtn();

	startnext();
}

static void
hook_dummy(void) {
}

void						(*smp_hook_rtn)(void) = hook_dummy;
void						(*mdriver_check)(void) = hook_dummy;
void						(*mdriver_hook)(void) = hook_dummy;


// Replacement for some C library stuff to minimize startup size
int errno;

int *
__get_errno_ptr(void) { return &errno; }


size_t
__stackavail(void) { return (size_t)~0; }

void
abort(void) { crash("ABORT"); for( ;; ) {} }


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/_main.c $ $Rev: 657836 $" );
