// @file utf8/tests/test_utf8_byte.c

#include "byte.h"
#include "test.h"

typedef struct TestUTF8ByteCount {
    const char* label;
    const uint8_t* payload;
    const int64_t expected;
} TestUTF8ByteCount;

int test_group_utf8_byte_count(TestUnit* unit) {
    TestUTF8ByteCount* data = (TestUTF8ByteCount*) unit->data;
    int64_t actual = utf8_byte_count(data->payload);

    // Check if the actual count matches the expected count
    ASSERT(
        actual == data->expected,
        "[TestUTF8ByteCount] Failed: unit=%zu, payload='%s', expected=%ld, got=%ld",
        unit->index,
        data->payload,
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
    const uint8_t* payload;
    const ptrdiff_t start;
    const ptrdiff_t end;
    ptrdiff_t expected;
} TestUTF8ByteDiff;

int test_group_utf8_byte_diff(TestUnit* unit) {
    TestUTF8ByteDiff* data = (TestUTF8ByteDiff*) unit->data;
    const uint8_t* start = data->payload ? data->payload + data->start : NULL;
    const uint8_t* end = data->payload ? data->payload + data->end : NULL;
    ptrdiff_t actual = utf8_byte_diff(start, end);

    ASSERT(
        actual == data->expected,
        "[TestUTF8ByteDiff] Failed: unit=%zu, payload='%s', expected=%ld, got=%ld",
        unit->index,
        data->payload ? (char*) data->payload : "NULL",
        data->expected,
        actual
    );

    return 0;
}

int test_suite_utf8_byte_diff(void) {
    TestUTF8ByteDiff data[] = {
        {"NULL start", NULL, 0, 1, -1},  // nul input
        {"NULL end", (uint8_t*) "abc", 0, 0, 0},  // no diff
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

int main(void) {
    TestSuite suites[] = {
        {"utf8_byte_count", test_suite_utf8_byte_count},
        {"utf8_byte_diff", test_suite_utf8_byte_diff},
    };
    size_t count = sizeof(suites) / sizeof(TestSuite);

    int result = 0;
    for (size_t i = 0; i < count; i++) {
        result |= test_suite_run(&suites[i]);
    }
    return result;
}
