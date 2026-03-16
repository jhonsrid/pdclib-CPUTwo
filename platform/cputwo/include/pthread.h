/* pthread.h — bare-metal stub for CPUTwo (single-threaded, no-op locks)
   Used by dlmalloc when USE_LOCKS=1 (as set by pdclib's malloc.c wrapper). */
#ifndef _CPUTWO_PTHREAD_H
#define _CPUTWO_PTHREAD_H

typedef unsigned int pthread_t;
typedef struct { int dummy; } pthread_mutex_t;
typedef struct { int type; } pthread_mutexattr_t;

#define PTHREAD_MUTEX_RECURSIVE      1
#define PTHREAD_MUTEX_INITIALIZER    { 0 }

static inline pthread_t pthread_self(void) { return 0; }
static inline int pthread_equal(pthread_t a, pthread_t b) { return a == b; }

static inline int pthread_mutexattr_init(pthread_mutexattr_t *a)
    { a->type = 0; return 0; }
static inline int pthread_mutexattr_settype(pthread_mutexattr_t *a, int t)
    { a->type = t; return 0; }
static inline int pthread_mutexattr_destroy(pthread_mutexattr_t *a)
    { (void)a; return 0; }

static inline int pthread_mutex_init(pthread_mutex_t *m, const pthread_mutexattr_t *a)
    { (void)m; (void)a; return 0; }
static inline int pthread_mutex_destroy(pthread_mutex_t *m)
    { (void)m; return 0; }
static inline int pthread_mutex_lock(pthread_mutex_t *m)
    { (void)m; return 0; }
static inline int pthread_mutex_unlock(pthread_mutex_t *m)
    { (void)m; return 0; }
static inline int pthread_mutex_trylock(pthread_mutex_t *m)
    { (void)m; return 0; }

static inline int pthread_atfork(void (*a)(void), void (*b)(void), void (*c)(void))
    { (void)a; (void)b; (void)c; return 0; }

#endif /* _CPUTWO_PTHREAD_H */
