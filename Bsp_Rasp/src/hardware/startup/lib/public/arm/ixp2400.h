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
 * IXP2400 processor
 */

#ifndef	__ARM_IXP2400_H_INCLUDED
#define	__ARM_IXP2400_H_INCLUDED


/*
 * -----------------------------------------------------------------------
 * UART
 * -----------------------------------------------------------------------
 */
#define	IXP2400_UART_BASE		0xc0030000
#define	IXP2400_UART_SIZE		0x20

/*
 * Register offsets from IXP2400_UART_BASE (registers that differ from 8250)
 */

#define	IXP2400_UART_IER		0x04

/*
 * UART_IER bits
 */

#define	IXP2400_UART_IER_RTOIEN	0x10		/* receiver data time out */
#define	IXP2400_UART_IER_NRZE	0x20		/* NRZ coding enable */
#define	IXP2400_UART_IER_UUE 	0x40		/* uart unit enable */



/*
 * -----------------------------------------------------------------------
 * Timer registers
 * -----------------------------------------------------------------------
 */
#define	IXP2400_TIMER_BASE		0xc0020000
#define	IXP2400_TIMER_SIZE		0x44


/*
 * Register offsets from IXP2400_TIMER_BASE
 */

#define	IXP2400_TIMER1_CONTROL		0x00
#define	IXP2400_TIMER2_CONTROL		0x04
#define	IXP2400_TIMER3_CONTROL		0x08
#define	IXP2400_TIMER4_CONTROL		0x0c

#define	IXP2400_TIMER1_LOAD		0x10
#define	IXP2400_TIMER2_LOAD		0x14
#define	IXP2400_TIMER3_LOAD		0x18
#define	IXP2400_TIMER4_LOAD		0x1c

#define	IXP2400_TIMER1_STATUS	0x20
#define	IXP2400_TIMER2_STATUS	0x24
#define	IXP2400_TIMER3_STATUS	0x28
#define	IXP2400_TIMER4_STATUS	0x2c

#define	IXP2400_TIMER1_CLEAR	0x30
#define	IXP2400_TIMER2_CLEAR	0x34
#define	IXP2400_TIMER3_CLEAR	0x38
#define	IXP2400_TIMER4_CLEAR	0x3c

#define IXP2400_TIMER4_TWDE		0x40


/*
 * TIMERx_CTRL bits
 */
#define	IXP2400_TIMER_CONTROL_STP_1		0x00	/* use core frequency */
#define	IXP2400_TIMER_CONTROL_STP_16	0x04	/* use core/16 */
#define	IXP2400_TIMER_CONTROL_STP_256	0x08	/* use core/256 */
#define	IXP2400_TIMER_CONTROL_ACT		0x80    /* activate timer */

/*
 * -----------------------------------------------------------------------
 * Interrupt Controller
 * -----------------------------------------------------------------------
 */
#define	IXP2400_IRQ_BASE		0xd6000000		/* physical address */
#define	IXP2400_IRQ_SIZE		0x00000210

/*
 * IRQ register 
 */

#define	IXP2400_IRQ_STATUS		0x008	    /* IRQ masked interrupt status */
#define	IXP2400_IRQ_ENABLE	 	0x010		/* interrupt enable  */
#define	IXP2400_IRQ_ENABLE_SET	0x010		/* interrupt enable set */
#define	IXP2400_IRQ_ENABLE_CLR	0x018		/* interrupt enable clr */


/*
 * -----------------------------------------------------------------------
 * PCI Address Space and Registers
 * -----------------------------------------------------------------------
 */

#define	IXP2400_PCI_CFG_BASE	0xda000000		/* physical address */
#define	IXP2400_PCI_CFG_SIZE	0x02000000

#define	IXP2400_PCI_TYPE0_CONFIG_BASE	0xda000000
#define	IXP2400_PCI_TYPE1_CONFIG_BASE	0xdb000000
#define	IXP2400_PCI_IO_BASE				0xd8000000
#define	IXP2400_PCI_IO_BASE_SIZE		0x02000000
#define	IXP2400_PCI_MEM_BASE			0xe0000000
#define	IXP2400_PCI_MEM_BASE_SIZE		0x08000000

/*
 * PCI Controller CFG Register Base
 */

#define IXP2400_PCI_CFG_REG_BASE		0xde000000
#define IXP2400_PCI_CFG_REG_SIZE		0x00000100

/*
 * PCI Configuration Registers (Offsets from IXP2400_PCI_CFG_REG_BASE)
 */

#define IXP2400_PCI_CFG_VEN_DEV_ID		0x00
#define IXP2400_PCI_CFG_CMD_STAT		0x04
#define IXP2400_PCI_CFG_REV_CLASS		0x08
#define IXP2400_PCI_CFG_CSR_BAR			0x10
#define IXP2400_PCI_CFG_SRAM_BAR		0x14
#define IXP2400_PCI_CFG_DRAM_BAR		0x18

/*
 *  PCI CFG CMD_STAT Bits
 */

#define IXP2400_PCI_CFG_CMDSTAT_RX_MA	0x20000000


/*
 * PCI Controller CSRs Base
 */

#define IXP2400_PCI_CSR_BASE			0xdf000000
#define IXP2400_PCI_CSR_SIZE			0x00000160

/*
 * PCI Controller CSRs (Offsets from IXP2400_PCI_CSR_BASE)
 */

#define	IXP2400_PCI_CSR_OUT_INT_STATUS	0x030
#define	IXP2400_PCI_CSR_OUT_INT_MASK	0x034
#define IXP2400_PCI_CSR_DRAM_BAR_MASK	0x100
#define	IXP2400_PCI_CSR_CONTROL			0x13c
#define	IXP2400_PCI_CSR_ADR_EXT			0x140
#define	IXP2400_XSCALE_INT_STATUS		0x158
#define	IXP2400_XSCALE_INT_ENABLE		0x15c

/*
 *  PCI Control bits
 */

#define IXP2400_PCI_CSR_CONTROL_PNR		0x00020000
#define IXP2400_PCI_CSR_CONTROL_DCT     0x00000100
#define IXP2400_PCI_CSR_CONTROL_RMA     0x00000020

/*
 * -----------------------------------------------------------------------
 * Global Control Registers
 * -----------------------------------------------------------------------
 *
 */


#define	IXP2400_GCAP_BASE 	0xc0004a00		/* physical address */
#define	IXP2400_GCAP_SIZE 	0x1c


/*
 *  Register offsets from IXP2400_GCAP_BASE
 */

#define IXP2400_GCAP_RESET0				0x0c
#define IXP2400_GCAP_STRAP_OPTIONS		0x18

/*
 *   GCAP_RESET0 bits
 */

#define IXP2400_GCAP_RESET0_RSTALL	0x00010000

/*
 * STRAP OPTION bits
 */

#define IXP2400_GCAP_STRAP_PCI_BOOT_HOST	0x00000004

	

/*
 * -----------------------------------------------------------------------
 * Microengine Registers
 * -----------------------------------------------------------------------
 */

/*nyi */

/*
 * -----------------------------------------------------------------------
 * SRAM Registers
 * -----------------------------------------------------------------------
 */

#define	IXP2400_SRAM_QDR_BASE		0xcc010000
#define IXP2400_SRAM_QDR_SIZE		0x340

#define IXP2400_SRAM_QDR_CH_OFFSET	0x400000



/*
 *  Register offsets from IXP2400_SRAM_QDRx_BASE
 */

#define IXP2400_SRAM_QDR_CONTROL	0x0

/*
 *   SRAM_QDR_CONTROL bits
 */

#define	IXP2400_SRAM_QDR_CONTROL_SIZE_SHIFT		7
#define	IXP2400_SRAM_QDR_CONTROL_SIZE_MASK		7



/*
 * -----------------------------------------------------------------------
 * SDRAM Registers
 * -----------------------------------------------------------------------
 */

#define	IXP2400_DDRSDRAM_BASE		0xd0009000		/* physical address */
#define	IXP2400_DDRSDRAM_SIZE		0x000007f8

/*  
 *  Register offset from IXP2400_DDRSDRAM_BASE
 */

#define IXP2400_DDRSDRAM_DU_CONTROL			0x000

/*
 *  DDRSDRAM DU_CONTROL bits
 */

#define IXP2400_DDRSDRAM_NUM_SIDES_SHIFT	26
#define IXP2400_DDRSDRAM_NUM_SIDES_MASK     1

#define IXP2400_DDRSDRAM_NUMROW_COL_SHIFT	23      
#define IXP2400_DDRSDRAM_NUMROW_COL_MASK    0x7


#endif /*__ARM_IXP2400_H_INCLUDED */

/* __SRCVERSION("ixp2400.h $Rev: 657836 $"); */
