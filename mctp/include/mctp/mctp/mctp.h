#ifndef MCTP_H
#define MCTP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MCTP_VERSION                0x01
#define MCTP_EID_NULL	            0x00
#define MCTP_EID_BROADCAST          0xFF


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

typedef struct mctp_generic_header_t
{
    mctp_msg_type_t type : 7;
    bool integrity_check : 1;
}
mctp_generic_header_t;

typedef uint8_t mctp_eid_t;

typedef void (*mctp_message_rx_t)(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t* message,
    size_t message_len,
    void* args
);


struct mctp_inst_t;
typedef struct mctp_inst_t mctp_inst_t;

struct mctp_binding_t;
typedef struct mctp_binding_t mctp_binding_t;


mctp_inst_t* mctp_init();

void mctp_destroy(
    mctp_inst_t* mctp_inst
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

void mctp_set_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_message_rx_t message_rx_callback,
    void* message_rx_args
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