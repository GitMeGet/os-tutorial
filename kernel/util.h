#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define ASCII_ZERO_OFFSET 48

void memory_copy(uint8_t* dest, uint8_t* src, uint32_t num_bytes);

void memory_set(uint8_t* dest, uint8_t val, uint32_t num_bytes);

void int_to_ascii(int32_t dec, char str[], uint8_t* len);

#endif /* UTIL_H */
