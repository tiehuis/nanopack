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

void _np_w2(uint8_t **p, uint16_t n, uint8_t op)
{
    uint8_t b[8];

    to_be_u8(n, b);
    *(*p)++ = op;
    *(*p)++ = b[6];
    *(*p)++ = b[7];
}

void _np_w4(uint8_t **p, uint32_t n, uint8_t op)
{
    uint8_t b[8];

    to_be_u8(n, b);
    *(*p)++ = op;
    *(*p)++ = b[4];
    *(*p)++ = b[5];
    *(*p)++ = b[6];
    *(*p)++ = b[7];
}

void _np_w8(uint8_t **p, uint64_t n, uint8_t op)
{
    uint8_t b[8];

    to_be_u8(n, b);
    *(*p)++ = op;
    *(*p)++ = b[0];
    *(*p)++ = b[1];
    *(*p)++ = b[2];
    *(*p)++ = b[3];
    *(*p)++ = b[4];
    *(*p)++ = b[5];
    *(*p)++ = b[6];
    *(*p)++ = b[7];
}

void _np_map_or_arr(uint8_t **p, uint32_t n, uint8_t c)
{
    if (n < 0x10) {
        *(*p)++ = (0x80 + (c << 4)) | (n & 0xF);
    }
    else if (n < 0x10000) {
        _np_w2(p, n, 0xDE - (2 * c));
    }
    else {
        _np_w4(p, n, 0xDF - (2 * c));
    }
}

void np_nil(uint8_t **p)
{
    *(*p)++ = 0xC0;
}

void np_bool(uint8_t **p, int n)
{
    *(*p)++ = n ? 0xC3 : 0xC2;
}

void np_str(uint8_t **p, const char *s)
{
    size_t n = np_strlen(s);

    if (n < 0x20) {
        *(*p)++ = 0xA0 | (n & 0x1F);
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

    np_memcpy(*p, s, n);
    *p += n;
}

#if NANOPACK_HAS_FP != 0
void np_float(uint8_t **p, float n)
{
    /* unimplemented */
}
#endif

