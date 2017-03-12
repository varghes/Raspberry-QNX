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





#ifndef _GRAPHICS_DRAW_H_INCLUDED
#define _GRAPHICS_DRAW_H_INCLUDED

#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#include <graphics/display.h>
#endif

typedef struct disp_2d_caps {
	int		size;			/* Size of this struct */
	unsigned	accel_flags;		/* Accelerated features */
	unsigned	flags;			/* Other driver properties */
	int		min_stride;		/* Minimum surface stride for 2d drawing */
	int		max_stride;		/* Maximum surface stride for 2d drawing */
	int		stride_gran;		/* stride (in bytes) must be a multiple of this */
	unsigned	poly_flags;		/* polygon classes than can be handled (DISP_POLY_CLASS_*) */
	unsigned	poly_max_points;	/* Max no. of points in a single polygon that h/w can handle */
} disp_2d_caps_t;

/* disp_2d_caps_t.accel_flags - flags defining which 2D operations are hardware accelerated */
#define DISP_2D_ACCEL_OPAQUE_BLIT		0x00000001	/* Simple, opaque blit operations */
#define DISP_2D_ACCEL_OPAQUE_FILL		0x00000001	/* Simple, opaque fill operations */
#define DISP_2D_ACCEL_MONO_PAT			0x00000002	/* Simple, draw operations with two colors and a pattern */
#define DISP_2D_ACCEL_TRANS_PAT			0x00000004	/* Simple, draw operations with a transparency pattern */
#define DISP_2D_ACCEL_SIMPLE_ROPS		0x00000008	/* Copy, XOR, AND and OR with a pattern */
#define DISP_2D_ACCEL_COMPLEX_ROPS		0x00000010	/* All 256 rop3 codes accelerated */
#define DISP_2D_ACCEL_SRCALPHA_BLEND_GLOBAL	0x00000020	/* Alpha blending with a global source factor is supported */
#define DISP_2D_ACCEL_SRCALPHA_BLEND_PIXEL	0x00000040	/* Alpha blending with a per pixel source factor is supported */
#define DISP_2D_ACCEL_SRCALPHA_BLEND_MAP	0x00000400	/* Alpha blending with an alpha map as source is supported */
#define DISP_2D_ACCEL_DSTALPHA_BLEND_GLOBAL	0x00000800	/* Alpha blending with a global destination factor */
#define DISP_2D_ACCEL_DSTALPHA_BLEND_PIXEL	0x00001000	/* Alpha blending with a per pixel destination factor */
#define DISP_2D_ACCEL_DSTALPHA_BLEND_MAP	0x00002000	/* Alpha blending with an alpha map as destination is supported */
#define DISP_2D_ACCEL_SRC_CHROMA		0x00000080	/* Source image chroma keying supported */
#define DISP_2D_ACCEL_DST_CHROMA		0x00000100	/* Destination image chroma keying supported */
#define DISP_2D_ACCEL_SCALED_BLIT		0x00000200	/* Scaled BitBlit supported */
#define DISP_2D_ACCEL_BRESENHAM_LINES		0x00004000	/* At least opaque Bresenham line drawing */
#define DISP_2D_ACCEL_SIMPLE_POLYGONS		0x00008000	/* Can at least render opaque convex polygons */
#define DISP_2D_ACCEL_COMPLEX_POLYGONS		0x00010000	/* Can at least render opaque complex polygons */
#define DISP_2D_ACCEL_ANTIALIAS			0x00020000	/* Can at least render anti-alias lines */

/* disp_2d_caps_t.flags - miscellaneous 2d renderer characteristics */
#define DISP_2D_SRC_DST_STRIDE_EQUAL	0x00000001	/* Separate source and destination strides cannot be specified */
#define DISP_2D_EXCLUSIVE_VM_ACCESS	0x00000002	/* Not safe to access any video memory surface with the CPU while the 2D engine is busy */

/* disp_draw_context_t.flags */
#define	DISP_DRAW_FLAG_USE_DST_ALPHA		0x00000004
#define	DISP_DRAW_FLAG_USE_ALPHA		0x00000008
#define	DISP_DRAW_FLAG_USE_CHROMA		0x00000010
#define	DISP_DRAW_FLAG_MONO_PATTERN		0x00000020
#define	DISP_DRAW_FLAG_TRANS_PATTERN		0x00000040
#define	DISP_DRAW_FLAG_SUBPIXEL			0x00000100
#define	DISP_DRAW_FLAG_COMPLEX_ROP		0x00000200
#define	DISP_DRAW_FLAG_SIMPLE_ROP		0x00000400
#define	DISP_DRAW_FLAG_FILTERED_SCALING		0x00000800
#define	DISP_DRAW_FLAG_SOFTWARE_SCALING		0x00001000
#define	DISP_DRAW_FLAG_USE_PLANE_MASK		0x00002000
#define	DISP_DRAW_FLAG_ANTIALIAS		0x00004000
#define	DISP_DRAW_FLAG_TRANSFORM		0x00008000
#define DISP_DRAW_FLAG_USE_SOFT_AA		0x00010000
#define DISP_DRAW_FLAGS_UNDEFINED \
	~(DISP_DRAW_FLAG_USE_DST_ALPHA | \
	DISP_DRAW_FLAG_USE_ALPHA | DISP_DRAW_FLAG_USE_CHROMA | \
	DISP_DRAW_FLAG_MONO_PATTERN | DISP_DRAW_FLAG_TRANS_PATTERN | \
	DISP_DRAW_FLAG_SUBPIXEL | DISP_DRAW_FLAG_COMPLEX_ROP | \
	DISP_DRAW_FLAG_SIMPLE_ROP | DISP_DRAW_FLAG_FILTERED_SCALING | \
	DISP_DRAW_FLAG_SOFTWARE_SCALING | DISP_DRAW_FLAG_USE_PLANE_MASK | \
	DISP_DRAW_FLAG_ANTIALIAS | DISP_DRAW_FLAG_TRANSFORM | DISP_DRAW_FLAG_USE_SOFT_AA)

#define	DISP_DRAW_FLAG_ROP_NON_OPAQUE		 (DISP_DRAW_FLAG_COMPLEX_ROP|DISP_DRAW_FLAG_SIMPLE_ROP)

/* disp_draw_context_t.line_flags */
#define	DISP_LINE_FLAG_DASH_MONO		0x00000001
#define	DISP_LINE_FLAG_DASH_TRANS		0x00000002

/* disp_draw_context_t.pattern_format */
#define DISP_PATTERN_FORMAT_MONO_8x1		0x00000001
#define DISP_PATTERN_FORMAT_MONO_8x8		0x00000002

#define DISP_DRAW_SIMPLE(flags)	(!((flags) & (DISP_DRAW_FLAG_SUBPIXEL | \
						DISP_DRAW_FLAG_ROP_NON_OPAQUE | \
						DISP_DRAW_FLAG_USE_DST_ALPHA | \
						DISP_DRAW_FLAG_USE_ALPHA | \
						DISP_DRAW_FLAG_USE_CHROMA | \
						DISP_DRAW_FLAG_USE_PLANE_MASK | \
						DISP_DRAW_FLAG_MONO_PATTERN | \
						DISP_DRAW_FLAG_TRANS_PATTERN | \
						DISP_DRAW_FLAG_ANTIALIAS | \
						DISP_DRAW_FLAG_TRANSFORM | \
						DISP_DRAW_FLAGS_UNDEFINED)))

#define DISP_DRAW_SIMPLE_PATTERN(flags)	(!((flags) & (DISP_DRAW_FLAG_SUBPIXEL | \
						DISP_DRAW_FLAG_ROP_NON_OPAQUE | \
						DISP_DRAW_FLAG_USE_DST_ALPHA | \
						DISP_DRAW_FLAG_USE_ALPHA | \
						DISP_DRAW_FLAG_USE_CHROMA | \
						DISP_DRAW_FLAG_USE_PLANE_MASK | \
						DISP_DRAW_FLAG_ANTIALIAS | \
						DISP_DRAW_FLAG_TRANSFORM | \
						DISP_DRAW_FLAGS_UNDEFINED)))

#define DISP_DRAW_ROP_ONLY(flags)		(!((flags) & (DISP_DRAW_FLAG_SUBPIXEL | \
						DISP_DRAW_FLAG_USE_DST_ALPHA | \
						DISP_DRAW_FLAG_USE_ALPHA | \
						DISP_DRAW_FLAG_USE_CHROMA | \
						DISP_DRAW_FLAG_USE_PLANE_MASK | \
						DISP_DRAW_FLAG_MONO_PATTERN | \
						DISP_DRAW_FLAG_TRANS_PATTERN | \
						DISP_DRAW_FLAG_ANTIALIAS | \
						DISP_DRAW_FLAG_TRANSFORM | \
						DISP_DRAW_FLAGS_UNDEFINED)))

#define DISP_DRAW_ROP_PATTERN_ONLY(flags)	(!((flags) & (DISP_DRAW_FLAG_SUBPIXEL | \
						DISP_DRAW_FLAG_USE_DST_ALPHA | \
						DISP_DRAW_FLAG_USE_ALPHA | \
						DISP_DRAW_FLAG_USE_CHROMA | \
						DISP_DRAW_FLAG_USE_PLANE_MASK | \
						DISP_DRAW_FLAG_TRANS_PATTERN | \
						DISP_DRAW_FLAG_ANTIALIAS | \
						DISP_DRAW_FLAG_TRANSFORM | \
						DISP_DRAW_FLAGS_UNDEFINED)))

#define DISP_DRAW_SIMPLE_ROP_ONLY(flags)	(!((flags) & (DISP_DRAW_FLAG_SUBPIXEL | \
						DISP_DRAW_FLAG_COMPLEX_ROP | \
						DISP_DRAW_FLAG_USE_DST_ALPHA | \
						DISP_DRAW_FLAG_USE_ALPHA | \
						DISP_DRAW_FLAG_USE_CHROMA | \
						DISP_DRAW_FLAG_USE_PLANE_MASK | \
						DISP_DRAW_FLAG_MONO_PATTERN | \
						DISP_DRAW_FLAG_TRANS_PATTERN | \
						DISP_DRAW_FLAG_ANTIALIAS | \
						DISP_DRAW_FLAG_TRANSFORM | \
						DISP_DRAW_FLAGS_UNDEFINED)))

#define DISP_DRAW_SIMPLE_ROP_PATTERN_ONLY(flags)	(!((flags) & (DISP_DRAW_FLAG_SUBPIXEL | \
						DISP_DRAW_FLAG_COMPLEX_ROP | \
						DISP_DRAW_FLAG_USE_DST_ALPHA | \
						DISP_DRAW_FLAG_USE_ALPHA | \
						DISP_DRAW_FLAG_USE_CHROMA | \
						DISP_DRAW_FLAG_USE_PLANE_MASK | \
						DISP_DRAW_FLAG_ANTIALIAS | \
						DISP_DRAW_FLAG_TRANSFORM | \
						DISP_DRAW_FLAGS_UNDEFINED)))

/* "flags" parameter for context "draw_poly_line" entrypoint */
#define DISP_DRAW_POLYLINE_CLOSE		0x00000001	/* Connect the first and last point */

/* "flags" parameter for core/context "fill_poly" entrypoint */
#define DISP_DRAW_FILLPOLY_ANTIALIAS		0x00000001	/* Antialiased the filled polygon edges */

/* disp_draw_context_t.line_join */
#define DISP_DRAW_LINEJOIN_BUTT				0x00000000
#define DISP_DRAW_LINEJOIN_BEVEL			0x00000001
#define DISP_DRAW_LINEJOIN_ROUND			0x00000002
#define DISP_DRAW_LINEJOIN_MITER			0x00000003

/* disp_draw_context_t.cap_style */
#define DISP_DRAW_CAPSTYLE_BUTT				0x00000000
#define DISP_DRAW_CAPSTYLE_ROUND			0x00000001
#define DISP_DRAW_CAPSTYLE_SQUARE			0x00000002

/* disp_draw_context_t.poly_fill */
#define DISP_DRAW_POLYFILL_EVENODD			0x00000000
#define DISP_DRAW_POLYFILL_NON_ZERO			0x00000001
#define DISP_DRAW_POLYFILL_CONVEX			0x00000002
#define DISP_DRAW_CAPSTYLE_MONOTONE			0x00000003

typedef enum {
	DISP_DRAW_DEVCTL_STATS = 1,		/* for used with devg-stats module */
} disp_draw_devctl_t;

/* Contains everything the flat frame buffer lib needs in order to draw */
typedef struct disp_draw_context {
	int			size;
	disp_adapter_t		*adapter;
	void			*gd_ctx;	/* Graphics drivers private context */
	struct disp_draw_corefuncs	*cfuncs;	/* Allow software routines to call into hw routines */
	unsigned		flags;		/* Draw flags */
	disp_color_t		fgcolor;
	disp_color_t		bgcolor;
	struct disp_draw_corefuncs	*corefuncs_sw;	/* S/W only core functions for currently targetted surface */
	struct disp_draw_contextfuncs	*ctxfuncs_sw;	/* S/W only context functions */
	uint8_t			*pat;
	unsigned short		pat_xoff;
	unsigned short		pat_yoff;
	unsigned short		pattern_format;
	unsigned short		rop3;
	unsigned short		chroma_mode;
	disp_color_t		chroma_color0;
	disp_color_t		chroma_color1;
	disp_color_t		chroma_mask;
	unsigned		alpha_mode;
	unsigned 		s_alpha;		/* Global multiplier 1 */
	unsigned		d_alpha;		/* Global multiplier 2 */
	unsigned		alpha_map_xoff, alpha_map_yoff;
	disp_surface_t		*alpha_map;
	disp_surface_t		*dsurf;			/* Default destination surface */
	unsigned char		*sysram_workspace;	/* Scratch workspace in system RAM */
	int			sysram_workspace_size;	/* Size of scratch workspace */
	unsigned		plane_mask;
	int			clip_left;
	int			clip_top;
	int			clip_right;
	int			clip_bottom;
	int			reserved0;
	int			reserved1;
	int			reserved2;
	int			reserved3;

	unsigned char		*palette_lut;		/* 4k palette look up table (444 true color to palette index conversions) */

	disp_color_t		*dest_palette;		/* Palette to be used for 8-bit alpha blending */
	int			dest_palette_size;

	disp_fx_t		xform_matrix[4];	/* 2x2 matrix */
	int			xlate_x;
	int			xlate_y;
	unsigned		line_flags;
	int			line_join;
	uint32_t		line_pat;
	int			line_repeat;
	int			line_initial_offset;
	int			cap_style;
	int			poly_fill;
	disp_surface_t		*dst_alpha_map;
	unsigned		dst_alpha_map_xoff, dst_alpha_map_yoff;
	uintptr_t		reserved[5];
} disp_draw_context_t;

__BEGIN_DECLS

typedef struct disp_nl_accessfuncs {
	void (*writescan)(disp_draw_context_t *ctx, disp_surface_t *dst,
	    int x, int y, int npixels, void *data);
	void (*readscan)(disp_draw_context_t *ctx, disp_surface_t *src,
	    int x, int y, int npixels, void *data);
	void (*setscan)(disp_draw_context_t *ctx, disp_surface_t *dst,
	    int x, int y, int npixels, disp_color_t color);
	void (*setscan_mono)(disp_draw_context_t *ctx, disp_surface_t *dst,
	    int x, int y, int npixels,
	    disp_color_t fgcolor, disp_color_t bgcolor, uint8_t pat, int patidx);
	void (*setscan_trans)(disp_draw_context_t *ctx, disp_surface_t *dst,
	    int x, int y, int npixels, disp_color_t color, uint8_t pat, int patidx);
	void (*setscan_bitmap)(disp_draw_context_t *ctx, disp_surface_t *dst,
	    int x, int y, int width,
	    disp_color_t fgcolor, disp_color_t bgcolor, int bit0_offset,
	    int transparent, uint8_t *sptr);
} disp_nl_accessfuncs_t;

typedef struct disp_draw_corefuncs {
	void (*wait_idle)(disp_draw_context_t *);
	int (*hw_idle)(disp_adapter_t *adapter, void *ignored);

	/* Simple drawing functions */
	void (*draw_span)(disp_draw_context_t *context,
	    disp_color_t color, int x1, int x2, int y);
	void (*draw_span_list)(disp_draw_context_t *context, int count,
	    disp_color_t color, int x1[], int x2[], int y[]);
	void (*draw_solid_rect)(disp_draw_context_t *context,
	    disp_color_t color, int x1, int y1, int x2, int y2);
	void (*draw_line_pat8x1)(disp_draw_context_t *context,
	    disp_color_t fgcolor, disp_color_t bgcolor,
	    int x1, int x2, int y, uint8_t pattern);
	void (*draw_line_trans8x1)(disp_draw_context_t *context,
	    disp_color_t color, int x1, int x2, int y, uint8_t pattern);

	void (*draw_rect_pat8x8)(disp_draw_context_t *context,
	    disp_color_t fgcolor, disp_color_t bgcolor, int x1, int y1,
	    int x2, int y2);
	void (*draw_rect_trans8x8)(disp_draw_context_t *context,
	    disp_color_t color, int x1, int y1, int x2, int y2);

	void (*blit1)(disp_draw_context_t *context, 
	    int sx, int sy, int dx, int dy, int width, int height);
	void (*blit2)(disp_draw_context_t *context, 
	    disp_surface_t *src, disp_surface_t *dst,
	    int sx, int sy, int dx, int dy, int width, int height);

	void (*draw_bitmap)(disp_draw_context_t *context,
	    uint8_t *image, int sstride, int bit0_offset,
	    disp_color_t fgcolor, disp_color_t bgcolor,
	    int transparent, int dx, int dy, int width, int height);

	void (*update_draw_surface)(disp_draw_context_t *context);
	void (*update_pattern)(disp_draw_context_t *context);

	void (*scaled_blit)(disp_draw_context_t *context, 
	    disp_surface_t *src, disp_surface_t *dst,
	    int sx1, int sy1, int sx2, int sy2,
	    int dx1, int dy1, int dx2, int dy2);

	void (*draw_line)(disp_draw_context_t *context, disp_color_t color,
	    int x1, int y1, int x2, int y2, unsigned flags);
	int (*fill_poly)(disp_draw_context_t *ctx, disp_color_t color,
	    disp_point_t *pointlist, int npoints, unsigned flags);

	int (*get_bresenham_params)(disp_draw_context_t *ctx,
	    int x1, int y1, int x2, int y2,
	    int *initial_error, int *major_inc, int *minor_inc);

	void (*update_transform)(disp_draw_context_t *context);
	void (*update_clipping)(disp_draw_context_t *context);

	void (*blend_pixels)(disp_draw_context_t *ctx, disp_color_t color,
	    int x[], int y[], uint8_t alpha[], int npixels);

	disp_nl_accessfuncs_t * (*get_nlfuncs)(disp_draw_context_t *ctx,
	    disp_surface_t *surf);

	void	(*reserved[8])(void);
} disp_draw_corefuncs_t; 

typedef struct disp_draw_contextfuncs {
	void (*draw_span)(disp_draw_context_t *context,
	    unsigned x1, unsigned x2, unsigned y);
	void (*draw_span_list)(disp_draw_context_t *context, int count,
	    int x1[], int x2[], int y[]);
	void (*draw_rect)(disp_draw_context_t *context,
	    unsigned x1, unsigned y1, unsigned x2, unsigned y2);

	void (*blit)(disp_draw_context_t *context, disp_surface_t *src,
	    disp_surface_t *dst, unsigned sx, unsigned sy,
	    unsigned dx, unsigned dy, unsigned width, unsigned height);

	/* Notify of changes in draw context */
	void (*update_general)(disp_draw_context_t *context);
	void (*update_color)(disp_draw_context_t *context);
	void (*update_rop3)(disp_draw_context_t *context);
	void (*update_chroma)(disp_draw_context_t *context);
	void (*update_alpha)(disp_draw_context_t *context);

	void (*scaled_blit)(disp_draw_context_t *context, 
	    disp_surface_t *src, disp_surface_t *dst,
	    int sx1, int sy1, int sx2, int sy2,
	    int dx1, int dy1, int dx2, int dy2);

	void (*update_planemask)(disp_draw_context_t *context);

	void (*draw_line)(disp_draw_context_t *context,
	    int x1, int y1, int x2, int y2, unsigned flags);
	int (*fill_poly)(disp_draw_context_t *ctx,
	    disp_point_t *pointlist, int npoints, unsigned flags);
	int (*draw_polyline)(disp_draw_context_t *ctx,
	    disp_point_t *pointlist, int npoints, int width, unsigned flags);
	void (*update_line)(disp_draw_context_t *context);
	void (*blend_pixels)(disp_draw_context_t *ctx,
	    int x[], int y[], uint8_t alpha[], int npixels);

	void	(*reserved[8])(void);
} disp_draw_contextfuncs_t; 

typedef struct disp_draw_miscfuncs {
	int (*init)(disp_adapter_t *adapter, char *optstring);
	void (*fini)(disp_adapter_t *adapter);
	void (*module_info)(disp_adapter_t *adapter, disp_module_info_t *info);
	int (*set_palette)(disp_adapter_t *ctx,
	    int index, int count, disp_color_t *pal);
	int (*set_hw_cursor)(disp_adapter_t *ctx,
	    uint8_t *fg_bmp, uint8_t *bg_bmp,
	    unsigned fg_color, unsigned bg_color,
	    int hotspot_x, int hotspot_y,
	    int size_x, int size_y, int bmp_stride);
	void (*enable_hw_cursor)(disp_adapter_t *ctx);
	void (*disable_hw_cursor)(disp_adapter_t *ctx);
	void (*set_hw_cursor_pos)(disp_adapter_t *ctx, int x, int y);
	void (*flushrect)(disp_draw_context_t *ctx,
	    int x1, int y1, int x2, int y2);
	int (*service_msg)(disp_draw_context_t *ctx,
	    void *data_in, int nbytes, void *data_out, int out_buffer_size);
	int (*get_2d_caps)(disp_adapter_t *adapter,
	    disp_surface_t *surf, disp_2d_caps_t *caps);

	int (*get_corefuncs_sw)(disp_adapter_t *adapter, unsigned pixel_format,
	    disp_draw_corefuncs_t *fns, int tabsize);
	int (*get_contextfuncs_sw)(disp_adapter_t *adapter,
	    disp_draw_contextfuncs_t *fns, int tabsize);

	int (*set_power_mode)(disp_adapter_t *adapter, pm_power_mode_t mode);

	int (*end_of_draw)(disp_adapter_t *adapter);
	
	/* Support for rendering from another process */
	int (*attach_external)(disp_adapter_t *adapter, disp_aperture_t aper[]);
	int (*detach_external)(disp_adapter_t *adapter);
	int (*recover)(disp_adapter_t *adapter);

	int (*wait_idle)(disp_adapter_t *adapter);
	int (*devctl)(disp_adapter_t *adapter, disp_draw_devctl_t cmd,
	    void *data_in, int nbytes, void *data_out, int *out_buffer_size);

	int (*terminate_notify)(disp_adapter_t *adapter, pid_t pid);

	void (*set_memory_zone)(disp_adapter_t *adapter, int zone);
	
	void	(*reserved[6])(void);
} disp_draw_miscfuncs_t;

typedef int (*get_miscfuncs_t)(disp_adapter_t *adapter,
    disp_draw_miscfuncs_t *fns, int tabsize);
typedef int (*get_corefuncs_t)(disp_adapter_t *adapter, unsigned pixel_format,
    disp_draw_corefuncs_t *fns, int tabsize);
typedef int (*get_contextfuncs_t)(disp_adapter_t *adapter,
    disp_draw_contextfuncs_t *fns, int tabsize);

/* Main draw driver entry points */
extern WINSIM_API int devg_get_miscfuncs(disp_adapter_t *adapter,
    disp_draw_miscfuncs_t *fns, int tabsize);
extern WINSIM_API int devg_get_corefuncs(disp_adapter_t *adapter, unsigned pixel_format,
    disp_draw_corefuncs_t *fns, int tabsize);
extern WINSIM_API int devg_get_contextfuncs(disp_adapter_t *adapter,
    disp_draw_contextfuncs_t *fns, int tabsize);

__END_DECLS

#endif /* _GRAPHICS_DRAW_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/draw.h $ $Rev: 657836 $" )
