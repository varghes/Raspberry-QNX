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
 * TI Davinci (DM355) EVM with ARM 926 core
 */


#define DM355_MEM_BASE				0x80000000

/* PLL CONTROLLER */
#define DM355_PLL_BASE				0x01C40800
#define DM355_PLL_PLLCTL			0x100
#define DM355_PLL_PLLM				0x110
#define DM355_PLL_PREDIV			0x114
#define DM355_PLL_PLLDIV1			0x118
#define DM355_PLL_POSTDIV			0x128
#define DM355_PLL_INPUT_CLK			24000000


/* TIMERS CONTROLLER */                                 
#define DM355_TIMER_SIZE			0x48

#define DM355_TMR_1_REGS			0x01C21400
#define DM355_TMR_2_REGS			0x01C21400
#define DM355_TMR_3_REGS			0x01C21800
#define DM355_TMR_4_REGS			0x01C21800
#define DM355_TMR_WDT_REGS			0x01C21C00
#define DM355_TMR_5_REGS			0x01C20800
#define DM355_TMR_6_REGS			0x01C20800

#define DM355_TMR_PID12				0x00		// PID12 Peripheral Identification Register 12
#define DM355_TMR_EMUMGT			0x04		// EMUMGT Emulation Management Register
#define DM355_TMR_TIM12				0x10		// TIM12 Timer Counter Register 12
#define DM355_TMR_TIM34				0x14		// TIM34 Timer Counter Register 34
#define DM355_TMR_PRD12				0x18		// PRD12 Timer Period Register 12
#define DM355_TMR_PRD34				0x1C		// PRD34 Timer Period Register 34
#define DM355_TMR_TCR				0x20		// TCR Timer Control Register
#define DM355_TMR_TGCR				0x24		// TGCR Timer Global Control Register
#define DM355_TMR_WDTCR				0x28		// WDTCR Watchdog Timer Control Register
#define DM355_TMR_REL12				0x34		// REL12 Timer Reload Register 12
#define DM355_TMR_REL34				0x38		// REL34 Timer Reload Register 34
#define DM355_TMR_CAP12				0x3C		// CAP12 Timer Capture Register 12
#define DM355_TMR_CAP34				0x40		// CAP34 Timer Capture Register 34
#define DM355_TMR_INTCTL_STAT		0x44		// INTCTL_STAT Timer Interrupt Control and Status Register

/* Asynchronous EMIF*/
#define DM355_AEMIF_CTRL_BASE		0x01E10000	// Asynchronous EMIF Control Register Base Address
#define DM355_AEMIF_CTRL_SIZE		0xE0		// Asynchronous EMIF Control Register Size
#define DM355_AEMIF_CE0_BASE		0x02000000	// Asynchronous EMIF Data (CE0) Base Address
#define DM355_AEMIF_CE1_BASE		0x04000000	// Asynchronous EMIF Data (CE1) Base Address

#define DM355_AEMIF_AWCCR			0x04		// Asynchronous Wait Cycle Configuration Register
#define DM355_AEMIF_A1CR			0x10		// Asynchronous 1 Configuration Register (CS2 space)
#define DM355_AEMIF_A2CR			0x14		// Asynchronous 2 Configuration Register (CS3 space)
#define DM355_AEMIF_EIRR			0x40		// EMIF Interrupt Raw Register
#define DM355_AEMIF_EIMR			0x44		// EMIF Interrupt Mask Register
#define DM355_AEMIF_EIMSR			0x48		// EMIF Interrupt Mask Set Register
#define DM355_AEMIF_EIMCR			0x4C		// EMIF Interrupt Mask Clear Register
#define DM355_AEMIF_ONENANDCTL		0x5C		// OneNAND Flash Control Register
#define DM355_AEMIF_NANDFCR			0x60		// NAND Flash Control Register
#define DM355_AEMIF_NANDFSR			0x64		// NAND Flash Status Register
#define DM355_AEMIF_NANDF1ECC		0x70		// NAND Flash 1-Bit ECC Register 1 (CS2 Space)
#define DM355_AEMIF_NANDF2ECC		0x74		// NAND Flash 1-Bit ECC Register 2 (CS3 Space)
#define DM355_AEMIF_NAND4BITECCLOAD	0xBC		// NANDFlash 4-Bit ECC Load Register
#define DM355_AEMIF_NAND4BITECC1	0xC0		// NAND Flash 4-Bit ECC Register 1
#define DM355_AEMIF_NAND4BITECC2	0xC4		// NAND Flash 4-Bit ECC Register 2
#define DM355_AEMIF_NAND4BITECC3	0xC8		// NAND Flash 4-Bit ECC Register 3
#define DM355_AEMIF_NAND3BITECC4	0xCC		// NAND Flash 4-Bit ECC Register 4
#define DM355_AEMIF_NANDERRADD1		0xD0		// NAND Flash 4-Bit ECC Error Address Register 1
#define DM355_AEMIF_NANDERRADD2		0xD4		// NAND Flash 4-Bit ECC Error Address Register 2
#define DM355_AEMIF_NANDERRVAL1		0xD8		// NAND Flash 4-Bit ECC Error Value Register 1
#define DM355_AEMIF_NANDERRVAL2		0xDC		// NAND Flash 4-Bit ECC Error Value Register 2

/* GPIO */
#define DM355_GPIO_BASE				0x01C67000
#define DM355_GPIO_SIZE				0xB0

#define DM355_GPIO_PID				0		// Peripheral Identification Register
#define DM355_GPIO_BINTEN			0x08	// GPIO Interrupt Per-Bank Enable Register
#define DM355_GPIO_DIR01			0x10	// GPIO Banks 0 and 1 Direction Register
#define DM355_GPIO_OUT_DATA01		0x14	// GPIO Banks 0 and 1 Output Data Register
#define DM355_GPIO_SET_DATA01		0x18	// GPIO Banks 0 and 1 Set Data Register
#define DM355_GPIO_CLR_DATA01		0x1C	// GPIO Banks 0 and 1 Clear Data Register
#define DM355_GPIO_IN_DATA01		0x20	// GPIO Banks 0 and 1 Input Data Register
#define DM355_GPIO_SET_RIS_TRIG01	0x24	// GPIO Banks 0 and 1 Set Rising Edge Interrupt Register
#define DM355_GPIO_CLR_RIS_TRIG01	0x28	// GPIO Banks 0 and 1 Clear Rising Edge Interrupt Register
#define DM355_GPIO_SET_FAL_TRIG01	0x2C	// GPIO Banks 0 and 1 Set Falling Edge Interrupt Register
#define DM355_GPIO_CLR_FAL_TRIG01	0x30	// GPIO Banks 0 and 1 Clear Falling Edge Interrupt Register
#define DM355_GPIO_INTSTAT01		0x34	// GPIO Banks 0 and 1 Interrupt Status Register
#define DM355_GPIO_DIR23			0x38	// GPIO Banks 2 and 3 Direction Register
#define DM355_GPIO_OUT_DATA23		0x3C	// GPIO Banks 2 and 3 Output Data Register
#define DM355_GPIO_SET_DATA23		0x40	// GPIO Banks 2 and 3 Set Data Register
#define DM355_GPIO_CLR_DATA23		0x44	// GPIO Banks 2 and 3 Clear Data Register
#define DM355_GPIO_IN_DATA23		0x48	// GPIO Banks 2 and 3 Input Data Register
#define DM355_GPIO_SET_RIS_TRIG23	0x4C	// GPIO Banks 2 and 3 Set Rising Edge Interrupt Register
#define DM355_GPIO_CLR_RIS_TRIG23	0x50	// GPIO Banks 2 and 3 Clear Rising Edge Interrupt Register
#define DM355_GPIO_SET_FAL_TRIG23	0x54	// GPIO Banks 2 and 3 Set Falling Edge Interrupt Register
#define DM355_GPIO_CLR_FAL_TRIG23	0x58	// GPIO Banks 2 and 3 Clear Falling Edge Interrupt Register
#define DM355_GPIO_INTSTAT23		0x5C	// GPIO Banks 2 and 3 Interrupt Status Register
#define DM355_GPIO_DIR45			0x60	// GPIO Bank 4 and 5 Direction Register
#define DM355_GPIO_OUT_DATA45		0x64	// GPIO Bank 4 and 5 Output Data Register
#define DM355_GPIO_SET_DATA45		0x68	// GPIO Bank 4 and 5 Set Data Register
#define DM355_GPIO_CLR_DATA45		0x6C	// GPIO Bank 4 and 5 Clear Data Register
#define DM355_GPIO_IN_DATA45		0x70	// GPIO Bank 4 and 5 Input Data Register
#define DM355_GPIO_SET_RIS_TRIG45	0x74	// GPIO Bank 4 and 5 Set Rising Edge Interrupt Register
#define DM355_GPIO_CLR_RIS_TRIG45	0x78	// GPIO Bank 4 and 5 Clear Rising Edge Interrupt Register
#define DM355_GPIO_SET_FAL_TRIG45	0x7C	// GPIO Bank 4 and 5 Set Falling Edge Interrupt Register
#define DM355_GPIO_CLR_FAL_TRIG45	0x80	// GPIO Bank 4 and 5 Clear Falling Edge Interrupt Register
#define DM355_GPIO_INTSTAT45		0x84	// GPIO Bank 4 and 5 Interrupt Status Register
#define DM355_GPIO_DIR6				0x88	// GPIO Bank 6 Direction Register
#define DM355_GPIO_OUT_DATA6		0x8C	// GPIO Bank 6 Output Data Register
#define DM355_GPIO_SET_DATA6		0x90	// GPIO Bank 6 Set Data Register
#define DM355_GPIO_CLR_DATA6		0x94	// GPIO Bank 6 Clear Data Register
#define DM355_GPIO_IN_DATA6			0x98	// GPIO Bank 6 Input Data Register
#define DM355_GPIO_SET_RIS_TRIG6	0x9C	// GPIO Bank 6 Set Rising Edge Interrupt Register
#define DM355_GPIO_CLR_RIS_TRIG6	0xA0	// GPIO Bank 6 Clear Rising Edge Interrupt Register
#define DM355_GPIO_SET_FAL_TRIG6	0xA4	// GPIO Bank 6 Set Falling Edge Interrupt Register
#define DM355_GPIO_CLR_FAL_TRIG6	0xA8	// GPIO Bank 6 Clear Falling Edge Interrupt Register
#define DM355_GPIO_INTSTAT6			0xAC	// GPIO Bank 6 Interrupt Status Register

/* I2C */
#define DM355_I2C_BASE				0x01c21000
#define DM355_I2C_SIZE				0x3C
#define DM355_I2C_ICOAR				0x00
#define DM355_I2C_ICIMR				0x04
	#define DM355_I2C_ICIMR_AL		(1 << 0)
	#define DM355_I2C_ICIMR_NACK	(1 << 1)
	#define DM355_I2C_ICIMR_ARDY	(1 << 2)
	#define DM355_I2C_ICIMR_ICRRDY	(1 << 3)
	#define DM355_I2C_ICIMR_ICXRDY	(1 << 4)
	#define DM355_I2C_ICIMR_SCD		(1 << 5)
	#define DM355_I2C_ICIMR_AAS		(1 << 6)
#define DM355_I2C_ICSTR				0x08
	#define DM355_I2C_ICSTR_SDIR	(1 << 14)
	#define DM355_I2C_ICSTR_NACKSNT (1 << 13)
	#define DM355_I2C_ICSTR_BB		(1 << 12)
	#define DM355_I2C_ICSTR_RSFULL	(1 << 11)
	#define DM355_I2C_ICSTR_XSMT	(1 << 10)
	#define DM355_I2C_ICSTR_AAS		(1 << 9)
	#define DM355_I2C_ICSTR_AD0		(1 << 8)
	#define DM355_I2C_ICSTR_SCD		(1 << 5)
	#define DM355_I2C_ICSTR_ICXRDY	(1 << 4)
	#define DM355_I2C_ICSTR_ICRRDY	(1 << 3)
	#define DM355_I2C_ICSTR_ARDY	(1 << 2)
	#define DM355_I2C_ICSTR_NACK	(1 << 1)
	#define DM355_I2C_ICSTR_AL		(1 << 0)
#define DM355_I2C_ICCLKL			0x0C
#define DM355_I2C_ICCLKH			0x10
#define DM355_I2C_ICCNT				0x14
#define DM355_I2C_ICDRR				0x18
#define DM355_I2C_ICSAR				0x1C
#define DM355_I2C_ICDXR				0x20
#define DM355_I2C_ICMDR				0x24
	#define DM355_I2C_ICMDR_STT		(1 << 13)
	#define DM355_I2C_ICMDR_STP		(1 << 11)
	#define DM355_I2C_ICMDR_MST		(1 << 10)
	#define DM355_I2C_ICMDR_TRX		(1 << 9)
	#define DM355_I2C_ICMDR_XA		(1 << 8)
	#define DM355_I2C_ICMDR_IRS		(1 << 5)
	#define DM355_I2C_ICMDR_STB		(1 << 4)
#define DM355_I2C_ICIVR				0x28
	#define DM355_I2C_ICIVR_AL		0x1
	#define DM355_I2C_ICIVR_NACK	0x2
	#define DM355_I2C_ICIVR_ARDY	0x3
	#define DM355_I2C_ICIVR_ICRRDY	0x4
	#define DM355_I2C_ICIVR_ICXRDY	0x5
	#define DM355_I2C_ICIVR_SCD		0x6
	#define DM355_I2C_ICIVR_AAS		0x7
#define DM355_I2C_ICEMDR			0x2C
#define DM355_I2C_ICPSC				0x30
#define DM355_I2C_ICPID1			0x34
#define DM355_I2C_ICPID2			0x38

/* NAND */
#define DM355_NAND_CHIP0_BASE		0x02000000			// AEMIF CE0, DM355_AEMIF_CE0_BASE
#define DM355_NAND_CHIP1_BASE		0x02004000			// AEMIF CE0, DM355_AEMIF_CE0_BASE + 0x4000
#define DM355_NAND_LED_OFF			0x200				// GPIO61/EM_A07, EM_A07 address line high
#define DM355_NAND_LED_ON			0					// GPIO61/EM_A07, EM_A07 address line low

