#ifndef MCTP_H
#define MCTP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MCTP_EID_NULL	   0x00
#define MCTP_EID_BROADCAST 0xFF
#define MCTP_VERSION       0x01


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


typedef struct mctp_packet_t 
{
    struct mctp_packet_t* next;
    size_t buffer_len; 
    uint8_t* buffer;
}
mctp_packet_t;

typedef struct mctp_pkt_buffer_t {
	size_t start;
    size_t end; 
    size_t size;
	size_t mctp_header_offset;
	struct mctp_pkt_buffer_t *next;
	
}
mctp_pkt_buffer_t;



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

struct mctp_binding_t;
typedef struct mctp_binding_t mctp_binding_t;


typedef struct mctp_bus_t {
	mctp_eid_t eid;
	mctp_binding_t* binding;
	mctp_packet_t* tx_queue_head;
}
mctp_bus_t;

typedef struct mctp_inst_t
{
    mctp_bus_t* bus;
	size_t max_msg_size;
}
mctp_inst_t;

typedef struct mctp_binding_t
{
    mctp_binding_type_t binding_type;
    uint8_t version;
    mctp_inst_t* mctp_inst;
    size_t max_transaction_size;
    size_t binding_header_size;
    size_t binding_trailer_size;
    void* transport_binding;
    void (*packet_tx)(struct mctp_binding_t* binding, mctp_packet_t* packet);
}
mctp_binding_t;



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
    void* transport_binding,
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


#endif //MCTP_H
