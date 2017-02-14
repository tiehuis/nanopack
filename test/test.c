/*
 * Test vectors are derived from
 * `https://github.com/ludocode/mpack/blob/v0.8.2/test/test-write.c`.
 *
 * Note: Currently requires a C99 compatible compiler for varargs. Change!
 */

#include <string.h>
#include <stdio.h>
#include "nanopack.h"

char *err_group = "ERR!";
int err_count = 0;
int test_count = 0;
uint8_t w[4096];

#define TEST_GROUP_BEGIN(group)                 \
do {                                            \
    err_group = (group);                        \
    err_count = 0;                              \
    test_count = 0;

#define TEST_GROUP_END()                                        \
    printf("%s: [%s] %d of %d passed\n",                        \
           err_count ? "FAILURE" : "SUCCESS", err_group,        \
           test_count - err_count, test_count);                 \
} while (0)

/* Note: subtract one from string to avoid comparing null-terminator. */
#define TEST_CHECK(string)                      \
{                                               \
    const uint8_t _s[] = string;                \
    test_count += 1;                            \
    if (memcmp(w, _s, sizeof(_s) - 1)) {        \
        err_count += 1;                         \
    }                                           \
}

#define TEST_SIMPLE(string, fn, ...)            \
do {                                            \
    uint8_t *_b = w;                            \
    fn(&_b, __VA_ARGS__);                       \
    TEST_CHECK(string);                         \
} while(0)

int main(void)
{
    TEST_GROUP_BEGIN("np_u8");
    TEST_SIMPLE("\xcc\x80", np_u8, 0x80);
    TEST_SIMPLE("\xcc\xff", np_u8, 0xff);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("np_u16");
    TEST_SIMPLE("\xcd\x01\x00", np_u16, 0x100);
    TEST_SIMPLE("\xcd\xff\xff", np_u16, 0xffff);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("np_u32");
    TEST_SIMPLE("\xce\x00\x01\x00\x00", np_u32, 0x10000);
    TEST_SIMPLE("\xce\xff\xff\xff\xff", np_u32, 0xffffffff);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("np_u64");
    TEST_SIMPLE("\xcf\x00\x00\x00\x01\x00\x00\x00\x00", np_u64, 0x100000000ULL);
    TEST_SIMPLE("\xcf\xff\xff\xff\xff\xff\xff\xff\xff", np_u64, 0xFFFFFFFFFFFFFFFFULL);
    TEST_GROUP_END();

    return 0;
}
