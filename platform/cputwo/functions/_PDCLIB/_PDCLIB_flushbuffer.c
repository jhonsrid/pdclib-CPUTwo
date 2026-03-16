/* _PDCLIB_flushbuffer( struct _PDCLIB_file_t * ) — CPUTwo bare-metal
   Writes the stream buffer to the UART TX register using MMIO polling.
   FDs 1 (stdout) and 2 (stderr) go to the UART TX port.

   UART MMIO map:
     0x03F00000 — status  (bit 0 = TX ready, bit 1 = RX available)
     0x03F00004 — TX data (write one byte)
     0x03F00008 — RX data (read one byte)

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdio.h>
#include <string.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

/* UART MMIO addresses for CPUTwo. */
#define UART_STATUS  ( *( volatile unsigned int * ) 0x03F00000u )
#define UART_TX_DATA ( *( volatile unsigned int * ) 0x03F00004u )
#define UART_TX_READY 0x1u

static void uart_putc( unsigned char c )
{
    while ( !( UART_STATUS & UART_TX_READY ) )
        ;
    UART_TX_DATA = c;
}

int _PDCLIB_flushbuffer( struct _PDCLIB_file_t * stream )
{
    _PDCLIB_size_t i;

    if ( !( stream->status & _PDCLIB_FBIN ) )
    {
        /* Text mode: no conversion needed for ASCII-only UART. */
    }

    for ( i = 0; i < stream->bufidx; ++i )
    {
        uart_putc( (unsigned char)stream->buffer[i] );
    }

    stream->pos.offset += ( _PDCLIB_int_least64_t )stream->bufidx;
    stream->bufidx = 0;
    return 0;
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
