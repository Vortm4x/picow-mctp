#ifndef MCTP_TRANSPORT_H
#define MCTP_TRANSPORT_H

#include <stdint.h>
#include <stddef.h>

#define PLDM_MCTP_MSG_TYPE 1

struct pldm_mctp_transport_t;
typedef struct pldm_mctp_transport_t pldm_mctp_transport_t;

pldm_mctp_transport_t* pldm_mctp_init();

void pldm_mctp_destroy(
    pldm_mctp_transport_t* mctp_transport
);

pldm_transport_t* pldm_mctp_get_core_transport(
    pldm_mctp_transport_t* mctp_transport
);

void pldm_mctp_message_rx(
    pldm_mctp_transport_t* mctp_transport,
    pldm_mctp_eid_t receiver,
    pldm_mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len
);

#endif // MCTP_TRANSPORT_H