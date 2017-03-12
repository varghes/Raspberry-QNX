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

#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include "gf_setup.h"
#include "gffont.h"


static int
find_rgb_format(gf_layer_t layer)
{
	gf_format_t format;
	unsigned criteria[] = {
		GF_2D_RENDERABLE, GF_TRUE,
		GF_RED_SIZE, 1,
		GF_GREEN_SIZE, 1,
		GF_BLUE_SIZE, 1,
		GF_NONE
	};

	if (gf_layer_choose_format(layer, &format, 1, criteria) < 1) {
		return -1;
	}

	return format;
}

/* set up a connection to GF using standard command-line switches:
   -D device Name (or index) of device to attach to (default is first)
   -d dispno Display index on device (default is 0)
   -l lindex Layer index (default is main layer)
   -f path to font file (.ttf format, required param) */

int
gf_setup(gf_setup_t *setup,
	int argc,
	char * const argv[],
	unsigned flags)
{
	gf_dev_t	gdev;
	unsigned	dispno = 0;
	const char 	*dev_name = NULL;
	int			layer_idx = GF_SETUP_LAYER_MAIN,
				rc,
				verbose = 0;
	gf_sid_t	sid = GF_SID_INVALID;
	unsigned	vx1 = 0, vx2 = ~0,
				vy1 = 0, vy2 = ~0;

	

	memset(setup, 0, sizeof *setup);

	setup->fname = strdup("tt0419m_.ttf");

#ifdef _MSC_VER
#else
	opterr = 0;

	while ((rc = getopt(argc, argv, "d:D:l:s:vx:y:f:")) != -1) {
		switch (rc) {
			case 'd':
				if (isdigit(*optarg)) {
					dev_name = GF_DEVICE_INDEX(atoi(optarg));
				} else {
					dev_name = optarg;
				}
				break;
			case 'D':
				dispno = atoi(optarg);
				break;
			case 'l':
				layer_idx = atoi(optarg);
				break;
			case 's':
				sid = (gf_sid_t)atoi(optarg);
				break;
			case 'v':
				verbose = ~0;
				break;
			case 'x':
				switch (*optarg++) {
					case '1':
						vx1 = atoi(optarg);
						break;
					case '2':
						vx2 = atoi(optarg);
						break;
					default:
						break;
				}

				break;
			case 'y':
				switch (*optarg++) {
					case '1':
						vy1 = atoi(optarg);
						break;
					case '2':
						vy2 = atoi(optarg);
						break;
					default:
						break;
				}

				break;
			case 'f':
				setup->fname = strdup(optarg);
				break;
			default:
				break;
		}
	}

#endif

	if ((rc = gf_dev_attach(&gdev, dev_name, NULL)) == GF_ERR_OK) {
		if (sid != GF_SID_INVALID) {
			gf_surface_t	surface;

			if ((rc = gf_surface_attach_by_sid(&surface, gdev, sid)) == GF_ERR_OK) {
				gf_context_t	context;

				if ((rc = gf_context_create(&context)) == GF_ERR_OK) {
					if ((rc = gf_context_set_surface(context, surface)) == GF_ERR_OK) {
						gf_surface_info_t	sinfo;

						gf_surface_get_info(surface, &sinfo);
						--sinfo.w;
						--sinfo.h;

						if (vx2 > sinfo.w) {
							vx2 = sinfo.w;
						}

						if (vy2 > sinfo.h) {
							vy2 = sinfo.h;
						}

						if (vx1 <= vx2 && vy1 <= vy2 &&
							(vx1 || vy1 || vx2 < sinfo.w || vy2 < sinfo.h)) {
							gf_context_set_clipping(context, vx1, vy1, vx2, vy2);
							setup->x1 = vx1;
							setup->x2 = vx2;
							setup->y1 = vy1;
							setup->y2 = vy2;
						}

						setup->surface1 = surface;
						setup->context = context;
						setup->gdev = gdev;

						return GF_ERR_OK;
					}

					gf_context_free(context);
				}

				gf_surface_free(surface);
			}
		} else {
			gf_display_t display;
			gf_display_info_t dinfo;

			if ((rc = gf_display_attach(&display, gdev, dispno, &dinfo)) == GF_ERR_OK) {
				gf_layer_t	layer;

				if (verbose) {
					fprintf(stderr, "Display %d:\n\tResolution: %dX%d\n",
						dispno, dinfo.xres, dinfo.yres);
					fprintf(stderr, "\tRefresh rate: %d\n\tPixel format: ", dinfo.refresh);
					switch (dinfo.format) {
						case GF_FORMAT_PAL8:
							fprintf(stderr, "PAL8");
							break;
						case GF_FORMAT_PACK_ARGB1555:
							fprintf(stderr, "ARGB1555");
							break;
						case GF_FORMAT_PACK_RGB565:
							fprintf(stderr, "RGB565");
							break;
						case GF_FORMAT_BGR888:
							fprintf(stderr, "BGR888");
							break;
						case GF_FORMAT_BGRA8888:
							fprintf(stderr, "BGRA8888");
							break;
						default:
							fprintf(stderr, "%x", (unsigned)dinfo.format);
							break;
					}

					fprintf(stderr, "\n\tNumber of layers: %d\n", dinfo.nlayers);
					fprintf(stderr, "\tMain layer index: %d\n", dinfo.main_layer_index);
				}

				if ((rc = gf_layer_attach(&layer, display, 
					layer_idx == GF_SETUP_LAYER_MAIN ? dinfo.main_layer_index : layer_idx,
					0)) == GF_ERR_OK) {
					gf_surface_t surface1, surface2 = NULL;
					int layer_format;

					layer_format = find_rgb_format(layer);
					if(verbose && layer_format != dinfo.format) {
						fprintf(stderr, "\tChoosing non-default layer format: ");
						switch (layer_format) {
							case GF_FORMAT_PAL8:
								fprintf(stderr, "PAL8");
								break;
							case GF_FORMAT_PACK_ARGB1555:
								fprintf(stderr, "ARGB1555");
								break;
							case GF_FORMAT_PACK_RGB565:
								fprintf(stderr, "RGB565");
								break;
							case GF_FORMAT_BGR888:
								fprintf(stderr, "BGR888");
								break;
							case GF_FORMAT_BGRA8888:
								fprintf(stderr, "BGRA8888");
								break;
							default:
								fprintf(stderr, "%x", (unsigned)layer_format);
								break;
						}
						fprintf(stderr, "\n");
					}
					dinfo.format = layer_format;

					if ((rc = gf_surface_create_layer(&surface1, &layer, 1, 0,
						dinfo.xres, dinfo.yres, 
						dinfo.format, NULL, 0)) == GF_ERR_OK &&
						(!(flags & GF_SETUP_FLAG_DBLBUFFER) || 
						 (rc = gf_surface_create_layer(&surface2, &layer, 1, 0,
						 dinfo.xres, dinfo.yres,
						 dinfo.format, NULL, 0)) == GF_ERR_OK)) {

						if (verbose) {
							gf_surface_info_t	sinfo;

							gf_surface_get_info(surface1, &sinfo);
							fprintf(stderr, "Surface:\n\tSID = %d\n", (unsigned)sinfo.sid);
							fprintf(stderr, "\tFlags = %x\n", sinfo.flags);
						}

						gf_layer_set_surfaces(layer, &surface1, 1);

						if ((rc = gf_layer_update(layer, 0)) == GF_ERR_OK) {
							gf_context_t context = NULL;

							if ((flags & GF_SETUP_FLAG_NOCONTEXT) ||
								((rc = gf_context_create(&context)) == GF_ERR_OK &&
								 (rc = gf_context_set_surface(context, surface1)) == GF_ERR_OK)) {
								setup->gdev = gdev;
								setup->display = display;
								memcpy(&setup->display_info, &dinfo, sizeof dinfo);
								setup->layer = layer;
								setup->surface1 = surface1;
								setup->surface2 = surface2;
								setup->context = context;
								return GF_ERR_OK;
							}
					
							if (context != NULL) {		
								gf_context_free(context);
							}
						}
					}
				}
			}
		}
	
		gf_dev_detach(gdev);
	}

	return rc;
}

__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/6.4.1/trunk/apps/gf/img_decode_simple/gf_setup.c $ $Rev: 217583 $" );
