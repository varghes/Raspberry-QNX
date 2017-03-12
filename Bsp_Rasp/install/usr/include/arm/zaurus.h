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




/*
 * Compaq iPaq Specifics
 *

 *
 */
#ifndef ARM_ZAURUS_H_INCLUDED
#define ARM_ZAURUS_H_INCLUDED

#include <sys/neutrino.h>
#include <hw/sysinfo.h>
#include <hw/inout.h>
#include <arm/sa1100.h>


#define ZAURUS_ON_KEY             SA1100_GPIO_0
#define ZAURUS_AC_IN              SA1100_GPIO_1
#define ZAURUS_STEREO_TXD         SA1100_GPIO_10
#define ZAURUS_SD_IRQ             SA1100_GPIO_11
#define ZAURUS_STEREO_SCLK        SA1100_GPIO_12
#define ZAURUS_STEREO_SFRM        SA1100_GPIO_13
#define ZAURUS_CF_IRQ             SA1100_GPIO_14
#define ZAURUS_nREMOCON_IRQ       SA1100_GPIO_15
#define ZAURUS_TC35143_RESET      SA1100_GPIO_16
#define ZAURUS_MIC_ON             SA1100_GPIO_17
#define ZAURUS_nREMOCON_1k_ON     SA1100_GPIO_18
#define ZAURUS_STEREO_64FS_CLK    SA1100_GPIO_19
#define ZAURUS_CO                 SA1100_GPIO_20
#define ZAURUS_921MHZ             SA1100_GPIO_21
#define ZAURUS_CF_DETECT          SA1100_GPIO_22
#define ZAURUS_TC35143_IRQ        SA1100_GPIO_23
#define ZAURUS_WAKEUP             SA1100_GPIO_24
#define ZAURUS_GA_IRQ             SA1100_GPIO_25
#define ZAURUS_BAT_LOW            SA1100_GPIO_26
#define ZAURUS_32KHZ_OUT          SA1100_GPIO_27


/*
 Scoop is the ASIC that manages some GPIO's and the CF
*/
#define ZAURUS_SCOOP_BASE    0x40800000
#define ZAURUS_SCOOP_LEN     0x100 

#define ZAURUS_SCOOP_MCR     0x00
#define ZAURUS_SCOOP_CDR     0x04
#define ZAURUS_SCOOP_CSR     0x08
        #define ZAURUS_SCOOP_CSR_READY     0x02
        #define ZAURUS_SCOOP_CSR_nDETECT   0x04
        #define ZAURUS_SCOOP_CSR_WRPROT    0x08
        #define ZAURUS_SCOOP_CSR_BVD1      0x10
        #define ZAURUS_SCOOP_CSR_BVD2      0x20
        #define ZAURUS_SCOOP_CSR_n3V       0x40
        #define ZAURUS_SCOOP_CSR_n5V       0x80
#define ZAURUS_SCOOP_CPR     0x0C
        #define ZAURUS_SCOOP_CPR_33V       0x01
        #define ZAURUS_SCOOP_CPR_50V       0x02
        #define ZAURUS_SCOOP_CPR_OENA      0x80
#define ZAURUS_SCOOP_CCR     0x10
        #define ZAURUS_SCOOP_CCR_RESET     0x80
#define ZAURUS_SCOOP_IMR     0x18
#define ZAURUS_SCOOP_IRM     0x14
#define ZAURUS_SCOOP_ISR     0x1C
#define ZAURUS_SCOOP_GPCR    0x20
#define ZAURUS_SCOOP_GPWR    0x24
#define ZAURUS_SCOOP_GPRR    0x28



/* GPIO Definitions */
#define ZAURUS_SCOOP_IO_CHARGE_ON  (1<<1)
#define ZAURUS_SCOOP_IO_DIAG1      (1<<2)
#define ZAURUS_SCOOP_IO_DIAG2      (1<<3)
#define ZAURUS_SCOOP_IO_MUTEL      (1<<4)
#define ZAURUS_SCOOP_IO_MUTER      (1<<5)
#define ZAURUS_SCOOP_IO_5V_ON      (1<<6)
#define ZAURUS_SCOOP_IO_AMP_ON     (1<<7)
#define ZAURUS_SCOOP_IO_VPEN       (1<<8)
#define ZAURUS_SCOOP_IO_VOLCHG     (1<<9)

#define ZAURUS_SCOOP_IO_OUTPUTS \
( \
  ZAURUS_SCOOP_IO_MUTEL | \
  ZAURUS_SCOOP_IO_MUTER | \
  ZAURUS_SCOOP_IO_5V_ON | \
  ZAURUS_SCOOP_IO_AMP_ON | \
  ZAURUS_SCOOP_IO_VPEN \
)


/*
 * LoCoMo is the big ASIC that handles everything Scoop doesn't.
 */
#define ZAURUS_LOCOMO_BASE    0x40000000
#define ZAURUS_LOCOMO_LEN     0x100

#define ZAURUS_LOCOMO_TADC    0x30
#define ZAURUS_LOCOMO_TFT     0x38
#define ZAURUS_LOCOMO_GPD     0x90
#define ZAURUS_LOCOMO_GPE     0x94
#define ZAURUS_LOCOMO_GPL     0x98
#define ZAURUS_LOCOMO_GPO     0x9c
#define ZAURUS_LOCOMO_GRIE    0xa0
#define ZAURUS_LOCOMO_GFIE    0xa4
#define ZAURUS_LOCOMO_GIS     0xa8
#define ZAURUS_LOCOMO_GWE     0xac
#define ZAURUS_LOCOMO_GIE     0xb0
#define ZAURUS_LOCOMO_GIR     0xb4
#define ZAURUS_LOCOMO_ALC     0xc8
#define ZAURUS_LOCOMO_ALR     0xcc
#define ZAURUS_LOCOMO_DAC     0xe0

/* LoCoMo Register Bit Defines */

#define ZAURUS_LOCOMO_TADC_CLK9MHZ    (1<<4)
#define ZAURUS_LOCOMO_TADC_XON        (1<<7)

#define ZAURUS_LOCOMO_TFT_CRST        (1<<2)
#define ZAURUS_LOCOMO_TFT_CPSOUT      (1<<1)
#define ZAURUS_LOCOMO_TFT_CPSEN       (1<<0)

#define ZAURUS_LOCOMO_ALC_EN         (1<<15)
#define ZAURUS_LOCOMO_ALC_INV        (1<<14)
#define ZAURUS_LOCOMO_ALC_CNT_SETEN  (1<<11)
#define ZAURUS_LOCOMO_ALC_CNT_STOP   (1<<10)
#define ZAURUS_LOCOMO_ALC_HSYSEN     (1<<8)

#define ZAURUS_LOCOMO_DAC_SDAOEB     (1<<0)
#define ZAURUS_LOCOMO_DAC_SDA        (1<<1)
#define ZAURUS_LOCOMO_DAC_TEST       (1<<2)
#define ZAURUS_LOCOMO_DAC_SCLOEB     (1<<3)

/* GPIO Pin Defines / Locations */
#define ZAURUS_LOCOMO_GPIO_RTS           (1<<0)
#define ZAURUS_LOCOMO_GPIO_CTS           (1<<1)
#define ZAURUS_LOCOMO_GPIO_DSR           (1<<2)
#define ZAURUS_LOCOMO_GPIO_DTR           (1<<3)
#define ZAURUS_LOCOMO_GPIO_LCD_VSHA_ON   (1<<4)
#define ZAURUS_LOCOMO_GPIO_LCD_VSHD_ON   (1<<5)
#define ZAURUS_LOCOMO_GPIO_LCD_VEE_ON    (1<<6)
#define ZAURUS_LOCOMO_GPIO_LCD_MODE      (1<<7)
#define ZAURUS_LOCOMO_GPIO_DAC_ON        (1<<8)
#define ZAURUS_LOCOMO_GPIO_FL_VR         (1<<9)
#define ZAURUS_LOCOMO_GPIO_PCM_SDATA     (1<<10)
#define ZAURUS_LOCOMO_GPIO_PCM_SCK       (1<<11)
#define ZAURUS_LOCOMO_GPIO_PCM_SLOAD     (1<<12)
#define ZAURUS_LOCOMO_GPIO_nSD_DETECT    (1<<13)
#define ZAURUS_LOCOMO_GPIO_nSD_WP        (1<<14)
#define ZAURUS_LOCOMO_GPIO_SD_VCC_ON     (1<<15)

#define ZAURUS_LOCOMO_GPIO_INPUTS \
(\
    ZAURUS_LOCOMO_GPIO_CTS | \
    ZAURUS_LOCOMO_GPIO_DSR | \
    ZAURUS_LOCOMO_GPIO_nSD_DETECT | \
    ZAURUS_LOCOMO_GPIO_nSD_WP \
)


#endif

/* __SRCVERSION("zaurus.h $Rev: 657836 $"); */
