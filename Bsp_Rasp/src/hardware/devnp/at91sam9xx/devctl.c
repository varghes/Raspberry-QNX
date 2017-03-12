/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. All Rights Reserved.
 * 
 * You must obtain a written license from and pay applicable license fees to QNX 
 * Software Systems before you may reproduce, modify or distribute this software, 
 * or any work that includes all or part of this software.   Free development 
 * licenses are available for evaluation and non-commercial purposes.  For more 
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *  
 * This file may contain contributions from others.  Please review this entire 
 * file for other proprietary rights or license notices, as well as the QNX 
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
 * for other information.
 * $
 */




#include "at91sam9xx.h"

#include <net/ifdrvcom.h>
#include <sys/sockio.h>


static void
at91sam9xx_get_stats(at91sam9xx_dev_t *at91sam9xx, void *data)
{
	nic_stats_t				*stats = data;

	// read nic hardware registers into at91sam9xx data struct stats 
	at91sam9xx_update_stats(at91sam9xx);

	// copy it to the user buffer
	memcpy(stats, &at91sam9xx->stats, sizeof(at91sam9xx->stats));
}

int
at91sam9xx_ioctl(struct ifnet *ifp, unsigned long cmd, caddr_t data)
{
	at91sam9xx_dev_t			*at91sam9xx = ifp->if_softc;
	int				error = 0;
	struct ifdrv_com		*ifdc;
	struct drvcom_config		*dcfgp;
	struct drvcom_stats		*dstp;

	switch (cmd) {
	case SIOCGDRVCOM:
		ifdc = (struct ifdrv_com *)data;
		switch (ifdc->ifdc_cmd) {
		case DRVCOM_CONFIG:
			dcfgp = (struct drvcom_config *)ifdc;

			if (ifdc->ifdc_len != sizeof(nic_config_t)) {
				error = EINVAL;
				break;
			}
			memcpy(&dcfgp->dcom_config, &at91sam9xx->cfg, sizeof(at91sam9xx->cfg));
			break;

		case DRVCOM_STATS:
			dstp = (struct drvcom_stats *)ifdc;

			if (ifdc->ifdc_len != sizeof(nic_stats_t)) {
				error = EINVAL;
				break;
			}
			at91sam9xx_get_stats(at91sam9xx, &dstp->dcom_stats);
			break;

		default:
			error = EOPNOTSUPP;
			break;

		}
		break;


    case SIOCSIFMEDIA:
    case SIOCGIFMEDIA: {
        struct ifreq *ifr = (struct ifreq *)data;

        error = ifmedia_ioctl(ifp, ifr, &at91sam9xx->bsd_mii.mii_media, cmd);
        break;
        }

	default:
		error = ether_ioctl(ifp, cmd, data);
		if (error == ENETRESET) {
			//
			// Multicast list has changed; set the
			// hardware filter accordingly.
			//
			if ((ifp->if_flags_tx & IFF_RUNNING) == 0) {
				//
				// interface is currently down: at91sam9xx_init() 
				// will call at91sam9xx_set_multicast() so
				// nothing to do
				//
			} else {
				//
				// interface is up, recalc and hit gaddrs
				//
			at91sam9xx_rx_set_mode(at91sam9xx);
			}
			error = 0;
			break;
		}
		break;
	}

	return (error);
}


__SRCVERSION( "$URL$ $REV$" )
