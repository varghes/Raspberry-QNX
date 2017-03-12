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
 * Intel PXA250TMDP Board
 *

 *
 */

#ifndef	ARM_PXA250TMDP_H_INCLUDED
#define	ARM_PXA250TMDP_H_INCLUDED

#define	PXA250TMDP_REG_BASE		0x0C000000		/* physical address */
#define	PXA250TMDP_REG_SIZE		0x00000A00		/* size of register space */

/*
 * Register offsets from PXA250TMDP_REG_BASE
 */

#define	PXA250TMDP_ROT_ENC1_STAT	0x100		/* Rotary Encoder 1 status */
#define	PXA250TMDP_ROT_ENC1_CTRL	0x110		/* Rotary Encoder 1 control */
#define	PXA250TMDP_ROT_ENC1_CNT	0x120		/* Rotary Encoder 1 count */
#define	PXA250TMDP_ROT_ENC2_STAT	0x200		/* Rotary Encoder 2 status */
#define	PXA250TMDP_ROT_ENC2_CTRL	0x210		/* Rotary Encoder 2 control */
#define	PXA250TMDP_ROT_ENC2_CNT	0x220		/* Rotary Encoder 2 count */
#define	PXA250TMDP_ID				0x300		/* ID register */
#define PXA250TMDP_REV				0x310		/* Revision Register */
#define	PXA250TMDP_INT_STAT		0x400		/* interrupt status register */
#define	PXA250TMDP_INT_MASK		0x410		/* interrupt control register */
#define	PXA250TMDP_KBD_STAT		0x500		/* keyboard status register */
#define	PXA250TMDP_KBD_CTRL		0x510		/* keyboard control register */
#define PXA250TMDP_KBD_COL0		0x550		/* keyboard column 0 register */
#define PXA250TMDP_KBD_COL1		0x560		/* keyboard column 1 register */
#define PXA250TMDP_KBD_COL2		0x570		/* keyboard column 2 register */
#define PXA250TMDP_KBD_COL3		0x580		/* keyboard column 3 register */
#define PXA250TMDP_KBD_COL4		0x590		/* keyboard column 4 register */
#define PXA250TMDP_KBD_COL5		0x5A0		/* keyboard column 5 register */
#define PXA250TMDP_COMMON_STAT		0x600		/* common status register */
#define PXA250TMDP_COMMON_CTRL		0x610		/* common control register */

/* Interrupt status and control register bits */
#define PXA250TMDP_KBD_INT			0x80000000  /* keyboard interrupt /enable */
#define PXA250TMDP_RE1_INT			0x40000000	/* rotary encoder 1 interrupt / enable */
#define PXA250TMDP_RE2_INT			0x20000000	/* rotary encoder 2 interrupt / enable */
#define PXA250TMDP_VID_CAP_INT		0x10000000  /* video capture interrupt / enable */

/* rotary encoder status register bits */
#define PXA250TMDP_REx_INT			0x40000000	/* Rotary encoder interrupt signal to FPGA controller */
#define PXA250TMDP_REx_AP			0x20000000	/* Rotary encoder switch AP output signal quadrature output state*/
#define PXA250TMDP_REx_BP			0x10000000	/* Rotary encoder switch BP output signal quadrature output state*/
#define	PXA250TMDP_REX_CW			0x08000000	/* Rotary encoder Clockwise move detected */
#define PXA250TMDP_REx_CCW			0x04000000	/* Rotary encoder Counter Clockwise move detected */

/* rotary encoder control register bits */
#define PXA250TMDP_REx_CNT_CLR		0x80000000	/* Rotary encoder count clear */
#define	PXA250TMDP_REx_SM_RST		0x40000000	/* Rotary encoder state machine reset signal */
#define PXA250TMDP_REx_INT_EN		0x20000000	/* Rotary encoder interrupt enable signal */
#define PXA250TMDP_REx_CLR_INT		0x10000000	/* Rotary encoder interrupt clear signal */

/* keyboard status register bits */
#define PXA250TMDP_KBD_STAT_INT	0x40000000	/* keyboard interrupt */

/* keyboard control register bits */
#define PXA250TMDP_KBD_RESET		0x40000000	/* reset keyboard */
#define PXA250TMDP_KBD_INT_EN		0x20000000	/* keyboard interrupt enable */
#define PXA250TMDP_KBD_INT_CLR		0x10000000	/* clear keyboard interrupt */

/* ID register bits */
#define	PXA250TMDP_VID_CAPTURE_SUP	0x00010000	/* supports video capture */
#define PXA250TMDP_ROT_ENC1_SUP	0x00020000	/* supports rotary encoder 1 */
#define PXA250TMDP_ROT_ENC2_SUP	0x00040000	/* supports rotary encoder 2 */
#define PXA250TMDP_KBD_SUP			0x00080000	/* supports keyboard */
#define PXA250TMDP_IRDA_SELECT		0x00100000	/* select IRDA A or IRDA B */
#define PXA250TMDP_IDE_CNTL_LOGIC	0x00200000  /* IDE control logic */
#define PXA250TMDP_BUS_CNTL		0x00400000  /* Bus requests supported */
#define PXA250TMDP_LAGUNA_HW_SUPT	0x20000000  
#define PXA250TMDP_HW_SUPT			0x40000000  



#define BASE_ADDRESS_SA1111     (0x08000000)     /* Base address for SA1111 companion chip    */
#define BASE_ADDRESS_ETHERNET   (0x14000000)     /* Base address for Ethernet controller      */
#define BASE_ADDRESS_PCMCIA_0   (0x20000000)     /* Base address for PCMCIA/CF - SLOT0        */
#define BASE_ADDRESS_PCMCIA_1   (0x30000000)     /* Base address for PCMCIA/CF - SLOT1        */


/* GPIO registers                                                                             */
#define PXA250TMDP_GPIO_BASE_ADDR   (0x40E00000)
/* Below are the offset of register's address relative to PXA250TMDP_GPIO_BASE_ADDR              */
#define PXA250TMDP_GPIO_GPLR0       (0x0000)        /* GPIO Pin Level Register 0                 */
#define PXA250TMDP_GPIO_GPLR1       (0x0004)        /* GPIO Pin Level Register 1                 */
#define PXA250TMDP_GPIO_GPLR2       (0x0008)        /* GPIO Pin Level Register 1                 */
#define PXA250TMDP_GPIO_GPDR0       (0x000C)        /* GPIO Pin Direction  Register 0            */
#define PXA250TMDP_GPIO_GPDR1       (0x0010)        /* GPIO Pin Direction  Register 1            */
#define PXA250TMDP_GPIO_GPDR2       (0x0014)        /* GPIO Pin Direction  Register 2            */
#define PXA250TMDP_GPIO_GPSR0       (0x0018)        /* GPIO Pin Output Set Register 0            */
#define PXA250TMDP_GPIO_GPSR1       (0x001C)        /* GPIO Pin Output Set Register 1            */
#define PXA250TMDP_GPIO_GPSR2       (0x0020)        /* GPIO Pin Output Set Register 2            */
#define PXA250TMDP_GPIO_GPCR0       (0x0024)        /* GPIO Pin Output Clear Register 0          */
#define PXA250TMDP_GPIO_GPCR1       (0x0028)        /* GPIO Pin Output Clear Register 1          */
#define PXA250TMDP_GPIO_GPCR2       (0x002C)        /* GPIO Pin Output Clear Register 2          */
#define PXA250TMDP_GPIO_GREP0       (0x0030)        /* GPIO Rising Edge detect Enable Register 0 */
#define PXA250TMDP_GPIO_GREP1       (0x0034)        /* GPIO Rising Edge detect Enable Register 1 */
#define PXA250TMDP_GPIO_GREP2       (0x0038)        /* GPIO Rising Edge detect Enable Register 2 */
#define PXA250TMDP_GPIO_GFER0       (0x003C)        /* GPIO Rising Edge detect Enable Register 0 */
#define PXA250TMDP_GPIO_GFER1       (0x0040)        /* GPIO Rising Edge detect Enable Register 1 */
#define PXA250TMDP_GPIO_GFER2       (0x0044)        /* GPIO Rising Edge detect Enable Register 2 */
#define PXA250TMDP_GPIO_GEDR0       (0x0048)        /* GPIO Edge Detect Status Register 0        */
#define PXA250TMDP_GPIO_GEDR1       (0x004C)        /* GPIO Edge Detect Status Register 1        */
#define PXA250TMDP_GPIO_GEDR2       (0x0050)        /* GPIO Edge Detect Status Register 2        */
#define PXA250TMDP_GPIO_GAFR0_L     (0x0054)        /* GPIO Alternate Function Register (0-15)   */
#define PXA250TMDP_GPIO_GAFR0_H     (0x0058)        /* GPIO Alternate Function Register (16-31)  */
#define PXA250TMDP_GPIO_GAFR1_L     (0x005C)        /* GPIO Alternate Function Register (32-47)  */
#define PXA250TMDP_GPIO_GAFR1_H     (0x0060)        /* GPIO Alternate Function Register (48-63)  */
#define PXA250TMDP_GPIO_GAFR2_L     (0x0064)        /* GPIO Alternate Function Register (64-79)  */
#define PXA250TMDP_GPIO_GAFR2_H     (0x0068)        /* GPIO Alternate Function Register (80)     */
#define PXA250TMDP_GPIO_AREA_SIZE   (PXA250TMDP_GPIO_GAFR2_H - PXA250TMDP_GPIO_GPLR0 + 4)


/* Interrupt Controller registers                                                             */
#define PXA250TMDP_INTR_BASE_ADDR   (0x40D00000)
/* Below are the offset of register's address relative to PXA250TMDP_GPIO_BASE_ADDR              */
#define PXA250TMDP_INTR_ICIP        (0x0000)        /* Interrupt Controller IRQ Pending Register  (Bits 8-31 are valid) */
#define PXA250TMDP_INTR_ICMR        (0x0004)        /* Interrupt Controller Mask Register (Bits 8-31 are valid) */
#define PXA250TMDP_INTR_ICLR        (0x0008)        /* Interrupt Controller Level Register (Bits 8-31 are valid) */
#define PXA250TMDP_INTR_ICFP        (0x000C)        /* Interrupt Controller FIQ Pending Register  (Bits 8-31 are valid) */
#define PXA250TMDP_INTR_ICPR        (0x0010)        /* Interrupt Controller Pending Register     */
#define PXA250TMDP_INTR_ICCR        (0x0014)        /* Interrupt Controller Control Register     */
#define PXA250TMDP_INTR_AREA_SIZE   (PXA250TMDP_INTR_ICCR - PXA250TMDP_INTR_ICIP + 4)


/*
 * PXA250TMDP_INTR_ICCR bits
 */

#define PXA250TMDP_INTR_ICCR_DIM    (0x0001)        /* Disable IDLE mask                         */


/* PXA250TMDP Interrupt vectors                                                                  */

#define PXA250TMDP_INT_GPIO0        (8)
#define PXA250TMDP_INT_GPIO1        (9)
#define PXA250TMDP_INT_GPIO_CMN     (10)            /* 'OR' of GPIO 2-80 2-nd level interrupts   */
#define PXA250TMDP_INT_USB          (11)
#define PXA250TMDP_INT_PMU          (12)            /* Performance Monitor                       */
#define PXA250TMDP_INT_I2S          (13)
#define PXA250TMDP_INT_AC97         (14)
#define PXA250TMDP_INT_LCD          (17)
#define PXA250TMDP_INT_I2C          (18)
#define PXA250TMDP_INT_ICP          (19)
#define PXA250TMDP_INT_STUART       (20)
#define PXA250TMDP_INT_BTUART       (21)
#define PXA250TMDP_INT_FFUART       (22)
#define PXA250TMDP_INT_MU           (23)            /* Multimedia Card                           */
#define PXA250TMDP_INT_SSP          (24)            /* Synchronous Serial Port                   */
#define PXA250TMDP_INT_DMA          (25)            /* DMA controller                            */
#define PXA250TMDP_INT_TIMER0       (26)            /* OS Timer 0                                */ 
#define PXA250TMDP_INT_TIMER1       (27)            /* OS Timer 1                                */ 
#define PXA250TMDP_INT_TIMER2       (28)            /* OS Timer 2                                */ 
#define PXA250TMDP_INT_TIMER3       (29)            /* OS Timer 3                                */
#define PXA250TMDP_INT_RTCLOCK_1    (30)            /* 1 Hz clock tic                            */
#define PXA250TMDP_INT_RTCLOCK_2    (31)            /* RTC equals alarm register                 */

#define PXA250TMDP_INT_UCB1400      (128)           /* Cascaded interrut from GPIO02             */


#endif

/* __SRCVERSION("pxa250tmdp.h $Rev: 657836 $"); */
