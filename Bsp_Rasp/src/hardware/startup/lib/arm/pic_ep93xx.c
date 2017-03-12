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

#include "startup.h"
#include <pic_ep93xx.h>

/* TODO - this file is a duplicate of ppc/pic_85xx.c. These files (and the
 * associated pic_85xx.h/pic_ep93xx.h need to be harmonized
*/

/*
 * =============================================================================
 *                                M A C R O S
 * =============================================================================
*/

/*
 * =============================================================================
 *                   F O R W A R D   D E C L A R A T I O N S
 * =============================================================================
*/

/*
 * =============================================================================
 *                               A P I 's
 * =============================================================================
*/


/*******************************************************************************
 * valid_pic_id
 * 
 * The interrupt controller contain a peripheral ID. We will make sure that they
 * are a supported value since a change may reflect a change in behaviour which
 * may signal a needed code change. We catch that here
 * 
 * Returns: true (!0) or false (0) depending on whether both VIC's are supported
*/
unsigned valid_pic_id(void)
{
	uint32_t vic1_id;
	uint32_t vic2_id;
	
	vic1_id = (((in32(VIC1(EP93xx_VIC_PERIPHERAL_ID0)) & 0xFFU) << 0) |
			   ((in32(VIC1(EP93xx_VIC_PERIPHERAL_ID1)) & 0xFFU) << 8) |
			   ((in32(VIC1(EP93xx_VIC_PERIPHERAL_ID2)) & 0xFFU) << 16) |
			   ((in32(VIC1(EP93xx_VIC_PERIPHERAL_ID3)) & 0xFFU) << 24));

	vic2_id = (((in32(VIC2(EP93xx_VIC_PERIPHERAL_ID0)) & 0xFFU) << 0) |
			   ((in32(VIC2(EP93xx_VIC_PERIPHERAL_ID1)) & 0xFFU) << 8) |
			   ((in32(VIC2(EP93xx_VIC_PERIPHERAL_ID2)) & 0xFFU) << 16) |
			   ((in32(VIC2(EP93xx_VIC_PERIPHERAL_ID3)) & 0xFFU) << 24));
	
	if (debug_flag > 1) kprintf("VIC1 id: %x, VIC2 id: %x\n", vic1_id, vic2_id);
	return ((vic1_id == 0x00041190) && (vic1_id == vic2_id));
}

/*******************************************************************************
* set_config_flags
* clr_config_flags
* 
* set or clear <flags> in the optional config flags table.
* 
* Return: the new flags value. Note that 0 will be returned if rv->config_flags
* 			is NULL
*/
cfg_flags_t set_config_flags(vector_tbl_t *rv, unsigned n, cfg_flags_t flags)
{
	if (rv->config_flags != NULL) rv->config_flags[n] |= flags;
	return rv->config_flags[n];
}
cfg_flags_t clr_config_flags(vector_tbl_t *rv, unsigned n, cfg_flags_t flags)
{
	if (rv->config_flags != NULL) rv->config_flags[n] &= ~flags;
	return rv->config_flags[n];
}

/*******************************************************************************
* set_regoffset
* 
* enter the register offset value <reg_off> in to the vector table <rv> at entry
* <n>. If a config_flags table is provided, make sure that the
* INTR_CONFIG_FLAG_DISALLOWED flag is turned off allowing the vector to be
* attached too.
*/
void set_regoffset(vector_tbl_t *rv, unsigned n, reg_off_t reg_off)
{
	rv->offset[n] = reg_off;
	clr_config_flags(rv, n, INTR_CONFIG_FLAG_DISALLOWED);
}



__SRCVERSION("pic_85xx.c $Rev: 220953 $");
