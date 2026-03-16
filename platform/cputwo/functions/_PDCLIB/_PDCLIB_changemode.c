/* _PDCLIB_changemode( FILE *, unsigned int ) — CPUTwo bare-metal
   Mode changes are not supported; freopen() must close and reopen,
   which also fails (no filesystem). Return INT_MIN to signal failure.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <limits.h>
#include <stddef.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

int _PDCLIB_changemode( struct _PDCLIB_file_t * stream, unsigned int mode )
{
    (void)stream;
    (void)mode;
    return INT_MIN;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    return TEST_RESULTS;
}

#endif
