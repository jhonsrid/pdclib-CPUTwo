/* _PDCLIB_close( _PDCLIB_fd_t ) — CPUTwo bare-metal
   FDs 0/1/2 are the UART and cannot be closed.
   Any other fd is invalid (open() always fails).

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdio.h>
#include <errno.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

int _PDCLIB_close( _PDCLIB_fd_t fd )
{
    (void)fd;
    /* Nothing to close on bare-metal; treat as success. */
    return 0;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    return TEST_RESULTS;
}

#endif
