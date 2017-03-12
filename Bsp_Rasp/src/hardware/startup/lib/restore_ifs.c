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



#include "startup.h"
#include "restore_ifs.h"

#define		RIFS_DEBUG_LEVEL	1

// Function prototypes
static Elf32_Phdr *rifs_readelf(paddr32_t paddr);
static int rifs_checksum(void *ptr, long len);
static void rifs_init(struct restore_ifs_info *rifs_info);
static int check_rifs_signature(struct restore_ifs_info *rifs_info);
static int check_ifs_signature(struct image_header	*ifs_hdr);

struct restore_ifs_info 	*rifs_info;
struct restore_ifs2_info 	*rifs2_info;
unsigned 					rifs_flag = RIFS_FLAG_NONE;
paddr32_t 					ifs2_paddr_src = 0;
paddr32_t 					ifs2_paddr_dst = 0;
unsigned 					ifs2_size = 0;
unsigned 					mdriver_cksum_max = KILO(500);

// Load a secondary (non-bootable) image file system
void load_ifs2_nonbootable(void)
{
	// Set the location of IFS2 in RAM if the user didn't specify 
	if(!(rifs_flag & RIFS_FLAG_IFS2_DST)) {
		// Find a default location to store our 2nd IFS (must by on a 4K page boundary)
		// NOTE: We assume that the address will be the same everytime.
		// This should OK since the alloc_ram/find_ram algorithm is deterministic.
		ifs2_paddr_dst = alloc_ram(NULL_PADDR, ifs2_size, 0x1000);
	}

	if(debug_flag > RIFS_DEBUG_LEVEL)	
	{
		kprintf("ifs2_paddr_dst: 0x%X\r\n", ifs2_paddr_dst);
	}
	
	// Attempt to restore IFS2 if it is already in RAM	
	if(!(rifs_flag & RIFS_FLAG_IFS2_RESTORE) || rifs_restore_ifs2() == -1) {
		// Normal (full) load of the non-bootable secondary IFS
		rifs_load_ifs2();
	}
}

// Restore a secondary (non-bootable) image files system
int rifs_restore_ifs2(void)
{
	struct image_header		*ifs2_hdr;
	int						status = 0;
	paddr32_t				paddr;
	
	// Allocate memory for the restore IFS2 info.
	// NOTE: We assume that the address will be the same everytime.
	// This should OK since the alloc_ram/find_ram algorithm is deterministic.
	paddr = alloc_ram(NULL_PADDR, sizeof(struct restore_ifs2_info), sizeof(uint64_t));
	rifs2_info = MAKE_1TO1_PTR(paddr);
	
	if(debug_flag > RIFS_DEBUG_LEVEL)	
	{
		kprintf("Restore IFS2 searching for valid IFS in RAM...\n");
		kprintf("rifs2_info PADDR = 0x%X\n", paddr);
		kprintf("rifs2_info ADDR = 0x%X\n", rifs2_info);
	}
	
	// Obtain a pointer to the IFS2 that *may* be in RAM.  At this point, we still 
	// don't know if it is valid or if it is safe to access this data structure.	
	ifs2_hdr = MAKE_1TO1_PTR(ifs2_paddr_dst);	
	
	// Determine if there is already an IFS2 in RAM and if the restore 
	// information stored from the last boot is valid.
	if(check_ifs_signature(ifs2_hdr) == 0 && rifs_checksum(rifs2_info, sizeof(struct restore_ifs2_info)) == 0)
	{
		// At this point, we know that the IFS2 signature is valid and the 
		// restore info is valid.  We still can't be 100% sure that the IFS is
		// valid until we peform a checksum on the IFS2.
		
		if(debug_flag > RIFS_DEBUG_LEVEL)	
		{
			kprintf("FOUND valid IFS2 signature and RIFS2 info in RAM.\n");
		}
		
		// Determine if we should checksum the IFS		
		if((rifs_flag & RIFS_FLAG_IFS2_CKSUM))
		{
			// Checksum the entire IFS to make sure it hasn't been corrupted.
			if(rifs_checksum(ifs2_hdr, rifs2_info->image_size) != 0)
			{
				if(debug_flag > RIFS_DEBUG_LEVEL)	
				{
					kprintf("WARNING: Checksum failed on IFS2!\n");
				}
				// Checksum failed - IFS is corrupt
				status = -1;
			}
		}
		else
		{
			if(debug_flag > RIFS_DEBUG_LEVEL)	
			{
				kprintf("WARNING: Skipped IFS2 checksum verification\n");
			}
		}
	}
	else
	{	
		// No IFS2 or invalid restore data
		status = -1;
	}
	
	if((status == -1) && (debug_flag > RIFS_DEBUG_LEVEL))
	{
		kprintf("Restore IFS2 failed - Reload entire IFS2.\n");
	}
	
	return(status);
}

// Load a secondary (non-bootable) image files system
int rifs_load_ifs2(void)
{
	struct image_header		*ifs2_hdr;

	ifs2_hdr = MAKE_1TO1_PTR(ifs2_paddr_dst);	
	
	// Set the default source location of IFS2 if the user didn't specify 
	if(!(rifs_flag & RIFS_FLAG_IFS2_SRC)) {
		// Look for 2nd IFS following directly after first IFS	
		ifs2_paddr_src = shdr->imagefs_paddr + shdr->stored_size - shdr->startup_size;
	}
	
	if(debug_flag > RIFS_DEBUG_LEVEL)	
	{
		kprintf("ifs2_paddr_src: 0x%X\r\n", ifs2_paddr_src);
		kprintf("ifs2_paddr_src (auto): 0x%X\r\n", shdr->imagefs_paddr + shdr->stored_size - shdr->startup_size);
	}
	
	// Reserve space for our 2nd IFS if it is a user specified location
	if(rifs_flag & RIFS_FLAG_IFS2_DST)
	{
		// Address must be on a 4K page boundary (handled by options parsing)
		alloc_ram(ifs2_paddr_dst, ifs2_size, 0x1000);	
	}

	// Copy 2nd IFS to RAM
	copy_memory(ifs2_paddr_dst, ifs2_paddr_src, ifs2_size);

	// Save the restore info for the next boot with SDRAM in self-refresh
	if(rifs_flag & RIFS_FLAG_IFS2_RESTORE) {
		char		sig[8] = RIFS_SIGNATURE; 
		int			i;
		
		// Initialize the signature	
		for(i = 0; i < 8; i++)
		{
			rifs2_info->signature[i] = sig[i];
		}
		
		// Save the image size to be used for the image checksum
		rifs2_info->image_size = ifs2_hdr->image_size;
		// Must initialize cksum to 0 before we can calculate cksum on data structure
		rifs2_info->cksum = 0;
	
		// Calculate the restore checksum such that a checksum will result in 0
		rifs2_info->cksum = 0xFFFFFFFF & (0x100000000ULL - rifs_checksum(rifs2_info, sizeof(struct restore_ifs2_info)));
	}
	
	return(0);
}

// Restore an IFS already stored in RAM (i.e. CPU was turned off, RAM was left in self-refresh)
int rifs_restore_ifs(paddr32_t ifs_paddr)
{
	paddr32_t					paddr_dst, paddr_src;
	struct image_header			*ifs_hdr;
	int							status = 0;
	int							i;
	paddr32_t					paddr;
	
	// Allocate memory for the restore ifs info.
	// NOTE: We assume that the address will be the same everytime.
	// This should OK since the alloc_ram/find_ram algorithm is deterministic.
	paddr = alloc_ram(NULL_PADDR, sizeof(struct restore_ifs_info), sizeof(uint64_t));
	rifs_info = MAKE_1TO1_PTR(paddr);
	
	// Obtain a pointer to the IFS that *may* be in RAM.  At this point, we still 
	// don't know if it is valid or if it is safe to access this data structure.	
	ifs_hdr = MAKE_1TO1_PTR(ifs_paddr);	
	
	if(debug_flag > RIFS_DEBUG_LEVEL)	
	{
		kprintf("Restore IFS searching for valid IFS in RAM...\n");
		kprintf("rifs_info PADDR = 0x%X\n", paddr);
		kprintf("rifs_info ADDR = 0x%X\n", rifs_info);
	}
	
	// Determine if there is already an IFS in RAM and if the restore 
	// information stored from the last boot is valid.
	if(check_ifs_signature(ifs_hdr) == 0 && check_rifs_signature(rifs_info) == 0)
	{
		// At this point, we know that the IFS signature is valid and the 
		// restore info is valid.  We still can't be 100% sure that the IFS is
		// valid until we restore the IFS and peform a checksum.
		
		if(debug_flag > RIFS_DEBUG_LEVEL)	
		{
			kprintf("FOUND valid IFS signature and RIFS info in RAM.\n");
			kprintf("IFS pre checksum = 0x%X (should not be 0x0)\n", rifs_checksum(ifs_hdr, rifs_info->image_size));
		}
		
		// Loop through all bootable executables in the image and restore only 
		// the writeable data section to default/original values
		for(i = 0; i < rifs_info->numboot; i++)		
		{
			if(debug_flag > RIFS_DEBUG_LEVEL)	
			{
				kprintf("bootable exec %d offset: 0x%X\r\n", i, rifs_info->elfinfo[i].offset);
				kprintf("bootable exec %d size: 0x%X\r\n", i, rifs_info->elfinfo[i].size);
			}
			
			// Determine location of the executable's data	
			paddr_dst = shdr->image_paddr + shdr->startup_size + rifs_info->elfinfo[i].offset;
			
			// Determine if the image is compressed	
			if(shdr->flags1 & STARTUP_HDR_FLAGS1_COMPRESS_MASK)
			{
				// Compressed image
				if(debug_flag > RIFS_DEBUG_LEVEL)	
				{
					kprintf("Compressed image src = 0x%X\n", rifs_info->elfinfo[i].data);
				}
				// Copy over the previously saved data (dst & src both in the 1-to-1 mapping region)
				// NOTE: Use copy_memory to support mini-drivers
				copy_memory(paddr_dst, rifs_info->elfinfo[i].data, rifs_info->elfinfo[i].size);
			}
			else
			{
				// IFS in uncompressed, we can take the data directly from the source image
				paddr_src = shdr->imagefs_paddr + rifs_info->elfinfo[i].offset;
				if(debug_flag > RIFS_DEBUG_LEVEL)	
				{
					kprintf("Uncompressed image paddr_src = 0x%X\n", paddr_src);
				}
				
				// Copy over data from the original image (dst in the 1-to-1 mapping region, src may be anywhere)
				copy_memory(paddr_dst, paddr_src, rifs_info->elfinfo[i].size);
			}
		}
		
		if(debug_flag > RIFS_DEBUG_LEVEL)	
		{
			kprintf("IFS post checksum = 0x%X (should be 0x0)\n", rifs_checksum(ifs_hdr, rifs_info->image_size));
		}
		
		// Determine if we should checksum the IFS		
		if((rifs_flag & RIFS_FLAG_CKSUM))
		{
			// Checksum the entire IFS to determine if it has been restored correctly and hasn't been corrupted.
			if(rifs_checksum(ifs_hdr, rifs_info->image_size) != 0)
			{
				if(debug_flag > RIFS_DEBUG_LEVEL)	
				{
					kprintf("WARNING: Checksum failed on image!\n");
				}
				// Checksum failed - IFS is corrupt
				status = -1;
			}
		}
		else
		{
			if(debug_flag > RIFS_DEBUG_LEVEL)	
			{
				kprintf("WARNING: Skipped image checksum verification\n");
			}
		}
	}
	else
	{
		// No IFS or invalid restore data
		status = -1;
	}
	
	// Restore info is not initialized until after we have checked the old data
	rifs_init(rifs_info);
	
	if((status == -1) && (debug_flag > RIFS_DEBUG_LEVEL))
	{
		kprintf("Restore IFS failed - Reload entire IFS.\n");
	}
	
	return(status);
}

// Save writeable data section of ELF executables
int rifs_save_elf32data(paddr32_t addr, union image_dirent *dir, int numboot)
{
	Elf32_Phdr 				*phdr;
	
	// Make sure the number of bootable executables isn't greater than the max	
	if(numboot >= RIFS_MAX_BOOTABLE)
		return(-1);
		
	// Read the ELF header	
	if((phdr = rifs_readelf(addr)))
	{
		if(debug_flag > RIFS_DEBUG_LEVEL)	
		{
			kprintf("Found procnto Elf header\n");
			kprintf("bootable exec data: offset %x, size %x\n", phdr->p_offset, phdr->p_filesz );
		}
		// Increment the number of bootable images found and save the related info
		rifs_info->numboot++;
		rifs_info->elfinfo[numboot].offset = dir->file.offset + phdr->p_offset;
		rifs_info->elfinfo[numboot].size = phdr->p_filesz;
	
		// If the image is compressed, save the data 
		if(shdr->flags1 & STARTUP_HDR_FLAGS1_COMPRESS_MASK)
		{
			if(debug_flag > RIFS_DEBUG_LEVEL)	
			{
				kprintf("Compressed image, store data\n");
			}
		
			// Allocate storage space. 
			// NOTE: We assume that the address will be the same everytime.
			// This should OK since the alloc_ram/find_ram algorithm is deterministic.
			rifs_info->elfinfo[numboot].data = alloc_ram(NULL_PADDR, rifs_info->elfinfo[numboot].size, sizeof(uint64_t));
			// Save a copy of the data	
			// NOTE: Use copy_memory to support mini-drivers
			copy_memory(rifs_info->elfinfo[numboot].data, 
					shdr->image_paddr + shdr->startup_size + rifs_info->elfinfo[numboot].offset, 
					rifs_info->elfinfo[numboot].size);
		}
	}
	else
	{
		if(debug_flag > RIFS_DEBUG_LEVEL)	
		{
			kprintf("Invalid ELF header\n");
		}
		// Error reading ELF header
		return(-1);
	}
	
	return(0);
}

// Calculate the checksum for the restore info and save the IFS's size
void rifs_set_cksum(struct image_header *ifs_hdr)
{
	if(debug_flag > RIFS_DEBUG_LEVEL)	
	{
		kprintf("Calculate restore info checksum\n");
	}
	// Save the image size to be used for the image checksum
	rifs_info->image_size = ifs_hdr->image_size;
	
	// Calculate the restore checksum such that a checksum will result in 0
	rifs_info->cksum = 0xFFFFFFFF & (0x100000000ULL - rifs_checksum(rifs_info, sizeof(struct restore_ifs_info)));
}

// Process the ELF header to find the location of the writeable data 
static Elf32_Phdr *rifs_readelf(paddr32_t paddr)
{
	uint8_t			*ptr;
	Elf32_Ehdr		*ehdr;
	Elf32_Phdr		*phdr;
	Elf32_Off		off;
	int				i;
	
	ptr = MAKE_1TO1_PTR(paddr);
	ehdr = (Elf32_Ehdr *)ptr;
	
	// Verify ELF header	
	if(	ehdr->e_ident[EI_MAG0] !=  ELFMAG0 ||
		ehdr->e_ident[EI_MAG1] !=  ELFMAG1 ||
		ehdr->e_ident[EI_MAG2] !=  ELFMAG2 ||
		ehdr->e_ident[EI_MAG3] !=  ELFMAG3 )
	{
		return NULL;
	}
		
	// Search through the ELF header for the writeable data information	
	off = ehdr->e_phoff;
	for(i = 0; i < ehdr->e_phnum; i++)
	{
		// Seek to the next type
		phdr = (Elf32_Phdr *)(ptr + off);
		// Look for PT_LOAD type
		if(phdr->p_type == PT_LOAD)
		{
			// Find PT_LOAD type marked as writeable
			if(phdr->p_flags & PF_W)
			{
				if(debug_flag > RIFS_DEBUG_LEVEL)	
				{
					kprintf("PT_LOAD RW: %x size is %x\n", phdr->p_offset, phdr->p_filesz );
				}
				// Found the match, return it to the caller
				return phdr;
			}
		}
		// Skip to the next type
		off += ehdr->e_phentsize;
	}
	
	return NULL;
}

// Calculate the sum of an array of 4byte numbers
static int rifs_checksum(void *ptr, long len)
{
	int			*data = (int *)ptr;
	long		mdriver_count = 0;
	int 		sum;
	unsigned	max;

	// The checksum may take a while for large images, so we want to poll the mini-driver
	max = (lsp.mdriver.size > 0) ? mdriver_cksum_max : len;
	
	sum = 0;
	while(len > 0)
	{
		sum += *data++;
		len -= 4;
		mdriver_count += 4;
		if(mdriver_count >= max) 
		{
			// Poll the mini-driver when we reach the limit
			mdriver_check();
			mdriver_count = 0;	
		}
	}
	return(sum);
}

// Initialize the restore info data structure
static void rifs_init(struct restore_ifs_info *rifs_info)
{
	char		sig[8] = RIFS_SIGNATURE; 
	int			i;
	
	for(i = 0; i < 8; i++)
	{
		rifs_info->signature[i] = sig[i];
	}
	rifs_info->cksum = 0;
	rifs_info->numboot = 0;
	rifs_info->image_size = 0;
	
	// Zero out bootable executable info	
	for(i = 0; i < RIFS_MAX_BOOTABLE; i++)
	{
		rifs_info->elfinfo[i].offset = 0;
		rifs_info->elfinfo[i].size = 0;
		rifs_info->elfinfo[i].data = 0;
	}
}

// Validate the restore info data structure
static int check_rifs_signature(struct restore_ifs_info *rifs_info)
{
	char	sig[8] = RIFS_SIGNATURE; 
	int		cksum;
	int		i;
	
	// We don't access the restore ifs info pointer until the data structure 
	// is validated with the checksum.
	
	// Verify the signature	
	for(i = 0; i < 8; i++)
	{
		if(sig[i] != *((char *)rifs_info + i))
		{
			if(debug_flag > RIFS_DEBUG_LEVEL)	
			{
				kprintf("INVALID RIFS signature\n");
			}
			// Signature does not match!
			return(-1);
		}
	}
	if(debug_flag > RIFS_DEBUG_LEVEL)	
	{
		kprintf("FOUND valid RIFS signature\n");
	}
	
	// Calculate the checksum on the rifs info data structure	
	cksum = rifs_checksum(rifs_info, sizeof(struct restore_ifs_info));
	if(cksum != 0)
	{
		if(debug_flag > RIFS_DEBUG_LEVEL)	
		{
			kprintf("INVALID RIFS info cksum\n");
		}
		// Invalid checksum, data structure is corrupt
		return(-1);
	}
	
	if(debug_flag > RIFS_DEBUG_LEVEL)	
	{
		kprintf("FOUND valid RIFS info\n");
	}
	// Found valid restore ifs data	
	return(0);
}

// Validate the IFS signature
static int check_ifs_signature(struct image_header	*ifs_hdr)
{
	char	sigifs[7] = IMAGE_SIGNATURE;
	int		i;
	
	// Verify the signature	
	for(i = 0; i < 7; i++)
	{
		if(sigifs[i] != *((char *)ifs_hdr + i))
		{
			if(debug_flag > RIFS_DEBUG_LEVEL)	
			{
				kprintf("INVALID IFS signature\n");
			}
			// Signature does not match!
			return(-1);
		}
	}
	
	if(debug_flag > RIFS_DEBUG_LEVEL)	
	{
		kprintf("FOUND valid IFS signature\n");
	}
	// Found valid IFS signature
	return(0);
}

__SRCVERSION("restore_ifs.c $Rev: 657836 $");
