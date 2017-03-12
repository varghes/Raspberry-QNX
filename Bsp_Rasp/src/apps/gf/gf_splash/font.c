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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>
#include <malloc.h>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if_dl.h>
#include <ifaddrs.h>
#include "gfpr/gfpr.h"
#include <gf/gf.h>
#include <FontFusion/T2K/t2k.h>
#include <FontFusion/T2K/t2kextra.h>
#include "gf_setup.h"
#include "gffont.h"
#if defined(__BIGENDIAN__)
#include <gulliver.h>
#endif

#define TRUETYPE_SPEC_ID 1

extern int verbose;

/********************************************************** PF_SIN_DEGREE() ***/
// A quick function to calculate sin value by using the util_FixSin() function.
// The util_FixSin() expects a range from 0 to 90 in 16dot16.
static F16Dot16
PF_SIN_DEGREE( int angle )
{
	angle %= 360;
	
	if( angle < 0 )
		angle = 360 + angle;
	
	if( angle <= 90 )
		return util_FixSin( angle << 16 );
	
	if( angle > 90 && angle <= 180 )
		return util_FixSin( (180 - angle) << 16 );
		
	if( angle > 180 && angle <= 270 )
		return -util_FixSin( (angle - 180) << 16 );
		
	return -util_FixSin( (360 - angle) << 16 ); 
}

#define PF_COS_DEGREE( in ) 	PF_SIN_DEGREE( 90 - (in) )

/************************************************************ DRAW_STRING() ***/
// Draw a string at the specified location -- returns the width of 
// the rendered string, or -1 on error.
static int
draw_string( gf_setup_t *info,
			 char *str, int font_size, int rot_angle, int rot_angle_increment,
			 int xoffset, int yoffset, int render_type, 
			 T2K_BorderFilerParams *params )
{
	int		        	pos_x = 0, pos_y = 0;
	int             	i, first = 1, n;
	T2K_TRANS_MATRIX	trans;
	int             	errCode = 0;
	uint8_t        		*img;
	gf_surface_t    	t_surface;
	F16Dot16			size = font_size * ONE16Dot16;
	F16Dot16			width = 0;
    long 				sin_val, cos_val;
	gf_context_t		context = info->context;
	T2K					*pscaler = info->scaler;

	sin_val = PF_SIN_DEGREE(rot_angle);
	cos_val = PF_COS_DEGREE(rot_angle);

	trans.t00 = util_FixMul( size, cos_val );
	trans.t01 = util_FixMul( size, sin_val );
	trans.t10 = util_FixMul( size, -sin_val );
	trans.t11 = util_FixMul( size, cos_val );

	// Do the transformation.
	T2K_NewTransformation( pscaler, true, 96, 96, &trans, true, &errCode );

	if( errCode != 0 )
		return -1;

	pos_y = pscaler->yAscender;

	if( verbose ) {
		printf( "Font Metric Information:\n" );
		printf( "\tY Ascender: %ld\n", pscaler->yAscender >> 16 );
		printf( "\tY Descender: %ld\n", pscaler->yDescender >> 16 );
	}

	/* Create a surface that we can re-attach to */
	if( GF_ERR_OK != gf_surface_attach( &t_surface, info->gdev, 0, 0, 0,
							render_type == DRAW_STRING_BORDERED ? GF_FORMAT_BGRA8888 : GF_FORMAT_BYTE, 
							NULL, NULL, 0 )
	) {
		return -1;
	}

	for( i = 0; str[i] != '\x0'; i+=n ) {
		int             iIndex = 0, mod = 0;
		wchar_t         wc;  
		
		n = mbtowc(&wc, &str[i], 4);
		if( n == -1 )
			return -1; //Invalid multibyte character found

		if( n == 0 )
			goto bail; //got a NULL byte, string is finished.
		
		if( wc != 0 )
			iIndex = T2K_GetGlyphIndex( pscaler, (uint16)wc, &errCode );

		if( errCode != 0 || ( wc != 0 && iIndex == 0 ) )
			return -1;

		FF_SetBitRange255( pscaler, render_type == DRAW_STRING_BORDERED ? false: true  );
		
		if( render_type == DRAW_STRING_BITMAP ) {
			T2K_RenderGlyph( pscaler, (uint32)wc, 0, 0, BLACK_AND_WHITE_BITMAP, T2K_SCAN_CONVERT | T2K_NAT_GRID_FIT, &errCode );

		} else if( render_type == DRAW_STRING_ALPHA ) {
			T2K_RenderGlyph( pscaler, (uint32)wc, 0, 0, GREY_SCALE_BITMAP_HIGH_QUALITY, T2K_SCAN_CONVERT, &errCode );

		} else if( render_type == DRAW_STRING_BORDERED ) {

			// render the character.
			T2K_RenderGlyph( pscaler, (uint32)wc, 0, 0, 
				GREY_SCALE_BITMAP_HIGH_QUALITY, 
				T2K_SCAN_CONVERT | T2K_TV_MODE, &errCode );
				
			// generate a border image now.
			T2K_CreateBorderedCharacter( pscaler, params );

#if defined(__BIGENDIAN__)
			// This is a workaround to GF's lack of support for BGRA8888 BE, convert to BGRA8888 LE
			// Right or wrong, the rowBytes is already multiplied by sizeof(uint32), no need to divide.
			{	int len = pscaler->rowBytes * pscaler->height;
				uint32 * pi = pscaler->baseARGB;
				int i;

				for(i = 0; i < len; i++) {
					ENDIAN_SWAP32(&pi[i]);
				}
			}
#endif
		}

		if( errCode != 0 )
			return -1;

		if( verbose ) {
			printf( "Metrics for '%x':\n", wc );
			printf( "\tLeft Bearing: %ld\n", pscaler->fLeft26Dot6 >> 6 );
			printf( "\tAdvance: %ld\n", pscaler->xAdvanceWidth16Dot16 >> 16 );
		}

		/* Account for symbols with negative left bearings, so we don't clip them. */
		if( first ) {
			first = 0;
			if( pscaler->fLeft26Dot6 < 0 )
				pos_x += -pscaler->fLeft26Dot6 << 10;
			mod = 0;
		} else {
			mod = pscaler->fLeft26Dot6 < 0 ? (pscaler->fLeft26Dot6 << 10): 0;
		}

		if( pscaler->baseARGB )
			img = (uint8_t*)pscaler->baseARGB;
		else
			img = (uint8_t*)pscaler->baseAddr;

		if( NULL == img ) {
			if( verbose )
				printf( "failed to load image for '%x'\n", wc );

		} else {
			gf_point_t      pos;
			int             bpl = pscaler->rowBytes;

			pos.x = (( pos_x + mod + 0x8000 ) >> 16) + xoffset;
			pos.y = ((pos_y + 0x8000) >> 16) - (pscaler->fTop26Dot6>>6) + yoffset;
			
			if( gf_draw_begin( context ) != -1 ) {
				gf_context_set_fgcolor( context, 0x000000 );

				if( render_type == DRAW_STRING_BITMAP ) {
					gf_draw_bitmap( context, img, bpl, 0, pos.x, pos.y, pscaler->width, pscaler->height, 0 );

				} else if( render_type == DRAW_STRING_ALPHA ) {
					gf_alpha_t      alpha;

					if( gf_surface_reattach( t_surface, pscaler->width, pscaler->height,
											 pscaler->width, NULL, img, 0 ) == GF_ERR_OK ) {
						alpha.mode = GF_ALPHA_M1_MAP | GF_BLEND_DST_1mM1 | GF_BLEND_SRC_M1;
						alpha.map = t_surface;
						alpha.map_x_offset = 0;
						alpha.map_y_offset = 0;
						gf_context_set_alpha( context, &alpha );

						gf_draw_rect( context, pos.x, pos.y, pos.x + pscaler->width - 1, pos.y + pscaler->height - 1 );
					}

				} else if( render_type == DRAW_STRING_BORDERED ) {
					
					if( GF_ERR_OK == gf_surface_reattach( t_surface,  pscaler->width, 
									pscaler->height, bpl*4, NULL, img, 0 )
					) {
						gf_alpha_t      alpha = { 0 };
						
						alpha.mode = GF_ALPHA_M1_SRC_PIXEL_ALPHA | GF_BLEND_SRC_M1 | GF_BLEND_DST_1mM1;
						
						gf_context_set_alpha( context, &alpha );

						gf_draw_blit2( context, t_surface, info->surface1,
							   0, 0, pscaler->width - 1, pscaler->height - 1,
							   pos.x, pos.y );
					}
				}
				
				gf_draw_flush( context );
				gf_draw_end( context );
			}
		}
		
		// simple width calculation.
		width += pscaler->xAdvanceWidth16Dot16;
		if( pscaler->fLeft26Dot6 < 0 )
			width += -pscaler->fLeft26Dot6 << 10;		
	
		pos_x += pscaler->xLinearAdvanceWidth16Dot16;
		pos_y -= pscaler->yLinearAdvanceWidth16Dot16;
		
		T2K_PurgeMemory( pscaler, 1, &errCode );

		if( errCode != 0 )
			return -1;
			
		if( rot_angle_increment ) {
			
			rot_angle += rot_angle_increment;
			
			sin_val = PF_SIN_DEGREE(rot_angle);
			cos_val = PF_COS_DEGREE(rot_angle);

			trans.t00 = util_FixMul( size, cos_val );
			trans.t01 = util_FixMul( size, sin_val );
			trans.t10 = util_FixMul( size, -sin_val );
			trans.t11 = util_FixMul( size, cos_val );

			// Do the transformation.
			T2K_NewTransformation( pscaler, true, 96, 96, &trans, true, &errCode );

			if( errCode != 0 )
				return -1;
		}
	}

bail:
	T2K_PurgeMemory( pscaler, 1, &errCode );

	if( errCode != 0 ) 
		return -1;

	return (width + 0x8000) >> 16;	// convert into pixels.
}

/************************************************************** FONT_INIT() ***/
int
font_init(gf_setup_t *info)
{
	tsiMemObject * mem = NULL;
	int errCode = 0, width;
	InputStream * in = NULL;
	sfntClass * font = NULL;
	short nType = -1;
	FILE* fp;
	long length;
	T2K * scaler;
	unsigned char *p; 
	int size = 12*info->display_info.xres/480;
	int	xoff = info->display_info.xres - (280*size/12), yoff = 0;
	struct utsname si;
	struct ifaddrs *ifaddrs;
	char buf[1024];
	char ipaddr[64];
    
	mem = tsi_NewMemhandler(&errCode);
	if (errCode != 0) {
		return -1;
	}

	if((fp = fopen(info->fname, "rb")) == NULL) {
		if(info->fname[0] != '/')
			strcpy(buf, "/usr/photon/font_repository/");
		else
			buf[0] = '\0';
		strncat(buf, info->fname, sizeof(buf)-strlen(buf));
		buf[sizeof(buf)-1] = '\0';
		if((fp = fopen(buf, "rb")) == NULL) {
			fprintf(stderr, "Unable to open font: %s\n", info->fname);
			return -1;
		}
	}

	if (fseek(fp, 0, SEEK_END)) {
		return -1;
	} else {
		length = ftell(fp);
	}

	p = calloc(length, sizeof(char));
	fseek(fp, 0, SEEK_SET);
	fread(p, length, sizeof(char), fp);
	fclose(fp);

	in  = New_InputStream3( mem, p, length, &errCode );
	if (errCode != 0) {
		return -1;
	}

	nType = FF_FontTypeFromStream(in, &errCode);
	if (errCode != 0) {
		mem = NULL;
		in = NULL;
		return -1;
	}

	if (nType == -1) {
		Delete_InputStream(in, &errCode  );
		if (errCode == 0) {
			tsi_DeleteMemhandler(mem);
		}
		mem = NULL;
		in = NULL;
		return -1;
	}

	/* Create an sfntClass object*/
	font = New_sfntClass(mem, nType, in, NULL, &errCode);
	if (errCode != 0) {
		return -1;
	}

	/* Create a T2K font scaler object.  */
	scaler = NewT2K(font->mem, font, &errCode );
	if (errCode != 0) {
		return -1;
	}

	Set_PlatformID(scaler, 3);
	Set_PlatformSpecificID(scaler, TRUETYPE_SPEC_ID);

	info->scaler = scaler;
	
	if(uname(&si) >= 0) {
		int	usize = 10*info->display_info.xres/480;
		char *proc = "unknown";

#if   defined(__ARMEL__)
		proc = "armle";
#elif defined(__ARMEB__)
		proc = "armbe";
#elif defined(__i686__)
		proc = "i686";
#elif defined(__i586__)
		proc = "i586";
#elif defined(__i486__)
		proc = "i486";
#elif defined(__i386__)
		proc = "i386";
#elif defined(__X86__)
		proc = "x86";
#elif defined(__MIPSEL__)
		proc = "mipsel";
#elif defined(__MIPSEB__)
		proc = "mipseb";
#elif defined(__SH4__)
		proc = "sh4";
#elif defined(__PPC__)
		proc = "ppc";
#endif
		sprintf(buf, "%s %s %s %s %s %s", si.sysname, si.nodename, si.release, si.version, si.machine, proc);
		width = draw_string( info, buf, 
				usize, 0, 0, 10, info->display_info.yres-7-(usize+3), DRAW_STRING_BITMAP, NULL );
		if( -1 == width ) {
			fprintf( stderr, "Unable to draw '%s'\n", buf );
			return -1;
		}
	}

	ipaddr[0] = '\0';
	if(getifaddrs(&ifaddrs) >= 0) {
		int i, donemac=0;
		struct ifaddrs *ifa;

		yoff = 25*info->display_info.yres/272;
		for(ifa=ifaddrs; ifa != NULL; ifa=ifa->ifa_next) {
			if(strncmp(ifa->ifa_name, "lo", 2) != 0 && ifa->ifa_addr != NULL) {
				if(inet_ntop(ifa->ifa_addr->sa_family, &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr, buf, sizeof(buf)) != NULL) {
					width = 0;
					width += draw_string( info, ifa->ifa_name, 
							size, 0, 0, xoff+width, yoff, DRAW_STRING_BITMAP, NULL );
					width += draw_string( info, ":  IP: ", 
							size, 0, 0, xoff+width, yoff, DRAW_STRING_BITMAP, NULL );
					width += draw_string( info, buf, 
							size, 0, 0, xoff+width, yoff, DRAW_STRING_BITMAP, NULL );
					yoff += size+3;
					donemac = 0;
					strcpy(ipaddr, buf);
				} else if(ifa->ifa_addr->sa_family == AF_LINK && donemac == 0) {
					buf[0] = '\0';
					for(i=0; i<((struct sockaddr_dl *)ifa->ifa_addr)->sdl_alen; i++)
						sprintf(buf+strlen(buf),"%c%02X", i==0?' ':':', (unsigned char)LLADDR((struct sockaddr_dl *)ifa->ifa_addr)[i]);
					width = 0;
					width += draw_string( info, ifa->ifa_name, 
							size, 0, 0, xoff+width, yoff, DRAW_STRING_BITMAP, NULL );
					width += draw_string( info, ": MAC:", 
							size, 0, 0, xoff+width, yoff, DRAW_STRING_BITMAP, NULL );
					width += draw_string( info, buf, 
							size, 0, 0, xoff+width, yoff, DRAW_STRING_BITMAP, NULL );
					yoff += size+3;
					donemac++;
				}
			}
		}
		freeifaddrs(ifaddrs);
	}

	yoff = 100*info->display_info.yres/272;

	draw_string( info, "Serial Console: 115200n8", 
			size, 0, 0, xoff, yoff, DRAW_STRING_BITMAP, NULL );
	yoff += size+3;
	yoff += (size+3)/2;

	if(ipaddr[0]) {
		width  = draw_string( info, "Network Console: (login root)", 
				size, 0, 0, xoff,       yoff, DRAW_STRING_BITMAP, NULL );
		yoff += size+3;

		width  = draw_string( info, "    $ telnet ", 
				size, 0, 0, xoff,       yoff, DRAW_STRING_BITMAP, NULL );
		width += draw_string( info, ipaddr, 
				size, 0, 0, xoff+width, yoff, DRAW_STRING_BITMAP, NULL );
		yoff += size+3;
		yoff += (size+3)/2;

		width  = draw_string( info, "Momentics IDE: ", 
				size, 0, 0, xoff,       yoff, DRAW_STRING_BITMAP, NULL );
		yoff += size+3;

		width  = draw_string( info, "    ", 
				size, 0, 0, xoff,       yoff, DRAW_STRING_BITMAP, NULL );
		width += draw_string( info, ipaddr, 
				size, 0, 0, xoff+width, yoff, DRAW_STRING_BITMAP, NULL );
		width += draw_string( info, ":8000", 
				size, 0, 0, xoff+width, yoff, DRAW_STRING_BITMAP, NULL );
		yoff += size+3;
		yoff += (size+3)/2;
	}

	return 0;
}


__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/6.4.1/trunk/apps/gf/font-test/font.c $ $Rev: 217583 $" );
