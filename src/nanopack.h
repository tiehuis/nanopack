#ifndef NANOPACK_H
#define NANOPACK_H

#ifndef NANOPACK_ENDIAN
#define NANOPACK_ENDIAN LITTLE_ENDIAN
#endif

/* Enables/disables floating-point packing support. */
#ifndef NANOPACK_HAS_FP
#define NANOPACK_HAS_FP 0
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
#define np_make_buf(b, c) (np_buf) { .w = b, .len = 0, .cap = c }

/* Can override with libc memcpy/strlen if present. */
#define np_memcpy _np_memcpy
#define np_strlen _np_strlen

/* Construct a map with the specified number of items (keys + values). */
#define np_map(p, n) _np_map_or_arr(p, n / 2, 0)

/* Construct an array with the specified number of items. */
#define np_arr(p, n) _np_map_or_arr(p, n, 1)

/* Should not be called directly. Use `np_map` or `np_arr` instead. */
void _np_map_or_arr(np_buf *p, uint32_t n, uint8_t c);

#define np_nil(p) _np_w0(p, 0xC0)
#define np_bool(p, n) _np_w0(p, n ? 0xC3 : 0xC2)

void np_str(np_buf *p, const char *s);

/* Short calls to avoid small function call overhead. */
#define _np_w0(p, n)        \
do {                        \
    *p->w++ = (n);          \
} while (0)

#define _np_w1(p, n, op)    \
do {                        \
    *p->w++ = (op);         \
    *p->w++ = (n);          \
} while (0)

void _np_w2(np_buf *p, uint16_t n, uint8_t op);
void _np_w4(np_buf *p, uint32_t n, uint8_t op);
void _np_w8(np_buf *p, uint64_t n, uint8_t op);
void _np_uint(np_buf *p, uint64_t n, uint8_t o);
void _np_int(np_buf *p, int64_t n, uint8_t o);

#define np_u8(p, n) _np_uint(p, n, 0)
#define np_u16(p, n) _np_uint(p, n, 0)
#define np_u32(p, n) _np_uint(p, n, 0)
#define np_u64(p, n) _np_uint(p, n, 0)

/* Signed integer values assume int -> uint cast is consistent across
 * machines. */
#define np_i8(p, n) _np_int(p, n, 4)
#define np_i16(p, n) _np_int(p, n, 4)
#define np_i32(p, n) _np_int(p, n, 4)
#define np_i64(p, n) _np_int(p, n, 4)

#if NANOPACK_HAS_FP != 0
void np_float(np_buf *p, float n);
#endif

#endif
