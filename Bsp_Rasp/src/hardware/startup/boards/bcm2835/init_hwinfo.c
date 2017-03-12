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





#include "startup.h"
#include <arm/bcm2835.h>

typedef struct _board_info {
        uint32_t cpu_id;
        char board_name[15];
} board_info;

static board_info board_list[] = {    // file this list when
                        {1, "bcm2835"}, //not 1,get chip Id
                        {0x0, "UNKNOWN"}
};
/*static void add_one_emac(unsigned off, unsigned int intr_rx, unsigned char *mac)
{
   ;
}

static void add_one_dmac(unsigned off,unsigned int intr_no)
{
  ;
}*/

/* 
 * get board name for the corresponding cpu id 
 */

char * get_board_name()
{
	int i;
	uint32_t cpu_id;

	//cpu_id = in32(register address to identify bcm2835/bcm2836/7);
	
	for(i=0 ; i < NUM_ELTS(board_list) - 1; i++)	
		if(cpu_id == board_list[i].cpu_id)
			return board_list[i].board_name;

	return  board_list[NUM_ELTS(board_list) - 1].board_name;

}


/*
 * Initialize hwinfo structure in the system page.
 * This code is hardware dependant and may have to be changed
 * changed by end users.
 */

void
init_hwinfo()
{
	unsigned char mac[6];
	mac[0] = 0x66;
	mac[1] = 0x22;
	mac[2] = 0x00;
	mac[3] = 0x02;
	mac[4] = 0x16;
	mac[5] = 0x15;

	//add_one_emac(store registers, mac);
}

