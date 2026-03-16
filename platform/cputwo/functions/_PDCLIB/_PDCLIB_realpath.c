/* _PDCLIB_realpath( const char * ) — CPUTwo bare-metal
   No filesystem; always returns NULL.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stddef.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

char * _PDCLIB_realpath( const char * path )
{
    (void)path;
    return NULL;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    return TEST_RESULTS;
}

#endif
