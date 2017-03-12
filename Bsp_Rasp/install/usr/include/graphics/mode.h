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





#ifndef _GRAPHICS_MODE_H_INCLUDED
#define _GRAPHICS_MODE_H_INCLUDED

#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#include <graphics/display.h>
#endif

typedef unsigned short	disp_mode_t;

#define DISP_MAX_MODES			72
#define DISP_MODE_LISTEND		(disp_mode_t)~0

/* disp_mode_info.flags */
#define DISP_MODE_PANEL_DETECTED	0x00000004	/* There is a flat panel currently attached */
#define DISP_MODE_FLAG_TVOUT_OVERSCAN	0x00000008	/* TV driven in "overscan" mode (else underscan) */
#define DISP_MODE_GENERIC		0x00020000	/* xres, yres, and refresh rate of this mode are not fixed */
#define DISP_MODE_XY_SWAPPED		0x00100000	/* For handling display timings on rotated displays */

/* "flags" argument to "set_mode()" */
#define DISP_MODE_FLAG_TV_STANDARD(__flags)	((__flags) & 0xff)
#define DISP_MODE_FLAG_DONT_SWITCH		0x00000100	/* Adapter is already in graphics mode; don't adjust video mode (e.g. if there's a splash screen being displayed) */

#define DISP_MODE_FLAG_TVOUT_NTSC		0x00000000
#define DISP_MODE_FLAG_TVOUT_PAL		0x00000001
#define DISP_MODE_FLAG_TVOUT_SECAM		0x00000002
#define DISP_MODE_FLAG_TVOUT_NTSC_JAPAN		0x00000003
#define DISP_MODE_FLAG_TVOUT_PAL_M		0x00000004
#define DISP_MODE_FLAG_TVOUT_PAL_N		0x00000005
#define DISP_MODE_FLAG_TVOUT_PAL_N_COMBO	0x00000006
#define DISP_MODE_FLAG_DISABLE_MONITOR		0x00000100	/* Turn off the monitor output */
#define DISP_MODE_FLAG_DISABLE_TVOUT		0x00000200	/* Turn off the TV output */
#define DISP_MODE_FLAG_DISABLE_PANEL		0x00000400	/* Turn off the panel output */
#define DISP_MODE_FLAG_DYNAMIC_SWITCH		0x00001000	/* Change display timings, but otherwise do not disturb display controller state */

/* disp_mode_info.caps */
#define DISP_MCAP_SET_DISPLAY_OFFSET	0x00000001
#define DISP_MCAP_VIRTUAL_PANNING	0x00000002
#define DISP_MCAP_DPMS_SUPPORTED	0x00000004
#define DISP_MCAP_TVOUT_ONLY		0x00000010	/* Can drive a TV with monitor switched off */
#define DISP_MCAP_TVOUT_WITH_MONITOR	0x00000020	/* Can drive a TV and monitor simultaneously */
#define DISP_MCAP_PANEL_ONLY		0x00000040	/* Can drive a panel with monitor switched off */
#define DISP_MCAP_PANEL_WITH_MONITOR	0x00000080	/* Can drive a panel and monitor simultaneously */
#define DISP_MCAP_TVOUT_NTSC		0x00010000
#define DISP_MCAP_TVOUT_PAL		0x00020000
#define DISP_MCAP_TVOUT_SECAM		0x00040000
#define DISP_MCAP_TVOUT_NTSC_JAPAN	0x00080000
#define DISP_MCAP_TVOUT_PAL_M		0x00100000
#define DISP_MCAP_TVOUT_PAL_N		0x00200000
#define DISP_MCAP_TVOUT_PAL_N_COMBO	0x00400000
#define DISP_MCAP_BACKLIGHT_CONTROL	0x00800000	/* Backlight brightness can be adjusted */

#define DISP_MODE_NUM_REFRESH		6

typedef struct disp_mode_info {
	short		size;		/* Size of this struct */
	disp_mode_t	mode;		/* Mode number */
	int		xres, yres;	/* Physical display dimensions */
	unsigned	pixel_format;	/* frame buffer pixel format */
	unsigned	flags;
	uintptr_t 	fb_addr;	/* Obsolete - DO NOT USE */
	int 		fb_stride;	/* Obsolete - DO NOT USE */
	unsigned	fb_size;	/* Obsolete - DO NOT USE */
	unsigned	crtc_start_gran;	/* In pixels */
	unsigned	caps;		/* available features */
	union {
		struct {
			short		refresh[DISP_MODE_NUM_REFRESH];	/* Possible refresh rates for this mode */
		} fixed;
		struct {
			int		min_vfreq, max_vfreq;	/* Monitor limitations - in Hz */
			int		min_hfreq, max_hfreq;	/* Monitor limitations - in Hz */
			int		min_pixel_clock;	/* in KHz */
			int		max_pixel_clock;	/* in KHz */
			uint8_t		h_granularity;		/* X resolution must be a multiple of this */
			uint8_t		v_granularity;		/* Y resolution must be a multiple of this */
			uint8_t		sync_polarity;
			uint8_t		reserved;
		} generic;
	} u;
	int		num_colors;	/* Palette modes only */
	unsigned	crtc_pitch_gran;
	unsigned	max_virtual_width;	/* Max width of virtual display (in pixels) */
	unsigned	max_virtual_height;	/* Max height of virtual display */
	unsigned	reserved[2];
} disp_mode_info_t;

/* values defined for sync_polarity in disp_crtc_settings struct */
#define DISP_SYNC_POLARITY_H_POS	1
#define DISP_SYNC_POLARITY_V_POS	2
#define DISP_SYNC_POLARITY_NN		0
#define DISP_SYNC_POLARITY_PN		DISP_SYNC_POLARITY_H_POS
#define DISP_SYNC_POLARITY_NP		DISP_SYNC_POLARITY_V_POS
#define DISP_SYNC_POLARITY_PP		(DISP_SYNC_POLARITY_H_POS|DISP_SYNC_POLARITY_V_POS)

/* DPMS power saving modes modes */
#define DISP_DPMS_MODE_ON		0
#define DISP_DPMS_MODE_STANDBY		1
#define DISP_DPMS_MODE_SUSPEND		2
#define DISP_DPMS_MODE_OFF		4

typedef struct disp_crtc_settings {
	int		xres, yres, refresh;
	unsigned	pixel_clock;
	uint8_t		sync_polarity;
	uint8_t		h_granularity;		/* X resolution must be a multiple of this */
	uint8_t		v_granularity;		/* Y resolution must be a multiple of this */
	uint8_t		reserved0;
	int		h_total;			/* In pixels */
	int		h_blank_start, h_blank_len;	/* In pixels */
	int		reserved1;
	int		reserved2;
	int		h_sync_start, h_sync_len;	/* In pixels */
	int		v_total;			/* In lines */
	int		v_blank_start, v_blank_len;	/* In lines */
	int		reserved3;
	int		reserved4;
	int		v_sync_start, v_sync_len;	/* In lines */
	unsigned	flags;
	int		h_freq;				/* In pixels */
	unsigned	reserved[5];
} disp_crtc_settings_t;

/* disp_layer_query_t.caps */
#define DISP_LAYER_CAP_FILTER			0x00000001
#define DISP_LAYER_CAP_SCALE_REPLICATE		0x00000002
#define DISP_LAYER_CAP_PAN_SOURCE		0x00000004
#define DISP_LAYER_CAP_SIZE_DEST		0x00000008
#define DISP_LAYER_CAP_PAN_DEST			0x00000010
#define DISP_LAYER_CAP_EDGE_CLAMP		0x00000020
#define DISP_LAYER_CAP_EDGE_WRAP		0x00000040
#define DISP_LAYER_CAP_DISABLE			0x00000080
#define DISP_LAYER_CAP_SET_BRIGHTNESS		0x00000100
#define DISP_LAYER_CAP_SET_CONTRAST		0x00000200
#define DISP_LAYER_CAP_SET_SATURATION		0x00000400
#define DISP_LAYER_CAP_ALPHA_WITH_CHROMA	0x00000800
#define DISP_LAYER_CAP_BLEND_WITH_FRONT		0x00001000
#define DISP_LAYER_CAP_PAN_DEST_NEGATIVE	0x00002000
#define DISP_LAYER_CAP_SET_HUE			0x00004000
#define DISP_LAYER_CAP_MAIN_DISPLAY		0x80000000

/* disp_layer_query_t.chromakey_caps */
#define DISP_LAYER_CHROMAKEY_CAP_SRC_SINGLE	0x00000001
#define DISP_LAYER_CHROMAKEY_CAP_SRC_RANGE	0x00000002
#define DISP_LAYER_CHROMAKEY_CAP_SRC_MASK	0x00000004
#define DISP_LAYER_CHROMAKEY_CAP_DST_SINGLE	0x00000100
#define DISP_LAYER_CHROMAKEY_CAP_DST_RANGE	0x00000200
#define DISP_LAYER_CHROMAKEY_CAP_DST_MASK	0x00000400
#define DISP_LAYER_CHROMAKEY_CAP_SHOWTHROUGH	0x00000800
#define DISP_LAYER_CHROMAKEY_CAP_BLOCK		0x00001000

/* "flags" arg. to layer_set_flags() */
#define DISP_LAYER_FLAG_DISABLE_FILTERING	0x00000001
#define DISP_LAYER_FLAG_EDGE_CLAMP		0x00000002
#define DISP_LAYER_FLAG_EDGE_WRAP		0x00000004

typedef enum {
	DISP_MODE_DEVCTL_SET_SYNC_SOURCE = 1,
	DISP_MODE_DEVCTL_UPDATE_DISPLAY_SETTINGS
} disp_mode_devctl_t;

typedef struct {
	int		size;				/* Allocator of this struct sets "size" to sizeof (disp_surface_info_t) */
	unsigned	pixel_format;
	unsigned	caps;
	unsigned	alpha_valid_flags;		/* Valid flags to layer_set_blending() */
	unsigned	alpha_combinations;
	unsigned	chromakey_caps;

	int		src_max_height;			/* Memory surface to be displayed by layer must be no taller than this */
	int		src_max_width;			/* Memory surface to be displayed by layer must be no wider than this */
	int		src_max_viewport_height;	/* Maximum height of the source data image that can be fetched from the memory surface to update the display */
	int		src_max_viewport_width;		/* Maximum width of the source data image that can be fetched from the memory surface to update the display */

	int		dst_max_height;
	int		dst_max_width;
	int		dst_min_height;
	int		dst_min_width;

	int		max_scaleup_x;
	int		max_scaleup_y;

	int		max_scaledown_x;
	int		max_scaledown_y;

	uint64_t	order_mask;			/* 1 bit per layer */
	uint32_t	output_mask;			/* 1 bit per output */
	uint32_t	vcap_mask;			/* 1 bit per capture unit */
	
	uintptr_t	reserved[7];
} disp_layer_query_t;

__BEGIN_DECLS

typedef struct disp_modefuncs {
	int (*init)(disp_adapter_t *adapter, char *optstring);
	void (*fini)(disp_adapter_t *adapter);
	void (*module_info)(disp_adapter_t *adapter, disp_module_info_t *info);
	int (*get_modeinfo)(disp_adapter_t *adapter, int dispno,
	    disp_mode_t mode, disp_mode_info_t *info);
	int (*get_modelist)(disp_adapter_t *adapter,
	    int dispno, disp_mode_t *modelist, int index, int size);
	int (*set_mode)(disp_adapter_t *adapter, int dispno,
	    disp_mode_t mode, disp_crtc_settings_t *settings,
	    disp_surface_t *surf, unsigned flags);
	void (*reserved)();
	int (*wait_vsync)(disp_adapter_t *adapter, int dispno);
	void (*set_backlight_intensity)(disp_adapter_t *adapter,
	    int dispno, int brightness);
	void (*restore_vga)(disp_adapter_t *adapter);
	int (*set_dpms_mode)(disp_adapter_t *adapter, int dispno, int mode);
	int (*set_display_offset)(disp_adapter_t *adapter, int dispno,
	    unsigned offset, int wait_vsync);
	int (*set_palette)(disp_adapter_t *adapter, int dispno,
	    int index, int count, disp_color_t *pal);
	void (*set_scroll_position)(disp_adapter_t *adapter,
	    int dispno, unsigned xoff, unsigned yoff);

	int (*layer_query)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, int fmt_idx, disp_layer_query_t *info);
	int (*layer_enable)(disp_adapter_t *adapter, int dispno, int layer_idx);
	int (*layer_disable)(disp_adapter_t *adapter, int dispno, int layer_idx);
	int (*layer_set_surface)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, unsigned layer_format, disp_surface_t *surf[]);
	int (*layer_set_source_viewport)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, int x1, int y1, int x2, int y2);
	int (*layer_set_dest_viewport)(disp_adapter_t *adapter,
	    int dispno, int layer_idx, int x1, int y1, int x2, int y2);
	int (*layer_set_blending)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, unsigned alpha_mode, int m1, int m2);
	int (*layer_set_chromakey)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, unsigned chroma_mode,
	    disp_color_t color0, disp_color_t color1, disp_color_t mask);
	int (*layer_set_brightness)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, int brightness);
	int (*layer_set_saturation)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, int saturation);
	int (*layer_set_contrast)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, int contrast);
	int (*layer_set_flags)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, unsigned flag_mask, unsigned flag_values);
	void (*layer_update_begin)(disp_adapter_t *adapter,
	    int dispno, uint64_t layer_mask);
	void (*layer_update_end)(disp_adapter_t *adapter,
	    int dispno, uint64_t layer_mask, int wait_vsync);
	void (*layer_reset)(disp_adapter_t *adapter, int dispno, int layer_idx);

	int (*set_power_mode)(disp_adapter_t *adapter, pm_power_mode_t mode);

	int (*layer_set_order)(disp_adapter_t *adapter, int dispno, unsigned order[]);

	int (*set_hw_cursor)(disp_adapter_t *ctx, int dispno,
	    uint8_t *fg_bmp, uint8_t *bg_bmp,
	    unsigned fg_color, unsigned bg_color,
	    int hotspot_x, int hotspot_y,
	    int size_x, int size_y, int bmp_stride);
	void (*enable_hw_cursor)(disp_adapter_t *ctx, int dispno);
	void (*disable_hw_cursor)(disp_adapter_t *ctx, int dispno);
	void (*set_hw_cursor_pos)(disp_adapter_t *ctx, int dispno, int x, int y);

	int (*i2c_read)(disp_adapter_t *ctx, int busno, int slaveaddr,
	    uint8_t *ibuf, int ibytes);
	int (*i2c_write)(disp_adapter_t *ctx, int busno, int slaveaddr,
	    uint8_t *obuf, int obytes);
	int (*i2c_writeread)(disp_adapter_t *ctx, int busno, int slaveaddr,
	    uint8_t *obuf, int obytes, uint8_t *ibuf, int ibytes);

	int (*layer_enable_outputs)(disp_adapter_t *adapter,
	    int dispno, int layer, uint32_t output_mask);

	int (*set_cursor_index)(disp_adapter_t *adapter,
	    int dispno, int cursor_idx);

	int (*layer_set_alpha_map)(disp_adapter_t *adapter,
	    int dispno, int layer, disp_surface_t *map);

	int (*layer_set_hue)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, int hue);

	int (*snapshot)(disp_adapter_t *adapter, int dispno, int output,
	    int x1, int y1, int x2, int y2, disp_surface_t *surf);

	int (*set_external_chroma)(disp_adapter_t *adapter, int dispno,
	    unsigned chroma_mode,
	    disp_color_t color0, disp_color_t color1, disp_color_t mask);

	int (*devctl)(disp_adapter_t *adapter, int dispno,
	    disp_mode_devctl_t cmd, void *data_in, int nbytes,
	    void *data_out, int *out_buffer_size);

	int (*set_color_lut16)(disp_adapter_t *adapter,
	    int dispno, const uint16_t *redLUT, const uint16_t *greenLUT, const uint16_t *blueLUT);

	int (*layer_flushrect)(disp_adapter_t *adapter, int dispno,
	    int layer_idx, int x1, int y1, int x2, int y2);
	    
	void (*reserved1[1])();
} disp_modefuncs_t;

/* Main mode-switcher entry point */
typedef int (*get_modefuncs_t)(disp_adapter_t *adapter,
    disp_modefuncs_t *funcs, int tabsize);

extern WINSIM_API int devg_get_modefuncs(disp_adapter_t *adapter,
    disp_modefuncs_t *funcs, int tabsize);

__END_DECLS

#endif /* _GRAPHICS_MODE_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/mode.h $ $Rev: 657836 $" )
