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
 * ARM MPCore SCU and distributed interrupt controller
 */

#ifndef	__ARM_MPCORE_H_INCLUDED
#define	__ARM_MPCORE_H_INCLUDED

/*
 * CP15 Auxiliary control register
 *
 * Accessed by mcr/mrc p15, 0, Rd, c1, c0, 1
 */
#define	MPCORE_CP15_AUX_SMP		(1 << 5)	/* enable SMP mode */
#define	MPCORE_CP15_AUX_EXCL	(1 << 4)	/* L1/L2 caches are exclusive */
#define	MPCORE_CP15_AUX_F		(1 << 3)	/* enable instruction folding */
#define	MPCORE_CP15_AUX_SB		(1 << 2)	/* enable static prediction */
#define	MPCORE_CP15_AUX_DB		(1 << 1)	/* enable dynamic prediction */
#define	MPCORE_CP15_AUX_RS		(1 << 0)	/* enable return stack */

/*
 * -----------------------------------------------------------------------
 * SCU register offsets from platform-specific SCU base address
 * -----------------------------------------------------------------------
 */

#define	MPCORE_SCU_CTRL			0x00	/* SCU control register */
#define	MPCORE_SCU_CONFIG		0x04	/* SCU configuration register */
#define	MPCORE_CPU_STATUS		0x08	/* SCU CPU status register */
#define	MPCORE_SCU_INVALIDATE	0x0c	/* SCU invalidate all register */

/*
 * SCU_CTRL bits
 */
#define	MPCORE_SCU_CTRL_EN		0x00000001	/* enable SCU */

/*
 * SCU_CONFIG bits
 */
#define	MPCORE_SCU_CONFIG_CPUNUM_MASK	0x00000003
#define	MPCORE_SCU_CONFIG_SMP_MASK		0x000000f0

#define	MPCORE_SCU_CONFIG_TAG_RAM_SIZE(config, cpu) \
		(((config) >> (8 + ((cpu) * 2))) & 3)
#define	MPCORE_SCU_CONFIG_TAG_RAM_16K	0
#define	MPCORE_SCU_CONFIG_TAG_RAM_32K	1
#define	MPCORE_SCU_CONFIG_TAG_RAM_64K	2
#define	MPCORE_SCU_CONFIG_TAG_RAM_RSVD	3

/*
 * SCU_CPU_STATUS bits
 */
#define	MPCORE_SCU_CPU_STATUS(status, cpu) \
		(((status) >> ((cpu) * 2)) & 3)
#define	MPCORE_SCU_CPU_STATUS_NORMAL	0
#define	MPCORE_SCU_CPU_STATUS_RSVD		1
#define	MPCORE_SCU_CPU_STATUS_DORMANT	2
#define	MPCORE_SCU_CPU_STATUS_OFF		2

/*
 * SCU_INVALIDATE bits
 */
#define	MPCORE_SCU_INVALIDATE_BITS(cpu, ways) \
		(((ways) & 0xf) << ((cpu) * 4))

/*
 * -----------------------------------------------------------------------
 * Interrupt Distributor registers
 *
 * Register offsets from MPCORE_GIC_BASE
 * -----------------------------------------------------------------------
 */

#define	MPCORE_GIC_DIST_BASE		0x1000	/* offset from SCU base */
#define	MPCORE_GIC_DIST_SIZE		0x1000

#define	MPCORE_GIC_DIST_CTRL		0x000	/* distributor control */
#define	MPCORE_GIC_DIST_TYPE		0x004
#define	MPCORE_GIC_DIST_ENABLE_SET	0x100
#define	MPCORE_GIC_DIST_ENABLE_CLR	0x180
#define	MPCORE_GIC_DIST_PENDING_SET	0x200
#define	MPCORE_GIC_DIST_PENDING_CLR	0x280
#define	MPCORE_GIC_DIST_ACTIVE_BIT	0x300
#define	MPCORE_GIC_DIST_PRIORITY	0x400
#define	MPCORE_GIC_DIST_TARGET		0x800
#define	MPCORE_GIC_DIST_CONFIG		0xc00
#define	MPCORE_GIC_DIST_SOFTINT		0xf00

#define	MPCORE_GIC_DIST_CTRL_EN		0x00000001

/*
 * -----------------------------------------------------------------------
 * Interrupt Distributor CPU interface
 *
 * Register offsets from MPCORE_GIC_BASE
 * -----------------------------------------------------------------------
 */

#define	MPCORE_GIC_CPU_BASE			0x0100	/* offset from SCU base */
#define MPCORE_GIC_CPU_SIZE			0x0100

#define	MPCORE_GIC_CPU_CTRL			0x00
#define	MPCORE_GIC_CPU_PRIO_MASK	0x04
#define	MPCORE_GIC_CPU_BINARY_POINT	0x08
#define	MPCORE_GIC_CPU_IACK			0x0c
#define	MPCORE_GIC_CPU_EOI			0x10
#define	MPCORE_GIC_CPU_RUNNING_PRI	0x14
#define	MPCORE_GIC_CPU_HIGHEST_PRI	0x18

/*
 * CPU_CTRL bits
 */
#define	MPCORE_GIC_CPU_CTRL_EN		0x00000001

/*
 * CPU_IACK bits
 */
#define	MPCORE_GIC_CPU_IACK_SRCID	0x00001c00
#endif	/* __ARM_MPCORE_H_INCLUDED */

/* __SRCVERSION("mpcore.h $Rev: 657836 $"); */
