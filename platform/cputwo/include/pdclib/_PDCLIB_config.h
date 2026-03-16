/* Internal PDCLib configuration <_PDCLIB_config.h>
   CPUTwo platform: 32-bit big-endian bare-metal RISC, no OS, no threads.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_CONFIG_H
#define _PDCLIB_CONFIG_H _PDCLIB_CONFIG_H

/* -------------------------------------------------------------------------- */
/* Misc                                                                       */
/* -------------------------------------------------------------------------- */

#define _PDCLIB_cc( x, y )     x ## y
#define _PDCLIB_concat( x, y ) _PDCLIB_cc( x, y )

#define _PDCLIB_SUCCESS 0
#define _PDCLIB_FAILURE -1

#define _PDCLIB_memswp( i, j, size ) \
    char tmp; \
    do { \
        tmp = *i; \
        *i++ = *j; \
        *j++ = tmp; \
    } while ( --size );

#define _PDCLIB_NORETURN __attribute__(( noreturn ))

/* -------------------------------------------------------------------------- */
/* Symbol Visibility                                                          */
/* -------------------------------------------------------------------------- */

#ifdef _PDCLIB_STATIC_DEFINE
  #define _PDCLIB_PUBLIC
  #define _PDCLIB_LOCAL
#else
  #if __GNUC__ >= 4
    #define _PDCLIB_PUBLIC __attribute__ ((visibility ("default")))
    #define _PDCLIB_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define _PDCLIB_PUBLIC
    #define _PDCLIB_LOCAL
  #endif
#endif

/* -------------------------------------------------------------------------- */
/* TCC compatibility: supply missing GCC/Clang predefined macros              */
/* TCC (cputwo-tcc) does not define __SCHAR_MAX__, __INT_FAST*__, etc.        */
/* Values are for CPUTwo ILP32 big-endian: char=8, short=16, int/long=32,    */
/* long long=64, float=IEEE32, double=long double=IEEE64.                    */
/* -------------------------------------------------------------------------- */

#ifdef __TINYC__

#define __SCHAR_MAX__         127
#define __SHRT_MAX__          32767
/* __INT_MAX__, __LONG_MAX__, __LONG_LONG_MAX__ already defined by TCC */

/* Exact-width integer types (complement what TCC already defines) */
#ifndef __INT8_TYPE__
#define __INT8_TYPE__         signed char
#endif
#ifndef __INT16_TYPE__
#define __INT16_TYPE__        short
#endif
/* __INT32_TYPE__ and __INT64_TYPE__ defined by TCC */
#ifndef __UINT8_TYPE__
#define __UINT8_TYPE__        unsigned char
#endif
#ifndef __UINT16_TYPE__
#define __UINT16_TYPE__       unsigned short
#endif
#ifndef __UINT32_TYPE__
#define __UINT32_TYPE__       unsigned int
#endif
#ifndef __UINT64_TYPE__
#define __UINT64_TYPE__       unsigned long long
#endif

/* Least-width integer types */
#define __INT_LEAST8_TYPE__   signed char
#define __INT_LEAST8_MAX__    127
#define __INT_LEAST8_MIN__    (-127-1)
#define __UINT_LEAST8_TYPE__  unsigned char
#define __UINT_LEAST8_MAX__   255U

#define __INT_LEAST16_TYPE__  short
#define __INT_LEAST16_MAX__   32767
#define __INT_LEAST16_MIN__   (-32767-1)
#define __UINT_LEAST16_TYPE__ unsigned short
#define __UINT_LEAST16_MAX__  65535U

#define __INT_LEAST32_TYPE__  int
#define __INT_LEAST32_MAX__   0x7fffffff
#define __INT_LEAST32_MIN__   (-0x7fffffff-1)
#define __UINT_LEAST32_TYPE__ unsigned int
#define __UINT_LEAST32_MAX__  0xffffffffU

#define __INT_LEAST64_TYPE__  long long
#define __INT_LEAST64_MAX__   0x7fffffffffffffffLL
#define __INT_LEAST64_MIN__   (-0x7fffffffffffffffLL-1)
#define __UINT_LEAST64_TYPE__ unsigned long long
#define __UINT_LEAST64_MAX__  0xffffffffffffffffULL

/* Fast integer types (use widest natural type for bare-metal) */
#define __INT_FAST8_TYPE__    int
#define __INT_FAST8_MAX__     0x7fffffff
#define __INT_FAST8_MIN__     (-0x7fffffff-1)
#define __UINT_FAST8_TYPE__   unsigned int
#define __UINT_FAST8_MAX__    0xffffffffU

#define __INT_FAST16_TYPE__   int
#define __INT_FAST16_MAX__    0x7fffffff
#define __INT_FAST16_MIN__    (-0x7fffffff-1)
#define __UINT_FAST16_TYPE__  unsigned int
#define __UINT_FAST16_MAX__   0xffffffffU

#define __INT_FAST32_TYPE__   int
#define __INT_FAST32_MAX__    0x7fffffff
#define __INT_FAST32_MIN__    (-0x7fffffff-1)
#define __UINT_FAST32_TYPE__  unsigned int
#define __UINT_FAST32_MAX__   0xffffffffU

#define __INT_FAST64_TYPE__   long long
#define __INT_FAST64_MAX__    0x7fffffffffffffffLL
#define __INT_FAST64_MIN__    (-0x7fffffffffffffffLL-1)
#define __UINT_FAST64_TYPE__  unsigned long long
#define __UINT_FAST64_MAX__   0xffffffffffffffffULL

/* intmax_t — use 32-bit long to avoid broken 64-bit ABI on CPUTwo.
 * This limits %jd/%ju to 32-bit range; %lld/%llu are unsupported. */
#define __INTMAX_TYPE__       long
#define __INTMAX_MAX__        0x7fffffffL
#define __INTMAX_MIN__        (-0x7fffffffL-1)
#define __UINTMAX_TYPE__      unsigned long
#define __UINTMAX_MAX__       0xffffffffUL

/* Pointer / size / diff */
/* __INTPTR_TYPE__, __UINTPTR_TYPE__, __PTRDIFF_TYPE__, __SIZE_TYPE__ defined by TCC */
#define __INTPTR_MAX__        0x7fffffff
#define __UINTPTR_MAX__       0xffffffffU
#define __PTRDIFF_MAX__       0x7fffffff
#define __SIZE_MAX__          0xffffffffU

/* sig_atomic_t */
#define __SIG_ATOMIC_TYPE__   int
#define __SIG_ATOMIC_MAX__    0x7fffffff
#define __SIG_ATOMIC_MIN__    (-0x7fffffff-1)

/* wchar / wint */
/* __WCHAR_TYPE__ and __WINT_TYPE__ defined by TCC */
#define __WCHAR_MAX__         0x7fffffff
#define __WCHAR_MIN__         (-0x7fffffff-1)
#define __WINT_MAX__          0xffffffffU
#define __WINT_MIN__          0U

/* Integer constant macros */
#define __INT8_C(c)           c
#define __UINT8_C(c)          c##U
#define __INT16_C(c)          c
#define __UINT16_C(c)         c##U
#define __INT32_C(c)          c
#define __UINT32_C(c)         c##U
#define __INT64_C(c)          c##LL
#define __UINT64_C(c)         c##ULL
#define __INTMAX_C(c)         c##L
#define __UINTMAX_C(c)        c##UL

/* Floating-point: IEEE 754 single / double; long double == double */
#define __FLT_RADIX__         2
#define __FLT_EVAL_METHOD__   0
#define __FLT_MANT_DIG__      24
#define __FLT_DECIMAL_DIG__   9
#define __FLT_DIG__           6
#define __FLT_MIN_EXP__       (-125)
#define __FLT_MIN_10_EXP__    (-37)
#define __FLT_MAX_EXP__       128
#define __FLT_MAX_10_EXP__    38
#define __FLT_MAX__           3.40282347e+38F
#define __FLT_EPSILON__       1.19209290e-07F
#define __FLT_MIN__           1.17549435e-38F
#define __FLT_DENORM_MIN__    1.40129846e-45F
#define __FLT_HAS_DENORM__    1

#define __DBL_MANT_DIG__      53
#define __DBL_DECIMAL_DIG__   17
#define __DBL_DIG__           15
#define __DBL_MIN_EXP__       (-1021)
#define __DBL_MIN_10_EXP__    (-307)
#define __DBL_MAX_EXP__       1024
#define __DBL_MAX_10_EXP__    308
#define __DBL_MAX__           1.7976931348623157e+308
#define __DBL_EPSILON__       2.2204460492503131e-16
#define __DBL_MIN__           2.2250738585072014e-308
#define __DBL_DENORM_MIN__    5.0e-324
#define __DBL_HAS_DENORM__    1

/* long double == double on CPUTwo */
#define __LDBL_MANT_DIG__     53
#define __LDBL_DECIMAL_DIG__  17
#define __LDBL_DIG__          15
#define __LDBL_MIN_EXP__      (-1021)
#define __LDBL_MIN_10_EXP__   (-307)
#define __LDBL_MAX_EXP__      1024
#define __LDBL_MAX_10_EXP__   308
#define __LDBL_MAX__          1.7976931348623157e+308L
#define __LDBL_EPSILON__      2.2204460492503131e-16L
#define __LDBL_MIN__          2.2250738585072014e-308L
#define __LDBL_DENORM_MIN__   5.0e-324L
#define __LDBL_HAS_DENORM__   1

#define __DECIMAL_DIG__       17

#endif /* __TINYC__ */

/* -------------------------------------------------------------------------- */
/* Integers                                                                   */
/* -------------------------------------------------------------------------- */

#define _PDCLIB_TWOS_COMPLEMENT 1

/* CPUTwo is big-endian: 4321. */
#define _PDCLIB_ENDIANESS 4321

#define _PDCLIB_MIN_CALC( max ) ( ( - max ) - 1 )

#define _PDCLIB_CHAR_BIT __CHAR_BIT__

#define _PDCLIB_SCHAR_MAX __SCHAR_MAX__
#define _PDCLIB_SCHAR_MIN _PDCLIB_MIN_CALC( __SCHAR_MAX__ )
#define _PDCLIB_UCHAR_MAX ( __SCHAR_MAX__ * 2 + 1 )

#ifdef __CHAR_UNSIGNED__
#define _PDCLIB_CHAR_MAX _PDCLIB_UCHAR_MAX
#define _PDCLIB_CHAR_MIN 0
#else
#define _PDCLIB_CHAR_MAX _PDCLIB_SCHAR_MAX
#define _PDCLIB_CHAR_MIN _PDCLIB_SCHAR_MIN
#endif

#define _PDCLIB_SHRT_MAX __SHRT_MAX__
#define _PDCLIB_SHRT_MIN _PDCLIB_MIN_CALC( __SHRT_MAX__ )
#define _PDCLIB_USHRT_MAX ( __SHRT_MAX__ * 2u + 1 )

#define _PDCLIB_INT_MAX __INT_MAX__
#define _PDCLIB_INT_MIN _PDCLIB_MIN_CALC( __INT_MAX__ )
#define _PDCLIB_UINT_MAX ( __INT_MAX__ * 2u + 1 )

#define _PDCLIB_LONG_MAX __LONG_MAX__
#define _PDCLIB_LONG_MIN _PDCLIB_MIN_CALC( __LONG_MAX__ )
#define _PDCLIB_ULONG_MAX ( __LONG_MAX__ * 2ul + 1 )

#define _PDCLIB_LLONG_MAX __LONG_LONG_MAX__
#define _PDCLIB_LLONG_MIN _PDCLIB_MIN_CALC( __LONG_LONG_MAX__ )
#define _PDCLIB_ULLONG_MAX ( __LONG_LONG_MAX__ * 2ull + 1 )

/* -------------------------------------------------------------------------- */
/* Fast / least integer types                                                 */
/* -------------------------------------------------------------------------- */

#define _PDCLIB_int_fast8_t        __INT_FAST8_TYPE__
#define _PDCLIB_INT_FAST8_MAX      __INT_FAST8_MAX__
#define _PDCLIB_INT_FAST8_MIN      _PDCLIB_MIN_CALC( __INT_FAST8_MAX__ )
#define _PDCLIB_uint_fast8_t       __UINT_FAST8_TYPE__
#define _PDCLIB_UINT_FAST8_MAX     __UINT_FAST8_MAX__

#define _PDCLIB_int_least8_t       __INT_LEAST8_TYPE__
#define _PDCLIB_INT_LEAST8_MAX     __INT_LEAST8_MAX__
#define _PDCLIB_INT_LEAST8_MIN     _PDCLIB_MIN_CALC( __INT_LEAST8_MAX__ )
#define _PDCLIB_uint_least8_t      __UINT_LEAST8_TYPE__
#define _PDCLIB_UINT_LEAST8_MAX    __UINT_LEAST8_MAX__

#define _PDCLIB_int_fast16_t       __INT_FAST16_TYPE__
#define _PDCLIB_INT_FAST16_MAX     __INT_FAST16_MAX__
#define _PDCLIB_INT_FAST16_MIN     _PDCLIB_MIN_CALC( __INT_FAST16_MAX__ )
#define _PDCLIB_uint_fast16_t      __UINT_FAST16_TYPE__
#define _PDCLIB_UINT_FAST16_MAX    __UINT_FAST16_MAX__

#define _PDCLIB_int_least16_t      __INT_LEAST16_TYPE__
#define _PDCLIB_INT_LEAST16_MAX    __INT_LEAST16_MAX__
#define _PDCLIB_INT_LEAST16_MIN    _PDCLIB_MIN_CALC( __INT_LEAST16_MAX__ )
#define _PDCLIB_uint_least16_t     __UINT_LEAST16_TYPE__
#define _PDCLIB_UINT_LEAST16_MAX   __UINT_LEAST16_MAX__

#define _PDCLIB_int_fast32_t       __INT_FAST32_TYPE__
#define _PDCLIB_INT_FAST32_MAX     __INT_FAST32_MAX__
#define _PDCLIB_INT_FAST32_MIN     _PDCLIB_MIN_CALC( __INT_FAST32_MAX__ )
#define _PDCLIB_uint_fast32_t      __UINT_FAST32_TYPE__
#define _PDCLIB_UINT_FAST32_MAX    __UINT_FAST32_MAX__

#define _PDCLIB_int_least32_t      __INT_LEAST32_TYPE__
#define _PDCLIB_INT_LEAST32_MAX    __INT_LEAST32_MAX__
#define _PDCLIB_INT_LEAST32_MIN    _PDCLIB_MIN_CALC( __INT_LEAST32_MAX__ )
#define _PDCLIB_uint_least32_t     __UINT_LEAST32_TYPE__
#define _PDCLIB_UINT_LEAST32_MAX   __UINT_LEAST32_MAX__

#define _PDCLIB_int_fast64_t       __INT_FAST64_TYPE__
#define _PDCLIB_INT_FAST64_MAX     __INT_FAST64_MAX__
#define _PDCLIB_INT_FAST64_MIN     _PDCLIB_MIN_CALC( __INT_FAST64_MAX__ )
#define _PDCLIB_uint_fast64_t      __UINT_FAST64_TYPE__
#define _PDCLIB_UINT_FAST64_MAX    __UINT_FAST64_MAX__

#define _PDCLIB_int_least64_t      __INT_LEAST64_TYPE__
#define _PDCLIB_INT_LEAST64_MAX    __INT_LEAST64_MAX__
#define _PDCLIB_INT_LEAST64_MIN    _PDCLIB_MIN_CALC( __INT_LEAST64_MAX__ )
#define _PDCLIB_uint_least64_t     __UINT_LEAST64_TYPE__
#define _PDCLIB_UINT_LEAST64_MAX   __UINT_LEAST64_MAX__

#define _PDCLIB_int8_t   __INT8_TYPE__
#define _PDCLIB_int16_t  __INT16_TYPE__
#define _PDCLIB_int32_t  __INT32_TYPE__
#define _PDCLIB_int64_t  __INT64_TYPE__
#define _PDCLIB_uint8_t  __UINT8_TYPE__
#define _PDCLIB_uint16_t __UINT16_TYPE__
#define _PDCLIB_uint32_t __UINT32_TYPE__
#define _PDCLIB_uint64_t __UINT64_TYPE__

#if defined( __INT8_C )
#define _PDCLIB_INT_LEAST8_C       __INT8_C
#define _PDCLIB_UINT_LEAST8_C      __UINT8_C
#define _PDCLIB_INT_LEAST16_C      __INT16_C
#define _PDCLIB_UINT_LEAST16_C     __UINT16_C
#define _PDCLIB_INT_LEAST32_C      __INT32_C
#define _PDCLIB_UINT_LEAST32_C     __UINT32_C
#define _PDCLIB_INT_LEAST64_C      __INT64_C
#define _PDCLIB_UINT_LEAST64_C     __UINT64_C
#elif defined( __INT8_C_SUFFIX__ )
#define _PDCLIB_INT_LEAST8_C(c)    _PDCLIB_concat( c, __INT8_C_SUFFIX__ )
#define _PDCLIB_UINT_LEAST8_C(c)   _PDCLIB_concat( c, __UINT8_C_SUFFIX__ )
#define _PDCLIB_INT_LEAST16_C(c)   _PDCLIB_concat( c, __INT16_C_SUFFIX__ )
#define _PDCLIB_UINT_LEAST16_C(c)  _PDCLIB_concat( c, __UINT16_C_SUFFIX__ )
#define _PDCLIB_INT_LEAST32_C(c)   _PDCLIB_concat( c, __INT32_C_SUFFIX__ )
#define _PDCLIB_UINT_LEAST32_C(c)  _PDCLIB_concat( c, __UINT32_C_SUFFIX__ )
#define _PDCLIB_INT_LEAST64_C(c)   _PDCLIB_concat( c, __INT64_C_SUFFIX__ )
#define _PDCLIB_UINT_LEAST64_C(c)  _PDCLIB_concat( c, __UINT64_C_SUFFIX__ )
#else
#error Unsupported compiler: cannot define INTn_C macros.
#endif

struct _PDCLIB_div_t    { int quot;           int rem; };
struct _PDCLIB_ldiv_t   { long int quot;      long int rem; };
struct _PDCLIB_lldiv_t  { long long int quot; long long int rem; };

/* -------------------------------------------------------------------------- */
/* Special types                                                              */
/* -------------------------------------------------------------------------- */

#define _PDCLIB_ptrdiff_t   __PTRDIFF_TYPE__
#define _PDCLIB_PTRDIFF_MAX __PTRDIFF_MAX__
#define _PDCLIB_PTRDIFF_MIN _PDCLIB_MIN_CALC( __PTRDIFF_MAX__ )

#ifdef __SIG_ATOMIC_TYPE__
#define _PDCLIB_sig_atomic_t   __SIG_ATOMIC_TYPE__
#endif
#define _PDCLIB_SIG_ATOMIC_MAX __SIG_ATOMIC_MAX__
#define _PDCLIB_SIG_ATOMIC_MIN _PDCLIB_MIN_CALC( __SIG_ATOMIC_MAX__ )

#define _PDCLIB_size_t   __SIZE_TYPE__
#define _PDCLIB_SIZE_MAX __SIZE_MAX__

#define _PDCLIB_wchar_t   __WCHAR_TYPE__
#define _PDCLIB_WCHAR_MAX __WCHAR_MAX__
#define _PDCLIB_WCHAR_MIN __WCHAR_MIN__

#define _PDCLIB_wint_t   __WINT_TYPE__
#define _PDCLIB_WINT_MAX __WINT_MAX__
#define _PDCLIB_WINT_MIN __WINT_MIN__

#define _PDCLIB_intptr_t     __INTPTR_TYPE__
#define _PDCLIB_INTPTR_MAX   __INTPTR_MAX__
#define _PDCLIB_INTPTR_MIN   _PDCLIB_MIN_CALC( __INTPTR_MAX__ )
#define _PDCLIB_uintptr_t    __UINTPTR_TYPE__
#define _PDCLIB_UINTPTR_MAX  __UINTPTR_MAX__

#define _PDCLIB_intmax_t     __INTMAX_TYPE__
#define _PDCLIB_INTMAX_MAX   __INTMAX_MAX__
#define _PDCLIB_INTMAX_MIN   _PDCLIB_MIN_CALC( __INTMAX_MAX__ )
#define _PDCLIB_INTMAX_C     __INTMAX_C
#define _PDCLIB_uintmax_t    __UINTMAX_TYPE__
#define _PDCLIB_UINTMAX_MAX  __UINTMAX_MAX__
#define _PDCLIB_UINTMAX_C    __UINTMAX_C

struct _PDCLIB_imaxdiv_t { _PDCLIB_intmax_t quot; _PDCLIB_intmax_t rem; };

/* -------------------------------------------------------------------------- */
/* Time                                                                       */
/* -------------------------------------------------------------------------- */

/* CPUTwo has a timer peripheral but no RTC. clock() returns timer ticks;
   time() returns -1 (no absolute time source). */
#define _PDCLIB_clock_t        long
#define _PDCLIB_CLOCKS_PER_SEC 1000000

#define _PDCLIB_time_t  long
#define _PDCLIB_TIME_MAX __LONG_MAX__
#define _PDCLIB_TIME_MIN _PDCLIB_MIN_CALC( __LONG_MAX__ )

#define _PDCLIB_EPOCH_BIAS INT64_C( 0 )
#define _PDCLIB_TIME_UTC 1

/* Bare-metal: no timezone data files. */
#define _PDCLIB_TZDIR     "/tzdata/"
#define _PDCLIB_TZDEFAULT "/tzdata/localtime"

/* -------------------------------------------------------------------------- */
/* Floating Point                                                             */
/* -------------------------------------------------------------------------- */

#define _PDCLIB_FLT_ROUNDS       -1
#define _PDCLIB_FLT_EVAL_METHOD  __FLT_EVAL_METHOD__

#define _PDCLIB_FLT_HAS_SUBNORM  __FLT_HAS_DENORM__
#define _PDCLIB_DBL_HAS_SUBNORM  __DBL_HAS_DENORM__
#define _PDCLIB_LDBL_HAS_SUBNORM __LDBL_HAS_DENORM__

#define _PDCLIB_FLT_RADIX        __FLT_RADIX__

#define _PDCLIB_FLT_MANT_DIG     __FLT_MANT_DIG__
#define _PDCLIB_DBL_MANT_DIG     __DBL_MANT_DIG__
#define _PDCLIB_LDBL_MANT_DIG    __LDBL_MANT_DIG__

#define _PDCLIB_FLT_DECIMAL_DIG  __FLT_DECIMAL_DIG__
#define _PDCLIB_DBL_DECIMAL_DIG  __DBL_DECIMAL_DIG__
#define _PDCLIB_LDBL_DECIMAL_DIG __LDBL_DECIMAL_DIG__

#define _PDCLIB_DECIMAL_DIG      __DECIMAL_DIG__

#define _PDCLIB_FLT_DIG          __FLT_DIG__
#define _PDCLIB_DBL_DIG          __DBL_DIG__
#define _PDCLIB_LDBL_DIG         __LDBL_DIG__

#define _PDCLIB_FLT_MIN_EXP      __FLT_MIN_EXP__
#define _PDCLIB_DBL_MIN_EXP      __DBL_MIN_EXP__
#define _PDCLIB_LDBL_MIN_EXP     __LDBL_MIN_EXP__

#define _PDCLIB_FLT_MIN_10_EXP   __FLT_MIN_10_EXP__
#define _PDCLIB_DBL_MIN_10_EXP   __DBL_MIN_10_EXP__
#define _PDCLIB_LDBL_MIN_10_EXP  __LDBL_MIN_10_EXP__

#define _PDCLIB_FLT_MAX_EXP      __FLT_MAX_EXP__
#define _PDCLIB_DBL_MAX_EXP      __DBL_MAX_EXP__
#define _PDCLIB_LDBL_MAX_EXP     __LDBL_MAX_EXP__

#define _PDCLIB_FLT_MAX_10_EXP   __FLT_MAX_10_EXP__
#define _PDCLIB_DBL_MAX_10_EXP   __DBL_MAX_10_EXP__
#define _PDCLIB_LDBL_MAX_10_EXP  __LDBL_MAX_10_EXP__

#define _PDCLIB_FLT_MAX          __FLT_MAX__
#define _PDCLIB_DBL_MAX          __DBL_MAX__
#define _PDCLIB_LDBL_MAX         __LDBL_MAX__

#define _PDCLIB_FLT_EPSILON      __FLT_EPSILON__
#define _PDCLIB_DBL_EPSILON      __DBL_EPSILON__
#define _PDCLIB_LDBL_EPSILON     __LDBL_EPSILON__

#define _PDCLIB_FLT_MIN          __FLT_MIN__
#define _PDCLIB_DBL_MIN          __DBL_MIN__
#define _PDCLIB_LDBL_MIN         __LDBL_MIN__

#define _PDCLIB_FLT_TRUE_MIN     __FLT_DENORM_MIN__
#define _PDCLIB_DBL_TRUE_MIN     __DBL_DENORM_MIN__
#define _PDCLIB_LDBL_TRUE_MIN    __LDBL_DENORM_MIN__

/* CPUTwo: long double == double (64-bit IEEE 754, LDBL_MANT_DIG == 53).
   Use the same macros as DBL. */
#define _PDCLIB_FLT_SIGN( data ) ( ( data[1] & 0x8000 ) >> 15 )
#define _PDCLIB_FLT_EXP( data )  ( ( data[1] & 0x7f80 ) >> 7 )
#define _PDCLIB_FLT_SIZE( data ) ( data[1] &= 0x007f, 2 )

#define _PDCLIB_DBL_SIGN( data )  ( ( data[3] & 0x8000 ) >> 15 )
#define _PDCLIB_DBL_EXP( data )   ( ( data[3] & 0x7ff0u ) >> 4 )
#define _PDCLIB_DBL_SIZE( data )  ( data[3] &= 0x000fu, 4 )

/* long double == double on CPUTwo */
#define _PDCLIB_LDBL_SIGN( data ) ( ( data[3] & 0x8000 ) >> 15 )
#define _PDCLIB_LDBL_EXP( data )  ( ( data[3] & 0x7ff0u ) >> 4 )
#define _PDCLIB_LDBL_SIZE( data ) ( data[3] &= 0x000fu, 4 )

/* Big-endian: mantissa operand goes from high to low address (subtract). */
#define _PDCLIB_FLT_OP -

/* -------------------------------------------------------------------------- */
/* Big Integer Arithmetic                                                     */
/* -------------------------------------------------------------------------- */

/* CPUTwo has MULH/MULHU for 64-bit products but no native 64-bit mul.
   Use 16-bit digits to keep intermediate products in 32 bits. */
#define _PDCLIB_BIGINT_DIGIT_BITS 16

/* -------------------------------------------------------------------------- */
/* offsetof                                                                   */
/* -------------------------------------------------------------------------- */

#define _PDCLIB_offsetof( type, member ) \
    ( (size_t) &( ( (type *) 0 )->member ) )

/* -------------------------------------------------------------------------- */
/* Variable Arguments (<stdarg.h>)                                            */
/* -------------------------------------------------------------------------- */

/* CPUTwo uses register-based parameter passing (r0-r3); use compiler
   builtins. Works with GCC, Clang, and TCC. */
typedef __builtin_va_list _PDCLIB_va_list;
#define _PDCLIB_va_arg( ap, type )      ( __builtin_va_arg( ap, type ) )
#define _PDCLIB_va_copy( dest, src )    ( __builtin_va_copy( dest, src ) )
#define _PDCLIB_va_end( ap )            ( __builtin_va_end( ap ) )
#define _PDCLIB_va_start( ap, parmN )   ( __builtin_va_start( ap, parmN ) )

/* -------------------------------------------------------------------------- */
/* OS Glue — I/O                                                              */
/* -------------------------------------------------------------------------- */

/* File descriptors: 0=stdin, 1=stdout, 2=stderr. No real filesystem. */
typedef int _PDCLIB_fd_t;
#define _PDCLIB_NOHANDLE ( (_PDCLIB_fd_t) -1 )

#define _PDCLIB_BUFSIZ       1024
#define _PDCLIB_FOPEN_MAX    8
#define _PDCLIB_FILENAME_MAX 128
#define _PDCLIB_L_tmpnam     52
#define _PDCLIB_TMP_MAX      1       /* tmpfile() not supported */

#define _PDCLIB_SEEK_SET 0
#define _PDCLIB_SEEK_CUR 1
#define _PDCLIB_SEEK_END 2

#define _PDCLIB_UNGETCBUFSIZE 1
#define _PDCLIB_ATEXIT_SLOTS  40

/* -------------------------------------------------------------------------- */
/* errno values (a small self-consistent set for bare-metal)                 */
/* -------------------------------------------------------------------------- */

#define _PDCLIB_EDOM             1
#define _PDCLIB_EILSEQ           2
#define _PDCLIB_ERANGE           3
#define _PDCLIB_EBADF            4
#define _PDCLIB_EINVAL           5
#define _PDCLIB_EIO              6
#define _PDCLIB_ENOMEM           7
#define _PDCLIB_ENOSYS           8
#define _PDCLIB_ESPIPE           9
#define _PDCLIB_ENOENT          10
#define _PDCLIB_EEXIST          11
#define _PDCLIB_EACCES          12
#define _PDCLIB_ENOTSUP         13
#define _PDCLIB_ENFILE          14
#define _PDCLIB_EMFILE          15
#define _PDCLIB_ENAMETOOLONG    16
#define _PDCLIB_EOVERFLOW       17
#define _PDCLIB_ENOSPC          18
#define _PDCLIB_EROFS           19
#define _PDCLIB_EPERM           20
#define _PDCLIB_ENOTDIR         21
#define _PDCLIB_EISDIR          22
#define _PDCLIB_ENOTEMPTY       23
#define _PDCLIB_EDEADLK         24
#define _PDCLIB_ENOLCK          25
#define _PDCLIB_ECANCELED       26
#define _PDCLIB_EINTR           27
#define _PDCLIB_EAGAIN          28
#define _PDCLIB_ENOEXEC         29
#define _PDCLIB_ECHILD          30
#define _PDCLIB_EBUSY           31
#define _PDCLIB_EMLINK          32
#define _PDCLIB_EXDEV           33

/* Aliases */
#define _PDCLIB_EWOULDBLOCK  _PDCLIB_EAGAIN
#define _PDCLIB_EOPNOTSUPP   _PDCLIB_ENOTSUP

/* All networking / socket errnos map to ENOSYS (no network on CPUTwo) */
#define _PDCLIB_EADDRINUSE      _PDCLIB_ENOSYS
#define _PDCLIB_EADDRNOTAVAIL   _PDCLIB_ENOSYS
#define _PDCLIB_EAFNOSUPPORT    _PDCLIB_ENOSYS
#define _PDCLIB_EALREADY        _PDCLIB_ENOSYS
#define _PDCLIB_EBADMSG         _PDCLIB_EIO
#define _PDCLIB_ECONNABORTED    _PDCLIB_ENOSYS
#define _PDCLIB_ECONNREFUSED    _PDCLIB_ENOSYS
#define _PDCLIB_ECONNRESET      _PDCLIB_ENOSYS
#define _PDCLIB_EDESTADDRREQ    _PDCLIB_ENOSYS
#define _PDCLIB_EFAULT          _PDCLIB_EINVAL
#define _PDCLIB_EFBIG           _PDCLIB_ENOSPC
#define _PDCLIB_EHOSTUNREACH    _PDCLIB_ENOSYS
#define _PDCLIB_EIDRM           _PDCLIB_ENOSYS
#define _PDCLIB_EINPROGRESS     _PDCLIB_ENOSYS
#define _PDCLIB_EISCONN         _PDCLIB_ENOSYS
#define _PDCLIB_ELOOP           _PDCLIB_ENOSYS
#define _PDCLIB_EMSGSIZE        _PDCLIB_ENOSYS
#define _PDCLIB_ENETDOWN        _PDCLIB_ENOSYS
#define _PDCLIB_ENETRESET       _PDCLIB_ENOSYS
#define _PDCLIB_ENETUNREACH     _PDCLIB_ENOSYS
#define _PDCLIB_ENOBUFS         _PDCLIB_ENOSYS
#define _PDCLIB_ENODATA         _PDCLIB_ENOSYS
#define _PDCLIB_ENODEV          _PDCLIB_ENOSYS
#define _PDCLIB_ENOLINK         _PDCLIB_ENOSYS
#define _PDCLIB_ENOMSG          _PDCLIB_ENOSYS
#define _PDCLIB_ENOPROTOOPT     _PDCLIB_ENOSYS
#define _PDCLIB_ENOSR           _PDCLIB_ENOSYS
#define _PDCLIB_ENOSTR          _PDCLIB_ENOSYS
#define _PDCLIB_ENOTCONN        _PDCLIB_ENOSYS
#define _PDCLIB_ENOTRECOVERABLE _PDCLIB_ENOSYS
#define _PDCLIB_ENOTSOCK        _PDCLIB_ENOSYS
#define _PDCLIB_ENXIO           _PDCLIB_ENOSYS
#define _PDCLIB_EOWNERDEAD      _PDCLIB_ENOSYS
#define _PDCLIB_EPIPE           _PDCLIB_EIO
#define _PDCLIB_EPROTO          _PDCLIB_ENOSYS
#define _PDCLIB_EPROTONOSUPPORT _PDCLIB_ENOSYS
#define _PDCLIB_EPROTOTYPE      _PDCLIB_ENOSYS
#define _PDCLIB_ESRCH           _PDCLIB_ENOSYS
#define _PDCLIB_ETIME           _PDCLIB_ENOSYS
#define _PDCLIB_ETIMEDOUT       _PDCLIB_ENOSYS
#define _PDCLIB_ETXTBSY         _PDCLIB_ENOSYS
#define _PDCLIB_E2BIG           _PDCLIB_ENOSYS

#define _PDCLIB_ERRNO_MAX 34

#define _PDCLIB_EUNKNOWN_TEXT (char*)"unknown error"

/* -------------------------------------------------------------------------- */
/* Locale                                                                     */
/* -------------------------------------------------------------------------- */

#define _PDCLIB_LOCALE_PATH     "/locale/"
#define _PDCLIB_LOCALE_PATH_ENV PDCLIB_I18N

/* -------------------------------------------------------------------------- */
/* Threads: not supported on CPUTwo bare-metal                               */
/* -------------------------------------------------------------------------- */

#define __STDC_NO_THREADS__ 1

#endif
