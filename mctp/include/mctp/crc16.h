#ifndef CRC16_H
#define CRC16_H

#include <stdint.h>
#include <stddef.h>

#define MCTP_CRC16_INIT 0xFFFF


uint16_t crc16_calc_byte(uint16_t crc, uint8_t byte);
uint16_t crc16_calc_block(uint16_t crc, uint8_t block[], size_t block_len);

#endif // CRC16_H