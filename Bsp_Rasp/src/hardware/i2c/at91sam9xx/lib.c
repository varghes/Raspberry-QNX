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
i2c_master_getfuncs(i2c_master_funcs_t *funcs, int tabsize)
{
	I2C_ADD_FUNC(i2c_master_funcs_t, funcs, 
			init, at91sam9xx_init, tabsize);
	I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
			fini, at91sam9xx_fini, tabsize);
	I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
			send, at91sam9xx_send, tabsize);
	I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
			recv, at91sam9xx_recv, tabsize);
	I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
			set_slave_addr, at91sam9xx_set_slave_addr, tabsize);
	I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
			set_bus_speed, at91sam9xx_set_bus_speed, tabsize);
	I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
			version_info, at91sam9xx_version_info, tabsize);
	I2C_ADD_FUNC(i2c_master_funcs_t, funcs,
			driver_info, at91sam9xx_driver_info, tabsize);
	return 0;
}
