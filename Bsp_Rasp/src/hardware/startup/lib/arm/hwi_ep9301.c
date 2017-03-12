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


#include "startup.h"
#include "hwinfo_private.h"
#include "arm/hwinfo_ep93xx_proto.h"
#include "arm/ep93xx_private.h"
#include <hw/hwinfo_ep93xx.h>
#include <drvr/hwinfo.h>		// for hwi support routines in libdrvr

/*
 * Add EP93xx specific devices to the hardware info section of the syspage
 *
 * Note that there is an internal SSP device capable of SPI, TI SSI or National
 * Semiconductor serial protocols. Additionally, the SSP pins are multiplexed
 * with the I2S controller therefore only 1 of these functions can actually be
 * enabled at a time. This is controlled with the <flags> parameter.
*/

void hwi_ep9301(hwiep93xx_flags_t flags)
{
	unsigned hwi_bus_ahb;
	unsigned hwi_bus_apb = HWI_NULL_OFF;
	const unsigned chip_rev = ep93xx_get_chiprev();	// used for handling errata

	/* add the high speed AHB and slower APB internal buses to which all SOC
	 * devices will be attached. We handle being called multiple times and will
	 * only add 1 instance of each */
	if ((hwi_bus_ahb = hwi_find_bus(EP93xx_HWI_AHB, 0)) == HWI_NULL_OFF)
	{
		hwi_bus_ahb = hwibus_add(EP93xx_HWI_AHB, HWI_NULL_OFF);
		ASSERT(hwi_find_unit(hwi_bus_ahb) == 0);
		hwi_bus_apb = hwibus_add(EP93xx_HWI_APB, hwi_bus_ahb);
		ASSERT(hwi_find_unit(hwi_bus_apb) == 0);
	}

	/* SSP device */
	{
		/* ... as SPI */
		unsigned hwi_off;
		hwiattr_spi_t attr = HWIATTR_SPI_T_INITIALIZER;
		struct hwi_inputclk clksrc = {.clk = ep93xx_get_sspclk(), .div = 1};

		HWIATTR_SPI_SET_NUM_IRQ(&attr, 3);	// SSP has 3 interrupts
		HWIATTR_SPI_SET_NUM_CLK(&attr, 1);
		HWIATTR_SPI_SET_DLL(&attr, "ep9301");	// DLL name for use as SPI

		/* create device and set the clock source */
		HWIATTR_SPI_SET_LOCATION(&attr, EP93xx_SPI_CTRL_BASE, EP93xx_SPI_CTRL_SIZE, 0, hwi_find_as(EP93xx_SPI_CTRL_BASE, 1));
		hwi_off = hwibus_add_spi(hwi_bus_apb, &attr);
		ASSERT(hwi_off != HWI_NULL_OFF);
		hwitag_set_inputclk(hwi_off, 0, &clksrc);
		hwi_add_synonym(hwi_off, EP93xx_HWI_SSP);	/* provide SSP name as an alias */
	}

	/* add 1 USB controller */
	{
		unsigned hwi_off;
		hwiattr_usb_t attr = HWIATTR_USB_T_INITIALIZER;
		unsigned long usb_clk = ep93xx_get_usbclk();

		HWIATTR_USB_SET_NUM_IRQ(&attr, 1);
//		HWIATTR_USB_SET_DLL(&attr, "ehci-???");

		/* REV E0 and E1 silicon has errata for the USB controller regarding buffer alignment
		 * and size requirements (quad word).
		 * Cirrus does not seem to use a unique errata number so this particular errata is
		 * called "Description 1" and can be found in the E1 errata doc ER636E1REV2.
		 * We will use the value 1 to flag this errata. A 2nd errata "Description 2" that
		 * discusses the maximum supported USB clock of 288MHz (also on E2 silicon but called
		 * "Description 1" .. thanks Cirrus for this bogus way of handling errata). It will
		 * also be added so that the driver could do something like restrict the link bandwidth
		 * as required */
		if ((chip_rev == 5/*E0*/) || (chip_rev == 6/*E1*/)) {
			HWIATTR_ENET_SET_NUM_ERRATA(&attr, (usb_clk > 288000000) ? 2 : 1);
			if (usb_clk > 288000000) {
				kprintf("\n*** WARNING: USB clock of %d is to high for this Silicon Rev (see errata)\n\n", usb_clk);
			}
		} else if ((chip_rev == 7/*E2*/) && (usb_clk > 288000000)) {
			HWIATTR_ENET_SET_NUM_ERRATA(&attr, 1);
			kprintf("\n*** WARNING: USB clock of %d is to high for this Silicon Rev (see errata)\n\n", usb_clk);
		}

		/* create usb0 */
		HWIATTR_USB_SET_LOCATION(&attr, EP93xx_USB_CTRL_BASE, EP93xx_USB_CTRL_SIZE, 0, hwi_find_as(EP93xx_USB_CTRL_BASE, 1));
		hwi_off = hwibus_add_usb(hwi_bus_ahb, &attr);
		ASSERT(hwi_off != HWI_NULL_OFF);
		hwitag_add_inputclk(hwi_off, usb_clk, 1);

		if ((chip_rev == 5/*E0*/) || (chip_rev == 6/*E1*/)) {
			hwitag_set_errata(hwi_off, 0, 1);	// errata 1 (Description 1)
			if (usb_clk > 288000000) {
				hwitag_set_errata(hwi_off, 1, 2);	// errata 2 (Description 2)
			}
		} else if ((chip_rev == 7/*E2*/) && (usb_clk > 288000000)) {
			hwitag_set_errata(hwi_off, 0, 2);	// need to call it errata 2 (Description 2) for the driver
		}
	}

	/* add 2 UART's */
	{
		unsigned hwi_off;
		hwiattr_uart_t attr = HWIATTR_UART_T_INITIALIZER;
		struct hwi_inputclk clksrc = {.clk = ep93xx_get_uartclk(), .div = 1};

		/* each UART has a separate Tx and Rx interrupt and well as a general interrupt */
		HWIATTR_UART_SET_NUM_IRQ(&attr, 3);
		HWIATTR_UART_SET_NUM_CLK(&attr, 1);

		/* create uart0 and set the clock source */
		HWIATTR_UART_SET_LOCATION(&attr, EP93xx_UART1_BASE, EP93xx_UART1_SIZE, 0, hwi_find_as(EP93xx_UART1_BASE, 1));
		hwi_off = hwidev_add_uart(EP93xx_HWI_UART, &attr, hwi_bus_apb);
		ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_inputclk(hwi_off, 0, &clksrc);

		/* create uart1 and set the clock source */
		HWIATTR_UART_SET_LOCATION(&attr, EP93xx_UART2_BASE, EP93xx_UART2_SIZE, 0, hwi_find_as(EP93xx_UART2_BASE, 1));
		hwi_off = hwidev_add_uart(EP93xx_HWI_UART, &attr, hwi_bus_apb);
		ASSERT(hwi_find_unit(hwi_off) == 1);
		hwitag_set_inputclk(hwi_off, 0, &clksrc);
	}

	/* add an ethernet device */
	{
		unsigned hwi_off;
		hwiattr_enet_t attr = HWIATTR_ENET_T_INITIALIZER;
		const uint8_t mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};	// a non zero mac

		/* if BSP wants to provide a MAC address, add in the tag */
		if (flags & hwiep93xx_flags_t_USE_MAC) HWIATTR_ENET_SET_MAC(&attr, mac);

		HWIATTR_ENET_SET_NUM_IRQ(&attr, 1);
		HWIATTR_ENET_SET_NUM_PHY(&attr, 1);
		HWIATTR_ENET_SET_DLL(&attr, "ep93xx");

		/* REV E0, E1 and E2 silicon has errata for the ethernet controller regarding 64 byte frames
		 * Cirrus does not seem to use a unique errata number so this particular errata is
		 * called "Description 1" and can be found in the E0 and E1 errata docs ER636E0 and
		 * ER636E1REV2 respectively. We will use the value 1 to flag this errata.
		 * A 2nd errata "Description 2" that discusses AHB bandwidth. It will also be added so
		 * that the driver could do something like restrict the link bandwidth as required */
		if ((chip_rev == 5/*E0*/) || (chip_rev == 6/*E1*/) || (chip_rev == 7/*E2*/)) {
			HWIATTR_ENET_SET_NUM_ERRATA(&attr, 2);
		}

		/* create device and set the PHY address */
		HWIATTR_ENET_SET_LOCATION(&attr, EP93xx_ENET_CTRL_BASE, EP93xx_ENET_CTRL_SIZE, 0, hwi_find_as(EP93xx_ENET_CTRL_BASE, 1));
		hwi_off = hwidev_add_enet(EP93xx_HWI_ENET, &attr, hwi_bus_ahb);
		ASSERT(hwi_off != HWI_NULL_OFF);
		/* By default we assume PHY's are connected in numeric order. If this is not
		 * the case, then correct it in the board specific init_hwinfo() */
		hwitag_set_phyaddr(hwi_off, 0, 0);

		if ((chip_rev == 5/*E0*/) || (chip_rev == 6/*E1*/) || (chip_rev == 7/*E2*/)) {
			hwitag_set_errata(hwi_off, 0, 1);	// errata 1 (Description 1)
			hwitag_set_errata(hwi_off, 1, 2);	// errata 2 (Description 2)
		}
	}

	/* add 1 dma controller (12 channels) */
	{
		unsigned hwi_off;
		hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

		HWIATTR_DMA_SET_NUM_IRQ(&attr, 12);	// each of the 12 channels can interrupt

		/* create DMA controller */
		HWIATTR_USB_SET_LOCATION(&attr, EP93xx_DMA_M2P_CHAN0, 0x3C4, 0, hwi_find_as(EP93xx_DMA_M2P_CHAN0, 1));
		hwi_off = hwidev_add_dma(EP93xx_HWI_DMA, &attr, hwi_bus_ahb);
		ASSERT(hwi_off != HWI_NULL_OFF);
	}

	/* add the IrDA device */
	{
		unsigned hwi_off = hwidev_add(EP93xx_HWI_IrDA, hwi_devclass_NONE, hwi_bus_apb);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

		ASSERT(hwi_off != HWI_NULL_OFF);

		HWIATTR_SET_LOCATION(&attr, EP93xx_IrDA_CTRL_BASE, EP93xx_IrDA_CTRL_SIZE, 0, hwi_find_as(EP93xx_IrDA_CTRL_BASE, 1));
		HWIATTR_SET_NUM_IRQ(&attr, 1);
		hwitag_add_common(hwi_off, &attr);
		hwitag_add_inputclk(hwi_off, ep93xx_get_mirclk(), 1);
	}

	/* add the PWM device (no interrupts) */
	{
		unsigned hwi_off = hwidev_add(EP93xx_HWI_PWM, hwi_devclass_NONE, hwi_bus_apb);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

		ASSERT(hwi_off != HWI_NULL_OFF);

		HWIATTR_SET_LOCATION(&attr, EP93xx_PWM_CTRL_BASE, EP93xx_PWM_CTRL_SIZE, 0, hwi_find_as(EP93xx_PWM_CTRL_BASE, 1));
		hwitag_add_common(hwi_off, &attr);
		hwitag_add_inputclk(hwi_off, ep93xx_get_pwmclk(), 1);
	}

	/* add the AC97 audio device */
	{
		unsigned hwi_off = hwidev_add(EP93xx_HWI_AUDIO, hwi_devclass_NONE, hwi_bus_apb);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

		ASSERT(hwi_off != HWI_NULL_OFF);

		HWIATTR_SET_LOCATION(&attr, EP93xx_AC97_CTRL_BASE, EP93xx_AC97_CTRL_SIZE, 0, hwi_find_as(EP93xx_AC97_CTRL_BASE, 1));
		HWIATTR_SET_NUM_IRQ(&attr, 1);
		hwitag_add_common(hwi_off, &attr);
		hwitag_add_inputclk(hwi_off, ep93xx_get_aacclk(), 1);
	}

	/* add the RTC device */
	{
		unsigned hwi_off;
		hwiattr_rtc_t rtcattr = HWIATTR_RTC_T_INITIALIZER;

		HWIATTR_RTC_SET_LOCATION(&rtcattr, EP93xx_RTC_CTRL_BASE, EP93xx_RTC_CTRL_SIZE, 0, hwi_find_as(EP93xx_RTC_CTRL_BASE, 1));
		/* the rtc has 3 interrupts, rtc match, 64Hz and 1Hz */
		HWIATTR_RTC_SET_NUM_IRQ(&rtcattr, 3);
		hwi_off = hwidev_add_rtc(EP93xx_HWI_RTC, &rtcattr, hwi_bus_apb);
		ASSERT(hwi_off != HWI_NULL_OFF);
		hwitag_add_inputclk(hwi_off, ep93xx_get_rtcclk(), 1);
	}

	/*
	 * add the GPIO device
	 * 19 of the EP9301 GPIO ports are interruptible and are handled via a
	 * cascaded interrupt entry. In order for devices specifically connected to
	 * those GPIO pins to have a driver InterruptAttach() we provide the assigned
	 * vector ranges with 3 irqrange tags corresponding to the PORTA, PORTB and
	 * PORTF interrupting port pins respectively. Within the range tag assignments,
	 * the 'irq' field corresponds to the LSb (pin 0 of the port) and 'irq' + 'num' - 1
	 * corresponds to the MSb (pin 7 of the port).
	*/
	{
		unsigned hwi_off = hwidev_add(EP93xx_HWI_GPIO, hwi_devclass_NONE, hwi_bus_apb);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

		ASSERT(hwi_off != HWI_NULL_OFF);

		HWIATTR_SET_LOCATION(&attr, EP93xx_GPIO_CTRL_BASE, EP93xx_GPIO_CTRL_SIZE, 0, hwi_find_as(EP93xx_GPIO_CTRL_BASE, 1));
		hwitag_add_common(hwi_off, &attr);
		hwitag_add_irqrange(hwi_off, HWI_ILLEGAL_VECTOR, 0);	// PORT A assigned vectors
		hwitag_add_irqrange(hwi_off, HWI_ILLEGAL_VECTOR, 0);	// PORT B assigned vectors
		hwitag_add_irqrange(hwi_off, HWI_ILLEGAL_VECTOR, 0);	// PORT F assigned vectors
	}

	/*
	 * add the 3 timer devices (note that timer3 we be dedicated to the qtimer). The clock sources
	 * for all 3 timers are the same and is either 508469Hz or 1994Hz depending on the setting
	 * of the CLKSEL bit in the respective timer control register.
	 * We do not currently configure Timer4 which is a free running up counter and based on a
	 * 983KHz clock.
	 * The watchdog timer is configured as a separate device
	*/
	{
		unsigned hwi_off;
		hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
		const struct hwi_inputclk clksrc_508KHz = {.clk = ep93xx_get_timerclk1(), .div = 1};
		const struct hwi_inputclk clksrc_2KHz = {.clk = ep93xx_get_timerclk2(), .div = 1};

		HWIATTR_TIMER_SET_NUM_IRQ(&attr, 1);
		HWIATTR_TIMER_SET_NUM_CLK(&attr, 2);

		/* timer 1 */
		HWIATTR_TIMER_SET_LOCATION(&attr, EP93xx_TIMER1_BASE, EP93xx_TIMER1_SIZE, 0, hwi_find_as(EP93xx_TIMER1_BASE, 1));
		hwi_off = hwidev_add_timer(EP93xx_HWI_TIMER, &attr, hwi_bus_apb);
		ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_2KHz);		// 2KHz when CLKSEL == 0
		hwitag_set_inputclk(hwi_off, 1, (struct hwi_inputclk *)&clksrc_508KHz);		// 508KHz when CLKSEL == 1

		/* timer 2 */
		HWIATTR_TIMER_SET_LOCATION(&attr, EP93xx_TIMER2_BASE, EP93xx_TIMER2_SIZE, 0, hwi_find_as(EP93xx_TIMER2_BASE, 1));
		hwi_off = hwidev_add_timer(EP93xx_HWI_TIMER, &attr, hwi_bus_apb);
		ASSERT(hwi_find_unit(hwi_off) == 1);
		hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_2KHz);		// 2KHz when CLKSEL == 0
		hwitag_set_inputclk(hwi_off, 1, (struct hwi_inputclk *)&clksrc_508KHz);		// 508KHz when CLKSEL == 1

		/* timer 3 */
		HWIATTR_TIMER_SET_LOCATION(&attr, EP93xx_TIMER3_BASE, EP93xx_TIMER3_SIZE, 0, hwi_find_as(EP93xx_TIMER3_BASE, 1));
		hwi_off = hwidev_add_timer(EP93xx_HWI_TIMER, &attr, hwi_bus_apb);
		ASSERT(hwi_find_unit(hwi_off) == 2);
		hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_2KHz);		// 2KHz when CLKSEL == 0
		hwitag_set_inputclk(hwi_off, 1, (struct hwi_inputclk *)&clksrc_508KHz);		// 508KHz when CLKSEL == 1
		/* use a synonym for the qtimer device */
		hwi_add_synonym(hwi_off, "qtimer");
	}

	/* add the WATCHDOG device */
	{
		unsigned hwi_off = hwidev_add(EP93xx_HWI_WDOG, hwi_devclass_TIMER, hwi_bus_apb);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

		ASSERT(hwi_off != HWI_NULL_OFF);

		HWIATTR_SET_LOCATION(&attr, EP93xx_WDOG_BASE, EP93xx_WDOG_SIZE, 0, hwi_find_as(EP93xx_WDOG_BASE, 1));
		HWIATTR_SET_NUM_IRQ(&attr, 1);
		hwitag_add_common(hwi_off, &attr);
		hwitag_add_inputclk(hwi_off, ep93xx_get_wdogclk(), 1);
	}

	/* add the (pseudo) external interrupt device (3 interrupt pins) */
	{
		unsigned hwi_off = hwidev_add(EP93xx_HWI_EXT_IRQ, hwi_devclass_NONE, hwi_bus_apb);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

		ASSERT(hwi_off != HWI_NULL_OFF);

		HWIATTR_SET_NUM_IRQ(&attr, 3);
		hwitag_add_common(hwi_off, &attr);
	}

	/* add the I2S device */
	{
		unsigned hwi_off = hwidev_add(EP93xx_HWI_I2S, hwi_devclass_NONE, hwi_bus_apb);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

		ASSERT(hwi_off != HWI_NULL_OFF);

		HWIATTR_SET_LOCATION(&attr, EP93xx_I2S_CTRL_BASE, EP93xx_I2S_CTRL_SIZE, 0, hwi_find_as(EP93xx_I2S_CTRL_BASE, 1));
		HWIATTR_SET_NUM_IRQ(&attr, 1);
		hwitag_add_common(hwi_off, &attr);
	}
}

__SRCVERSION("$URL$ $Rev$");
