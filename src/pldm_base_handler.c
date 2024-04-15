#include "pldm_base_handler.h"
#include "dump.h"
#include <pldm/util.h>
#include <stdlib.h>
#include <string.h>


void pldm_base_message_rx_callback(
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
        case PLDM_BASE_CMD_GET_TID:
        {
            if(base_header->request)
            {
                handle_req_get_tid(
                    transport,
                    message,
                    message_len
                );
            }
        }
        break;

        case PLDM_BASE_CMD_GET_PLDM_VERSION:
        {
            if(base_header->request)
            {
                handle_req_get_pldm_ver(
                    transport,
                    message,
                    message_len
                );
            }
        }
        break;

        case PLDM_BASE_CMD_GET_PLDM_TYPE:
        {
            if(base_header->request)
            {
                handle_req_get_pldm_type(
                    transport,
                    message,
                    message_len
                );
            }
        }
        break;

        case PLDM_BASE_CMD_GET_PLDM_COMMANDS:
        {
            if(base_header->request)
            {
                handle_req_get_pldm_cmd(
                    transport,
                    message,
                    message_len
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


void handle_req_get_tid(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;

    if(message_len != sizeof(pldm_req_get_tid_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_resp_get_tid_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
        .tid = pldm_get_terminus_id(transport),
    };

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_get_tid_t)
    );
}


void handle_req_get_pldm_ver(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;

    if(message_len != sizeof(pldm_req_get_pldm_ver_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_get_pldm_ver_t* req = (pldm_req_get_pldm_ver_t*)message;
    size_t version_count = 0;

    if(!req->is_first_xfer)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_DATA_TRANSFER_HANDLE 
        );
    }

    switch (req->pldm_type)
    {
        case PLDM_TYPE_BASE:
        {
            version_count = 2;
        }
        break;
    
        case PLDM_TYPE_PLATFORM:
        {
            version_count = 3;
        }
        break;

    }

    if(version_count == 0)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_PLDM_TYPE_IN_REQ_DATA 
        );
    }

    size_t versions_len = sizeof(ver32_t) * version_count;
    size_t resp_data_len = sizeof(pldm_resp_get_pldm_ver_t) + versions_len + sizeof(uint32_t);
    uint8_t resp_data[resp_data_len];
    pldm_resp_get_pldm_ver_t* resp = (pldm_resp_get_pldm_ver_t*)(&resp_data[0]);

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;

    resp->completion_code = PLDM_CMD_CC_SUCCESS;
    resp->xfer_pos.is_start = true;
    resp->xfer_pos.is_end = true;
    resp->next_xfer_handle = req->xfer_handle;

    switch (req->pldm_type)
    {
        case PLDM_TYPE_BASE:
        {
            resp->versions[0].version = 0xF1F1FF00;
            resp->versions[1].version = 0xF1F0FF00;
        }
        break;

        case PLDM_TYPE_PLATFORM:
        {
            resp->versions[0].version = 0xF1F2FF00;
            resp->versions[1].version = 0xF1F1FF00;
            resp->versions[2].version = 0xF1F0FF00;
        }
        break;
    }

    uint32_t crc32 = crc32_calc_block(CRC32_INIT, (uint8_t*)resp->versions, versions_len);
    memcpy(resp->versions + version_count, &crc32, sizeof(uint32_t));

    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
}


void handle_req_get_pldm_type(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;

    if(message_len != sizeof(pldm_req_get_pldm_cmd_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_resp_get_pldm_type_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
        .protocol_support = {
            .base = true,
            .monitor = true,
            .redfish = false,
        }
    };

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_get_pldm_type_t)
    );
}


void handle_req_get_pldm_cmd(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;

    if(message_len != sizeof(pldm_req_get_pldm_cmd_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_get_pldm_cmd_t* req = (pldm_req_get_pldm_cmd_t*)message;

    pldm_resp_get_pldm_cmd_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
    };

    switch (req->pldm_type)
    {
        case PLDM_TYPE_BASE:
        {
            pldm_cmd_support_t cmd_support = {
                .base_get_tid                   = true,
                .base_get_pldm_version          = true,
                .base_get_pldm_type             = true,
                .base_get_pldm_commands         = true,
                .platform_num_sens_get_reading  = true,
                .platform_pdr_repo_info         = true,
                .platform_pdr_repo_get          = true,
                .platform_pdr_repo_sig          = true,
            };

            memcpy(&resp.cmd_support, &cmd_support, sizeof(pldm_cmd_support_t));
        }
        break;
    
        default:
        {
            return pldm_resp_error_tx(
                transport,
                base_header,
                PLDM_CMD_CC_INVALID_PLDM_TYPE_IN_REQ_DATA 
            );
        }
        break;
    }

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_get_pldm_cmd_t)
    );
}