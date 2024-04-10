#ifndef CONTROL_HANDLER_H
#define CONTROL_HANDLER_H

#include <mctp/mctp.h>
#include <mctp/control.h>


void mctp_ctrl_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_mctp_error(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    mctp_ctrl_cc_t error_code
);

void handle_req_set_endpoint_id(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    size_t message_len
);

void handle_req_get_endpoint_id(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    size_t message_len
);

void handle_req_get_endpoint_uuid(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    size_t message_len
); 

void handle_req_get_mctp_ver(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    size_t message_len
); 

void handle_req_get_msg_type(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    size_t message_len
);

#endif // CONTROL_HANDLER_H