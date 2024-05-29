#include "pldm_platform_handler.h"
#include "dump.h"

#include <pldm/pldm.h>
#include <pldm/terminus.h>
#include <pldm/message/platform.h>

#include <pldm/multipart.h>
#include <pldm/util.h>

#include <pldm/pdr/repo.h>
#include <pldm/pdr/record/terminus_locator.h>
#include <pldm/pdr/record/numeric_sensor.h>
#include <pldm/pdr/record/redfish_resource.h>

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
        case PLDM_PLATFORM_CMD_NUM_SENS_GET_READING:
        {
            if(base_header->request)
            {
                handle_req_num_sens_get_reading(
                    transport,
                    message,
                    message_len,
                    args
                );
            }
        }
        break;

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

        case PLDM_PLATFORM_CMD_PRD_REPO_SIG:
        {
            if(base_header->request)
            {
                handle_req_pdr_repo_sig(
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
    pldm_base_header_t* base_header = (pldm_base_header_t*)message;    

    if(message_len != sizeof(pldm_req_num_sens_get_reading_t))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_ERROR_INVALID_LENGTH  
        );
    }

    pldm_req_num_sens_get_reading_t* req = (pldm_req_num_sens_get_reading_t*)message;

    pldm_pdr_numeric_sensor_ref_t* numeric_sensor_ref = pldm_pdr_repo_get_numeric_sensor(req->sensor_id);

    if(numeric_sensor_ref == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_SENSOR_ID  
        );
    }
    
    pldm_pdr_base_numeric_sensor_t* numeric_sensor_base = pldm_pdr_numeric_sensor_ref_get_base(numeric_sensor_ref);

    pldm_pdr_data_size_t data_size = *(pldm_pdr_data_size_t*)(numeric_sensor_base + 1);

    uint32_t resp_data_len = sizeof(pldm_resp_num_sens_get_reading_t);

    switch (data_size)
    {
        case PLDM_PDR_DATA_SIZE_UNSIGNED_INT8:
            resp_data_len += sizeof(uint8_t);
        break;

        case PLDM_PDR_DATA_SIZE_SIGNED_INT8:
            resp_data_len += sizeof(int8_t);
        break;

        case PLDM_PDR_DATA_SIZE_UNSIGNED_INT16:
            resp_data_len += sizeof(uint16_t);
        break;

        case PLDM_PDR_DATA_SIZE_SIGNED_INT16:
            resp_data_len += sizeof(int16_t);
        break;

        case PLDM_PDR_DATA_SIZE_UNSIGNED_INT32:
            resp_data_len += sizeof(uint32_t);
        break;

        case PLDM_PDR_DATA_SIZE_SIGNED_INT32:
            resp_data_len += sizeof(int32_t);
        break;

        default:
        break;
    }
    
    uint8_t resp_data[resp_data_len];
    pldm_resp_num_sens_get_reading_t* resp = (pldm_resp_num_sens_get_reading_t*)&resp_data[0];

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;

    resp->completion_code = PLDM_CMD_CC_SUCCESS;
    resp->data_size = data_size;
    resp->oper_state = PLDM_PDR_OPER_STATE_ENABLED;
    resp->event_msg_enable = PLDM_SENSOR_EVENT_MSG_NO_GEN;
    resp->present_state = PLDM_SENSOR_STATE_NORMAL;
    resp->prev_state = PLDM_SENSOR_STATE_UNKNOWN;
    resp->event_state = PLDM_SENSOR_STATE_UNKNOWN;

    pldm_pdr_numeric_sensor_ref_read_value(
        numeric_sensor_ref,
        (pldm_numeric_sensor_value_t*)resp->reading_data
    );

    pldm_message_tx(
        transport,
        resp_data,
        resp_data_len
    );
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


    pldm_resp_pdr_repo_info_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
        .repo_state = PLDM_PDR_REPO_STATE_AVAILABLE,
        .repo_size = pldm_pdr_repo_get_size(),
        .largest_record_size = pldm_pdr_repo_get_largest_record_size(),
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

    pldm_req_pdr_repo_get_t* req = (pldm_req_pdr_repo_get_t*)message;

    pldm_pdr_entry_t* entry = pldm_pdr_repo_get_entry(req->record_hanlde);

    if(entry == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_RECORD_HANDLE 
        );
    }

    pldm_pdr_header_t* record = pldm_pdr_entry_get_record(entry);

    if(((req->record_change != pldm_pdr_get_record_change(record)) && !req->is_first_xfer)
    || ((req->record_change != PLDM_PDR_NULL_RECORD_CHANGE) && req->is_first_xfer))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_RECORD_CHANGE_NUMBER
        );
    }

    pldm_multipart_t* outcomming = pldm_terminus_get_multipart(
        transport,
        req->xfer_hanlde,
        true
    );
    
    if((outcomming == NULL) && (req->xfer_hanlde != PLDM_MULTIPART_NULL_XFER_HANDLE))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_DATA_TRANSFER_HANDLE 
        );
    }

    
    uint32_t xfer_offset = pldm_multipart_outcomming_get_xfer_offset(outcomming);    

    if((xfer_offset == 0) ^ req->is_first_xfer)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_TRANSFER_OP_FLAG
        );
    }

    if(outcomming == NULL)
    {
        outcomming = pldm_terminus_add_multipart_outcomming(
            transport,
            (uint8_t*)record,
            pldm_pdr_entry_get_size(entry)
        );
    }

    uint32_t record_data_len = pldm_multipart_get_data_len(outcomming);
    uint8_t* record_data = pldm_multipart_get_data(outcomming);
    uint32_t chunk_size = record_data_len - xfer_offset;

    if(chunk_size > req->max_record_len)
    {
        chunk_size = req->max_record_len;
    }


    uint32_t resp_data_len = sizeof(pldm_resp_pdr_repo_get_t) + chunk_size + sizeof(uint8_t);
    uint8_t resp_data[resp_data_len];
    pldm_resp_pdr_repo_get_t* resp = (pldm_resp_pdr_repo_get_t*)&resp_data[0];

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;

    pldm_pdr_header_t* next_record = pldm_pdr_entry_get_record(
        pldm_pdr_entry_get_next(entry)
    );

    resp->next_record_handle = pldm_pdr_get_record_handle(next_record);
    resp->record_len = chunk_size;

    resp->xfer_pos = pldm_multipart_outcomming_read(
        outcomming,
        resp->record_data_part,
        resp->record_len
    );

    resp->next_xfer_handle = pldm_multipart_get_xfer_handle(outcomming);

    // memcpy(resp->record_data_part, &record_data[xfer_offset], resp->record_len);

    if(resp->xfer_pos.is_end)
    {
        uint8_t crc = crc8_calc(0, record_data, record_data_len);
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

/*
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

    pldm_req_pdr_repo_get_t* req = (pldm_req_pdr_repo_get_t*)message;

    pldm_pdr_entry_t* entry = pldm_pdr_repo_get_entry(req->record_hanlde);

    if(entry == NULL)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_RECORD_HANDLE 
        );
    }

    pldm_pdr_header_t* record = pldm_pdr_entry_get_record(entry);

    if(((req->record_change != pldm_pdr_get_record_change(record)) && !req->is_first_xfer)
    || ((req->record_change != PLDM_PDR_NULL_RECORD_CHANGE) && req->is_first_xfer))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_RECORD_CHANGE_NUMBER
        );
    }

    pldm_multipart_outcomming_t* outcomming = pldm_terminus_get_multipart_outcomming(
        transport,
        req->xfer_hanlde
    );
    
    if((outcomming == NULL) && (req->xfer_hanlde != PLDM_MULTIPART_NULL_XFER_HANDLE))
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_DATA_TRANSFER_HANDLE 
        );
    }

    
    uint32_t xfer_offset = pldm_multipart_outcomming_get_xfer_offset(outcomming);    

    if((xfer_offset == 0) ^ req->is_first_xfer)
    {
        return pldm_resp_error_tx(
            transport,
            base_header,
            PLDM_CMD_CC_INVALID_TRANSFER_OP_FLAG
        );
    }

    if(outcomming == NULL)
    {
        outcomming = pldm_terminus_add_multipart_outcomming(
            transport,
            (uint8_t*)record,
            pldm_pdr_entry_get_size(entry)
        );
    }

    uint32_t record_data_len = pldm_multipart_outcomming_get_data_len(outcomming);
    uint8_t* record_data = pldm_multipart_outcomming_get_data(outcomming);
    uint32_t chunk_size = record_data_len - xfer_offset;

    if(chunk_size > req->max_record_len)
    {
        chunk_size = req->max_record_len;
    }


    uint32_t resp_data_len = sizeof(pldm_resp_pdr_repo_get_t) + chunk_size + sizeof(uint8_t);
    uint8_t resp_data[resp_data_len];
    pldm_resp_pdr_repo_get_t* resp = (pldm_resp_pdr_repo_get_t*)&resp_data[0];

    memset(resp_data, 0, resp_data_len);

    resp->header.version = base_header->version;
    resp->header.pldm_type = base_header->pldm_type;
    resp->header.command = base_header->command;
    resp->header.instance = base_header->instance;
    resp->completion_code = PLDM_CMD_CC_SUCCESS;

    pldm_pdr_header_t* next_record = pldm_pdr_entry_get_record(
        pldm_pdr_entry_get_next(entry)
    );

    resp->next_record_handle = pldm_pdr_get_record_handle(next_record);

    resp->xfer_pos = pldm_multipart_outcomming_update_xfer(
        outcomming,
        chunk_size
    );

    resp->next_xfer_handle = pldm_multipart_outcomming_get_xfer_handle(outcomming);
    resp->record_len = chunk_size;

    memcpy(resp->record_data_part, &record_data[xfer_offset], resp->record_len);

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
*/

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


    pldm_resp_pdr_repo_sig_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = PLDM_CMD_CC_SUCCESS,
        .pdr_repo_sig = pldm_pdr_repo_get_signature()
    };

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_pdr_repo_info_t)
    );   
}