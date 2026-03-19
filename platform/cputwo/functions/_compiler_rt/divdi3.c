/* 64-bit integer division builtins for CPUTwo
 *
 * Provides: __udivdi3, __umoddi3, __divdi3, __moddi3, __udivmoddi4
 *
 * Based on the algorithm from compiler-rt (The PowerPC Compiler Writer's Guide,
 * Figure 3-40), simplified into a single self-contained file.
 */

#include <stdint.h>
#include <limits.h>

typedef int32_t  si_int;
typedef uint32_t su_int;
typedef int64_t  di_int;
typedef uint64_t du_int;

typedef union {
    du_int all;
    struct {
        su_int low;
        su_int high;
    } s;
} udwords;

typedef union {
    di_int all;
    struct {
        su_int low;
        si_int high;
    } s;
} dwords;

/* ---------- unsigned 64-bit division (shift-and-subtract) ---------- */

du_int __udivmoddi4(du_int a, du_int b, du_int *rem) {
    const unsigned n_uword_bits = sizeof(su_int) * CHAR_BIT;
    const unsigned n_udword_bits = sizeof(du_int) * CHAR_BIT;
    udwords n, d, q, r;
    unsigned sr;

    n.all = a;
    d.all = b;

    /* Special cases */
    if (n.s.high == 0) {
        if (d.s.high == 0) {
            if (rem)
                *rem = n.s.low % d.s.low;
            return n.s.low / d.s.low;
        }
        if (rem)
            *rem = n.s.low;
        return 0;
    }

    if (d.s.low == 0) {
        if (d.s.high == 0) {
            if (rem)
                *rem = n.s.high % d.s.low;
            return n.s.high / d.s.low;
        }
        if (n.s.low == 0) {
            if (rem) {
                r.s.high = n.s.high % d.s.high;
                r.s.low = 0;
                *rem = r.all;
            }
            return n.s.high / d.s.high;
        }
        if ((d.s.high & (d.s.high - 1)) == 0) {
            if (rem) {
                r.s.low = n.s.low;
                r.s.high = n.s.high & (d.s.high - 1);
                *rem = r.all;
            }
            return n.s.high >> __builtin_ctz(d.s.high);
        }
        sr = __builtin_clz(d.s.high) - __builtin_clz(n.s.high);
        if (sr > n_uword_bits - 2) {
            if (rem)
                *rem = n.all;
            return 0;
        }
        ++sr;
        q.s.low = 0;
        q.s.high = n.s.low << (n_uword_bits - sr);
        r.s.high = n.s.high >> sr;
        r.s.low = (n.s.high << (n_uword_bits - sr)) | (n.s.low >> sr);
    } else {
        if (d.s.high == 0) {
            if ((d.s.low & (d.s.low - 1)) == 0) {
                if (rem)
                    *rem = n.s.low & (d.s.low - 1);
                if (d.s.low == 1)
                    return n.all;
                sr = __builtin_ctz(d.s.low);
                q.s.high = n.s.high >> sr;
                q.s.low = (n.s.high << (n_uword_bits - sr)) | (n.s.low >> sr);
                return q.all;
            }
            sr = 1 + n_uword_bits + __builtin_clz(d.s.low) - __builtin_clz(n.s.high);
            if (sr == n_uword_bits) {
                q.s.low = 0;
                q.s.high = n.s.low;
                r.s.high = 0;
                r.s.low = n.s.high;
            } else if (sr < n_uword_bits) {
                q.s.low = 0;
                q.s.high = n.s.low << (n_uword_bits - sr);
                r.s.high = n.s.high >> sr;
                r.s.low = (n.s.high << (n_uword_bits - sr)) | (n.s.low >> sr);
            } else {
                q.s.low = n.s.low << (n_udword_bits - sr);
                q.s.high = (n.s.high << (n_udword_bits - sr)) |
                           (n.s.low >> (sr - n_uword_bits));
                r.s.high = 0;
                r.s.low = n.s.high >> (sr - n_uword_bits);
            }
        } else {
            sr = __builtin_clz(d.s.high) - __builtin_clz(n.s.high);
            if (sr > n_uword_bits - 1) {
                if (rem)
                    *rem = n.all;
                return 0;
            }
            ++sr;
            q.s.low = 0;
            if (sr == n_uword_bits) {
                q.s.high = n.s.low;
                r.s.high = 0;
                r.s.low = n.s.high;
            } else {
                q.s.high = n.s.low << (n_uword_bits - sr);
                r.s.high = n.s.high >> sr;
                r.s.low = (n.s.high << (n_uword_bits - sr)) | (n.s.low >> sr);
            }
        }
    }

    /* Long division loop */
    su_int carry = 0;
    for (; sr > 0; --sr) {
        r.s.high = (r.s.high << 1) | (r.s.low >> (n_uword_bits - 1));
        r.s.low  = (r.s.low  << 1) | (q.s.high >> (n_uword_bits - 1));
        q.s.high = (q.s.high << 1) | (q.s.low >> (n_uword_bits - 1));
        q.s.low  = (q.s.low  << 1) | carry;
        const di_int s = (di_int)(d.all - r.all - 1) >> (n_udword_bits - 1);
        carry = s & 1;
        r.all -= d.all & s;
    }
    q.all = (q.all << 1) | carry;
    if (rem)
        *rem = r.all;
    return q.all;
}

du_int __udivdi3(du_int a, du_int b) {
    return __udivmoddi4(a, b, (du_int *)0);
}

du_int __umoddi3(du_int a, du_int b) {
    du_int r;
    __udivmoddi4(a, b, &r);
    return r;
}

di_int __divdi3(di_int a, di_int b) {
    const int bits = (int)(sizeof(di_int) * CHAR_BIT) - 1;
    di_int s_a = a >> bits;         /* -1 if a < 0, else 0 */
    di_int s_b = b >> bits;
    du_int a_u = (du_int)(a ^ s_a) + (du_int)(-s_a);  /* abs(a) */
    du_int b_u = (du_int)(b ^ s_b) + (du_int)(-s_b);  /* abs(b) */
    s_a ^= s_b;                     /* sign of result */
    return ((di_int)__udivmoddi4(a_u, b_u, (du_int *)0) ^ s_a) + (-s_a);
}

di_int __moddi3(di_int a, di_int b) {
    const int bits = (int)(sizeof(di_int) * CHAR_BIT) - 1;
    di_int s = b >> bits;
    du_int b_u = (du_int)(b ^ s) + (du_int)(-s);
    s = a >> bits;
    du_int a_u = (du_int)(a ^ s) + (du_int)(-s);
    du_int res;
    __udivmoddi4(a_u, b_u, &res);
    return ((di_int)res ^ s) + (-s);
}
