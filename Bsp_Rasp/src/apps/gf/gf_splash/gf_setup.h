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

#ifndef _GF_SETUP_H_INCLUDED
#define _GF_SETUP_H_INCLUDED

#ifndef _GF_H_INCLUDED
#include <gf/gf.h>
#endif
#include <FontFusion/T2K/t2k.h>
#include <FontFusion/T2K/t2kextra.h>

__BEGIN_DECLS

typedef struct {
	gf_dev_t			gdev;
	gf_display_t		display;
	gf_display_info_t	display_info;
	gf_layer_t			layer;
	gf_surface_t		surface1, surface2;
	gf_context_t		context;
	unsigned			x1, y1, x2, y2;
	const char			*fname;
	T2K					*scaler;
} gf_setup_t;

extern int gf_setup(gf_setup_t *setup,
	int argc,
	char * const argv[],
	unsigned flags);

#define GF_SETUP_LAYER_MAIN		(-1)		/* use main display layer */

#define GF_SETUP_FLAG_DBLBUFFER	0x00000001	/* create 2 surfaces/contexts */
#define GF_SETUP_FLAG_NOCONTEXT	0x00000002	/* do NOT create contexts */

__END_DECLS

#endif

__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/6.4.1/trunk/apps/gf/img_decode_simple/gf_setup.h $ $Rev: 219612 $" )
