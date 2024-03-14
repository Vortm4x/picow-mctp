#ifndef MCTP_H
#define MCTP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MCTP_EID_NULL	            0x00
#define MCTP_EID_BROADCAST          0xFF
#define MCTP_VERSION                0x01
#define MCTP_MAX_PAYLOAD_SIZE       0x40
#define MCTP_MESSAGE_TAG_COUNT      (1 << 3)

typedef uint8_t mctp_eid_t;

typedef enum mctp_msg_type_t
{
    MCTP_MSG_TYPE_CONTROL       = 0x00,
    MCTP_MSG_TYPE_PLDM          = 0x01,
    MCTP_MSG_TYPE_NCSI          = 0x02,
    MCTP_MSG_TYPE_ETHERNET      = 0x03,
    MCTP_MSG_TYPE_NVM_EXPRESS   = 0x04,
    MCTP_MSG_TYPE_VENDOR_PCI    = 0x7E,
    MCTP_MSG_TYPE_VENDOR_IANA   = 0x7F,
}
mctp_msg_type_t;


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



typedef struct mctp_generic_header_t
{
    mctp_msg_type_t type : 7;
    bool integrity_check : 1;
}
mctp_generic_header_t;


typedef struct mctp_packet_buffer_t 
{
    struct mctp_packet_buffer_t* next;
    size_t buffer_len;
    size_t mctp_header_offset;
    uint8_t* buffer;
}
mctp_packet_buffer_t;


typedef enum mctp_binding_type_t
{
    MCTP_BINDING_TYPE_RESERVED  = 0x00,
    MCTP_BINDING_TYPE_SMBUS     = 0x01,
    MCTP_BINDING_TYPE_PCIE_VDM  = 0x02,
    MCTP_BINDING_TYPE_USB       = 0x03,
    MCTP_BINDING_TYPE_KCS       = 0x04,
    MCTP_BINDING_TYPE_SERIAL    = 0x05,
}
mctp_binding_type_t;

struct mctp_bus_t;
typedef struct mctp_bus_t mctp_bus_t;
struct mctp_inst_t;
typedef struct mctp_inst_t mctp_inst_t;


typedef struct mctp_binding_t
{
    mctp_binding_type_t binding_type;
    uint8_t version;
    mctp_bus_t* bus;
    size_t binding_header_size;
    size_t binding_trailer_size;
    void* transport_binding;
    void (*packet_tx)(struct mctp_binding_t* binding, mctp_packet_buffer_t* packet);
}
mctp_binding_t;


typedef struct mctp_message_ctx_t
{
    mctp_packet_buffer_t* rx_queue_head;
    mctp_packet_buffer_t* rx_queue_tail;
    size_t message_len;
    size_t packet_count;
    mctp_eid_t sender;
    bool tag_owner;
}
mctp_message_ctx_t;


typedef void (*mctp_message_rx_t)(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t* message,
    size_t message_len,
    void* args
);

typedef struct mctp_bus_t {
	mctp_eid_t eid;
	mctp_binding_t* binding;
    mctp_inst_t* mctp_inst;
	mctp_packet_buffer_t* tx_queue_head;
    mctp_message_ctx_t* incoming_messages[MCTP_MESSAGE_TAG_COUNT];
}
mctp_bus_t;

typedef struct mctp_inst_t
{
    mctp_bus_t* bus;
	size_t max_msg_size;
    mctp_message_rx_t message_rx_callback;
    void* message_rx_args;
}
mctp_inst_t;



mctp_inst_t* mctp_init();

void mctp_destroy(
    mctp_inst_t* mctp_inst
);

void mctp_set_max_msg_size(
    mctp_inst_t* mctp_inst, 
    size_t max_msg_size
);

void mctp_register_bus(
    mctp_inst_t* mctp_inst, 
    mctp_binding_t *binding,
	mctp_eid_t eid
);

void mctp_unregister_bus(
    mctp_inst_t* mctp_inst, 
    mctp_binding_t *binding
);


void mctp_message_tx(
    mctp_inst_t* mctp_inst, 
    mctp_eid_t destination, 
    bool tag_owner,
	uint8_t message_tag, 
    uint8_t* message, 
    size_t message_len
);

void mctp_transaction_rx(
    mctp_bus_t* bus,
    mctp_packet_buffer_t* transaction
);

mctp_packet_buffer_t* mctp_packet_buffer_init(
    size_t alloc_size,
    size_t mctp_header_offset
);

void mctp_packet_buffer_destroy(
    mctp_packet_buffer_t* packet_buffer
);

mctp_message_ctx_t* mctp_message_ctx_init(
    mctp_eid_t sender
);

void mctp_message_ctx_destroy(
    mctp_message_ctx_t* message_ctx
);

void mctp_message_ctx_add_transaction(
    mctp_message_ctx_t* message_ctx,
    mctp_packet_buffer_t* transaction
);

void mctp_set_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_message_rx_t message_rx_callback,
    void* message_rx_args
);

void mctp_messsage_rx(
    mctp_inst_t* mctp_inst,
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t* message,
    size_t message_len
);

mctp_header_t* mctp_packet_buffer_header(
    mctp_packet_buffer_t* packet_buffer
);

uint8_t* mctp_packet_buffer_data(
    mctp_packet_buffer_t* packet_buffer
);

#endif //MCTP_H
