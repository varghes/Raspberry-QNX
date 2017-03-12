/* Copyright 2009, QNX Software Systems. All Rights Reserved.
 * 
 * You must obtain a written license from and pay applicable license fees to 
 * QNX Software Systems before you may reproduce, modify or distribute this 
 * software, or any work that includes all or part of this software.  Free 
 * development licenses are available for evaluation and non-commercial purposes.  
 * For more information visit http://licensing.qnx.com or email licensing@qnx.com.
 * 
 * This file may contain contributions from others.  Please review this entire 
 * file for other proprietary rights or license notices, as well as the QNX 
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 */
#ifndef _GFPR_GFPR_H_
#define _GFPR_GFPR_H_

#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER)
#if !defined(_MSC_VER) && !defined(__CYGWIN__)
	#include <gfpr/mman.h>
#endif
	#include <stdio.h>
	
	#ifndef __BEGIN_DECLS
		#ifdef __cplusplus
		#define __BEGIN_DECLS	extern "C" {
		#define __END_DECLS		}
		#else
		#define __BEGIN_DECLS
		#define __END_DECLS
		#endif
	#endif
 
	#define ENDIAN_LE16(__x)	(__x)
	#define ENDIAN_LE32(__x)	(__x)
	#define ENDIAN_RET16(__x)		((((__x) >> 8) & 0xff) | \
								(((__x) & 0xff) << 8))
	#define ENDIAN_RET32(__x)		((((__x) >> 24) & 0xff) | \
								(((__x) >> 8) & 0xff00) | \
								(((__x) & 0xff00) << 8) | \
								(((__x) & 0xff) << 24))
	#define UNALIGNED_RET32(__p)	(((uint8_t  *)(__p))[0] | \
								(((uint8_t  *)(__p))[1] << 8) | \
								(((uint8_t  *)(__p))[2] << 16) | \
								(((uint8_t  *)(__p))[3] << 24))

	#define UNALIGNED_RET16(__p) (((uint8_t *)(__p))[0] | \
			(((uint8_t *)(__p))[1] << 8))

	#define UNALIGNED_PUT16(__p,__x) (((uint8_t *)(__p))[0] = (__x) & 0xff, \
			((uint8_t *)(__p))[1] = ((__x) >> 8) & 0xff)

	#define UNALIGNED_PUT32(__p,__x) \
			(((uint8_t *)(__p))[0] = (__x) & 0xff, \
            ((uint8_t *)(__p))[1] = ((__x) >> 8) & 0xff, \
			((uint8_t *)(__p))[2] = ((__x) >> 16) & 0xff, \
			((uint8_t *)(__p))[3] = ((__x) >> 24) & 0xff)

	#define ENDIAN_SWAP16(__x) (*(uint16_t *)(__x) = ENDIAN_RET16(*(uint16_t *) (__x)))
	#define ENDIAN_SWAP32(__x) (*(uint32_t *)(__x) = ENDIAN_RET32(*(uint32_t *) (__x)))
	#define ENDIAN_BE32(__x)        ENDIAN_RET32(__x)
								
	#if !defined(min) && !defined(__cplusplus)
	#define min(a,b)  (((a) < (b)) ? (a) : (b))
	#endif
	#if !defined(max) && !defined(__cplusplus)
	#define max(a,b)  (((a) > (b)) ? (a) : (b))
	#endif
	
	#ifndef NAME_MAX
	#define NAME_MAX 255
	#endif

	#ifndef PATH_MAX
	#define PATH_MAX 256
	#endif
	
	#define RTLD_LOCAL	0
	#define RTLD_NOLOAD	1
	
	#define GFPR_MUTEX_INITIALIZER	NULL
	
	#ifndef EOK
	#define EOK              0  /* No error                                 */
	#endif
	#ifndef ENOTSUP
	#define ENOTSUP         48  /* Not supported (1003.1b-1993)             */
	#endif
	#ifndef ECHRNG
	#define ECHRNG          37  /* Channel number out of range              */
	#endif
	
	#ifndef __PAGESIZE
	#define __PAGESIZE	4096
	#endif
	
	#define __C(a, b)				a + b
	#define _SLOGC_GRAPHICS			1
	#define	_SLOGC_GRAPHICS_GL		2
	#define _SLOG_SHUTDOWN  0   /* Shut down the system NOW. eg: for OEM use */
	#define _SLOG_CRITICAL  1   /* Unexpected unrecoverable error. eg: hard disk error */
	#define _SLOG_ERROR     2   /* Unexpected recoverable error. eg: needed to reset a hw controller */
	#define _SLOG_WARNING   3   /* Expected error. eg: parity error on a serial port */
	#define _SLOG_NOTICE    4   /* Warnings. eg: Out of paper */
	#define _SLOG_INFO      5   /* Information. eg: Printing page 3 */


#ifdef __QNXNTO__
#include <sys/srcversion.h>
__SRCVERSION( "$URL: http://svn/product/tags/internal/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/1.0.0/latest/lib/gfpr/public/gfpr/gfpr.h $ $Rev: 224587 $" )
#endif
	
#ifndef __CYGWIN__
	/* QNX types */
	typedef int	gid_t;
	typedef int	pid_t;
#endif
	
#if defined(__MINGW32__) || defined(__CYGWIN__)
	#ifndef __CYGWIN__
	#include <stdint.h>
	typedef uint16_t	_Uint16t;
	typedef uint64_t	_uint64;
	typedef uint32_t	_uint32;
	typedef uint16_t	_uint16;
	typedef uint8_t		_uint8;
	typedef int64_t		_int64;
	typedef int32_t		_int32;
	typedef int16_t		_int16;
	typedef int8_t		_int8;
	typedef char *      caddr_t;
	typedef uintptr_t	_Uintptrt;
	#endif
	typedef int64_t		paddr64_t;
#elif defined(_MSC_VER) // VC++
#if (_MSC_VER < 1400)
	typedef unsigned long 	uintptr_t;
	typedef long 			intptr_t;
#endif
	typedef long			int32_t;
	typedef unsigned __int64	uint64_t;
	typedef unsigned __int64	_uint64;
	typedef unsigned long	_uint32;
	typedef unsigned long	uint32_t;
	typedef unsigned short	uint16_t;
	typedef short			int16_t;
	typedef unsigned short	_uint16;
	typedef unsigned char	_uint8;
	typedef unsigned char	uint8_t;
	typedef unsigned short	_Uint16t;
	typedef unsigned long	_Uintptrt;
	typedef _int64 int64_t;

#if (_MSC_VER < 1400)
__BEGIN_DECLS
	extern float sqrtf (float);
	extern float powf (float x, float y);
	extern float expf (float x);
	extern float cosf (float);
	extern float sinf (float);
	extern float frexpf (float x, int* expn);
__END_DECLS
#endif

	#define __LITTLEENDIAN__
	#define INFINITY __INFINITY__
	#define INVALID_SET_FILE_POINTER   ((DWORD)-1)
	typedef int ssize_t;
#endif

	/* pthread types */
	typedef int			gfpr_thread_t;
	typedef void *		gfpr_mutex_t;
	typedef int			gfpr_cond_t;
	typedef int			gfpr_thread_key_t;
	
	/* resmgr types */
	typedef void *	iofunc_attr_t;
	typedef void *	iofunc_ocb_t;
	typedef void *	dispatch_t;
	typedef void *	resmgr_context_t;

__BEGIN_DECLS
	
	void 	*gfpr_dlopen(const char *pathname, int mode);
	void 	*gfpr_dlsym(void *handle, const char *name);
	int 	gfpr_dlclose(void *handle);
	char 	*gfpr_dlerror( void );

	char 	*gfpr_fgets(char *buf, int n, FILE *fp);
	int 	gfpr_slogf(int code, int severity, const char *fmt, ...);
	
	int gfpr_thread_create(gfpr_thread_t* thread, void *attr,
		void* (*start_routine)(void* ), void *arg);
	int gfpr_mutex_destroy(gfpr_mutex_t *mutex);
	int gfpr_mutex_init(gfpr_mutex_t *mutex, void *attr);
	int gfpr_mutex_lock(gfpr_mutex_t *mutex);
	int gfpr_mutex_unlock(gfpr_mutex_t *mutex);
	int gfpr_cond_init(gfpr_cond_t* cond, void *attr);
	int gfpr_cond_broadcast( gfpr_cond_t* cond );
	int gfpr_cond_wait(gfpr_cond_t* cond, gfpr_mutex_t* mutex );
	int gfpr_cond_destroy(gfpr_cond_t* cond);
	int gfpr_thread_key_create(gfpr_thread_key_t *key, 
		void (*destructor)(void *));
	int gfpr_thread_key_delete(gfpr_thread_key_t key);
	int gfpr_thread_setspecific(gfpr_thread_key_t key, void *value);
	void *gfpr_thread_getspecific(gfpr_thread_key_t key);
	gfpr_thread_t gfpr_thread_self(void);

__END_DECLS

#else
	//
	// The Windows case is above. This here is for Neutrino.
	//
	#ifndef _PTHREAD_H_INCLUDED
	#include <pthread.h>
	#endif

	#include <dlfcn.h>

	#define GFPR_MUTEX_INITIALIZER		PTHREAD_MUTEX_INITIALIZER
	
	#define gfpr_dlopen				dlopen
	#define gfpr_dlsym				dlsym
	#define gfpr_dlclose			dlclose
	#define gfpr_dlerror			dlerror
	#define gfpr_fgets				fgets
	
	#define gfpr_mutex_init			pthread_mutex_init
	#define gfpr_mutex_destroy		pthread_mutex_destroy
	#define gfpr_mutex_lock			pthread_mutex_lock
	#define gfpr_mutex_unlock		pthread_mutex_unlock
	#define gfpr_thread_key_create	pthread_key_create
	#define gfpr_thread_key_delete	pthread_key_delete
	#define gfpr_thread_setspecific	pthread_setspecific
	#define gfpr_thread_getspecific	pthread_getspecific
	#define	gfpr_thread_create		pthread_create
	#define gfpr_thread_cancel		pthread_cancel
	#define gfpr_thread_join		pthread_join
	#define gfpr_thread_sigmask		pthread_sigmask
	#define gfpr_thread_self		pthread_self
	#define gfpr_cond_init			pthread_cond_init	
	#define gfpr_cond_broadcast		pthread_cond_broadcast
	#define gfpr_cond_wait			pthread_cond_wait
	#define gfpr_cond_destroy		pthread_cond_destroy
	
	typedef pthread_t				gfpr_thread_t;
	typedef pthread_mutex_t			gfpr_mutex_t;
	typedef pthread_cond_t			gfpr_cond_t;
	typedef pthread_key_t			gfpr_thread_key_t;
	
	#define gfpr_mmap				mmap
	#define gfpr_munmap				munmap
	#define gfpr_shm_open			shm_open
	#define gfpr_shm_unlink			shm_unlink
		
	#define gfpr_slogf				slogf
#endif

#endif /* _GF_GFPR_H_ */


#ifdef __QNXNTO__
#include <sys/srcversion.h>
__SRCVERSION( "$URL: http://svn/product/tags/internal/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/1.0.0/latest/lib/gfpr/public/gfpr/gfpr.h $ $Rev: 224587 $" )
#endif
