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




/* Types and defines for Video Overlay / Scaler support */
#ifndef _GRAPHICS_VID_H_INCLUDED
#define _GRAPHICS_VID_H_INCLUDED

#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#include <graphics/display.h>
#endif

#define DISP_VID_MAX_PLANES	3
#define DISP_VID_MAX_ALPHA	4

/* Describes an alpha-blended area of the video viewport */
typedef struct {
	unsigned char	src_alpha, dst_alpha;	/* Blending factors */
	unsigned 	alpha_mode;		/* Blending mode */
	unsigned short	x1, y1, x2, y2;
} disp_vid_alpha_t;

/* YUV data formats			FourCC		   Layout		H sample (YUV)	V sample (YUV)	BPP */
#define DISP_PACKED_YUV_FORMAT_IYU1	0x31555949	/* U2Y2Y2V2Y2Y2		144		111		12  */
#define DISP_PACKED_YUV_FORMAT_IYU2	0x32555949	/* U4Y4V4U4Y4V4		111		111		24  */
#define DISP_PACKED_YUV_FORMAT_UYVY	0x59565955	/* U8Y8V8Y8		122		111		16  */
#define DISP_PACKED_YUV_FORMAT_YUY2	0x32595559	/* Y8U8Y8V8		122		111		16  */
#define DISP_PACKED_YUV_FORMAT_YVYU	0x55595659	/* Y8V8Y8U8		122		111		16  */
#define DISP_PACKED_YUV_FORMAT_V422	0x56343232	/* V8Y8U8Y8		122		111		16  */
#define DISP_PACKED_YUV_FORMAT_CLJR	0x524a4c43	/* V6U6Y5Y5Y5Y5		133		111		8   */

#define DISP_PLANAR_YUV_FORMAT_YVU9	0x39555659	/* YVU			144		144		9   */
#define DISP_PLANAR_YUV_FORMAT_YV12	0x32315659	/* YUV			122		122		12  */

/* There doesn't seem to be a FourCC for this one */
#define DISP_PLANAR_YUV_FORMAT_YUV420	0x00000100	/* YUV			122		111		16  */

/* These formats are the same as YV12, except the U and V planes do not have to contiguously follow the Y plane */
#define DISP_PLANAR_YUV_FORMAT_CLPL	DISP_PLANAR_YUV_FORMAT_YV12	/* Cirrus Logic Planar format */
#define DISP_PLANAR_YUV_FORMAT_VBPL	DISP_PLANAR_YUV_FORMAT_YV12	/* VooDoo Banshee planar format */

#define DISP_VID_CAP_SRC_CHROMA_KEY	0x00001000	/* Video viewport supports chroma-keying on frame data */
#define DISP_VID_CAP_DST_CHROMA_KEY	0x00002000	/* Video viewport supports chroma-keying on desktop data */
#define	DISP_VID_CAP_BUSMASTER		0x00008000	/* Scaler device can bus-master the data from system ram */
#define	DISP_VID_CAP_DOUBLE_BUFFER	0x00010000	/* Scaler channel can be double-buffered */
#define	DISP_VID_CAP_BRIGHTNESS_ADJUST	0x00080000	/* Brightness of video viewport can be adjusted */
#define	DISP_VID_CAP_CONTRAST_ADJUST	0x00100000	/* Contrast of video viewport can be adjusted */

/* General capabilities of a Video Scaler, for a given format */
typedef struct {
	unsigned short	size;			/* Size of this struct */
	unsigned short	reserved0;
	unsigned	flags;
	unsigned	format;
	int		src_max_x;		/* Maximum width of source frames */
	int		src_max_y;		/* Maximum height of source frames */
	int		max_mag_factor_x;	/* Magnification - 1 means cannot scale upwards */
	int		max_mag_factor_y;	/* Magnification - 1 means cannot scale upwards */
	int		max_shrink_factor_x;	/* 1 means cannot scale downwards */
	int		max_shrink_factor_y;	/* 1 means cannot scale downwards */
	unsigned	reserved[8];
} disp_vid_channel_caps_t;

/* disp_vid_channel_props.flags */
#define DISP_VID_FLAG_ENABLE		0x00000001	/* Enable the video viewport */
#define DISP_VID_FLAG_CHROMA_ENABLE	0x00000002	/* Perform chroma-keying */
#define DISP_VID_FLAG_DOUBLE_BUFFER	0x00000004	/* Perform double-buffering */
#define DISP_VID_FLAG_DRIVER_DOES_COPY	0x00000010	/* Driver performs the copy of frame data in the nextframe routine */
#define DISP_VID_FLAG_APP_DOES_COPY	0x00000020	/* Driver copies the frame data adter calling the nextframe routine */
#define DISP_VID_FLAG_DISABLE_FILTERING	0x00000040	/* Do scaling by replication, turn of bilear filtering, interpolation etc. */
#define DISP_VID_FLAG_DRAWABLE_SURFACE	0x00000080	/* Want to be able to draw to the video surfaces */
#define DISP_VID_FLAG_TO_FRONT		0x00000100	/* This overlay should be brought in front of any other overlays */
#define DISP_VID_FLAG_TO_BACK		0x00000200	/* This overlay should be put underneath any other overlays */

/* disp_vid_channel_props.chroma_mode */
#define DISP_VID_CHROMA_FLAG_DST	0x00000001	/* Perform chroma test on desktop data */
#define DISP_VID_CHROMA_FLAG_SRC	0x00000002	/* Perform chroma test on video frame data */

/* Configurable properties of a video scaler channel */
typedef struct {
	unsigned short		size;
	unsigned short		reserved0;
	unsigned		flags;
	unsigned		format;			/* Format of the frame data */
	disp_color_t		chroma_key0;		/* Chroma-key color */
	unsigned		reserved1;
	unsigned		chroma_flags;		/* Chroma-key comparison operation */
	disp_color_t		chroma_key_mask;	/* Colors are masked with this before chroma comparison */
	disp_color_t		chroma_mode;		/* Type of chroma key match to perform */
	int			x1, y1;			/* Top left corner of video viewport in display coords*/
	int			x2, y2;			/* Bottom right corner of video viewport in display coords */
	int			src_width, src_height;	/* Dimensions of the video source data */
	unsigned		fmt_index;		/* Selects the format of the source frame data */
	short			brightness;		/* Brightness adjust.  0x7fff = normal, 0 darkest, 0xffff brightest */
	short			contrast;		/* Contrast adjust.  0x7fff = normal, 0 minimum, 0xffff maximun */
	disp_vid_alpha_t	alpha[DISP_VID_MAX_ALPHA];	/* Regions of the video viewport to be blended with desktop */
	unsigned		reserved[8];
} disp_vid_channel_props_t;

__BEGIN_DECLS

typedef struct disp_vidfuncs {
	int (*init)(disp_adapter_t *adapter, char *optstring);
	void (*fini)(disp_adapter_t *adapter);
	void (*module_info)(disp_adapter_t *adapter, disp_module_info_t *info);
	int (*get_channel_caps)(disp_adapter_t *adapter, int channel,
	    int fmt_index, disp_vid_channel_caps_t *caps);
	int (*set_channel_props)(disp_adapter_t *adapter, int channel,
	    disp_vid_channel_props_t *props,
	    disp_surface_t **yplane1, disp_surface_t **yplane2,
	    disp_surface_t **uplane1, disp_surface_t **uplane2,
	    disp_surface_t **vplane1, disp_surface_t **vplane2);
	int (*next_frame)(disp_adapter_t *adapter, int channel);
	int (*close_channel)(disp_adapter_t *adapter, int channel);
} disp_vidfuncs_t;

/* Main video overlay module entry point */
typedef int (*get_vidfuncs_t)(disp_adapter_t *adapter,
    disp_vidfuncs_t *funcs, int tabsize);

extern int devg_get_vidfuncs(disp_adapter_t *adapter,
    disp_vidfuncs_t *funcs, int tabsize);

__END_DECLS

#endif /* _GRAPHICS_VID_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/vid.h $ $Rev: 657836 $" )
