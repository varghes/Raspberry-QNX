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

//#include "at91sam9xx.h"
#include "bcm2835.h"
#include <malloc.h>



/*
 * Populate the miscellaneuous graphics driver function table.  
 * tabsize is the size of the function table in bytes.
 */
int
devg_get_miscfuncs(disp_adapter_t *adapter,
    disp_draw_miscfuncs_t *funcs, int tabsize)
{
	DISP_ADD_FUNC(disp_draw_miscfuncs_t, funcs,
	    init, bcm2835_draw_init, tabsize);
	DISP_ADD_FUNC(disp_draw_miscfuncs_t, funcs,
	    fini, bcm2835_draw_fini, tabsize);
	DISP_ADD_FUNC(disp_draw_miscfuncs_t, funcs,
	    module_info, bcm2835_module_info, tabsize);

        DISP_ADD_FUNC(disp_draw_miscfuncs_t, funcs,
            get_corefuncs_sw, ffb_get_corefuncs, tabsize);
        DISP_ADD_FUNC(disp_draw_miscfuncs_t, funcs,
            get_contextfuncs_sw, ffb_get_contextfuncs, tabsize);
         DISP_ADD_FUNC(disp_draw_miscfuncs_t, funcs,
            attach_external, bcm2835_attach_external, tabsize);
        DISP_ADD_FUNC(disp_draw_miscfuncs_t, funcs,
            detach_external, bcm2835_detach_external, tabsize);
        DISP_ADD_FUNC(disp_draw_miscfuncs_t, funcs,
            recover, bcm2835_recover, tabsize);
        DISP_ADD_FUNC(disp_draw_miscfuncs_t, funcs,
            wait_idle, bcm2835_misc_wait_idle, tabsize);

	return 0;
	
}

void
bcm2835_module_info(disp_adapter_t *adapter, disp_module_info_t *info)
{	
		info->description = "AT91SAM9XX Processor";
        info->ddk_version_major = DDK_VERSION_MAJOR;
        info->ddk_version_minor = DDK_VERSION_MINOR;
        info->ddk_rev = DDK_REVISION;
        info->driver_rev = 0;
}

int
bcm2835_draw_init(disp_adapter_t *adapter, char *opt)
{

	return 0;
}

void
bcm2835_draw_fini(disp_adapter_t *adapter)
{

}

/*
 * Set up things so that miscfuncs, corefuncs and contextfuncs
 * can be called by an external process.
 */
int
bcm2835_attach_external(disp_adapter_t *adapter, disp_aperture_t aper[])
{
    at91sam9xx_context_t		*at91sam9xx = adapter->shmem;
	adapter->ms_ctx = at91sam9xx;
	return 0;

}

int
bcm2835_detach_external(disp_adapter_t *adapter)
{
        return 0;
}

/*
 * Called when a client process terminated unexpectedly.  This could have
 * happened in the middle of adding draw commands to the ring buffer.
 * Reset the ring buffer so any partial draw commands are not sent to
 * the Radeon.
 */
int
bcm2835_recover(disp_adapter_t *adapter)
{
        return 0;
}	
int
bcm2835_misc_wait_idle(disp_adapter_t *adapter)
{
	at91sam9xx_context_t             *at91sam9xx = adapter->shmem;

       	return 0;
}

