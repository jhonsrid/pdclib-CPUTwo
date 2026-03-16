/* _PDCLIB_seek( FILE *, int_least64_t, int ) — CPUTwo bare-metal
   The UART is not seekable; always returns ESPIPE.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdio.h>
#include <errno.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

_PDCLIB_int_least64_t _PDCLIB_seek( struct _PDCLIB_file_t * stream,
                                     _PDCLIB_int_least64_t   offset,
                                     int                     whence )
{
    (void)stream;
    (void)offset;
    (void)whence;
    *_PDCLIB_errno_func() = _PDCLIB_ESPIPE;
    return EOF;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    /* Testing covered by ftell.c */
    return TEST_RESULTS;
}

#endif
