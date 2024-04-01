#ifndef UUID_H
#define UUID_H

#include <stdint.h>

typedef union __attribute__ ((__packed__)) mctp_uuid_t
{
    struct __attribute__ ((__packed__))
    {
        uint64_t node       : 48;
        uint16_t clock_seq  : 14;
        uint8_t variant     : 2;
        uint16_t time_high  : 12;
        uint8_t version     : 4;
        uint16_t time_mid   : 16;
        uint32_t time_low   : 32;
    };
    
    uint8_t bytes[16];
    uint64_t rng_map[2];
}
mctp_uuid_t;

void uuid_gen(mctp_uuid_t* uuid);

#endif //UUID_H