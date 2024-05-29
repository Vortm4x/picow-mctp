#include "redfish_handler.h"
#include "../dump.h"
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

        case PLDM_REDFISH_CMD_RDE_MULTIPART_SEND:
        {
            if(base_header->request)
            {
                handle_req_rde_multipart_send(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        case PLDM_REDFISH_CMD_RDE_MULTIPART_RECEIVE:
        {
            if(base_header->request)
            {
                handle_req_rde_multipart_receive(
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
            PLDM_CMD_CC_ERROR_OPERATION_UNSUPPORTED
        );
    }

    pldm_rde_resource_t* resource = pldm_rde_provider_get_resource(req->resource_id);

    if(resource == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_NO_SUCH_RESOURCE
        );   
    }

    bej_dict_header_t* dict = pldm_rde_schema_get_dict(
        pldm_rde_resource_get_schema(resource)
    );

    pldm_multipart_t* outcomming = pldm_terminus_add_multipart_outcomming(
        transport,
        (uint8_t*)dict,
        dict->dict_size
    );

    pldm_multipart_outcomming_read(outcomming, NULL, 0);

    pldm_resp_get_schema_dict_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
        .dict_format = dict->version_tag,
        .xfer_handle = pldm_multipart_get_xfer_handle(outcomming)
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
            PLDM_CMD_CC_ERROR_OPERATION_UNSUPPORTED
        );
    }

    pldm_rde_resource_t* resource = pldm_rde_provider_get_resource(req->resource_id);

    if(resource == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_NO_SUCH_RESOURCE  
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

        curr_frag = (var_str_t*)(curr_frag->data + curr_frag->len);
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
            PLDM_CMD_CC_ERROR_INVALID_DATA  
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

    uint32_t max_resp_len = pldm_get_max_message_len(transport);
    uint32_t resp_data_len = 0;

    uint8_t resp_data[max_resp_len];
    memset(resp_data, 0, max_resp_len);

    pldm_resp_rde_operation_init_t* resp = (pldm_resp_rde_operation_init_t*)&resp_data[resp_data_len];
    resp_data_len += sizeof(pldm_resp_rde_operation_init_t);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;

    resp->oper_status = RDE_OP_STATUS_INACTIVE;
    resp->completion_percentage = 0;
    resp->completion_time_seconds = PLDM_REDFISH_OPER_COMPLETION_TIME_UNKNOWN;
    resp->result_xfer_handle = PLDM_MULTIPART_NULL_XFER_HANDLE;
    resp->response_payload_len = 0;
    resp->oper_permission_flags.read = true;
    resp->oper_permission_flags.update = true;

    var_str_t* etag_var = (var_str_t*)&resp_data[resp_data_len];
    resp_data_len += sizeof(var_str_t);

    etag_var->str_type = VAR_STR_ASCII;
    etag_var->len = 1;
    resp_data_len += etag_var->len;

    pldm_rde_resource_t* resource = pldm_rde_provider_get_resource(req->resource_id);

    if(resource == NULL)
    {
        resp->completion_code = PLDM_CMD_CC_ERROR_NO_SUCH_RESOURCE;
        return pldm_message_tx(transport, resp_data, resp_data_len);
    }

    pldm_rde_operation_t* operation = pldm_rde_provider_get_operation(req->oper_id);

    if(operation != NULL)
    {
        resp->completion_code = PLDM_CMD_CC_ERROR_OPERATION_EXISTS;
        return pldm_message_tx(transport, resp_data, resp_data_len);
    }

    if(req->oper_flags.excerpt_read
    || req->oper_flags.req_custom_params
    || req->oper_flags.locator_valid)
    {
        resp->completion_code = PLDM_CMD_CC_ERROR_OPERATION_NOT_ALLOWED;
        return pldm_message_tx(transport, resp_data, resp_data_len);
    }

    bej_encoding_t* encoding = pldm_rde_resource_get_encoding(resource);

    switch (req->oper_type)
    {
        case RDE_OP_TYPE_READ:
        {
            pldm_rde_resource_etag_t* etag = (pldm_rde_resource_etag_t*)etag_var->data;
            pldm_rde_resource_get_etag(resource, etag);

            resp_data_len -= etag_var->len;
            etag_var->len = sizeof(pldm_rde_resource_etag_t);
            resp_data_len += etag_var->len;

            size_t encoding_data_len = 0;
            uint8_t* encoding_data = bej_encoding_pack(encoding, &encoding_data_len);

            pldm_multipart_t* outcomming = NULL;
            pldm_rde_oper_status_t status = RDE_OP_STATUS_INACTIVE;

            if(encoding_data_len <= max_resp_len - resp_data_len)
            {
                uint8_t* resp_encoding_data = (uint8_t*)(etag + 1);
                memcpy(resp_encoding_data, encoding_data, encoding_data_len);
                resp_data_len += encoding_data_len;

                status = RDE_OP_STATUS_COMPLETED;
                resp->completion_percentage = 100;
                resp->response_payload_len = (uint32_t)encoding_data_len;
                resp->oper_exec_flags.result_payload = true;
            }
            else
            {
                outcomming = pldm_terminus_add_multipart_outcomming(
                    transport,
                    encoding_data,
                    encoding_data_len
                );

                pldm_multipart_outcomming_read(outcomming, NULL, 0);

                status = RDE_OP_STATUS_HAVE_RESULTS;
                resp->result_xfer_handle = pldm_multipart_get_xfer_handle(outcomming);
            }

            free(encoding_data);
            
            operation = pldm_rde_provider_add_operation(
                resource,
                req->oper_id,
                req->oper_type,
                outcomming
            );

            pldm_rde_operation_set_status(operation, status);
        }
        break;

        case RDE_OP_TYPE_UPDATE:
        {
            pldm_multipart_t* incomming = NULL;
            pldm_rde_oper_status_t status = RDE_OP_STATUS_INACTIVE;

            if(req->oper_flags.req_payload)
            {
                uint8_t* locator_data = (uint8_t*)(req + 1);
                uint8_t* encoding_data = locator_data + req->oper_locator_len;

                pldm_rde_schema_t* schema =  pldm_rde_resource_get_schema(resource);

                bej_encoding_t* update = bej_encoding_unpack(
                    encoding_data, 
                    req->req_payload_len, 
                    (uint8_t*)pldm_rde_schema_get_dict(schema)
                );

                bej_encoding_update(encoding, update);
                bej_encoding_destroy(update);

                status = RDE_OP_STATUS_COMPLETED;
            }
            else
            {
                incomming = pldm_terminus_add_multipart_incomming(
                    transport,
                    req->send_xfer_handle
                );

                status = RDE_OP_STATUS_NEEDS_INPUT;
            }

            operation = pldm_rde_provider_add_operation(
                resource,
                req->oper_id,
                req->oper_type,
                incomming
            );

            pldm_rde_operation_set_status(operation, status);
        }
        break;
    
        default:
        {
            resp->completion_code = PLDM_CMD_CC_ERROR_OPERATION_UNSUPPORTED;
            return pldm_message_tx(transport, resp_data, resp_data_len);
        }
        break;
    }

    resp->oper_exec_flags.task_spawned = true;
    resp->oper_status = pldm_rde_operation_get_status(operation);

    pldm_message_tx(transport, resp_data, resp_data_len);
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
            PLDM_CMD_CC_ERROR_NO_SUCH_RESOURCE  
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

    uint32_t max_resp_len = pldm_get_max_message_len(transport);
    uint32_t resp_data_len = 0;

    uint8_t resp_data[max_resp_len];
    memset(resp_data, 0, max_resp_len);

    pldm_resp_rde_operation_init_t* resp = (pldm_resp_rde_operation_init_t*)&resp_data[resp_data_len];
    resp_data_len += sizeof(pldm_resp_rde_operation_init_t);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;

    resp->oper_status = RDE_OP_STATUS_INACTIVE;
    resp->completion_percentage = 0;
    resp->completion_time_seconds = PLDM_REDFISH_OPER_COMPLETION_TIME_UNKNOWN;
    resp->result_xfer_handle = PLDM_MULTIPART_NULL_XFER_HANDLE;
    resp->response_payload_len = 0;
    resp->oper_permission_flags.read = true;
    resp->oper_permission_flags.update = true;

    var_str_t* etag_var = (var_str_t*)&resp_data[resp_data_len];
    resp_data_len += sizeof(var_str_t);

    etag_var->str_type = VAR_STR_ASCII;
    etag_var->len = 1;
    resp_data_len += etag_var->len;

    pldm_rde_resource_t* resource = pldm_rde_provider_get_resource(req->resource_id);

    if(resource == NULL)
    {
        resp->completion_code = PLDM_CMD_CC_ERROR_NO_SUCH_RESOURCE;
        return pldm_message_tx(transport, resp_data, resp_data_len);
    }

    pldm_rde_operation_t* operation = pldm_rde_provider_get_operation(req->oper_id);

    pldm_rde_oper_status_t status = pldm_rde_operation_get_status(operation);

    if(status == RDE_OP_STATUS_COMPLETED)
    {
        pldm_rde_resource_etag_t* etag = (pldm_rde_resource_etag_t*)etag_var->data;
        pldm_rde_resource_get_etag(resource, etag);

        resp_data_len -= etag_var->len;
        etag_var->len = sizeof(pldm_rde_resource_etag_t);
        resp_data_len += etag_var->len;

        resp->completion_percentage = 100;
    }
    else
    {
        pldm_multipart_t* outcomming = pldm_rde_operation_get_multipart(operation);
        pldm_multipart_get_xfer_handle(outcomming);
    }

    pldm_message_tx(transport, resp_data, resp_data_len);
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

        curr = pldm_rde_operation_get_next(curr);
    }
    
    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
}


void handle_req_rde_multipart_send(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;

    if(message_len < sizeof(pldm_req_rde_multipart_send_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_rde_multipart_send_t* req = (pldm_req_rde_multipart_send_t*)message;

    if(message_len != sizeof(pldm_req_rde_multipart_send_t) + req->data_len)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_resp_rde_multipart_send_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
    };

    pldm_multipart_t* incomming = pldm_terminus_get_multipart(
        transport,
        req->xfer_handle,
        false
    );

    uint32_t data_len = pldm_multipart_get_data_len(incomming);

    if(incomming == NULL || (data_len == 0 && (req->xfer_flag != RDE_XFER_FLAG_START && req->xfer_flag != RDE_XFER_FLAG_START_AND_END)))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA 
        );
    }
    
    pldm_multipart_incomming_write(
        incomming,
        req->data,
        req->data_len
    );

    if(req->xfer_flag == RDE_XFER_FLAG_START_AND_END || req->xfer_flag == RDE_XFER_FLAG_END)
    {
        data_len = pldm_multipart_get_data_len(incomming);

        uint8_t* data = pldm_multipart_get_data(incomming);

        uint32_t crc_calc = 0;
        uint32_t crc_sent = 0;
        uint32_t block_len = 0;

        if(data_len >= sizeof(uint32_t))
        {
            block_len = data_len - sizeof(uint32_t);

            crc_calc = crc32_calc(0, data, block_len);
            memcpy(&crc_sent, &data[block_len], sizeof(uint32_t));
        }

        if(crc_sent == crc_calc && block_len > 0)
        {
            pldm_rde_operation_t* operation = pldm_rde_provider_get_operation(req->oper_id);

            if(operation != NULL)
            {
                pldm_rde_oper_type_t oper_type = pldm_rde_operation_get_type(operation);

                if(oper_type == RDE_OP_TYPE_UPDATE)
                {
                    pldm_rde_resource_t* resource = pldm_rde_operation_get_resource(operation);
                    bej_encoding_t* encoding = pldm_rde_resource_get_encoding(resource);
                    pldm_rde_schema_t* schema =  pldm_rde_resource_get_schema(resource);

                    bej_encoding_t* update = bej_encoding_unpack(
                        data, 
                        block_len, 
                        (uint8_t*)pldm_rde_schema_get_dict(schema)
                    );

                    bej_encoding_update(encoding, update);
                    bej_encoding_destroy(update);

                    pldm_rde_operation_set_status(operation, RDE_OP_STATUS_COMPLETED);
                }
                else
                {
                    pldm_rde_operation_set_status(operation, RDE_OP_STATUS_FAILED);
                }
            }
        }
        else
        {
            pldm_terminus_remove_multipart(
                transport,
                req->xfer_handle,
                false
            );

            return pldm_resp_error_tx(
                transport,
                base_header,
                PLDM_CMD_CC_ERROR_BAD_CHECKSUM
            );
        }

        resp.xfer_oper = RDE_XFER_OPER_COMPLETE;
    }
    else
    {
        resp.xfer_oper = RDE_XFER_OPER_NEXT;
    }

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_rde_multipart_send_t)
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

    pldm_multipart_t* outcomming = pldm_terminus_get_multipart(
        transport,
        req->xfer_handle,
        true
    );

    uint32_t xfer_offset = pldm_multipart_outcomming_get_xfer_offset(outcomming);

    if(outcomming == NULL || (xfer_offset == 0 && req->xfer_oper != RDE_XFER_OPER_FIRST))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_DATA 
        );
    }

    uint32_t xfer_data_len = pldm_multipart_get_data_len(outcomming);
    uint32_t xfer_data_left = xfer_data_len - xfer_offset;
    uint8_t* xfer_data = pldm_multipart_get_data(outcomming);

    uint32_t max_resp_len = pldm_get_max_message_len(transport);
    uint8_t resp_data[max_resp_len];
    memset(resp_data, 0, max_resp_len);

    pldm_resp_rde_multipart_receive_t* resp = (pldm_resp_rde_multipart_receive_t*)&resp_data[0];

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;

    resp->data_len = max_resp_len - sizeof(pldm_resp_rde_multipart_receive_t);

    if(resp->data_len > xfer_data_left)
    {
        resp->data_len = xfer_data_left;
    }

    pldm_xfer_pos_t xfer_pos = pldm_multipart_outcomming_read(
        outcomming,
        resp->data,
        resp->data_len
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

    resp->next_xfer_handle = pldm_multipart_get_xfer_handle(outcomming);

    uint32_t resp_data_len = resp->data_len + sizeof(pldm_resp_rde_multipart_receive_t);

    if(xfer_pos.is_end)
    {
        if((max_resp_len - resp_data_len) < sizeof(uint32_t))
        {
            if(RDE_XFER_FLAG_START_AND_END)
            {
                resp->xfer_flag = RDE_XFER_FLAG_START;
            }
            else
            {
                resp->xfer_flag = RDE_XFER_FLAG_MIDDLE;
            }
        }
        else
        {
            uint32_t crc = crc32_calc(0, xfer_data, xfer_data_len);
            memcpy(resp->data + resp->data_len, &crc, sizeof(uint32_t));

            resp->data_len += sizeof(uint32_t);
            resp_data_len += sizeof(uint32_t);

            pldm_terminus_remove_multipart(
                transport,
                resp->next_xfer_handle,
                true
            );

            resp->next_xfer_handle = PLDM_MULTIPART_NULL_XFER_HANDLE;

            pldm_rde_operation_t* operation = pldm_rde_provider_get_operation(req->oper_id);

            if(operation != NULL)
            {
                pldm_rde_operation_set_status(operation, RDE_OP_STATUS_COMPLETED);
            }
        }
    }
    
    pldm_message_tx(transport, resp_data, resp_data_len);
}
