/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. 
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

#ifndef _PIC_EP93XX_H_
#define _PIC_EP93XX_H_

#include <stdint.h>
#include <arm/ep93xx.h>
#include "arm/ep93xx_private.h"

/*
 * =============================================================================
 *                                T Y P E S
 * =============================================================================
*/

/* define REG_OFF_T and CFG_OFF_T to desired type before including this file */
#ifndef REG_OFF_T
#define REG_OFF_T	uint32_t
#endif	/* REG_OFF_T */

#ifndef CFG_FLAGS_T
#define CFG_FLAGS_T	uint32_t
#endif	/* CFG_FLAGS_T */

typedef REG_OFF_T	reg_off_t;
typedef CFG_FLAGS_T	cfg_flags_t;
/*
 * vector_tbl_t
 * 
 * TODO - not nuts about the name of this type. If it is going to apply to all
 * 			architectures, can't assume uint16_t's for the 'start' and
 * 			'dst_vectbl_off' fields
*/

typedef struct
{
	uint16_t	num;			// number of vectors in 'offset[]'
	uint16_t	start;			// the starting internal PIC vector number
	uint16_t	dst_tbl_off;	// offset in destination table to copy (applies to both offset and config flags)
	uint16_t	spare;
	reg_off_t	*offset;		// pointer to start of 'num' PIC register offsets
	cfg_flags_t	*config_flags;	// pointer to start of 'num' config flags (optional)
} vector_tbl_t;


/*
 * =============================================================================
 *                                M A C R O S
 * =============================================================================
*/
#define EP93xx_INTR_CLASS		_NTO_INTR_CLASS_EXTERNAL

#define VIC1(_o_)	(EP93xx_VIC_CTRL1_BASE + (_o_))
#define VIC2(_o_)	(EP93xx_VIC_CTRL2_BASE + (_o_))

#define EP93xx_VECTORED_INT		(1U << 5)
#define EP93xx_VECTOR(n)		(n % 32)

/*
 * =============================================================================
 *                   F O R W A R D   D E C L A R A T I O N S
 * =============================================================================
*/
extern unsigned valid_pic_id(void);
extern cfg_flags_t set_config_flags(vector_tbl_t *rv, unsigned n, cfg_flags_t flags);
extern cfg_flags_t clr_config_flags(vector_tbl_t *rv, unsigned n, cfg_flags_t flags);
extern void set_regoffset(vector_tbl_t *rv, unsigned n, reg_off_t val);

extern struct callout_rtn interrupt_mask_ep93xx;
extern struct callout_rtn interrupt_unmask_ep93xx;
extern struct callout_rtn interrupt_id_ep93xx;
extern struct callout_rtn interrupt_eoi_ep93xx;
extern struct callout_rtn interrupt_config_ep93xx;

extern struct callout_rtn interrupt_mask_ep93xx_gpio_ab;
extern struct callout_rtn interrupt_unmask_ep93xx_gpio_ab;
extern struct callout_rtn interrupt_id_ep93xx_gpio_ab;
extern struct callout_rtn interrupt_eoi_ep93xx_gpio_ab;
extern struct callout_rtn interrupt_config_ep93xx_gpio_ab;

extern struct callout_rtn interrupt_mask_ep93xx_gpio_f;
extern struct callout_rtn interrupt_unmask_ep93xx_gpio_f;
extern struct callout_rtn interrupt_id_ep93xx_gpio_f;
extern struct callout_rtn interrupt_eoi_ep93xx_gpio_f;
extern struct callout_rtn interrupt_config_ep93xx_gpio_f;

extern uint32_t ep9301_config_pic(uint32_t os_vector_base, uint32_t pic_vector_base, vector_tbl_t *reg_vectors);
extern int ep93xx_set_vector_priority(unsigned vector, unsigned priority);



#endif	/* _PIC_EP93XX_H_ */


__SRCVERSION("pic_85xx.h $Rev: 222554 $");
