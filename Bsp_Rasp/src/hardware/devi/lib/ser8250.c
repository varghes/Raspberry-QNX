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
 * ser8250.c
 *
 * Hardware device module for interfacing with a 8250 type
 * serial controller.  Normally the fd module is used in
 * conjunction with the system serial driver to talk to a 
 * serial device.  However if memory contraints apply, it
 * might be easier to eliminate the serial driver and use
 * this module.
 *
 */

#include <sys/devi.h>
#include <assert.h>

#define	FLAG_INIT			0x0100
#define FLAG_IDLE			0x0200
#define FLAG_TXINT			0x0400
#define	TRUE				1
#define	FALSE				!TRUE
#define UART_RX                         1
#define UART_TX                         2

#define MAX_EVENTS                      (20)	// Size of event pool

#define PROMOTE_PRTY                    (5)

#define NO_PULSE_ASSIGNED              (-1)
#define PROCESS_PRTY                   (15)

const struct sigevent *uart_irqhandler (void *arg, int id);
static int uart_init (input_module_t * module);
static int uart_devctrl (input_module_t * module, int event, void *ptr);
static int uart_pulse (message_context_t * ctx, int code, unsigned flags,
											 void *data);
static int uart_reset (input_module_t * module);
static int uart_output (input_module_t * module, void *p, int num);
static int uart_parm (input_module_t * module, int opt, char *optarg);
static int uart_shutdown (input_module_t * module, int ms);


input_module_t uart = {

	NULL,
	NULL,
	NULL,
	0,
	DEVI_CLASS_NONE | DEVI_MODULE_TYPE_DEVICE,
	"Uart",
	__DATE__,
	"12P:p:i:",
	NULL,													/* initialized by us (in init) */
	uart_init,
	uart_reset,
	NULL,													/* input not used (EVBL_TERMINATE) */
	uart_output,
	uart_pulse,
	uart_parm,
	uart_devctrl,
	uart_shutdown
};

struct private_data
{

	unsigned flags;
	struct sigevent ev[MAX_EVENTS];	/* Event pool                                   */
	int priority;									/* Input events processing priority             */
	unsigned port;
	int irq;
	unsigned baud;
	tcflag_t cflag;
	int count;
	int pc;
	int trigger_level;
	int needs_kick;
	int iid;
	buffer_t *ibuff;
	buffer_t *obuff;
};

// We use this structure to transfer data from interrupt handler to pulse processor 
typedef struct _in_msg
{
	_uint8 nBlkNum;								// Number of used event block
	_uint8 nCode;									// Code to send
}
in_msg, *pIn_msg;

/* Description: Service function; checks if 8250 controller exists                 */
/* Input      : struct private_data *dp - pointer to common driver dat block       */
/* Output     : None                                                               */
/* Return     : EOK if OK, otherwise -1                                            */
int
is_8250 (struct private_data *dp)
{
	unsigned port = dp->port;

	/* Use Line control latch to determine if controller card is present */

	InterruptMask (dp->irq, dp->iid);
	out8 (port + 3, 0x05);

	if (in8 (port + 3) == 0x05) {

		out8 (port + 3, 0x0a);

		if (in8 (port + 3) == 0x0a) {

			/* Found one. Clear any pending interrupts. */
			out8 (port + 1, 0x00);		/* Disable all interrupts */
			in8 (port + 5);						/* Clear Line Status Interrupt */
			in8 (port + 0);						/* Clear RX Interrupt */
			in8 (port + 2);						/* Clear TX Interrupt */
			in8 (port + 6);						/* Clear Modem Interrupt */
			InterruptUnmask (dp->irq, dp->iid);
			return (-1);
		}
	}

	InterruptUnmask (dp->irq, dp->iid);
	return (EOK);
}


/* Description: Service function; setups 8250 controller                            */
/* Input      : struct private_data *dp - pointer to common driver dat block        */
/*              unsigned baud - baud rate                                           */
/*              tcflag_t cflag - input mods                                         */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
set_8250 (struct private_data *dp, unsigned baud, tcflag_t cflag)
{
	unsigned port = dp->port;

	InterruptMask (dp->irq, dp->iid);

	out8 (port + 3, in8 (port + 3) | 0x80);	/* Program divisor latch */
	out8 (port + 0, 11520 / (baud / 10));	/* Baud hi */
	out8 (port + 1, (11520 / (baud / 10)) >> 8);	/* Baud lo */
	out8 (port + 3, ((cflag & CSIZE) >> 4) |
				((cflag & CSTOPB) >> 4) |
				((cflag & PARENB) >> 5) | (((cflag & PARODD) ^ PARODD) >> 5));

	InterruptUnmask (dp->irq, dp->iid);
}


/* Description: 8250 controller interrupt handler                                   */
/* Input      : void * arg  - pointer to private data block (see InterruptAttach)   */
/*              int id - intrrupt ID (not used here)                                */
/* Output     : None                                                                */
/* Return     : pulse event or NULL                                                 */
const struct sigevent *
uart_irqhandler (void *arg, int id)
{
	struct private_data *dp = (struct private_data *) arg;
	struct sigevent *pEvent = NULL;
	unsigned char iir;
	int nBlkNum;
	pIn_msg pMsg = NULL;
	char c;


	// Find unused event block  
	for (nBlkNum = 0; nBlkNum < MAX_EVENTS; ++nBlkNum) {
		if (NO_PULSE_ASSIGNED == dp->ev[nBlkNum].sigev_code) {	/* Free block */
			// Prepare it for sending pulse
			pEvent = dp->ev + nBlkNum;
			pMsg = (pIn_msg) & (pEvent->sigev_value.sival_int);
			pMsg->nBlkNum = nBlkNum;
			pMsg->nCode = 0;
			break;
		}
	}



	while (((iir = in8 (dp->port + 2)) & 0x01) == 0) {

		switch (iir) {

		case 0x06:									/* Highest: Receiver Line Status */
			in8 (dp->port + 5);
			break;

		case 0x04:									/* Second:  Received Data Available */
			buff_putc (dp->ibuff, in8 (dp->port + 0));
			if (dp->flags & FLAG_IDLE) {
				if (dp->count) {
					dp->count--;
				} else {
					if (NULL != pMsg)
						pMsg->nCode = UART_RX;
					dp->flags &= ~FLAG_IDLE;
				}
			}
			break;

		case 0x02:									/* Third: Transmitter Holding Register Empty */

			if (0 == buff_getc ((buffer_t *)(dp->obuff), (char *)(&c)))
				out8 (dp->port, c);
			else {
				if (NULL != pMsg)
					pMsg->nCode = UART_TX;
				dp->needs_kick = TRUE;
			}
			break;


		case 0x00:									/* Forth:   Modem Status */
			in8 (dp->port + 6);

			break;
		}
	}

	if (NULL != pMsg) {
		if (pMsg->nCode) {
			pEvent->sigev_priority = dp->priority;
			pEvent->sigev_code = dp->pc;
		} else
			pEvent = NULL;
	}

	return pEvent;
}


/* Description: service function: resets 8250 controller                            */
/* Input      : struct private_data *dp - pointer to common driver dat block        */
/*              unsigned msec - delay                                               */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
reset_8250 (struct private_data *dp, unsigned msec)
{
	unsigned port = dp->port;

	InterruptMask (dp->irq, dp->iid);
	out8 (port + 4, (in8 (port + 4) & 0xe4) | 0x08);	/* RTS DTR down */
	InterruptUnmask (dp->irq, dp->iid);
	delay (msec);
	InterruptMask (dp->irq, dp->iid);
	out8 (port + 4, (in8 (port + 4) & 0xe4) | 0x0b);	/* RTS DTR up */
	InterruptUnmask (dp->irq, dp->iid);
}


/* Description: service function: enables 8250 controller                           */
/* Input      : struct private_data *dp - pointer to common driver dat block        */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
enable_8250 (struct private_data *dp)
{
	unsigned port = dp->port;

	InterruptMask (dp->irq, dp->iid);

	/* RTS DTR up */
	out8 (port + 4, (in8 (port + 4) & 0xe4) | 0x0b);

	/* Enable rx & tx */
	out8 (port + 1, 0x3);

	InterruptUnmask (dp->irq, dp->iid);
}


/* Description: callback function: initializes driver                               */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : None                                                                */
/* Return     : EOK if OK, othervise -1                                             */
int
uart_init (input_module_t * module)
{
	struct private_data *dp = module->data;

	if (!module->data) {

		if (!(dp = module->data = calloc (sizeof *dp, 1))) {
			return -1;
		}
		dp->needs_kick = TRUE;
		dp->port = 0x3f8;
		dp->irq = 4;
		dp->trigger_level = 1;
		dp->priority =
			min ((getprio (0) + PROMOTE_PRTY),
					 sched_get_priority_max (sched_getscheduler (0)));
	}

	return EOK;
}


/* Description: this is a callback function for DEVCTRL command processing          */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int event  - DEVCTRL command code                                   */
/*              void * ptr - pointer to data exchange block                         */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise -1                                               */
int
uart_devctrl (input_module_t * module, int event, void *ptr)
{
	struct private_data *dp = module->data;

	switch (event) {

	case DEVCTL_RESET:

		if (dp->flags & FLAG_INIT) {
			int *delayp = ptr;
			int delay = 1;

			if (delayp == NULL) {
				delay = 1;
			} else {
				delay = (delayp) ? *delayp : 1;
			}

			reset_8250 (dp, delay);
		}
		break;

	case DEVCTL_SETBAUD:

		if (dp->flags & FLAG_INIT) {
			struct devctl_setbaud *devctl = ptr;

			if (devctl) {
				dp->baud = devctl->baud;
				dp->cflag = devctl->cflag & DEVCTL_CFLAG_MASK;
				set_8250 (dp, dp->baud, dp->cflag);
			}
		}
		break;

	case DEVCTL_SETTRIG:{

			struct devctl_settrig *trig = ptr;

			if (trig) {
				dp->flags |= FLAG_IDLE;
				dp->trigger_level = trig->trigger_level;
				dp->count = dp->trigger_level - 1;
			}
			break;
		}
	default:
		return (-1);
	}

	return EOK;
}


/* Description: this is a callback that kernel activates asychroniously each time   */
/*              when keyboard or mouse interrupt occures                            */
/* Input      : message_context_t *ctp - pointer to module descriptor               */
/*              int code - program doesn't use this parameter                       */
/*              unsigned flags - program doesn't use this parameter                 */
/*              void *data - pointer to (input_module_t	mouse) structure               */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise -1                                               */
int
uart_pulse (message_context_t * ctx, int code, unsigned flags, void *data)
{

	input_module_t *module = (input_module_t *) data;
	struct private_data *dp = module->data;
	input_module_t *up = module->up;
	int count;
	_uint8 c = 0;
	int rc = 0;
	char buffer[16];
	pIn_msg pMsg = (pIn_msg) & (ctx->msg->pulse.value.sival_int);


	assert (pMsg->nBlkNum < MAX_EVENTS);

	if (pMsg->nCode == UART_RX) {

		do {

			for (count = 0; count < sizeof buffer; count++) {
				if ((rc = buff_getc ((buffer_t *)(dp->ibuff), (char *)(&c))) != 0) {
					break;
				}
				buffer[count] = c;
			}

			if (count) {
				(up->input) (up, count, (void *) buffer);
			}

			if (rc == -1) {
				InterruptMask (dp->irq, dp->iid);
				if (buff_waiting (dp->ibuff)) {
					rc = 0;

				} else {

					dp->flags |= FLAG_IDLE;
					dp->count = dp->trigger_level - 1;
					if (dp->count < 0) {
						dp->count = 0;
					}
				}

				InterruptUnmask (dp->irq, dp->iid);
			}
		} while (rc == 0);

	} else {

		if (verbosity >= 4) {
			fprintf (stderr, "Transmit buffer empty\n");
		}
		dp->flags &= ~FLAG_TXINT;
	}


	dp->ev[pMsg->nBlkNum].sigev_code = NO_PULSE_ASSIGNED;
	return EOK;
}


/* Description: this callback funtion is called when the module is linked into the  */
/*              event bus;it is used to set initial module state on the device level*/
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise -1                                             */
int
uart_reset (input_module_t * module)
{
	struct private_data *dp = module->data;
	int i;

	if ((dp->flags & FLAG_INIT) == 0) {

		if (!is_8250 (dp)) {
			return (-1);
		}

		if ((dp->ibuff = buff_create (128, 0)) == NULL) {
			perror ("buff_create failed");
			return (-1);
		}

		if ((dp->obuff = buff_create (32, 0)) == NULL) {
			perror ("buff_create failed");
			return (-1);
		}

		/* 
		 * Attach to our irq.  Note in this case, we will be
		 * using an actual interrupt handler instead of going
		 * straight to the pulse handler.  
		 */

		dp->pc = devi_register_pulse (module, DEVI_PULSE_ALLOC, uart_pulse);
		if (dp->pc < 0) {
			char *pMsgTxt = "Error: %s - unable to register pulse callback\n";

			fprintf (stderr, pMsgTxt, module->name);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
						 module->name);
			return (-1);
		}

		memset (&dp->ev, 0, sizeof (dp->ev));
		for (i = 0; i < MAX_EVENTS; ++i) {
			SIGEV_PULSE_INIT (&(dp->ev[i]), devi_get_coid (), dp->priority,
												NO_PULSE_ASSIGNED, 0);
		}


		if ((dp->iid = InterruptAttach (dp->irq, uart_irqhandler, dp,
																		sizeof (*dp), _NTO_INTR_FLAGS_TRK_MSK |
																		_NTO_INTR_FLAGS_PROCESS)) < 0) {
			perror ("InterruptAttach");
			return (-1);
		}

		enable_8250 (dp);
		delay (250);

		buff_delete (dp->ibuff, NULL, 200);

		dp->flags |= (FLAG_INIT | FLAG_IDLE);
		dp->needs_kick = TRUE;

		if (verbosity) {
			printf ("Using UART at ioport %X irq %d\n", dp->port, dp->irq);
		}
	}
	return EOK;
}


/* Description: callback function: transmits data to device                         */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              void * p - pointer to data block                                    */
/*              int num - number of bytes to transmit                               */
/* Output     : None                                                                */
/* Return     : always EOK                                                          */
int
uart_output (input_module_t * module, void *p, int num)
{
	struct private_data *dp = module->data;
	char kick = 0;
	int i = 0;
	char *ptr = (char *) p;


	if (dp->needs_kick) {
		kick = *ptr++;
		num--;
	}

	while (num) {

		buff_putc (dp->obuff, *(ptr + i));
		num--;
		i++;
	}

	if (dp->needs_kick) {

		out8 (dp->port, kick);
		dp->needs_kick = FALSE;
	}
	return EOK;
}


/* Description: this is a callback function for command line parameter processing   */
/*              (all valid parameters for device module are listed in uart.args)    */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int opt  - parameter code                                           */
/*              char * optarg - optional parameter value                            */
/* Output     : None                                                                */
/* Return     : 0 if OK, otherwise -1                                               */
int
uart_parm (input_module_t * module, int opt, char *optarg)
{
	struct private_data *dp = module->data;

	switch (opt) {

	case '1':
		dp->port = 0x3f8;
		dp->irq = 4;
		break;

	case '2':
		dp->port = 0x2f8;
		dp->irq = 3;
		break;

	case 'p':
		dp->port = strtol (optarg, 0, 16);
		break;

	case 'P':
		if (optarg) {
			int nPrty = atoi (optarg);

			if ((nPrty > 0)
					&& (nPrty <= sched_get_priority_max (sched_getscheduler (0))))
				dp->priority = nPrty;
		}
		break;

	case 'i':
		dp->irq = strtol (optarg, 0, 0);
		break;

	default:
		return (-1);
	}
	return EOK;
}


/* Description: this is a callback function which is called when driver is shutting */
/*              down                                                                */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int ms  - program doesn't use this parameter                        */
/* Output     : None                                                                */
/* Return     : 0 if OK, otherwise -1                                               */
int
uart_shutdown (input_module_t * module, int ms)
{
	return (0);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/ser8250.c $ $Rev: 657836 $" );
