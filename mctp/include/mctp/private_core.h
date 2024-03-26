#ifndef PRIVATE_CORE_H
#define PRIVATE_CORE_H

#include <mctp/mctp.h>

#define MCTP_MESSAGE_TAG_COUNT      (1 << 3)
#define MCTP_MAX_PAYLOAD_SIZE       0x40


struct mctp_bus_t;
typedef struct mctp_bus_t mctp_bus_t;

struct mctp_packet_buffer_t;
typedef struct mctp_packet_buffer_t mctp_packet_buffer_t;

struct mctp_message_ctx_t;
typedef struct mctp_message_ctx_t mctp_message_ctx_t;


typedef struct mctp_binding_t
{
    mctp_binding_type_t binding_type;
    uint8_t version;
    mctp_bus_t* bus;
    size_t binding_header_size;
    size_t binding_trailer_size;
    void* transport_binding;
    void (*packet_tx)(mctp_binding_t* binding, mctp_packet_buffer_t* packet);
}
mctp_binding_t;

typedef struct mctp_bus_t {
	mctp_eid_t eid;
	mctp_binding_t* binding;
    mctp_inst_t* mctp_inst;
	mctp_packet_buffer_t* tx_queue_head;
    mctp_message_ctx_t* incoming_messages[MCTP_MESSAGE_TAG_COUNT];
}
mctp_bus_t;

typedef struct mctp_header_t
{
    uint8_t version : 4;
    uint8_t : 4;
    mctp_eid_t destination;
    mctp_eid_t source;
    uint8_t message_tag : 3;
    bool tag_owner : 1;
    uint8_t packet_sequence : 2;
    bool end_of_message : 1;
    bool start_of_message : 1;    
}
mctp_header_t;


#define MCTP_PACKET_SIZE(transaction_size, header_size, trail_size) \
    (header_size + transaction_size + trail_size)

#define MCTP_TRANSACTION_SIZE(payload_size) \
    (payload_size + sizeof(mctp_header_t))

#define MCTP_PAYLOAD_SIZE(transaction_size) \
    (transaction_size - sizeof(mctp_header_t))


void mctp_transaction_rx(
    mctp_bus_t* bus,
    mctp_packet_buffer_t* transaction
);

#endif // PRIVATE_CORE_H
