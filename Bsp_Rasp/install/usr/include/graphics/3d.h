/* Copyright 2009, QNX Software Systems. 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"). You may not 
 * reproduce, modify or distribute this software except in compliance with the 
 * License. You may obtain a copy of the License at:
 * 
 * http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" basis, WITHOUT WARRANTIES OF ANY 
 * KIND, either express or implied.
 * 
 * This file may contain contributions from others, either as contributors under 
 * the License or as licensors under other terms.  Please review this entire file 
 * for other proprietary rights or license notices, as well as the QNX Development 
 * Suite License Guide at http://licensing.qnx.com/license-guide/ for other information.
 */
#ifndef _GRAPHICS_3D_H_INCLUDED
#define _GRAPHICS_3D_H_INCLUDED

#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#include <graphics/display.h>
#endif

#ifndef _GLES_GL_H_INCLUDED
#include <GLES/gl.h>
#endif
#ifndef _GLES_GLEXT_H_INCLUDED
#include <GLES/glext.h>
#endif

#ifndef _GRAPHICS_REND_H_INCLUDED
#include <graphics/rend.h>
#endif

#ifdef _MSC_VER
#define near d_near
#define far d_far
#endif

/* Sizes of the matrix stacks */
#define	DISP_3D_MODEL_MATRIX_ENTRIES	16
#define	DISP_3D_PROJ_MATRIX_ENTRIES	2
#define	DISP_3D_TEX_MATRIX_ENTRIES	2

/* Max supported lights */
#define DISP_3D_NUM_LIGHTS		8

/* Max supported texture mapping units */
#define DISP_3D_MAX_TMUS		4

/* Max supported texture dimensions */
#define DISP_3D_MAX_TEXTURE_SIZE	2048
#define DISP_3D_LOG2_MAX_TEXTURE_SIZE	11

#define __MKFMT(__bpp, __id)	((__id) | (__bpp)<<24)
#define DISP_TEX_BITS_PER_PIXEL(__fmt)	((__fmt)>>24)

#define DISP_TEXFORMAT_UNDEFINED	0
#define DISP_TEXFORMAT_RGBA8888		__MKFMT(32, 1)
#define DISP_TEXFORMAT_RGB888		__MKFMT(24, 2)
#define DISP_TEXFORMAT_PK_RGBA4444	__MKFMT(16, 3)
#define DISP_TEXFORMAT_PK_ARGB4444	__MKFMT(16, 4)
#define DISP_TEXFORMAT_PK_RGBA5551	__MKFMT(16, 5)
#define DISP_TEXFORMAT_PK_RGB565	__MKFMT(16, 6)
#define DISP_TEXFORMAT_PKOE_RGBA4444	__MKFMT(16, 7)
#define DISP_TEXFORMAT_PKOE_ARGB4444	__MKFMT(16, 8)
#define DISP_TEXFORMAT_PKOE_RGB565	__MKFMT(16, 9)
#define DISP_TEXFORMAT_LA88		__MKFMT(16, 10)
#define DISP_TEXFORMAT_L8		__MKFMT(8, 11)
#define DISP_TEXFORMAT_A8		__MKFMT(8, 12)
#define DISP_TEXFORMAT_PAL8_RGB888	__MKFMT(8, 13)
#define DISP_TEXFORMAT_PAL8_RGBA8888	__MKFMT(8, 14)
#define DISP_TEXFORMAT_PAL4_RGB888	__MKFMT(4, 15)
#define DISP_TEXFORMAT_PAL4_RGBA8888	__MKFMT(4, 16)
#define DISP_TEXFORMAT_ARGB8888		__MKFMT(32, 17)
#define DISP_TEXFORMAT_PK_ARGB1555	__MKFMT(16, 18)
#define DISP_TEXFORMAT_PK_XRGB1555	__MKFMT(16, 19)
#define DISP_TEXFORMAT_PKOE_ARGB1555	__MKFMT(16, 20)
#define DISP_TEXFORMAT_PKOE_XRGB1555	__MKFMT(16, 21)
#define DISP_TEXFORMAT_BGR888		__MKFMT(24, 22)
#define DISP_TEXFORMAT_BGRA8888		__MKFMT(32, 23)
#define DISP_TEXFORMAT_PKOE_RGBA5551	__MKFMT(16, 24)
#define DISP_TEXFORMAT_ABGR8888		__MKFMT(32, 25)
#define DISP_TEXFORMAT_AL88		__MKFMT(16, 26)
/* Fujitsu proprietary compression */
#define DISP_TEXFORMAT_FJCMP_RGBA5551	__MKFMT(16, 27)
#define DISP_TEXFORMAT_FJCMP_RGBA8888	__MKFMT(32, 28)

/* Only valid when indicating a preferred internal texture format */
#define DISP_TEXFORMAT_RGB_ANY		100
#define DISP_TEXFORMAT_RGBA_ANY		101

#define	DISP_3D_STATE_MODELMATRIX	0x00000001
#define	DISP_3D_STATE_PROJMATRIX	0x00000002
#define	DISP_3D_STATE_TEXMATRIX		0x00000004
#define	DISP_3D_STATE_NORMAL		0x00000008
#define	DISP_3D_STATE_DEPTH_RANGE	0x00000010
#define	DISP_3D_STATE_SHADING		0x00000020
#define	DISP_3D_STATE_DEPTH		0x00000040
#define	DISP_3D_STATE_ALPHA_TEST	0x00000080
#define	DISP_3D_STATE_BLEND		0x00000100
#define	DISP_3D_STATE_STENCIL		0x00000200
#define	DISP_3D_STATE_LOGIC_OP		0x00000400
#define	DISP_3D_STATE_FOG		0x00000800
#define	DISP_3D_STATE_TMU		0x00001000
#define	DISP_3D_STATE_COLORMASK		0x00002000
#define	DISP_3D_STATE_POLYGON_OFFSET	0x00004000
#define	DISP_3D_STATE_VIEWPORT		0x00008000
#define	DISP_3D_STATE_CULLFACE		0x00010000
#define	DISP_3D_STATE_SCISSOR		0x00020000
#define	DISP_3D_STATE_LIGHT		0x00040000
#define	DISP_3D_STATE_FLAGS		0x00080000
#define	DISP_3D_STATE_ANTIALIAS		0x00100000
#define	DISP_3D_STATE_POINT		0x00200000
#define	DISP_3D_STATE_LINE		0x00400000
#define	DISP_3D_STATE_FSOPS		0x00800000

/* Global light state */
#define	DISP_3D_LIGHT_STATE_TWO_SIDE		0x00000001
#define	DISP_3D_LIGHT_STATE_AMBIENT		0x00000002
#define	DISP_3D_LIGHT_STATE_MAT_AMBIENT		0x00000004
#define	DISP_3D_LIGHT_STATE_MAT_DIFFUSE		0x00000008
#define	DISP_3D_LIGHT_STATE_MAT_SPECULAR	0x00000010
#define	DISP_3D_LIGHT_STATE_MAT_EMISSION	0x00000020
#define	DISP_3D_LIGHT_STATE_MAT_SHININESS	0x00000040
#define	DISP_3D_LIGHT_STATE_COLOR_MATERIAL	0x00000080

/* Per-light state */
#define	DISP_3D_PER_LIGHT_STATE_AMBIENT			0x00000001
#define	DISP_3D_PER_LIGHT_STATE_DIFFUSE			0x00000002
#define	DISP_3D_PER_LIGHT_STATE_SPECULAR		0x00000004
#define	DISP_3D_PER_LIGHT_STATE_POSITION		0x00000008
#define	DISP_3D_PER_LIGHT_STATE_SPOT_DIRECTION		0x00000010
#define	DISP_3D_PER_LIGHT_STATE_SPOT_EXPONENT		0x00000020
#define	DISP_3D_PER_LIGHT_STATE_SPOT_CUTOFF		0x00000040
#define	DISP_3D_PER_LIGHT_STATE_CONSTANT_ATTENUATION	0x00000080
#define	DISP_3D_PER_LIGHT_STATE_LINEAR_ATTENUATION	0x00000100
#define	DISP_3D_PER_LIGHT_STATE_QUADRATIC_ATTENUATION	0x00000200

/* Buffer objects */
#define	DISP_3D_BUFOBJ_NONE				0x00000000
#define	DISP_3D_BUFOBJ_VERTEX				0x00000001
#define	DISP_3D_BUFOBJ_COLOR				0x00000002
#define	DISP_3D_BUFOBJ_NORMAL				0x00000004
#define	DISP_3D_BUFOBJ_TEXCOORD0			0x00000008
#define	DISP_3D_BUFOBJ_TEXCOORD1			0x00000010
#define	DISP_3D_BUFOBJ_TEXCOORD2			0x00000020
#define	DISP_3D_BUFOBJ_TEXCOORD3			0x00000040
#define	DISP_3D_BUFOBJ_INDICES				0x00010000

/* Datatypes for data in buffer objects */
#define	DISP_3D_DATATYPE_NONE				0x00000000
#define	DISP_3D_DATATYPE_CHAR		 		0x00000001
#define	DISP_3D_DATATYPE_UCHAR		 		0x00000002
#define	DISP_3D_DATATYPE_SHORT				0x00000003
#define	DISP_3D_DATATYPE_USHORT				0x00000004
#define	DISP_3D_DATATYPE_FIXED32			0x00000005
#define	DISP_3D_DATATYPE_FLOAT32			0x00000006

/* For specifying how primitive information will be sent to driver */
typedef enum {
	DISP_3D_SEND_OBJECT_COORDS =	0x00000001,	/* Send vertices in Object/World coordinates */
	DISP_3D_SEND_LIT_EYE_COORDS =	0x00000002,	/* Send vertices in Eye coordinates, after lighting */
	DISP_3D_SEND_CLIPPED_PRIMS =	0x00000004,	/* Send clipped primitives */
	DISP_3D_SEND_SPANS =		0x00000010,	/* Render primitives as a series of spans */
} disp_3d_send_type;

/* Actions to be taken on vertices before sending to driver */
typedef enum {
	DISP_3D_PRIMSTATE_XFORM_TEX = 	0x00000001,	/* Multiply texture coordinates by current texture matrix */
	DISP_3D_PRIMSTATE_DIV_BY_W = 	0x00000002,	/* Perform division by W (clipped primitives only) */
	DISP_3D_PRIMSTATE_VP_XLATE = 	0x00000004,	/* Translate to viewport coordinates (clipped primitives only) */
} disp_3d_primstate_flag;

typedef enum {
	DISP_3D_DATA_COORDS,		/* X, Y, Z, W */
	DISP_3D_DATA_NORMAL,		/* X, Y, Z */
	DISP_3D_DATA_RGBA,		/* R, G, B, A */
	DISP_3D_DATA_STRQ0,		/* S, T, R, Q */
	DISP_3D_DATA_STRQ1,		/* S, T, R, Q */
	DISP_3D_DATA_STRQ2,		/* S, T, R, Q */
	DISP_3D_DATA_STRQ3,		/* S, T, R, Q */
	DISP_3D_DATA_FOG,		/* Fog distance */
} disp_3d_data_type;

typedef enum {
	DISP_3D_CTX_FLAG_USE_VTXCOLOR =		0x00000001,
	DISP_3D_CTX_FLAG_USE_VTXNORM =		0x00000002,
	DISP_3D_CTX_FLAG_PRIM_USE_VTXCOLOR =	0x00000004
} disp_3d_ctx_flag;

/* Driver may set these flags when processing Lit Eye Co-ordinates, if it
 * determines that it can't process the specified primitives correctly.
 * For example, the driver may be able to render triangle strips and
 * fans etc., but cannot handle the case where vertices are outside of the
 * far or near clipping planes.
 */
typedef enum {
	DISP_3D_PCTL_FLAG_LEC_FBACK_MM_CP =	0x00000001,	/* Matrix multiplication still needs to be performed on the vertices, and the driver is prepared to accept clipped primitives */
	DISP_3D_PCTL_FLAG_LEC_FBACK_CP =	0x00000002,	/* The driver is prepared to accept clipped primitives */
	DISP_3D_PCTL_FLAG_LEC_FBACK_MM_SPANS =	0x00000004,	/* Matrix multiplication still needs to be performed on the vertices, and the driver can only accept spans */
	DISP_3D_PCTL_FLAG_LEC_FBACK_SPANS =	0x00000008,	/* The driver can only accept spans */
} disp_3d_pctl_flag;

#define DISP_3D_PCTL_FLAG_LEC_FBACK_MASK \
		(DISP_3D_PCTL_FLAG_LEC_FBACK_MM_CP | \
		DISP_3D_PCTL_FLAG_LEC_FBACK_CP | \
		DISP_3D_PCTL_FLAG_LEC_FBACK_MM_SPANS | \
		DISP_3D_PCTL_FLAG_LEC_FBACK_SPANS)

typedef enum {
	DISP_3D_MATRIX_FLAG_IS_IDENTITY = 0x00000001,
} disp_3d_matrix_flag;

/* Defines for "flags" argument to prim_begin() */
#define	DISP_3D_VTX_FLAG_DFLT_Z		0x00000001
#define	DISP_3D_VTX_FLAG_DFLT_W		0x00000002
#define	DISP_3D_VTX_FLAG_DFLT_R(__tmu)	(1<<((__tmu)+4))
#define	DISP_3D_VTX_FLAG_DFLT_Q(__tmu)	(1<<((__tmu)+12))

typedef struct {
	int		allocated;
	unsigned	format[DISP_3D_LOG2_MAX_TEXTURE_SIZE+1];	/* See DISP_3D_TEXFORMAT_* */
	int		max_lod;
	GLfixed		switchover;
	void		*image[DISP_3D_LOG2_MAX_TEXTURE_SIZE+1];
	int		width[DISP_3D_LOG2_MAX_TEXTURE_SIZE+1];
	int		height[DISP_3D_LOG2_MAX_TEXTURE_SIZE+1];
	int		stride[DISP_3D_LOG2_MAX_TEXTURE_SIZE+1];
	void		*palette;
	int		compressed_size;
	GLenum		min_filter;
	GLenum		mag_filter;
	int		min_linear;
	GLenum		wrap_s;
	GLenum		wrap_t;
	void		*driver_private;
} disp_texture_t;

typedef struct {
	unsigned	name;
	GLenum		usage;
	unsigned	size;		/* In bytes */
	void		*dataptr;
	void		*driver_private;
} disp_bufobj_t;

typedef struct light_state {
	struct {
		disp_real	r;
		disp_real	g;
		disp_real	b;
		disp_real	a;
	} ambient;
	struct {
		disp_real	r;
		disp_real	g;
		disp_real	b;
		disp_real	a;
	} diffuse;
	struct {
		disp_real	r;
		disp_real	g;
		disp_real	b;
		disp_real	a;
	} specular;
	struct {
		disp_real	x;
		disp_real	y;
		disp_real	z;
		disp_real	w;
	} position;
	struct {
		disp_real	x;
		disp_real	y;
		disp_real	z;
		GLboolean	transformed;
	} spot_direction;
	disp_real	spot_exponent;
	disp_real	spot_cutoff;
	disp_real	cos_spot_cutoff;
	disp_real	constant_attenuation;
	disp_real	linear_attenuation;
	disp_real	quadratic_attenuation;
	GLboolean	enabled;
	unsigned	new_state;
	unsigned	imp_flags;
	unsigned	reserved[3];
} disp_3d_light_state_t;

typedef struct {
	int			enabled;
	int			complete;
	disp_real		s;
	disp_real		t;
	disp_real		r;
	disp_real		q;
	GLenum			env_mode;
	int			function;
	disp_real		env_r;
	disp_real		env_g;
	disp_real		env_b;
	disp_real		env_a;
	disp_real		tex_matrix[DISP_3D_TEX_MATRIX_ENTRIES][16];
	disp_3d_matrix_flag	tex_matrix_flags[DISP_3D_TEX_MATRIX_ENTRIES];
	int			tex_matrix_stackptr;
} disp_tmu_state_t;

typedef struct {
	disp_real		x;
	disp_real		y;
	disp_real		z;
	disp_real		w;
	disp_real		r[2];
	disp_real		g[2];
	disp_real		b[2];
	disp_real		a;
	disp_real		fog;
	struct {
		disp_real		s;
		disp_real		t;
		disp_real		r;
		disp_real		q;
	} t[DISP_3D_MAX_TMUS];
	union {
		struct {
			disp_real	x;
			disp_real	y;
			disp_real	z;
		} normal;
		struct {
			/*
			 * Stores NDC's or Viewports coords, depending
			 * on what the device wants.
			 */
			disp_real	x;
			disp_real	y;
			disp_real	z;
		} ndc;
	} u;
	unsigned		flags;		/* Not for driver's use */
} disp_3d_vertex_t;

typedef struct disp_3d_span_params {
	struct disp_rendercontext	*rctx;
	GLfixed				r_start;
	GLfixed				r_step;
	GLfixed				g_start;
	GLfixed				g_step;
	GLfixed				b_start;
	GLfixed				b_step;
	GLfixed				a_start;
	GLfixed				a_step;

	GLfixed				f_start;
	GLfixed				f_step;

	int				z_start;
	int				z_step;

	GLfixed				*spanbuf;
	GLfixed				*cvgbuf;
	unsigned		reserved[7];
} disp_3d_span_params_t;

typedef struct disp_3d_prim_state {
	disp_3d_send_type	send_type;
	disp_3d_primstate_flag	flags;
	unsigned		reserved[4];
} disp_3d_prim_state_t;

typedef struct disp_3d_data_ptr {
	disp_real	*xptr;
	disp_real	*yptr;
	disp_real	*zptr;
	disp_real	*wptr;

	disp_real	*rptr;
	disp_real	*gptr;
	disp_real	*bptr;
	disp_real	*aptr;

	disp_real	*tsptr[DISP_3D_MAX_TMUS];
	disp_real	*ttptr[DISP_3D_MAX_TMUS];
	disp_real	*trptr[DISP_3D_MAX_TMUS];
	disp_real	*tqptr[DISP_3D_MAX_TMUS];

	int		xstride;
	int		ystride;
	int		zstride;
	int		wstride;

	int		rstride;
	int		gstride;
	int		bstride;
	int		astride;

	int		tsstride[DISP_3D_MAX_TMUS];
	int		ttstride[DISP_3D_MAX_TMUS];
	int		trstride[DISP_3D_MAX_TMUS];
	int		tqstride[DISP_3D_MAX_TMUS];

	disp_real	*nxptr;
	disp_real	*nyptr;
	disp_real	*nzptr;

	int		nxstride;
	int		nystride;
	int		nzstride;

	uint32_t	reserved[6];
} disp_3d_data_ptrs_t;

__BEGIN_DECLS

typedef struct disp_3d_pipeline_ctl {
	disp_3d_prim_state_t	tri;
	disp_3d_prim_state_t	line;
	disp_3d_prim_state_t	points;

	void (*prim_begin)(void *context,
	    GLenum primitive, int nvertices, unsigned flags);
	void (*prim_end)(void *context);

	/* For drivers that can do lighting and/or view volume clipping */
	void (*send_vertices)(void *context, int nvertices);

	/* For drivers that can do view volume clipping, but not lighting */
	void (*send_lit_vertices)(void *context,
	    disp_3d_vertex_t *v, int nvertices);

	/* For drivers that take pre-lit, pre-clipped primitives */
	void (*draw_triangle)(void *context,
	    disp_3d_vertex_t *v1, disp_3d_vertex_t *v2, disp_3d_vertex_t *v3);
	void (*draw_line)(void *context,
	    disp_3d_vertex_t *v1, disp_3d_vertex_t *v2);
	void (*draw_points)(void *context, disp_3d_vertex_t *v, int npoints);

	/* Span function supporting OpenGL ES per-fragment operations */
	void (*draw_span)(void *context,
	    int x, int y, int count, disp_3d_span_params_t *params);

	disp_3d_data_ptrs_t	*data_ptrs;
	int			max_vertices;

	/* For drivers with direct access to array data via buffer objects */
	int (*draw_arrays)(void *context, GLenum prim, int first, int nvertices);
	int (*draw_elements)(void *context, GLenum,
	    unsigned nvertices, disp_bufobj_t *obj, unsigned datatype);
	int (*draw_elements_immed)(void *context, GLenum,
	    unsigned nvertices, const void *data, unsigned datatype);

	/* See DISP_3D_PCTL_FLAG_* */
	uint32_t		flags;

	uint32_t		reserved[3];
} disp_3d_pipeline_ctl_t;

typedef struct disp_rendercontext {
	unsigned		new_state;		/* See DISP_3D_STATE_* */
	unsigned		new_global_light_state;	/* See DISP_3D_LIGHT_STATE_* */
	unsigned		new_light_state;	/* Per-light bitmask */
	unsigned		new_tmu_state;		/* Per-tmu bitmask */

	unsigned		flags;			/* See DISP_3D_CTX_FLAG_* */

	unsigned		enabled_features;	/* Driver must check! */

	struct {
		GLboolean	alpha_test;
		GLboolean	blend;
		GLboolean	color_logic_op;
		GLboolean	color_material;
		GLboolean	cull_face;
		GLboolean	depth_test;
		GLboolean	fog;
		GLboolean	lighting;
		GLboolean	line_smooth;
		GLboolean	normalize;
		GLboolean	point_smooth;
		GLboolean	polygon_offset_fill;
		GLboolean	rescale_normal;
		GLboolean	scissor_test;
		GLboolean	stencil_test;
	} enabled;

	int		lights_on;

	struct light_state light[DISP_3D_NUM_LIGHTS];

	struct {
		struct {
			disp_real	r;
			disp_real	g;
			disp_real	b;
			disp_real	a;
		} ambient;
		GLboolean		twoside;
	} lightmodel;

	struct {
		struct {
			disp_real	r;
			disp_real	g;
			disp_real	b;
			disp_real	a;
		} ambient;
		struct {
			disp_real	r;
			disp_real	g;
			disp_real	b;
			disp_real	a;
		} diffuse;
		struct {
			disp_real	r;
			disp_real	g;
			disp_real	b;
			disp_real	a;
		} specular;
		struct {
			disp_real	r;
			disp_real	g;
			disp_real	b;
			disp_real	a;
		} emission;
		disp_real		shininess;
	} material;

	GLenum	frontface;
	GLenum	cullface;

	struct {
		/* Buffer clear values */
		disp_real	r;
		disp_real	g;
		disp_real	b;
		disp_real	a;
		disp_real	depth;
		GLint		stencil;
	} clear;

	struct {
		int		x1;
		int		x2;
		int		y1;
		int		y2;
	} cliprect;

	struct {
		disp_real	near;		
		disp_real	far;
		GLenum		func;
		GLboolean	mask;
	} depth;

	struct {
		GLenum		func;
		GLenum		fail;
		GLenum		zfail;
		GLenum		zpass;
		GLuint		testmask;
		GLuint		planemask;
		GLint		ref;
	} stencil;

	struct {
		int		x;
		int		y;
		int		width;
		int		height;
	} viewport;

	struct {
		disp_real	r;	
		disp_real	g;	
		disp_real	b;	
		disp_real	a;	
	} color;

	struct {
		GLboolean	r;
		GLboolean	g;
		GLboolean	b;
		GLboolean	a;
	} colormask;

	struct {
		GLenum		func;
		disp_real	ref;
	} alpha;

	struct {
		GLenum		sfactor;
		GLenum		dfactor;
	} blend;

	struct {
		GLenum		mode;
		disp_real	density;
		disp_real	start;
		disp_real	end;
		disp_real	r;
		disp_real	g;
		disp_real	b;
		disp_real	a;
	} fog;

	GLenum			rop;

	GLenum			shademodel;

	struct {
		disp_real	x;
		disp_real	y;
		disp_real	z;
	} normal;

	/* Space for matrix stacks */
	disp_real		model_matrix[DISP_3D_MODEL_MATRIX_ENTRIES][16];
	disp_real		proj_matrix[DISP_3D_PROJ_MATRIX_ENTRIES][16];

	disp_3d_matrix_flag	model_matrix_flags[DISP_3D_MODEL_MATRIX_ENTRIES];
	disp_3d_matrix_flag	proj_matrix_flags[DISP_3D_PROJ_MATRIX_ENTRIES];

	/* Indices into matrix stacks */
	int			model_matrix_stackptr;
	int			proj_matrix_stackptr;

	disp_real		point_size;
	disp_real		line_width;

	struct {
		disp_real	factor;
		disp_real	units;
	} polygon_offset;

	disp_tmu_state_t	tmu[DISP_3D_MAX_TMUS];
	int			enabled_tmus;
	int			complete_tmus;

	disp_texture_t		*textures[DISP_3D_MAX_TMUS];

	disp_3d_pipeline_ctl_t	pipeline_ctl;

	uint32_t		ddflags;	/* Driver debugging flags */

	uint32_t		line_stipple;	/* Only 16 bits used */

	struct {
		GLboolean	dither;
		GLboolean	fsaa;
		GLboolean	reserved[2];
	} enabled2;

	uint32_t		vtx_flags;
	uint32_t		reserved[28];
} disp_rendercontext_t;

typedef enum {
	DISP_3D_CAP_ACCEL =			0x00000001,	/* Driver has some form of 3D acceleration support */
	DISP_3D_CAP_INDEPENDANT_PM =		0x00000020	/* 3D engine can be independently power managed (i.e. not tied to 2D engine) */
} disp_3d_cap_flag;

typedef struct disp_3d_caps {
	disp_3d_cap_flag	flags;				/* See DISP_3D_CAP_* */
	int			fixed_point;
	int			max_vertices;
	const char		*extensions;
	unsigned		num_tmus;
	unsigned		max_line_width;
	unsigned		max_point_size;
	unsigned		max_line_width_aa;
	unsigned		max_point_size_aa;
	unsigned		reserved[1];
} disp_3d_caps_t;

typedef struct disp_draw_3dfuncs {
	void * (*init)(disp_adapter_t *adapter, const char *options);
	void (*fini)(void *handle);
	void (*module_info)(disp_adapter_t *adapter, disp_module_info_t *info);

	int (*query_caps)(void *handle, disp_3d_caps_t *caps);

	int (*query_buffer_config)(void *handle,
	    int index, disp_rend_buffer_config_t *config);

	void * (*create_context)(void *handle, disp_rendercontext_t *rctx,
	    disp_rend_context_params_t *params);
	void (*destroy_context)(void *context);

	void * (*create_target)(void *handle,
	    int cfg_index, int width, int height);
	void (*destroy_target)(void *handle, void *target);

	void (*create_texture)(void *context, disp_texture_t *tex);
	void (*destroy_texture)(void *context, disp_texture_t *tex);
	void (*set_tex_image)(void *context, disp_texture_t *tex,
	    int width, int height, int lod, unsigned format,
	    unsigned reqformat, int stride, const void *data, void *palette);
	void (*set_tex_image_copy)(void *context, disp_texture_t *tex,
	    disp_surface_t *src, int sxoff, int syoff, int width, int height,
	    int lod, unsigned reqformat);
	void (*update_tex_image)(void *context, disp_texture_t *tex,
	    int dxoff, int dyoff, int width, int height, int lod,
	    unsigned format, int stride, const void *data, void *palette);
	void (*update_tex_image_copy)(void *context, disp_texture_t *tex,
	    disp_surface_t *src, int sxoff, int syoff, int dxoff, int dyoff,
	    int width, int height, int lod);

	void (*read_image)(void *handle, int sxoff, int syoff,
	    int width, int height, unsigned dformat, int dstride,
	    disp_surface_t * src, void *dst, int y_order_ascending);

	void (*flush)(void *context);
	void (*wait_idle)(void *context);

	void (*make_current)(void *context, void *target);

	void (*set_draw_surface)(void *context, disp_surface_t *surf);

	/* Notifies driver that span rendering is about to occur */
	int (*span_prepare)(void *context);

	GLbitfield (*clear)(void *context, GLbitfield buffer_mask);

	int (*update_state)(void *context);

	int (*set_power_mode)(disp_adapter_t *adapter, pm_power_mode_t mode);

	/* Buffer Objects (optional - may be NULL) */
	int (*bufobj_create)(void *context, disp_bufobj_t *obj);
	void (*bufobj_destroy)(void *context, disp_bufobj_t *obj);
	int (*bufobj_set_data)(void *context, disp_bufobj_t *obj,
	    uintptr_t offset, uintptr_t nbytes, const void *data, int realloc);
	int (*bufobj_set_datasource)(void *context, disp_bufobj_t *obj,
	    unsigned array, uintptr_t offset,
	    unsigned datatype, int stride, int n_elements);
	void (*bufobj_set_enabled)(void *context, unsigned array, int enabled);

	/* Display Lists - UNSUPPORTED */
	void * (*dlist_begin)(void *context);
	int (*dlist_end)(void *context);
	void (*dlist_execute)(void *context, void *dlist);
	void (*dlist_destroy)(void *context, void *dlist);

	/* Driver-specific extension support */
	int (*state_set_enabled)(void *context, GLenum cap, int enabled,
	    int active_tmu, int active_client_tmu, intptr_t reserved);
	void (*(*get_proc_addr)(void *context, const char *procname))();
	int (*query_tex_format)(void *context,
	    GLenum type, int *bpp, unsigned *tex_format);

	void (*reserved[5])(void);
} disp_draw_3dfuncs_t;

typedef int (*get_rendfuncs_t)(disp_adapter_t *adapter,
    disp_draw_3dfuncs_t *funcs, int tabsize);

/* Main draw driver entry points */
extern SOFT3D_API int devg_get_rendfuncs(disp_adapter_t *adapter,
    disp_draw_3dfuncs_t *funcs, int tabsize);

__END_DECLS

#endif /* _GRAPHICS_3D_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/3d.h $ $Rev: 657836 $" )
