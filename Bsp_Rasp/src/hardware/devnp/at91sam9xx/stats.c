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


//
// called from devctl or at91sam9xx_process_interrupt() 
//
// read the nic hardware mib registers into our data struct
//
void    
at91sam9xx_update_stats (at91sam9xx_dev_t *at91sam9xx)
{
    uint32_t                *base = at91sam9xx->reg;
    nic_ethernet_stats_t    *estats = &at91sam9xx->stats.un.estats;

	at91sam9xx->stats.rxed_ok += *(base + EMAC_FRO);
    estats->fcs_errors += *(base + EMAC_FCSE);
    estats->align_errors += *(base + EMAC_ALE);
    estats->no_carrier += *(base + EMAC_CSE);
    estats->short_packets += *(base + EMAC_USF);
    estats->internal_rx_errors += *(base + EMAC_ROV);

    at91sam9xx->stats.txed_ok += *(base + EMAC_FTO);
    estats->single_collisions += *(base + EMAC_SCF);
    estats->multi_collisions += *(base + EMAC_MCF);
    estats->late_collisions += *(base + EMAC_LCOL);
    estats->xcoll_aborted += *(base + EMAC_ECOL);
	estats->total_collision_frames = estats->late_collisions + estats->single_collisions + estats->multi_collisions;

	// refresh for devctl
	at91sam9xx->stats.media = at91sam9xx->cfg.media;
}



//
// called from at91sam9xx_init()
//
void    
at91sam9xx_clear_stats (at91sam9xx_dev_t *at91sam9xx)
{

	// first, clear hardware register values by reading them
	at91sam9xx_update_stats(at91sam9xx);

	// now clear counters in our data structure
	memset(&at91sam9xx->stats, 0, sizeof(at91sam9xx->stats));
	
    // reset stats stuff for devctl
    at91sam9xx->stats.revision = NIC_STATS_REVISION;

    at91sam9xx->stats.valid_stats =
      NIC_STAT_TXED_MULTICAST |
      NIC_STAT_RXED_MULTICAST |
      NIC_STAT_TXED_BROADCAST |
      NIC_STAT_RXED_BROADCAST |
      NIC_STAT_TX_FAILED_ALLOCS |
      NIC_STAT_RX_FAILED_ALLOCS;

    at91sam9xx->stats.un.estats.valid_stats =
      NIC_ETHER_STAT_FCS_ERRORS |
      NIC_ETHER_STAT_ALIGN_ERRORS |
      NIC_ETHER_STAT_LENGTH_FIELD_MISMATCH |
      NIC_ETHER_STAT_NO_CARRIER |
      NIC_ETHER_STAT_SHORT_PACKETS |
      NIC_ETHER_STAT_OVERSIZED_PACKETS |
      NIC_ETHER_STAT_INTERNAL_RX_ERRORS |
      NIC_ETHER_STAT_JABBER_DETECTED |
      NIC_ETHER_STAT_EXCESSIVE_DEFERRALS |
      NIC_ETHER_STAT_SINGLE_COLLISIONS |
      NIC_ETHER_STAT_MULTI_COLLISIONS |
      NIC_ETHER_STAT_LATE_COLLISIONS |
      NIC_ETHER_STAT_XCOLL_ABORTED |
      NIC_ETHER_STAT_TOTAL_COLLISION_FRAMES |
      NIC_ETHER_STAT_JABBER_DETECTED |
      NIC_ETHER_STAT_INTERNAL_TX_ERRORS;
}


__SRCVERSION( "$URL$ $REV$" )
