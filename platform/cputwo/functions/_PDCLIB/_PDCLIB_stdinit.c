/* _PDCLIB_stdinit — CPUTwo bare-metal
   stdin → UART RX (fd 0), stdout → UART TX (fd 1), stderr → UART TX (fd 2).
   No OS, no threads, no filesystem; all locale data is the "C" locale.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdio.h>
#include <locale.h>
#include <limits.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_tzcode.h"

/* -------------------------------------------------------------------------- */
/* Standard streams                                                           */
/* -------------------------------------------------------------------------- */

static char _PDCLIB_sin_buffer[BUFSIZ];
static char _PDCLIB_sout_buffer[BUFSIZ];
static char _PDCLIB_serr_buffer[BUFSIZ];

static struct _PDCLIB_file_t _PDCLIB_serr = {
    2, _PDCLIB_serr_buffer, BUFSIZ, 0, 0, { 0, 0 }, 0, { 0 },
    _IONBF | _PDCLIB_FWRITE,
#ifndef __STDC_NO_THREADS__
    _PDCLIB_MTX_RECURSIVE_INIT,
#endif
    NULL, NULL
};
static struct _PDCLIB_file_t _PDCLIB_sout = {
    1, _PDCLIB_sout_buffer, BUFSIZ, 0, 0, { 0, 0 }, 0, { 0 },
    _IOLBF | _PDCLIB_FWRITE,
#ifndef __STDC_NO_THREADS__
    _PDCLIB_MTX_RECURSIVE_INIT,
#endif
    NULL, &_PDCLIB_serr
};
static struct _PDCLIB_file_t _PDCLIB_sin = {
    0, _PDCLIB_sin_buffer, BUFSIZ, 0, 0, { 0, 0 }, 0, { 0 },
    _IOLBF | _PDCLIB_FREAD,
#ifndef __STDC_NO_THREADS__
    _PDCLIB_MTX_RECURSIVE_INIT,
#endif
    NULL, &_PDCLIB_sout
};

struct _PDCLIB_file_t * stdin  = &_PDCLIB_sin;
struct _PDCLIB_file_t * stdout = &_PDCLIB_sout;
struct _PDCLIB_file_t * stderr = &_PDCLIB_serr;

struct _PDCLIB_file_t * _PDCLIB_filelist = &_PDCLIB_sin;

#ifndef __STDC_NO_THREADS__
_PDCLIB_mtx_t _PDCLIB_filelist_mtx = _PDCLIB_MTX_PLAIN_INIT;
_PDCLIB_mtx_t _PDCLIB_time_mtx     = _PDCLIB_MTX_PLAIN_INIT;
#endif

/* -------------------------------------------------------------------------- */
/* C locale — ASCII-7 ctype table (129 entries: [0]=EOF, [1..128]=0x00..0x7F) */
/* -------------------------------------------------------------------------- */

static struct _PDCLIB_lc_ctype_entry_t _ctype_entries_C[ _PDCLIB_CHARSET_SIZE + 1 ] =
{
    { /* EOF */    0,    0,    0 },
    { /* NUL */ _PDCLIB_CTYPE_CNTRL,                                             0x00, 0x00 },
    { /* SOH */ _PDCLIB_CTYPE_CNTRL,                                             0x01, 0x01 },
    { /* STX */ _PDCLIB_CTYPE_CNTRL,                                             0x02, 0x02 },
    { /* ETX */ _PDCLIB_CTYPE_CNTRL,                                             0x03, 0x03 },
    { /* EOT */ _PDCLIB_CTYPE_CNTRL,                                             0x04, 0x04 },
    { /* ENQ */ _PDCLIB_CTYPE_CNTRL,                                             0x05, 0x05 },
    { /* ACK */ _PDCLIB_CTYPE_CNTRL,                                             0x06, 0x06 },
    { /* BEL */ _PDCLIB_CTYPE_CNTRL,                                             0x07, 0x07 },
    { /*  BS */ _PDCLIB_CTYPE_CNTRL,                                             0x08, 0x08 },
    { /*  HT */ _PDCLIB_CTYPE_CNTRL | _PDCLIB_CTYPE_BLANK | _PDCLIB_CTYPE_SPACE, 0x09, 0x09 },
    { /*  LF */ _PDCLIB_CTYPE_CNTRL | _PDCLIB_CTYPE_SPACE,                       0x0A, 0x0A },
    { /*  VT */ _PDCLIB_CTYPE_CNTRL | _PDCLIB_CTYPE_SPACE,                       0x0B, 0x0B },
    { /*  FF */ _PDCLIB_CTYPE_CNTRL | _PDCLIB_CTYPE_SPACE,                       0x0C, 0x0C },
    { /*  CR */ _PDCLIB_CTYPE_CNTRL | _PDCLIB_CTYPE_SPACE,                       0x0D, 0x0D },
    { /*  SO */ _PDCLIB_CTYPE_CNTRL,                                             0x0E, 0x0E },
    { /*  SI */ _PDCLIB_CTYPE_CNTRL,                                             0x0F, 0x0F },
    { /* DLE */ _PDCLIB_CTYPE_CNTRL,                                             0x10, 0x10 },
    { /* DC1 */ _PDCLIB_CTYPE_CNTRL,                                             0x11, 0x11 },
    { /* DC2 */ _PDCLIB_CTYPE_CNTRL,                                             0x12, 0x12 },
    { /* DC3 */ _PDCLIB_CTYPE_CNTRL,                                             0x13, 0x13 },
    { /* DC4 */ _PDCLIB_CTYPE_CNTRL,                                             0x14, 0x14 },
    { /* NAK */ _PDCLIB_CTYPE_CNTRL,                                             0x15, 0x15 },
    { /* SYN */ _PDCLIB_CTYPE_CNTRL,                                             0x16, 0x16 },
    { /* ETB */ _PDCLIB_CTYPE_CNTRL,                                             0x17, 0x17 },
    { /* CAN */ _PDCLIB_CTYPE_CNTRL,                                             0x18, 0x18 },
    { /*  EM */ _PDCLIB_CTYPE_CNTRL,                                             0x19, 0x19 },
    { /* SUB */ _PDCLIB_CTYPE_CNTRL,                                             0x1A, 0x1A },
    { /* ESC */ _PDCLIB_CTYPE_CNTRL,                                             0x1B, 0x1B },
    { /*  FS */ _PDCLIB_CTYPE_CNTRL,                                             0x1C, 0x1C },
    { /*  GS */ _PDCLIB_CTYPE_CNTRL,                                             0x1D, 0x1D },
    { /*  RS */ _PDCLIB_CTYPE_CNTRL,                                             0x1E, 0x1E },
    { /*  US */ _PDCLIB_CTYPE_CNTRL,                                             0x1F, 0x1F },
    { /*  SP */ _PDCLIB_CTYPE_BLANK | _PDCLIB_CTYPE_SPACE,                       0x20, 0x20 },
    { /* '!' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x21, 0x21 },
    { /* '"' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x22, 0x22 },
    { /* '#' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x23, 0x23 },
    { /* '$' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x24, 0x24 },
    { /* '%' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x25, 0x25 },
    { /* '&' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x26, 0x26 },
    { /* ''' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x27, 0x27 },
    { /* '(' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x28, 0x28 },
    { /* ')' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x29, 0x29 },
    { /* '*' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x2A, 0x2A },
    { /* '+' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x2B, 0x2B },
    { /* ',' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x2C, 0x2C },
    { /* '-' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x2D, 0x2D },
    { /* '.' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x2E, 0x2E },
    { /* '/' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x2F, 0x2F },
    { /* '0' */ _PDCLIB_CTYPE_GRAPH,                                             0x30, 0x30 },
    { /* '1' */ _PDCLIB_CTYPE_GRAPH,                                             0x31, 0x31 },
    { /* '2' */ _PDCLIB_CTYPE_GRAPH,                                             0x32, 0x32 },
    { /* '3' */ _PDCLIB_CTYPE_GRAPH,                                             0x33, 0x33 },
    { /* '4' */ _PDCLIB_CTYPE_GRAPH,                                             0x34, 0x34 },
    { /* '5' */ _PDCLIB_CTYPE_GRAPH,                                             0x35, 0x35 },
    { /* '6' */ _PDCLIB_CTYPE_GRAPH,                                             0x36, 0x36 },
    { /* '7' */ _PDCLIB_CTYPE_GRAPH,                                             0x37, 0x37 },
    { /* '8' */ _PDCLIB_CTYPE_GRAPH,                                             0x38, 0x38 },
    { /* '9' */ _PDCLIB_CTYPE_GRAPH,                                             0x39, 0x39 },
    { /* ':' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x3A, 0x3A },
    { /* ';' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x3B, 0x3B },
    { /* '<' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x3C, 0x3C },
    { /* '=' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x3D, 0x3D },
    { /* '>' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x3E, 0x3E },
    { /* '?' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x3F, 0x3F },
    { /* '@' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x40, 0x40 },
    { /* 'A' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x41, 0x61 },
    { /* 'B' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x42, 0x62 },
    { /* 'C' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x43, 0x63 },
    { /* 'D' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x44, 0x64 },
    { /* 'E' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x45, 0x65 },
    { /* 'F' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x46, 0x66 },
    { /* 'G' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x47, 0x67 },
    { /* 'H' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x48, 0x68 },
    { /* 'I' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x49, 0x69 },
    { /* 'J' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x4A, 0x6A },
    { /* 'K' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x4B, 0x6B },
    { /* 'L' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x4C, 0x6C },
    { /* 'M' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x4D, 0x6D },
    { /* 'N' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x4E, 0x6E },
    { /* 'O' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x4F, 0x6F },
    { /* 'P' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x50, 0x70 },
    { /* 'Q' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x51, 0x71 },
    { /* 'R' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x52, 0x72 },
    { /* 'S' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x53, 0x73 },
    { /* 'T' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x54, 0x74 },
    { /* 'U' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x55, 0x75 },
    { /* 'V' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x56, 0x76 },
    { /* 'W' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x57, 0x77 },
    { /* 'X' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x58, 0x78 },
    { /* 'Y' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x59, 0x79 },
    { /* 'Z' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_UPPER, 0x5A, 0x7A },
    { /* '[' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x5B, 0x5B },
    { /* '\' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x5C, 0x5C },
    { /* ']' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x5D, 0x5D },
    { /* '^' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x5E, 0x5E },
    { /* '_' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x5F, 0x5F },
    { /* '`' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x60, 0x60 },
    { /* 'a' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x41, 0x61 },
    { /* 'b' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x42, 0x62 },
    { /* 'c' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x43, 0x63 },
    { /* 'd' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x44, 0x64 },
    { /* 'e' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x45, 0x65 },
    { /* 'f' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x46, 0x66 },
    { /* 'g' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x47, 0x67 },
    { /* 'h' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x48, 0x68 },
    { /* 'i' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x49, 0x69 },
    { /* 'j' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x4A, 0x6A },
    { /* 'k' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x4B, 0x6B },
    { /* 'l' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x4C, 0x6C },
    { /* 'm' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x4D, 0x6D },
    { /* 'n' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x4E, 0x6E },
    { /* 'o' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x4F, 0x6F },
    { /* 'p' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x50, 0x70 },
    { /* 'q' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x51, 0x71 },
    { /* 'r' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x52, 0x72 },
    { /* 's' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x53, 0x73 },
    { /* 't' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x54, 0x74 },
    { /* 'u' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x55, 0x75 },
    { /* 'v' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x56, 0x76 },
    { /* 'w' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x57, 0x77 },
    { /* 'x' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x58, 0x78 },
    { /* 'y' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x59, 0x79 },
    { /* 'z' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_ALPHA | _PDCLIB_CTYPE_LOWER, 0x5A, 0x7A },
    { /* '{' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x7B, 0x7B },
    { /* '|' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x7C, 0x7C },
    { /* '}' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x7D, 0x7D },
    { /* '~' */ _PDCLIB_CTYPE_GRAPH | _PDCLIB_CTYPE_PUNCT,                       0x7E, 0x7E },
    { /* DEL */ _PDCLIB_CTYPE_CNTRL,                                             0x7F, 0x7F },
};

struct _PDCLIB_lc_ctype_t _PDCLIB_lc_ctype_C =
{
    0,
    0x30, 0x39,   /* digits   '0'..'9' */
    0x41, 0x46,   /* Xdigits  'A'..'F' */
    0x61, 0x66,   /* xdigits  'a'..'f' */
    &_ctype_entries_C[1]
};

struct _PDCLIB_lc_ctype_t * _PDCLIB_lc_ctype = &_PDCLIB_lc_ctype_C;

struct _PDCLIB_lc_collate_t _PDCLIB_lc_collate_C = { 0 };
struct _PDCLIB_lc_collate_t * _PDCLIB_lc_collate = &_PDCLIB_lc_collate_C;

/* -------------------------------------------------------------------------- */
/* C locale — lconv / numeric_monetary                                        */
/* -------------------------------------------------------------------------- */

struct lconv _PDCLIB_lconv =
{
    /* decimal_point      */ ( char * )".",
    /* thousands_sep      */ ( char * )"",
    /* grouping           */ ( char * )"",
    /* mon_decimal_point  */ ( char * )"",
    /* mon_thousands_sep  */ ( char * )"",
    /* mon_grouping       */ ( char * )"",
    /* positive_sign      */ ( char * )"",
    /* negative_sign      */ ( char * )"",
    /* currency_symbol    */ ( char * )"",
    /* int_curr_symbol    */ ( char * )"",
    /* frac_digits        */ CHAR_MAX,
    /* p_cs_precedes      */ CHAR_MAX,
    /* n_cs_precedes      */ CHAR_MAX,
    /* p_sep_by_space     */ CHAR_MAX,
    /* n_sep_by_space     */ CHAR_MAX,
    /* p_sign_posn        */ CHAR_MAX,
    /* n_sign_posn        */ CHAR_MAX,
    /* int_frac_digits    */ CHAR_MAX,
    /* int_p_cs_precedes  */ CHAR_MAX,
    /* int_n_cs_precedes  */ CHAR_MAX,
    /* int_p_sep_by_space */ CHAR_MAX,
    /* int_n_sep_by_space */ CHAR_MAX,
    /* int_p_sign_posn    */ CHAR_MAX,
    /* int_n_sign_posn    */ CHAR_MAX
};

struct _PDCLIB_lc_numeric_monetary_t _PDCLIB_lc_numeric_monetary =
{
    &_PDCLIB_lconv,
    0, /* numeric_alloced  */
    0  /* monetary_alloced */
};

/* -------------------------------------------------------------------------- */
/* C locale — error messages (errno 0..33, matching _PDCLIB_config.h)         */
/* -------------------------------------------------------------------------- */

struct _PDCLIB_lc_messages_t _PDCLIB_lc_messages_C =
{
    0,
    {
        /* 0  */ ( char * )"unknown error",
        /* 1  EDOM        */ ( char * )"EDOM (Domain error)",
        /* 2  EILSEQ      */ ( char * )"EILSEQ (Illegal byte sequence)",
        /* 3  ERANGE      */ ( char * )"ERANGE (Range error)",
        /* 4  EBADF       */ ( char * )"EBADF (Bad file descriptor)",
        /* 5  EINVAL      */ ( char * )"EINVAL (Invalid argument)",
        /* 6  EIO         */ ( char * )"EIO (I/O error)",
        /* 7  ENOMEM      */ ( char * )"ENOMEM (Not enough memory)",
        /* 8  ENOSYS      */ ( char * )"ENOSYS (Function not supported)",
        /* 9  ESPIPE      */ ( char * )"ESPIPE (Invalid seek)",
        /* 10 ENOENT      */ ( char * )"ENOENT (No such file or directory)",
        /* 11 EEXIST      */ ( char * )"EEXIST (File exists)",
        /* 12 EACCES      */ ( char * )"EACCES (Permission denied)",
        /* 13 ENOTSUP     */ ( char * )"ENOTSUP (Not supported)",
        /* 14 ENFILE      */ ( char * )"ENFILE (Too many files open in system)",
        /* 15 EMFILE      */ ( char * )"EMFILE (Too many open files)",
        /* 16 ENAMETOOLONG*/ ( char * )"ENAMETOOLONG (Filename too long)",
        /* 17 EOVERFLOW   */ ( char * )"EOVERFLOW (Value too large)",
        /* 18 ENOSPC      */ ( char * )"ENOSPC (No space left on device)",
        /* 19 EROFS       */ ( char * )"EROFS (Read-only file system)",
        /* 20 EPERM       */ ( char * )"EPERM (Operation not permitted)",
        /* 21 ENOTDIR     */ ( char * )"ENOTDIR (Not a directory)",
        /* 22 EISDIR      */ ( char * )"EISDIR (Is a directory)",
        /* 23 ENOTEMPTY   */ ( char * )"ENOTEMPTY (Directory not empty)",
        /* 24 EDEADLK     */ ( char * )"EDEADLK (Resource deadlock would occur)",
        /* 25 ENOLCK      */ ( char * )"ENOLCK (No locks available)",
        /* 26 ECANCELED   */ ( char * )"ECANCELED (Operation canceled)",
        /* 27 EINTR       */ ( char * )"EINTR (Interrupted function)",
        /* 28 EAGAIN      */ ( char * )"EAGAIN (Resource unavailable, try again)",
        /* 29 ENOEXEC     */ ( char * )"ENOEXEC (Executable file format error)",
        /* 30 ECHILD      */ ( char * )"ECHILD (No child processes)",
        /* 31 EBUSY       */ ( char * )"EBUSY (Device or resource busy)",
        /* 32 EMLINK      */ ( char * )"EMLINK (Too many links)",
        /* 33 EXDEV       */ ( char * )"EXDEV (Cross-device link)",
    }
};

struct _PDCLIB_lc_messages_t * _PDCLIB_lc_messages = &_PDCLIB_lc_messages_C;

/* -------------------------------------------------------------------------- */
/* C locale — time                                                            */
/* -------------------------------------------------------------------------- */

struct _PDCLIB_lc_time_t _PDCLIB_lc_time_C =
{
    0,
    /* month_name_abbr */
    {
        ( char * )"Jan", ( char * )"Feb", ( char * )"Mar",
        ( char * )"Apr", ( char * )"May", ( char * )"Jun",
        ( char * )"Jul", ( char * )"Aug", ( char * )"Sep",
        ( char * )"Oct", ( char * )"Nov", ( char * )"Dec"
    },
    /* month_name_full */
    {
        ( char * )"January",   ( char * )"February", ( char * )"March",
        ( char * )"April",     ( char * )"May",      ( char * )"June",
        ( char * )"July",      ( char * )"August",   ( char * )"September",
        ( char * )"October",   ( char * )"November", ( char * )"December"
    },
    /* day_name_abbr */
    {
        ( char * )"Sun", ( char * )"Mon", ( char * )"Tue",
        ( char * )"Wed", ( char * )"Thu", ( char * )"Fri", ( char * )"Sat"
    },
    /* day_name_full */
    {
        ( char * )"Sunday",    ( char * )"Monday",    ( char * )"Tuesday",
        ( char * )"Wednesday", ( char * )"Thursday",  ( char * )"Friday",
        ( char * )"Saturday"
    },
    /* date / time format */ ( char * )"%a %b %e %T %Y",
    /* 12h time format    */ ( char * )"%I:%M:%S %p",
    /* date format        */ ( char * )"%m/%d/%y",
    /* time format        */ ( char * )"%T",
    /* AM / PM            */ { ( char * )"AM", ( char * )"PM" }
};

struct _PDCLIB_lc_time_t * _PDCLIB_lc_time = &_PDCLIB_lc_time_C;

/* Required by _PDCLIB_tzcode.h internals */
struct state _PDCLIB_lclmem;
struct state _PDCLIB_gmtmem;
struct tm _PDCLIB_tm;

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    /* Testing covered by several other testdrivers using stdin / stdout /
       stderr.
    */
    return TEST_RESULTS;
}

#endif
