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
 * buffer.c
 *
 * Buffer manipulation functions.
 *
 */
#include <sys/devi.h>

#define	MAX_BUFF_BYTES	512


buffer_t *
buff_create (unsigned size, unsigned rsize)
{
	buffer_t *cbptr;

	if ((cbptr = smalloc (sizeof *cbptr))) {

		cbptr->bufsize = size ? size : MAX_BUFF_BYTES - (MAX_BUFF_BYTES % rsize);
		cbptr->cnt = 0;

		if ((cbptr->head = cbptr->tail = cbptr->buff = malloc (cbptr->bufsize))) {

			pthread_mutex_init (&cbptr->mutex, NULL);

		} else {
			free (cbptr);
			cbptr = NULL;
		}
	}

	return (cbptr);
}


int
buff_append (buffer_t * bptr, char *dptr, int n)
{
	int t;

	if (n && (bptr->cnt < bptr->bufsize)) {

		while (n) {

			t = min (n, (&bptr->buff[bptr->bufsize] - bptr->head));
			if (t) {

				if (dptr) {
					memcpy (bptr->head, dptr, t);
					dptr += t;
				}

				bptr->cnt += t;
				bptr->head += t;

				if (bptr->head >= &bptr->buff[bptr->bufsize]) {
					bptr->head = bptr->buff;
				}
				n -= t;
			}
		}
	}

	return (bptr->cnt);
}


int
buff_delete (buffer_t * bptr, char *dptr, int n)
{
	int t, v = 0;

	pthread_mutex_lock (&bptr->mutex);

	if (bptr->cnt) {

		while (n) {

			t = min (n, bptr->cnt);

			if (t > 0) {
				if (dptr) {
					memcpy (dptr, bptr->tail, t);
					dptr += t;
				}

				bptr->tail += t;
				bptr->cnt -= t;
				if (bptr->tail >= &bptr->buff[bptr->bufsize]) {
					bptr->tail = bptr->buff;
				}

				n -= t;
				v += t;
			} else {
				n = 0;
			}
		}
	}

	pthread_mutex_unlock (&bptr->mutex);

	return (v);
}


int
buff_flush (buffer_t * bptr)
{
	bptr->tail = bptr->head = bptr->buff;
	bptr->cnt = 0;

	return (EOK);
}


int
buff_putc (buffer_t * bptr, char c)
{

	static intrspin_t spin;

	InterruptLock (&spin);

	if (bptr->cnt < bptr->bufsize) {
		bptr->cnt++;
		*bptr->head++ = c;
		if (bptr->head >= &bptr->buff[bptr->bufsize]) {
			bptr->head = bptr->buff;
		}
	}

	InterruptUnlock (&spin);

	return (bptr->cnt);
}


int
buff_getc (buffer_t * bptr, char *c)
{

	static intrspin_t spin;

	InterruptLock (&spin);

	if (bptr->cnt == 0) {

		InterruptUnlock (&spin);

		return (-1);
	}

	*c = *bptr->tail;
	--bptr->cnt;

	if (++bptr->tail >= &bptr->buff[bptr->bufsize]) {
		bptr->tail = bptr->buff;
	}

	InterruptUnlock (&spin);

	return 0;
}


int
buff_waiting (buffer_t * bptr)
{
	return (bptr->cnt);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/buffer.c $ $Rev: 657836 $" );
