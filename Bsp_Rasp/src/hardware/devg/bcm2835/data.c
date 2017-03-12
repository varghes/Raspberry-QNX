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
/*
 * Copyright 2016, Varghese Alphonse.
*/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bcm2835.h"
//#include "at91sam9xx.h"

static char *at91sam9xx_opts[] = {
#define X_VALUE		0
			"x",
#define Y_VALUE		1
			"y",
#define BOARD_MCK_VALUE	2
			"board_mck",
#define VIDBASE_VALUE	3
			"vidbase",
#define REGBASE_VALUE	4
			"regbase",
#define DBL_PIXEL_VALUE 5 
			"dbl_pixelclock",
			NULL
};

int
get_config_data(at91sam9xx_context_t *at91sam9xx, const char *filename)
{
	FILE	*fin = NULL;
	char	buf[256], *c, *opt, *value;
	if (filename == NULL){
		disp_printf(at91sam9xx->adapter,
		    "No config file specified\n");
		    
	}
	else if ((fin = fopen(filename, "r")) == NULL){
		disp_printf(at91sam9xx->adapter,
		    "Could not open config file \"%s\": %s\n",
		    filename, strerror(errno));
		    
	}

	/* Setting default values */
	/* lcd base */
	at91sam9xx->dcbase = DC_REGBASE;
	/* dimension */
	at91sam9xx->width = LCD_DFT_WIDTH;
	at91sam9xx->height = LCD_DFT_HEIGHT;
	if (fin == NULL) {
		return 0;
	}

	while (fgets(buf, sizeof (buf), fin) != NULL) {
		c = buf;
		while (*c == ' ' || *c == '\t')
			c++;
		if (*c == '\015' || *c== '\032' || *c == '\0' || *c == '\n' || *c == '#')
			continue;
		opt = c;
		while (*c == '\015' || *c== '\032' ||( *c != '\0' && *c != '\n' && *c != '#'))
			c++;
		*c = '\0';
		break;
	}


	while (*opt != '\0') {
		c = opt;
		switch (getsubopt(&opt, at91sam9xx_opts, &value)) {
                case X_VALUE:
                        at91sam9xx->width = strtol(value, NULL, 0);
			break;
                case Y_VALUE:
                        at91sam9xx->height = strtol(value, NULL, 0);
			break;
                case BOARD_MCK_VALUE:
                        at91sam9xx->board_mck = strtol(value, NULL, 0);
			break;
                case VIDBASE_VALUE:
                        at91sam9xx->vidbase = strtol(value, NULL, 0);
			break;
                case REGBASE_VALUE:
                        at91sam9xx->dcbase = strtol(value, NULL, 0);
			break;
                case DBL_PIXEL_VALUE:
                        at91sam9xx->dbl_pixelclock = strtol(value, NULL, 0);
			break;
		default:
			disp_printf(at91sam9xx->adapter, "Unknown option %s\n", c);
			break;
		}
	}
	 at91sam9xx->vidbase=0x5EA83000;
	 at91sam9xx->width=LCD_DFT_WIDTH;
	at91sam9xx->height=LCD_DFT_HEIGHT;
	fclose(fin);

	return 0;
}

