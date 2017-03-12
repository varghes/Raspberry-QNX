/*
 * $QNXLicenseC: 
 * Copyright 2008, 2009, QNX Software Systems.  
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


#include "proto.h"

int
at91sam9xx_options(at91sam9xx_dev_t *dev, int argc, char *argv[])
{
	int	c;
	int	prev_optind;
	int	done = 0;

	/* Initialization */
	dev->intr = -1;
	dev->iid = -1;
	dev->physbase = 0;
	dev->reglen = AT91SAM9XX_I2C_REGLEN;
	dev->restart = 0;
	dev->slave_addr = 0;
	dev->clk = -1;
	dev->options = 0;

	while (!done) {
		prev_optind = optind;
		c = getopt(argc, argv, "i:p:s:c:v");
		switch (c) {
			case 'p':
				dev->physbase = strtoul(optarg, &optarg, NULL);
				break;
			case 'i':
				dev->intr = strtol(optarg, &optarg, NULL);
				break;
			case 's':
				dev->slave_addr = strtoul(optarg, &optarg, NULL);
				break;
			case 'c':
				dev->clk = strtoul(optarg, &optarg, NULL);
				break;
			case 'v':
				dev->options |= AT91SAM9XX_OPT_VERBOSE;
				break; 
			case '?':
				if (optopt == '-') {
					++optind;
					break;
				}
				return -1;

			case -1:
				if (prev_optind < optind) /* -- */
					return -1;

				if (argv[optind] == NULL) {
					done = 1;
					break;
				} 
				if (*argv[optind] != '-') {
					++optind;
					break;
				}
				return -1;

			case ':':
			default:
				return -1;
		}
	}

	if (dev->physbase == 0)
	{
		printf("The base address of I2C controller must be specified\n");
		return -1;
	}
	
	if (dev->intr == -1)
	{
		printf("The IRQ number of I2C controller must be specified\n");
		return -1;
	}
	
	if (dev->clk == -1)
	{
		printf("The clock of I2C controller must be specified\n");
		return -1;
	}
		
	return 0;
}
