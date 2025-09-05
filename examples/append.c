// @file utf8/examples/append.c
#include "byte.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    // copy stack strings to heap
    uint8_t* arr1 = utf8_byte_copy((uint8_t*)"abc");
    uint8_t* arr2 = utf8_byte_copy((uint8_t*)"def");
    uint8_t* arr3 = utf8_byte_copy((uint8_t*)"ghi");

    uint64_t count = 0;
    uint8_t** parts = calloc(1, sizeof(uint8_t*));

    parts = utf8_byte_append(arr1, parts, &count);
    parts = utf8_byte_append(arr2, parts, &count);
    parts = utf8_byte_append(arr3, parts, &count);

    for (uint64_t i = 0; i < count; i++) {
        printf("%s\n", parts[i]);
    }

    utf8_byte_split_free(parts, count);
    return 0;
}
