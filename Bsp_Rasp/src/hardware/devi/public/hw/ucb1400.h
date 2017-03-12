/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008, QNX Software Systems.  
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


#include <sys/srcversion.h>






/*
 *  ucb1400.h
 *
 * 
 *  Philips audio codec with touch screen controller and power management monitor
 * 
 */

#ifndef __HW_UCB1400_H_INCLUDED
#define __HW_UCB1400_H_INCLUDED

#define UCB1400_RESET            (0x00)
#define UCB1400_CODEC_ID         (0x02a0)

#define UCB1400_RESET_ID9        (0x0200)    /* Always 1 (20-bit ADC resolution supported) */
#define UCB1400_RESET_ID7        (0x0080)    /* Always 1 (20-bit DAC resolution supported) */
#define UCB1400_RESET_ID5        (0x0020)    /* Always 1 (Loudness (bass boost) supported) */
			

#define UBC1400_MASTER_VOL       (0x02)

#define UBC1400_MASTER_VOL_MM    (0x8000)    /* Master mute                                */
#define UBC1400_MASTER_VOL_ML    (0x3f00)    /* Left channel attenuation in 1.5 dB step    */
#define UBC1400_MASTER_VOL_MR    (0x003f)    /* Right channel attenuation in 1.5 dB step   */


#define UCB1400_MIC_VOL          (0x0e)

#define UCB1400_MIC_VOL_20dB     (0x0040)    /* 20 dB burst                                */


#define UCB1400_RECORD_SEL       (0x1a)

#define UCB1400_RECORD_SEL_SL    (0x0700)    /* Left record source (000 - MIC, 100 - Line in L) */                       
#define UCB1400_RECORD_SEL_SR    (0x0007)    /* Right record source (000 - copy Left, 100 - Line in R) */


#define UCB1400_RECORD_GAIN      (0x1c)    

#define UCB1400_RECORD_GAIN_GL   (0x0f00)    /* Left channel record gain in steps of 1.5 dB */
#define UCB1400_RECORD_GAIN_GR   (0x000f)    /* Right channel record gain in steps of 1.5 dB */


#define UCB1400_GEN_PURPOSE      (0x20)

#define UCB1400_GEN_PURPOSE_LPBK (0x0080)    /* ADC/DAC loopback mode                       */


#define UCB1400_PWR_DOWN         (0x26)

#define UCB1400_PWR_DOWN_PR5     (0x2000)    /* Internal clock disable                      */
#define UCB1400_PWR_DOWN_PR4     (0x1000)    /* AC-link power down                          */
#define UCB1400_PWR_DOWN_PR3     (0x0800)    /* Audio V-ref power-down                      */
#define UCB1400_PWR_DOWN_PR1     (0x0200)    /* Audio DAC & output path power-down          */
#define UCB1400_PWR_DOWN_PR0     (0x0100)    /* Audio ADC & input path power-down           */
#define UCB1400_PWR_DOWN_REF     (0x0008)    /* Audio V-ref up to nominal level             */
#define UCB1400_PWR_DOWN_DAC     (0x0002)    /* Audio DAC section ready to accept data      */
#define UCB1400_PWR_DOWN_ADC     (0x0001)    /* Audio ADC section ready to transmit data    */


#define UCB1400_EXT_AUDIO        (0x28)

#define UCB1400_EXT_AUDIO_ID     (0xc000)    /* Always 00 - this is a primary codec         */
#define UCB1400_EXT_AUDIO_VRA    (0x0001)    /* Always 1 (variable ratePCM Audio supportrd  */


#define UCB1400_EXT_AUD_STAT     (0x2a)

#define UCB1400_EXT_AUD_STAT_VRA (0x0001)    /* 1 enables variable rate audio mode          */


#define UCB1400_SAMPL_RATE_DAC   (0x2c)


#define UCB1400_SAMPL_RATE_ADC   (0x32)


#define UCB1400_IO_DATA          (0x5a)

#define UCB1400_IO_DATA_PINS     (0x03ff)    /* Actual state of all IO pins                 */


#define UCB1400_IO_DIRECT        (0x5c)

#define UCB1400_IO_DIRECT_PINS   (0x03ff)    /* Each 1 means that PIN is output             */


#define UCB1400_POSITIV_INT      (0x5e)

#define UCB1400_POSITIV_INT_OVLP (0x8000)    /* 1 - rising edge interrupt of OVFL signal is enabled */
#define UCB1400_POSITIV_INT_CLPP (0x4000)    /* 1 - rising edge interrupt of CLIP signal is enabled */
#define UCB1400_POSITIV_INT_TMXP (0x2000)    /* 1 - rising edge interrupt of TSMX signal is enabled */
#define UCB1400_POSITIV_INT_TPXP (0x1000)    /* 1 - rising edge interrupt of TSPX signal is enabled */
#define UCB1400_POSITIV_INT_ADCP (0x0800)    /* 1 - rising edge interrupt of ADC Ready  signal is enabled */
#define UCB1400_POSITIV_INT_IOP  (0x03ff)    /* 1 - rising edge interrupt of associated IO pin is enabled */


#define UCB1400_NEGATIV_INT      (0x60)

#define UCB1400_NEGATIV_INT_OVLN (0x8000)    /* 1 - falling edge interrupt of OVFL signal is enabled */
#define UCB1400_NEGATIV_INT_CLPN (0x4000)    /* 1 - falling edge interrupt of CLIP signal is enabled */
#define UCB1400_NEGATIV_INT_TMXN (0x2000)    /* 1 - falling edge interrupt of TSMX signal is enabled */
#define UCB1400_NEGATIV_INT_TPXN (0x1000)    /* 1 - falling edge interrupt of TSPX signal is enabled */
#define UCB1400_NEGATIV_INT_ADCN (0x0800)    /* 1 - falling edge interrupt of ADCP signal is enabled */
#define UCB1400_NEGATIV_INT_ION  (0x03ff)    /* 1 - falling edge interrupt of associated IO pin is enabled */


#define UCB1400_CLEAR_STAT       (0x62)

#define UCB1400_CLEAR_STAT_OVLS (0x8000)    /* Read - returns OVFL status, Clear by writing 1 this bit */
#define UCB1400_CLEAR_STAT_CLPS (0x4000)    /* Read - returns CLIP status, Clear by writing 1 this bit */
#define UCB1400_CLEAR_STAT_TMXS (0x2000)    /* Read - returns TSMX status, Clear by writing 1 this bit */
#define UCB1400_CLEAR_STAT_TPXS (0x1000)    /* Read - returns TSPX status, Clear by writing 1 this bit */
#define UCB1400_CLEAR_STAT_ADCS (0x0800)    /* Read - returns ADC  status, Clear by writing 1 this bit */
#define UCB1400_CLEAR_STAT_IOS  (0x03ff)    /* Read - returns associated IO pin status, Clear by writing 1 this bit */


#define UCB1400_TOUCH_CTRL       (0x64)

#define UCB1400_TOUCH_CTRL_MX    (0x2000)   /* State of TSMX pin                           */             
#define UCB1400_TOUCH_CTRL_PX    (0x1000)   /* State of TSPX pin                           */             
#define UCB1400_TOUCH_CTRL_BIAS  (0x0800)   /* 1 if touch screen bias circuity is activated*/
#define UCB1400_TOUCH_CTRL_HYSD  (0x0400)   /* 1 if hysteresis is deactivated              */
#define UCB1400_TOUCH_CTRL_MODE  (0x0300)   /* Operation mode (00  interrupt, 01 - pressure*/
                                            /* measurement , 1x - position measurement     */
#define UCB1400_TOUCH_CTRL_PRESS (0x0100)   /* Press measurement                           */
#define UCB1400_TOUCH_CTRL_POS   (0x0200)   /* Position measurement                        */
#define UCB1400_TOUCH_CTRL_PYG   (0x0080)   /* 1 if TSPY is grounded                       */             
#define UCB1400_TOUCH_CTRL_MYG   (0x0040)   /* 1 if TSMY is grounded                       */             
#define UCB1400_TOUCH_CTRL_PXG   (0x0020)   /* 1 if TSPX is grounded                       */             
#define UCB1400_TOUCH_CTRL_MXG   (0x0010)   /* 1 if TSMX is grounded                       */             
#define UCB1400_TOUCH_CTRL_PYP   (0x0008)   /* 1 if TSPY is powered                        */             
#define UCB1400_TOUCH_CTRL_MYP   (0x0004)   /* 1 if TSMY is powered                        */             
#define UCB1400_TOUCH_CTRL_PXP   (0x0002)   /* 1 if TSPX is powered                        */             
#define UCB1400_TOUCH_CTRL_MXP   (0x0001)   /* 1 if TSMX is powered                        */             

#define UCB1400_ADC_CTRL         (0x66)

#define UCB1400_ADC_CTRL_AE      (0x8000)   /* 1 f ADC is activated                        */
#define UCB1400_ADC_CTRL_AS      (0x0080)   /* Writing 1 starts ADC conversion sequence    */
#define UCB1400_ADC_CTRL_AI      (0x001c)   /* ADC input selection                         */
#define UCB1400_ADC_CTRL_ASE     (0x0001)   /* If 1,ADC is armed by the AS bit and started */
                                            /* by the rising edge of ADCSYNC pin           */
#define UCB1400_ADC_CTRL_TSPX    (0x0000)   /* TSPX mesurement                             */
#define UCB1400_ADC_CTRL_TSMX    (0x0004)   /* TSMX mesurement                             */
#define UCB1400_ADC_CTRL_TSPY    (0x0008)   /* TSPX mesurement                             */
#define UCB1400_ADC_CTRL_TSMY    (0x000c)   /* TSMY mesurement                             */

#define UCB1400_ADC_DATA         (0x68)

#define UCB1400_ADC_DATA_ADV     (0x8000)   /* ADC conversion is completed                 */
#define UCB1400_ADC_DATA_VALUE(x) (x & 0x03ff) /* Real data in ADC register                */


#define UCB1400_FEATURE          (0x6a)

#define UCB1400_FEATURE_SMT      (0x8000)   /* Always 0                                    */
#define UCB1400_FEATURE_SUEV     (0x6000)   /* Always 0                                    */
#define UCB1400_FEATURE_BB       (0x1800)   /* Bass boost                                  */
#define UCB1400_FEATURE_TR       (0x0600)   /* Treble boost                                */
#define UCB1400_FEATURE_M        (0x0180)   /* Bass boost                                  */
#define UCB1400_FEATURE_HPEN     (0x0040)   /* 1 if headphone driver is enabled            */
#define UCB1400_FEATURE_DE       (0x0020)   /* 1 if de-emphasis  is enabled                */
#define UCB1400_FEATURE_DC       (0x0010)   /* 1 if DC filter is enabled                   */
#define UCB1400_FEATURE_HIPS     (0x0008)   /* 1 activates high-pass filter in decimator   */
#define UCB1400_FEATURE_GIEN     (0x0004)   /* 1 if GPIO_INT is enabled                    */
#define UCB1400_FEATURE_OVFL     (0x0001)   /* Reads or clears ADC overflow status         */


#define UCB1400_FEATURE_2        (0x6c)

#define UCB1400_FEATURE_2_SP     (0x0030)   /* Smart Low Power modes                       */
#define UCB1400_FEATURE_2_EV     (0x0007)   /* Always 0                                    */


#define UCB1400_TEST_CTRL        (0x6e)

#define UCB1400_TEST_TM          (0x007f)   /* Reserved for test purpose                   */


#define UCB1400_EXTRA_INT        (0x70)

#define UCB1400_EXTRA_CLPL       (0x8000)   /* Status of CLIPL signal; write 1 to clear    */
#define UCB1400_EXTRA_CLPR       (0x4000)   /* Status of CLIPR signal; write 1 to clear    */
#define UCB1400_EXTRA_CLPG       (0x2000)   /* Status of CLIPGND signal; write 1 to clear  */


#define UCB1400_VENDOR_ID1       (0x7c)
#define UCB1400_ID1              (0x5053)   /* Real vendor ID1 value expected to be in reg 0x7c */

#define UCB1400_VENDOR_ID2       (0x7e)
#define UCB1400_ID2              (0x4304)   /* Real vendor ID2 value expected to be in reg 0x7e */

#endif /*  __HW_UCB1400_H_INCLUDED  */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/public/hw/ucb1400.h $ $Rev: 657836 $" )
