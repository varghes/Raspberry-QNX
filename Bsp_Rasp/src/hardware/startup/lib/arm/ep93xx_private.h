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
 * Cirrus Logic EP93xx
 * (ARM 920T based SOC)
*/

#ifndef	_EP93xx_PRIVATE_H_
#define	_EP93xx_PRIVATE_H_

#include <arm/ep93xx.h>

/*
 * The order of definition follows the EP93xx Internal Register Map starting on
 * pg 217 (table 2-8) of the Cirrus Logic EP93xx User's Guide. The values are
 * used by startup to populate the HWINFO section and is for startup use only.
 * 
 * This file is not public because drivers should not be using the defines for
 * base addresses in this file. 
 * The HWINFO section of the syspage (and/or command line options) should be
 * used to obtain the required device attributes
*/


/*
 * ------------------------------------------------------------------------
 * DMA Controllers
 * ------------------------------------------------------------------------
*/
#define EP93xx_DMA_M2P_CHAN0	0x80000000U
#define EP93xx_DMA_M2P_CHAN1	0x80000040U
#define EP93xx_DMA_M2P_CHAN2	0x80000080U
#define EP93xx_DMA_M2P_CHAN3	0x800000C0U
#define EP93xx_DMA_M2P_CHAN4	0x80000240U
#define EP93xx_DMA_M2P_CHAN5	0x80000200U
#define EP93xx_DMA_M2P_CHAN6	0x800002C0U
#define EP93xx_DMA_M2P_CHAN7	0x80000280U
#define EP93xx_DMA_M2P_CHAN8	0x80000340U
#define EP93xx_DMA_M2P_CHAN9	0x80000300U

/*
 * ------------------------------------------------------------------------
 * Ethernet Controller
 * ------------------------------------------------------------------------
*/
#define EP93xx_ENET_CTRL_BASE		0x80010000U
#define EP93xx_ENET_CTRL_SIZE		0x5100U

/*
 * ------------------------------------------------------------------------
 * USB Controller
 * ------------------------------------------------------------------------
*/
#define EP93xx_USB_CTRL_BASE		0x80020000U
#define EP93xx_USB_CTRL_SIZE		0x88U

/*
 * ------------------------------------------------------------------------
 * RASTER Controller
 * ------------------------------------------------------------------------
*/
#define EP93xx_RASTER_CTRL_BASE		0x80020000U
#define EP93xx_RASTER_CTRL_SIZE		0x800U

/*
 * ------------------------------------------------------------------------
 * SDRAM Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_SDRAM_CTRL_BASE		0x80060000U
#define	EP93xx_SDRAM_CTRL_SIZE		0x20U

/*
 * ------------------------------------------------------------------------
 * SMC (and PCMCIA) Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_SMC_CTRL_BASE		0x80080000U
#define	EP93xx_SMC_CTRL_SIZE		0x44U

/*
 * ------------------------------------------------------------------------
 * BOOT ROM
 * ------------------------------------------------------------------------
*/
#define	EP93xx_BOOT_ROM_BASE		0x80090000U
#define	EP93xx_BOOT_ROM_SIZE		0x4000U

/*
 * ------------------------------------------------------------------------
 * IDE Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_IDE_CTRL_BASE		0x800A0000U
#define	EP93xx_IDE_CTRL_SIZE		0x38U

/*
 * ------------------------------------------------------------------------
 * Interrupt Controller (VIC1 and VIC2)
 * ------------------------------------------------------------------------
*/
#define	EP93xx_VIC_CTRL1_BASE		0x800B0000U
#define	EP93xx_VIC_CTRL1_SIZE		0x1000U

#define	EP93xx_VIC_CTRL2_BASE		0x800C0000U
#define	EP93xx_VIC_CTRL2_SIZE		0x1000U

/*
 * ------------------------------------------------------------------------
 * Clock/Timer Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_TIMER1_BASE		0x80810000U
#define	EP93xx_TIMER1_SIZE		0x10

#define	EP93xx_TIMER2_BASE		0x80810020U
#define	EP93xx_TIMER2_SIZE		0x10

#define	EP93xx_TIMER3_BASE		0x80810080U
#define	EP93xx_TIMER3_SIZE		0x10

/*
 * ------------------------------------------------------------------------
 * I2S Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_I2S_CTRL_BASE		0x80820000U
#define	EP93xx_I2S_CTRL_SIZE		0x70U

/*
 * ------------------------------------------------------------------------
 * GPIO Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_GPIO_CTRL_BASE		0x80840000U
#define	EP93xx_GPIO_CTRL_SIZE		0xCCU

/*
 * ------------------------------------------------------------------------
 * AC97 Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_AC97_CTRL_BASE		0x80880000U
#define	EP93xx_AC97_CTRL_SIZE		0xACU

/*
 * ------------------------------------------------------------------------
 * SPI Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_SPI_CTRL_BASE		0x808A0000U
#define	EP93xx_SPI_CTRL_SIZE		0x18U

/*
 * ------------------------------------------------------------------------
 * IrDA Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_IrDA_CTRL_BASE		0x808B0000U
#define	EP93xx_IrDA_CTRL_SIZE		0x8CU

/*
 * ------------------------------------------------------------------------
 * UART Controllers
 * ------------------------------------------------------------------------
*/
#define EP93xx_UART1_BASE		0x808C0000U
#define EP93xx_UART1_SIZE		0x220

#define EP93xx_UART2_BASE		0x808D0000U
#define EP93xx_UART2_SIZE		0x2C

#define EP93xx_UART3_BASE		0x808E0000U
#define EP93xx_UART3_SIZE		0x220

/*
 * ------------------------------------------------------------------------
 * KEYPAD Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_KEYPAD_CTRL_BASE		0x808F0000U
#define	EP93xx_KEYPAD_CTRL_SIZE		0xCU

/*
 * ------------------------------------------------------------------------
 * TOUCH Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_TOUCH_CTRL_BASE		0x80900000U
#define	EP93xx_TOUCH_CTRL_SIZE		0x28U

/*
 * ------------------------------------------------------------------------
 * PWM Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_PWM_CTRL_BASE		0x80910000U
#define	EP93xx_PWM_CTRL_SIZE		0x34U

/*
 * ------------------------------------------------------------------------
 * RTC Controller
 * ------------------------------------------------------------------------
*/
#define	EP93xx_RTC_CTRL_BASE		0x80920000U
#define	EP93xx_RTC_CTRL_SIZE		0x9CU

/*
 * ------------------------------------------------------------------------
 * System Controller
 * ------------------------------------------------------------------------
*/

#define	EP93xx_SYSCTRL_BASE			0x80930000U
#define	EP93xx_SYSCTRL_SIZE			0xC4U

/*
 * ------------------------------------------------------------------------
 * WATCHDOG Timer
 * ------------------------------------------------------------------------
*/
#define	EP93xx_WDOG_BASE		0x80940000U
#define	EP93xx_WDOG_SIZE		0x8U


/*
 * prototypes for startup use only
*/
/* base clock frequencies based on system config */ 
unsigned long ep93xx_get_fclk(void);
unsigned long ep93xx_get_hclk(void);
unsigned long ep93xx_get_pclk(void);
unsigned long ep93xx_get_cpuclk(void);
unsigned long ep93xx_get_uartclk(void);
unsigned long ep93xx_get_sspclk(void);
unsigned long ep93xx_get_pwmclk(void);
unsigned long ep93xx_get_aacclk(void);
unsigned long ep93xx_get_timerclk1(void);
unsigned long ep93xx_get_timerclk2(void);
unsigned long ep93xx_get_timerclk3(void);
unsigned long ep93xx_get_rtcclk(void);
unsigned long ep93xx_get_wdogclk(void);
unsigned long ep93xx_get_usbclk(void);
unsigned long ep93xx_get_vclk(void);
unsigned long ep93xx_get_mirclk(void);
unsigned long ep93xx_get_mclk(void);
unsigned long ep93xx_get_sclk(void);
unsigned long ep93xx_get_lrclk(void);
unsigned long ep93xx_get_adcclk(void);
unsigned long ep93xx_get_keyclk(void);
unsigned long ep93xx_get_touchclk(void);
unsigned long ep93xx_get_filtclk(void);

unsigned ep93xx_get_resetflags(void);
unsigned ep93xx_clr_resetflags(void);
unsigned ep93xx_is_wdogreset(void);
unsigned ep93xx_is_pwronreset(void);
unsigned ep93xx_is_testreset(void);
unsigned ep93xx_is_rtsoreset(void);
unsigned ep93xx_is_swreset(void);
const char *ep93xx_get_resettype_str(void);
unsigned ep93xx_get_chipid(void);
unsigned ep93xx_get_chiprev(void);
unsigned ep93xx_get_chipver(void);
unsigned ep93xx_get_chipman(void);
void ep93xx_clr_wdog_int(void);
void ep93xx_clr_bootmode(void);
unsigned ep93xx_get_bootmode(void);
unsigned ep93xx_is_bootmode_external(void);
unsigned ep93xx_is_bootmode_serial(void);
unsigned ep93xx_is_bootmode_synchronous(void);
const char *ep93xx_get_bootmode_str(void);


#endif	/* _EP93xx_PRIVATE_H_ */
