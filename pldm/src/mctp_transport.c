#include <pldm/pldm.h>
#include <pldm/mctp_transport.h>
#include <private_core.h>
#include <stdlib.h>
#include <string.h>

#define PLDM_MCTP_MSG_TYPE 1

typedef uint8_t pldm_mctp_eid_t; 

typedef struct __attribute__ ((__packed__)) pldm_mctp_message_t
{
    uint8_t mctp_msg_type : 7;
    bool intergrity_check : 1;
    uint8_t pldm_msg[];
}
pldm_mctp_message_t;

typedef struct pldm_mctp_msg_ctx_t
{
    pldm_mctp_eid_t sender;
    bool tag_owner : 1;
}
pldm_mctp_msg_ctx_t;


typedef struct pldm_mctp_transport_t
{
    pldm_transport_t transport;
    // pldm_mctp_msg_ctx_t incomming[8];
}

pldm_mctp_transport_t;


pldm_mctp_transport_t* pldm_mctp_init()
{
    pldm_mctp_transport_t* mctp_transport = malloc(sizeof(pldm_mctp_transport_t));

    if(mctp_transport != NULL)
    {
        memset(mctp_transport, 0, sizeof(pldm_mctp_transport_t));
    }

    return mctp_transport;
}

void pldm_mctp_destroy(
    pldm_mctp_transport_t* mctp_transport
)
{
    free(mctp_transport);
}

pldm_transport_t* pldm_mctp_get_core_transport(
    pldm_mctp_transport_t* mctp_transport
)
{
    return &mctp_transport->transport;
}

void pldm_mctp_message_rx(
    pldm_mctp_transport_t* mctp_transport,
    pldm_mctp_eid_t receiver,
    pldm_mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len
)
{
    pldm_mctp_message_t* mctp_message = (pldm_mctp_message_t*)message;

    if(mctp_message->intergrity_check == false
    && mctp_message->mctp_msg_type == PLDM_MCTP_MSG_TYPE)
    {
        pldm_message_rx(
            mctp_transport->transport.endpoint,
            mctp_message->pldm_msg,
            message_len - sizeof(pldm_mctp_message_t)
        );
    }
}