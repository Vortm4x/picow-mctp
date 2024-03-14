#include <packet_buffer.h>
#include <stdlib.h>
#include <string.h>


mctp_packet_buffer_t* mctp_packet_buffer_init(
    size_t alloc_size,
    size_t mctp_header_offset
)
{
    mctp_packet_buffer_t* packet_buffer = malloc(sizeof(mctp_packet_buffer_t));
    memset(packet_buffer, 0, sizeof(mctp_packet_buffer_t));

    packet_buffer->buffer = calloc(alloc_size, sizeof(uint8_t));
    packet_buffer->mctp_header_offset = mctp_header_offset;

    return packet_buffer;
}

void mctp_packet_buffer_destroy(
    mctp_packet_buffer_t* packet_buffer
)
{
    free(packet_buffer->buffer);
    free(packet_buffer);
}

mctp_header_t* mctp_packet_buffer_header(
    mctp_packet_buffer_t* packet_buffer
)
{
    return (mctp_header_t*)(&packet_buffer->buffer[packet_buffer->mctp_header_offset]);
}

uint8_t* mctp_packet_buffer_data(
    mctp_packet_buffer_t* packet_buffer
)
{
    return (uint8_t*)(mctp_packet_buffer_header(packet_buffer) + 1);
}