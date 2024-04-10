#include <pldm/pldm.h>
#include <pldm/mctp_transport.h>
#include <private_core.h>
#include <stdlib.h>
#include <string.h>

#define PLDM_MCTP_MSG_TYPE 1


typedef struct __attribute__ ((__packed__)) pldm_mctp_message_t
{
    uint8_t mctp_msg_type : 7;
    bool intergrity_check : 1;
    uint8_t pldm_msg[];
}
pldm_mctp_message_t;

typedef struct pldm_mctp_msg_ctx_t
{
    uint8_t message_tag : 3;
    bool tag_owner : 1;
    uint8_t sender;
    uint8_t receiver;
}
pldm_mctp_msg_ctx_t;


typedef struct pldm_mctp_transport_t
{
    pldm_transport_t transport;
    pldm_mctp_msg_ctx_t* current_context;
    pldm_mctp_message_tx_t mctp_message_tx;
    void* mctp_message_tx_args;
}
pldm_mctp_transport_t;

static void pldm_mctp_message_prepare(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
);

static void pldm_mctp_create_message_ctx(
    pldm_mctp_transport_t* mctp_transport,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t sender,
    uint8_t receiver
);

static void pldm_mctp_drop_message_ctx(
    pldm_mctp_transport_t* mctp_transport
);

pldm_mctp_transport_t* pldm_mctp_init()
{
    pldm_mctp_transport_t* mctp_transport = malloc(sizeof(pldm_mctp_transport_t));

    if(mctp_transport != NULL)
    {
        memset(mctp_transport, 0, sizeof(pldm_mctp_transport_t));

        mctp_transport->transport.header_size = 1;
        mctp_transport->transport.trailer_size = 0;
        mctp_transport->transport.transport_binding = mctp_transport;
        mctp_transport->transport.message_tx = pldm_mctp_message_prepare;
        mctp_transport->current_context = NULL;
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
    uint8_t receiver,
    uint8_t sender,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len
)
{
    pldm_mctp_message_t* mctp_message = (pldm_mctp_message_t*)message;

    if(mctp_message->intergrity_check == false
    && mctp_message->mctp_msg_type == PLDM_MCTP_MSG_TYPE
    && mctp_transport->current_context == NULL)
    {
        pldm_mctp_create_message_ctx(
            mctp_transport,
            message_tag,
            tag_owner,
            sender,
            receiver
        );

        pldm_message_rx(
            mctp_transport->transport.endpoint,
            mctp_message->pldm_msg,
            message_len - sizeof(pldm_mctp_message_t)
        );
    }

    pldm_mctp_drop_message_ctx(mctp_transport);
}

void pldm_mctp_set_message_tx_callback(
    pldm_mctp_transport_t* mctp_transport,
    pldm_mctp_message_tx_t mctp_message_tx,
    void* mctp_message_tx_args
)
{
    mctp_transport->mctp_message_tx = mctp_message_tx;
    mctp_transport->mctp_message_tx_args = mctp_message_tx_args;
}

void pldm_mctp_message_prepare(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
)
{
    size_t mctp_message_len = message_len + transport->header_size + transport->trailer_size;
    pldm_mctp_message_t* mctp_message = (pldm_mctp_message_t*)calloc(mctp_message_len, sizeof(uint8_t));

    memcpy(mctp_message->pldm_msg, message, message_len);
    mctp_message->mctp_msg_type = PLDM_MCTP_MSG_TYPE;

    pldm_mctp_transport_t* mctp_transport = (pldm_mctp_transport_t*)transport->transport_binding;
    pldm_mctp_msg_ctx_t* msg_ctx = mctp_transport->current_context;

    mctp_transport->mctp_message_tx(
        msg_ctx->sender,
        msg_ctx->message_tag,
        !msg_ctx->tag_owner,
        (uint8_t*)mctp_message,
        mctp_message_len,
        mctp_transport->mctp_message_tx_args
    );
}

void pldm_mctp_create_message_ctx(
    pldm_mctp_transport_t* mctp_transport,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t sender,
    uint8_t receiver
)
{
    pldm_mctp_msg_ctx_t* msg_ctx  = malloc(sizeof(pldm_mctp_msg_ctx_t));
    
    if(msg_ctx != NULL)
    {
        memset(msg_ctx, 0, sizeof(pldm_mctp_msg_ctx_t));

        msg_ctx->message_tag = message_tag;
        msg_ctx->tag_owner = tag_owner;
        msg_ctx->sender = sender;
        msg_ctx->receiver = receiver;

        mctp_transport->current_context = msg_ctx;
    }
}

void pldm_mctp_drop_message_ctx(
    pldm_mctp_transport_t* mctp_transport
)
{
    free(mctp_transport->current_context);
    mctp_transport->current_context = NULL;
}