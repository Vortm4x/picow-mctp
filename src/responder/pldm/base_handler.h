#ifndef PLDM_BASE_HANDLER_H
#define PLDM_BASE_HANDLER_H

#include <pldm/pldm.h>
#include <pldm/message/base.h>

void pldm_base_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_get_tid(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
);

void handle_req_get_pldm_ver(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
);

void handle_req_get_pldm_type(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
);

void handle_req_get_pldm_cmd(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
);



#endif // PLDM_BASE_HANDLER_H