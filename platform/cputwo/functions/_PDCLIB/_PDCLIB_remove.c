/* _PDCLIB_remove( const char * ) — CPUTwo bare-metal
   No filesystem; always fails with ENOSYS.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <errno.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

int _PDCLIB_remove( const char * pathname )
{
    (void)pathname;
    *_PDCLIB_errno_func() = _PDCLIB_ENOSYS;
    return -1;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    return TEST_RESULTS;
}

#endif
