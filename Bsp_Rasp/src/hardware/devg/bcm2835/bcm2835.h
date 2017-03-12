/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems.
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


#ifndef __at91sam9xx_H_INCLUDED
#define __at91sam9xx_H_INCLUDED

#include <graphics/display.h>
#include <graphics/mode.h>
#include <graphics/disputil.h>
#include <graphics/ffb.h>
#include <graphics/vbios.h>
#include <graphics/vesa.h>
#include <string.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>


#define _SLOGC_GRAPHICS      8

#define dc_rptr(x) (uint32_t volatile *)(((unsigned char volatile *)at91sam9xx->dc_regptr)+x)
#define mailbox_rptr(x) (uint32_t volatile *)(((unsigned char volatile *)at91sam9xx->mailboxptr)+x)
#define Tstscr_rptr(x) (uint32_t volatile *)(((unsigned char volatile *)at91sam9xx->testscr)+x)


/* Display width in pixels */
#define LCD_DFT_WIDTH             800
/* Display height in pixels */
#define LCD_DFT_HEIGHT            480
#define LCD_DFT_FRAMERATE         60

#define DC_REGBASE	0x40040000
#define DC_REGSIZE	0x30

typedef struct at91sam9xx_context {
	disp_adapter_t		*adapter;
	unsigned char		*vidptr;
	unsigned 		vidbase;
	unsigned		dcbase;
	
	uint32_t        *mailboxptr;
	uint32_t		*dc_regptr;
	uint32_t		*testscr;
	unsigned 		vidsize;
	unsigned         vsync_counter;

	int			free_context;
	
	disp_color_t		*palette;

        short           	width;
        short           	height;	
        unsigned                board_mck;
        unsigned                dbl_pixelclock;
} at91sam9xx_context_t;


/*
 * Mode related entry points
 */
int bcm2835_init(disp_adapter_t *adapter, char *opt);
void bcm2835_fini(disp_adapter_t *adapter);
void bcm2835_module_info(disp_adapter_t *adapter, disp_module_info_t *info);
int devg_get_modefuncs(disp_adapter_t *adp,disp_modefuncs_t *funcs, int tabsize);
int bcm2835_get_modelist(disp_adapter_t *adapter, int dispno,
    disp_mode_t *list, int index, int size);
int bcm2835_get_modeinfo(disp_adapter_t *adapter, int dispno,
    disp_mode_t mode, disp_mode_info_t *info);
int bcm2835_set_mode(disp_adapter_t *adapter, int dispno, disp_mode_t mode,
    disp_crtc_settings_t *settings, disp_surface_t *surf, unsigned flags);
int bcm2835_set_display_offset(disp_adapter_t *ctx, int dispno,
    unsigned offset, int wait_vsync);
int bcm2835_wait_vsync(disp_adapter_t *adapter, int dispno);
void bcm2835_lcd_init(at91sam9xx_context_t *at91sam9xx, disp_mode_t mode);
void bcm2835_lcd_enable(at91sam9xx_context_t *at91sam9xx);
int get_config_data(at91sam9xx_context_t *at91sam9xx, const char *filename);

/*
 * Core function entry points
 */

/* Miscellaneous functions */
void bcm2835_wait_idle(disp_draw_context_t *ctx);
int bcm2835_get_2d_caps(disp_adapter_t *adapter, disp_surface_t *surf,
    disp_2d_caps_t *caps);

/* Miscellaneous draw related functions */
int bcm2835_draw_init(disp_adapter_t *adapter, char *opt);
void bcm2835_draw_fini(disp_adapter_t *adapter);
int bcm2835_set_hw_cursor(disp_adapter_t *ctx,int dispno, uint8_t *fg_bmp,
    uint8_t *bg_bmp, disp_color_t color0, disp_color_t color1,
    int hotspot_x, int hotspot_y, int size_x, int size_y, int bmp_stride);
void bcm2835_enable_hw_cursor(disp_adapter_t *ctx,int dispno);
void bcm2835_disable_hw_cursor(disp_adapter_t *ctx,int dispno);
void bcm2835_set_hw_cursor_pos(disp_adapter_t *ctx,int dispno, int x, int y);
int bcm2835_attach_external(disp_adapter_t *adapter, disp_aperture_t aper[]);
int bcm2835_detach_external(disp_adapter_t *adapter);
int bcm2835_recover(disp_adapter_t *adapter);
int bcm2835_misc_wait_idle(disp_adapter_t *adapter);


/*
 * Memory manager entry points
 */
int bcm2835_mem_init(disp_adapter_t *adapter, char *optstring);
void bcm2835_mem_fini(disp_adapter_t *adapter);
int bcm2835_mem_reset(disp_adapter_t *adapter, disp_surface_t *surf);
disp_surface_t *bcm2835_alloc_surface(disp_adapter_t *adapter,
    int width, int height, unsigned format, unsigned flags, unsigned user_flags);
int bcm2835_free_surface(disp_adapter_t *adapter, disp_surface_t *surf);
unsigned long bcm2835_mem_avail(disp_adapter_t *adapter, unsigned sflags);

int bcm2835_query_apertures(disp_adapter_t *adp, disp_aperture_t *aptertures);
int bcm2835_query_surface(disp_adapter_t *adapter,
    disp_surface_t *surf, disp_surface_info_t *info);
int bcm2835_get_alloc_info(disp_adapter_t *adp,
    int width, int height, unsigned format,
    unsigned flags, unsigned user_flags, disp_alloc_info_t *info);
int bcm2835_get_alloc_layer_info(disp_adapter_t *adp, int dispno[], int layer_idx[],
    int nlayers, unsigned format, int surface_index, int width, int height,
    unsigned sflags, unsigned hint_flags, disp_alloc_info_t *info);


#endif /* __at91sam9xx_H_INCLUDED */
