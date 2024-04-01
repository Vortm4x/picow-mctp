#include <string.h>
#include "control_handler.h"

void handle_error(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    mctp_ctrl_cc_t error_code
)
{
    mctp_resp_error_t resp = {
        .header = {
            .base.type = ctrl_header->base.type,
            .command = ctrl_header->command,
            .instance = ctrl_header->instance,
        },
        .completion_code = error_code,
    };

    mctp_message_tx(
        mctp_inst,
        mctp_get_bus_eid(core_binding),
        false,
        message_tag,
        (uint8_t*)&resp,
        sizeof(mctp_resp_error_t)
    );
}


void handle_req_set_endpoint_id(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    size_t message_len
)
{
    if(message_len != sizeof(mctp_req_set_endpoint_id_t))
    {
        return handle_error(
            mctp_inst,
            core_binding,
            sender,
            message_tag,
            ctrl_header,
            MCTP_CTRL_CC_ERROR_INVALID_LENGTH
        );
    }

    mctp_req_set_endpoint_id_t* req = (mctp_req_set_endpoint_id_t*)ctrl_header;

    mctp_resp_set_endpoint_id_t resp = {
        .header = {
            .base.type = ctrl_header->base.type,
            .command = ctrl_header->command,
            .instance = ctrl_header->instance,
        },
        .eid_alloc_status = MCTP_EID_ALLOC_STATUS_POOL_NOT_USED,
    };

    switch (req->operation)
    {
        case MCTP_SET_EID_OP_SET_EID:
        case MCTP_SET_EID_OP_FORCE_EID:
        {
            if(req->eid == MCTP_EID_NULL || req->eid == MCTP_EID_BROADCAST)
            {
                return handle_error(
                    mctp_inst,
                    core_binding,
                    sender,
                    message_tag,
                    ctrl_header,
                    MCTP_CTRL_CC_ERROR_INVALID_DATA
                );
            }

            if(mctp_is_bus_eid_assigned(core_binding))
            {
                mctp_set_bus_eid(
                    core_binding, 
                    req->eid, 
                    req->operation == MCTP_SET_EID_OP_SET_EID
                );

                resp.eid_assign_status = MCTP_EID_ASSIGN_STATUS_ACCEPTED;
            }
            else
            {
                resp.eid_assign_status = MCTP_EID_ASSIGN_STATUS_REJECTED;
            }
        }
        break;

        default:
        {        
            return handle_error(
                mctp_inst,
                core_binding,
                sender,
                message_tag,
                ctrl_header,
                MCTP_CTRL_CC_ERROR_INVALID_DATA
            );   
        }
    }

    resp.completion_code = MCTP_CTRL_CC_SUCCESS;
    resp.eid_setting = mctp_get_bus_eid(core_binding);

    mctp_message_tx(
        mctp_inst,
        sender,
        false,
        message_tag,
        (uint8_t*)&resp,
        sizeof(mctp_resp_set_endpoint_id_t)
    );
}


void handle_req_get_endpoint_id(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    size_t message_len
)
{
    if(message_len != sizeof(mctp_req_get_endpoint_id_t))
    {
        return handle_error(
            mctp_inst,
            core_binding,
            sender,
            message_tag,
            ctrl_header,
            MCTP_CTRL_CC_ERROR_INVALID_LENGTH
        );
    }

    mctp_resp_get_endpoint_id_t resp = {
        .header = {
            .base.type = ctrl_header->base.type,
            .command = ctrl_header->command,
            .instance = ctrl_header->instance,
        },
        .completion_code = MCTP_CTRL_CC_SUCCESS,
        .eid = mctp_get_bus_eid(core_binding),
        .endpoint_type = MCTP_ENDPOINT_SIMPLE,
        .eid_type = MCTP_EID_DYNAMIC,
    };

    mctp_message_tx(
        mctp_inst,
        sender,
        false,
        message_tag,
        (uint8_t*)&resp,
        sizeof(mctp_resp_get_endpoint_id_t)
    );
}

void handle_req_get_endpoint_uuid(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    size_t message_len
)
{
    if(message_len != sizeof(mctp_req_get_endpoint_uuid_t))
    {
        return handle_error(
            mctp_inst,
            core_binding,
            sender,
            message_tag,
            ctrl_header,
            MCTP_CTRL_CC_ERROR_INVALID_LENGTH
        );
    }

    mctp_resp_get_endpoint_uuid_t resp = {
        .header = {
            .base.type = ctrl_header->base.type,
            .command = ctrl_header->command,
            .instance = ctrl_header->instance,
        },
        .completion_code = MCTP_CTRL_CC_SUCCESS,
    };

    mctp_get_bus_uuid(core_binding, &resp.uuid);    

    mctp_message_tx(
        mctp_inst,
        sender,
        false,
        message_tag,
        (uint8_t*)&resp,
        sizeof(mctp_resp_get_endpoint_uuid_t)
    );
}

void handle_req_get_mctp_ver(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    size_t message_len
)
{
    if(message_len != sizeof(mctp_req_get_mctp_ver_t))
    {
        return handle_error(
            mctp_inst,
            core_binding,
            sender,
            message_tag,
            ctrl_header,
            MCTP_CTRL_CC_ERROR_INVALID_LENGTH
        );
    }

    mctp_req_get_mctp_ver_t* req = (mctp_req_get_mctp_ver_t*)ctrl_header;

    switch (req->message_type)
    {
        case MCTP_MSG_TYPE_BASE_SPEC:
        case MCTP_MSG_TYPE_CONTROL:
        {
            uint8_t version_count = 4;
            size_t resp_data_len = sizeof(mctp_resp_get_mctp_ver_t) + (version_count - 1) * sizeof(mctp_ver_t);
            uint8_t resp_data[resp_data_len]; 
            mctp_resp_get_mctp_ver_t* resp = (mctp_resp_get_mctp_ver_t*)&resp_data;

            memset(resp_data, 0, resp_data_len);
            
            resp->header.base.type = ctrl_header->base.type;
            resp->header.command = ctrl_header->command;
            resp->header.instance = ctrl_header->instance;

            resp->completion_code = MCTP_CTRL_CC_SUCCESS;
            resp->version_count = version_count;
            
            mctp_ver_t* versions = &resp->version;
            versions[0].version = 0xF1F0FF00;
            versions[1].version = 0xF1F1FF00;
            versions[2].version = 0xF1F2FF00;
            versions[3].version = 0xF1F3F100;

            mctp_message_tx(
                mctp_inst,
                sender,
                false,
                message_tag,
                resp_data,
                resp_data_len
            );
        }
        break;

        default:
        {
            return handle_error(
                mctp_inst,
                core_binding,
                sender,
                message_tag,
                ctrl_header,
                MCTP_CTRL_CC_MSG_TYPE_NOT_SUPPORTED
            );
        }
    }
}

void handle_req_get_msg_type(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t sender,
    uint8_t message_tag,
    mctp_ctrl_header_t* ctrl_header,
    size_t message_len
)
{
    if(message_len != sizeof(mctp_req_get_msg_type_t))
    {
        return handle_error(
            mctp_inst,
            core_binding,
            sender,
            message_tag,
            ctrl_header,
            MCTP_CTRL_CC_ERROR_INVALID_LENGTH
        );
    }

    uint8_t msg_type_count = 4;
    size_t resp_data_len = sizeof(mctp_resp_get_msg_type_t) + (msg_type_count - 1) * sizeof(mctp_msg_type_t);
    uint8_t resp_data[resp_data_len];
    mctp_resp_get_msg_type_t* resp = (mctp_resp_get_msg_type_t*)&resp_data;

    memset(resp_data, 0, resp_data_len);
    
    resp->header.base.type = ctrl_header->base.type;
    resp->header.command = ctrl_header->command;
    resp->header.instance = ctrl_header->instance;

    resp->completion_code = MCTP_CTRL_CC_SUCCESS;
    resp->msg_type_count = msg_type_count;
    
    mctp_msg_type_t* msg_types = &resp->msg_type;
    msg_types[0] = MCTP_MSG_TYPE_CONTROL;
    msg_types[1] = MCTP_MSG_TYPE_PLDM;

    mctp_message_tx(
        mctp_inst,
        sender,
        false,
        message_tag,
        resp_data,
        resp_data_len
    );
}