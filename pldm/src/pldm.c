#include <pldm/pldm.h>
#include <pldm/base.h>
#include <pldm/monitor.h>
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

void pldm_register_terminus(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport
)
{
    pldm_terminus_t* terminus = malloc(sizeof(pldm_terminus_t));

    if(terminus != NULL)
    {
        terminus->tid = PLDM_TID_UNASSIGNED;
        terminus->transport = transport;
        terminus->pldm_inst = pldm_inst;

        pldm_inst->endpoint = terminus;

        transport->endpoint = terminus;
    }
}

void pldm_unregister_terminus(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport   
)
{
    free(pldm_inst->endpoint);
    pldm_inst->endpoint = NULL;
    transport->endpoint = NULL;
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

void pldm_set_monitor_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_message_rx_t monitor_message_rx,
    void* monitor_message_args
)
{
    pldm_inst->monitor_message_rx = monitor_message_rx;
    pldm_inst->monitor_message_args = monitor_message_args;
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
            pldm_inst->base_message_rx(
                pldm_inst,
                transport,
                message,
                message_len,
                pldm_inst->base_message_args
            );
        }
        break;

        case PLDM_TYPE_MONITOR:
        {}
        break;

        case PLDM_TYPE_REDFISH:
        {}
        break;

        default:
        {}
        break;
    }
}