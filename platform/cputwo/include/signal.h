/* signal.h — CPUTwo bare-metal platform
   Signals are software-only: no OS delivers them. raise() is the only source.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_SIGNAL_H
#define _PDCLIB_SIGNAL_H _PDCLIB_SIGNAL_H

#include "pdclib/_PDCLIB_lib_ext1.h"
#include "pdclib/_PDCLIB_internal.h"

/* Signal numbers */
#define SIGABRT 1
#define SIGFPE  2
#define SIGILL  3
#define SIGINT  4
#define SIGSEGV 5
#define SIGTERM 6

/* Special handler values */
#define SIG_DFL ( (void (*)( int )) 0 )
#define SIG_ERR ( (void (*)( int )) -1 )
#define SIG_IGN ( (void (*)( int )) 1 )

typedef _PDCLIB_sig_atomic_t sig_atomic_t;

#ifdef __cplusplus
extern "C" {
#endif

_PDCLIB_PUBLIC void ( *signal( int sig, void ( *func )( int ) ) )( int );
_PDCLIB_PUBLIC int raise( int sig );

#ifdef __cplusplus
}
#endif

#endif
