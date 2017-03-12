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







#ifndef ARM_DBPXA270_H_INCLUDED
#define ARM_DBPXA270_H_INCLUDED

/*
 * PXA270 Development Platform
 */

/* Memory Map */
#define DBPXA270_BOOTROM_BASE       PXA270_SCS0
#define DBPXA270_ALTROM_BASE        PXA270_SCS1
#define DBPXA270_SDRAM_BASE         PXA270_SDRAM0

#define DBPXA270_REG_BASE           PXA270_SCS2
#define DBPXA270_REG_LEN            (0x100)

/* Platform Registers, offset from DBPXA270_REG_BASE */
#define DBPXA270_LEDDAT1            (0x10)
#define DBPXA270_LEDDAT2            (0x14)
#define DBPXA270_LEDCTRL            (0x40)
#define DBPXA270_GPSWR              (0x60)
#define DBPXA270_MSCWR1             (0x80)
#define DBPXA270_MSCWR2             (0x84)
#define DBPXA270_MSCRD              (0x90)
#define DBPXA270_INTMSKENA          (0xc0)
#define DBPXA270_INTSETCLR          (0xd0) 
#define DBPXA270_PCMCIA0            (0xe0)
#define DBPXA270_PCMCIA1            (0xe4)
#define DBPXA270_REVID              (0xf0)

/* Platform Misc Write Register 1 bit definitions */
#define DBPXA270_MSCWR1_CAMERA_ON   (0x8000)
#define DBPXA270_MSCWR1_CAMERA_SEL  (0x4000)
#define DBPXA270_MSCWR1_LCD_CTL     (0x2000)
#define DBPXA270_MSCWR1_MS_ON       (0x1000)
#define DBPXA270_MSCWR1_MMC_ON      (0x0800)
#define DBPXA270_MSCWR1_MS_SEL      (0x0400)
#define DBPXA270_MSCWR1_BB_SEL      (0x0200)
#define DBPXA270_MSCWR1_nBT_OFF     (0x0100)
#define DBPXA270_MSCWR1_BTDTR       (0x0080)
#define DBPXA270_MSCWR1_IRDA_FIR    (0x0010)
#define DBPXA270_MSCWR1_GREENLED    (0x0008)
#define DBPXA270_MSCWR1_PDC_CTL     (0x0004)
#define DBPXA270_MSCWR1_MTR_ON      (0x0002)
#define DBPXA270_MSCWR1_SYSRESET    (0x0001)

/*
 * PXA270 Development Platform Configuration
 */

/* GPIO settings (Table 33, Platform User's Guide */
#define DBPXA270_GPSR0          (0x00008004) 
#define DBPXA270_GPSR1          (0x00020080)
#define DBPXA270_GPSR2          (0x16C14000)
#define DBPXA270_GPSR3          (0x0003E000)

#define DBPXA270_GPCR0          (0x0)
#define DBPXA270_GPCR1          (0x00000380)
#define DBPXA270_GPCR2          (0x0)
#define DBPXA270_GPCR3          (0x0)

#define DBPXA270_GPLR0          (0x0)
#define DBPXA270_GPLR1          (0x0)
#define DBPXA270_GPLR2          (0x0)
#define DBPXA270_GPLR3          (0x0)

#define DBPXA270_GPDR0          (0xCFE3BDE4)
#define DBPXA270_GPDR1          (0x003FAB81)
#define DBPXA270_GPDR2          (0x1EC3FC00)
#define DBPXA270_GPDR3          (0x018FFE8F)

#define DBPXA270_GAFR0_L        (0x84400000)
#define DBPXA270_GAFR0_U        (0xA5000510)
#define DBPXA270_GAFR1_L        (0x099A9558)
#define DBPXA270_GAFR1_U        (0x0005A1AA)
#define DBPXA270_GAFR2_L        (0x60000000)
#define DBPXA270_GAFR2_U        (0x00000802)
#define DBPXA270_GAFR3_L        (0x00000000)
#define DBPXA270_GAFR3_U        (0x00000000)

/* Memory configuration */
#define DBPXA270_MSC0_value	    (0x7ff0b8f2)
#define DBPXA270_MSC1_value     (0x0000A691)
#define DBPXA270_MSC2_value     (0x0000B884)

#define DBPXA270_MECR_value     (0x00000001)
#define DBPXA270_MCMEM0_value   (0x0001C391)
#define DBPXA270_MCMEM1_value   (0x0001C391)
#define DBPXA270_MCATT0_value   (0x0001C391)
#define DBPXA270_MCATT1_value   (0x0001C391)
#define DBPXA270_MCIO0_value    (0x0001C391)
#define DBPXA270_MCIO1_value    (0x0001C391)
#define DBPXA270_FLYCNFG_value  (0x00010001)
#define DBPXA270_MDMRSLP_value  (0x0000C008)
#define DBPXA270_SXCNFG_value   (0x40044004)    /* Default value at boot up */
#define DBPXA270_MDCNFG_value   (0x000009C8)    /* SDRAM Config Reg (Non-MCP Version) */
#define DBPXA270_MDREFR_value   (0x00000018)
#define DBPXA270_MDMRS_value    (0x00000000)    /* SDRAM Mode Reg Set Config Reg */
#endif

/* __SRCVERSION("dbpxa270dp.h $Rev: 657836 $"); */
