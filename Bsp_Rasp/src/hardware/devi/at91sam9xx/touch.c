/*
 * $QNXLicenseC: 
 * Copyright 2009,   QNX Software Systems.  
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

/*=============================================================================

  Program:      touch

  Description:  This driver is for the Atmel Sam boards TSADC touch
                controller.

=============================================================================*/


#include <sys/devi.h>
#include "touch.h"
#include <arm/bcm2835.h>

#define _SLOGC_TOUCH      8
/*==============================================================================

  Data Structure input_module_t

  Our protocol module is represented by the following input_module_t
  data structure.  We always create one static instance of the module.
  If more than one are needed, ie in multiple bus lines; then the system
  will allocate a new module and copy the contents of the static one
  into it.

==============================================================================*/

input_module_t  touch = {
        NULL,                   /* up, filled in at runtime */
        NULL,                   /* down, filled in at runtime */
        NULL,                   /* line we belong to, filled in at runtime */
        0,                      /* flags, leave as zero */
        DEVI_CLASS_ABS|DEVI_MODULE_TYPE_PROTO|DEVI_MODULE_TYPE_DEVICE,
                                               /* Our type, we are a
                                                * protocol module or class
                                                * relative.  This info will
                                                * tell the runtime system
                                                * which filter module to
                                                * link above us
                                                */
        "touch",            /* name, must match what you specify
                             * on the command line when invoking this
                             * module
                             */
        __DATE__,           /* date of compilation, used for output when
                             * the cmdline option -l is given to the
                             * driver
                             */
        "i:a:vd:D:p:",        /* command line parameters */
        NULL,               /* pointer to private data, set this up
                             * in the init() callback
                             */
        touch_init,         /* init() callback, required */
        touch_reset,        /* reset() callback, required */
        NULL,               /* input() callback */
        NULL,               /* output(), not used */
        NULL,        /* pulse(), called when the timer expires
                             * used for injecting a release event */
        touch_parm,         /* parm() callback, required */
        touch_devctrl,      /* devctrl() callback */
        touch_shutdown      /* shutdown() callback, required */
};

void set_touchscreen_mode (void *data);
 int read_data1 (unsigned int *x, unsigned int *y, void *data);
 void spi_init ( void *data );

/*=============================================================================

  Name:         touch_init 

  Description:	This is our init() callback specified in the input_module_t.
                Just simple init routine.
 
=============================================================================*/
static int touch_init(input_module_t *module)
{
	private_data_t *dp = module->data;

	//fprintf(stderr,"touch_init\n");

	if(!module->data)
	{
		if(!(dp = module->data = scalloc(sizeof *dp)))
		{
			return (-1);
		}
		
		ThreadCtl (_NTO_TCTL_IO, 0);

		dp->physbase = INVALID_BASE;
		
		dp->flags = FLAG_RESET;
		dp->irq = TOUCH_INT;  //ADC interrupt , no need in this case
		dp->irq_pc = DEVI_PULSE_ALLOC;
		dp->lastx = 0;
		dp->lasty = 0;
		dp->param.sched_priority = PULSE_PRIORITY; 
		dp->event.sigev_priority = dp->param.sched_priority;
		dp->release_delay = RELEASE_DELAY;
		dp->intr_delay = INTR_DELAY;
		dp->pen_toggle = 0;
		pthread_mutex_init (&dp->mutex, NULL);
	}
	return (0);
}


/*=============================================================================

  Name:		      touch_parm

  Description:	This callback is called by the input runtime system to parse
                command line options to the 'touch' module of devi-at91sam9xx.
 
=============================================================================*/
static int touch_parm(input_module_t *module, int opt, char *optarg)
{
	private_data_t *dp = module->data;
	//fprintf(stderr,"touch_parm\n");
 /*	switch (opt)
	{
		case 'v':
				dp->verbose++;
				break;
		case 'i':
				dp->irq = atoi (optarg);
				break;
		case 'a':
				dp->physbase = strtoul (optarg, NULL, 0);
				break;
		case 'p':
				dp->param.sched_priority = atoi (optarg);
				dp->event.sigev_priority = dp->param.sched_priority;
				break;
		case 'd':
				dp->release_delay = (atol (optarg)) * 100000;   /* Convert to nsecs */
/*				break;
		case 'D':
				dp->intr_delay = atoi (optarg);
				break;
 		default:
				break;
 	}
*/

	return (0);
}


/*=============================================================================

  Name:         touch_reset

  Description:  Resets the Touch screen controller. Also create a timer, the purpose
                of this timer is to inject the release events, since the touch
                controller does not give any indentification of a release.

                Also create a separate thread to handle the IRQ's from the
                touch controller.

=============================================================================*/
static int touch_reset(input_module_t *module)
{
	unsigned int  mask,PinMask,Reg,gpio_no;
	private_data_t	*dp = module->data;
//	fprintf(stderr,"touch_reset\n");
	if((dp->flags & FLAG_INIT) == 0)
	{
	/*	if (dp->physbase == INVALID_BASE)
		{
			perror ("missing the base address of the touch screen controller");
			exit (-1);
		}*/
		if (ThreadCtl(_NTO_TCTL_IO, NULL) == -1) {
			perror("ThreadCtl: ");
			exit(EXIT_FAILURE);
		}

		if ((dp->touch_base = mmap_device_io (0x100, BCM2835_GPIO_BASE)) == (uintptr_t)MAP_FAILED)
		{
			perror ("mmap_device_io failed");
			exit (-1);
		}

		if ((dp->spi_base = mmap_device_io (0x30, BCM2835_SPI0_BASE)) == (uintptr_t)MAP_FAILED)
		{
			perror ("mmap_device_io failed");
			exit (-1);
		}

		if ((dp->aux_base = mmap_device_io (0x10, AUX_PERIPHL_BASE)) == (uintptr_t)MAP_FAILED)
		{
			perror ("mmap_device_io failed");
			exit (-1);
		}

	//	out32(dp->touch_base + TSADCC_CR, SWRST);

		/* Clear interrupt and Unmask */
//		out32(dp->touch_base + TSADCC_IDR, DISABLE_INTR);



// activate interrupt
		  gpio_no=25;

		    PinMask = (1 << (gpio_no % 32));
		    if (gpio_no < 32)
		    {
				  Reg= in32(dp->touch_base+BCM2835_GPIO_GPFEN0);
				  out32(dp->touch_base+BCM2835_GPIO_GPFEN0,(Reg|PinMask));
				  Reg=in32(dp->touch_base+BCM2835_GPIO_GPEDS0);
				  out32(dp->touch_base+BCM2835_GPIO_GPEDS0,(Reg|PinMask));//clear bit
		    }
		    else
		    {
			       Reg= in32(dp->touch_base+BCM2835_GPIO_GPFEN1);
				   out32(dp->touch_base+BCM2835_GPIO_GPFEN1,(Reg|PinMask));
				   Reg=in32(dp->touch_base+BCM2835_GPIO_GPEDS1);
				   out32(dp->touch_base+BCM2835_GPIO_GPEDS1,(Reg|PinMask));//clear bit
		    }


		/*
		 *  IRQ 49 is gpio_int[0]
		 *  IRQ 50 is gpio_int[1]
		 *  IRQ 51 is gpio_int[2]
		 *  IRQ 52 is gpio_int[3]
		 */
		//    WILL BE loaded by interrupt callback routines
		   // mask = 1 << ((gpio_no / 32) + 17); // fix it ,documentation issue
			//Reg=GET32(IRQ_ENABLE_2);
		    //PUT32(IRQ_ENABLE_2,(Reg|mask));



		/* Put PMIC touch ADC into standby (required for pen down interrupt) */
	//	set_touchscreen_mode (module);

		/* Create touch release timer */
		dp->timerid = devi_register_timer (module, 15, &dp->irq_pc, NULL);

		/* Setup the interrupt handler thread */
		if ((dp->chid = ChannelCreate (_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1)
		{
			perror ("Error: ChannelCreate");
			exit (-1);
		}

		if ((dp->coid = ConnectAttach (0, 0, dp->chid, _NTO_SIDE_CHANNEL, 0)) == -1)
		{
			perror ("Error: ConnectAttach");
			exit (-1);
		}

		pthread_attr_init (&dp->pattr);
		pthread_attr_setschedpolicy (&dp->pattr, SCHED_RR);
		pthread_attr_setschedparam (&dp->pattr, &dp->param);
		pthread_attr_setinheritsched (&dp->pattr, PTHREAD_EXPLICIT_SCHED);
		pthread_attr_setdetachstate (&dp->pattr, PTHREAD_CREATE_DETACHED);
		pthread_attr_setstacksize (&dp->pattr, 4096);

		dp->event.sigev_notify     = SIGEV_PULSE;
		dp->event.sigev_coid       = dp->coid;
		dp->event.sigev_code       = 1;

		/* Create interrupt handler thread */
		if (pthread_create (NULL, &dp->pattr, (void *)intr_thread, module))
		{
			perror ("Error: pthread_create");
			exit (-1);
		}

		/* Attach interrupt. */
		if (dp->verbose >= 3)
			fprintf (stderr, "Attaching to interrupt %d\n", dp->irq);

		if ((dp->iid = InterruptAttachEvent(dp->irq, &dp->event, _NTO_INTR_FLAGS_TRK_MSK)) == -1)
		{
			perror ("Error: InterruptAttachEvent");
			exit (-1);
		}

		dp->flags |= FLAG_INIT;
	}

	return (0);
}


/*=============================================================================

	Name:         touch_devctrl

  Description:  This callback is used by modules in an event bus line to send
                information further up the line to other modules (e.g. abs).

                This allows the other modules to know how many buttons we
                have, pointer coordinates, and the range of the coordinates.

=============================================================================*/
static int touch_devctrl(input_module_t *module, int event, void *ptr)
{
	private_data_t 	*dp = module->data;
	//fprintf(stderr,"touch_devctrl\n");
	slogf(_SLOGC_TOUCH, _SLOG_INFO, "touch_devctrl");
	switch(event)
	{
		case DEVCTL_GETDEVFLAGS:
			*(unsigned short *)ptr = (dp->flags & FLAGS_GLOBAL);
			break;
		case DEVCTL_GETPTRBTNS:
			*(unsigned long *)ptr = 1L;
			break;
		case DEVCTL_GETPTRCOORD:
			*(unsigned char *)ptr = '\02';
			break;
		case DEVCTL_GETCOORDRNG:
		{
			struct devctl_coord_range *range = ptr;

			range->min = 0;
			//range->max = 1024;
			//range->max = 800;
			range->max = 4096;
			break;
		}
		default:
			return (-1);
	}
	
	return (0);
}


/*=============================================================================

  Name:         set_touchscreen_mode

  Description:  This is the code for the interrupt handler thread.  It simply

=============================================================================*/
void set_touchscreen_mode (void *data)
{
	input_module_t    *module = (input_module_t *) data;
	private_data_t    *dp = module->data;
	uint32_t tsadcc_mr, prescal, startuptime, sampleholdtime;
	uint32_t debounceperiod, ldebounceperiod, pendbc;

	//fprintf(stderr,"set_touchscreen_mode\n");
		/* Setup Touchscreen only Mode */
		tsadcc_mr = in32(dp->touch_base + TSADCC_MR);
		tsadcc_mr &= ~TSAMOD;
		out32(dp->touch_base + TSADCC_MR, tsadcc_mr | TSAMOD_TSONLYMODE);

		/* Enable Pen Detection */
		out32(dp->touch_base + TSADCC_MR, in32(dp->touch_base + TSADCC_MR) | PENDET_ENABLE);

		/* Setup ADC Clock of 1 MHz */
		prescal = (((BOARD_MCK / ADCCLK) / 3 / 2 / 3) * 200); 
		tsadcc_mr = in32(dp->touch_base + TSADCC_MR);
		tsadcc_mr &= ~PRESCAL;
		if(prescal > 0xff)
                        prescal = 0xff;
		out32(dp->touch_base + TSADCC_MR, tsadcc_mr | (prescal <<8));

		/* Setup Start up time */
		/* Startup Time = (STARTUP+1) * 8 / ADCCLK */
		startuptime = ((STARTUP_TIME * ADCCLK) / 8000000 ) - 1;
		tsadcc_mr = in32(dp->touch_base + TSADCC_MR);
		tsadcc_mr &= ~STARTUP;
		out32(dp->touch_base + TSADCC_MR, tsadcc_mr | (startuptime <<16));

		/* Setup Sample and Hold time */
		/* Sample & Hold Time = (SHTIM+1) / ADCCLK */
		/* To avoid overflow and underflow in the calculation divisor is split */
		sampleholdtime = ((SAMPLE_HOLD_TIME * (ADCCLK / 1000)) / 1000000 ) - 1;
		tsadcc_mr = in32(dp->touch_base + TSADCC_MR);
		tsadcc_mr &= ~SHTIM;
		out32(dp->touch_base + TSADCC_MR, tsadcc_mr | (sampleholdtime << 24));
		out32(dp->touch_base + TSADCC_TSR, (sampleholdtime << 24));

		/* Setup Pen Detect Debouncing period */
		/* Period = 2^PENDBC/ADCCLK */
		/* To avoid overflow and underflow in the calculation divisor is split */
		debounceperiod = (DEBOUNCE_PERIOD / 1000000) * (ADCCLK / 1000);
		tsadcc_mr = in32(dp->touch_base + TSADCC_MR);
		pendbc = 0;
		ldebounceperiod = 1;
		while (ldebounceperiod < debounceperiod) {
			pendbc ++;
			ldebounceperiod *= 2;
		} 
		tsadcc_mr &= ~PENDBC;
		out32(dp->touch_base + TSADCC_MR, tsadcc_mr | ( pendbc << 28));

		out32(dp->touch_base + TSADCC_TRGR, PENDET_TRGR);

		/* Enable Pen Contact Interrupt */
		out32(dp->touch_base + TSADCC_IER, PENCNT);
}


/*=============================================================================

  Name:         start_conversion 

  Description:  This is the code for the interrupt handler thread.  It simply

=============================================================================*/
void set_dp(void *data)
{
	//fprintf(stderr,"set_dp\n");
	input_module_t    *module = (input_module_t *) data;
	private_data_t    *dp = module->data;
	uint32_t dpd = 10, debounceperiod, tsadcc_mr, ldebounceperiod, pendbc;

	/* Setup Pen Detect Debouncing period */
	debounceperiod = (dpd ) * (ADCCLK / 1000)/ 1000000;
	tsadcc_mr = in32(dp->touch_base + TSADCC_MR);
	pendbc = 0;
	ldebounceperiod = 1;
	while (ldebounceperiod < debounceperiod) {
		pendbc ++;
		ldebounceperiod *= 2;
	}
	tsadcc_mr &= ~PENDBC;
	out32(dp->touch_base + TSADCC_MR, tsadcc_mr | ( pendbc << 28));
}

/*=============================================================================

  Name:         read_conversion 

  Description:  This is the code to get measurements

=============================================================================*/
void read_conversion (void *data)
{
	input_module_t    *module = (input_module_t *) data;
	private_data_t    *dp = module->data;
	//fprintf(stderr,"read_conversion\n");
	dp->tp.x = (in32(dp->touch_base + TSADCC_CDR3) * 1024)/in32(dp->touch_base + TSADCC_CDR2);

	dp->tp.y = (in32(dp->touch_base + TSADCC_CDR1) * 1024)/in32(dp->touch_base + TSADCC_CDR0);
}

/*=============================================================================

  Name:         intr_thread

  Description:  This is the code for the interrupt handler thread.  It simply
                waits on a pulse that is generated by the interrupt and then
                requests the X and Y coordinates from the touch controller.
                Since the touch controller generates an interrupt when we
                request the data we have a simple toggle to ignor the second
                interrupt.

                Once the data has been fetched, a timer is started to inject
                a release event.  The timer will go off if no other interrupt
                has been received, this is so we can simulate a release to the
                upper layers.
=============================================================================*/

static void *intr_thread( void *data )
{ 
	input_module_t    *module = (input_module_t *) data;
	private_data_t    *dp = module->data;
	input_module_t    *up = module->up;
	struct _pulse   pulse;
	iov_t           iov;
	int             rcvid;
	int eoc;
	uint32_t status, tsadcc_mr, lastm =0,interrupt_count=0;
	unsigned int bits,i,count=0,xyval=0;
	unsigned int    x = 0, y = 0xFFF;
//	slogf(_SLOGC_TOUCH, _SLOG_INFO, "thread %d",count);
//fprintf(stderr,"intr_thread\n");
	SETIOV (&iov, &pulse, sizeof(pulse));

	  spi_init(data);
	while (1)
	{
		if ((rcvid = MsgReceivev (dp->chid, &iov, 1, NULL)) == -1)
		{
			if (errno == ESRCH)
			{
				pthread_exit (NULL);
			}
			continue;
		}

		switch (pulse.code)
		{
			case PULSE_CODE:
				pthread_mutex_lock (&dp->mutex);
			       if (xyval<800)
			    	   xyval=xyval+20;
			       else
			    	   xyval=0;
			       fprintf(stderr,"Got Interrupt\n");
				/* Stop timer */
				dp->itime.it_value.tv_sec = 0;
				dp->itime.it_value.tv_nsec = 0;
				dp->itime.it_interval.tv_sec = 0;
				dp->itime.it_interval.tv_nsec = 0;

				/* Set touch release timer */
				timer_settime(dp->timerid, 0, &dp->itime, NULL);

				read_data1( &x, &y, data );

				bits = in32(dp->touch_base+BCM2835_GPIO_GPEDS0);
				if(bits)
				{
					for(i=0;i<32;i++)
					{
						if(bits&(1<<i))
						{
							out32(dp->touch_base+BCM2835_GPIO_GPEDS0,1<<i); // clear 1 bit
											break;
						}
				    }
			     }
			     bits = in32(dp->touch_base+BCM2835_GPIO_GPEDS1);
				 if(bits)
				 {
					for(i=0;i<32;i++)
					{
						if(bits&(1<<i))
						{
							out32(dp->touch_base+BCM2835_GPIO_GPEDS1,1<<i);
							break;
						}
					}
				}

			//	set_touchscreen_mode(data, POSITION_MODE);
			//	read_conversion(data, raw_x, raw_y, raw_r);

			//	process_data(raw_x, raw_y, raw_r, &x, &y, data);
		//		 read_conv(data, raw_x, raw_y, raw_r);

                dp->tp.x=x;
                dp->tp.y=y;
                if(x>1)
                {
                dp->tp.buttons = _POINTER_BUTTON_LEFT;
                }
                else
                	dp->tp.buttons = 0;
                clk_get(&dp->tp.timestamp);

				(up->input)(up, 1, &dp->tp);
				dp->lastx =  dp->tp.x;  // varghese added
                dp->lasty = dp->tp.y;  //end

                if (dp->tp.buttons != 0L)
				{
					/* Start the timer to poll the hardware */
					/* (Re)Start timer */
					dp->itime.it_value.tv_sec = 0;
					dp->itime.it_value.tv_nsec = dp->release_delay;
					dp->itime.it_interval.tv_sec = 0;
					dp->itime.it_interval.tv_nsec = 0;
					/* Set touch release timer */
					timer_settime(dp->timerid, 0, &dp->itime, NULL);
				}
				delay (dp->intr_delay);
			//	delay (300);
				pthread_mutex_unlock (&dp->mutex);
				InterruptUnmask (dp->irq, dp->iid);// varghese
			//	fprintf(stderr,"done\n");
				break;
			default:
				if (rcvid)
				{
					fprintf(stderr,"reply sending\n");
					MsgReplyv (rcvid, ENOTSUP, &iov, 1);
				}
				break;


       }


	}
}

/*=============================================================================

  Name:         touch_shutdown

  Description:  This callback preforms the cleanup of the driver when it is
                signaled to quit.

=============================================================================*/
static int touch_shutdown(input_module_t *module, int delay)
{
	private_data_t  *dp = module->data;
//	fprintf(stderr,"touch_shutdown\n");
	if (munmap_device_io ((uintptr_t)dp->touch_base, 0x100) == -1)
	{
		perror ("touch_shutdown: Error munmap_device_io\n");
		_exit(-1);
	}

	free (module->data);

	return (0);
}




void spi_init ( void *data )
{
    unsigned int ra;
	input_module_t    *module = (input_module_t *) data;
	private_data_t    *dp = module->data;
    ra=in32(dp->aux_base+AUX_ENABLES);
    ra|=2; //enable spi0
    out32(dp->aux_base+AUX_ENABLES,ra);

    ra=in32(dp->touch_base+BCM2835_GPIO_GPFSEL0);
    ra&=~(7<<27); //gpio9
    ra|=4<<27;    //alt0
    ra&=~(7<<24); //gpio8
    ra|=4<<24;    //alt0
    ra&=~(7<<21); //gpio7
    ra|=4<<21;    //alt0
    out32(dp->touch_base+BCM2835_GPIO_GPFSEL0,ra);
    ra=in32(dp->touch_base+BCM2835_GPIO_GPFSEL1);
    ra&=~(7<<0); //gpio10/
    ra|=4<<0;    //alt0
    ra&=~(7<<3); //gpio11/
    ra|=4<<3;    //alt0
    out32(dp->touch_base+BCM2835_GPIO_GPFSEL1,ra);
   // ra=GET32(GPFSEL2);
   // ra&=~(7<<15); //gpio25/
   // ra|=1<<15;    //output
   // PUT32(GPFSEL2,ra);

    out32(dp->spi_base+BCM2835_SPI0_CS,0x0000030);
    out32(dp->spi_base+BCM2835_SPI0_CLK,0); //slowest possible, could probably go faster here
    //PUT32(AUX_SPI0_CLK,26); */

    out32(dp->spi_base+BCM2835_SPI0_CS,0x000000B1);

}

unsigned int spi_Read( void *data)
{
	unsigned int  data1,data2;
	unsigned int value;
	input_module_t    *module = (input_module_t *) data;
	private_data_t    *dp = module->data;

	out32(dp->spi_base+BCM2835_SPI0_FIFO,0x0);
	while(1) if(in32(dp->spi_base+BCM2835_SPI0_CS)&(1<<16)) break;
    while(1) if(in32(dp->spi_base+BCM2835_SPI0_CS)&(1<<17)) break;
    data1 = in32(dp->spi_base+BCM2835_SPI0_FIFO);
    out32(dp->spi_base+BCM2835_SPI0_FIFO,0x0);
	while(1) if(in32(dp->spi_base+BCM2835_SPI0_CS)&(1<<16)) break;
    while(1) if(in32(dp->spi_base+BCM2835_SPI0_CS)&(1<<17)) break;
    data2 = in32(dp->spi_base+BCM2835_SPI0_FIFO);
	value = (data1 << 5 | data2 >> 3);
	return value;
}

void spi_one_byte_write (void *data, unsigned int x )
{
	unsigned int data2;
	input_module_t    *module = (input_module_t *) data;
	private_data_t    *dp = module->data;

    while(1)
    {
        if(in32(dp->spi_base+BCM2835_SPI0_CS)&(1<<18)) break; //TXD
    }
    out32(dp->spi_base+BCM2835_SPI0_FIFO,x);

    while(1) if(in32(dp->spi_base+BCM2835_SPI0_CS)&(1<<16)) break;
    while(1) if(in32(dp->spi_base+BCM2835_SPI0_CS)&(1<<17)) break; //should I wait for this?
    data2 = in32(dp->spi_base+BCM2835_SPI0_FIFO);  //clean fifo*/

}


/* This function is responsible for reading the touch data over I2C */
 int read_data1 (unsigned int *x, unsigned int *y, void *data)
{
	input_module_t    *module = (input_module_t *) data;
	private_data_t    *dp = module->data;

	out32(dp->spi_base+BCM2835_SPI0_CS,0x000000B1);
    spi_one_byte_write(data,0xD0);
    *x=spi_Read(data);
    out32(dp->spi_base+BCM2835_SPI0_CS,0x000000B1);
    spi_one_byte_write(data,0x90);
    *y=spi_Read(data);
	 //if(((*x>0x75) && (*y>0xB4)) ||((*x<0xF90)&&(*y<0xF60)))
    if(((*x>0x5) && (*y>0x5)) ||((*x<0xF90)&&(*y<0xF60)))
	 {
		// *x=*x/5;
		// *y=*y/8;
		// *y=*y/5;
		 ;
	 }
	 else
	 {
		 *x=0;
		 *y=0;
	 }
	 fprintf(stderr,"X Val:%d\n",*x);
	 fprintf(stderr,"Y Val:%d\n",*y);
	return (0);
}
