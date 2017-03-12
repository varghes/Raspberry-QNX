/*
 * $QNXLicenseC:
 * Copyright 2009,2012 QNX Software Systems. All Rights Reserved.
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



#include "bpfilter.h"

#include "at91sam9xx.h"

#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif


/*
 * A jumbo packet could be handled by several at91sam9xx_receive() calls,
 * so these 2 temporary variables should be preserved between calls
 */
caddr_t 	cur_data=NULL;
struct mbuf	*cur_rpkt= NULL;

static void 
at91sam9xx_add_pkt (at91sam9xx_dev_t *at91sam9xx, struct mbuf *new, int idx)
{
    mpc_bd_t	*bd		= &at91sam9xx->rx_bd[idx];

	// remember mbuf pointer for this rx descriptor
	at91sam9xx->rx_pkts[idx] = new;
	bd->buffer =  pool_phys(new->m_data, new->m_ext.ext_page);
	
	// stuff rx descriptor
	bd->status = 0;
         
	CACHE_INVAL(&at91sam9xx->cachectl, new->m_data, bd->buffer, new->m_ext.ext_size);	
	
	// set wrap bit if on last rx descriptor
	if (idx == at91sam9xx->num_rx_descriptors -1) {
		bd->buffer |= EMAC_RBD_WRAP;  
    }   
}

void
at91sam9xx_receive (at91sam9xx_dev_t *at91sam9xx, struct nw_work_thread *wtp)
{
	struct mbuf		*rpkt;
	struct mbuf		*new;
    int      		this_idx;
    uint32_t  		status,buffer;
    uint32_t  		length,len = 0;
    mpc_bd_t  		*rx_bd;
	struct ifnet	*ifp = &at91sam9xx->ecom.ec_if;
	off64_t         phys = 0;

		// probe phy optimization - rx pkt activity
		at91sam9xx->rxd_pkts = 1;  

		for(;;) {
	
			if (at91sam9xx->cfg.verbose > 5) {
				log(LOG_ERR, "%s(): rx_cidx %d\n", __FUNCTION__, at91sam9xx->rx_cidx);
				}

			// is there an rxd packet at the next descriptor?
			this_idx	= at91sam9xx->rx_cidx;
			rx_bd		= &at91sam9xx->rx_bd[this_idx];
			status		= rx_bd->status;
			buffer		= rx_bd->buffer;
			if (!(buffer & EMAC_RBD_DONE)) {
				break;
				}

			length = (status & 0xFFF);

			// pull rxd packet out of corresponding queue
			rpkt = at91sam9xx->rx_pkts[this_idx];		
			at91sam9xx->rx_pkts[this_idx] = NULL;

			// update rx descriptor consumer index for next loop iteration
			at91sam9xx->rx_cidx = NEXT_RX(this_idx);

			// get an empty mbuf
			new = m_getcl_wtp(M_DONTWAIT, MT_DATA, M_PKTHDR, wtp);
			if (new == NULL) {
				// give old mbuf back to nic
				at91sam9xx_add_pkt(at91sam9xx, rpkt, this_idx);
				log(LOG_ERR, "%s(): mbuf alloc failed!", __FUNCTION__);
				at91sam9xx->stats.rx_failed_allocs++;
				ifp->if_ierrors++;
				continue;
				}

			// give new mbuf to nic - modifies what rx_bd points to!!
			at91sam9xx_add_pkt(at91sam9xx, new, this_idx);
			// dump frag if user requested it with verbose=8 cmd line option
			if (at91sam9xx->cfg.verbose > 7) {
				unsigned char *p = (unsigned char *)rpkt->m_data;
				int len = length;
				int I;

				log(LOG_ERR,"Rxd dev_idx %d  bytes %d\n", at91sam9xx->cfg.device_index, len);

				for (I=0; I<min(len,80); I++) {
					log(LOG_ERR,"%02X ",*p++);
					if ((I+1)%16 == 0) {
						log(LOG_ERR,"\n");
						}
					}
				if (I < 64) {
					log(LOG_ERR,"\n");
					}
				}

			// update rpkt for this rxd fragment
			rpkt->m_pkthdr.rcvif = ifp;
			rpkt->m_len = length - 4;  // if last of fragmented, corrected below
			rpkt->m_next = 0;
			
			// early-out high runner single packet

			if ((status & (EMAC_RBD_SOF | EMAC_RBD_EOF)) == (EMAC_RBD_SOF | EMAC_RBD_EOF)) {
				rpkt->m_pkthdr.len = rpkt->m_len;
				goto done;
				}

			// multi-fragment (jumbo) packet
			if (status & EMAC_RBD_SOF) {  // first fragment?
				cur_rpkt = m_getcl_wtp(M_DONTWAIT, MT_DATA, M_PKTHDR, wtp);
				phys  = pool_phys(cur_rpkt->m_data, cur_rpkt->m_ext.ext_page);
				CACHE_INVAL(&at91sam9xx->cachectl, cur_rpkt->m_data, phys, cur_rpkt->m_ext.ext_size);

				cur_data = cur_rpkt->m_data;
				memcpy(cur_data, rpkt->m_data, RX_BUF_SIZE);
				if(length == 0)
					len = RX_BUF_SIZE;
				cur_data+=RX_BUF_SIZE;
				m_freem(rpkt);
				continue;
				}
			else if(!(status & EMAC_RBD_EOF)){  // middle or last fragment
					if (!cur_data){
						m_freem(rpkt);
						continue;
						}
					memcpy(cur_data, rpkt->m_data, RX_BUF_SIZE);
					if(length == 0)
						len += RX_BUF_SIZE;
					cur_data+=RX_BUF_SIZE;
					m_freem(rpkt);
					continue;
					}
				if (status & EMAC_RBD_EOF) {  // last fragment?  Pass it up
					if (!cur_data || !cur_rpkt){
						m_freem(rpkt);
						continue;
						}
					memcpy(cur_data, rpkt->m_data, min (length - len,  RX_BUF_SIZE));
					CACHE_INVAL(&at91sam9xx->cachectl, cur_rpkt->m_data, phys, cur_rpkt->m_ext.ext_size);
					cur_rpkt->m_pkthdr.len = length;
					cur_rpkt->m_pkthdr.rcvif = ifp;
					cur_rpkt->m_len = length;  // if last of fragmented, corrected below
					cur_rpkt->m_flags |= M_HASFCS;  // length includes 4 byte crc
					cur_rpkt->m_next = NULL;
						
					m_freem(rpkt);
					rpkt = cur_rpkt;
					}
done:
#if NBPFILTER > 0
		/* Pass this up to any BPF listeners. */
		if (ifp->if_bpf) {
			bpf_mtap(ifp->if_bpf, rpkt);
			}
#endif
		ifp->if_ipackets++;
		(*ifp->if_input)(ifp, rpkt);
		} // for

} // at91sam9xx_receive()

__SRCVERSION( "$URL$ $REV$" )
