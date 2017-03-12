/*
 * $QNXLicenseC:
 * Copyright 2007, 2008, QNX Software Systems.
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

// Module Description:  MMC command processing

#include <sim_mmc.h>

static void mmc_media_check(SIM_HBA *hba);

ssize_t mmc_slogf(SIM_HBA *hba, int opcode, int severity, int vlevel, const char *fmt, ...)
{
	ssize_t		ret;
	va_list		arglist;
	int			verbosity;

	ret			= 0;
	verbosity	= hba ? hba->verbosity : mmc_ctrl.verbosity;
	
	if (verbosity > 5) {
		va_start(arglist, fmt);
		vfprintf(stderr, fmt, arglist);
		va_end(arglist);
		fprintf(stderr, "\n");
	}
	if (vlevel <= 4 || verbosity >= vlevel) {
		va_start(arglist, fmt);
		ret = vslogf(opcode, severity, fmt, arglist);
		va_end(arglist);
	}
	return (ret);
}

int mmc_sense(SIM_HBA *hba, int sense, int asc, int ascq)
{
	SIM_MMC_EXT	*ext = (SIM_MMC_EXT *)hba->ext;
	CCB_SCSIIO	*ccb = ext->nexus;
	SCSI_SENSE	*sptr;

	if (ccb) {
		ccb->cam_scsi_status	= SCS_CHECK;
		ccb->cam_ch.cam_status	= CAM_REQ_CMP_ERR;
		sptr					= (SCSI_SENSE *)ccb->cam_sense_ptr;

		if ((ccb->cam_ch.cam_flags & CAM_DIS_AUTOSENSE) || sptr == NULL) {
			return( ccb->cam_ch.cam_status );
		}

		ccb->cam_ch.cam_status |= CAM_AUTOSNS_VALID;

		memset(sptr, 0, sizeof(*sptr));
		sptr->error	= 0x70;		// Error code
		sptr->sense = sense;	// Sense key
		sptr->asc	= asc;		// Additional sense code (Invalid field in CDB)
		sptr->ascq	= ascq;		// Additional sense code qualifier
	}
	return( ccb->cam_ch.cam_status );
}

static void mmc_reset(SIM_HBA *hba)
{
	// TODO
}

int mmc_wait_command(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	struct _pulse	pulse;
	iov_t			iov;
	int				rcvid, intr=0, retry=0;

	ext   = (SIM_MMC_EXT *)hba->ext;
	SETIOV(&iov, &pulse, sizeof(pulse));
	
	while (1) {
		if ((rcvid = MsgReceivev(hba->chid, &iov, 1, NULL)) == -1)
			continue;
		intr = 0;
		switch (pulse.code) {
			case SIM_INTERRUPT:
					intr=ext->interrupt(hba, hba->cfg.IRQRegisters[0], ext->resp_type, ext->mmc_resp);
					InterruptUnmask(hba->cfg.IRQRegisters[0], hba->iid);
					break;
			case SIM_DMA_INTERRUPT:
					intr=ext->interrupt(hba, hba->cfg.IRQRegisters[0], ext->resp_type, ext->mmc_resp);
					if(hba->cfg.NumIRQs > 1) {
						InterruptUnmask(hba->cfg.IRQRegisters[1], ext->iid);
					}
					break;
			case SIM_TIMER:
					// Only check media if card detection interrupt is not supported
					if (!(ext->hccap & MMC_HCCAP_CD_INTR))
						mmc_media_check(hba);
					if(!(ext->eflags & MMC_EFLAG_INSERTED))
						return MMC_FAILURE;
					break;
			default:
					break;
		}
		if(intr & MMC_INTR_CARD){
			mmc_media_check(hba);
			return MMC_FAILURE;
		}
		
		if((intr & MMC_INTR_COMMAND) && !(intr & MMC_INTR_ERROR)) 
			return MMC_SUCCESS;
		
		if(retry++ >5)
		   return MMC_FAILURE;
	}
	return MMC_FAILURE;
}

static int mmc_get_card_status(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;

	ext = (SIM_MMC_EXT *)hba->ext;

	if(mmc_sendcmd(hba, MMC_SEND_STATUS, MMC_RSP_R1, MMC_CMD_INTR, ext->rca << 16)!=MMC_SUCCESS)
		return MMC_FAILURE;

	return (mmc_wait_command(hba));
}

int mmc_wait_card_ready(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	int			retry=0;

	ext = (SIM_MMC_EXT *)hba->ext;

	if(!(ext->hccap & MMC_HCCAP_NOCD_BUSY)){
		return (MMC_SUCCESS);
	}
	
	while(retry++ <ext->busy_retry){
		if(mmc_get_card_status(hba)!=MMC_SUCCESS){
			slogf(_SLOGC_SIM_MMC, _SLOG_ERROR, "%s: failed %x", __func__, ext->mmc_resp[0]);
			break;
		}
		if( ext->mmc_resp[0] & 0x100){
			return MMC_SUCCESS;
		}
		//TOD. what about error status (not in programming state)
	}
	slogf(_SLOGC_SIM_MMC, _SLOG_ERROR, "%s: timeout,  card status %x", __func__, ext->mmc_resp[0]);

	return MMC_FAILURE;
}

static void mmc_error(SIM_HBA *hba, int mmc_status)
{
	SIM_MMC_EXT	*ext = (SIM_MMC_EXT *)hba->ext;
	CCB_SCSIIO	*ccb = ext->nexus;

	if (ccb) {
		switch (mmc_status) {
			case MMC_DATA_OVERRUN:
				ccb->cam_ch.cam_status = CAM_DATA_RUN_ERR;
				break;
			case MMC_NOT_PRESENT:
				mmc_sense(hba, SK_NOT_RDY, ASC_MEDIA_NOT_PRESENT, 0);
				break;
			case MMC_TIMEOUT:
			case MMC_COMMAND_FAILURE:
				mmc_reset(hba);
				ccb->cam_ch.cam_status = CAM_CMD_TIMEOUT;
				break;
			case MMC_READ_ERROR:			// CRC errors
			case MMC_WRITE_ERROR:
			default:
				ccb->cam_ch.cam_status = CAM_CMD_TIMEOUT;
				break;
		}
	}
}

static void mmc_msense(SIM_HBA *hba)
{
	SIM_MMC_EXT			*ext;
	CCB_SCSIIO			*ccb;
	MODE_PARM_HEADER10	*hdr;

	ext = (SIM_MMC_EXT *)hba->ext;
	ccb = ext->nexus;

	if (!(ext->eflags & MMC_EFLAG_READY)) {
		mmc_sense(hba, SK_NOT_RDY, ASC_MEDIA_NOT_PRESENT, 0);
		return;
	}

	switch (ccb->cam_cdb_io.cam_cdb_bytes[2]) {
		case MP_RW_ERR:
			hdr = (MODE_PARM_HEADER10 *)ccb->cam_data.cam_data_ptr;
			memset(hdr, 0, sizeof(*hdr));
			if (ext->eflags & MMC_EFLAG_WP)
				hdr->device_specific |= MP_DS_WP;

			ccb->cam_ch.cam_status = CAM_REQ_CMP;
			ccb->cam_scsi_status   = SCS_GOOD;
			break;
		default:
			mmc_sense(hba, SK_ILLEGAL, ASC_INVALID_FIELD, 0);
			break;
	}
}

static void mmc_unit_ready(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext = (SIM_MMC_EXT *)hba->ext;
	CCB_SCSIIO	*ccb = ext->nexus;

	if (ext->eflags & MMC_EFLAG_MEDIA_CHANGED) {
		mmc_sense(hba, SK_UNIT_ATN, ASC_MEDIUM_CHANGED, 0);
		atomic_clr(&ext->eflags, MMC_EFLAG_MEDIA_CHANGED);
	} else if (ext->eflags & MMC_EFLAG_READY) {
		if (ext->detect(hba) != MMC_SUCCESS) {
			mmc_media_check(hba);
			mmc_sense(hba, SK_NOT_RDY, ASC_MEDIA_NOT_PRESENT, 0);
		} else
			ccb->cam_ch.cam_status = CAM_REQ_CMP;
	} else
		mmc_sense(hba, SK_NOT_RDY, ASC_MEDIA_NOT_PRESENT, 0);
}

int mmc_evpd_inquiry( SIM_HBA *hba, CCB_SCSIIO *ccb )
{
	SIM_MMC_EXT						*ext;
	VPD_HEADER						*vh;
	BLOCK_LIMITS					*bl;
	ID_HEADER		*id;
	BLOCK_DEVICE_CHARACTERISTICS	*bdc;
	uint8_t							*is;
	uint8_t							*vpd;
	uint16_t						lps;

	ext	= (SIM_MMC_EXT *)hba->ext;

	vpd	= (uint8_t *)ccb->cam_data.cam_data_ptr;
	vh	= (VPD_HEADER *)vpd;
	id	= (ID_HEADER *)(vh + 1);
	is	= (uint8_t *)(id + 1);
	bl	= (BLOCK_LIMITS *)(vh+1);
	bdc	= (BLOCK_DEVICE_CHARACTERISTICS *)(vh+1);
	memset( vpd, 0, ccb->cam_dxfer_len );

	switch( ccb->cam_cdb_io.cam_cdb_bytes[2] ) {
		case INQ_PC_VPD_SUPPORTED: {
			uint8_t		evp_sup[] = { D_DIR_ACC, INQ_PC_VPD_SUPPORTED, 0, 3,
							INQ_PC_VPD_SUPPORTED, INQ_PC_BLOCK_LIMITS,
							INQ_PC_BLOCK_DEVICE_CHARACTERISTICS };

			if( ccb->cam_dxfer_len < sizeof( evp_sup ) ) {
				return( mmc_sense( hba, SK_ILLEGAL, ASC_INVALID_FIELD, 0 ) );
			}
			memcpy( vpd, evp_sup, sizeof( evp_sup ) );
			break;
		}

		case INQ_PC_BLOCK_LIMITS:
			if( ccb->cam_dxfer_len < ( sizeof( *vh ) + sizeof( *bl ) ) ) {
				return( mmc_sense( hba, SK_ILLEGAL, ASC_INVALID_FIELD, 0 ) );
			}
			vh->peripheral		= D_DIR_ACC;
			vh->page_code		= INQ_PC_BLOCK_LIMITS;
			vh->page_length		= ENDIAN_BE16( 60 );

			lps = 1;
			bl->optimal_xfer_len_granularity	= ENDIAN_BE16( lps );

			if( ext->eflags & MMC_EFLAG_TRIM ) {
				bl->max_unmap_lba_count			= ENDIAN_BE32( MMC_TRIM_MAX_LBA );
				bl->max_unmap_desc_count		= ENDIAN_BE32( 1 );
				bl->optimal_unmap_granularity	= ENDIAN_BE32( 1 );
			}
			break;

		case INQ_PC_BLOCK_DEVICE_CHARACTERISTICS:
			if( ccb->cam_dxfer_len < ( sizeof( *vh ) + sizeof( *bdc ) ) ) {
				return( mmc_sense( hba, SK_ILLEGAL, ASC_INVALID_FIELD, 0 ) );
			}
			vh->peripheral				= D_DIR_ACC;
			vh->page_code				= INQ_PC_BLOCK_DEVICE_CHARACTERISTICS;
			vh->page_length				= ENDIAN_BE16( 60 );

			bdc->medium_rotation_rate	= ENDIAN_BE16( 1 );		// 1 indicates non rotating
			bdc->nominal_form_factor	= 0;

			break;

		default:
			return( mmc_sense( hba, SK_ILLEGAL, ASC_INVALID_FIELD, 0 ) );
			break;
	}

	ccb->cam_ch.cam_status	= CAM_REQ_CMP;
	ccb->cam_scsi_status	= SCS_GOOD;
	return( ccb->cam_ch.cam_status );
}

int mmc_std_inquiry( SIM_HBA *hba, CCB_SCSIIO *ccb )
{
	SIM_MMC_EXT		*ext;
	SCSI_INQUIRY	*iptr;
	char			buf[8];

	ext		= (SIM_MMC_EXT *)hba->ext;
	iptr	= (SCSI_INQUIRY *)ccb->cam_data.cam_data_ptr;

	memset(iptr, 0, sizeof(*iptr));

	iptr->peripheral	= D_DIR_ACC;
	iptr->rmb			= CAM_TRUE;
	iptr->version		= INQ_VER_SPC3;			// SPC-3
	iptr->adlen			= 32;

	if (ext->version < MMC_VERSION_1) {
		/* Vendor ID */
		strcpy((char *)&iptr->vend_id[0], "SD:");
		ultoa(ext->cid.sd_cid.mid, buf, 10);
		iptr->vend_id[3] = buf[0];
		iptr->vend_id[4] = buf[1];
		iptr->vend_id[5] = buf[2];

		/* Product ID */
		strcpy((char *)&iptr->prod_id[0], (char *)ext->cid.sd_cid.pnm);

		/* Product revision level, BCD code */
		iptr->prod_rev[0] = (ext->cid.sd_cid.prv >> 4) + '0';
		iptr->prod_rev[1] = '.';
		iptr->prod_rev[2] = (ext->cid.sd_cid.prv & 0x0F) + '0';
	} else {
		strcpy((char *)&iptr->vend_id[0], "MMC:");

		if (ext->csd.mmc_csd.mmc_prot < 2) {
			ultoa(ext->cid.mmc_cid.mid >> 16, buf, 10);

			/* Product revision level, BCD code */
			iptr->prod_rev[0] = ext->cid.mmc_cid.hwr + '0';
			iptr->prod_rev[1] = '.';
			iptr->prod_rev[2] = ext->cid.mmc_cid.fwr + '0';
		} else {
			ultoa(ext->cid.mmc_cid.mid, buf, 10);
		}
		iptr->vend_id[4] = buf[0];
		iptr->vend_id[5] = buf[1];
		iptr->vend_id[6] = buf[2];
		/* Product ID */
		strcpy((char *)&iptr->prod_id[0], (char *)ext->cid.mmc_cid.pnm);

		// Vendor Specific
		ultoa(ext->cid.mmc_cid.psn, (char *)&iptr->vend_spc[0], 16);
	}

	ccb->cam_ch.cam_status = CAM_REQ_CMP;
	ccb->cam_scsi_status   = SCS_GOOD;
	return( ccb->cam_ch.cam_status );
}

int mmc_fake_inquiry( SIM_HBA *hba, CCB_SCSIIO *ccb )
{
	SIM_MMC_EXT		*ext;
	SCSI_INQUIRY	*iptr;

	ext		= (SIM_MMC_EXT *)hba->ext;
	iptr	= (SCSI_INQUIRY *)ccb->cam_data.cam_data_ptr;

	memset(iptr, 0, sizeof(*iptr));
	iptr->peripheral	= D_DIR_ACC;
	iptr->rmb			= CAM_TRUE;
	iptr->version		= INQ_VER_SPC3;			// SPC-3
	iptr->adlen			= 32;
	strcpy((char *)&iptr->vend_id[0], "MMCSD");
	ccb->cam_ch.cam_status = CAM_REQ_CMP;
	ccb->cam_scsi_status   = SCS_GOOD;
	return( ccb->cam_ch.cam_status );
}


int mmc_inquiry( SIM_HBA *hba, CCB_SCSIIO *ccb )
{
	SIM_MMC_EXT		*ext;

	ext		= (SIM_MMC_EXT *)hba->ext;

	if( ext->state == MMCSD_STATE_IDENT ) {
		return( ccb->cam_ch.cam_status );
	}

	if( ( ext->eflags & MMC_EFLAG_READY ) ) {
		if( ccb->cam_cdb_io.cam_cdb_bytes[1] & INQ_OPT_EVPD ) {
			mmc_evpd_inquiry( hba, ccb );
		}
		else {
			mmc_std_inquiry( hba, ccb );
		}
		return( ccb->cam_ch.cam_status );
	}
	if(!ext->normval ){
		return( mmc_sense( hba, SK_NOT_RDY, ASC_MEDIA_NOT_PRESENT, 0 ) );
	}else{
		return( mmc_fake_inquiry( hba, ccb ) );
	}
}

// this routine is only needed if the driver reports a not ready condition
static void mmc_spindle(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	CCB_SCSIIO	*ccb;

	ext = (SIM_MMC_EXT *)hba->ext;
	ccb = ext->nexus;

	if (!(ext->eflags & MMC_EFLAG_READY)) {
		mmc_sense(hba, SK_NOT_RDY, ASC_MEDIA_NOT_PRESENT, 0);
		return;
	}

	switch (ccb->cam_cdb_io.cam_cdb_bytes[4]) {
		case LD_CMD_STOP:
		case LD_CMD_START:
			ccb->cam_ch.cam_status	= CAM_REQ_CMP;
			ccb->cam_scsi_status	= SCS_GOOD;
			break;
		case LD_CMD_EJECT:
		case LD_CMD_LOAD:
		default:
			mmc_sense(hba, SK_ILLEGAL, 0x24, 0);
			break;
	}
}

static void mmc_capacity(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	READ_CAPACITY	*cptr;
	CCB_SCSIIO		*ccb;
	uint32_t		elba;

	ext		= (SIM_MMC_EXT *)hba->ext;
	ccb		= ext->nexus;
	cptr	= (READ_CAPACITY *)ccb->cam_data.cam_data_ptr;

	if (ccb->cam_ch.cam_flags & CAM_DATA_PHYS) {
		mmc_error(hba, CAM_PROVIDE_FAIL);
		return;
	}

	if (!(ext->eflags & MMC_EFLAG_READY)) {
		mmc_sense(hba, SK_NOT_RDY, ASC_MEDIA_NOT_PRESENT, 0);
		return;
	}

	if (ext->state == MMCSD_STATE_IDENT)
		return;

	elba					= ext->partitions[ccb->cam_ch.cam_target_lun].nlba - 1;
	cptr->lba				= ENDIAN_BE32(elba);
	cptr->blk_size			= ENDIAN_BE32(ext->mmc_blksize);

	ccb->cam_ch.cam_status	= CAM_REQ_CMP;
	ccb->cam_scsi_status	= SCS_GOOD;
}

static int mmc_get_serialnumber( SIM_HBA *hba, CCB_DEVCTL *ccb )
{
	SIM_MMC_EXT *ext;
	char		*dsn;

	ext 	= (SIM_MMC_EXT *)hba->ext;

	if (!(ext->eflags & MMC_EFLAG_READY)) {
		mmc_sense(hba, SK_NOT_RDY, ASC_MEDIA_NOT_PRESENT, 0);
		return( CAM_REQ_CMP_ERR );
	}

	dsn = (char *)(ccb->cam_devctl_data);
	memset( dsn, 0, sizeof( ccb->cam_devctl_size ) );
	if (ext->version < MMC_VERSION_1) { //SD
		snprintf( dsn, ccb->cam_devctl_size, "%08x", ext->cid.sd_cid.psn);
	}else{ // MMC
		snprintf( dsn, ccb->cam_devctl_size, "%08x", ext->cid.mmc_cid.psn);
	}
	return( CAM_REQ_CMP );
}

static int mmc_get_cid( SIM_HBA *hba, CCB_DEVCTL *ccb )
{
	SIM_MMC_EXT *ext;
	MMCSD_CID	*cid;

	ext 	= (SIM_MMC_EXT *)hba->ext;
	cid= (MMCSD_CID *)ccb->cam_devctl_data;
	
	cid->pid = getpid();
	cid->flags &= MMCSD_FULL_CID;
	cid->media_change = ext->media_change;
	strncpy(cid->slotname, ext->slotname, MMCSD_MAX_SLOTNAME_LEN);
	if(!(ext->eflags & MMC_EFLAG_INSERTED)){
		cid->flags |= MMCSD_DEV_NO_MEDIA;
	}else{
		if(ext->eflags & MMC_EFLAG_CARD_PRELOAD)
			cid->flags |= MMCSD_DEV_PRELOAD;
		
		if(ext->eflags & MMC_EFLAG_WP)
			cid->flags |= MMCSD_DEV_RDONLY;

		if(ext->eflags & MMC_EFLAG_INVALID_CARD){
			cid->flags |= MMCSD_DEV_MEDIA_ERROR;
			return( CAM_REQ_CMP );
		}
		if(ext->eflags & MMC_EFLAG_READY){
			cid->flags |= MMCSD_DEV_RDY;
			cid->speed = ext->speed;
			cid->hwspec_version = ext->version;
			
			if(cid->flags==MMCSD_FULL_CID){
				memcpy(cid->cid.full_cid.cid, ext->raw_cid, 16);
			}else{
				if (ext->version < MMC_VERSION_1) { //SD
					cid->type = MMCSD_CARD_TYPE_SD;
					cid->csd_version = ext->csd.sd_csd.csd_structure;
					cid->mmcprot_version = 0;
					memcpy((void *)&cid->cid.parsed_cid.sd_cid, (void *)&ext->cid.sd_cid, sizeof(sd_cid_t));
				}else{ //MMC
					cid->type = MMCSD_CARD_TYPE_MMC;
					cid->csd_version = ext->csd.mmc_csd.csd_structure;
					cid->mmcprot_version = ext->csd.mmc_csd.mmc_prot;
					memcpy((void *)&cid->cid.parsed_cid.mmc_cid, (void *)&ext->cid.mmc_cid, sizeof(mmc_cid_t));
				}
			}
		}
	}

	return( CAM_REQ_CMP );
}

static int mmc_wp( SIM_HBA *hba, CCB_DEVCTL *ccb )
{
	SIM_MMC_EXT			*ext;
	MMCSD_WRITE_PROTECT	*wp;
	int					lun;

	ext = (SIM_MMC_EXT *)hba->ext;
	wp	= (MMCSD_WRITE_PROTECT *)ccb->cam_devctl_data;
	lun	= ccb->cam_ch.cam_target_lun;

	if( wp->nlba < ext->partitions[lun].slba ) {
		if( mmc_write_protect( hba, wp->action, ext->partitions[lun].slba + wp->lba, wp->nlba ) ) {
			ccb->cam_devctl_status = EIO;
		}
	}
	else {
		ccb->cam_devctl_status = EINVAL;
	}

	return( CAM_REQ_CMP );
}

static int mmc_get_csd( SIM_HBA *hba, CCB_DEVCTL *ccb )
{
	SIM_MMC_EXT *ext;
	MMCSD_CSD   *csd;

	ext = (SIM_MMC_EXT *)hba->ext;
	csd = (MMCSD_CSD *)ccb->cam_devctl_data;

	csd->pid = getpid();
	csd->flags &= MMCSD_FULL_CID;

	if (!(ext->eflags & MMC_EFLAG_INSERTED)) {
		csd->flags |= MMCSD_DEV_NO_MEDIA;
	} else {
		if (ext->eflags & MMC_EFLAG_CARD_PRELOAD)
			csd->flags |= MMCSD_DEV_PRELOAD;

		if (ext->eflags & MMC_EFLAG_WP)
			csd->flags |= MMCSD_DEV_RDONLY;

		if (ext->eflags & MMC_EFLAG_INVALID_CARD) {
			csd->flags |= MMCSD_DEV_MEDIA_ERROR;
			return (CAM_REQ_CMP);
		}

		if (ext->eflags & MMC_EFLAG_READY) {
			csd->flags |= MMCSD_DEV_RDY;
			if (ext->version < MMC_VERSION_1) { //SD
				csd->type = MMCSD_CARD_TYPE_SD;
				csd->csd_version = ext->csd.sd_csd.csd_structure;
				csd->mmcprot_version = 0;
				memcpy((void *)&csd->csd.sd_csd, (void*)&ext->csd.sd_csd, sizeof(sd_csd_t));
			} else { //MMC
				csd->type = MMCSD_CARD_TYPE_MMC;
				csd->csd_version = ext->csd.mmc_csd.csd_structure;
				csd->mmcprot_version = ext->csd.mmc_csd.mmc_prot;
				memcpy((void *)&csd->csd.mmc_csd, (void*)&ext->csd.mmc_csd, sizeof(mmc_csd_t));
			}
		}
	}

	return( CAM_REQ_CMP );
}

static void mmc_devctl(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext = (SIM_MMC_EXT *)hba->ext;
	CCB_DEVCTL	*ccb = (CCB_DEVCTL *)ext->nexus;

	ccb->cam_devctl_status = EOK;
	switch (ccb->cam_devctl_dcmd) {
		case DCMD_MMCSD_GET_CID:
			ccb->cam_ch.cam_status = mmc_get_cid( hba, ccb );
			break;
		case DCMD_MMCSD_WRITE_PROTECT:
			ccb->cam_ch.cam_status = mmc_wp( hba, ccb );
			break;
		case DCMD_MMCSD_GET_CSD:
			ccb->cam_ch.cam_status = mmc_get_csd(hba, ccb);
			break;
		case DCMD_CAM_DEV_SERIAL_NUMBER:
			ccb->cam_ch.cam_status = mmc_get_serialnumber( hba, ccb );
			break;
		default:
			ccb->cam_devctl_status = _RESMGR_DEFAULT;
			break;
	}
	ccb->cam_ch.cam_status = CAM_REQ_CMP;
}

paddr_t mmc_mphys(CCB_SCSIIO *ccb, paddr_t dptr, int sgi)
{
	mdl_t	*mdl;
	int		cnt;
	ioreq_t	*ioreq;
	off64_t	_off;

	if (ccb->cam_ch.cam_flags & CAM_DATA_PHYS)
		return (dptr);

	if ((ioreq = (ioreq_t *)ccb->cam_req_map)) {
		mdl = ioreq->mdl;
		if (mdl[sgi].vaddr == (caddr_t)dptr)
			return (mdl[sgi].paddr);
 
		for (cnt = ioreq->nmdl; cnt; cnt--, mdl++) {
			if (mdl->vaddr == (caddr_t)dptr)
				return (mdl->paddr);
		}
	}

	mem_offset64((void *)(dptr), NOFD, 1, &_off, NULL);

	return ((paddr_t)_off);
}

static int mmc_stop_xfer(SIM_HBA *hba)
{
	return mmc_sendcmd(hba, MMC_STOP_TRANSMISSION, MMC_RSP_R1B, MMC_CMD_INTR, 0);
}

static int mmc_pio_xfer(SIM_HBA *hba, uint32_t nbytes)
{
	SIM_MMC_EXT	*ext;

	ext	= (SIM_MMC_EXT *)hba->ext;

	if(mmc_wait_card_ready(hba)!=MMC_SUCCESS)
		return 0;

	if (ext->setup_pio(hba, nbytes, ext->dir) == nbytes)
		return mmc_send_rwcmd(hba, nbytes, 0);

	return 0;
}

static int mmc_dma_xfer(SIM_HBA *hba, paddr_t paddr, uint32_t nbytes)
{
	SIM_MMC_EXT	*ext;

	ext	= (SIM_MMC_EXT *)hba->ext;

	if(mmc_wait_card_ready(hba)!=MMC_SUCCESS)
		return 0;

	if ((nbytes = ext->setup_dma(hba, paddr, nbytes, ext->dir)) > 0)
		return mmc_send_rwcmd(hba, nbytes, MMC_CMD_DATA_DMA);

	return 0;
}

static void mmc_dma_rw(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	CCB_SCSIIO	*ccb;
	uint32_t	nbytes;

	ext	= (SIM_MMC_EXT *)hba->ext;
	ccb	= ext->nexus;

	if (ext->nbytes == ext->offset) {	// All done
		ccb->cam_ch.cam_status = CAM_REQ_CMP;
		ccb->cam_scsi_status   = SCS_GOOD;
		return;
	}

	if ((nbytes = mmc_dma_xfer(hba, ext->paddr + ext->offset, ext->nbytes - ext->offset)) > 0) {
		ext->offset += nbytes;
		ext->blkno  += nbytes / ext->mmc_blksize;
	} else
		mmc_error(hba, CAM_PROVIDE_FAIL);
}

static inline void mmc_pio_initiate(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	CCB_SCSIIO	*ccb;
	uint32_t	blkno, nbytes = 0;

	ext		= (SIM_MMC_EXT *)hba->ext;
	ccb		= ext->nexus;
	blkno	= ext->blkno;

	if (ccb->cam_ch.cam_flags & CAM_DATA_PHYS) {
		mmc_error(hba, CAM_PROVIDE_FAIL);
		return;
	}

	if (ccb->cam_ch.cam_flags & CAM_SCATTER_VALID) {
		SG_ELEM		*sge = ccb->cam_data.cam_sg_ptr;
		uint32_t	sgi;

		for (sgi = 0; sgi < ccb->cam_sglist_cnt; sgi++, sge++)
			nbytes += sge->cam_sg_count;
	} else
		nbytes = ccb->cam_dxfer_len;

	ext->nbytes = nbytes;
	if (mmc_pio_xfer(hba, nbytes) > 0) {
		ext->state = MMCSD_STATE_DATA;
		ccb->cam_ch.cam_status = CAM_REQ_INPROG;
		return;
	}

	mmc_error(hba, CAM_PROVIDE_FAIL);
}

static inline void mmc_dma_initiate(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext;
	CCB_SCSIIO	*ccb;
	uint32_t	nbytes;

	ext	= (SIM_MMC_EXT *)hba->ext;
	ccb	= ext->nexus;

	if (ccb->cam_ch.cam_flags & CAM_SCATTER_VALID) {
		if (ext->setup_sgdma) {
			if(mmc_wait_card_ready(hba)!=MMC_SUCCESS){
				mmc_error(hba, CAM_PROVIDE_FAIL);
				return ;
			}

			if ((nbytes = ext->setup_sgdma(hba, ext->dir))) {
				if (mmc_send_rwcmd(hba, nbytes, MMC_CMD_DATA_DMA) == nbytes) {
					ccb->cam_ch.cam_status = CAM_REQ_INPROG;
					ext->state = MMCSD_STATE_DATA;
					ext->nbytes = ext->offset = nbytes;	// all done

					return;
				}
			}
		}

		mmc_error(hba, CAM_PROVIDE_FAIL);
	} else {
		ccb->cam_ch.cam_status = CAM_REQ_INPROG;

		ext->paddr = mmc_mphys(ccb, ccb->cam_data.cam_data_ptr, 0);
		ext->nbytes = ccb->cam_dxfer_len;

		mmc_dma_rw(hba);

		if (ccb->cam_ch.cam_status == CAM_REQ_INPROG)
			ext->state = MMCSD_STATE_DATA;
	}
}

static void mmc_pio_stop(SIM_HBA *hba, int dir)
{
	SIM_MMC_EXT	*ext;
	CCB_SCSIIO	*ccb;

	ext = (SIM_MMC_EXT *)hba->ext;
	ccb = ext->nexus;

	if (ext->dir == dir) {
		if (ext->stop == MMCSD_STOP_PEND) {
			if (mmc_stop_xfer(hba) != MMC_SUCCESS) {
				mmc_error(hba, CAM_PROVIDE_FAIL);
				ext->stop = MMCSD_STOP_NONE;
				return;
			}

			ext->stop = MMCSD_STOP_ISSUED;
		} else {
			ccb->cam_ch.cam_status = CAM_REQ_CMP;
			ccb->cam_scsi_status   = SCS_GOOD;
		}
	}
}

static void mmc_pio_rw(SIM_HBA *hba, int status)
{
	SIM_MMC_EXT	*ext;
	CCB_SCSIIO	*ccb;
	uint32_t	nbytes;

	ext = (SIM_MMC_EXT *)hba->ext;
	ccb = ext->nexus;

	if (ext->offset == -1)
		return mmc_pio_stop(hba, MMC_DIR_OUT);

	if (ccb->cam_ch.cam_flags & CAM_SCATTER_VALID) {
		SG_ELEM		*sge = ccb->cam_data.cam_sg_ptr;
		uint32_t	sgi = ext->cur_sg;

		do {
			nbytes = ext->pio_done(hba, (char *)sge[sgi].cam_sg_address + ext->offset,
						sge[sgi].cam_sg_count - ext->offset, ext->dir);
			ext->offset += nbytes;
			if (ext->offset == sge[sgi].cam_sg_count) {
				if (++sgi >= ccb->cam_sglist_cnt)		// All done
					ext->offset = -1;
				else
					ext->offset = 0;
			}
		} while (nbytes > 0 && ext->offset != -1);

		ext->cur_sg = sgi;
	} else {
		do {
			nbytes = ext->pio_done(hba, (char *)ccb->cam_data.cam_data_ptr + ext->offset,
						ccb->cam_dxfer_len - ext->offset, ext->dir);

			ext->offset += nbytes;
			if (ext->offset == ext->nbytes)
				ext->offset = -1;
		} while (nbytes > 0 && ext->offset != -1);
	}

	if (ext->offset == -1)
		return mmc_pio_stop(hba, MMC_DIR_IN);
}

int mmc_set_partition( SIM_HBA *hba, uint32_t lun )
{
	SIM_MMC_EXT		*ext;

	ext = (SIM_MMC_EXT *)hba->ext;

	if( ext->pactive == lun ) {
		return( CAM_SUCCESS );
	}

	if( mmc_switch( hba, 1, MMC_SWITCH_CMDSET_DFLT, MMC_SWITCH_MODE_WRITE, EXT_CSD_PART_CONFIG, ext->partitions[lun].config ) == MMC_SUCCESS ) {
		ext->pactive = lun;
		return( CAM_SUCCESS );
	}

	return( MMC_FAILURE );
}

void mmc_write_same( SIM_HBA *hba, CCB_SCSIIO *ccb )
{
	SIM_MMC_EXT		*ext;
	CDB				*cdb;
	uint64_t		lba;
	uint32_t		nlba;

	ext						= (SIM_MMC_EXT *)hba->ext;
	cdb						= cam_cdb( ccb );
	ccb->cam_ch.cam_status	= CAM_REQ_CMP;

	lba		= ENDIAN_BE64( UNALIGNED_RET64( cdb->write_same16.lba ) );
	nlba	= ENDIAN_BE32( UNALIGNED_RET32( cdb->write_same16.transfer_len ) );

	if( !( ext->eflags & MMC_EFLAG_TRIM ) || !( cdb->write_same16.opt & WS_OPT_UNMAP ) ) {
		mmc_sense( hba, SK_ILLEGAL, ASC_INVALID_FIELD, 0 );
		return;
	}

#if 0
	if( ext->devid == MMC_SANDISK_INAND ) {
			// INAND require a vendor unique command for TRIM
		if( mmc_switch( hba, 1, MMC_SWITCH_CMDSET_DFLT, MMC_SWITCH_MODE_WRITE, 113, 1 ) ) {
			mmc_sense( hba, SK_ILLEGAL, ASC_INVALID_FIELD, 0 );
			return;
		}
	}
#endif

	if( mmc_erase( hba, MMC_ERASE_TRIM, lba, nlba ) ) {
		mmc_sense( hba, SK_ILLEGAL, ASC_INVALID_FIELD, 0 );
	}

#if 0
	if( ext->devid == MMC_SANDISK_INAND ) {
		if( mmc_switch( hba, 1, MMC_SWITCH_CMDSET_DFLT, MMC_SWITCH_MODE_WRITE, 113, 0 ) ) {
			mmc_sense( hba, SK_ILLEGAL, ASC_INVALID_FIELD, 0 );
			return;
		}
	}
#endif
}

// interpret SCSI commands
void mmc_io_cmds(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext = (SIM_MMC_EXT *)hba->ext;
	CCB_SCSIIO	*ccb = ext->nexus;
	uint32_t	blkno;

	if (ext->state == MMCSD_STATE_IDENT)
		return;

	switch (ccb->cam_cdb_io.cam_cdb_bytes[0]) {
		case SC_MSENSE10:
			mmc_msense(hba);
			break;

		case SC_UNIT_RDY:
			mmc_unit_ready(hba);
			break;

		case SC_INQUIRY:
			mmc_inquiry(hba, ccb);
			break;

		case SC_RD_CAP:
			mmc_capacity(hba);
			break;

		case SC_SPINDLE:
			mmc_spindle(hba);
			break;

		case SC_SYNC:
			ccb->cam_ch.cam_status  = CAM_REQ_CMP;
			ccb->cam_scsi_status    = SCS_GOOD;
			break;

		case SC_WRITE10:
			if (ext->eflags & MMC_EFLAG_WP) {		// Write protected
				mmc_sense(hba, SK_DATA_PROT, ASC_WRITE_PROTECTED, 0);
				break;
			}
		case SC_READ10:
			if (ext->eflags & MMC_EFLAG_READY) {
				ext->offset = 0;
				ext->cur_sg = 0;
				ext->dir    = ccb->cam_cdb_io.cam_cdb_bytes[0] == SC_READ10 ? MMC_DIR_IN : MMC_DIR_OUT;
				blkno       = UNALIGNED_RET32(&ccb->cam_cdb_io.cam_cdb_bytes[2]);
				ext->blkno  = ENDIAN_BE32(blkno);

				if( mmc_set_partition( hba, ccb->cam_ch.cam_target_lun ) == MMC_FAILURE ) {
					mmc_error( hba, CAM_PROVIDE_FAIL );
					break;
				}

				if (ext->eflags & MMC_EFLAG_PIO) {
					if (ext->setup_pio)
						mmc_pio_initiate(hba);
					else
						mmc_error(hba, CAM_PROVIDE_FAIL);
				} else {
					if (ext->setup_dma)
						mmc_dma_initiate(hba);
					else
						mmc_error(hba, CAM_PROVIDE_FAIL);
				}
			} else {
				mmc_sense(hba, SK_NOT_RDY, ASC_MEDIA_NOT_PRESENT, 0);
			}
			break;

#ifdef MMC_TRIM_SUP
		case SC_WR_SAME16:
			mmc_write_same( hba, ccb );
			break;
#endif

		default:
			mmc_sense(hba, SK_ILLEGAL, 0x24, 0);
			break;
	}
}

// start executing a new CCB
static void mmc_start_ccb(SIM_HBA *hba)
{
	SIM_MMC_EXT		*ext;
	CCB_SCSIIO		*ccb;

	ext = (SIM_MMC_EXT *)hba->ext;

	do{
	if (ext->nexus && !(simq_ccb_state(ext->nexus, SIM_CCB_QUERY) & SIM_CCB_ABORT))
		return;

	if ((ext->nexus = ccb = simq_ccb_dequeue(hba->simq)) == NULL)
		return;

	switch (ccb->cam_ch.cam_func_code) {
		case XPT_SCSI_IO:
			mmc_io_cmds(hba);
			break;

		case XPT_DEVCTL:
			mmc_devctl(hba);
			break;

		default:
			slogf(_SLOGC_SIM_MMC, _SLOG_ERROR,
					"mmc_start_ccb: unsupported function code %d", ccb->cam_ch.cam_func_code);
			ccb->cam_ch.cam_status	= CAM_REQ_CMP_ERR;
			ccb->cam_scsi_status	= SCS_CHECK;
			break;
	}

	if (ccb->cam_ch.cam_status != CAM_REQ_INPROG) {
		ext->nexus = NULL;
		simq_post_ccb(hba->simq, ccb);
	}
	} while ( ext->nexus == NULL );
}

static void mmc_dma_irq(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext = (SIM_MMC_EXT *)hba->ext;

	if (ext->stop == MMCSD_STOP_PEND) {
		if (mmc_stop_xfer(hba) != MMC_SUCCESS) {
			mmc_error(hba, CAM_PROVIDE_FAIL);
			ext->stop = MMCSD_STOP_NONE;
			return;
		}
		ext->stop = MMCSD_STOP_ISSUED;
		return;
	}

	mmc_dma_rw(hba);
}

static void mmc_ident_irq(SIM_HBA *hba, int intr)
{
	SIM_MMC_EXT	*ext = (SIM_MMC_EXT *)hba->ext;
	CCB_SCSIIO	*ccb = ext->nexus;

	if (mmc_card_ident(hba, intr) == MMC_SUCCESS) {
		if (ext->ident == MMCSD_IDENT_NONE) {	// ident complete
			ext->state = MMCSD_STATE_IDLE;
			if(!(ext->eflags & MMC_EFLAG_READY)){
				atomic_set(&ext->eflags, MMC_EFLAG_READY);
				MsgSendPulse(mmc_ctrl.coid, ext->mmc_priority, MMCSD_PULSE_CARD_READY, 0);

			}
			if (ccb)
				mmc_io_cmds(hba);
		}
	} else {
		ext->state = MMCSD_STATE_IDLE;

		if (ext->detect(hba) == MMC_SUCCESS) {
			// unsupported card
			slogf(_SLOGC_SIM_MMC, _SLOG_ERROR, "Unsupported card inserted!");
			atomic_set(&ext->eflags, MMC_EFLAG_INVALID_CARD);
			ext->powerdown(hba);	// powerdown the card
		} else {
			// card removed before ident is done
			mmc_media_check(hba);
		}

		if (ccb)
			mmc_error(hba, CAM_PROVIDE_FAIL);
	}

}

static void mmc_data_irq(SIM_HBA *hba, int intr)
{
	SIM_MMC_EXT	*ext = (SIM_MMC_EXT *)hba->ext;
	CCB_SCSIIO	*ccb = ext->nexus;
	static int 	stops_retry=0;

	if (ext->stop == MMCSD_STOP_ISSUED) {			// stop command issued ?
		if (intr & MMC_INTR_ERROR) {				// error ?
			slogf(_SLOGC_SIM_MMC, _SLOG_ERROR,
					"mmc_process_interrupt: stop command error %x", intr);
			if(stops_retry++>5 || (mmc_stop_xfer(hba) != MMC_SUCCESS)){
				mmc_error(hba, CAM_PROVIDE_FAIL);
				ext->stop = MMCSD_STOP_NONE;
			}
		} else if (intr & MMC_INTR_COMMAND) {		// command complete ?
			stops_retry = 0;
			ext->stop = MMCSD_STOP_NONE;
			if (ccb->cam_scsi_status == SCS_CHECK) {
				mmc_error(hba, CAM_PROVIDE_FAIL);
			}
			else if (ext->eflags & MMC_EFLAG_PIO) {
				ccb->cam_ch.cam_status = CAM_REQ_CMP;
				ccb->cam_scsi_status   = SCS_GOOD;
			} else
				mmc_dma_irq(hba);
		}
	} else {
		stops_retry = 0;
		if (intr & MMC_INTR_ERROR) {
			slogf(_SLOGC_SIM_MMC, _SLOG_ERROR,
					"mmc_process_interrupt: data error %x", intr);
			if (mmc_stop_xfer(hba) != MMC_SUCCESS) {
				mmc_error(hba, CAM_PROVIDE_FAIL);
				ext->stop = MMCSD_STOP_NONE;
				return;
			}

			ext->stop = MMCSD_STOP_ISSUED;
				// set check condition as a flag for us to mark the
				// request as errored when the stop completes.
			ccb->cam_scsi_status = SCS_CHECK;
		} else if (intr & MMC_INTR_DATA) {
			if (ext->eflags & MMC_EFLAG_PIO)
				return mmc_pio_rw(hba, intr);

			if (ext->dma_done(hba, ext->dir) == MMC_SUCCESS)
				return mmc_dma_irq(hba);

			mmc_error(hba, CAM_PROVIDE_FAIL);
			slogf(_SLOGC_SIM_MMC, _SLOG_ERROR,
					"mmc_process_interrupt: data error %x", intr);
		} else if (intr & (MMC_INTR_RBRDY | MMC_INTR_WBRDY))
			mmc_pio_rw(hba, intr);
	}
}

static void mmc_media_check(SIM_HBA *hba)
{
	SIM_MMC_EXT	*ext = (SIM_MMC_EXT *)hba->ext;

	if (ext->detect(hba) == MMC_SUCCESS) {
		if (!(atomic_set_value(&ext->eflags, MMC_EFLAG_INSERTED) & MMC_EFLAG_INSERTED)) {
			if(ext->normval)
				atomic_set(&ext->eflags, MMC_EFLAG_MEDIA_CHANGED);
			// Insertion
			if (hba->verbosity)
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, "MMCSD : card inserted");
			ext->media_change++;
			MsgSendPulse(mmc_ctrl.coid, ext->mmc_priority, MMCSD_PULSE_CARD_INSERTION, 0);

		}
	} else {
		if (atomic_clr_value(&ext->eflags, MMC_EFLAG_INSERTED) & MMC_EFLAG_INSERTED) {
			if(ext->normval)
				atomic_set(&ext->eflags, MMC_EFLAG_MEDIA_CHANGED);
			// Removal
			if (hba->verbosity)
				slogf(_SLOGC_SIM_MMC, _SLOG_INFO, "MMCSD : card removed");
			atomic_clr(&ext->eflags, MMC_EFLAG_READY | MMC_EFLAG_INVALID_CARD);
			ext->powerdown(hba);	// powerdown the card
			ext->media_change++;
			MsgSendPulse(mmc_ctrl.coid, ext->mmc_priority, MMCSD_PULSE_CARD_REMOVAL, 0);
		}
	}
}

static int mmc_process_interrupt(SIM_HBA *hba, int irq)
{
	SIM_MMC_EXT	*ext = (SIM_MMC_EXT *)hba->ext;
	CCB_SCSIIO	*ccb = ext->nexus;
	uint32_t	intr;

	if ((intr = ext->interrupt(hba, irq, ext->resp_type, ext->mmc_resp)) == 0)
		return 0;

	if (intr & MMC_INTR_CARD) {
		mmc_media_check(hba);

		mmc_error(hba, CAM_PROVIDE_FAIL);
	} else {
		switch (ext->state) {
		case MMCSD_STATE_IDENT:
			mmc_ident_irq(hba, intr);
			break;
		case MMCSD_STATE_DATA:
			mmc_data_irq(hba, intr);
			break;
		default:
			slogf(_SLOGC_SIM_MMC, _SLOG_INFO,
				"MMCSD : unexpected interrupt, state = %d, intr status = %x", ext->state, intr);
			break;
		}
	}

	if (ccb) {
		if (ccb->cam_ch.cam_status != CAM_REQ_INPROG) {
			ext->nexus = NULL;
			ext->state = MMCSD_STATE_IDLE;
			simq_post_ccb(hba->simq, ccb);
		}
	}

	return 0;
}

// the driver thread starts here
static inline void mmc_pulse_handler(SIM_HBA *hba)
{
	struct _pulse	pulse;
	iov_t			iov;
	int				rcvid;
	SIM_MMC_EXT		*ext;

	ext = (SIM_MMC_EXT *)hba->ext;

	SETIOV(&iov, &pulse, sizeof(pulse));

	while (1) {
		if ((rcvid = MsgReceivev(hba->chid, &iov, 1, NULL)) == -1)
			continue;
		switch (pulse.code) {
			case SIM_ENQUEUE:
				mmc_start_ccb(hba);
				break;

			case SIM_INTERRUPT:
				mmc_process_interrupt(hba, hba->cfg.IRQRegisters[0]);
				InterruptUnmask(hba->cfg.IRQRegisters[0], hba->iid);
				break;

			case SIM_DMA_INTERRUPT:
				mmc_process_interrupt(hba, hba->cfg.IRQRegisters[1]);
				if(hba->cfg.NumIRQs > 1) {
					InterruptUnmask(hba->cfg.IRQRegisters[1], ext->iid);
				}
				break;

			case SIM_TIMER:
				// Only check media if card detection interrupt is not supported
				if (!(ext->hccap & MMC_HCCAP_CD_INTR))
					mmc_media_check(hba);
				break;

			default:
				if (rcvid)
					MsgReplyv(rcvid, ENOTSUP, &iov, 1);
				break;
		}

		if (ext->nexus == NULL)
			mmc_start_ccb(hba);
	}
}

void *mmc_driver_thread(void *data)
{ 
	SIM_HBA			*hba;
	SIM_MMC_EXT		*ext;
	struct sigevent	event;

	hba = (SIM_HBA *)data;
	ext = (SIM_MMC_EXT *)hba->ext;

#if (defined(_NTO_VERSION) && (_NTO_VERSION >= 632))
		/* set the interrupt handler thread name */
		pthread_setname_np( NULL, "mmc_pulse_handler" );
#endif

	if (hba->cfg.NumIRQs) {
		SIGEV_PULSE_INIT(&event, hba->coid, ext->mmc_irq_priority, SIM_INTERRUPT, NULL);

		if ((hba->iid = InterruptAttachEvent(hba->cfg.IRQRegisters[0], &event, _NTO_INTR_FLAGS_TRK_MSK)) == -1)
			perror("Interrupt attach failed:");

		if (hba->cfg.NumIRQs > 1) {
			SIGEV_PULSE_INIT(&event, hba->coid, ext->mmc_irq_priority, SIM_DMA_INTERRUPT, NULL);

			if ((ext->iid = InterruptAttachEvent(hba->cfg.IRQRegisters[1], &event, _NTO_INTR_FLAGS_TRK_MSK)) == -1)
				perror("Interrupt attach failed:");
		}
	}

	atomic_set(&ext->eflags, MMC_EFLAG_DRIVER_READY);

	while (1) {
		if (ext->eflags & MMC_EFLAG_MONITOR_READY)
			break;
		delay(1);
	}

	mmc_pulse_handler(hba);

	return (CAM_SUCCESS);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/sim_mmc.c $ $Rev: 657836 $" );
