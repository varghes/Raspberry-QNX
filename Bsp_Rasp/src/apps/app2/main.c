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

/* Very basic example to demonstrate how to load an image and render it using
   GF. This example takes advantage of imglib default allocation routines which
   direct image data into allocated system RAM. You then simply wrap a surface
   around this memory and blit it to any other surface you wish. The code is
   quick and simple, although you must be aware that blitting an image from
   system RAM cannot be done in h/w, so the blit will fall back to software.
   This may be an important consideration depdending on image size and frequency
   of rendering. To have h/w accelerated blitting, you must allocate a surface
   using gf_surface_create() and locate your image data there. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "gf_setup.h"
#include "gffont.h"
#include <img/img.h>
#ifdef _MSC_VER
int optind = 1;
#endif

extern gf_format_t img_fmt_to_gf(img_format_t format);
extern const char *img_fmt_to_string(img_format_t format);
extern const img_color_t g8pal[];

int verbose = 0;

int
main(int argc, char *argv[])
{
	img_lib_t				ilib = NULL;
	gf_setup_t				setup;
	int						rc;
	img_t					img;
	gf_surface_t			img_surf;
	gf_palette_t			palette;
	int ixoff, iyoff, iw, ih, x1, y1;


#ifdef TESTING
	memset(&palette, 0, sizeof palette);


#ifdef _MSC_VER
	putenv("LIBIMG_CFGFILE=img.conf");
#endif
	/* initialize img library */
	if ((rc = img_lib_attach(&ilib)) != IMG_ERR_OK) {
		fprintf(stderr, "img_lib_attach() failed: %d\n", rc);
		return -1;
	}

	/* establish connection with GF */
	if ((rc = gf_setup(&setup, argc, argv, 0)) != GF_ERR_OK) {
		fprintf(stderr, "gf_setup() failed: %d\n", rc);
		return -1;
	}

	if (argv[optind] == NULL) {
		fprintf(stderr, "no image file specified\n");
		return -1;
	}

	/* initialize an img_t by setting its flags to 0 */
	img.flags = 0;

	/* if we want, we can preselect a format (ie force the image to be
	   loaded in the format we specify) by enabling the following two
	   lines */
#if 0
	img.format = IMG_FMT_PKLE_ARGB1555;
	img.flags |= IMG_FORMAT;
#endif

	/* likewise, we can 'clip' the loaded image by enabling the following */
#if 0
	img.w = 400;
	img.flags |= IMG_W;
#endif
#if 0
	img.h = 100;
	img.flags |= IMG_H;
#endif

	if ((rc = img_load_file(ilib, argv[optind], NULL, &img)) != IMG_ERR_OK) {
		fprintf(stderr, "img_load_file(%s) failed: %d\n", argv[optind], rc);
		return -1;
	}

	fprintf(stdout, "img is %dx%dx%d (%s)\n", img.w, img.h, IMG_FMT_BPP(img.format),
		img_fmt_to_string(img.format));

	/* for our purposes we're done with the img lib */
	img_lib_detach(ilib);

	if (img.format & IMG_FMT_PALETTE) {
		/* setup palette if necessary */
		palette.ncolors = img.npalette;
		palette.colors = img.palette;
	} else if (img.format == IMG_FMT_G8) {
		/* we can render G8 images in GF by using a palette of grays */
		palette.ncolors = 256;
		palette.colors = (img_color_t*)g8pal;
	}

	/* attach a surface to the image data; this allows us to blit the image
	   data to another surface (or the display) in GF */
	if ((rc = gf_surface_attach(&img_surf, setup.gdev,
		img.w, img.h, img.access.direct.stride, img_fmt_to_gf(img.format),
		&palette, img.access.direct.data, 0)) != GF_ERR_OK) {
		fprintf(stderr, "gf_surface_attach() failed: %d\n", rc);

		/* might fail here if the decoder gave us a format that cannot
		   map to GF; in this case we could have preselected a format
		   that is supported by GF (code above shows how to do this) */

		return -1;
	}

	/* lock the h/w for rendering */
	if (gf_draw_begin(setup.context) != GF_ERR_OK) {
		fprintf(stderr, "gf_draw_begin() failed: %d\n", rc);
		return -1;
	}

	/* blank the screen with a checkerboard pattern */
	gf_context_set_fgcolor(setup.context, 0xffffff);
	gf_context_set_bgcolor(setup.context, 0xdddddd);
	gf_context_set_pattern(setup.context,
		(unsigned char *)"\x0f\x0f\x0f\x0f\xf0\xf0\xf0\xf0",
		0, 0, 
		GF_CONTEXT_PATTERN_BACKFILL);
	gf_draw_rect(setup.context, 0, 0, setup.display_info.xres, setup.display_info.yres);

	if (img.flags & IMG_TRANSPARENCY) {
		/* we can handle transparency in GF using chroma */

		gf_chroma_t	chroma;
		memset(&chroma, 0, sizeof chroma);
		chroma.mode = GF_CHROMA_OP_SRC_MATCH | GF_CHROMA_OP_NO_DRAW;
		if (img.format & IMG_FMT_PALETTE) {
			chroma.color0 = img.palette[img.transparency.index];
		} else if (IMG_FMT_BPP(img.format) < 24) {
			chroma.color0 = img.transparency.rgb16;
		} else {
			chroma.color0 = img.transparency.rgb32;
		}

		gf_context_set_chroma(setup.context, &chroma);
	}

	if (img.format & IMG_FMT_ALPHA) {
		gf_alpha_t	alpha;
		memset(&alpha, 0, sizeof alpha);
		alpha.mode = GF_ALPHA_M1_SRC_PIXEL_ALPHA | GF_BLEND_SRC_M1 | GF_BLEND_DST_1mM1;
		gf_context_set_alpha(setup.context, &alpha);
	}

	/*
	 * If image is larger than our screen then display lower right corner on screen.
	 * If image is smaller then center image on screen.
	 */
	ixoff = iyoff = 0;
	x1 = y1 = 0;
	iw = img.w; 
	ih = img.h;
	if(iw > setup.display_info.xres) {
		ixoff = iw - setup.display_info.xres;
		iw -= ixoff;
	} else
		x1 = (setup.display_info.xres-iw)/2;
	if(ih > setup.display_info.yres) {
		iyoff = ih - setup.display_info.yres;
		ih -= iyoff;
	} else
		y1 = (setup.display_info.yres-ih)/2;

	/* render the loaded image */
	gf_draw_blit2(setup.context, img_surf, NULL,
		ixoff, iyoff, img.w - 1, img.h - 1, x1, y1);
	
	/* it's a good idea to do this before we free the image to ensure the
	   renderer is done with the data */
	gf_draw_finish(setup.context);

	/* unlock the h/w */
	gf_draw_end(setup.context);

	if (font_init(&setup) == -1) {
		fprintf(stderr, "Font Init failed\n");
		return(-1);
	}

	if (img.format & IMG_FMT_ALPHA) {
		gf_context_disable_alpha(setup.context);
	}

	if (img.flags & IMG_TRANSPARENCY) {
		gf_context_disable_chroma(setup.context);
	}

	/* release the attached surface; we're not going to blit from it any
	   more (in real life this surface could be recycled if needed) */
	gf_surface_free(img_surf);

	/* above code only releases the surface; we still have the actual image
	   data hanging around. We relied on the library to allocate this for
	   us. The following is all that's needed when we rely on this default
	   behaviour (note that this free() takes care of the palette also,
	   if applicable, since the lib allocates it all in one chunk) */
	free(img.access.direct.data);

	/*
	 * wait for a signal to exit
	 */
	pause();
#endif

	printf("I am application 2\n");

	return 0;
}

__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/6.4.1/trunk/apps/gf/img_decode_simple/main.c $ $Rev: 217583 $" );
