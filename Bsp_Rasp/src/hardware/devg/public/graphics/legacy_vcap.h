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





/* Types and defines for TV tuner / video capture support */

#ifndef _GRAPHICS_LEGACY_VCAP_H_INCLUDED
#define _GRAPHICS_LEGACY_VCAP_H_INCLUDED

#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#include <graphics/display.h>
#endif

/* Capture / tuner device capability flags (disp_vcap_channel_caps_t.flags) */
#define DISP_VCAP_CAP_SOURCE_TUNER		0x00000001
#define DISP_VCAP_CAP_SOURCE_SVIDEO		0x00000002
#define DISP_VCAP_CAP_SOURCE_COMPOSITE		0x00000004
#define	DISP_VCAP_CAP_BRIGHTNESS_ADJUST		0x00000008
#define	DISP_VCAP_CAP_CONTRAST_ADJUST		0x00000010
#define	DISP_VCAP_CAP_SATURATION_ADJUST		0x00000020
#define	DISP_VCAP_CAP_HUE_ADJUST		0x00000040
#define DISP_VCAP_CAP_VOLUME_ADJUST		0x00000080
#define DISP_VCAP_CAP_AUDIO_SOURCE_MUTE		0x00000100	/* Audio source can be muted */
#define DISP_VCAP_CAP_AUDIO_SOURCE_TUNER	0x00000200
#define DISP_VCAP_CAP_AUDIO_SOURCE_EXTERNAL	0x00000400	/* Line in */
#define DISP_VCAP_CAP_TEMPORAL_DECIMATION	0x00000800
#define DISP_VCAP_CAP_DOWNSCALING		0x00001000
#define DISP_VCAP_CAP_UPSCALING			0x00002000
#define DISP_VCAP_CAP_CROPPING			0x00004000
#define DISP_VCAP_CAP_DOUBLE_BUFFER		0x00008000

/* Capture / tuner device property flags (disp_vcap_channel_props_t.flags) */
#define DISP_VCAP_FLAG_AFT_ON			0x00000001
#define DISP_VCAP_FLAG_RUNNING			0x00000002
#define DISP_VCAP_FLAG_DOUBLE_BUFFER		0x00000004
#define DISP_VCAP_FLAG_SYNC_WITH_SCALER		0x00000008

/* Capture / tuner device status flags (disp_vcap_channel_status_t.flags) */
#define DISP_VCAP_STATUS_TUNED			0x00000001	/* Tuner PLL has locked */
#define DISP_VCAP_STATUS_CHANNEL_PRESENT	0x00000002	/* Good signal detected on current Radio or TV channel */
#define DISP_VCAP_STATUS_VIDEO_PRESENT		0x00000004	/* Video signal detected */
#define DISP_VCAP_STATUS_STEREO			0x00000008	/* Stereo audio signal */

/* Capture / tuner device status flags (disp_vcap_channel_props_t.source) */
#define DISP_VCAP_SOURCE_TUNER			0x00000001
#define DISP_VCAP_SOURCE_SVIDEO			0x00000002
#define DISP_VCAP_SOURCE_COMPOSITE		0x00000004
#define DISP_VCAP_AUDIO_SOURCE_MUTE		0x00000100
#define DISP_VCAP_AUDIO_SOURCE_TUNER		0x00000200
#define DISP_VCAP_AUDIO_SOURCE_EXTERNAL		0x00000400

#define DISP_VCAP_UPDATE_VIDEO_SOURCE		0x00000001	/* "video_source" has changed */
#define DISP_VCAP_UPDATE_AUDIO_SOURCE		0x00000002	/* "audio_source" has changed */
#define DISP_VCAP_UPDATE_INPUT_FORMAT		0x00000004	/* "input_format" has changed */
#define DISP_VCAP_UPDATE_OUTPUT_FORMAT		0x00000008	/* "output_format" has changed */
#define DISP_VCAP_UPDATE_TUNER			0x00000010	/* "tuner_channel" and/or "Fif" and/or "radio_modulation" has changed */
#define DISP_VCAP_UPDATE_OUTPUT_SIZE		0x00000020	/* one or more of "crop_top", "crop bottom", "crop_left", "crop_right", "dst_width" and "dst_height" has changed */
#define DISP_VCAP_UPDATE_BRIGHTNESS		0x00000040	/* "brightness" has changed */
#define DISP_VCAP_UPDATE_CONTRAST		0x00000080	/* "contrast" has changed */
#define DISP_VCAP_UPDATE_SATURATION		0x00000100	/* "u_saturation" and/or "v_saturation" has changed */
#define DISP_VCAP_UPDATE_HUE			0x00000200	/* "hue" has changed */
#define DISP_VCAP_UPDATE_VOLUME			0x00000400	/* "volume" has changed */
#define DISP_VCAP_UPDATE_OUTPUT_TARGET		0x00000800	/* one or more of ?plane? members have changed */

/* Capabilities of a Video Frame Capture device */
typedef struct {
	unsigned	flags;

	unsigned	input_format;		/* PAL, NTSC, etc. */
	int		frame_width;
	int		frame_height;
	int		frame_rate;

	unsigned	output_format;		/* YUV, RGB, etc. */
	int		stride_granularity;	/* Stride of capture buffer must be a multiple of this */

	unsigned	reserved[10];
} disp_vcap_channel_caps_t;

/* Status of a Video Frame Capture device */
typedef struct {
	unsigned 	size;
	unsigned	flags;
	unsigned	signal;
	int 		current_channel;
	unsigned	reserved[8];
} disp_vcap_channel_status_t;

typedef struct {
	unsigned 	size;
	unsigned	flags;
	unsigned	video_source;		/* Device to capture from */
	unsigned	audio_source;		/* Audio source device */
	unsigned	input_format;		/* PAL, NTSC etc. */
	unsigned	output_format;		/* YUV, RGB etc. */
	int		tuner_channel;
	int		Fif;
	short		dst_width;		/* Output width of scaled image */
	short		dst_height;		/* Output height of scaled image */
	short		crop_top;		/* Lines to skip at top of pre-scaled source image */
	short		crop_bottom;		/* Lines to skip at bottom of pre-scaled source image */
	short		crop_left;		/* Samples to skip at left of pre-scaled source image */
	short		crop_right;		/* Samples to skip at right of pre-scaled source image */
	short		brightness;
	short		contrast;
	short		u_saturation;
	short		v_saturation;
	short		hue;
	short		reserved;
	unsigned	update_flags;
	int		scaler_index;
	unsigned	reserved2[6];
} disp_vcap_channel_props_t;

__BEGIN_DECLS

typedef struct disp_legacy_vcapfuncs {
	int (*init)(disp_adapter_t *adapter, char *optstring);
	void (*fini)(disp_adapter_t *adapter);
	void (*module_info)(disp_adapter_t *adapter, disp_module_info_t *info);
	int (*get_channel_caps)(disp_adapter_t *adapter, int channel,
	    int input_fmt_index, int output_fmt_index,
	    disp_vcap_channel_caps_t *caps);
	int (*get_channel_status)(disp_adapter_t *adapter, int channel,
	    disp_vcap_channel_status_t *caps);
	int (*set_channel_props)(disp_adapter_t *adapter, int channel,
	    disp_vcap_channel_props_t *props,
	    disp_surface_t *yplane1, disp_surface_t *yplane2,
	    disp_surface_t *uplane1, disp_surface_t *uplane2,
	    disp_surface_t *vplane1, disp_surface_t *vplane2);
	int (*next_frame)(disp_adapter_t *adapter, int channel);
	int (*close_channel)(disp_adapter_t *adapter, int channel);
	int (*set_power_mode)(disp_adapter_t *adapter, pm_power_mode_t mode);
} disp_legacy_vcapfuncs_t;

/* Main video capture module entry point */
typedef int (*get_legacy_vcapfuncs_t)(disp_adapter_t *adapter,
    disp_legacy_vcapfuncs_t *funcs, int tabsize);

extern int devg_get_legacy_vcapfuncs(disp_adapter_t *adapter,
    disp_legacy_vcapfuncs_t *funcs, int tabsize);

__END_DECLS

#ifndef _GRAPHICS_TV_H_INCLUDED
#include <graphics/tv.h>
#endif

#endif /* _GRAPHICS_VCAP_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/legacy_vcap.h $ $Rev: 657836 $" )
