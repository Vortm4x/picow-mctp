#ifndef PLDM_REDFISH_HANDLER_H
#define PLDM_REDFISH_HANDLER_H

#include <pldm/pldm.h>

void pldm_redfish_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_negotiate_redfish_params(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_negotiate_medium_params(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_get_schema_dict(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_get_schema_uri(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_get_resource_etag(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_rde_operation_init(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_rde_operation_complete(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_rde_operation_status(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_rde_operation_enumerate(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

void handle_req_rde_multipart_receive(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);


#endif // PLDM_REDFISH_HANDLER_H