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

#include <string.h>
#include <img/img.h>
#include <gf/gf.h>

static const struct {
	img_format_t	format;
	const char	*string;
	gf_format_t	gf_format;
} fmt_table[] = {
	{ IMG_FMT_MONO, "IMG_FMT_MONO", GF_FORMAT_INVALID },
	{ IMG_FMT_G8, "IMG_FMT_G8", GF_FORMAT_PAL8 },
	{ IMG_FMT_PAL1, "IMG_FMT_PAL1", GF_FORMAT_INVALID },
	{ IMG_FMT_PAL4, "IMG_FMT_PAL4", GF_FORMAT_INVALID },
	{ IMG_FMT_PAL8, "IMG_FMT_PAL8", GF_FORMAT_PAL8 },
	{ IMG_FMT_PKLE_RGB565, "IMG_FMT_PKLE_RGB565", GF_FORMAT_PKLE_RGB565 },
	{ IMG_FMT_PKBE_RGB565, "IMG_FMT_PKBE_RGB565", GF_FORMAT_PKBE_RGB565 },
	{ IMG_FMT_PKLE_ARGB1555, "IMG_FMT_PKLE_ARGB1555", GF_FORMAT_PKLE_ARGB1555 },
	{ IMG_FMT_PKBE_ARGB1555, "IMG_FMT_PKBE_ARGB1555", GF_FORMAT_PKBE_ARGB1555 },
	{ IMG_FMT_PKLE_XRGB1555, "IMG_FMT_PKLE_XRGB1555", GF_FORMAT_PKLE_ARGB1555 },
	{ IMG_FMT_PKBE_XRGB1555, "IMG_FMT_PKBE_XRGB1555", GF_FORMAT_PKBE_ARGB1555 },
	{ IMG_FMT_BGR888, "IMG_FMT_BGR888", GF_FORMAT_BGR888 },
	{ IMG_FMT_RGB888, "IMG_FMT_RGB888", GF_FORMAT_BGR888 },
	{ IMG_FMT_PKLE_ARGB8888, "IMG_FMT_PKLE_ARGB8888", GF_FORMAT_BGRA8888 },
	{ IMG_FMT_PKBE_ARGB8888, "IMG_FMT_PKBE_ARGB8888", GF_FORMAT_INVALID },
	{ IMG_FMT_PKLE_XRGB8888, "IMG_FMT_PKLE_XRGB8888", GF_FORMAT_BGRA8888 },
	{ IMG_FMT_PKBE_XRGB8888, "IMG_FMT_PKBE_XRGB8888", GF_FORMAT_INVALID },
	{ IMG_FMT_RGBA8888, "IMG_FMT_RGBA8888", GF_FORMAT_BGRA8888 },
};

static const struct {
	gf_format_t	gformat;
	img_format_t	format;
} gf2img[] = {
	{ GF_FORMAT_PAL8, IMG_FMT_PAL8 },
	{ GF_FORMAT_PACK_RGB565, IMG_FMT_PKLE_RGB565 },
	{ GF_FORMAT_PKLE_RGB565, IMG_FMT_PKLE_RGB565 },
	{ GF_FORMAT_PKBE_RGB565, IMG_FMT_PKBE_RGB565 },
	{ GF_FORMAT_PACK_ARGB1555, IMG_FMT_PKLE_ARGB1555 },
	{ GF_FORMAT_PKLE_ARGB1555, IMG_FMT_PKLE_ARGB1555 },
	{ GF_FORMAT_PKBE_ARGB1555, IMG_FMT_PKBE_ARGB1555 },
	{ GF_FORMAT_BGR888, IMG_FMT_BGR888 },
	{ GF_FORMAT_BGRA8888, IMG_FMT_PKLE_ARGB8888 },
};

const char *
img_fmt_to_string(img_format_t format)
{
	int i;

	for (i = 0;i < sizeof (fmt_table) / sizeof (*fmt_table);++i) {
		if (fmt_table[i].format == format) {
			return fmt_table[i].string;
		}
	}

	return "unrecognized format";
}

img_format_t
img_fmt_from_string(const char *string)
{
	int i;

	for (i = 0;i < sizeof (fmt_table) / sizeof (*fmt_table);++i) {
		if (!stricmp(string, fmt_table[i].string)) {
			return fmt_table[i].format;
		}
	}

	return 0;
}

gf_format_t
img_fmt_to_gf(img_format_t format)
{
	int i;

	for (i = 0;i < sizeof (fmt_table) / sizeof (*fmt_table);++i) {
		if (fmt_table[i].format == format) {
			return fmt_table[i].gf_format;
		}
	}

	return GF_FORMAT_INVALID;
}

img_format_t
img_fmt_from_gf(gf_format_t format)
{
	int i;

	for (i = 0;i < sizeof (gf2img) / sizeof (*gf2img);++i) {
		if (gf2img[i].gformat == format) {
			return gf2img[i].format;
		}
	}

	return IMG_FMT_INVALID;
}

__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/6.4.1/trunk/apps/gf/img_decode_simple/img_fmt.c $ $Rev: 217583 $" );
