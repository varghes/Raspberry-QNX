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
 * Primecell SP804 dual timer
 */

#ifndef __ARM_SP804_H_INCLUDED
#define __ARM_SP804_H_INCLUDED

#define SP804_SIZE		0x1000

#define	SP804_LOAD		0x00
#define SP804_VALUE		0x04
#define	SP804_CTRL		0x08
#define SP804_INTCLR	0x0c

#define SP804_CTRL_EN				(1<<7)
#define SP804_CTRL_PERIODIC			(1<<6)
#define SP804_CTRL_INTEN			(1<<5)
#define SP804_CTRL_PRESCALE_MASK	(3<<2)
#define SP804_CTRL_PRESCALE_1		(0<<2)
#define SP804_CTRL_PRESCALE_16		(1<<2)
#define SP804_CTRL_PRESCALE_256		(2<<2)
#define SP804_CTRL_32BIT			(1<<1)
#define SP804_CTRL_ONESHOT			(1<<0)

#endif	/* __ARM_SP804_H_INCLUDED */

/* __SRCVERSION("sp804.h $Rev: 657836 $"); */
