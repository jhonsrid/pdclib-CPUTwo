/* _PDCLIB_open( const char * const, int ) — CPUTwo bare-metal
   No filesystem: all open() attempts fail with ENOSYS.
   stdin/stdout/stderr (fds 0/1/2) are pre-opened in _PDCLIB_stdinit.c.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdio.h>
#include <errno.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

_PDCLIB_fd_t _PDCLIB_open( const char * const filename, unsigned int mode )
{
    (void)filename;
    (void)mode;
    *_PDCLIB_errno_func() = _PDCLIB_ENOSYS;
    return _PDCLIB_NOHANDLE;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    /* No filesystem on CPUTwo; open always fails. */
    return TEST_RESULTS;
}

#endif
