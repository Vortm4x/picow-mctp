#ifndef PLDM_UTIL_H
#define PLDM_UTIL_H

#include <stdint.h>
#include <stddef.h>

uint32_t crc32_calc(uint32_t crc, uint8_t block[], size_t block_len);

uint8_t crc8_calc(uint8_t crc, uint8_t block[], size_t block_len);

#endif // PLDM_UTIL_H