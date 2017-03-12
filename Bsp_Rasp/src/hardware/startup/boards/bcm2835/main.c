/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems.
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
/* QNX - Raspberry Pi BCM2835 board
 * Copyright 2016, Varghese Alphonse
 */

#include "startup.h"
#include <arm/bcm2835.h>

//#define BCM2835_DEBUG_LED
#define GPIO_BASE       0x20200000
#define LED_GPFSEL_GREEN      0x10
#define LED_GPFSEL_RED      0x0C
#define LED_GPFBIT_GREEN      21
#define LED_GPFBIT_RED      15
#define LED_GPSET       0x20
#define LED_GPCLR       0x2c
#define LED_GPIO_BIT_GREEN    15
#define LED_GPIO_BIT_RED    3
uintptr_t gpio;


extern struct callout_rtn 	reboot_at91sam9xx;
//extern struct callout_rtn 	reboot_bcm2835;

extern struct callout_rtn	display_char_bcm2835;
extern struct callout_rtn	poll_key_bcm2835;
extern struct callout_rtn	break_detect_bcm2835;


void init_bcm2835_debug(unsigned, const char *, const char *);
void put_bcm2835(int);

extern void pio_init (void );
extern char * get_board_name();
extern void init_qtime_bcm2835();

const struct callout_slot callouts[] = {
	{ CALLOUT_SLOT( reboot, _at91sam9xx ) },
};

const struct debug_device debug_devices[] = {
	{ 	"bcm2835",
		{	"0xFFFFEE00^0.0.133000000.16",	/* Use whatever boot loader baud rate */
		},
		init_bcm2835_debug,
		put_bcm2835,
		{	&display_char_bcm2835,
			&poll_key_bcm2835,
			&break_detect_bcm2835,
		}
	},
};

 // clock value has to be read using mailbox , Not implemented

unsigned long BCM2835_clock(int OP) {
        unsigned int dwClk;
        dwClk=700000000; //700MHz default clock , Not a real value
        return dwClk ;
}

static void inline loop( unsigned count ) {
	volatile unsigned	 tmp = count;

	__asm__ __volatile__(
		"1:	subs	%0, %0, #1;"
		"	bgt		1b"
		:
		: "r" (tmp)
	);
}

/*
 * main()
 *	Startup program executing out of RAM
 *
 * 1. It gathers information about the system and places it in a structure
 *    called the system page. The kernel references this structure to
 *    determine everything it needs to know about the system. This structure
 *    is also available to user programs (read only if protection is on)
 *    via _syspage->.
 *
 * 2. It (optionally) turns on the MMU and starts the next program
 *    in the image file system.
 */

int
main(int argc, char **argv, char **envv)
{
	int	opt, osc_startup_ms = 0;

	add_callout_array(callouts, sizeof(callouts));  // fix me

	while ( ( opt = getopt( argc, argv, COMMON_OPTIONS_STRING "s:" ) ) != -1 ) {
		switch ( opt ) {
		case 's':	osc_startup_ms = strtoul( optarg, NULL, 10 );	break;
		default:		handle_common_option( opt );							break;
		}
	}

#ifdef BCM2835_DEBUG_LED
// This code works for Raspberry 1 B+
	unsigned int val, j,tim;
	gpio = startup_io_map(0x20, GPIO_BASE);

    val=in32(gpio+LED_GPFSEL_GREEN);
    val&=~(7<<LED_GPFBIT_GREEN);
	val|=1 << LED_GPFBIT_GREEN;
	out32(gpio+LED_GPFSEL_GREEN,val);

    val=in32(gpio+LED_GPFSEL_RED);
    val&=~(7<<LED_GPFBIT_RED);
    val|=1<<LED_GPFBIT_RED;
    out32(gpio+LED_GPFSEL_RED,val);

	for(j=0;j<3;j++)
		{
		        for(tim = 0; tim < 5000000; tim++)
		        {
		           out32(gpio+LED_GPCLR,1<<LED_GPIO_BIT_GREEN);
		           out32(gpio+LED_GPCLR,1<<LED_GPIO_BIT_RED);
		        }
		        for(tim = 0; tim < 5000000; tim++)
		        {
		        	out32(gpio+LED_GPSET,1 << LED_GPIO_BIT_GREEN);
		        	out32(gpio+LED_GPSET,1 << LED_GPIO_BIT_RED);
		        }

		 }
#endif

	/*
	 * set CPU frequency
	 */
	if (cpu_freq == 0)
	{
               cpu_freq = BCM2835_clock(0);
	}

	/*
	 * Initialise debugging output
	 */
	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	//out32(BCM2835_GPPUD, 0x00000000);
	out32(BCM2835_GPIO_BASE+BCM2835_GPIO_GPPUD, 0x00000000);
	//delay(150);

	// Disable pull up/down for pin 14,15 & delay for 150 cycles.
	//out32(BCM2835_GPPUDCLK0, (1 << 14) | (1 << 15));
	out32(BCM2835_GPIO_BASE+BCM2835_GPIO_GPPUDCLK0, (1 << 14) | (1 << 15));

	// Write 0 to GPPUDCLK0 to make it take effect.
	//out32(BCM2835_GPPUDCLK0, 0x00000000);
	out32(BCM2835_GPIO_BASE+BCM2835_GPIO_GPPUDCLK0, 0x00000000);

	select_debug(debug_devices, sizeof(debug_devices));

	/*
	 * Initialize PIO
	 */
	 pio_init();  //empty call

	/*
	 * Collect information on all free RAM in the system
	 */
	init_raminfo();

	/*
	 * Do board specific initialization
	 */


	/*
	 * Remove RAM used by modules in the image
	 */
	alloc_ram(shdr->ram_paddr, shdr->ram_size, 1);
	if (shdr->flags1 & STARTUP_HDR_FLAGS1_VIRTUAL)
	{
		init_mmu();
	}

	init_intrinfo();
	init_qtime_bcm2835();
	init_cacheattr();
	init_cpuinfo();
	init_hwinfo();
	/* Startup time must be provided in us. */
	//init_slow_clock( osc_startup_ms * 1000 );

	add_typed_string(_CS_MACHINE, (const char *)get_board_name());

	/*
	 * Load bootstrap executables in the image file system and Initialise
	 * various syspage pointers. This must be the _last_ initialisation done
	 * before transferring control to the next program.
	 */
	init_system_private();

	/*
	 * This is handy for debugging a new version of the startup program.
	 * Commenting this line out will save a great deal of code.
	 */
	print_syspage();

	kprintf("Jumping to QNX\n");

	return 0;
}
