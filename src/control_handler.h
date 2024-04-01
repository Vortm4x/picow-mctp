#ifndef CONTROL_HANDLER
#define CONTROL_HANDLER

#include <mctp/mctp.h>
#include <mctp/control.h>

void handle_error(
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

#endif