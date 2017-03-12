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
 * SA1110 Development Board Specific Registers
 *

 *
 */
#ifndef	ARM_SA1110DB_H_INCLUDED
#define	ARM_SA1110DB_H_INCLUDED

#include <sys/neutrino.h>
#include <hw/sysinfo.h>
#include <hw/inout.h>
#include <arm/sa1100.h>

#define	HWI_TAG_NAME_sa1110db_bcr	"SA1110DB_BCR"
#define	HWI_TAG_ALIGN_sa1110db_bcr	(sizeof(unsigned))
struct hwi_sa1110db_bcr {
	struct hwi_item		prefix;
	unsigned			addr;
	unsigned			value;
};

/*
 * Write-only BCR register
 */
#define	SA1110DB_BCR_BASE		0x12000000	/* physical address */
#define	SA1110DB_BCR_CF_PWR		0x00000001	/* CF power on */
#define	SA1110DB_BCR_CF_RST		0x00000002	/* CF power up reset */
#define	SA1110DB_BCR_SOFT_RST	0x00000004	/* Codec reset */
#define	SA1110DB_BCR_IRDA_FSEL	0x00000008	/* IRDA MIR/FIR select */
#define	SA1110DB_BCR_IRDA_MD0	0x00000010	/* IRDA range/power */
#define	SA1110DB_BCR_IRDA_MD1	0x00000020	/* IRDA range/power */
#define	SA1110DB_BCR_STEREO_LB	0x00000040	/* Stereo loopback */
#define	SA1110DB_BCR_CF_BUS_OFF	0x00000080	/* CF Bus off (float) */
#define	SA1110DB_BCR_AUDIO_ON	0x00000100	/* Audio power on */
#define	SA1110DB_BCR_LIGHT		0x00000200	/* Backlight on */
#define	SA1110DB_BCR_LCD16		0x00000400	/* LCD 16bit RGB565 */
#define	SA1110DB_BCR_LCD_ON		0x00000800	/* LCD power on */
#define	SA1110DB_BCR_RS232EN	0x00001000	/* RS232 auto power on */
#define	SA1110DB_BCR_D9_LED		0x00002000	/* D9 LED off */
#define	SA1110DB_BCR_D8_LED		0x00004000	/* D8 LED off */
#define	SA1110DB_BCR_VIB_ON		0x00008000	/* Vibration motor on */
#define	SA1110DB_BCR_COM_DTR	0x00010000	/* COM port DTR */
#define	SA1110DB_BCR_COM_RTS	0x00020000	/* COM port RTS */
#define	SA1110DB_BCR_RAD_WU		0x00040000	/* Radio wake up interrupt */
#define	SA1110DB_BCR_SMB_EN		0x00080000	/* System management bus enable */
#define	SA1110DB_BCR_TV_IR_DEC	0x00100000	/* TV IR decode enable */
#define	SA1110DB_BCR_QMUTE		0x00200000	/* Quick mute */
#define	SA1110DB_BCR_RAD_ON		0x00400000	/* Radio power on */
#define	SA1110DB_BCR_SPK_OFF	0x00800000	/* Speaker amplifier off */

/*
 * Default value for BCR set by startup program
 */
#define	SA1110DB_BCR_DEFAULT		\
	(								\
		SA1110DB_BCR_SOFT_RST	|	\
		SA1110DB_BCR_CF_BUS_OFF |	\
		SA1110DB_BCR_AUDIO_ON	|	\
		SA1110DB_BCR_LCD16		|	\
		SA1110DB_BCR_LCD_ON		|	\
		SA1110DB_BCR_RS232EN	|	\
		SA1110DB_BCR_D8_LED		|	\
		SA1110DB_BCR_D9_LED			\
	)

/*
 * Read-only BSR register (same address as BCR)
 */
#define	SA1110DB_BSR_RS232_VALID	0x01000000	/* Valid RS232 level detected */
#define	SA1110DB_BSR_COM_DCD		0x02000000	/* COM port DCD */
#define	SA1110DB_BSR_COM_CTS		0x04000000	/* COM port CTS */
#define	SA1110DB_BSR_COM_DSR		0x08000000	/* COM port DSR */
#define	SA1110DB_BSR_RAD_CTS		0x10000000	/* Radio port CTS */
#define	SA1110DB_BSR_RAD_DSR		0x20000000	/* Radio port DSR */
#define	SA1110DB_BSR_RAD_DCD		0x40000000	/* Radio port DCD */
#define	SA1110DB_BSR_RAD_RI			0x80000000	/* Radio port RI */

/*
 * Macros for manipulating BCR register
 */

/*
** Locate BCR from syspage hwinfo structure
*/
static __inline__ struct hwi_sa1110db_bcr * __attribute__((__unused__))
sa1110db_bcr_attach()
{
	unsigned	off;

	off = hwi_find_item(HWI_NULL_OFF, "sa1110db_bcr", NULL);
	return hwi_off2tag(off);
}

/*
** Set a bit in the BCR
*/
static __inline__ void __attribute__((__unused__))
sa1110db_bcr_set(struct hwi_sa1110db_bcr *bcr, unsigned bits)
{
	InterruptDisable();
	bcr->value |= bits;
	out32(bcr->addr, bcr->value);
	InterruptEnable();
}

/*
** Clear a bit in the BCR
*/
static __inline__ void __attribute__((__unused__))
sa1110db_bcr_clr(struct hwi_sa1110db_bcr *bcr, unsigned bits)
{
	InterruptDisable();
	bcr->value &= ~bits;
	out32(bcr->addr, bcr->value);
	InterruptEnable();
}

/*
** Read BSR value
*/
static __inline__ unsigned __attribute__((__unused__))
sa1110db_bsr_get(struct hwi_sa1110db_bcr *bcr)
{
	return in32(bcr->addr);
}

/*
 * SA1110-DB specific GPIO lines
 */

#define	SA1110DB_CF_IRQ		SA1100_GPIO_21	/* CompactFlash IRQ */
#define	SA1110DB_CF_RDY		SA1100_GPIO_21	/* CompactFlash Ready signal */
#define	SA1110DB_CF_CD		SA1100_GPIO_22	/* CompactFlash Card Detect */
#define	SA1110DB_UCB_IRQ	SA1100_GPIO_23	/* UCB1300 IRQ */
#define	SA1110DB_CF_BVD2	SA1100_GPIO_24	/* CompactFlash BVD2 signal */
#define	SA1110DB_CF_BVD1	SA1100_GPIO_25	/* CompactFlash BVD1 signal */

#define	SA1110DB_SA1111_IRQ	SA1100_GPIO_25	/* SA1111 chip IRQ */

/* 
 * SA1111-DB (Neposet) Registers and Definitions 
 */
#define SA1111DB_CNTRL_REG0               0x100000A0  /* Physical Address */
#define SA1111DB_CNTRL_REG0_GP01_OFF      0x00000001  /* Bit Locations */
#define SA1111DB_CNTRL_REG0_TP_PWR_ON     0x00000002
#define SA1111DB_CNTRL_REG0_MS_PWR_ON     0x00000004
#define SA1111DB_CNTRL_REG0_ENET_OSC_EN   0x00000008
#define SA1111DB_CNTRL_REG0_SPI_KB_WKUP   0x00000010
#define SA1111DB_CNTRL_REG0_A0VPP         0x00000020
#define SA1111DB_CNTRL_REG0_A1VPP         0x00000040

#define SA1111DB_LEDS                     0x10000010  /* Physical Address */

#endif

/* __SRCVERSION("sa1110db.h $Rev: 657836 $"); */
