#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

#define CRC32_INIT 0xFFFFFFFF

uint32_t crc32_calc_byte(uint32_t crc, uint8_t byte);

uint32_t crc32_calc_block(uint32_t crc, uint8_t block[], size_t block_len);

uint8_t crc8_calc_byte(uint8_t crc, uint8_t data);

uint8_t crc8_calc_block(uint8_t crc, uint8_t block[], size_t block_len);


#endif // UTIL_H