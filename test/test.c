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
int i, j;

#define MAX_DUMP_LENGTH 64

#define TEST_GROUP_BEGIN(group)                                     \
pass_two = 0;                                                       \
do {                                                                \
    err_group = (group);                                            \
    err_count = 0;                                                  \
    test_count = 0;

#define DUMP(id)                                                    \
do {                                                                \
    printf("  line %d\n", __LINE__);                                \
    printf("      found: ");                                        \
    dump_array(w, id.len);                                          \
    printf("     expect: ");                                        \
    dump_array(_s, sizeof(_s));                                     \
} while (0)

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
            DUMP(_b);                                               \
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
            DUMP(_b);                                               \
        }                                                           \
    }                                                               \
} while(0)

/* Use `buf` as the np_buf argument. */
#define TEST_COMPLEX(string, statements)                            \
do {                                                                \
    np_buf buf = np_make_buf(w, sizeof(w));                         \
    const uint8_t _s[] = string;                                    \
    statements                                                      \
    test_count += 1;                                                \
                                                                    \
    if (memcmp(w, _s, sizeof(_s) - 1)) {                            \
        if (!pass_two) {                                            \
            err_count += 1;                                         \
            global_err_count += 1;                                  \
        }                                                           \
        else {                                                      \
            DUMP(buf);                                              \
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
        continue;                                                   \
    }                                                               \
    else {                                                          \
        if (pass_two) {                                             \
            printf("\n\n");                                         \
        }                                                           \
        break;                                                      \
    }                                                               \
} while (1)



static void dump_array(const uint8_t *a, size_t l)
{
    size_t i;
    for (i = 0; i < l; ++i) {
        if (i > MAX_DUMP_LENGTH) {
            printf("...");
            break;
        }

        printf("%02X ", a[i]);
    }
    printf("\n");
}

int main(void)
{
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

    TEST_GROUP_BEGIN("misc");
    TEST_SIMPLE0("\xc0", np_nil);
    TEST_SIMPLE1("\xc2", np_bool, 0);
    TEST_SIMPLE1("\xc3", np_bool, 1);
    TEST_GROUP_END();

    TEST_GROUP_BEGIN("basic fixarr/arr16/arr32");

    TEST_COMPLEX(
        "\x90",
        np_arr(&buf, 0);
    );

    TEST_COMPLEX(
        "\x91\xc0",
        np_arr(&buf, 1);
        np_nil(&buf);
    );

    TEST_COMPLEX(
        "\x9f\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e",
        np_arr(&buf, 15);
        for (i = 0; i < 15; ++i) {
            np_i32(&buf, i);
        }
    );

    TEST_COMPLEX(
        "\xdc\x00\x10\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c"
        "\x0d\x0e\x0f",
        np_arr(&buf, 16);
        for (i = 0; i < 16; ++i) {
            np_u32(&buf, i);
        }
    );

    TEST_GROUP_END();

    TEST_GROUP_BEGIN("basic fixmap/map16/map32");

    TEST_COMPLEX(
        "\x80",
        np_map(&buf, 0);
    );

    TEST_COMPLEX(
        "\x81\xc0\xc0",
        np_map(&buf, 2);
        np_nil(&buf);
        np_nil(&buf);
    );

    TEST_COMPLEX(
        "\x82\x00\x00\x01\x01",
        np_map(&buf, 4);
        np_i8(&buf, 0);
        np_i16(&buf, 0);
        np_u8(&buf, 1);
        np_u16(&buf, 1);
    );

    TEST_COMPLEX(
        "\x8f\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e"
        "\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d",
        np_map(&buf, 30);
        for (i = 0; i < 30; ++i) {
            np_i8(&buf, i);
        }
    );

    TEST_COMPLEX(
        "\xde\x00\x10\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c"
        "\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c"
        "\x1d\x1e\x1f",
        np_map(&buf, 32);
        for (i = 0; i < 32; ++i) {
            np_int(&buf, i);
        }
    );

    TEST_GROUP_END();

    TEST_GROUP_BEGIN("nested arr");

    TEST_COMPLEX(
        "\x91\x90",
        np_arr(&buf, 1);
            np_arr(&buf, 0);
    );

    TEST_COMPLEX(
        "\x93\x90\x91\x00\x92\x01\x02",
        np_arr(&buf, 3);
            np_arr(&buf, 0);
            np_arr(&buf, 1);
                np_int(&buf, 0);
            np_arr(&buf, 2);
                np_int(&buf, 1);
                np_int(&buf, 2);
    );

    TEST_COMPLEX(
        "\x95\x90\x91\xc0\x92\x90\x91\xc0\x9f\x00\x01\x02\x03\x04\x05\x06"
        "\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\xdc\x00\x10\x00\x01\x02\x03\x04"
        "\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f",
        np_arr(&buf, 5);
            np_arr(&buf, 0);
            np_arr(&buf, 1);
                np_nil(&buf);
            np_arr(&buf, 2);
                np_arr(&buf, 0);
                np_arr(&buf, 1);
                    np_nil(&buf);
            np_arr(&buf, 15);
                for (i = 0; i < 15; ++i) {
                    np_int(&buf, i);
                }
            np_arr(&buf, 16);
                for (i = 0; i < 16; ++i) {
                    np_int(&buf, i);
                }
    );

    TEST_GROUP_END();

    TEST_GROUP_BEGIN("nested map");

    TEST_COMPLEX(
        "\x85\x00\x80\x01\x81\x00\xc0\x02\x82\x00\x80\x01\x81\xc0\xc0\x03"
        "\x8f\x00\x00\x01\x01\x02\x02\x03\x03\x04\x04\x05\x05\x06\x06\x07"
        "\x07\x08\x08\x09\x09\x0a\x0a\x0b\x0b\x0c\x0c\x0d\x0d\x0e\x0e\x04"
        "\xde\x00\x10\x00\x00\x01\x01\x02\x02\x03\x03\x04\x04\x05\x05\x06"
        "\x06\x07\x07\x08\x08\x09\x09\x0a\x0a\x0b\x0b\x0c\x0c\x0d\x0d\x0e"
        "\x0e\x0f\x0f",
        np_map(&buf, 10);
            np_int(&buf, 0);
            np_map(&buf, 0);
            np_int(&buf, 1);
            np_map(&buf, 2);
                np_int(&buf, 0);
                np_nil(&buf);
            np_int(&buf, 2);
            np_map(&buf, 4);
                np_int(&buf, 0);
                np_map(&buf, 0);
                np_int(&buf, 1);
                np_map(&buf, 2);
                    np_nil(&buf);
                    np_nil(&buf);
            np_int(&buf, 3);
            np_map(&buf, 30);
                for (i = 0; i < 15; ++i) {
                    np_int(&buf, i);
                    np_int(&buf, i);
                }
            np_int(&buf, 4);
            np_map(&buf, 32);
                for (i = 0; i < 16; ++i) {
                    np_int(&buf, i);
                    np_int(&buf, i);
                }
    );

    TEST_COMPLEX(
       "\x85\xd0\xd1\x91\xc0\x90\x81\xc0\x00\xc0\x82\xc0\x90\x04\x05\xa5"
        "\x68\x65\x6c\x6c\x6f\x93\xa7\x62\x6f\x6e\x6a\x6f\x75\x72\xc0\xff"
        "\x91\x5c\xcd\x01\x5e",
        np_map(&buf, 10);
            np_int(&buf, -47);
            np_arr(&buf, 1);
                np_nil(&buf);
            np_arr(&buf, 0);
            np_map(&buf, 2);
                np_nil(&buf);
                np_int(&buf, 0);
            np_nil(&buf);
            np_map(&buf, 4);
                np_nil(&buf);
                np_arr(&buf, 0);
                np_int(&buf, 4);
                np_int(&buf, 5);
            np_str(&buf, "hello");
            np_arr(&buf, 3);
                /* TODO: Consider str_or_nil */
                np_str(&buf, "bonjour");
                np_nil(&buf);
                np_int(&buf, -1);
            np_arr(&buf, 1);
                np_int(&buf, 92);
            np_int(&buf, 350);
    );

    TEST_GROUP_END();

    return global_err_count;
}
