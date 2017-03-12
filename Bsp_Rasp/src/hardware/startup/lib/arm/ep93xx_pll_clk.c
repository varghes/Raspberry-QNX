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
 * EP93xx PLL/Clock setting and value retrieval
 *
 * This file contains a collection of PLL and Clock setting and value
 * determination routines for use by boards utilizing the Cirrus Logic EP93xx
 * family of processors
 *
 * Note that the information in this file is based on Fig 5-2 from the EP93xx
 * Users Guide as well as the various system configuration register settings
 *
*/

#include "startup.h"
#include "ep93xx_private.h"

/* this is defined in the EP93xx manual. It must not be board specific */
static const unsigned long crystal_freq = 14745600;

extern paddr_t syscfg_base;		// defined in ep93xx_syscfg.c
#define MAP_REG \
		do { \
			if (syscfg_base == 0) { \
				syscfg_base = (paddr_t)startup_io_map(EP93xx_SYSCTRL_SIZE, EP93xx_SYSCTRL_BASE); \
			} \
		} while(0)

#define IN32(_x_)			in32(syscfg_base + (_x_))
#define OUT32(_x_, _v_)		out32(syscfg_base + (_x_), (_v_))


static unsigned long calc_pll_fout(uint32_t clkset_regval);
static unsigned long get_vco(unsigned pll);


/*
 * =============================================================================
 *                             G E T   A P I's
 * =============================================================================
*/

/*******************************************************************************
 * ep93xx_get_fclk
 *
 * Return the frequency of FCLK (processor clock)
 *
 * FCLK is determined from the VCO and the FCLK divider in CLKSET1
*/
unsigned long ep93xx_get_fclk(void)
{
	MAP_REG;
	const unsigned long vco_freq = get_vco(1);
	uint32_t fclkdiv = (IN32(EP93xx_SYSCTRL_CLKSET1) & 0x0E000000U) >> 25;

	if (fclkdiv > 4) fclkdiv = 0;
	return (vco_freq >> fclkdiv);
}

/*******************************************************************************
 * ep93xx_get_hclk
 *
 * Return the frequency of HCLK (AHB high speed bus clock)
 *
 * HCLK is determined from the VCO and the HCLK divider in CLKSET1
*/
unsigned long ep93xx_get_hclk(void)
{
	MAP_REG;
	const uint64_t vco_freq = get_vco(1);
	uint32_t hclkdiv;
	static const unsigned _hclkdiv[] = {[0]=1, [1]=2, [2]=4, [3]=5, [4]=6, [5]=8, [6]=16, [7]=32};

	hclkdiv = (IN32(EP93xx_SYSCTRL_CLKSET1) & 0x00700000U) >> 20;

	return (unsigned long)(vco_freq / _hclkdiv[hclkdiv]);
}

/*******************************************************************************
 * ep93xx_get_pclk
 *
 * Return the frequency of PCLK (APB peripheral bus clock)
 *
 * PCLK is determined from the HCLK and the PCLK divider in CLKSET1
*/
unsigned long ep93xx_get_pclk(void)
{
	MAP_REG;
	const unsigned long hclk = ep93xx_get_hclk();
	uint32_t pclkdiv = (IN32(EP93xx_SYSCTRL_CLKSET1) & 0x000C0000U) >> 18;

	return (hclk >> pclkdiv);
}

/*******************************************************************************
 * ep93xx_get_cpuclk
 *
 * Return the frequency of the clock actually driving the CPU core.
 *
 * The clock which drives the CPU core is selected by the iA and nF bits in
 * register 1 of co-processor 15. These bits determine whether FCLK or HCLK
 * is selected to drive the core.
 *
 * Page 73 of the EP9301 Users Guide talks about 3 clocking modes supported by
 * the processor. The text says the following ...
 *
 * <quote>
 * 		It also must be pointed out that even though FCLK is the ARM processor
 * 		clock, the ARM Processor has the option to run the CPU using HCLK. The
 * 		ARM9 Processor supports three different clocking modes:
 *
 * 		  • Async mode
 * 		  • Sync mode
 * 		  • Fast Bus mode
 *
 *		Both Async mode and Sync mode use FCLK and potentially FCLK can be
 * 		faster than HCLK which would yield higher CPU performance. Async mode
 * 		and Sync mode have different clock skew requirements between FCLK and
 * 		HCLK, associated with different throughput penalties due to the clock
 * 		synchronization. Fast Bus mode bypasses FCLK, and the ARM runs from
 * 		HCLK. In this mode, the ARM potentially has lower performance than the
 * 		other two modes. When the device starts up, it defaults to Fast Bus mode.
 * 		(The selection of ARM clocking modes is determined by the iA and nF bits
 * 		in the ARM co-processor 15 register 1.)
 * </quote>
 *
 * The following description of the clocking modes and the meaning of the iA and
 * nf bits is taken from the "ARM920T Architecture Reference Manual"
 *
 * <quote>
 *
 * 		31		iA bit Asynchronous clock select See Table 2-11 on page 2-13.
 * 		30		nF bit notFastBus select         See Table 2-11 on page 2-13.
 *
 * Register 1 bits [31:30] select the clocking mode of the ARM920T, as shown in
 * Table 2-11.
 *
 *                                                     Table 2-11 Clocking modes
 *
 *                                                      Clocking mode     iA  nF
 *                                                       FastBus mode      0   0
 *                                                       Synchronous       0   1
 *                                                       Reserved          1   0
 *                                                       Asynchronous      1   1
 * </quote>
 *
 * For the purposes of selecting which clock is driving the processor, from the
 * description above, if Fast Bus mode is selected, HCLK drives the processor,
 * otherwise FCLK drives the processor
*/
#define CPU_CLKMODE_BITS	(ARM_MMU_CR_nF | ARM_MMU_CR_iA)
unsigned long ep93xx_get_cpuclk(void)
{
	MAP_REG;
	uint32_t CP15_reg1;

	/* read CP15 register 1 and test the setting of the iA[] and nF[] bits */
	__asm__ volatile("mrc  p15, 0, %0, c1, c0, 0" : "=r"(CP15_reg1));
//kprintf("CP15 reg 1, op 0 = 0x%x\n", CP15_reg1);
	return (CP15_reg1 & CPU_CLKMODE_BITS) ? ep93xx_get_fclk() : ep93xx_get_hclk();
}

/*******************************************************************************
 * ep93xx_get_uartclk
 *
 * Return the frequency of the clock driving the UART's
 *
 * The returned value is dependent on the setting of bit 29 in the PWRCNT
 * register.
 *
*/
unsigned long ep93xx_get_uartclk(void)
{
	MAP_REG;
	uint32_t reg_val = IN32(EP93xx_SYSCTRL_PWRCNT);
	return (reg_val & 0x20000000U) ? crystal_freq : (crystal_freq / 2);
}

/*******************************************************************************
 * ep93xx_get_sspclk
 *
 * Return the frequency of the clock driving the SSP
*/
unsigned long ep93xx_get_sspclk(void) {return crystal_freq / 2;}

/*******************************************************************************
 * ep93xx_get_pwmclk
 *
 * Return the frequency of the clock driving the PWM
*/
unsigned long ep93xx_get_pwmclk(void) {return crystal_freq;}

/*******************************************************************************
 * ep93xx_get_aacclk
 *
 * Return the frequency of the clock driving the Audio Codec
*/
unsigned long ep93xx_get_aacclk(void) {return crystal_freq / 5;}

/*******************************************************************************
 * ep93xx_get_timerclk1
 * ep93xx_get_timerclk2
 * ep93xx_get_timerclk3
 *
 * Return the frequency of first, second and third clock sources driving the
 * timers. The third clock source is only used by TIMER4. The first and second
 * are selectable for each of the other 3 timers.
 *
*/
unsigned long ep93xx_get_timerclk1(void) {return crystal_freq / 29;}	/* 508469 Hz */
unsigned long ep93xx_get_timerclk2(void) {return crystal_freq / 7395;}	/* 1994 Hz */
unsigned long ep93xx_get_timerclk3(void) {return crystal_freq / 15;}	/* 983000 Hz */

/*******************************************************************************
 * ep93xx_get_rtcclk
 *
 * Return the frequency of the clock driving the RTC
 * The RTC is driven by the 32KHz clock which is actually 32768 Hz
*/
unsigned long ep93xx_get_rtcclk(void) {return 32768;}

/*******************************************************************************
 * ep93xx_get_wdogclk
 *
 * Return the frequency of the clock driving the Watchdog.
 * It is derived from the RTC clock.
*/
unsigned long ep93xx_get_wdogclk(void) {return ep93xx_get_rtcclk() / 128 ;}

/*******************************************************************************
 * ep93xx_get_usbclk
 *
 * Return the frequency of the clock driving the USB controller
*/
unsigned long ep93xx_get_usbclk(void)
{
	MAP_REG;
	const uint64_t vco_freq = get_vco(2);
	uint32_t usbclkdiv = (IN32(EP93xx_SYSCTRL_CLKSET2) & 0xF0000000U) >> 28;

	usbclkdiv = (usbclkdiv + 1) % 15;
	return (unsigned long)(vco_freq / usbclkdiv);
}

/*******************************************************************************
 * ep93xx_get_vclk
 *
 * Return the frequency of the clock driving the VCLK (video circuits)
 *
 * The VCLK is determined by the settings of the VIDCLKDIV register and can be
 * derived from either the external oscillator, PLL1 or PLL2. From there it
 * is pre-divided from the PDIV field and then divided by the VDIV field in
 * order to obtain the final frequency
*/
unsigned long ep93xx_get_vclk(void)
{
	MAP_REG;
	const uint32_t reg_val = IN32(EP93xx_SYSCTRL_VIDCLKDIV);
	const uint32_t psel = (reg_val & 0x00002000U) >> 13;
	const uint32_t esel = (reg_val & 0x00004000U) >> 14;
	const uint32_t pdiv = (reg_val & 0x00000300U) >> 8;
	const uint32_t vdiv = (reg_val & 0x0000007FU) >> 0;
	const uint64_t clk = esel ? get_vco(psel + 1) : crystal_freq;
	const unsigned _pdiv_x_10[] = {[1] = 20, [2] = 25, [3] = 30};

	return ((pdiv == 0) || (vdiv < 2)) ? 0 : ((10 * clk) / _pdiv_x_10[pdiv]) / vdiv;
}

/*******************************************************************************
 * ep93xx_get_mirclk
 *
 * Return the frequency of the clock driving the MIRCLK (IrDA controller)
 *
 * The MIRCLK is determined by the settings of the MIRCLKDIV register and can be
 * derived from either the external oscillator, PLL1 or PLL2. From there it
 * is pre-divided from the PDIV field and then divided by the MDIV field in
 * order to obtain the final frequency
*/
unsigned long ep93xx_get_mirclk(void)
{
	MAP_REG;
	const uint32_t reg_val = IN32(EP93xx_SYSCTRL_MIRCLKDIV);
	const uint32_t psel = (reg_val & 0x00002000U) >> 13;
	const uint32_t esel = (reg_val & 0x00004000U) >> 14;
	const uint32_t pdiv = (reg_val & 0x00000300U) >> 8;
	const uint32_t mdiv = (reg_val & 0x0000007FU) >> 0;
	const uint64_t clk = esel ? get_vco(psel + 1) : crystal_freq;
	const unsigned _pdiv_x_10[] = {[1] = 20, [2] = 25, [3] = 30};

	return ((pdiv == 0) || (mdiv == 0)) ? 0 : ((10 * clk) / _pdiv_x_10[pdiv]) / mdiv;
}

/*******************************************************************************
 * ep93xx_get_mclk
 *
 * Return the frequency of the clock driving the MCLK (audio controller)
 *
 * The MCLK is determined by the settings of the I2SCLKDIV register and can be
 * derived from either the external oscillator, PLL1 or PLL2. From there it
 * is pre-divided from the PDIV field and then divided by the MDIV field in
 * order to obtain the final frequency
*/
unsigned long ep93xx_get_mclk(void)
{
	MAP_REG;
	const uint32_t reg_val = IN32(EP93xx_SYSCTRL_I2SCLKDIV);
	const uint32_t psel = (reg_val & 0x00002000U) >> 13;
	const uint32_t esel = (reg_val & 0x00004000U) >> 14;
	const uint32_t pdiv = (reg_val & 0x00000300U) >> 8;
	const uint32_t mdiv = (reg_val & 0x0000007FU) >> 0;
	const uint64_t clk = esel ? get_vco(psel + 1) : crystal_freq;
	const unsigned _pdiv_x_10[] = {[1] = 20, [2] = 25, [3] = 30};

	return ((pdiv == 0) || (mdiv == 0)) ? 0 : ((10 * clk) / _pdiv_x_10[pdiv]) / mdiv;
}

/*******************************************************************************
 * ep93xx_get_sclk
 *
 * Return the frequency of the clock driving the SCLK (audio controller)
*/
unsigned long ep93xx_get_sclk(void)
{
	MAP_REG;
	const uint32_t reg_val = IN32(EP93xx_SYSCTRL_I2SCLKDIV);
	const uint32_t sdiv = (reg_val & 0x00010000U) >> 16;
	const uint64_t sclk = ep93xx_get_mclk() / (2 << sdiv);
	return (unsigned long)sclk;
}

/*******************************************************************************
 * ep93xx_get_lrclk
 *
 * Return the frequency of the clock driving the LRCLK (audio controller)
*/
unsigned long ep93xx_get_lrclk(void)
{
	MAP_REG;
	const uint32_t reg_val = IN32(EP93xx_SYSCTRL_I2SCLKDIV);
	const uint32_t lrdiv = (reg_val & 0x00060000U) >> 17;
	const uint64_t lrclk = ep93xx_get_sclk() / (32 << (lrdiv % 3));
	return (unsigned long)lrclk;
}

/*******************************************************************************
 * ep93xx_get_adcclk
 *
 * Return the frequency of the clock driving the ADC_CLK (keypad and touch screen)
*/
unsigned long ep93xx_get_adcclk(void)
{
	MAP_REG;
	const uint32_t reg_val = IN32(EP93xx_SYSCTRL_KEYTCHCLKDIV);
	const uint32_t adiv = (reg_val & 0x00010000U) >> 16;
	const uint64_t adcclk = crystal_freq / (adiv ? 4 : 16);
	return (unsigned long)adcclk;
}

/*******************************************************************************
 * ep93xx_get_keyclk
 *
 * Return the frequency of the clock driving the KEY_CLK (keypad and touch screen)
*/
unsigned long ep93xx_get_keyclk(void)
{
	MAP_REG;
	const uint32_t reg_val = IN32(EP93xx_SYSCTRL_KEYTCHCLKDIV);
	const uint32_t kdiv = (reg_val & 0x00000001U) >> 0;
	const uint64_t keyclk = crystal_freq / (kdiv ? 4 : 16);
	return (unsigned long)keyclk;
}

/*******************************************************************************
 * ep93xx_get_touchclk
 *
 * Return the frequency of the clock driving the TOUCH_CLK (keypad and touch screen)
*/
unsigned long ep93xx_get_touchclk(void) {return ep93xx_get_adcclk() / 4;}

/*******************************************************************************
 * ep93xx_get_filtclk
 *
 * Return the frequency of the clock driving the FILT_CLK (keypad and touch screen)
*/
unsigned long ep93xx_get_filtclk(void) {return ep93xx_get_adcclk() / 2;}

/*
 * =============================================================================
 *                             S E T   A P I's
 * =============================================================================
*/


/*
 * =============================================================================
 *                     E N A B L E / I S E N A B L E D
 *                                 A P I's
 * =============================================================================
*/


/*
 * =============================================================================
 *                   S U P P O R T   F U N C T I O N S
 * =============================================================================
*/

/*******************************************************************************
 * calc_pll_fout
 *
 * Given the <clkset_reg> value, calculate and return the frequency Fout for
 * the PLL associated with <clkset_regval>
 *
 * The formula given to calculate Fout is on 5-5 of the EP93xx Users Guide and
 * is as follows (note that 14.7456 is multiplied by the other terms
 *
 *                               ( PLL1_X1FBD + 1 ) × ( PLL1_X2FBD + 1 )
 *        Fout = 14.7456MHz x    _______________________________________
 *                                   ( PLL1_X2IPD + 1 ) × 2^PLL1_PS
 *
 * The bit fields in the formula are all obtained from the CLKSET1 and CLKSET2
 * registers repectively for PLL1 and PLL2
 *
 * After reset, the default values for PLL1 are such that the equation reduces
 * to
 *        Fout = 14.7456MHz x 2.3
 *             = 36.864 MHz
 * and for PLL2
 *        Fout = 14.7456MHz x 3.255
 *             = 48 MHz
*/
static unsigned long calc_pll_fout(uint32_t clkset_regval)
{
	uint32_t PLL1_X1FBD = (clkset_regval & 0x0000F800U) >> 11;
	uint32_t PLL1_X2FBD = (clkset_regval & 0x000007E0U) >> 5;
	uint32_t PLL1_X2IPD = (clkset_regval & 0x0000001FU) >> 0;
	uint32_t PLL1_PS = (clkset_regval & 0x00030000U) >> 16;
	uint64_t fout;

	/* the value of 1000 is to increase the precision of the final result w/o FP */
	fout = (1000 * (PLL1_X1FBD + 1) * (PLL1_X2FBD + 1)) / (PLL1_X2IPD + 1);
	fout /= (1 << PLL1_PS);
	fout *= crystal_freq;
	fout /= 1000;
	return (unsigned long)fout;
}

/*******************************************************************************
 * get_vco
 *
 * Return the frequency of the VCO for the specified <pll>.
 *
 * The VCO is determined by the nBYP bit in the respective CLKSET register for
 * either PLL1 or PLL2. If the bit is set, the PLL is selected as the VCO output.
 * If the bit is clear, the external 'crystal_freq' is selected.
*/
static unsigned long get_vco(unsigned pll)
{
	MAP_REG;
	ASSERT((pll == 1) || (pll == 2));
	uint32_t reg_val = IN32((pll == 1) ? EP93xx_SYSCTRL_CLKSET1 : EP93xx_SYSCTRL_CLKSET2);
	const uint32_t BYP_mask = (pll == 1) ? 0x00800000U : 0x00080000U;
	unsigned long vco_freq = (reg_val & BYP_mask) ? calc_pll_fout(reg_val) : crystal_freq;

//kprintf("CLKSET%d = 0x%x\n", pll, reg_val);
	if (debug_flag > 1) kprintf("VCO%d --> %s, %d Hz\n", pll, (reg_val & 0x00800000U) ? "PLL" : "OSC", vco_freq);
	return vco_freq;
}

