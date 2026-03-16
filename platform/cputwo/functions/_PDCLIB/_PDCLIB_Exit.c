/* _PDCLIB_Exit( int ) — CPUTwo bare-metal
   Executes a HALT instruction. If running on the host during testing,
   falls back to an infinite loop so the call never returns.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdlib.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

void _PDCLIB_Exit( int status )
{
    (void)status;
#ifdef __cputwo__
    /* Emit the CPUTwo HALT instruction inline. */
    __asm__ volatile ( "halt" );
#endif
    /* Fallback for host builds / if HALT somehow returns. */
    for ( ;; )
        ;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
#ifndef REGTEST
    int UNEXPECTED_RETURN = 0;
    _PDCLIB_Exit( 0 );
    TESTCASE( UNEXPECTED_RETURN );
#endif
    return TEST_RESULTS;
}

#endif
