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

/*
 * EP93xx system configuration get/set routines excluding clock/pll
*/

#include "startup.h"
#include "ep93xx_private.h"

paddr_t syscfg_base = 0;	// shared with ep93xx_pll_clk.c
#define MAP_REG \
		do { \
			if (syscfg_base == 0) { \
				syscfg_base = (paddr_t)startup_io_map(EP93xx_SYSCTRL_SIZE, EP93xx_SYSCTRL_BASE); \
			} \
		} while(0)

#define IN32(_x_)			in32(syscfg_base + (_x_))
#define OUT32(_x_, _v_)		out32(syscfg_base + (_x_), (_v_))


/*
 * =============================================================================
 *                             G E T   A P I's
 * =============================================================================
*/

/*******************************************************************************
 * ep93xx_get_resetflags
 * 
 * Return the set of reset causes.
 * All bits except the reset cause bits are masked from the read of PwrSts
 * register.
 * There are a bunch of ep93xx_is_xxreset() calls that can be used to check for
 * specific reset types
*/
unsigned ep93xx_get_resetflags(void)
{
	MAP_REG;
	return IN32(EP93xx_SYSCTRL_PWRSTS) & 0x0000BC00U;
}

/*******************************************************************************
 * ep93xx_clr_resetflags
 * 
 * Clear the reset flags returning the previous values
*/
unsigned ep93xx_clr_resetflags(void)
{
	unsigned reset_flags = ep93xx_get_resetflags();
	OUT32(EP93xx_SYSCTRL_STFCLR, 1);	// any value will do
	return reset_flags;
}

/*******************************************************************************
 * ep93xx_is_wdogreset
 * 
 * Return whether the watchdog timer was the cause of the last reset (true/false)
*/
unsigned ep93xx_is_wdogreset(void)
{
	return ((ep93xx_get_resetflags() & 0x00008000U) == 0x00008000U);
}

/*******************************************************************************
 * ep93xx_is_pwronreset
 * 
 * Return whether a power on was the cause of the last reset (true/false)
*/
unsigned ep93xx_is_pwronreset(void)
{
	return ((ep93xx_get_resetflags() & 0x00002000U) == 0x00002000U);
}

/*******************************************************************************
 * ep93xx_is_testreset
 * 
 * Return whether a power on was the cause of the last reset (true/false)
*/
unsigned ep93xx_is_testreset(void)
{
	return ((ep93xx_get_resetflags() & 0x00001000U) == 0x00001000U);
}

/*******************************************************************************
 * ep93xx_is_rstoreset
 * 
 * Return whether an active low RSTOn was the cause of the last reset (true/false)
 * This is also called User reset in the docs
*/
unsigned ep93xx_is_rstoreset(void)
{
	return ((ep93xx_get_resetflags() & 0x00000800U) == 0x00000800U);
}

/*******************************************************************************
 * ep93xx_is_swreset
 * 
 * Return whether software was the cause of the last reset (true/false)
*/
unsigned ep93xx_is_swreset(void)
{
	return ((ep93xx_get_resetflags() & 0x00000400U) == 0x00000400U);
}

/*******************************************************************************
 * ep93xx_get_resettype_str
 * 
 * Return the set of reset causes as a string which can be displayed
*/
const char *ep93xx_get_resettype_str(void)
{
	static char reset_str[24];
	unsigned n;

	reset_str[0] = '\0';
	if (ep93xx_is_pwronreset()) {
		n = strlen(reset_str);
		ksprintf(&reset_str[n], "%s", (n > 0)? "|PWR":"PWR");
	}
	if (ep93xx_is_wdogreset()) {
		n = strlen(reset_str);
		ksprintf(&reset_str[n], "%s", (n > 0)? "|WDOG":"WDOG");
	}
	if (ep93xx_is_rstoreset()) {
		n = strlen(reset_str);
		ksprintf(&reset_str[n], "%s", (n > 0)? "|RSTO":"RSTO");
	}
	if (ep93xx_is_swreset()) {
		n = strlen(reset_str);
		ksprintf(&reset_str[n], "%s", (n > 0)? "|SW":"SW");
	}
	if (ep93xx_is_testreset()) {
		n = strlen(reset_str);
		ksprintf(&reset_str[n], "%s", (n > 0)? "|TST":"TST");
	}
	
	return (const char *)reset_str;
}

/*******************************************************************************
 * ep93xx_get_bootmode
 * 
 * Return the set of reset causes.
 * All bits except the reset cause bits are masked from the read of PwrSts
 * register.
 * There are a bunch of ep93xx_is_xxreset() calls that can be used to check for
 * specific reset types
*/
unsigned ep93xx_get_bootmode(void)
{
	MAP_REG;
	return IN32(EP93xx_SYSCTRL_SYSCFG) & 0x000001FFU;
}

/*******************************************************************************
 * ep93xx_is_bootmode_internal
 * 
 * Return whether the boot mode is internal (true) or external (false)
*/
unsigned ep93xx_is_bootmode_internal(void)
{
	return ((ep93xx_get_bootmode() & 0x00000008U) == 0x00000008U);
}

/*******************************************************************************
 * ep93xx_is_bootmode_serial
 * 
 * Return whether the boot mode is serial (true) or normal (false)
*/
unsigned ep93xx_is_bootmode_serial(void)
{
	return ((ep93xx_get_bootmode() & 0x00000100U) == 0x00000100U);
}

/*******************************************************************************
 * ep93xx_is_bootmode_synchronous
 * 
 * Return whether the boot mode is synchronous (true) or asynchronous (false)
*/
unsigned ep93xx_is_bootmode_synchronous(void)
{
	return ((ep93xx_get_bootmode() & 0x00000020U) == 0x00000020U);
}

/*******************************************************************************
 * ep93xx_get_resettype_str
 * 
 * Return the boot mode as a string which can be displayed
*/
const char *ep93xx_get_bootmode_str(void)
{
	static char bootmode_str[40];
	unsigned n;
	unsigned boot_mode;

	bootmode_str[0] = '\0';
	if (ep93xx_is_bootmode_internal()) {
		n = strlen(bootmode_str);
		ksprintf(&bootmode_str[n], "%s", (n > 0)? "|INT":"INT");
	} else {
		n = strlen(bootmode_str);
		ksprintf(&bootmode_str[n], "%s", (n > 0)? "|EXT":"EXT");
	}

	if (ep93xx_is_bootmode_synchronous()) {
		n = strlen(bootmode_str);
		ksprintf(&bootmode_str[n], "%s", (n > 0)? "|SYNC":"SYNC");
	} else {
		n = strlen(bootmode_str);
		ksprintf(&bootmode_str[n], "%s", (n > 0)? "|ASYNC":"ASYNC");
	}

	if (ep93xx_is_bootmode_serial()) {
		n = strlen(bootmode_str);
		ksprintf(&bootmode_str[n], "%s", (n > 0)? "|SER":"SER");
	} else {
		n = strlen(bootmode_str);
		ksprintf(&bootmode_str[n], "%s", (n > 0)? "|NRML":"NRML");
	}

	/* display the LCSn bits 7,6 and 2,1
	 * 
	 * Note silicon revision E2 talks about the Boot Configuration errata
	 * whereby the LCSn bits *may* be incorrectly latched. This may lead to
	 * an incorrect display of information
	*/
	n = strlen(bootmode_str);
	boot_mode = ep93xx_get_bootmode();
	ksprintf(&bootmode_str[n], "|LCSn[%s%s...%s%s.]",	// kprintf() does not support %c 
			(boot_mode & 0x80) ? "1" : "0", (boot_mode & 0x40) ? "1" : "0",
			(boot_mode & 0x2) ? "1" : "0", (boot_mode & 0x1) ? "1" : "0");

	return (const char *)bootmode_str;
}

/*******************************************************************************
 * ep93xx_get_chipid
 * 
 * Return the CHIP ID register value
*/
unsigned ep93xx_get_chipid(void)
{
	MAP_REG;
	return (IN32(EP93xx_SYSCTRL_CHIPID) & 0x0000FFFFU) >> 0;
}

/*******************************************************************************
 * ep93xx_get_chiprev
 * 
 * Return the revision from the CHIP ID register
 * Note that this info seems to also be available in the SYSCFG register
 * although in the EP93xx Users Guide there are different values documented
 * between the registers and in the EP9301 Users guide they are the same
*/
unsigned ep93xx_get_chiprev(void)
{
	MAP_REG;
	return (IN32(EP93xx_SYSCTRL_CHIPID) & 0xF0000000U) >> 28;
}

/*******************************************************************************
 * ep93xx_get_chipver ????
 * 
 * Return the CHIP version (this is the chip id field of the PWRSTS register
 * not the CHIP ID register). This value gives the version of the specific
 * processor
*/
unsigned ep93xx_get_chipver(void)
{
	MAP_REG;
	return (IN32(EP93xx_SYSCTRL_PWRSTS) & 0x00FF0000U) >> 16;
}

/*******************************************************************************
 * ep93xx_get_chipman
 * 
 * Return the CHIP manufacturer
*/
unsigned ep93xx_get_chipman(void)
{
	MAP_REG;
	return (IN32(EP93xx_SYSCTRL_PWRSTS) & 0xFF000000U) >> 24;
}

/*******************************************************************************
 * ep93xx_clr_wdog_int
 * 
 * Clear the watchdog timer interrupt
*/
void ep93xx_clr_wdog_int(void)
{
	OUT32(EP93xx_SYSCTRL_TEOI, 1);
}


/*******************************************************************************
 * ep93xx_clr_bootmode
 * 
 * Clear the boot mode removing the boot ROM address remap
*/
void ep93xx_clr_bootmode(void)
{
	OUT32(EP93xx_SYSCTRL_BOOTMODECLR, 1);
}



/*
 * =============================================================================
 *                   S U P P O R T   F U N C T I O N S
 * =============================================================================
*/


