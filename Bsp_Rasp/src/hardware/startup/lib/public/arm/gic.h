/*
 * $QNXLicenseC:
 * Copyright 2011, QNX Software Systems. 
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
 * ARM Generic Interrupt Controller
 */

#ifndef	__ARM_GIC_H_INCLUDED
#define	__ARM_GIC_H_INCLUDED

/*
 * -----------------------------------------------------------------------
 * Interrupt Distributor registers
 * -----------------------------------------------------------------------
 */

#define	ARM_GICD_SIZE			0x1000

#define	ARM_GICD_CTLR			0x000
#define	ARM_GICD_TYPER			0x004
#define ARM_GICD_IIDR			0x008
#define ARM_GICD_IGROUPRn		0x080
#define	ARM_GICD_ISENABLERn		0x100
#define	ARM_GICD_ICENABLERn		0x180
#define	ARM_GICD_ISPENDRn		0x200
#define	ARM_GICD_ICPENDRn		0x280
#define	ARM_GICD_ISACTIVERn		0x300
#define	ARM_GICD_ICACTIVERn		0x300
#define	ARM_GICD_IPRIORITYn		0x400
#define	ARM_GICD_ITARGETSRn		0x800
#define	ARM_GICD_ICFGRn			0xc00
#define	ARM_GICD_SGIR			0xf00
#define ARM_GICD_CPENDSGIRn		0xf10
#define ARM_GICD_SPENDSGIRn		0xf20
#define ARM_GICD_PIDR4			0xfd0
#define ARM_GICD_PIDR5			0xfd4
#define ARM_GICD_PIDR6			0xfd8
#define ARM_GICD_PIDR7			0xfdc
#define ARM_GICD_PIDR0			0xfe0
#define ARM_GICD_PIDR1			0xfe4
#define ARM_GICD_PIDR2			0xfe8
#define ARM_GICD_PIDR3			0xfec
#define ARM_GICD_CIDR0			0xff0
#define ARM_GICD_CIDR1			0xff4
#define ARM_GICD_CIDR2			0xff8
#define ARM_GICD_CIDR3			0xffc

#define ARM_GICD_TYPER_ITLN		0x1f
#define	ARM_GICD_CTLR_EN		0x00000001

/*
 * -----------------------------------------------------------------------
 * CPU interface registers
 * -----------------------------------------------------------------------
 */

#define ARM_GICC_SIZE		0x2000

#define	ARM_GICC_CTLR		0x00
#define	ARM_GICC_PMR		0x04
#define	ARM_GICC_BBPR		0x08
#define	ARM_GICC_IAR		0x0c
#define	ARM_GICC_EOIR		0x10
#define	ARM_GICC_RPR		0x14
#define	ARM_GICC_HPPIR		0x18
#define	ARM_GICC_ABPR		0x1c
#define	ARM_GICC_AIAR		0x20
#define	ARM_GICC_AEOIR		0x24
#define	ARM_GICC_AHPPIR		0x28
#define	ARM_GICC_APR0		0xd0
#define	ARM_GICC_NSAPR0		0xe0
#define	ARM_GICC_IIDR		0xfc
#define	ARM_GICC_DIR		0x18

/*
 * CPU_CTRL bits
 */
#define	ARM_GICC_CTLR_EN		0x00000001

/*
 * CPU_IACK bits
 */
#define	ARM_GICC_IAR_SRCID	0x00001c00

#endif	/* __ARM_GIC_H_INCLUDED */

/* __SRCVERSION("gic.h $Rev: 657836 $"); */
