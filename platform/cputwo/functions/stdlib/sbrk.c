/* sbrk( intptr_t ) — CPUTwo bare-metal
   Provides a simple static heap pool for dlmalloc.
   Heap size is 512 KiB; adjust _CPUTWO_HEAP_SIZE as needed.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stddef.h>
#include <stdint.h>
#include <errno.h>

#ifndef REGTEST

#define _CPUTWO_HEAP_SIZE ( 512u * 1024u )

static unsigned char _heap_pool[ _CPUTWO_HEAP_SIZE ];
static unsigned char * _heap_ptr = _heap_pool;

void * sbrk( intptr_t increment )
{
    unsigned char * old_ptr = _heap_ptr;
    unsigned char * new_ptr = _heap_ptr + increment;

    if ( new_ptr < _heap_pool || new_ptr > _heap_pool + _CPUTWO_HEAP_SIZE )
    {
        errno = ENOMEM;
        return (void *)-1;
    }

    _heap_ptr = new_ptr;
    return (void *)old_ptr;
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
