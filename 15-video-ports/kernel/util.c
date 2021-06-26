#include "util.h"

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
