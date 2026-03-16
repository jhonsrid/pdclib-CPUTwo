/* timespec_get( struct timespec *, int ) — CPUTwo bare-metal
   No real-time clock; always returns 0 (failure).

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <time.h>

#ifndef REGTEST

int timespec_get( struct timespec * ts, int base )
{
    (void)ts;
    (void)base;
    return 0;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    TESTCASE( NO_TESTDRIVER );
    return TEST_RESULTS;
}

#endif
