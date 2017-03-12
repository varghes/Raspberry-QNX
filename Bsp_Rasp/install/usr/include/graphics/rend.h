/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008, QNX Software Systems.  
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



#ifndef _GRAPHICS_REND_H_INCLUDED
#define _GRAPHICS_REND_H_INCLUDED

typedef struct disp_rend_buffer_config {
	unsigned	color_format;				/* See DISP_LAYER_FORMAT_* */
	unsigned	read_format;				/* See DISP_TEXFORMAT_* */
	unsigned	red_bpp;				/* Red bits per pixel of color buffer */
	unsigned	green_bpp;				/* Green bits per pixel of color buffer */
	unsigned	blue_bpp;				/* Blue bits per pixel of color buffer */
	unsigned	alpha_bpp;				/* Alpha bits per pixel of color buffer */
	unsigned	depth_bpp;				/* Bits per pixel of depth buffer */
	unsigned	stencil_bpp;				/* Bits per pixel of stencil buffer */
	int		max_surf_width;				/* Maximum width of a buffer, in pixels, that can be rendered to */
	int		max_surf_height;			/* Maximum height of a buffer, in pixels, that can be rendered to */
	int		slow_config;
	unsigned	sample_buffers;
	unsigned	samples;
	unsigned	reserved[6];				/* Must be zero */
} disp_rend_buffer_config_t;

typedef struct disp_rend_context_params {
	void * (*get_current_context)();
} disp_rend_context_params_t;

#endif /* _GRAPHICS_REND_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/rend.h $ $Rev: 657836 $" )
