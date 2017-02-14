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

/* Can override with libc memcpy/strlen if present. */
#define np_memcpy _np_memcpy
#define np_strlen _np_strlen

/* Construct a map with the specified number of items (keys + values). */
#define np_map(p, n) _np_map_or_arr(p, n / 2, 0)

/* Construct an array with the specified number of items. */
#define np_arr(p, n) _np_map_or_arr(p, n, 1)

/* Should not be called directly. Use `np_map` or `np_arr` instead. */
void _np_map_or_arr(uint8_t **p, uint32_t n, uint8_t c);

void np_nil(uint8_t **p);
void np_bool(uint8_t **p, int n);
void np_str(uint8_t **p, const char *s);

/* Short calls to avoid small function call overhead. */
#define _np_w0(p, n)        \
do {                        \
    *(*p)++ = (op);         \
} while (0)

#define _np_w1(p, n, op)    \
do {                        \
    *(*p)++ = (op);         \
    *(*p)++ = (n);          \
} while (0)

void _np_w2(uint8_t **p, uint16_t n, uint8_t op);
void _np_w4(uint8_t **p, uint32_t n, uint8_t op);
void _np_w8(uint8_t **p, uint64_t n, uint8_t op);

#define np_u8(p, n) _np_w1(p, n, 0xCC)
#define np_u16(p, n) _np_w2(p, n, 0xCD)
#define np_u32(p, n) _np_w4(p, n, 0xCE)
#define np_u64(p, n) _np_w8(p, n, 0xCF)

/* Signed integer values assume int -> uint cast is consistent across
 * machines. */
#define np_i8(p, n) _np_w1(p, n, 0xD0)
#define np_i16(p, n) _np_w2(p, n, 0xD1)
#define np_i32(p, n) _np_w4(p, n, 0xD2)
#define np_i64(p, n) _np_w8(p, n, 0xD3)

#if NANOPACK_HAS_FP != 0
void np_float(uint8_t **p, float n);
#endif

#endif
