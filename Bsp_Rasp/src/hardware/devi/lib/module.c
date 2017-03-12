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







/* 
 * module.c
 *
 * Module manipulation functions.
 *
 * It is important to note that the devi driver will have one static
 * instance of each module.  If a module needs to go on more than one
 * bus, then a copy needs to be made.
 *
 */
#include <sys/devi.h>


/* module_lookup
 *
 * Search for a module in the global ModuleTable.  If a module
 * is found and is already in use in another event bus line, then
 * allocate and copy it to a new one.  
 *
 *  Returns: a pointer to the module if found
 *           NULL if not found
 */

input_module_t *
module_lookup (char *name)
{
	int i = 0;

	if (verbosity)
		printf ("Looking up %s in module table\n", name);

	for (i = 0; i < MODULE_TABLE_SIZE && ModuleTable[i]; i++) {

		if (strcmp (strlwr (ModuleTable[i]->name), strlwr (name)) == 0) {
			break;
		}
	}
	if (ModuleTable[i]) {
		if (ModuleTable[i]->flags & MODULE_FLAG_INUSE) {
			input_module_t *module;

			if ((module = smalloc (sizeof (*module))) == NULL) {
				return (NULL);
			}

			memcpy (module, ModuleTable[i], sizeof (*module));
			module->down = NULL;
			module->up = NULL;
			module->data = NULL;
			module->flags = 0;
			module->line = NULL;

			if (verbosity >= 8) {
				printf ("Allocated dynamic module %s at %p\n", module->name, module);
			}
			return (module);
		}

		else {

			ModuleTable[i]->flags |= MODULE_FLAG_INUSE;
			if (verbosity >= 8) {
				printf ("Allocated static module %s at %p\n", ModuleTable[i]->name,
								ModuleTable[i]);
			}
			return (ModuleTable[i]);
		}
	}
	return (NULL);
}


/* module_filter_lookup
 *
 * Lookup the filter module base on the type given.  Type can be one of 
 * DEVI_CLASS_KBD, DEVI_CLASS_REL, or DEVI_CLASS_ABS.  
 *
 * Returns: pointer to filter module if found
 *          NULL if not found
 */

input_module_t *
module_filter_lookup (int type)
{
	input_module_t *mp = NULL;
	input_module_t **list = ModuleTable;

	while (*list) {

		if ((*list)->type & type) {

			if ((*list)->flags & MODULE_FLAG_INUSE) {

				if ((mp = smalloc (sizeof (*mp))) == NULL) {
					return (NULL);
				}

				memcpy (mp, *list, sizeof (*mp));
				mp->up = NULL;
				mp->down = NULL;
				mp->data = NULL;
				mp->flags = 0;
				mp->line = NULL;

				if (verbosity >= 8) {
					fprintf (stderr, "Allocated dynamic module %s at %p\n",
									 mp->name, mp);
				}
			} else {
				(*list)->flags |= MODULE_FLAG_INUSE;
				mp = *list;
			}
			if (verbosity >= 8) {
				fprintf (stderr, "Located module %s at %p\n", mp->name, mp);
			}
			break;
		}
		list++;
	}

	return (mp);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/module.c $ $Rev: 657836 $" );
