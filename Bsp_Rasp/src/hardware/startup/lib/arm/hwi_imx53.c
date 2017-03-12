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
#include "arm/hwinfo_imx53_proto.h"
#include <hw/hwinfo_imx53.h>
#include <drvr/hwinfo.h>                // for hwi support routines in libdrvr
#include <arm/mx53.h>

/*
 * Add IMX53 specific devices to the hardware info section of the syspage
*/

extern uint32_t uart_clock;

/* instead of hwibus_add_can() function use the following function to avoid empty tag creation 
that happens inside hwibus_add_can(), as facing difficulty to fill up that empty tag */

static unsigned imx53_hwibus_add_can(unsigned parent_hwi_off, hwiattr_can_t *attr);
	 
void hwi_imx53()
{
	unsigned hwi_bus_internal = 0;

	/* add I2C (unless directed not to) */
	{
		unsigned hwi_off;
		hwiattr_i2c_t attr = HWIATTR_I2C_T_INITIALIZER;              
		HWIATTR_I2C_SET_NUM_IRQ(&attr, 1);
             
		/* create i2c0 and set the clock source */
		HWIATTR_I2C_SET_LOCATION(&attr, MX53_I2C1_BASE, MX53_I2C_SIZE, 0, hwi_find_as(MX53_I2C1_BASE, 1));
		hwi_off = hwibus_add_i2c(hwi_bus_internal, &attr);
		ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_ivec(hwi_off, 0, MX53_I2C1_IRQ);

		/* create i2c1 and set the clock source */
		HWIATTR_I2C_SET_LOCATION(&attr, MX53_I2C2_BASE, MX53_I2C_SIZE, 0, hwi_find_as(MX53_I2C2_BASE, 1));
		hwi_off = hwibus_add_i2c(hwi_bus_internal, &attr);
		ASSERT(hwi_find_unit(hwi_off) == 1);
		hwitag_set_ivec(hwi_off, 0, MX53_I2C2_IRQ);
	}

	/* add  UART */
	{
		unsigned hwi_off;
		hwiattr_uart_t attr = HWIATTR_UART_T_INITIALIZER;
		struct hwi_inputclk clksrc = {.clk = uart_clock, .div = 16};				
		HWIATTR_UART_SET_NUM_IRQ(&attr, 1);
		HWIATTR_UART_SET_NUM_CLK(&attr, 1);

		/* create uart0 */
		HWIATTR_UART_SET_LOCATION(&attr, MX53_UART1_BASE, MX53_UART_SIZE, 0, hwi_find_as(MX53_UART1_BASE, 1));
		hwi_off = hwidev_add_uart(IMX53_HWI_UART, &attr, hwi_bus_internal);
		ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_ivec(hwi_off, 0, MX53_UART1_INTR);
		hwitag_set_inputclk(hwi_off, 0, &clksrc);

		/* historically the UART's were called 'mxl' so add these synonyms */
		hwi_add_synonym(hwi_find_device(IMX53_HWI_UART, 0), "sermxl");
	}

	/* add the FEC */
	{
		unsigned hwi_off;
		hwiattr_enet_t attr = HWIATTR_ENET_T_INITIALIZER;		
		HWIATTR_USB_SET_NUM_IRQ(&attr, 1);

		/* create eTSEC0 and set the PHY address */
		HWIATTR_ENET_SET_LOCATION(&attr, MX53_FEC_BASE, 0x4000, 0, hwi_find_as(MX53_FEC_BASE, 1));
		hwi_off = hwidev_add_enet(IMX53_HWI_LEGACY_FEC, &attr, hwi_bus_internal);
		ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_avail_ivec(hwi_off, 0, MX53_FEC_INTR);
	}


	/* add 1 USB controller */
	{
		unsigned hwi_off;
		hwiattr_usb_t attr = HWIATTR_USB_T_INITIALIZER;
		HWIATTR_USB_SET_NUM_IRQ(&attr, 1);               
		
		/* create usb0 */
		HWIATTR_USB_SET_LOCATION(&attr, MX53_USBH1_BASE, MX53_USB_SIZE, 0, hwi_find_as(MX53_USBH1_BASE, 1));
		hwi_off = hwibus_add_usb(hwi_bus_internal, &attr);
		ASSERT(hwi_off != HWI_NULL_OFF);
		hwitag_set_avail_ivec(hwi_off, 0, MX53_USBH1_INTR);
	}

	/* add 1 sdma controllers */
	{
		unsigned hwi_off;
		hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;
		HWIATTR_DMA_SET_NUM_IRQ(&attr, 1);

		/* create DMA controller 0 */
		HWIATTR_USB_SET_LOCATION(&attr, MX53_SDMA_BASE, MX53_SDMA_SIZE, 0, hwi_find_as(MX53_SDMA_BASE, 1));
		hwi_off = hwidev_add_dma(IMX53_HWI_DMA, &attr, hwi_bus_internal);
		ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_avail_ivec(hwi_off, 0, MX53_SDMA_INTRU);
	}

	/* add the SSI device */
	{
		unsigned hwi_off = hwidev_add(IMX53_HWI_SSI, hwi_devclass_NONE, HWI_NULL_OFF);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
		ASSERT(hwi_off != HWI_NULL_OFF);
		HWIATTR_SET_LOCATION(&attr, MX53_SSI1_BASE, MX53_SSI_SIZE, 0, hwi_find_as(MX53_SSI1_BASE, 1));
		hwitag_add_common(hwi_off, &attr);
	}

	/*add the SDRAM */
	{
		unsigned hwi_off = hwidev_add(IMX53_HWI_SDRAM, hwi_devclass_NONE, HWI_NULL_OFF);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
		ASSERT(hwi_off != HWI_NULL_OFF);
		HWIATTR_SET_LOCATION(&attr, MX53_SDRAM_BASE, MX53_SDRAM_SIZE, 0, hwi_find_as(MX53_SDRAM_BASE, 1));
		hwitag_add_common(hwi_off, &attr);
	}


	/* add the WATCHDOG device */
	{
		unsigned hwi_off;
		hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
		const struct hwi_inputclk clksrc_kick = {.clk = 10, .div = 1};	
		HWIATTR_TIMER_SET_NUM_CLK(&attr, 1);
		HWIATTR_TIMER_SET_LOCATION(&attr, MX53_WDOG1_BASE, MX53_WDOG_SIZE, 0, hwi_find_as(MX53_WDOG1_BASE, 1));
		hwi_off = hwidev_add_timer(IMX53_HWI_WDOG, &attr,  HWI_NULL_OFF);
		ASSERT(hwi_off != HWI_NULL_OFF);
		hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_kick);		
	}

	/* add CAN */
	{
		unsigned hwi_off;
		hwiattr_can_t attr = HWIATTR_CAN_T_INITIALIZER;
		HWIATTR_CAN_SET_NUM_IRQ(&attr, 1);
		HWIATTR_CAN_SET_NUM_MEMADDR(&attr, 1);
             
		/* create CAN0 */
		HWIATTR_CAN_SET_LOCATION(&attr, MX53_CAN1_PORT, MX53_CAN_SIZE, 0, hwi_find_as(MX53_CAN1_PORT, 1));		
		hwi_off = imx53_hwibus_add_can(hwi_bus_internal, &attr);		
		hwitag_add_location(hwi_off, MX53_CAN1_MEM, MX53_CAN_SIZE, 0, 0);		
		ASSERT(hwi_find_unit(hwi_off) == 0);
		hwitag_set_ivec(hwi_off, 0, MX53_CAN1_IRQ);

		/* create CAN1 */
		HWIATTR_CAN_SET_LOCATION(&attr, MX53_CAN2_PORT, MX53_CAN_SIZE, 0, hwi_find_as(MX53_CAN2_PORT, 1));		
		hwi_off = imx53_hwibus_add_can(hwi_bus_internal, &attr);
		hwitag_add_location(hwi_off, MX53_CAN2_MEM, MX53_CAN_SIZE, 0, 0);	
		ASSERT(hwi_find_unit(hwi_off) == 1);
		hwitag_set_ivec(hwi_off, 0, MX53_CAN2_IRQ);
	}

	/* add MLB */
	{	
		unsigned hwi_off = hwidev_add("mlb", hwi_devclass_NONE, HWI_NULL_OFF);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;	
		HWIATTR_SET_NUM_IRQ(&attr, 1);
		HWIATTR_SET_LOCATION(&attr, MX53_MLB_BASE, MX53_MLB_SIZE, 0, hwi_find_as(MX53_MLB_BASE, 1));
		hwitag_add_common(hwi_off, &attr);	
		ASSERT(hwi_off != HWI_NULL_OFF);	
		hwitag_set_ivec(hwi_off, 0, MX53_MLB_IRQ);		
	}

	/* add GPU: Graphics Processing Unit */
	{	
		/* create GPU2D */
		unsigned hwi_off = hwidev_add(IMX53_HWI_GPU_2D, hwi_devclass_NONE, HWI_NULL_OFF);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;			
		HWIATTR_SET_LOCATION(&attr, MX53_GPU2D_BASE, MX53_GPU2D_SIZE, 0, hwi_find_as(MX53_GPU2D_BASE, 1));
		hwitag_add_common(hwi_off, &attr);	
		ASSERT(hwi_off != HWI_NULL_OFF);		
		
		/* create GPU3D */
		hwi_off = hwidev_add(IMX53_HWI_GPU_3D, hwi_devclass_NONE, HWI_NULL_OFF);
		HWIATTR_SET_NUM_IRQ(&attr, 1);			
		HWIATTR_SET_LOCATION(&attr, MX53_GPU3D_BASE, MX53_GPU3D_SIZE, 0, hwi_find_as(MX53_GPU3D_BASE, 1));
		hwitag_add_common(hwi_off, &attr);	
		ASSERT(hwi_off != HWI_NULL_OFF);	
		hwitag_set_ivec(hwi_off, 0, MX53_GPU3D_IRQ);
	}

	/* add IPU: Image Processing Unit */
	{	
		unsigned hwi_off = hwidev_add(IMX53_HWI_IPU, hwi_devclass_NONE, HWI_NULL_OFF);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;			
		HWIATTR_SET_LOCATION(&attr, MX53_IPU_BASE, MX53_IPU_SIZE, 0, hwi_find_as(MX53_IPU_BASE, 1));
		hwitag_add_common(hwi_off, &attr);	
		ASSERT(hwi_off != HWI_NULL_OFF);		
	}

	/* add VPU: Video Processing Unit */
	{	
		unsigned hwi_off = hwidev_add(IMX53_HWI_VPU, hwi_devclass_NONE, HWI_NULL_OFF);
		hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;	
		HWIATTR_SET_NUM_IRQ(&attr, 1);
		HWIATTR_SET_LOCATION(&attr, MX53_VPU_BASE, MX53_VPU_SIZE, 0, hwi_find_as(MX53_VPU_BASE, 1));
		hwitag_add_common(hwi_off, &attr);	
		ASSERT(hwi_off != HWI_NULL_OFF);	
		hwitag_set_ivec(hwi_off, 0, MX53_VPU_IRQ);		
	}
		
}

static unsigned imx53_hwibus_add_can(unsigned parent_hwi_off, hwiattr_can_t *attr)
{
	unsigned hwi_off = hwibus_add(HWI_ITEM_BUS_CAN, parent_hwi_off);
	if ((hwi_off != HWI_NULL_OFF) && (attr != NULL))
	{
		unsigned i;
		hwitag_add_common(hwi_off, &attr->common);
		for (i=0; i<attr->num_clks; i++)
			hwitag_add_inputclk(hwi_off, 0, 1);		// make divisor 1, just in case ;)		
	}
	return hwi_off;
}
__SRCVERSION("$URL$ $Rev$");
