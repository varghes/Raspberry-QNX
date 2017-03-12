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
 * fd.c
 * 
 * Software device module.  Typically used for getting data from a 
 * serial device via the serial driver.  Can be used for any device
 * that has another driver already talking to it and has established
 * a file like interface to it.  The fd module will open the device
 * file, spawn a thread and just read from it.
 *
 */
#include <sys/devi.h>

#define	FLAG_OPENED			0x0100
#define	FLAG_INIT			0x0200

#define FLAG_SER			0x1000
#define FLAG_LED			0x2000
#define FLAG_KBD			0x4000
#define FLAG_UNKNOWN			0x8000
#define TYPE_MASK			0xf000

#define PROMOTE_PRTY                    (5)

static int fd_init (input_module_t * module);
static int fd_devctrl (input_module_t * module, int event, void *ptr);
static int fd_reset (input_module_t * module);
static int fd_output (input_module_t * module, void *ptr, int num);
static int fd_parm (input_module_t * module, int opt, char *optarg);
static int fd_shutdown (input_module_t * module, int ms);
static int setup_serial_port (input_module_t * module);


//#define FD_USE_IONOTIFY
#ifdef FD_USE_IONOTIFY
static int fd_pulse (message_context_t * ctx, int code, unsigned flags,
										 void *data);
#else
static void *fd_thread (void *data);
#endif


input_module_t fd = {
	NULL,
	NULL,
	NULL,
	0,
	DEVI_CLASS_NONE | DEVI_MODULE_TYPE_DEVICE,
	"Fd",
	__DATE__,
	"P:sd:",
	NULL,
	fd_init,
	fd_reset,
	NULL,
	fd_output,
	NULL,
	fd_parm,
	fd_devctrl,
	fd_shutdown,
};

struct private_data
{

	unsigned flags;
	int fd;
	unsigned mode;
	int priority;
	short trigger_level;
	short pulse_code;
	struct sigevent event;
	struct termios tios;
	struct termios p_tios;
	unsigned p_ctrl_state;
	char *dev_name;
#ifndef FD_USE_IONOTIFY
	pthread_t tid;
#endif
};


static int
set_vmin (struct private_data *dp, int min)
{
	struct termios *tios;

	if (tcgetattr (dp->fd, &dp->tios) < 0) {
		perror ("tcgetattr");
		return (-1);
	}

	tios = &dp->tios;
	tios->c_cc[VMIN] = min;

	if (tcsetattr (dp->fd, TCSANOW, tios) < 0) {
		perror ("tcsetattr");
		return (-1);
	}
	return (0);
}


static int
setup_serial_port (input_module_t * module)
{
	struct private_data *dp = (struct private_data *) module->data;

	/* get original state */

	if (tcgetattr (dp->fd, &dp->p_tios) < 0) {
		perror ("tcgetattr");
		return (-1);
	}

	if (devctl (dp->fd, DCMD_CHR_LINESTATUS, &dp->p_ctrl_state,
							sizeof (unsigned), NULL) != EOK) {
		perror ("devctl DCMD_CHR_LINESTATUS");
		return (-1);
	}

	dp->tios.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR
												| IGNCR | ICRNL | IXON);
	dp->tios.c_oflag &= ~OPOST;
	dp->tios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	dp->tios.c_cflag &= ~(CSIZE | PARENB | IHFLOW | OHFLOW);
	dp->tios.c_cflag |= CREAD;
	dp->tios.c_cflag |= CS8;

	if (tcsetattr (dp->fd, TCSANOW, &dp->tios) < 0) {
		perror ("tcsetattr");
		return (-1);
	}

	if (tcflush (dp->fd, TCIOFLUSH) < 0) {
		perror ("tcflush");
		return (-1);
	}

	if (set_vmin (dp, 1) == -1) {
		return (-1);
	}

	return (EOK);
}


static int
fd_init (input_module_t * module)
{
	struct private_data *dp = module->data;

	if (!module->data) {
		if (!(dp = module->data = calloc (sizeof *dp, 1))) {
			return (-1);
		}
		dp->dev_name = "/dev/ser1";
		dp->trigger_level = 1;
		/* Default priority  */
		dp->priority =
			min ((getprio (0) + PROMOTE_PRTY),
					 sched_get_priority_max (sched_getscheduler (0)));
	}

	return (0);
}


static int
fd_devctrl (input_module_t * module, int event, void *ptr)
{
	struct private_data *dp = module->data;
	struct termios tios;

	switch (event) {

	case DEVCTL_RESET:

		if (dp->flags & FLAG_SER) {

			int sw, i, *ms = (int *) ptr;

			if (ms == NULL)
				return (-1);

			for (i = 0; i < 2; i++) {

				sw = _SERCTL_DTR_CHG | _SERCTL_DURATION (*ms);
				devctl (dp->fd, DCMD_CHR_SERCTL, &sw, sizeof (sw), NULL);
				sw = _SERCTL_RTS_CHG | _SERCTL_DURATION (*ms);
				devctl (dp->fd, DCMD_CHR_SERCTL, &sw, sizeof (sw), NULL);
			}
			break;
		}
		return (-1);

	case DEVCTL_SETBAUD:

		if (dp->flags & FLAG_SER) {

			struct devctl_setbaud *portctl = ptr;

			if (tcgetattr (dp->fd, &tios) < 0)
				return (-1);

			if (portctl->cflag & (CS5 | CS6 | CS7 | CS8))
				tios.c_cflag &= ~CSIZE;

			tios.c_cflag = (tios.c_cflag & ~CLOCAL) | portctl->cflag;
			cfsetospeed (&tios, portctl->baud);
			cfsetispeed (&tios, portctl->baud);

			tcsetattr (dp->fd, TCSANOW, &tios);
			break;
		} else {
			if (verbosity)
				printf ("Unable to set baud rate, not a serial device\n");
		}

		return (-1);

	case DEVCTL_SETTRIG:{

			struct devctl_settrig *trig = ptr;

			if (dp->trigger_level != trig->trigger_level) {

				if (dp->flags & FLAG_SER) {
					if (set_vmin (dp, trig->trigger_level) == -1)
						return (-1);
				}
				dp->trigger_level = trig->trigger_level;
			}
			break;
		}

	case DEVCTL_LED:{

			short *led = (short *) ptr;
			unsigned flag = 0;

			if (dp->flags & FLAG_KBD) {

				flag |= _CONCTL_SCROLL_CHG | _CONCTL_NUM_CHG | _CONCTL_CAPS_CHG;
				if (*led & KEYIND_SCROLL_LOCK)
					flag |= _CONCTL_SCROLL;
				if (*led & KEYIND_CAPS_LOCK)
					flag |= _CONCTL_CAPS;
				if (*led & KEYIND_NUM_LOCK)
					flag |= _CONCTL_NUM;

				if (devctl (dp->fd, DCMD_CHR_SERCTL, &flag, sizeof (flag), 0) != 0)
					perror ("devctl");
				break;
			} else
				return (-1);
		}

	default:
		return (-1);
	}

	return (0);
}


static int
fd_reset (input_module_t * module)
{
	struct private_data *dp = module->data;
	struct _ttyinfo tinfo;

	memset (&tinfo, 0, sizeof (tinfo));

	if ((dp->flags & FLAG_INIT) == 0) {

		if (!(dp->flags & FLAG_OPENED)) {

			if ((dp->fd = open (dp->dev_name, O_RDWR)) == -1) {
				char *pMsgTxt = "Error: input device %s is not available\n";

				fprintf (stderr, pMsgTxt, dp->dev_name);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
							 dp->dev_name);
				return (-1);
			}

			if (devctl (dp->fd, DCMD_CHR_TTYINFO, &tinfo,
									sizeof (tinfo), NULL) != EOK) {
				char *pMsgTxt = "Error: DCMD_CHR_TTYINFO failed\n";

				fprintf (stderr, pMsgTxt);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
				return (-1);
			}

			if (tinfo.ttyname) {

				if (strstr (tinfo.ttyname, "ser")) {
					if (verbosity)
						printf ("Opened a serial device\n");
					dp->flags |= FLAG_SER;
				} else if (strstr (tinfo.ttyname, "kbd")) {
					if (verbosity)
						printf ("Opened the keyboard device\n");
					dp->flags |= FLAG_KBD;
				} else {
					if (verbosity)
						printf ("Unknown device type opened");
					dp->flags |= FLAG_UNKNOWN;
				}
			}
			dp->flags |= FLAG_OPENED;
		}

		if (dp->flags & FLAG_SER) {
			if (setup_serial_port (module) != EOK) {
				char *pMsgTxt = "Error: failed to setup device %s\n";

				fprintf (stderr, pMsgTxt, dp->dev_name);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
							 dp->dev_name);
			}
		}
#ifdef FD_USE_IONOTIFY

		dp->pulse_code = devi_register_pulse (module, DEVI_PULSE_ALLOC, fd_pulse);

		if (dp->pulse_code < 0) {
			char *pMsgTxt = "Error: %s - unable to alloc pulse handler\n";

			fprintf (stderr, pMsgTxt, module->name);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
						 module->name);
			return (-1);
		}

		memset (&dp->event, 0, sizeof (dp->event));
		SIGEV_PULSE_INIT (&dp->event, ip_coid, 21, dp->pulse_code, 0);

		if (ionotify (dp->fd, _NOTIFY_ACTION_POLLARM,
									_NOTIFY_COND_INPUT, &dp->event) < 0) {
			char *pMsgTxt = "Error: %s - ionotify failed\n";

			fprintf (stderr, pMsgTxt, module->name);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
						 module->name);
			return (-1);
		}
#else
		if (pthread_create (&dp->tid, NULL, fd_thread, (void *) module) != EOK) {
			char *pMsgTxt = "Error: %s - error create thread (%s)\n";

			fprintf (stderr, pMsgTxt, module->name, strerror (errno));
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
						 module->name, strerror (errno));
			return (-1);
		}
#endif
	}

	return (0);
}



static int
fd_output (input_module_t * module, void *ptr, int num)
{
	struct private_data *dp = module->data;
	int n, q = 0;
	char *p = ptr;

	while (num) {

		if ((n = write (dp->fd, p, num)) > 0) {
			num -= n;
			p += n;
			q = 0;
		} else {
			if (n < 0 && errno != EINTR && ++q >= 3) {
				break;
			}
		}
	}
	return (0);
}

#ifdef FD_USE_IONOTIFY


static int
fd_pulse (message_context_t * ctx, int code, unsigned flags, void *data)
{

	input_module_t *module = (input_module_t *) data;
	struct private_data *dp = module->data;
	input_module_t *out = module->out;
	char buffer[32];
	int count;

	count = read (dp->fd, buffer, sizeof (buffer));
	(out->input) (out, count, buffer);

	if (ionotify (dp->fd, _NOTIFY_ACTION_POLLARM,
								_NOTIFY_COND_INPUT, &dp->event) < 0) {
		perror ("ionotify");
		return (-1);
	}

	return (0);
}

#else

static void *
fd_thread (void *data)
{
	input_module_t *module = (input_module_t *) data;
	struct private_data *dp = (struct private_data *) module->data;
	input_module_t *up = module->up;
	char buffer[32];
	int count;
	int err_count = 0;
	int priority = 0;

	for (;;) {
		if (dp->priority != priority) {	/* priority was changed                      */
			dp->priority = priority =
				min (dp->priority, sched_get_priority_max (sched_getscheduler (0)));
			setprio (0, priority);
		}
		count = read (dp->fd, buffer, sizeof (buffer));
		if (count < 0) {

			if (err_count > 10) {
				char *pMsgTxt = "Error: too many errors, fd_thread exiting\n";

				fprintf (stderr, pMsgTxt);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
				pthread_exit (NULL);
			}
			if (verbosity >= 3)
				perror ("Module fd: read error");
			sleep (1);
			err_count++;
		}
		if (count > 0) {
			err_count = 0;
			(up->input) (up, count, buffer);
		}
	}

	return (NULL);
}

#endif

static int
fd_parm (input_module_t * module, int opt, char *optarg)
{
	struct private_data *dp = module->data;

	if (verbosity >= 3) {
		printf ("fd_parm: %c\n", opt);
		if (optarg)
			printf ("optarg: %s\n", optarg);
	}

	switch (opt) {

	case 'd':
		dp->dev_name = strdup (optarg);
		break;

	case 's':
		dp->flags |= FLAG_SER;
		break;

	case 'P':
		if (optarg) {
			int nPrty = atoi (optarg);

			if (nPrty > 0)
				dp->priority = nPrty;
		}
		break;

	default:
		return (-1);
	}

	return (0);
}


static int
fd_shutdown (input_module_t * module, int ms)
{
	struct private_data *dp = (struct private_data *) module->data;

	/* restore previous line settings */

	if (dp->flags & FLAG_SER) {

		if (verbosity)
			printf ("fd_shutdown: restoring previous settings\n");

		if (tcsetattr (dp->fd, TCSANOW, &dp->p_tios) < 0) {
			if (verbosity)
				perror ("tcsetattr");
		}

		if (devctl (dp->fd, DCMD_CHR_SERCTL, &dp->p_ctrl_state,
								sizeof (unsigned), NULL) != 0) {
			if (verbosity)
				perror ("devctl DCMD_CHR_SERCTL");
		}
	}
#ifndef FD_USE_IONOTIFY
	pthread_cancel (dp->tid);
#endif

	return (0);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/fd.c $ $Rev: 657836 $" );
