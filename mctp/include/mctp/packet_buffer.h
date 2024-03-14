#ifndef PACKET_BUFFER_H
#define PACKET_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <private_core.h>


typedef struct mctp_packet_buffer_t 
{
    struct mctp_packet_buffer_t* next;
    size_t buffer_len;
    size_t mctp_header_offset;
    uint8_t* buffer;
}
mctp_packet_buffer_t;


mctp_packet_buffer_t* mctp_packet_buffer_init(
    size_t alloc_size,
    size_t mctp_header_offset
);

void mctp_packet_buffer_destroy(
    mctp_packet_buffer_t* packet_buffer
);

mctp_header_t* mctp_packet_buffer_header(
    mctp_packet_buffer_t* packet_buffer
);

uint8_t* mctp_packet_buffer_data(
    mctp_packet_buffer_t* packet_buffer
);


#endif //PACKET_BUFFER_H