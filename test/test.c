/*
 * Test vectors are derived from
 * `https://github.com/ludocode/mpack/blob/v0.8.2/test/test-write.c`.
 *
 * Note: Current requires a C99 compiler for long-long suffixes.
 */

#include <string.h>
#include <stdio.h>
#include "nanopack.h"

char *err_group = "ERR!";
int err_count = 0;
int global_err_count = 0;
int test_count = 0;
int pass_two = 0;
uint8_t w[4096];

#define TEST_GROUP_BEGIN(group)                                     \
pass_two = 0;                                                       \
do {                                                                \
    err_group = (group);                                            \
    err_count = 0;                                                  \
    test_count = 0;

/* Note: subtract one from string to avoid comparing null-terminator. */
#define TEST_SIMPLE0(string, fn)                                    \
do {                                                                \
    np_buf _b = np_make_buf(w, sizeof(w));                          \
    const uint8_t _s[] = string;                                    \
    fn(&_b);                                                        \
    test_count += 1;                                                \
                                                                    \
    if (memcmp(w, _s, sizeof(_s) - 1)) {                            \
        if (!pass_two) {                                            \
            err_count += 1;                                         \
            global_err_count += 1;                                  \
        }                                                           \
        else {                                                      \
            printf("%d ", __LINE__);                                \
        }                                                           \
    }                                                               \
} while(0)

#define TEST_SIMPLE1(string, fn, arg)                               \
do {                                                                \
    np_buf _b = np_make_buf(w, sizeof(w));                          \
    const uint8_t _s[] = string;                                    \
    fn(&_b, arg);                                                   \
    test_count += 1;                                                \
                                                                    \
    if (memcmp(w, _s, sizeof(_s) - 1)) {                            \
        if (!pass_two) {                                            \
            err_count += 1;                                         \
            global_err_count += 1;                                  \
        }                                                           \
        else {                                                      \
            printf("%d ", __LINE__);                                \
        }                                                           \
    }                                                               \
} while(0)

#define TEST_GROUP_END()                                            \
    if (!pass_two) {                                                \
        printf("%s: [%s] %d of %d passed\n",                        \
               err_count ? "FAILURE" : "SUCCESS", err_group,        \
               test_count - err_count, test_count);                 \
    }                                                               \
    if (err_count) {                                                \
        pass_two = 1;                                               \
        printf("  !! failures: ");                                  \
        continue;                                                   \
    }                                                               \
    else {                                                          \
        if (pass_two) {                                             \
            printf("\n\n");                                         \
        }                                                           \
        break;                                                      \
    }                                                               \
} while (1)


int main(void)
{
#if 0
    uint8_t b[64];
    np_buf t = np_make_buf(b, sizeof(b));
    np_i8(&t, -128);
    for (size_t i = 0; i < t.len; ++i) {
        printf("%02X ", b[i]);
    }
    printf("\n");
    return 0;
#endif


    TEST_GROUP_BEGIN("+fixnum \\w +functions");
    TEST_SIMPLE1("\x00", np_u8, 0);
    TEST_SIMPLE1("\x01", np_u8, 1);
    TEST_SIMPLE1("\x02", np_u8, 2);
    TEST_SIMPLE1("\x0f", np_u8, 0x0f);
    TEST_SIMPLE1("\x10", np_u8, 0x10);
    TEST_SIMPLE1("\x7f", np_u8, 0x7f);
    TEST_SIMPLE1("\x00", np_u16, 0);
    TEST_SIMPLE1("\x01", np_u16, 1);
    TEST_SIMPLE1("\x02", np_u16, 2);
    TEST_SIMPLE1("\x0f", np_u16, 0x0f);
    TEST_SIMPLE1("\x10", np_u16, 0x10);
    TEST_SIMPLE1("\x7f", np_u16, 0x7f);
    TEST_SIMPLE1("\x00", np_u32, 0);
    TEST_SIMPLE1("\x01", np_u32, 1);
    TEST_SIMPLE1("\x02", np_u32, 2);
    TEST_SIMPLE1("\x0f", np_u32, 0x0f);
    TEST_SIMPLE1("\x10", np_u32, 0x10);
    TEST_SIMPLE1("\x7f", np_u32, 0x7f);
    TEST_SIMPLE1("\x00", np_u64, 0);
    TEST_SIMPLE1("\x01", np_u64, 1);
    TEST_SIMPLE1("\x02", np_u64, 2);
    TEST_SIMPLE1("\x0f", np_u64, 0x0f);
    TEST_SIMPLE1("\x10", np_u64, 0x10);
    TEST_SIMPLE1("\x7f", np_u64, 0x7f);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("+fixnum \\w -functions");
    TEST_SIMPLE1("\x00", np_i8, 0);
    TEST_SIMPLE1("\x01", np_i8, 1);
    TEST_SIMPLE1("\x02", np_i8, 2);
    TEST_SIMPLE1("\x0f", np_i8, 0x0f);
    TEST_SIMPLE1("\x10", np_i8, 0x10);
    TEST_SIMPLE1("\x7f", np_i8, 0x7f);
    TEST_SIMPLE1("\x00", np_i16, 0);
    TEST_SIMPLE1("\x01", np_i16, 1);
    TEST_SIMPLE1("\x02", np_i16, 2);
    TEST_SIMPLE1("\x0f", np_i16, 0x0f);
    TEST_SIMPLE1("\x10", np_i16, 0x10);
    TEST_SIMPLE1("\x7f", np_i16, 0x7f);
    TEST_SIMPLE1("\x00", np_i32, 0);
    TEST_SIMPLE1("\x01", np_i32, 1);
    TEST_SIMPLE1("\x02", np_i32, 2);
    TEST_SIMPLE1("\x0f", np_i32, 0x0f);
    TEST_SIMPLE1("\x10", np_i32, 0x10);
    TEST_SIMPLE1("\x7f", np_i32, 0x7f);
    TEST_SIMPLE1("\x00", np_i64, 0);
    TEST_SIMPLE1("\x01", np_i64, 1);
    TEST_SIMPLE1("\x02", np_i64, 2);
    TEST_SIMPLE1("\x0f", np_i64, 0x0f);
    TEST_SIMPLE1("\x10", np_i64, 0x10);
    TEST_SIMPLE1("\x7f", np_i64, 0x7f);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("-fixnum");
    TEST_SIMPLE1("\xff", np_i8, -1);
    TEST_SIMPLE1("\xfe", np_i8, -2);
    TEST_SIMPLE1("\xf0", np_i8, -16);
    TEST_SIMPLE1("\xe0", np_i8, -32);
    TEST_SIMPLE1("\xff", np_i16, -1);
    TEST_SIMPLE1("\xfe", np_i16, -2);
    TEST_SIMPLE1("\xf0", np_i16, -16);
    TEST_SIMPLE1("\xe0", np_i16, -32);
    TEST_SIMPLE1("\xff", np_i32, -1);
    TEST_SIMPLE1("\xfe", np_i32, -2);
    TEST_SIMPLE1("\xf0", np_i32, -16);
    TEST_SIMPLE1("\xe0", np_i32, -32);
    TEST_SIMPLE1("\xff", np_i64, -1);
    TEST_SIMPLE1("\xfe", np_i64, -2);
    TEST_SIMPLE1("\xf0", np_i64, -16);
    TEST_SIMPLE1("\xe0", np_i64, -32);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("uint");
    TEST_SIMPLE1("\xcc\x80", np_u8, 0x80);
    TEST_SIMPLE1("\xcc\xff", np_u8, 0xff);
    TEST_SIMPLE1("\xcc\x80", np_u16, 0x80);
    TEST_SIMPLE1("\xcc\xff", np_u16, 0xff);
    TEST_SIMPLE1("\xcd\x01\x00", np_u16, 0x100);
    TEST_SIMPLE1("\xcd\xff\xff", np_u16, 0xffff);
    TEST_SIMPLE1("\xcc\x80", np_u32, 0x80);
    TEST_SIMPLE1("\xcc\xff", np_u32, 0xff);
    TEST_SIMPLE1("\xcd\x01\x00", np_u32, 0x100);
    TEST_SIMPLE1("\xcd\xff\xff", np_u32, 0xffff);
    TEST_SIMPLE1("\xce\x00\x01\x00\x00", np_u32, 0x10000);
    TEST_SIMPLE1("\xce\xff\xff\xff\xff", np_u32, 0xffffffff);
    TEST_SIMPLE1("\xcc\x80", np_u64, 0x80);
    TEST_SIMPLE1("\xcc\xff", np_u64, 0xff);
    TEST_SIMPLE1("\xcd\x01\x00", np_u64, 0x100);
    TEST_SIMPLE1("\xcd\xff\xff", np_u64, 0xffff);
    TEST_SIMPLE1("\xce\x00\x01\x00\x00", np_u64, 0x10000);
    TEST_SIMPLE1("\xce\xff\xff\xff\xff", np_u64, 0xffffffff);
    TEST_SIMPLE1("\xcf\x00\x00\x00\x01\x00\x00\x00\x00", np_u64, 0x100000000ull);
    TEST_SIMPLE1("\xcf\xff\xff\xff\xff\xff\xff\xff\xff", np_u64, 0xffffffffffffffffull);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("+int");
    TEST_SIMPLE1("\xcc\x80", np_i16, 0x80);
    TEST_SIMPLE1("\xcc\xff", np_i16, 0xff);
    TEST_SIMPLE1("\xcd\x01\x00", np_i16, 0x100);
    TEST_SIMPLE1("\xcd\x7f\xff", np_i16, 0x7fff);
    TEST_SIMPLE1("\xcc\x80", np_i32, 0x80);
    TEST_SIMPLE1("\xcc\xff", np_i32, 0xff);
    TEST_SIMPLE1("\xcd\x01\x00", np_i32, 0x100);
    TEST_SIMPLE1("\xcd\x7f\xff", np_i32, 0x7fff);
    TEST_SIMPLE1("\xcd\xff\xff", np_i32, 0xffff);
    TEST_SIMPLE1("\xce\x00\x01\x00\x00", np_i32, 0x10000);
    TEST_SIMPLE1("\xce\x7f\xff\xff\xff", np_i32, 0x7fffffff);
    TEST_SIMPLE1("\xcc\x80", np_i64, 0x80);
    TEST_SIMPLE1("\xcc\xff", np_i64, 0xff);
    TEST_SIMPLE1("\xcd\x01\x00", np_i64, 0x100);
    TEST_SIMPLE1("\xcd\x7f\xff", np_i64, 0x7fff);
    TEST_SIMPLE1("\xcd\xff\xff", np_i64, 0xffff);
    TEST_SIMPLE1("\xce\x00\x01\x00\x00", np_i64, 0x10000);
    TEST_SIMPLE1("\xce\x7f\xff\xff\xff", np_i64, 0x7fffffffll);
    TEST_SIMPLE1("\xce\xff\xff\xff\xff", np_i64, 0xffffffffll);
    TEST_SIMPLE1("\xcf\x00\x00\x00\x01\x00\x00\x00\x00", np_i64, 0x100000000ll);
    TEST_SIMPLE1("\xcf\x7f\xff\xff\xff\xff\xff\xff\xff", np_i64, 0x7fffffffffffffffll);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("-int");
    TEST_SIMPLE1("\xd0\xdf", np_i8, -33);
    TEST_SIMPLE1("\xd0\x80", np_i8, -128);
    TEST_SIMPLE1("\xd0\xdf", np_i16, -33);
    TEST_SIMPLE1("\xd0\x80", np_i16, -128);
    TEST_SIMPLE1("\xd1\xff\x7f", np_i16, -129);
    TEST_SIMPLE1("\xd1\x80\x00", np_i16, -32768);
    TEST_SIMPLE1("\xd0\xdf", np_i32, -33);
    TEST_SIMPLE1("\xd0\x80", np_i32, -128);
    TEST_SIMPLE1("\xd1\xff\x7f", np_i32, -129);
    TEST_SIMPLE1("\xd1\x80\x00", np_i32, -32768);
    TEST_SIMPLE1("\xd2\xff\xff\x7f\xff", np_i32, -32769);
    TEST_SIMPLE1("\xd2\x80\x00\x00\x00", np_i32, -2147483648ll);
    TEST_SIMPLE1("\xd2\x80\x00\x00\x00", np_i64, -2147483648ll);
    TEST_SIMPLE1("\xd0\xdf", np_i64, -33);
    TEST_SIMPLE1("\xd0\x80", np_i64, -128);
    TEST_SIMPLE1("\xd1\xff\x7f", np_i64, -129);
    TEST_SIMPLE1("\xd1\x80\x00", np_i64, -32768);
    TEST_SIMPLE1("\xd2\xff\xff\x7f\xff", np_i64, -32769);
    TEST_SIMPLE1("\xd3\xff\xff\xff\xff\x7f\xff\xff\xff", np_i64, -2147483649ll);
    TEST_SIMPLE1("\xd3\x80\x00\x00\x00\x00\x00\x00\x00", np_i64, INT64_MIN);
    TEST_GROUP_END();

    return global_err_count;
}
