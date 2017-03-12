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




/*
 * sys/startup.h:	startup header definitions
 *

 *
 */

#ifndef __STARTUP_H_INCLUDED
#define __STARTUP_H_INCLUDED

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

#include _NTO_HDR_(_pack64.h)


/*
** In the comments the following letters tell you who uses the info
**
**   I  - used by the QNX IPL
**   S  - used by the startup program
**   IS - used by both the QNX IPL and the startup program
*/
struct startup_header {
	unsigned long	signature;			/*I  Header sig, see below*/
	unsigned short	version;			/*I  Header vers, see below*/
	unsigned char	flags1;				/*IS Misc flags, see below*/
	unsigned char	flags2;             /*   No flags defined yet*/
	unsigned short	header_size;		/*S  sizeof(struct startup_header)*/
	unsigned short	machine;			/*IS Machine type from sys/elf.h*/
	unsigned long	startup_vaddr;		/*I  Virtual Address to transfer*/
										/*   to after IPL is done*/
	unsigned long	paddr_bias;			/*S  Value to add to physical address*/
										/*   to get a value to put into a*/
										/*   pointer and indirected through*/
	unsigned long	image_paddr;		/*IS Physical address of image*/
	unsigned long	ram_paddr;			/*IS Physical address of RAM to copy*/
										/*   image to (startup_size bytes copied)*/
	unsigned long	ram_size;			/*S  Amount of RAM used by the startup*/
										/*   program and executables contained*/
										/*   in the file system*/
	unsigned long	startup_size;		/*I  Size of startup (never compressed)*/
	unsigned long	stored_size;		/*I  Size of entire image*/
	unsigned long	imagefs_paddr;		/*IS Set by IPL to where the imagefs is when startup runs*/
	unsigned long	imagefs_size;		/*S  Size of uncompressed imagefs*/
	unsigned short	preboot_size;		/*I  Size of loaded before header*/
	unsigned short	zero0;				/*   Zeros */
	unsigned long	zero[3];			/*   Zeros */
	unsigned long	info[48];			/*IS Array of startup_info* structures*/
};

/* We keep the flags as chars so they are endian neutral */
#define STARTUP_HDR_FLAGS1_VIRTUAL			0x01
#define STARTUP_HDR_FLAGS1_BIGENDIAN		0x02
#define STARTUP_HDR_FLAGS1_COMPRESS_MASK	0x1c
#define STARTUP_HDR_FLAGS1_COMPRESS_SHIFT	0x02
#define STARTUP_HDR_FLAGS1_COMPRESS_NONE	0x00
#define STARTUP_HDR_FLAGS1_COMPRESS_ZLIB	0x04
#define STARTUP_HDR_FLAGS1_COMPRESS_LZO		0x08
#define STARTUP_HDR_FLAGS1_COMPRESS_UCL		0x0c

/* All values are stored in target endian format */
#define STARTUP_HDR_SIGNATURE			0x00ff7eeb
#define STARTUP_HDR_VERSION				1

/*
 The following explains some of the fields used by the IPL and Startup
 for various types of boot. These fields are stuffed by mkifs.

LINEAR ROM XIP
==============
                                     
       ROM                                          RAM
 +-------------+ <-image_paddr          +-------------+ <-ram_paddr 
 | startup hdr |      ^       ^         | startup hdr |            ^
 |             |      |       |         |             |            |
 | startup     |   startup    |         | startup     | <-startup  |
 |             |     size     |         |             |   addr    ram
 |             |      |       |         |             |           size
 |             |      v       |         |             |            |
 +-------------+      -     stored      +-------------+            |
 | imagefs hdr |      ^      size       | reserved for|            |
 |             |      |       |         | imagefs data|            |
 | imagefs     |      |       |         +-------------+            v
 |             |   imagefs    |
 |             |     size     |
 |             |      |       |
 |             |      |       |
 +-------------+      v       v

I checksum(image_paddr, startup_size)
I checksum(image_paddr + startup_size, stored_size - startup_size)
I copy(image_paddr, ram_paddr, copy_size)
I jump(startup_vaddr)


Linear ROM Compressed
=====================

       ROM                                        RAM
 +-------------+ <-image_paddr         +-------------+ <-ram_paddr 
 | startup hdr |      ^       ^        | startup hdr |            ^
 |             |      |       |        |             |            |
 | startup     |   startup    |        | startup     | <-startup  |
 |             |     size     |        |             |   addr    ram
 |             |      |       |        |             |           size
 |             |      v       |        |             |            |
 +-------------+      -     stored     +-------------+    -       |
 | compressed  |             size      | imagefs hdr |    ^       |
 | imagefs hdr |              |        |             |    |       |
 | imagefs     |              |        | imagefs     |    |       |
 |             |              |        |             | imagefs    |
 +-------------+              V        |             |   size     |
                                       |             |    |       |
                                       |             |    |       |
                                       +-------------+    v       v

I checksum(image_paddr, startup_size)
I checksum(image_paddr + startup_size, stored_size - startup_size)
I copy(image_paddr, ram_paddr, copy_size)
I jump(startup_vaddr)
S uncompress(ram_paddr + startup_size,
             image_paddr + startup_size, stored_size - startup_size)


                                    
ROM NON-XIP (usually because the ROM is too slow to XIP out of)
===========

       ROM                                  RAM
 +-------------+ <-image_paddr         +-------------+ <-ram_paddr 
 | startup hdr |      ^       ^        | startup hdr |            ^
 |             |      |       |        |             |            |
 | startup     |   startup    |        | startup     | <-startup  |
 |             |     size     |        |             |   addr    ram
 |             |      |       |        |             |           size
 |             |      v       |        |             |            |
 +-------------+      -     stored     +-------------+    -       |
 | imagefs hdr |      ^      size      | imagefs hdr |    ^       |
 |             |      |       |        |             |    |       |
 | imagefs     |      |       |        | imagefs     |    |       |
 |             |   imagefs    |        |             | imagefs    |
 |             |     size     |        |             |   size     |
 |             |      |       |        |             |    |       |
 |             |      |       |        |             |    |       |
 +-------------+      v       v        +-------------+    v       v

I checksum(image_paddr, startup_size)
I checksum(image_paddr + startup_size, stored_size - startup_size)
I copy(image_paddr, ram_paddr, copy_size)
I jump(startup_vaddr)

                                    
Disk/Network
============

       RAM
 +-------------+ 
 | jump ipl    | 
 +++++++++++++++ <-image_paddr  <-ram_paddr       
 | startup hdr |               ^       ^         ^  
 |             |               |       |         |  
 | startup     | <-startup  startup    |         |  
 |             |   addr       size     |        ram 
 |             |               |       |        size
 |             |               v       |         |  
 +-------------+               -     stored      |  
 | imagefs hdr |               ^      size       |  
 |             |               |       |         |  
 | imagefs     |               |       |         |  
 |             |            imagefs    |         |  
 |             |              size     |         |  
 |             |               |       |         |  
 |             |               |       |         |  
 +-------------+               v       v         v  

In this case our full IPL is not involved. The image is loaded into memory
and transfered to by an existing BIOS IPL. Since the existing IPL does not
know where in startup tp jump it always jumps to the start of the image.
We build a tiny IPL on the front of the image which jumps to startup_vaddr.

I jump(startup_vaddr)

                                    
Disk/Network Compressed
=======================

       RAM                                           RAM       
 +-------------+                               +-------------+ <-ram_paddr    
 | jump ipl    |                               |             |
 +++++++++++++++ <-image_paddr                 |             |
 | startup hdr |              ^      ^     ^   | imagefs hdr |    ^
 |             |              |      |     |   |             |    |
 | startup     | <-startup startup   |    ram  | imagefs     |    |
 |             |   addr      size    |    size |             | imagefs  
 |             |              |      |     |   |             |   size
 |             |              v      |     |   |             |    |
 +-------------+              -    stored  |   |             |    |  
 | imagefs hdr |                    size   |   |             |    |  
 |             |                     |     |   |             |    |  
 | imagefs     |                     |     |   |             |    |  
 +-------------+                     v     v   +-------------+    v  
                                                   

In this case our full IPL is not involved. The image is loaded into memory
and transfered to by an existing BIOS IPL. Since the existing IPL does not
know where in startup tp jump it always jumps to the start of the image.
We build a tiny IPL on the front of the image which jumps to startup_vaddr.

I jump(startup_vaddr)
S uncompress(ram2_paddr,
             ram_paddr + startup_size, stored_size - startup_size)


Bank Switched ROM
=================
This is much like a Disk/Network boot except you get to write the code
which copies the image into RAM using.

checksum(image_paddr, startup_size)
checksum(image_paddr + startup_size, stored_size - startup_size)
copy(image_paddr, ram_paddr, copy_size)
jump(startup_vaddr)
goto Disk/Network or Disk/Network Compressed

You will need to map the physical addresses and sizes into bank switching
as needed. Have fun and next time DON'T BANK SWITCH YOUR ROM! Make it linear
in the address space.
*/


enum startup_info_types {
	STARTUP_INFO_SKIP = 0,		/* If size 0 then end of list */
	STARTUP_INFO_MEM,
	STARTUP_INFO_DISK,
	STARTUP_INFO_TIME,
	STARTUP_INFO_BOX,
	STARTUP_INFO_USER = 0x8000	/* User reserved numbers start here */
};

struct startup_info_hdr {
	unsigned short		type;
	unsigned short		size;
};

struct startup_info_skip {
	struct startup_info_hdr	hdr;
};

struct startup_info_mem {
	struct startup_info_hdr	hdr;
	unsigned long			addr;
	unsigned long			size;
};

/*
 * This structure is used when reporting memory >4G. It uses a
 * STARTUP_INFO_MEM type - you can tell the difference between a
 * startup_info_mem entry and this one by the size field.
 */
struct startup_info_mem_extended {
	struct startup_info_mem	mem;
	unsigned long			addr_hi;
	unsigned long			size_hi;
};

struct startup_info_disk {
	struct startup_info_hdr	hdr;
	unsigned char			drive;
	unsigned char			zero;
	unsigned short			heads;
	unsigned short			cylinders;
	unsigned short			sectors;
	unsigned long			blocks;
};

struct startup_info_time {
	struct startup_info_hdr	hdr;
	unsigned long			time;
};

struct startup_info_box {
	struct startup_info_hdr	hdr;
	unsigned char			boxtype;
	unsigned char			bustype;
	unsigned char			spare[2];
};

struct bootargs_entry {
	unsigned char			 size_lo;	/* Includes entire structure */
	unsigned char			 size_hi;
	char					 argc;
	char					 envc;
	unsigned long			 shdr_addr;
	char					 args[1];	/* variable length */
};


#if defined(__LITTLEENDIAN__) || defined(__X86__)
   #define __SC(ch,pos)		((_Uint64t)(ch)<<((pos)*8))
#else
   #define __SC(ch,pos)		((_Uint64t)(ch)<<((7-(pos))*8))
#endif
#define __STACK_SIG (__SC('d',0)+__SC('d',1)+__SC('p',2)+__SC('v',3) \
					+__SC('b',4)+__SC('s',5)+__SC('k',6)+__SC('r',7))

struct startup_trailer {
	unsigned long			cksum;	/* Checksum from start of header to start of trailer */
};

#include _NTO_HDR_(_packpop.h)

#endif /* __STARTUP_H_INCLUDED */

/* __SRCVERSION("startup.h $Rev: 657836 $"); */
