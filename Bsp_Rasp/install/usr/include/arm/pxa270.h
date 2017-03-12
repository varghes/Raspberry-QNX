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






#ifndef ARM_PXA270_H_INCLUDED
#define ARM_PXA270_H_INCLUDED

#define PXA270_SCS0             (0x00000000)
#define PXA270_SCS1             (0x04000000)
#define PXA270_SCS2             (0x08000000)
#define PXA270_SCS3             (0x0C000000)
#define PXA270_SCS4             (0x10000000)
#define PXA270_SCS5             (0x14000000)
#define PXA270_SDRAM0           (0xA0000000)

/*
 * PXA270 Registers, additional to PXA250
 */

/* GPIO (offset from PXA250_GPIO_BASE) */
#define PXA270_GAFR2_U		(0x068)   /* GPIO Alternate Function Select Register GPIO<95-80> */
#define PXA270_GAFR3_L          (0x06C)  /* Bits 111:96 */
#define PXA270_GAFR3_U          (0x070)  /* Bits 127:112 */
#define PXA270_GPLR3            (0x100)
#define PXA270_GPDR3            (0x10c)
#define PXA270_GPSR3            (0x118)
#define PXA270_GPCR3            (0x124)
#define PXA270_GRER3            (0x130)
#define PXA270_GFER3            (0x13c)
#define PXA270_GEDR3            (0x148)

/* Power Manager */
#define PXA270_PM_BASE          (0x40f00000)

#define PXA270_PSSR             (0x04)

#define PXA270_PWER				(0x0C) /* Power Wake-up Enable Register */
#define PXA270_PEDR 				(0x18) /* Power Edge Detect Register */

#define PXA270_PSSR_PH          (0x10)
#define PXA270_PSSR_RDH         (0x20)

/* Memory Controller (offset from PXA250_MEM_CTLR_BASE) */
#define PXA270_FLYCNFG          (0x20)

/* Memory Controller bit definitions */
#define PXA270_MDREFR_K0DB4     (0x20000000)    /* Sync Static Clock 0 divide by 4 control/status */
#define PXA270_MDREFR_K2FREE    (0x02000000)    /* Set to force SDCLK[2] to be free running */
#define PXA270_MDREFR_K1FREE    (0x01000000)    /* Set to force SDCLK[1] to be free running */
#define PXA270_MDREFR_K0FREE    (0x00800000)    /* Set to force SDCLK[0] to be free running */
#define PXA270_MDREFR_SLFRSH    (0x00400000)    /* Self Refresh Control Status bit */
#define PXA270_MDREFR_APD       (0x00100000)    /* Auto Power Down bit */
#define PXA270_MDREFR_K2DB2     (0x00080000)    /* SDRAM clock pin 2 divide by 2 control/status */
#define PXA270_MDREFR_K1DB2     (0x00020000)    /* SDRAM clock pin 1 divide by 2 control/status */
#define PXA270_MDREFR_K1RUN     (0x00010000)    /* SDRAM clock pin 1 run/control status */
#define PXA270_MDREFR_E1PIN     (0x00008000)    /* SDRAM clock Enable pin 1 level control/status */
#define PXA270_MDREFR_K0DB2     (0x00004000)    /* Sync Static Memory Clock divide by 2 control/status */
#define PXA270_MDREFR_K0RUN     (0x00002000)    /* Sync Static Memory Clock Pin 0 */

#define PXA270_MDCNFG_DE0       (0x00000001)    /* SDRAM enable bit for partition 0 */
#define PXA270_MDCNFG_DE1       (0x00000002)    /* SDRAM enable bit for partition 1 */
#define PXA270_MDCNFG_DE2       (0x00010000)    /* SDRAM enable bit for partition 2 */
#define PXA270_MDCNFG_DE3       (0x00020000)    /* SDRAM enable bit for partition 3 */
#define PXA270_MDCNFG_DWID0     (0x00000004)    /* SDRAM bus width (clear = 32 bits, set = 16 bits) */

/* Interrupt Controller (offset from PXA250_INTR_BASE) */
#define PXA270_ICHP             (0x18)
#define PXA270_IPR(x)           (0x1c + ((x) << 2))         /* IPR0-IPR31 */
#define PXA270_IPRx(x)          (0xb0 + (((x) - 32) << 2))  /* IPR32-IPR39 */
#define PXA270_ICIP2            (0x9c)
#define PXA270_ICMR2            (0xa0)
#define PXA270_ICLR2            (0xa4)
#define PXA270_ICFP2            (0xa8)
#define PXA270_ICPR2            (0xac)

#define PXA270_IPR_VALID        (0x80000000)

/* UHC: USB Host Controller (OHCI) register definitions */
#define PXA270_UHC_BASE	(0x4C000000)
#define PXA270_UHCREV		(0x000000) /* UHC HCI Spec Revision */
#define PXA270_UHCHCON	(0x000004) /* UHC Host Control Register */
#define PXA270_UHCCOMS	(0x000008) /* UHC Command Status Register */
#define PXA270_UHCINTS		(0x00000C) /* UHC Interrupt Status Register */
#define PXA270_UHCINTE		(0x000010) /* UHC Interrupt Enable */
#define PXA270_UHCINTD	(0x000014) /* UHC Interrupt Disable */
#define PXA270_UHCHCCA	(0x000018) /* UHC Host Controller Comm. Area */
#define PXA270_UHCPCED	(0x00001C) /* UHC Period Current Endpt Descr */
#define PXA270_UHCCHED	(0x000020) /* UHC Control Head Endpt Descr */
#define PXA270_UHCCCED	(0x000024) /* UHC Control Current Endpt Descr */
#define PXA270_UHCBHED	(0x000028) /* UHC Bulk Head Endpt Descr */
#define PXA270_UHCBCED	(0x00002C) /* UHC Bulk Current Endpt Descr */
#define PXA270_UHCDHEAD	(0x000030) /* UHC Done Head */
#define PXA270_UHCFMI		(0x000034) /* UHC Frame Interval */
#define PXA270_UHCFMR		(0x000038) /* UHC Frame Remaining */
#define PXA270_UHCFMN		(0x00003C) /* UHC Frame Number */
#define PXA270_UHCPERS	(0x000040) /* UHC Periodic Start */
#define PXA270_UHCLS		(0x000044) /* UHC Low Speed Threshold */
#define PXA270_UHCRHDA	(0x000048) /* UHC Root Hub Descriptor A */
#define PXA270_UHCRHDB	(0x00004C) /* UHC Root Hub Descriptor B */
#define PXA270_UHCRHS		(0x000050) /* UHC Root Hub Status */
#define PXA270_UHCRHPS1	(0x000054) /* UHC Root Hub Port 1 Status */
#define PXA270_UHCRHPS2	(0x000058) /* UHC Root Hub Port 2 Status */
#define PXA270_UHCRHPS3	(0x00005C) /* UHC Root Hub Port 3 Status */
#define PXA270_UHCHR		(0x000064) /* UHC Reset Register */
#define PXA270_UHCHIE		(0x000068) /* UHC Interrupt Enable Register*/

#endif

/* __SRCVERSION("pxa270.h $Rev: 657836 $"); */
