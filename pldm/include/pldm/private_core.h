#ifndef PRIVATE_CORE_H
#define PRIVATE_CORE_H

struct pldm_terminus_t;
typedef struct pldm_terminus_t pldm_terminus_t;

typedef struct pldm_inst_t
{
    pldm_terminus_t* endpoint;
    pldm_message_rx_t base_message_rx;
    void* base_message_args;
    pldm_message_rx_t monitor_message_rx;
    void* monitor_message_args;
    pldm_message_rx_t redfish_message_rx;
    void* redfish_message_args;
}
pldm_inst_t;

typedef struct pldm_transport_t
{
    void* transport_binding;
    pldm_terminus_t* endpoint;
}
pldm_transport_t;

typedef struct pldm_terminus_t
{
    pldm_transport_t* transport;
    pldm_inst_t* pldm_inst;
    pldm_tid_t* tid;
}
pldm_terminus_t;


void pldm_message_rx(
    pldm_terminus_t* terminus,
    uint8_t* message,
    size_t message_len
);

#endif