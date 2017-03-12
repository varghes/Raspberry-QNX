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



/* Types and defines for video capture support */

#ifndef _GRAPHICS_VCAP_H_INCLUDED
#define _GRAPHICS_VCAP_H_INCLUDED

#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#include <graphics/display.h>
#endif

/* disp_vcap_props.flags argument to "set_flags" */
#define	DISP_VCAP_FLAG_WEAVE		0x00000001
#define	DISP_VCAP_FLAG_BOB		0x00000002
#define	DISP_VCAP_FLAG_ODD_FRAMES_ONLY	0x00000004
#define	DISP_VCAP_FLAG_EVEN_FRAMES_ONLY	0x00000008

typedef struct disp_vcap_props {
	unsigned	flags;
	int		input_source;
	int		src_width;	/* Unscaled, uncropped source width */
	int		src_height;	/* Unscaled, uncropped source height */
	int		src_x1, src_y1;	/* Source crop rectangle upper left */
	int		src_x2, src_y2;	/* Source crop rectangle lower right */
	int		dst_width;	/* Post-scaling width */
	int		dst_height;	/* Post-scaling height */
	int		h_offset;	/* Horizontal sync offset */
	int		v_offset;	/* Vertical sync offset */
	int		h_total;	/* Pixels clocks per horizontal period */
	int		v_total;	/* Total scanlines per vertical period */
	intptr_t	reserved[6];
} disp_vcap_props_t;

typedef struct disp_vcap_adjust {
	int		brightness;	/* Range -128->127, 0 means "normal" */
	int		contrast;	/* Range -128->127, 0 means "normal" */
	int		saturation;	/* Range -128->127, 0 means "normal" */
	int		hue;		/* Range -128->127, 0 means "normal" */
	intptr_t	reserved[4];
} disp_vcap_adjust_t;

__BEGIN_DECLS

typedef struct disp_vcapfuncs {
	int (*init)(disp_adapter_t *adapter, char *optstring);
	void (*fini)(disp_adapter_t *adapter);
	void (*module_info)(disp_adapter_t *adapter, disp_module_info_t *info);
	int (*set_props)(disp_adapter_t *adapter,
	    int unit, disp_vcap_props_t *props);
	int (*set_adjustments)(disp_adapter_t *adapter,
	    int unit, disp_vcap_adjust_t *adjustments);
	int (*bind_layer)(disp_adapter_t *adapter, int unit,
	    int dispno, int layer_idx);
	int (*set_enabled)(disp_adapter_t *adapter, int unit, int enabled);
	void (*wait_vsync)(disp_adapter_t *adapter, int unit);
	int (*set_power_mode)(disp_adapter_t *adapter,
	    int unit, pm_power_mode_t mode);

	void (*reserved[8])();
} disp_vcapfuncs_t;

/* Main video capture module entry point */
typedef int (*get_vcapfuncs_t)(disp_adapter_t *adapter,
    disp_vcapfuncs_t *funcs, int tabsize);

extern int devg_get_vcapfuncs(disp_adapter_t *adapter,
    disp_vcapfuncs_t *funcs, int tabsize);

__END_DECLS

#endif /* _GRAPHICS_VCAP_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/vcap.h $ $Rev: 657836 $" )
