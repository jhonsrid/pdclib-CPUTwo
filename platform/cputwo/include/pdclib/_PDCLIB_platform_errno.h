/* _PDCLIB_platform_errno.h — CPUTwo bare-metal
   On CPUTwo there is no OS errno; pdclib's own errno.h IS the platform errno.
   The 1:1 mapping is established in _PDCLIB_config.h.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_PLATFORM_ERRNO
#define _PDCLIB_PLATFORM_ERRNO

/* Include pdclib's own errno.h — it defines errno values from _PDCLIB_config.h. */
#include <errno.h>

#endif
