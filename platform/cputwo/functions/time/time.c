/* time( time_t * ) — CPUTwo bare-metal
   No real-time clock; always returns (time_t)-1.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <time.h>

#ifndef REGTEST

time_t time( time_t * timer )
{
    if ( timer != NULL )
    {
        *timer = (time_t)-1;
    }

    return (time_t)-1;
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
