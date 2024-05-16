#include <pldm/pldm.h>
#include <pldm/message/base.h>
#include <pldm/message/platform.h>
#include <private_core.h>
#include <stdlib.h>
#include <string.h>


pldm_inst_t* pldm_init()
{
    pldm_inst_t* pldm_inst = malloc(sizeof(pldm_inst_t));

    if(pldm_inst != NULL)
    {
        memset(pldm_inst, 0, sizeof(pldm_inst_t));
    }

    return pldm_inst;
}


void pldm_destroy(
    pldm_inst_t* pldm_inst
)
{
    free(pldm_inst);
}


uint32_t pldm_get_max_message_len(
    pldm_transport_t* transport
)
{
    if(transport == NULL)
    {
        return 0;
    }

    return transport->max_message_len;
}


void pldm_set_max_message_len(
    pldm_transport_t* transport,
    uint32_t max_message_len
)
{
    if(transport == NULL)
    {
        return;
    }

    transport->max_message_len = max_message_len;
}


void pldm_set_base_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_message_rx_t base_message_rx,
    void* base_message_args
)
{
    pldm_inst->base_message_rx = base_message_rx;
    pldm_inst->base_message_args = base_message_args;
}


void pldm_set_platform_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_message_rx_t platform_message_rx,
    void* platform_message_args
)
{
    pldm_inst->platform_message_rx = platform_message_rx;
    pldm_inst->platform_message_args = platform_message_args;
}


void pldm_set_redfish_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_message_rx_t redfish_message_rx,
    void* redfish_message_args
)
{
    pldm_inst->redfish_message_rx = redfish_message_rx;
    pldm_inst->redfish_message_args = redfish_message_args;
}


void pldm_message_rx(
    pldm_terminus_t* terminus,
    uint8_t* message,
    size_t message_len
)
{
    if(terminus == NULL)
    {
        return;
    }

    pldm_inst_t* pldm_inst = terminus->pldm_inst;
    pldm_transport_t* transport = terminus->transport;
    pldm_base_header_t* header = (pldm_base_header_t*)message;

    switch (header->pldm_type)
    {
        case PLDM_TYPE_BASE:
        {
            if(pldm_inst->base_message_rx == NULL)
            {
                break;
            }

            pldm_inst->base_message_rx(
                pldm_inst,
                transport,
                message,
                message_len,
                pldm_inst->base_message_args
            );
        }
        break;

        case PLDM_TYPE_PLATFORM:
        {
            if(pldm_inst->platform_message_rx == NULL)
            {
                break;
            }

            pldm_inst->platform_message_rx(
                pldm_inst,
                transport,
                message,
                message_len,
                pldm_inst->platform_message_args
            );            
        }
        break;

        case PLDM_TYPE_REDFISH:
        {
            if(pldm_inst->redfish_message_rx == NULL)
            {
                break;
            }

            pldm_inst->redfish_message_rx(
                pldm_inst,
                transport,
                message,
                message_len,
                pldm_inst->redfish_message_args
            );            
        }
        break;

        default:
        {
            pldm_resp_error_tx(
                transport,
                header,
                PLDM_CMD_CC_INVALID_PLDM_TYPE
            );
        }
        break;
    }
}


void pldm_message_tx(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
)
{
    transport->message_tx(
        transport,
        message,
        message_len
    );
}


void pldm_resp_error_tx(
    pldm_transport_t* transport,
    pldm_base_header_t* base_header,
    pldm_cmd_cc_t error_code
)
{
    pldm_resp_error_t resp = {
        .header = {
            .version = base_header->version,
            .pldm_type = base_header->pldm_type,
            .command = base_header->command,
            .instance = base_header->instance
        },
        .completion_code = error_code,
    };

    pldm_message_tx(
        transport,
        (uint8_t*)&resp,
        sizeof(pldm_resp_error_t)
    );
}