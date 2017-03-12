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

#define IXP23XX_INTR_BASE			0xc8003000
#define IXP23XX_INTR_SIZE			0x1000

#define IXP23XX_INTR_ST1			0x00
#define IXP23XX_INTR_ST2			0x04
#define IXP23XX_INTR_ST3			0x08
#define IXP23XX_INTR_ST4			0x0c
#define IXP23XX_INTR_EN1			0x10
#define IXP23XX_INTR_EN2			0x14
#define IXP23XX_INTR_EN3			0x18
#define IXP23XX_INTR_EN4			0x1c
#define IXP23XX_INTR_SEL1			0x20
#define IXP23XX_INTR_SEL2			0x24
#define IXP23XX_INTR_SEL3			0x28
#define IXP23XX_INTR_SEL4			0x2c
#define IXP23XX_INTR_IRQ_ST1		0x30
#define IXP23XX_INTR_IRQ_ST2		0x34
#define IXP23XX_INTR_IRQ_ST3		0x38
#define IXP23XX_INTR_IRQ_ST4		0x3c
#define IXP23XX_INTR_FIQ_ST1		0x40
#define IXP23XX_INTR_FIQ_ST2		0x44
#define IXP23XX_INTR_FIQ_ST3		0x48
#define IXP23XX_INTR_FIQ_ST4		0x4c
#define IXP23XX_INTR_PRTY			0x50
#define	IXP23XX_INTR_IRQ_ENC_ST		0x54
#define IXP23XX_INTR_FIQ_ENC_ST		0x58
#define IXP23XX_INTR_ASYNC_SFTRST_N	0x5c


/* needed to detect Xscale core freq */

#define IXP23XX_EXP_BUS_BASE		0xC4000000
#define IXP23XX_EXP_CNFG_0			0x20
#define XSCALE_SPEED_SEL_SHIFT		22


/* GPIO controller definitions */
#define IXP23XX_GPIO_BASE			0xc8004000
#define IXP23XX_GPIO_SIZE			0x1000

#define IXP23XX_GPOUTR				0x00
#define IXP23XX_GPOER				0x04
#define IXP23XX_GPINR				0x08
#define IXP23XX_GPISR				0x0c
#define IXP23XX_GPIT1R				0x10
#define IXP23XX_GPIT2R				0x14
#define IXP23XX_GPCLKR				0x18
#define IXP23XX_GPDSELR				0x1c

/* OS Timer definitions */

#define IXP23XX_OST_BASE			0xc8005000
#define	IXP23XX_OST_SIZE			0x1000

#define IXP23XX_OST_TS				0x00
#define IXP23XX_OST_TIM0			0x04
#define IXP23XX_OST_TIM0_RL			0x08
#define IXP23XX_OST_TIM1			0x0c
#define IXP23XX_OST_TIM1_RL			0x10
#define IXP23XX_OST_WDOG			0x14
#define IXP23XX_OST_WDOG_ENAB		0x18
#define IXP23XX_OST_WDOG_KEY		0x1c
#define IXP23XX_OST_STS				0x20
#define IXP23XX_OST_SW_RESET		0x24
#define IXP23XX_OST_SW_RST_ENAB		0x28


/* XSI2CPP Bridge Registers and bit definitions */

#define IXP23XX_XSI2CPP_BRIDGE_BASE	0xa0000000
#define IXP23XX_XSI2CPP_BRIDGE_SIZE	0x3a0

#define CMD0_REG1_LO				0x00
#define CMD0_REG1_HI				0x04
#define CMD0_REG2_LO				0x08
#define CMD0_REG2_HI				0x0c

#define CMD1_REG1_LO				0x10
#define CMD1_REG1_HI				0x14
#define CMD1_REG2_LO				0x18
#define CMD1_REG2_HI				0x1c

#define CMD2_REG1_LO				0x20
#define CMD2_REG1_HI				0x24
#define CMD2_REG2_LO				0x28
#define CMD2_REG2_HI				0x2c

#define CMD3_REG1_LO				0x30
#define CMD3_REG1_HI				0x34
#define CMD3_REG2_LO				0x38
#define CMD3_REG2_HI				0x3c

#define CMD4_REG1_LO				0x40
#define CMD4_REG1_HI				0x44
#define CMD4_REG2_LO				0x48
#define CMD4_REG2_HI				0x4c

#define CMD5_REG1_LO				0x50
#define CMD5_REG1_HI				0x54
#define CMD5_REG2_LO				0x58
#define CMD5_REG2_HI				0x5c

#define MAN_REG						0x80
#define SRC_DST_ID_REG				0x88
#define CURR_XFER_REG1_LO			0x90
#define CURR_XFER_REG1_HI			0x94
#define CURR_XFER_REG2_LO			0x98
#define CURR_XFER_REG2_HI			0x9c

#define ATOM_DATA0_LO				0xa0
#define ATOM_DATA0_HI				0xa4
#define ATOM_DATA1_LO				0xa8
#define ATOM_DATA1_HI				0xac
#define ATOM_DATA2_LO				0xb0
#define ATOM_DATA2_HI				0xb4
#define ATOM_DATA3_LO				0xb8
#define ATOM_DATA3_HI				0xbc
#define ATOM_DATA4_LO				0xc0
#define ATOM_DATA4_HI				0xc4
#define ATOM_DATA5_LO				0xc8
#define ATOM_DATA5_HI				0xcc

#define WRITE_BUF_CMD0				0x200

#define READ_BUF_CMD0				0x300


#define SIG_TYPE_NONE				0x0
#define SIG_TYPE_PUSH				0x1
#define SIG_TYPE_PULL				0x2
#define SIG_TYPE_BOTH				0x3

#define XFER_TYPE_NONE				0x0
#define XFER_TYPE_PUSH				0x1
#define XFER_TYPE_PULL				0x2
#define XFER_TYPE_BOTH				0x3

/* these bits constitute bits 29:24 of the address register, and define the target */

#define TARGET_ID_PCI_MEM			0x20
#define TARGET_ID_PCI_IO			0x00
#define TARGET_ID_PCI_CONFIG_0		0x02
#define TARGET_ID_PCI_CONFIG_1		0x03
#define TARGET_ID_PCI_SPECIAL		0x04
#define TARGET_ID_PCI_CSR			0x06

/*
 *  PCI
 */

#define IXP23XX_PCI_CSR_CONTROL         0x13c
#define IXP23XX_PCI_CSR_ADR_EXT         0x140

#define IXP23XX_PCI_CSR_CONTROL_PNR     0x00020000
#define IXP23XX_PCI_CSR_CONTROL_DCT     0x00000100
#define IXP23XX_PCI_CSR_CONTROL_RMA     0x00000020

/*
 * PCI Configuration Registers (Offsets from IXP23XX_PCI_CFG_REG_BASE)
 */

#define IXP23XX_PCI_CFG_VEN_DEV_ID      0x00
#define IXP23XX_PCI_CFG_CMD_STAT        0x04
#define IXP23XX_PCI_CFG_REV_CLASS       0x08
#define IXP23XX_PCI_CFG_CSR_BAR         0x10
#define IXP23XX_PCI_CFG_SRAM_BAR        0x14
#define IXP23XX_PCI_CFG_DRAM_BAR        0x18

#define IXP23XX_PCI_CFG_CMDSTAT_RX_MA       0x20000000









/* __SRCVERSION("ixp23xx.h $Rev: 657836 $"); */
