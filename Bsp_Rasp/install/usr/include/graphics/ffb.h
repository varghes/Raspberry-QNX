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





#ifndef _GRAPHICS_FFB_H_INCLUDED
#define _GRAPHICS_FFB_H_INCLUDED

#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#include <graphics/display.h>
#endif

typedef struct {
	int		rbank_size;	/* Size of read aperture */
	int		wbank_size;	/* Size of write aperture */
	int		rbank_current;
	int		wbank_current;
	void		(*set_rbank)(void *arg, int bankno);
	void		(*set_wbank)(void *arg, int bankno);
	uint8_t		*rptr;		/* Virtual address of read aperture */
	uint8_t		*wptr;		/* Virtual address of write aperture */
	void		*arg;		/* Argument to pass to bank-switch routines */
	uint32_t	reserved[4];
} ffb_bankinfo_t;

__BEGIN_DECLS

/* Core functions */
void ffb_core_blit1(disp_draw_context_t *ctx,
    int sx, int sy, int dx, int dy, int width, int height);
void ffb_core_blit2(disp_draw_context_t *ctx,
    disp_surface_t *src, disp_surface_t *dst,
    int sx, int sy, int dx, int dy, int width, int height);
void ffb_core_scaled_blit(disp_draw_context_t *ctx,
    disp_surface_t *src, disp_surface_t *dst,
    int sx1, int sy1, int sx2, int sy2,
    int dx1, int dy1, int dx2, int dy2);
void ffb_core_draw_line(disp_draw_context_t *ctx, disp_color_t color,
    int x1, int y1, int x2, int y2, unsigned flags);
void ffb_core_blend_pixels(disp_draw_context_t *ctx,
    disp_color_t color, int x[], int y[], uint8_t alpha[], int npixels);
int ffb_core_fill_poly(disp_draw_context_t *ctx, disp_color_t color,
    disp_point_t *pointlist, int npoints, unsigned flags);
int ffb_get_bresenham_params(disp_draw_context_t *ctx,
    int x1, int y1, int x2, int y2,
    int *initial_error, int *major_inc, int *minor_inc);

void ffb_draw_span_8(disp_draw_context_t *context,
    disp_color_t color, int x1, int x2, int y);
void ffb_draw_span_list_8(disp_draw_context_t *context, int count,
    disp_color_t color, int x1[], int x2[], int y[]);
void ffb_draw_solid_rect_8(disp_draw_context_t *context,
    disp_color_t color, int x1, int y1, int x2, int y2);
void ffb_draw_line_pat8x1_8(disp_draw_context_t *context, disp_color_t fgcolor,
    disp_color_t bgcolor, int x1, int x2, int y, uint8_t pattern);
void ffb_draw_line_trans8x1_8(disp_draw_context_t *context, disp_color_t color,
    int x1, int x2, int y, uint8_t pattern);
void ffb_draw_rect_pat8x8_8(disp_draw_context_t *context,
    disp_color_t fgcolor, disp_color_t bgcolor, int x1, int y1,
    int x2, int y2);
void ffb_draw_rect_trans8x8_8(disp_draw_context_t *context,
    disp_color_t color, int x1, int y1,
    int x2, int y2);
void ffb_draw_bitmap_8(disp_draw_context_t *context,
    uint8_t *image, int sstride, int bit0_offset,
    disp_color_t fgcolor, disp_color_t bgcolor,
    int transparent, int dx, int dy, int width, int height);

void ffb_draw_span_16(disp_draw_context_t *context,
    disp_color_t color, int x1, int x2, int y);
void ffb_draw_span_list_16(disp_draw_context_t *context, int count,
    disp_color_t color, int x1[], int x2[], int y[]);
void ffb_draw_solid_rect_16(disp_draw_context_t *context,
    disp_color_t color, int x1, int y1, int x2, int y2);
void ffb_draw_line_pat8x1_16(disp_draw_context_t *context, disp_color_t fgcolor,
    disp_color_t bgcolor, int x1, int x2, int y, uint8_t pattern);
void ffb_draw_line_trans8x1_16(disp_draw_context_t *context, disp_color_t color,
    int x1, int x2, int y, uint8_t pattern);
void ffb_draw_rect_pat8x8_16(disp_draw_context_t *context,
    disp_color_t fgcolor, disp_color_t bgcolor, int x1, int y1,
    int x2, int y2);
void ffb_draw_rect_trans8x8_16(disp_draw_context_t *context,
    disp_color_t color, int x1, int y1,
    int x2, int y2);
void ffb_draw_bitmap_16(disp_draw_context_t *context,
    uint8_t *image, int sstride, int bit0_offset,
    disp_color_t fgcolor, disp_color_t bgcolor,
    int transparent, int dx, int dy, int width, int height);

void ffb_draw_span_24(disp_draw_context_t *context,
    disp_color_t color, int x1, int x2, int y);
void ffb_draw_span_list_24(disp_draw_context_t *context, int count,
    disp_color_t color, int x1[], int x2[], int y[]);
void ffb_draw_solid_rect_24(disp_draw_context_t *context,
    disp_color_t color, int x1, int y1, int x2, int y2);
void ffb_draw_line_pat8x1_24(disp_draw_context_t *context, disp_color_t fgcolor,
    disp_color_t bgcolor, int x1, int x2, int y, uint8_t pattern);
void ffb_draw_line_trans8x1_24(disp_draw_context_t *context, disp_color_t color,
    int x1, int x2, int y, uint8_t pattern);
void ffb_draw_rect_pat8x8_24(disp_draw_context_t *context,
    disp_color_t fgcolor, disp_color_t bgcolor, int x1, int y1,
    int x2, int y2);
void ffb_draw_rect_trans8x8_24(disp_draw_context_t *context,
    disp_color_t color, int x1, int y1,
    int x2, int y2);
void ffb_draw_bitmap_24(disp_draw_context_t *context,
    uint8_t *image, int sstride, int bit0_offset,
    disp_color_t fgcolor, disp_color_t bgcolor,
    int transparent, int dx, int dy, int width, int height);

void ffb_draw_span_32(disp_draw_context_t *context,
    disp_color_t color, int x1, int x2, int y);
void ffb_draw_span_list_32(disp_draw_context_t *context, int count,
    disp_color_t color, int x1[], int x2[], int y[]);
void ffb_draw_solid_rect_32(disp_draw_context_t *context,
    disp_color_t color, int x1, int y1, int x2, int y2);
void ffb_draw_line_pat8x1_32(disp_draw_context_t *context, disp_color_t fgcolor,
    disp_color_t bgcolor, int x1, int x2, int y, uint8_t pattern);
void ffb_draw_line_trans8x1_32(disp_draw_context_t *context, disp_color_t color,
    int x1, int x2, int y, uint8_t pattern);
void ffb_draw_rect_pat8x8_32(disp_draw_context_t *context,
    disp_color_t fgcolor, disp_color_t bgcolor, int x1, int y1,
    int x2, int y2);
void ffb_draw_rect_trans8x8_32(disp_draw_context_t *context,
    disp_color_t color, int x1, int y1,
    int x2, int y2);
void ffb_draw_bitmap_32(disp_draw_context_t *context,
    uint8_t *image, int sstride, int bit0_offset,
    disp_color_t fgcolor, disp_color_t bgcolor,
    int transparent, int dx, int dy, int width, int height);

/* Draw state update notify functions */
void ffb_update_draw_surface(disp_draw_context_t *context);
void ffb_update_pattern(disp_draw_context_t *context);
void ffb_update_transform(disp_draw_context_t *context);
void ffb_update_clipping(disp_draw_context_t *context);

/* Context functions */
void ffb_ctx_draw_span(disp_draw_context_t *context,
    unsigned x1, unsigned x2, unsigned y);
void ffb_ctx_draw_span_list(disp_draw_context_t *context,
    int count, int x1[], int x2[], int y[]);
void ffb_ctx_draw_rect(disp_draw_context_t *context,
    unsigned x1, unsigned y1, unsigned x2, unsigned y2);

void ffb_ctx_blit(disp_draw_context_t *ctx,
    disp_surface_t *src, disp_surface_t *dst,
    unsigned sx, unsigned sy, unsigned dx, unsigned dy,
    unsigned width, unsigned height);
void ffb_ctx_scaled_blit(disp_draw_context_t *ctx,
    disp_surface_t *src, disp_surface_t *dst,
    int sx1, int sy1, int sx2, int sy2,
    int dx1, int dy1, int dx2, int dy2);
void ffb_ctx_draw_line(disp_draw_context_t *ctx,
    int x1, int y1, int x2, int y2, unsigned flags);
int ffb_ctx_fill_poly(disp_draw_context_t *ctx,
    disp_point_t *point_list, int npoints, unsigned flags);
int ffb_ctx_draw_polyline(disp_draw_context_t *ctx,
    disp_point_t *pointlist, int npoints, int width, unsigned flags);
void ffb_ctx_blend_pixels(disp_draw_context_t *ctx,
    int x[], int y[], uint8_t alpha[], int npixels);

/* Miscellaneous */
void ffb_wait_idle(disp_draw_context_t *context);
int ffb_hw_idle(disp_adapter_t *context, void *ignored);
void ffb_set_draw_surface(disp_draw_context_t *context, disp_surface_t *surf);
int ffb_get_2d_caps(disp_adapter_t *adapter, disp_surface_t *surf,
    disp_2d_caps_t *caps);

/* Draw state update notify functions */
void ffb_ctx_update_general(disp_draw_context_t *context);
void ffb_ctx_update_color(disp_draw_context_t *context);
void ffb_ctx_update_rop3(disp_draw_context_t *context);
void ffb_ctx_update_chroma(disp_draw_context_t *context);
void ffb_ctx_update_alpha(disp_draw_context_t *context);
void ffb_ctx_update_planemask(disp_draw_context_t *context);
void ffb_ctx_update_line(disp_draw_context_t *context);

/* Draw function retrieval routines */
int ffb_get_corefuncs(disp_adapter_t *context,
    unsigned pixel_format, disp_draw_corefuncs_t *funcs, int tabsize);
int ffb_get_contextfuncs(disp_adapter_t *context, disp_draw_contextfuncs_t *funcs, int tabsize);

typedef void (*ffb_cvtfunc_t)(void *, void *, int, void *);
ffb_cvtfunc_t ffb_cvtfunc(disp_surface_t *src,
    disp_surface_t *dst, void **extra);
ffb_cvtfunc_t ffb_cvtfunc_swap(disp_surface_t *src,
    disp_surface_t *dst, void **extra);

/* Scanline scaling-in-place routine */
void ffb_scale_scanline(unsigned pixel_format,
    uint8_t *data, int swidth, int dwidth, int startpixel, int npixels);

__END_DECLS

#endif /* _GRAPHICS_FFB_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/ffb.h $ $Rev: 657836 $" )
