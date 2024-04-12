#ifndef PLDM_PLATFORM_HANDLER_H
#define PLDM_PLATFORM_HANDLER_H

#include <pldm/pldm.h>

void pldm_platform_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_num_sens_get_reading(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_pdr_repo_info(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_pdr_repo_get(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_pdr_repo_sig(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);




#endif // PLDM_PLATFORM_HANDLER_H