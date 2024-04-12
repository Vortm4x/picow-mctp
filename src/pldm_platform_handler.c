#include "pldm_platform_handler.h"
#include "dump.h"
#include <pldm/pldm.h>
#include <pldm/util.h>
#include <pldm/platform.h>
#include <pldm/pdr/repo.h>
#include <pldm/pdr/term_loc.h>
#include <pldm/pdr/num_sens.h>
#include <pldm/pdr/redfish_resource.h>
#include <stdlib.h>
#include <string.h>

void pldm_platform_message_rx_callback(
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

        case PLDM_PLATFORM_CMD_PRD_REPO_INFO:
        {
            if(base_header->request)
            {
                handle_req_pdr_repo_info(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

        case PLDM_PLATFORM_CMD_PRD_REPO_GET:
        {
            if(base_header->request)
            {
                handle_req_pdr_repo_get(
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


void handle_req_num_sens_get_reading(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{

}

void handle_req_pdr_repo_info(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_pdr_repo_info_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_pdr_repo_t* pdr_repo = (pldm_pdr_repo_t*)args;

    pldm_resp_pdr_repo_info_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
        .repo_state = PLDM_PDR_REPO_STATE_AVAILABLE,
        //.upd_time
        //.oem_upd_time
        .repo_size = pldm_pdr_repo_total_size(pdr_repo),
        .largest_record_size = pldm_pdr_repo_largest_record_size(pdr_repo),
        .xfer_handle_timeout = 0x01
    };

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_pdr_repo_info_t)
    );
}

void handle_req_pdr_repo_get(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_pdr_repo_get_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_pdr_repo_t* pdr_repo = (pldm_pdr_repo_t*)args;

    pldm_req_pdr_repo_get_t* req = (pldm_req_pdr_repo_get_t*)message;

    pldm_pdr_repo_entry_t* entry = pldm_pdr_repo_get_entry(
        pdr_repo,
        req->record_hanlde
    );

    if(entry == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_RECORD_HANDLE 
        );
    }

    if(pldm_pdr_repo_entry_get_xfer_handle(entry) != req->xfer_hanlde)
    {
        pldm_pdr_repo_entry_xfer_reset(entry);

        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_DATA_TRANSFER_HANDLE 
        );
    }
    
    if((pldm_pdr_repo_entry_get_xfer_offset(entry) == 0) ^ req->is_first_xfer)
    {
        pldm_pdr_repo_entry_xfer_reset(entry);

        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_TRANSFER_OP_FLAG
        );
    }

    pldm_pdr_header_t* record = pldm_pdr_repo_entry_get_record(entry);

    if(((req->record_change != record->record_change) && !req->is_first_xfer)
    || ((req->record_change != PLDM_PDR_NULL_RECORD_CHANGE) && req->is_first_xfer))
    {
        pldm_pdr_repo_entry_xfer_reset(entry);

        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_RECORD_CHANGE_NUMBER
        );
    }


    uint32_t record_data_len = pldm_pdr_record_len(record);
    uint8_t* record_data = (uint8_t*)record;

    uint32_t record_data_part_offset = pldm_pdr_repo_entry_get_xfer_offset(entry);
    uint32_t record_data_part_len = record_data_len - record_data_part_offset;

    if(record_data_part_len > req->max_record_len)
    {
        record_data_part_len = req->max_record_len;
    }

    uint32_t resp_data_len = sizeof(pldm_resp_pdr_repo_get_t) + record_data_part_len + sizeof(uint8_t);
    uint8_t resp_data[resp_data_len];
    pldm_resp_pdr_repo_get_t* resp = (pldm_resp_pdr_repo_get_t*)&resp_data[0];

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;

    resp->completion_code = PLDM_CMD_CC_SUCCESS;

    pldm_pdr_header_t* next_record = pldm_pdr_repo_entry_get_record(
        pldm_pdr_repo_entry_get_next_entry(entry)
    );

    if(next_record != NULL)
    {
        resp->next_record_handle = next_record->record_handle;
    }
    else
    {
        resp->next_record_handle = PLDM_PDR_NULL_HANDLE;
    }

    resp->xfer_pos = pldm_pdr_repo_entry_xfer_upd(entry, record_data_part_len);
    resp->next_xfer_handle = pldm_pdr_repo_entry_get_xfer_handle(entry);
    resp->record_len = record_data_part_len;

    memcpy(resp->record_data_part, &record_data[record_data_part_offset], resp->record_len);

    if(resp->xfer_pos.is_end)
    {
        uint8_t crc = crc8_calc_block(0, record_data, record_data_len);
        memcpy(resp->record_data_part + resp->record_len, &crc, sizeof(uint8_t));
    }
    else
    {
        resp_data_len -= sizeof(uint8_t);
    }

    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
}

void handle_req_pdr_repo_sig(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_pdr_repo_sig_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_pdr_repo_t* pdr_repo = (pldm_pdr_repo_t*)args;

    pldm_resp_pdr_repo_sig_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
        .pdr_repo_sig = pldm_pdr_repo_signature(pdr_repo)
    };

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_pdr_repo_info_t)
    );   
}