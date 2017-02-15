#ifndef NANOPACK_H
#define NANOPACK_H

#ifndef NANOPACK_ENDIAN
#define NANOPACK_ENDIAN LITTLE_ENDIAN
#endif

/* Enables/disables floating-point packing support. */
#ifndef NANOPACK_HAS_FP
#define NANOPACK_HAS_FP 0
#endif

/* Debug switches on overflow checking and tests upper limits against the
 * used buffers. */
#ifndef NANOPACK_DEBUG
#define NANOPACK_DEBUG 0
#endif

#if NANOPACK_DEBUG != 0
/* This must be provided by the caller and will be called on error. */
void np_err_callback(int line, const char *filename, const char *err_msg);

#define np_debug_buf_req(p, n)                                      \
do {                                                                \
    if (p->len + n >= p->cap) {                                     \
        np_err_callback(__LINE__, __FILE__, "buffer overflow!");    \
    }                                                               \
} while (0)
#else
#define np_debug_buf_req(p, n)
#endif

#include <stdint.h> /* For fixed-width integers */
#include <stddef.h> /* For size_t */

typedef struct {
    /* Write pointer into buffer */
    uint8_t *w;

    /* Length of buffer written */
    size_t len;

    /* Capacity of buffer */
    size_t cap;
} np_buf;

/* Construct a new `np_buf` object. */
np_buf np_make_buf(uint8_t *w, size_t l);

/* Can override with libc memcpy/strlen if present. */
#define np_memcpy _np_memcpy
#define np_memset _np_memset
#define np_strlen _np_strlen

/* Construct a map with the specified number of items (keys + values). */
#define np_map(p, n) _np_map_or_arr(p, n / 2, 0)

/* Construct an array with the specified number of items. */
#define np_arr(p, n) _np_map_or_arr(p, n, 1)

/* Should not be called directly. Use `np_map` or `np_arr` instead. */
void _np_map_or_arr(np_buf *p, uint32_t n, uint8_t c);

void np_nil(np_buf *p);
void np_bool(np_buf *p, int n);
void np_str(np_buf *p, const char *s);

/* Short calls to avoid small function call overhead. */
#define _np_w0(p, n)        \
do {                        \
    np_debug_buf_req(p, 1); \
    *p->w++ = (n);          \
    p->len += 1;            \
} while (0)

#define _np_w1(p, n, op)    \
do {                        \
    np_debug_buf_req(p, 2); \
    *p->w++ = (op);         \
    *p->w++ = (n);          \
    p->len += 2;            \
} while (0)

void _np_w2(np_buf *p, uint16_t n, uint8_t op);
void _np_w4(np_buf *p, uint32_t n, uint8_t op);
void _np_w8(np_buf *p, uint64_t n, uint8_t op);

void np_uint(np_buf *p, uint64_t n);
void np_int(np_buf *p, int64_t n);

#define np_u8(p, n) np_uint(p, n)
#define np_u16(p, n) np_uint(p, n)
#define np_u32(p, n) np_uint(p, n)
#define np_u64(p, n) np_uint(p, n)

/* Signed integer values assume int -> uint cast is consistent across
 * machines. */
#define np_i8(p, n) np_int(p, n)
#define np_i16(p, n) np_int(p, n)
#define np_i32(p, n) np_int(p, n)
#define np_i64(p, n) np_int(p, n)

#if NANOPACK_HAS_FP != 0
void np_float(np_buf *p, float n);
#endif

#endif
