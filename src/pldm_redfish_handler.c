#include "pldm_redfish_handler.h"
#include "dump.h"
#include <pldm/pldm.h>
#include <pldm/util.h>
#include <pldm/message/redfish.h>
#include <pldm/pdr/repo.h>
#include <pldm/rde/provider.h>
#include <stdlib.h>
#include <string.h>


void pldm_redfish_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*) message;

    pldm_dump_base(base_header);

    switch (base_header->command)
    {
        case PLDM_REDFISH_CMD_NEGOTIATE_REDFISH_PARAMS:
        {
            if(base_header->request)
            {
                handle_req_negotiate_redfish_params(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        case PLDM_REDFISH_CMD_NEGOTIATE_MEDIUM_PARAMS:
        {
            if(base_header->request)
            {
                handle_req_negotiate_medium_params(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        case PLDM_REDFISH_CMD_GET_SCHEMA_DICT:
        {
            if(base_header->request)
            {
                handle_req_get_schema_dict(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        case PLDM_REDFISH_CMD_GET_SCHEMA_URI:
        {
            if(base_header->request)
            {
                handle_req_get_schema_uri(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        case PLDM_REDFISH_CMD_GET_RESOURCE_ETAG:
        {
            if(base_header->request)
            {
                handle_req_get_resource_etag(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        case PLDM_REDFISH_CMD_RDE_OPERATION_INIT:
        {
            if(base_header->request)
            {
                handle_req_rde_operation_init(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        case PLDM_REDFISH_CMD_SUPPLY_REQ_PARAMS:
        {
            if(base_header->request)
            {

            }
        }
        break;

        case PLDM_REDFISH_CMD_RDE_OPERATION_COMPLETE:
        {
            if(base_header->request)
            {
                handle_req_rde_operation_complete(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        case PLDM_REDFISH_CMD_RDE_OPERATION_STATUS:
        {
            if(base_header->request)
            {
                handle_req_rde_operation_status(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        case PLDM_REDFISH_CMD_RDE_OPERATION_ENUMERATE:
        {
            if(base_header->request)
            {
                handle_req_rde_operation_enumerate(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        default:
        {
            if(base_header->request)
            {
                pldm_resp_error_tx(
                    transport,
                    base_header,
                    PLDM_CMD_CC_ERROR_UNSUPPORTED_CMD
                );
            }
        }
        break;
    }
}


void handle_req_negotiate_redfish_params(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_negotiate_redfish_params_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_negotiate_redfish_params_t* req = (pldm_req_negotiate_redfish_params_t*)message;
    
    if(req->mc_concurrency == 0 || !req->base_feature.read)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA
        );
    }

    uint8_t provider_name[] = "Raspberry Pi Foundation";

    uint32_t config_sig = pldm_rde_provider_get_signature();

    uint32_t resp_data_len = sizeof(pldm_resp_negotiate_redfish_params_t) + sizeof(provider_name);
    uint8_t resp_data[resp_data_len];
    pldm_resp_negotiate_redfish_params_t* resp = (pldm_resp_negotiate_redfish_params_t*)(&resp_data[0]);

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;

    resp->completion_code = PLDM_CMD_CC_SUCCESS;
    resp->device_concurrency = 1;
    resp->base_feature.read = true;
    resp->device_config_sig = config_sig;

    resp->device_provider.str_type = VAR_STR_ASCII;
    resp->device_provider.len = sizeof(provider_name);
    memcpy(resp->device_provider.data, provider_name, resp->device_provider.len);

    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
}


void handle_req_negotiate_medium_params(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_negotiate_medium_params_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_negotiate_medium_params_t* req = (pldm_req_negotiate_medium_params_t*)message;

    pldm_resp_negotiate_medium_params_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
        .device_max_xfer_size = req->mc_max_xfer_size,
    };

    if(req->mc_max_xfer_size > pldm_get_max_message_len(transport))
    {
        resp.device_max_xfer_size = pldm_get_max_message_len(transport);
    }
    else
    {
        pldm_set_max_message_len(transport, req->mc_max_xfer_size);
    }

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_negotiate_medium_params_t)
    );
}


void handle_req_get_schema_dict(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_get_schema_dict_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_get_schema_dict_t* req = (pldm_req_get_schema_dict_t*)message;

    if(req->schema_class != RDE_SCHEMA_CLASS_MAJOR)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA //ERROR_UNSUPPORTED
        );
    }

    pldm_rde_resource_t* resource = pldm_rde_provider_get_resource(req->resource_id);

    if(resource == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA //ERROR_NO_SUCH_RESOURCE
        );   
    }

    bej_dict_header_t* dict = pldm_rde_schema_get_dict(
        pldm_rde_resource_get_schema(resource)
    );

    pldm_multipart_outcomming_t* outcomming = pldm_terminus_add_multipart_outcomming(
        transport,
        (uint8_t*)dict,
        dict->dict_size
    );

    pldm_multipart_outcomming_update_xfer(outcomming, 0);

    pldm_resp_get_schema_dict_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
        .dict_format = dict->version_tag,
        .xfer_handle = pldm_multipart_outcomming_get_xfer_handle(outcomming)
    };

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_get_schema_dict_t)
    );
}


void handle_req_get_schema_uri(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_get_schema_uri_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_get_schema_uri_t* req = (pldm_req_get_schema_uri_t*)message;

    if(req->schema_class != RDE_SCHEMA_CLASS_MAJOR || req->oem_ext_number != 0)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA //ERROR_UNSUPPORTED
        );
    }

    pldm_rde_resource_t* resource = pldm_rde_provider_get_resource(req->resource_id);

    if(resource == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA  // ERROR_NO_SUCH_RESOURCE  
        );   
    }

    pldm_rde_schema_t* schema = pldm_rde_resource_get_schema(resource);
    char** schema_uri_frags = pldm_rde_schema_get_uri_frags(schema);
    uint32_t uri_frags_count = pldm_rde_schema_get_uri_frags_count(schema);

    uint32_t uri_frags_size = 0;

    for(uint32_t i = 0; i < uri_frags_count; ++i)
    {
        uri_frags_size += strlen(schema_uri_frags[i]) + 1;
    }

    uint32_t resp_data_len = sizeof(pldm_resp_get_schema_uri_t) + uri_frags_size + sizeof(var_str_t) * uri_frags_count;
    uint8_t resp_data[resp_data_len];
    pldm_resp_get_schema_uri_t* resp = (pldm_resp_get_schema_uri_t*)&resp_data[0];

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;

    resp->string_fragment_count = uri_frags_count;

    var_str_t* curr_frag = (var_str_t*)(resp + 1);

    for(uint32_t i = 0; i < uri_frags_count; ++i)
    {
        curr_frag->str_type = VAR_STR_UTF_8;
        curr_frag->len = strlen(schema_uri_frags[i]) + 1;
        memcpy(curr_frag->data, schema_uri_frags[i], curr_frag->len);

        curr_frag += 1;
        curr_frag = (var_str_t*)((uint8_t*)curr_frag + curr_frag->len);
    }

    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
}


void handle_req_get_resource_etag(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_get_resource_etag_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_get_resource_etag_t* req = (pldm_req_get_resource_etag_t*)message;

    pldm_rde_resource_t* resource = pldm_rde_provider_get_resource(req->resource_id);

    if(resource == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA  // ERROR_NO_SUCH_RESOURCE  
        );   
    }

    uint32_t resp_data_len = sizeof(pldm_resp_get_resource_etag_t) + sizeof(pldm_rde_resource_etag_t);
    uint8_t resp_data[resp_data_len];
    pldm_resp_get_resource_etag_t* resp = (pldm_resp_get_resource_etag_t*)&resp_data[0];

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;

    resp->etag.len = sizeof(pldm_rde_resource_etag_t);
    resp->etag.str_type = VAR_STR_UTF_8;
    
    pldm_rde_resource_get_etag(
        resource,
        (pldm_rde_resource_etag_t*)resp->etag.data
    );

    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
}


void handle_req_rde_operation_init(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len < sizeof(pldm_req_rde_operation_init_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }


    pldm_req_rde_operation_init_t* req = (pldm_req_rde_operation_init_t*)message;

    if(message_len != sizeof(pldm_req_rde_operation_init_t) + req->oper_locator_len + req->req_payload_len)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    if(req->oper_type != RDE_OP_TYPE_READ)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA  // ERROR_NO_SUCH_RESOURCE  
        );   
    }


    pldm_rde_resource_t* resource = pldm_rde_provider_get_resource(req->resource_id);

    if(resource == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA  // ERROR_UNSUPPORTED  
        );   
    }


    uint32_t resp_data_len = sizeof(pldm_resp_rde_operation_init_t);
    uint8_t resp_data[resp_data_len];
    pldm_resp_rde_operation_init_t* resp = (pldm_resp_rde_operation_init_t*)&resp_data[0];

    memset(resp_data, 0, resp_data_len);

    pldm_multipart_outcomming_t* outcomming = pldm_terminus_add_multipart_outcomming(
        transport,
        (uint8_t*)pldm_rde_resource_get_encoding(resource),
        pldm_rde_resource_get_encoding_size(resource)
    );

    pldm_multipart_outcomming_update_xfer(outcomming, 0);

    pldm_rde_operation_t* operation = pldm_rde_provider_add_operation(
        resource,
        req->oper_type,
        outcomming
    );

    pldm_rde_operation_set_status(
        operation,
        RDE_OP_STATUS_HAVE_RESULTS
    );

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;
    resp->oper_status = pldm_rde_operation_get_status(operation);
    resp->completion_percentage = 0xFE;
    resp->completion_time_seconds = 0xFFFFFFFF;
    resp->oper_exec_flags.task_spawned = true;
    resp->result_xfer_handle = pldm_multipart_outcomming_get_xfer_handle(outcomming);
    resp->oper_permission_flags.read = true;
    resp->response_payload_len = 0;

    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
}


void handle_req_rde_operation_complete(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_rde_operation_complete_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_rde_operation_complete_t* req = (pldm_req_rde_operation_complete_t*)message;

    pldm_rde_resource_t* resource = pldm_rde_provider_get_resource(req->resource_id);

    if(resource == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA  // ERROR_NO_SUCH_RESOURCE  
        );   
    }

    pldm_rde_provider_remove_operation(
        req->oper_id
    );

    pldm_resp_rde_operation_complete_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS
    };

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_rde_operation_complete_t)
    );
}


void handle_req_rde_operation_status(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_rde_operation_status_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_rde_operation_status_t* req = (pldm_req_rde_operation_status_t*)message;


    pldm_rde_resource_t* resource = pldm_rde_provider_get_resource(req->resource_id);

    if(resource == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA  // ERROR_UNSUPPORTED  
        );   
    }

    pldm_rde_operation_t* operation = pldm_rde_provider_get_operation(req->oper_id);

    if(operation == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA  // ERROR_UNSUPPORTED  
        );   
    }

    uint32_t resp_data_len = sizeof(pldm_resp_rde_operation_status_t);

    if(pldm_rde_operation_get_status(operation) == RDE_OP_STATUS_COMPLETED)
    {
        resp_data_len += sizeof(pldm_rde_resource_etag_t);
    }

    uint8_t resp_data[resp_data_len];
    pldm_resp_rde_operation_status_t* resp = (pldm_resp_rde_operation_status_t*)&resp_data[0];

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;
    resp->oper_status = pldm_rde_operation_get_status(operation);
    resp->completion_percentage = 0xFE;
    resp->completion_time_seconds = 0xFFFFFFFF;
    resp->oper_exec_flags.task_spawned = true;
    resp->oper_permission_flags.read = true;

    if(pldm_rde_operation_get_status(operation) != RDE_OP_STATUS_COMPLETED)
    {
        pldm_multipart_outcomming_t* outcomming = pldm_rde_operation_get_outcomming(operation);

        resp->result_xfer_handle = pldm_multipart_outcomming_get_xfer_handle(outcomming);
        resp->response_payload_len = 0;
    }
    else
    {
        resp->result_xfer_handle = PLDM_MULTIPART_NULL_XFER_HANDLE;
        resp->response_payload_len = sizeof(pldm_rde_resource_etag_t);

        pldm_rde_resource_etag_t* etag = (pldm_rde_resource_etag_t*)(resp + 1);

        pldm_rde_resource_get_etag(
            resource,
            etag
        );
    }

    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
}


void handle_req_rde_operation_enumerate(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_rde_operation_enumerate_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    uint16_t oper_count = pldm_rde_provider_get_operation_count();
    uint32_t resp_data_len = sizeof(pldm_resp_rde_operation_enumerate_t) + (oper_count * sizeof(pldm_rde_oper_info_t));
    uint8_t resp_data[resp_data_len];
    pldm_resp_rde_operation_enumerate_t* resp = (pldm_resp_rde_operation_enumerate_t*)&resp_data[0];

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;
    
    resp->oper_count = oper_count;

    pldm_rde_operation_t* curr = pldm_rde_provider_get_first_operation();
    uint16_t curr_idx = 0;

    while (curr != NULL)
    {
        resp->operations[curr_idx].oper_id = pldm_rde_operation_get_id(curr);
        resp->operations[curr_idx].oper_type = pldm_rde_operation_get_type(curr);
        resp->operations[curr_idx].resource_id = pldm_rde_resource_get_id(
            pldm_rde_operation_get_resource(curr)
        );
    }
    
    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
}


void handle_req_rde_multipart_receive(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_rde_multipart_receive_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_rde_multipart_receive_t* req = (pldm_req_rde_multipart_receive_t*)message;

    pldm_multipart_outcomming_t* outcomming = pldm_terminus_get_multipart_outcomming(
        transport,
        req->xfer_handle
    );

    if(outcomming == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA 
        );
    }

    if(pldm_multipart_outcomming_get_xfer_offset(outcomming) == 0 && req->xfer_oper != RDE_XFER_OPER_FIRST)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA 
        );
    }
    

    uint32_t chunk_size = pldm_get_max_message_len(transport) - sizeof(pldm_resp_rde_multipart_receive_t);
    uint32_t xfer_offset = pldm_multipart_outcomming_get_xfer_offset(outcomming);
    uint32_t xfer_data_len = pldm_multipart_outcomming_get_data_len(outcomming);

    {
        uint32_t data_left = xfer_data_len - xfer_offset;

        if(data_left < chunk_size)
        {
            chunk_size = data_left;
        }
    }

    uint8_t* xfer_data = pldm_multipart_outcomming_get_data(outcomming);

    uint32_t resp_data_len = sizeof(pldm_resp_rde_multipart_receive_t) + chunk_size + sizeof(uint32_t);
    uint8_t resp_data[resp_data_len];
    pldm_resp_rde_multipart_receive_t* resp = (pldm_resp_rde_multipart_receive_t*)&resp_data[0];

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;

    pldm_xfer_pos_t xfer_pos = pldm_multipart_outcomming_update_xfer(
        outcomming,
        chunk_size
    );
    
    if(xfer_pos.is_start && xfer_pos.is_end)
    {
        resp->xfer_flag = RDE_XFER_FLAG_START_AND_END;
    }
    else
    if(xfer_pos.is_start)
    {
        resp->xfer_flag = RDE_XFER_FLAG_START;
    }
    else
    if(xfer_pos.is_end)
    {
        resp->xfer_flag = RDE_XFER_FLAG_END;
    }
    else
    if(xfer_pos.is_middle)
    {
        resp->xfer_flag = RDE_XFER_FLAG_MIDDLE;
    }

    resp->next_xfer_handle = pldm_multipart_outcomming_get_xfer_handle(outcomming);
    resp->data_len = chunk_size;

    memcpy(resp->data, &xfer_data[xfer_offset], chunk_size);

    if(xfer_pos.is_end)
    {
        if(resp_data_len > pldm_get_max_message_len(transport))
        {
            if(RDE_XFER_FLAG_START_AND_END)
            {
                resp->xfer_flag = RDE_XFER_FLAG_START;
            }
            else
            {
                resp->xfer_flag = RDE_XFER_FLAG_MIDDLE;
            }

            resp_data_len -= sizeof(uint32_t);
        }
        else
        {
            uint32_t crc = crc32_calc_block(0, xfer_data, xfer_data_len);

            memcpy(resp->data + resp->data_len, &crc, sizeof(uint32_t));

            resp->data_len += sizeof(uint32_t);
            resp->next_xfer_handle = PLDM_MULTIPART_NULL_XFER_HANDLE;

            pldm_terminus_remove_multipart_outcomming(
                transport,
                pldm_multipart_outcomming_get_xfer_handle(outcomming)
            );

            pldm_rde_operation_t* operation = pldm_rde_provider_get_operation(req->oper_id);

            if(operation != NULL)
            {
                pldm_rde_operation_set_status(operation, RDE_OP_STATUS_COMPLETED);
            }
        }
    }
    else
    {
        resp_data_len -= sizeof(uint32_t);
    }

    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
}