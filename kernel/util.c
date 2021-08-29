#include "kernel/util.h"

#include <stdint.h>

void memory_copy(uint8_t* dest, uint8_t* src, uint32_t num_bytes) {
    for (int i = 0; i < num_bytes; i++) {
        *(dest++) = *(src++);
    }
}

void memory_set(uint8_t* dest, uint8_t val, uint32_t num_bytes) {
    for (int i = 0; i < num_bytes; i++) {
        *(dest++) = val;
    }
}

void int_to_ascii(int32_t dec, char str[], uint8_t* len) {
    uint8_t base = 10;
    uint8_t ascii_char;
    uint8_t idx = 0;

    if (dec == 0) {
        str[idx++] = '0';
    }

    if (dec < 0) {
        str[idx++] = '-';
        dec *= -1;
    }

    while (dec > 0) {
        ascii_char = (dec % base) + ASCII_ZERO_OFFSET;
        str[idx++] = ascii_char;
        dec /= base;
    }

    *len = idx;
}
