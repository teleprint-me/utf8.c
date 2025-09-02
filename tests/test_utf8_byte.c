// @file utf8/tests/test_utf8_byte.c

#include "byte.h"
#include "test.h"

typedef struct TestUTF8ByteCount {
    const char* label;
    const uint8_t* start;
    const int64_t expected;
} TestUTF8ByteCount;

int test_group_utf8_byte_count(TestUnit* unit) {
    TestUTF8ByteCount* data = (TestUTF8ByteCount*) unit->data;
    int64_t actual = utf8_byte_count(data->start);

    // Check if the actual length matches the expected length
    ASSERT(
        actual == data->expected,
        "[TestUTF8ByteCount] Invalid byte length: unit=%zu, data='%s', expected=%ld, got=%ld",
        unit->index,
        data->start,
        data->expected,
        actual
    );

    return 0; // Success
}

int test_suite_utf8_byte_count(void) {
    TestUTF8ByteCount data[] = {
        {"Empty", (const uint8_t*) "", 0},
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

int main(void) {
    TestSuite suites[] = {
        {"utf8_byte_count", test_suite_utf8_byte_count},
    };
    size_t count = sizeof(suites) / sizeof(TestSuite);

    int result = 0;
    for (size_t i = 0; i < count; i++) {
        result |= test_suite_run(&suites[i]);
    }
    return result;
}
