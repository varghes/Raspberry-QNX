/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008 QNX Software Systems.  
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
 * eventbus.c
 *
 * Functions used to create and manipulate event bus lines.
 *
 */
#include <sys/devi.h>


/* bus_create
 *
 * Create and initialize an event bus line.
 *
 * Returns: pointer to a bus line if ok
 *          NULL on error
 */

event_bus_line_t *
bus_create (void)
{
	event_bus_line_t *p;

	p = malloc (sizeof (event_bus_line_t));

	if (p) {
		p->next = NULL;
		p->bottom = p->top = NULL;
		if (pthread_mutex_init (&p->mutex, NULL) != EOK) {
			errno_print ("pthread_mutex_init");
			free (p);
			return (NULL);
		}

		if (pthread_cond_init (&p->cond, NULL) != EOK) {
			errno_print ("pthread_cond_init");
			free (p);
			return (NULL);
		}
		p->head = 0;
		p->attr = NULL;
	}

	return (p);
}


/* bus_add
 *
 * Add a module to an event bus line.
 *
 * Returns: 1 - Need to create a new bus line (makes it easier to parse the
 *              command line in options.c, believe me :-)
 *	    0 - OK
 *	   -1 - error
 */

int
bus_add (event_bus_line_t * bp, input_module_t * mp)
{
	if (!bp)
		return (1);

	if (!mp)
		return (-1);

	/* This tells us that a new bus line must be created */

	if (bp->flags & DEVI_LINE_HAVE_PROTO && mp->type & DEVI_MODULE_TYPE_PROTO) {

		/* add the filter module to the current bus if needed */
		if (bus_complete (bp) < 0)
			return (-1);
		return (1);
	}

	/* check if we can to insert */

	if (bp->flags & DEVI_LINE_HAVE_DEVICE && mp->type & DEVI_MODULE_TYPE_DEVICE) {
		char *pMsgTxt =
			"Error: insertion of module %s into busline that already has a device module\n";
		fprintf (stderr, pMsgTxt, mp->name);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, mp->name);
		return (-1);
	}

	if (bp->flags & DEVI_LINE_HAVE_FILTER && mp->type & DEVI_MODULE_TYPE_FILTER) {

		char *pMsgTxt =
			"Error: insertion of module %s into busline that already has a filter module\n";
		fprintf (stderr, pMsgTxt, mp->name);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, mp->name);
		return (-1);
	}

	/* insert */

	if (mp->type & DEVI_MODULE_TYPE_DEVICE)
		bp->flags |= DEVI_LINE_HAVE_DEVICE;
	if (mp->type & DEVI_MODULE_TYPE_PROTO)
		bp->flags |= DEVI_LINE_HAVE_PROTO;
	if (mp->type & DEVI_MODULE_TYPE_FILTER)
		bp->flags |= DEVI_LINE_HAVE_FILTER;


	/* protocol modules must be inserted first */

	if (bp->bottom == NULL && bp->top == NULL) {

		if (!(mp->type & DEVI_MODULE_TYPE_PROTO)) {
			char *pMsgTxt = "Error: initial module %s must be a protocol module\n";

			fprintf (stderr, pMsgTxt, mp->name);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, mp->name);
			return (-1);
		} else {
			bp->bottom = bp->top = mp;
			mp->line = bp;
			return (0);
		}
	}

	/* device mods go at the bottom */
	if (mp->type & DEVI_MODULE_TYPE_DEVICE) {
		if (bp->bottom) {
			mp->up = bp->bottom;
			bp->bottom->down = mp;
		}
		bp->bottom = mp;
	}

	/* filter mods go at the top */
	else if (mp->type & DEVI_MODULE_TYPE_FILTER) {
		if (verbosity >= 3)
			printf ("Adding filter module: %s\n", mp->name);
		if (bp->top) {
			mp->down = bp->top;
			bp->top->up = mp;
		}
		bp->top = mp;
	}
	mp->line = bp;
	return (0);
}


/* bus_complete
 *
 * Add bus to our global list of buses, Bus.  Check to see if a bus has a 
 * filter module.  If it doesn't add it to the top of the bus.
 *
 * Returns  0 on success
 *         -1 on error
 */

int
bus_complete (event_bus_line_t * bp)
{
	input_module_t *filter;
	int type = 0;

	if (!(bp->flags & DEVI_LINE_HAVE_DEVICE)) {
		char *pMsgTxt = "Error: device must be specified to protocol %s\n";

		fprintf (stderr, pMsgTxt, bp->bottom->name);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
					 bp->bottom->name);
		return (-1);
	}

	/* insert new bus at front of list of buses */
	if (Bus)
		bp->next = Bus;
	Bus = bp;

	if (bp->top->type & DEVI_CLASS_ABS)
		type = DEVI_CLASS_ABS;
	else if (bp->top->type & DEVI_CLASS_KBD)
		type = DEVI_CLASS_KBD;
	else if (bp->top->type & DEVI_CLASS_REL)
		type = DEVI_CLASS_REL;
	else if (bp->top->type & DEVI_CLASS_JOYSTICK)
		type = DEVI_CLASS_JOYSTICK;
	else if (bp->top->type & DEVI_CLASS_GAMEPAD)
		bp = bp;
	else if (bp->top->type & DEVI_CLASS_CONTROL)
		type = DEVI_CLASS_CONTROL;
	else {
		char *pMsgTxt =
			"Error: unable to determine type of filter to add to bus\n";
		fprintf (stderr, pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		return (-1);
	}
	bp->type = type;

	if (bp->flags & DEVI_LINE_HAVE_FILTER)
		return (0);

	filter = module_filter_lookup (type);

	if (!filter) {
		char *pMsgTxt = "Error: unable to find filter module\n";

		fprintf (stderr, pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		return (-1);
	}

	if (filter->init (filter) < 0) {
		char *pMsgTxt = "Error: unable to init module %s\n";

		fprintf (stderr, pMsgTxt, filter->name);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
					 filter->name);
		return (-1);
	}

	if (bp->top) {
		filter->down = bp->top;
		bp->top->up = filter;
	}
	bp->top = filter;
	filter->line = bp;
	bp->flags |= DEVI_LINE_HAVE_FILTER;

	return (0);
}


/* bus_interrogate
 *
 * Searches through the global list of event buses, Bus and returns 
 * the filter module corresponding to the bus type specified in the 
 * type parameter, ie DEVI_CLASS_*.  
 * 
 * This function is non-reentrant.  State info is kept as to which
 * bus line was processed last.
 *
 * Returns 1 if module is found
 *         0 if not
 *
 * Note the filter module is passed back via the second parameter
 * to the function, mpp.
 */

int
bus_interrogate (int type, input_module_t ** mpp)
{
	int found = 0;
	static int last_type;
	static event_bus_line_t *bp;

	*mpp = NULL;

	if (type) {
		last_type = type;
		bp = Bus;
	}

	else {
		if (bp == NULL)
			return (0);
		type = last_type;
	}

	do {
		if (bp->type & type) {
			found++;
			*mpp = bp->top;
			bp = bp->next;
			break;
		}
	} while ((bp = bp->next));

	if (*mpp && verbosity)
		printf ("interrogate bus, returning %s\n", (*mpp)->name);

	return (found);
}

/* bus_reset
 *
 * Iterate through the global list of buses, Bus, and call the reset()
 * callback of each module in the bus starting from the device module
 * up to the filter module.  If we're configured to run the resource
 * manager interface, start it up for each bus line, ie create the
 * device.
 *
 * Returns:  0 on success
 *          -1 on error
 */

int
bus_reset (void)
{
	event_bus_line_t *bp;
	input_module_t *mp;

	for (bp = Bus; bp; bp = bp->next) {
		for (mp = bp->bottom; mp; mp = mp->up) {
			if (verbosity)
				printf ("Resetting module %s\n", mp->name);
			if (!mp->reset || mp->reset (mp) < 0) {
				char *pMsgTxt = "Error: unable to reset module %s\n";

				fprintf (stderr, pMsgTxt, mp->name);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
							 mp->name);
				return (-1);
			}
		}

		/* initialize resmgr i/f for this line if needed */
		if (OptFlags & OPT_RESMGR) {

			/* create our resmgr device queue */

			switch (bp->type & DEVI_CLASS_MASK) {

			case DEVI_CLASS_REL:

				bp->elem_size = sizeof (struct _mouse_packet);
				bp->u.mq = malloc (bp->elem_size * RESMGR_Q_SIZE);
				if (bp->u.mq == NULL) {
					char *pMsgTxt = "Error: memory allocation\n";

					fprintf (stderr, pMsgTxt);
					slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
					return (-1);
				}
				break;

			case DEVI_CLASS_KBD:

				bp->type = DEVI_CLASS_KBD;
				bp->elem_size = sizeof (struct _keyboard_packet);
				bp->u.kq = malloc (bp->elem_size * RESMGR_Q_SIZE);
				if (bp->u.kq == NULL) {
					char *pMsgTxt = "Error: memory allocation fault\n";

					fprintf (stderr, pMsgTxt);
					slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
					return (-1);
				}
				break;

			case DEVI_CLASS_ABS:

				bp->type = DEVI_CLASS_ABS;
				bp->elem_size = sizeof (struct _touch1_packet);
				bp->u.tq = malloc (bp->elem_size * RESMGR_Q_SIZE);
				if (bp->u.tq == NULL) {
					char *pMsgTxt = "Error: memory allocation fault\n";

					fprintf (stderr, pMsgTxt);
					slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
					return (-1);
				}

				break;

			case DEVI_CLASS_CONTROL:
				bp->type = DEVI_CLASS_CONTROL;
				bp->elem_size = sizeof (struct _control_packet);
				bp->u.cq = malloc (bp->elem_size * RESMGR_Q_SIZE);

				if (bp->u.cq == NULL) {
					char *pMsgTxt = "Error: memory allocation fault\n";

					fprintf (stderr, pMsgTxt);
					slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
					return (-1);
				}

				break;

			case DEVI_CLASS_JOYSTICK:
				bp->type = DEVI_CLASS_JOYSTICK;
				bp->elem_size = sizeof (struct _joystick_packet);
				bp->u.jq = malloc (bp->elem_size * RESMGR_Q_SIZE);

				if (bp->u.jq == NULL) {
					char *pMsgTxt = "Error: memory allocation fault\n";

					fprintf (stderr, pMsgTxt);
					slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
					return (-1);
				}

				break;

			}

			if (resmgr_create_device (bp) < 0) {
				char *pMsgTxt = "Error: unable to initialize resource manager\n";

				fprintf (stderr, pMsgTxt);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
				return (-1);
			}
		}
	}

	return (0);
}


/* bus_dump
 *
 * Used for debugging, prints out a bus line
 *
 */

void
bus_dump (void)
{
	input_module_t *mp;
	event_bus_line_t *bp;

	for (bp = Bus; bp; bp = bp->next) {

		if (bp->type & DEVI_CLASS_KBD)
			printf ("Class: kbd\n");
		else if (bp->type & DEVI_CLASS_REL)
			printf ("Class: rel\n");
		else if (bp->type & DEVI_CLASS_ABS)
			printf ("Class: abs\n");
		else if (bp->type & DEVI_CLASS_CONTROL)
			printf ("Class: ctrl\n");
		else if (bp->type & DEVI_CLASS_JOYSTICK)
			printf ("CLass: joy\n");

		printf ("Filter -> Device\n");
		for (mp = bp->top; mp; mp = mp->down)
			printf ("%s ", mp->name);
		printf ("\n");
		printf ("Device -> Filter\n");
		for (mp = bp->bottom; mp; mp = mp->up)
			printf ("%s ", mp->name);
		printf ("\n");
	}
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/eventbus.c $ $Rev: 657836 $" );
