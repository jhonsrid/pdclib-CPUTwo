/* _PDCLIB_fillbuffer( struct _PDCLIB_file_t * ) — CPUTwo bare-metal
   Reads from the UART RX register using MMIO polling.
   FD 0 (stdin) comes from the UART RX port.

   UART MMIO map:
     0x03F00000 — status  (bit 0 = TX ready, bit 1 = RX available)
     0x03F00004 — TX data (write one byte)
     0x03F00008 — RX data (read one byte)

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdio.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

/* UART MMIO addresses for CPUTwo. */
#define UART_STATUS  ( *( volatile unsigned int * ) 0x03F00000u )
#define UART_RX_DATA ( *( volatile unsigned int * ) 0x03F00008u )
#define UART_RX_AVAIL 0x2u

/* Block until one byte is available, then return it. */
static unsigned char uart_getc( void )
{
    while ( !( UART_STATUS & UART_RX_AVAIL ) )
        ;
    return (unsigned char)( UART_RX_DATA & 0xFFu );
}

int _PDCLIB_fillbuffer( struct _PDCLIB_file_t * stream )
{
    _PDCLIB_size_t i = 0;
    unsigned char c;

    /* Read bytes into the buffer until it is full or we get a newline
       (line-buffered terminal behaviour).
    */
    while ( i < stream->bufsize )
    {
        c = uart_getc();
        stream->buffer[i++] = (char)c;

        if ( c == '\n' )
        {
            break;
        }
    }

    if ( i == 0 )
    {
        stream->status |= _PDCLIB_EOFFLAG;
        return EOF;
    }

    if ( !( stream->status & _PDCLIB_FBIN ) )
    {
        /* Text mode: no conversion needed for ASCII UART. */
    }

    stream->pos.offset += ( _PDCLIB_int_least64_t )i;
    stream->bufend = i;
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
