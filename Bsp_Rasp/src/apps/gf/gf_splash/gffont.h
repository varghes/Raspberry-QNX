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

#ifndef _GFTEXT_H
#define _GFTEXT_H

#ifndef _MSC_VER
#include <inttypes.h>
#endif

#include <gf/gf.h>
#include "gf_setup.h"

typedef struct {
	int width;
	int height;
	int line_width;
	int layer_format;
	gf_dev_t gdev;
	gf_layer_t layer;
	gf_surface_t surface;
} app_info_t;

enum {
	DRAW_STRING_BITMAP,
	DRAW_STRING_ALPHA,
	DRAW_STRING_BORDERED,
};

int font_init(gf_setup_t *info);
#endif

__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/6.4.1/trunk/apps/gf/font-test/gffont.h $ $Rev: 219612 $" )
