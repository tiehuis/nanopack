#include "nanopack.h"

static void to_be_u8(uint64_t n, uint8_t *b)
{
#if NANOPACK_ENDIAN == LITTLE_ENDIAN
    b[0] = (n >> 56) & 0xFF;
    b[1] = (n >> 48) & 0xFF;
    b[2] = (n >> 40) & 0xFF;
    b[3] = (n >> 32) & 0xFF;
    b[4] = (n >> 24) & 0xFF;
    b[5] = (n >> 16) & 0xFF;
    b[6] = (n >>  8) & 0xFF;
    b[7] = (n >>  0) & 0xFF;
#else
    b[0] = (n >>  0) & 0xFF;
    b[1] = (n >>  8) & 0xFF;
    b[2] = (n >> 16) & 0xFF;
    b[3] = (n >> 24) & 0xFF;
    b[4] = (n >> 32) & 0xFF;
    b[5] = (n >> 40) & 0xFF;
    b[6] = (n >> 48) & 0xFF;
    b[7] = (n >> 56) & 0xFF;
#endif
}

static size_t _np_strlen(const char *p)
{
    size_t i = 0;

    while (*p++) {
        i += 1;
    }
    return i;
}

static void* _np_memset(void *s, int c, size_t n)
{
    size_t i;
    uint8_t *o = s;
    uint8_t *_s = s;

    for (i = 0; i < n; ++i) {
        *_s++ = c;
    }

    return o;
}

static void* _np_memcpy(void *d, const void *s, size_t n)
{
    size_t i;
    uint8_t *o = d;
    uint8_t *_d = d;
    const uint8_t *_s = s;

    for (i = 0; i < n; ++i) {
        *_d++ = *_s++;
    }

    return o;
}

np_buf np_make_buf(uint8_t *w, size_t l)
{
    np_buf np;
    np_memset(&np, 0, sizeof(np_buf));
    np.w = w;
    np.len = 0;
    np.cap = l;
    return np;
}

/* void _np_w0(np_buf *p, uint8_t n); */

/* void _np_w1(np_buf *p, uint8_t n, uint8_t op); */

void _np_w2(np_buf *p, uint16_t n, uint8_t op)
{
    uint8_t b[8];

    np_debug_buf_req(p, 3);
    to_be_u8(n, b);
    *p->w++ = op;
    *p->w++ = b[6];
    *p->w++ = b[7];
    p->len += 3;
}

void _np_w4(np_buf *p, uint32_t n, uint8_t op)
{
    uint8_t b[8];

    np_debug_buf_req(p, 5);
    to_be_u8(n, b);
    *p->w++ = op;
    *p->w++ = b[4];
    *p->w++ = b[5];
    *p->w++ = b[6];
    *p->w++ = b[7];
    p->len += 5;
}

void _np_w8(np_buf *p, uint64_t n, uint8_t op)
{
    uint8_t b[8];

    np_debug_buf_req(p, 9);
    to_be_u8(n, b);
    *p->w++ = op;
    *p->w++ = b[0];
    *p->w++ = b[1];
    *p->w++ = b[2];
    *p->w++ = b[3];
    *p->w++ = b[4];
    *p->w++ = b[5];
    *p->w++ = b[6];
    *p->w++ = b[7];
    p->len += 9;
}

void _np_map_or_arr(np_buf *p, uint32_t n, uint8_t c)
{
    if (n < 0x10) {
        _np_w0(p, (0x80 + (c << 4)) | n);
    }
    else if (n < 0x10000) {
        _np_w2(p, n, 0xDE - (2 * c));
    }
    else {
        _np_w4(p, n, 0xDF - (2 * c));
    }
}

void np_nil(np_buf *p)
{
    _np_w0(p, 0xC0);
}

void np_bool(np_buf *p, int n)
{
    _np_w0(p, n ? 0xC3 : 0xC2);
}

void np_str(np_buf *p, const char *s)
{
    size_t n = np_strlen(s);

    if (n < 0x20) {
        _np_w0(p, 0xA0 | (n & 0x1F));
    }
    else if (n < 0x100) {
        _np_w1(p, n, 0xD9);
    }
    else if (n < 0x10000) {
        _np_w2(p, n, 0xDA);
    }
    else {
        _np_w4(p, n, 0xDB);
    }

    np_memcpy(p->w, s, n);
    p->w += n;
    p->len += n;
}

void np_uint(np_buf *p, uint64_t n)
{
    if (n <= 0x7F) {
        _np_w0(p, n);
    }
    else if (n <= 0xFF) {
        _np_w1(p, n, 0xCC);
    }
    else if (n <= 0xFFFF) {
        _np_w2(p, n, 0xCD);
    }
    else if (n <= 0xFFFFFFFF) {
        _np_w4(p, n, 0xCE);
    }
    else {
        _np_w8(p, n, 0xCF);
    }
}

void np_int(np_buf *p, int64_t n)
{
    if (n >= 0) {
        np_uint(p, n);
    }
    if (n >= -32) {
        _np_w0(p, 0xE0 | n);
    }
    else if (n >= INT8_MIN) {
        _np_w1(p, n, 0xD0);
    }
    else if (n >= INT16_MIN) {
        _np_w2(p, n, 0xD1);
    }
    else if (n >= INT32_MIN) {
        _np_w4(p, n, 0xD2);
    }
    else {
        _np_w8(p, n, 0xD3);
    }
}

#if NANOPACK_HAS_FP != 0
void np_float(np_buf *p, float n)
{
    /* unimplemented */
}
#endif

