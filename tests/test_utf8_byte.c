/**
 * @file utf8/tests/test_utf8_byte.c
 */

#include <stdlib.h>
#include <string.h>

#include "byte.h"
#include "test.h"

typedef struct TestUTF8ByteCount {
    const char* label;
    const uint8_t* src;
    const int64_t expected;
} TestUTF8ByteCount;

int test_group_utf8_byte_count(TestUnit* unit) {
    TestUTF8ByteCount* data = (TestUTF8ByteCount*) unit->data;
    int64_t actual = utf8_byte_count(data->src);

    // Check if the actual count matches the expected count
    ASSERT_EQ(
        actual,
        data->expected,
        "[TestUTF8ByteCount] Failed: unit=%zu, src='%s', expected=%ld, got=%ld",
        unit->index,
        data->src,
        data->expected,
        actual
    );

    return 0;  // Success
}

int test_suite_utf8_byte_count(void) {
    TestUTF8ByteCount data[] = {
        {"NULL", NULL, -1},
        {"Empty", (const uint8_t*) "", 0},
        {"ASCII NULL", (const uint8_t*) "\0", 0},
        {"ASCII a", (const uint8_t*) "a", 1},
        {"ASCII DEL", (const uint8_t*) "\x7F", 1},
        {"2-byte ¢", (const uint8_t*) "\u00A2", 2},
        {"3-byte €", (const uint8_t*) "\u20AC", 3},
        {"4-byte 😀", (const uint8_t*) "\U0001F600", 4},
        {"Hello", (const uint8_t*) "Hello!", 6},
    };
    size_t count = sizeof(data) / sizeof(TestUTF8ByteCount);

    TestUnit units[count];
    for (size_t i = 0; i < count; i++) {
        units[i].data = &data[i];
    }

    TestGroup group = {
        .name = "utf8_byte_count",
        .count = count,
        .units = units,
        .run = test_group_utf8_byte_count,
    };

    return test_group_run(&group);
}

typedef struct TestUTF8ByteDiff {
    const char* label;
    const uint8_t* src;
    const ptrdiff_t start;
    const ptrdiff_t end;
    const ptrdiff_t expected;
} TestUTF8ByteDiff;

int test_group_utf8_byte_diff(TestUnit* unit) {
    TestUTF8ByteDiff* data = (TestUTF8ByteDiff*) unit->data;
    const uint8_t* start = data->src ? data->src + data->start : NULL;
    const uint8_t* end = data->src ? data->src + data->end : NULL;
    ptrdiff_t actual = utf8_byte_diff(start, end);

    ASSERT_EQ(
        actual,
        data->expected,
        "[TestUTF8ByteDiff] Failed: unit=%zu, src='%s', expected=%ld, got=%ld",
        unit->index,
        data->src ? (char*) data->src : "NULL",
        data->expected,
        actual
    );

    return 0;
}

int test_suite_utf8_byte_diff(void) {
    TestUTF8ByteDiff data[] = {
        {"NULL start", NULL, 0, 1, -1},  // nul input
        {"start == end", (uint8_t*) "abc", 0, 0, 0},  // no diff
        {"end < start", (uint8_t*) "abc", 2, 1, -1},
        {"diff 0", (uint8_t*) "abc", 0, 0, 0},
        {"diff 2", (uint8_t*) "abc", 0, 2, 2},
        {"full string", (uint8_t*) "Hello!", 0, 5, 5},
    };
    size_t count = sizeof(data) / sizeof(TestUTF8ByteDiff);

    TestUnit units[count];
    for (size_t i = 0; i < count; i++) {
        units[i].data = &data[i];
    }

    TestGroup group = {
        .name = "utf8_byte_diff",
        .count = count,
        .units = units,
        .run = test_group_utf8_byte_diff,
    };

    return test_group_run(&group);
}

typedef struct TestUTF8ByteCopy {
    const char* label;
    const uint8_t* src;
    const uint8_t* expected;
} TestUTF8ByteCopy;

int test_group_utf8_byte_copy(TestUnit* unit) {
    TestUTF8ByteCopy* data = (TestUTF8ByteCopy*) unit->data;
    uint8_t* actual = utf8_byte_copy(data->src);

    // Check NULL sets
    if (NULL == data->src) {
        ASSERT_EQ(
            actual,
            NULL,
            "[TestUTF8ByteCopy]Failed: unit=%zu, input=NULL, expected NULL, got non-NULL",
            unit->index
        );
    }

    // Compare length
    int64_t count = utf8_byte_count(data->src);
    int64_t actual_count = utf8_byte_count(actual);

    ASSERT_EQ(
        actual_count,
        count,
        "[TestUTF8ByteCopy] Failed: unit=%zu, input='%s', expected length=%ld, got=%ld",
        unit->index,
        data->src ? (char*) data->src : "NULL",
        count,
        actual_count
    );

    // Compare contents (including null byte)
    for (int64_t i = 0; i <= count; i++) {
        ASSERT_EQ(
            actual[i],
            data->src[i],
            "[TestUTF8ByteCopy] Failed: unit=%zu, index=%ld, expected=%c, got=%c",
            unit->index,
            i,
            (unsigned char) data->expected[i],
            (unsigned char) actual[i]
        );
    }

    // NULL is an aliased pointer
    if (actual != NULL && data->src != NULL) {
        // Test for aliasing
        ASSERT_NEQ(
            (uintptr_t) actual,
            (uintptr_t) data->src,
            "[TestUTF8ByteCopy] Failed: unit=%zu, returned pointer alias input (shallow copy)",
            unit->index
        );
    }

    free(actual);
    return 0;
}

int test_suite_utf8_byte_copy(void) {
    TestUTF8ByteCopy data[] = {
        {"NULL", NULL, NULL},
        {"Empty", (uint8_t*) "", (uint8_t*) ""},
        {"ASCII", (uint8_t*) "abc", (uint8_t*) "abc"},
        {"2-byte", (uint8_t*) "\u00A2", (uint8_t*) "\u00A2"},
        {"Long", (uint8_t*) "Hello, world!", (uint8_t*) "Hello, world!"},
    };
    size_t count = sizeof(data) / sizeof(TestUTF8ByteCopy);

    TestUnit units[count];
    for (size_t i = 0; i < count; i++) {
        units[i].data = &data[i];
    }

    TestGroup group = {
        .name = "utf8_byte_copy",
        .count = count,
        .units = units,
        .run = test_group_utf8_byte_copy,
    };

    return test_group_run(&group);
}

typedef struct TestUTF8ByteCopyN {
    const char* label;
    const uint8_t* src;
    uint64_t n;
    const uint8_t* expected;
} TestUTF8ByteCopyN;

int test_group_utf8_byte_copy_n(TestUnit* unit) {
    TestUTF8ByteCopyN* data = (TestUTF8ByteCopyN*) unit->data;
    uint8_t* actual = utf8_byte_copy_n(data->src, data->n);

    if (actual && data->src) {
        // Compare output byte-by-byte
        int64_t count = utf8_byte_count(data->expected);
        int64_t result = memcmp(actual, data->expected, count + 1);

        ASSERT_EQ(
            result,
            0,  // expected
            "[TestUTF8ByteCopyN] Failed: unit=%zu, label=%s, expected='%s', got='%s'",
            unit->index,
            data->label,
            data->expected,
            actual
        );

        // No aliasing
        ASSERT_NEQ(
            (uintptr_t) actual,
            (uintptr_t) data->src,
            "[TestUTF8ByteCopyN] Failed: unit=%zu, label=%s, result is aliased",
            unit->index,
            data->label
        );
    } else {
        // Expect failure (NULL result)
        ASSERT_EQ(
            actual,
            data->expected,
            "[TestUTF8ByteCopyN] Failed: unit=%zu, label=%s, expected='%s', got '%s'",
            unit->index,
            data->label,
            data->expected ? (char*) data->expected : "NULL",
            actual ? (char*) actual : "NULL"
        );
    }

    free(actual);
    return 0;
}

int test_suite_utf8_byte_copy_n(void) {
    TestUTF8ByteCopyN data[] = {
        {"NULL", NULL, 1, NULL},
        {"Empty, n=0", (uint8_t*) "", 0, (uint8_t*) ""},
        {"Empty, n=1", (uint8_t*) "", 1, NULL},
        {"ASCII full", (uint8_t*) "abc", 3, (uint8_t*) "abc"},
        {"ASCII partial", (uint8_t*) "abc", 2, (uint8_t*) "ab"},
        {"ASCII n=0", (const uint8_t*) "abc", 0, (const uint8_t*) ""},
        {"n > len", (const uint8_t*) "abc", 5, NULL},
        {"Multi-byte", (const uint8_t*) "\u20ACabc", 3, (const uint8_t*) "\u20AC"},  // euro sign
        {"Multi-byte partial", (const uint8_t*) "\u20ACabc", 4, (const uint8_t*) "\u20ACa"},
    };
    size_t count = sizeof(data) / sizeof(TestUTF8ByteCopyN);

    TestUnit units[count];
    for (size_t i = 0; i < count; i++) {
        units[i].data = &data[i];
    }

    TestGroup group = {
        .name = "utf8_byte_copy_n",
        .count = count,
        .units = units,
        .run = test_group_utf8_byte_copy_n,
    };

    return test_group_run(&group);
}

typedef struct TestUTF8ByteCopySlice {
    const char* label;
    const uint8_t* src;
    const ptrdiff_t start;
    const ptrdiff_t end;
    const uint8_t* expected;
} TestUTF8ByteCopySlice;

int test_group_utf8_byte_copy_slice(TestUnit* unit) {
    TestUTF8ByteCopySlice* data = (TestUTF8ByteCopySlice*) unit->data;
    const uint8_t* start = data->src ? data->src + data->start : NULL;
    const uint8_t* end = data->src ? data->src + data->end : NULL;
    uint8_t* actual = utf8_byte_copy_slice(start, end);

    if (actual && data->src) {
        int64_t expected_len = utf8_byte_count(data->expected);
        int64_t actual_len = utf8_byte_count(actual);
        ASSERT_EQ(
            actual_len,
            expected_len,
            "[TestUTF8ByteCopySlice] Failed: unit=%zu, label=%s, expected len=%ld, got=%ld",
            unit->index,
            data->label,
            expected_len,
            actual_len
        );

        int64_t result = memcmp(actual, data->expected, expected_len + 1);
        ASSERT_EQ(
            result,
            0,  // expected
            "[TestUTF8ByteCopySlice] Failed: unit=%zu, label=%s, expected='%s', got='%s'",
            unit->index,
            data->label,
            data->expected,
            actual
        );

        // No aliasing
        ASSERT_NEQ(
            (uintptr_t) actual,
            (uintptr_t) data->src,
            "[TestUTF8ByteCopySlice] Failed: unit=%zu, label=%s, result is aliased",
            unit->index,
            data->label
        );
    } else {
        // Expect failure (NULL result)
        ASSERT_EQ(
            actual,
            data->expected,
            "[TestUTF8ByteCopySlice] Failed: unit=%zu, label=%s, expected='%s', got '%s'",
            unit->index,
            data->label,
            data->expected ? (char*) data->expected : "NULL",
            actual ? (char*) actual : "NULL"
        );
    }

    free(actual);
    return 0;
}

int test_suite_utf8_byte_copy_slice(void) {
    TestUTF8ByteCopySlice data[] = {0};
    size_t count = sizeof(data) / sizeof(TestUTF8ByteCopySlice);

    TestUnit units[count];
    for (size_t i = 0; i < count; i++) {
        units[i].data = &data[i];
    }

    TestGroup group = {
        .name = "utf8_byte_slice",
        .count = count,
        .units = units,
        .run = test_group_utf8_byte_copy_slice,
    };

    return test_group_run(&group);
}

typedef struct TestUtf8ByteCmp {
    const char* label;
    const uint8_t* a;
    const uint8_t* b;
    UTF8ByteCompare expected;
} TestUtf8ByteCmp;

int test_group_utf8_byte_cmp(TestUnit* unit) {
    TestUtf8ByteCmp* data = (TestUtf8ByteCmp*) unit->data;
    UTF8ByteCompare actual = utf8_byte_cmp(data->a, data->b);

    ASSERT_EQ(
        actual,
        data->expected,
        "[TestUtf8ByteCmp] Failed: unit=%zu, a='%s', b='%s', expected=%d, got=%d",
        unit->index,
        data->a,
        data->b,
        data->expected,
        actual
    );

    return 0;
}

int test_suite_utf8_byte_cmp(void) {
    TestUtf8ByteCmp data[] = {
        {"Invalid", NULL, NULL, UTF8_COMPARE_INVALID},  // -2
        {"Empty", (uint8_t*) "", (uint8_t*) "", UTF8_COMPARE_EQUAL},  // 0
        {"Equal", (uint8_t*) "abc", (uint8_t*) "abc", UTF8_COMPARE_EQUAL},  // 0
        {"Less", (uint8_t*) "abc", (uint8_t*) "def", UTF8_COMPARE_LESS},  // -1
        {"Greater", (uint8_t*) "def", (uint8_t*) "abc", UTF8_COMPARE_GREATER},  // 1
        {"Left NULL", NULL, (uint8_t*) "abc", UTF8_COMPARE_INVALID},
        {"Right NULL", (uint8_t*) "abc", NULL, UTF8_COMPARE_INVALID},
        {"Shorter", (uint8_t*) "abc", (uint8_t*) "abcd", UTF8_COMPARE_LESS},
        {"Longer", (uint8_t*) "abcd", (uint8_t*) "abc", UTF8_COMPARE_GREATER},
        {"Multibyte less",
         (uint8_t*) "\xC2\xA2",
         (uint8_t*) "\xE2\x82\xAC",
         UTF8_COMPARE_LESS},  // ¢ vs €
        {"Multibyte greater",
         (uint8_t*) "\xE2\x82\xAC",
         (uint8_t*) "\xC2\xA2",
         UTF8_COMPARE_GREATER},  // € vs ¢
    };
    size_t count = sizeof(data) / sizeof(TestUtf8ByteCmp);

    TestUnit units[count];
    for (size_t i = 0; i < count; i++) {
        units[i].data = &data[i];
    }

    TestGroup group = {
        .name = "utf8_byte_cmp",
        .count = count,
        .units = units,
        .run = test_group_utf8_byte_cmp,
    };

    return test_group_run(&group);
}

int main(void) {
    TestSuite suites[] = {
        {"utf8_byte_count", test_suite_utf8_byte_count},
        {"utf8_byte_diff", test_suite_utf8_byte_diff},
        {"utf8_byte_copy", test_suite_utf8_byte_copy},
        {"utf8_byte_copy_n", test_suite_utf8_byte_copy_n},
        {"utf8_byte_copy_slice", test_suite_utf8_byte_copy_slice},
        {"utf8_byte_cmp", test_suite_utf8_byte_cmp},
    };
    size_t count = sizeof(suites) / sizeof(TestSuite);

    int result = 0;
    for (size_t i = 0; i < count; i++) {
        result |= test_suite_run(&suites[i]);
    }
    return result;
}
