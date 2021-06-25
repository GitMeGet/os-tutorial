#include "util.h"

#include <stdint.h>

void memory_copy(uint8_t* restrict src, uint8_t* restrict dest, uint32_t num_bytes) {
    for (int i = 0; i < num_bytes; i++) {
        *(dest++) = *(src++);
    }
}
