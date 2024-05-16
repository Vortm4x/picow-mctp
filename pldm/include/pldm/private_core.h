#ifndef PLDM_PRIVATE_CORE_H
#define PLDM_PRIVATE_CORE_H

#include <private_terminus.h>

typedef struct pldm_inst_t
{
    pldm_terminus_t* endpoint;
    pldm_message_rx_t base_message_rx;
    void* base_message_args;
    pldm_message_rx_t platform_message_rx;
    void* platform_message_args;
    pldm_message_rx_t redfish_message_rx;
    void* redfish_message_args;
}
pldm_inst_t;

typedef void (*pldm_msg_tx_t)(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
);

typedef struct pldm_transport_t
{
    void* transport_binding;
    pldm_terminus_t* endpoint;
    pldm_msg_tx_t message_tx;
    size_t header_size;
    size_t trailer_size;
    uint32_t max_message_len;
}
pldm_transport_t;

void pldm_message_rx(
    pldm_terminus_t* terminus,
    uint8_t* message,
    size_t message_len
);

#endif // PLDM_PRIVATE_CORE_H