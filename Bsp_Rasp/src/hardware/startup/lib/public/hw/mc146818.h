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
 * mc146818.h:	MC146818 register definitions
 *

 *
 */
#define MC146818_SRA		0x0A	/* MC146818 Status Register A */
#define MC146818_SRA_UIP	0x80	/* Update in progress */

#define	MC146818_SRB		0x0B	/* MC146818 Status Register B */
#define MC146818_SRB_DSE	0x01
#define MC146818_SRB_24_12	0x02
#define MC146818_SRB_DM		0x04
#define MC146818_SRB_SQWE	0x08
#define MC146818_SRB_UIE	0x10
#define MC146818_SRB_AIE	0x20	/* Alarm interrupt enable */
#define MC146818_SRB_PIE	0x40
#define MC146818_SRB_SET 	0x80 	/* Snap User Copy of time */

#define	MC146818_SRC		0x0C		/* MC146818 Status Register C */
#define MC146818_SRD		0x0D		/* MC146818 Status Register D */

extern int      rtc_read(struct tm *tmbuf);
extern int      rtc_write(struct tm *tmbuf);
extern unsigned __rcmos(unsigned addr);
extern unsigned __wcmos(unsigned addr, unsigned value);

/* __SRCVERSION("mc146818.h $Rev: 657836 $"); */
