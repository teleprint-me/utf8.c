// @file utf8/tests/test_utf8_byte.c

#include "byte.h"
#include "test.h"

int test_suite_utf8_byte_count(void) {
    return 0;
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
