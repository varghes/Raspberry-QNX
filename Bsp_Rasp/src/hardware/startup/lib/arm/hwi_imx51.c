/*
 * $QNXLicenseC: 
 * Copyright 2010, QNX Software Systems.  
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
#include "arm/hwinfo_imx51_proto.h"
#include <hw/hwinfo_imx51.h>
#include <drvr/hwinfo.h>                // for hwi support routines in libdrvr
#include <arm/mx51.h>

/*
 * Add IMX51 specific devices to the hardware info section of the syspage
*/

extern uint32_t uart_clock;
	 
void hwi_imx51()
{
        unsigned hwi_bus_internal = 0;

        /* add I2C (unless directed not to) */
        {
                unsigned hwi_off;
                hwiattr_i2c_t attr = HWIATTR_I2C_T_INITIALIZER;
              
                HWIATTR_I2C_SET_NUM_IRQ(&attr, 1);
             
                /* create i2c0 and set the clock source */
                HWIATTR_I2C_SET_LOCATION(&attr, MX51_I2C1_BASE, MX51_I2C_SIZE, 0, hwi_find_as(MX51_I2C1_BASE, 1));
                hwi_off = hwibus_add_i2c(hwi_bus_internal, &attr);
                ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_ivec(hwi_off, 0, MX51_I2C1_IRQ);


                /* create i2c1 and set the clock source */
                HWIATTR_I2C_SET_LOCATION(&attr, MX51_I2C2_BASE, MX51_I2C_SIZE, 0, hwi_find_as(MX51_I2C2_BASE, 1));
                hwi_off = hwibus_add_i2c(hwi_bus_internal, &attr);
                ASSERT(hwi_find_unit(hwi_off) == 1);
		hwitag_set_ivec(hwi_off, 0, MX51_I2C2_IRQ);
        }

	/* add  UART */
        {
                unsigned hwi_off;
                hwiattr_uart_t attr = HWIATTR_UART_T_INITIALIZER;
				struct hwi_inputclk clksrc = {.clk = uart_clock, .div = 16};
				
                HWIATTR_UART_SET_NUM_IRQ(&attr, 1);
				HWIATTR_UART_SET_NUM_CLK(&attr, 1);

                /* create uart0 */
                HWIATTR_UART_SET_LOCATION(&attr, MX51_UART1_BASE, MX51_UART_SIZE, 0, hwi_find_as(MX51_UART1_BASE, 1));
                hwi_off = hwidev_add_uart(IMX51_HWI_UART, &attr, hwi_bus_internal);
                ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_ivec(hwi_off, 0, MX51_UART1_INTR);
		hwitag_set_inputclk(hwi_off, 0, &clksrc);

                /* historically the UART's were called 'mxl' so add these synonyms */
                hwi_add_synonym(hwi_find_device(IMX51_HWI_UART, 0), "sermxl");
        }

	/*add the FEC */
	{
		unsigned hwi_off;
		hwiattr_enet_t attr = HWIATTR_ENET_T_INITIALIZER;
		
		HWIATTR_USB_SET_NUM_IRQ(&attr, 1);
        	/* create eTSEC0 and set the PHY address */
		HWIATTR_ENET_SET_LOCATION(&attr, MX51_FEC_BASE, 0x4000, 0, hwi_find_as(MX51_FEC_BASE, 1));
		hwi_off = hwidev_add_enet("fec", &attr, hwi_bus_internal);
		ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_avail_ivec(hwi_off, 0, MX51_FEC_INTR);
   }


	/* add 1 USB controller */
	{
                unsigned hwi_off;
                hwiattr_usb_t attr = HWIATTR_USB_T_INITIALIZER;

                HWIATTR_USB_SET_NUM_IRQ(&attr, 1);
               
                /* create usb0 */
                HWIATTR_USB_SET_LOCATION(&attr, MX51_USBH1_BASE, MX51_USB_SIZE, 0, hwi_find_as(MX51_USBH1_BASE, 1));
                hwi_off = hwibus_add_usb(hwi_bus_internal, &attr);
                ASSERT(hwi_off != HWI_NULL_OFF);
		hwitag_set_avail_ivec(hwi_off, 0, MX51_USB_INTR);

        }

	/* add 1 sdma controllers */
        {
                unsigned hwi_off;
                hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

                HWIATTR_DMA_SET_NUM_IRQ(&attr, 1);

                /* create DMA controller 0 */
                HWIATTR_USB_SET_LOCATION(&attr, MX51_SDMA_BASE, MX51_SDMA_SIZE, 0, hwi_find_as(MX51_SDMA_BASE, 1));
                hwi_off = hwidev_add_dma(IMX51_HWI_DMA, &attr, hwi_bus_internal);
                ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_avail_ivec(hwi_off, 0, MX51_SDMA_INTRU);

	     }

	/* add the SSI device */
	{
		unsigned hwi_off = hwidev_add("ssi", hwi_devclass_NONE, HWI_NULL_OFF);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

		ASSERT(hwi_off != HWI_NULL_OFF);

		HWIATTR_SET_LOCATION(&attr, MX51_SSI1_BASE, MX51_SSI_SIZE, 0, hwi_find_as(MX51_SSI1_BASE, 1));
		hwitag_add_common(hwi_off, &attr);
	}

	/*add the SDRAM */
	{
		unsigned hwi_off = hwidev_add("sdram", hwi_devclass_NONE, HWI_NULL_OFF);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

		ASSERT(hwi_off != HWI_NULL_OFF);

		HWIATTR_SET_LOCATION(&attr, MX51_SDRAM_BASE, MX51_SDRAM_SIZE, 0, hwi_find_as(MX51_SDRAM_BASE, 1));
		hwitag_add_common(hwi_off, &attr);
	}


       	/* add the WATCHDOG device */
	{
		unsigned hwi_off;
		hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
		const struct hwi_inputclk clksrc_kick = {.clk = 10, .div = 1};
	
		HWIATTR_TIMER_SET_NUM_CLK(&attr, 1);

		HWIATTR_TIMER_SET_LOCATION(&attr, MX51_WDOG1_BASE, MX51_WDOG_SIZE, 0, hwi_find_as(MX51_WDOG1_BASE, 1));
		hwi_off = hwidev_add_timer("wdog", &attr,  HWI_NULL_OFF);
		ASSERT(hwi_off != HWI_NULL_OFF);
		hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_kick);		
	}
		
}


__SRCVERSION("$URL$ $Rev$");
