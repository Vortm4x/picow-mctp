#ifndef MCTP_H
#define MCTP_H

#include <mctp/control.h>
#include <mctp/message.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MCTP_VERSION                0x01
#define MCTP_EID_NULL	            0x00
#define MCTP_EID_BROADCAST          0xFF


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

// typedef void (*mctp_message_rx_t)(
//     mctp_eid_t receiver,
//     mctp_eid_t sender,
//     uint8_t message_tag,
//     bool tag_owner,
//     uint8_t* message,
//     size_t message_len,
//     void* args
// );

typedef void (*mctp_ctrl_message_rx_t)(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    bool integrity_check,
    mctp_ctrl_header_t* ctrl_header,
    uint8_t* message_body,
    size_t body_len,
    void* args
);

typedef void (*mctp_pldm_message_rx_t)(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    bool integrity_check,
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

void mctp_set_bus_eid(
    mctp_binding_t *binding,
    mctp_eid_t eid
);

void mctp_set_ctrl_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_ctrl_message_rx_t ctrl_message_rx,
    void* ctrl_message_args
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