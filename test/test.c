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

/* Note: subtract one from string to avoid comparing null-terminator. */
#define TEST_SIMPLE(string, fn, ...)                                \
do {                                                                \
    np_buf _b = np_make_buf(w, sizeof(w));                          \
    const uint8_t _s[] = string;                                    \
    fn(&_b, __VA_ARGS__);                                           \
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

int main(void)
{
    TEST_GROUP_BEGIN("+fixnum \\w +functions");
    TEST_SIMPLE("\x00", np_u8, 0);
    TEST_SIMPLE("\x01", np_u8, 1);
    TEST_SIMPLE("\x02", np_u8, 2);
    TEST_SIMPLE("\x0f", np_u8, 0x0f);
    TEST_SIMPLE("\x10", np_u8, 0x10);
    TEST_SIMPLE("\x7f", np_u8, 0x7f);
    TEST_SIMPLE("\x00", np_u16, 0);
    TEST_SIMPLE("\x01", np_u16, 1);
    TEST_SIMPLE("\x02", np_u16, 2);
    TEST_SIMPLE("\x0f", np_u16, 0x0f);
    TEST_SIMPLE("\x10", np_u16, 0x10);
    TEST_SIMPLE("\x7f", np_u16, 0x7f);
    TEST_SIMPLE("\x00", np_u32, 0);
    TEST_SIMPLE("\x01", np_u32, 1);
    TEST_SIMPLE("\x02", np_u32, 2);
    TEST_SIMPLE("\x0f", np_u32, 0x0f);
    TEST_SIMPLE("\x10", np_u32, 0x10);
    TEST_SIMPLE("\x7f", np_u32, 0x7f);
    TEST_SIMPLE("\x00", np_u64, 0);
    TEST_SIMPLE("\x01", np_u64, 1);
    TEST_SIMPLE("\x02", np_u64, 2);
    TEST_SIMPLE("\x0f", np_u64, 0x0f);
    TEST_SIMPLE("\x10", np_u64, 0x10);
    TEST_SIMPLE("\x7f", np_u64, 0x7f);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("+fixnum \\w -functions");
    TEST_SIMPLE("\x00", np_i8, 0);
    TEST_SIMPLE("\x01", np_i8, 1);
    TEST_SIMPLE("\x02", np_i8, 2);
    TEST_SIMPLE("\x0f", np_i8, 0x0f);
    TEST_SIMPLE("\x10", np_i8, 0x10);
    TEST_SIMPLE("\x7f", np_i8, 0x7f);
    TEST_SIMPLE("\x00", np_i16, 0);
    TEST_SIMPLE("\x01", np_i16, 1);
    TEST_SIMPLE("\x02", np_i16, 2);
    TEST_SIMPLE("\x0f", np_i16, 0x0f);
    TEST_SIMPLE("\x10", np_i16, 0x10);
    TEST_SIMPLE("\x7f", np_i16, 0x7f);
    TEST_SIMPLE("\x00", np_i32, 0);
    TEST_SIMPLE("\x01", np_i32, 1);
    TEST_SIMPLE("\x02", np_i32, 2);
    TEST_SIMPLE("\x0f", np_i32, 0x0f);
    TEST_SIMPLE("\x10", np_i32, 0x10);
    TEST_SIMPLE("\x7f", np_i32, 0x7f);
    TEST_SIMPLE("\x00", np_i64, 0);
    TEST_SIMPLE("\x01", np_i64, 1);
    TEST_SIMPLE("\x02", np_i64, 2);
    TEST_SIMPLE("\x0f", np_i64, 0x0f);
    TEST_SIMPLE("\x10", np_i64, 0x10);
    TEST_SIMPLE("\x7f", np_i64, 0x7f);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("-fixnum");
    TEST_SIMPLE("\xff", np_i8, -1);
    TEST_SIMPLE("\xfe", np_i8, -2);
    TEST_SIMPLE("\xf0", np_i8, -16);
    TEST_SIMPLE("\xe0", np_i8, -32);
    TEST_SIMPLE("\xff", np_i16, -1);
    TEST_SIMPLE("\xfe", np_i16, -2);
    TEST_SIMPLE("\xf0", np_i16, -16);
    TEST_SIMPLE("\xe0", np_i16, -32);
    TEST_SIMPLE("\xff", np_i32, -1);
    TEST_SIMPLE("\xfe", np_i32, -2);
    TEST_SIMPLE("\xf0", np_i32, -16);
    TEST_SIMPLE("\xe0", np_i32, -32);
    TEST_SIMPLE("\xff", np_i64, -1);
    TEST_SIMPLE("\xfe", np_i64, -2);
    TEST_SIMPLE("\xf0", np_i64, -16);
    TEST_SIMPLE("\xe0", np_i64, -32);
    TEST_GROUP_END();

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

    return global_err_count;
}
