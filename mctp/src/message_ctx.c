#include <mctp/mctp.h>
#include <message_ctx.h>
#include <packet_buffer.h>
#include <private_core.h>
#include <stdlib.h>
#include <string.h>


mctp_message_ctx_t* mctp_message_ctx_init(
    mctp_eid_t sender
)
{
    mctp_message_ctx_t* message_ctx = malloc(sizeof(mctp_message_ctx_t));

    if(message_ctx != NULL)
    {
        memset(message_ctx, 0, sizeof(mctp_message_ctx_t));

        message_ctx->sender = sender;
    }

    return message_ctx;
}

void mctp_message_ctx_destroy(
    mctp_message_ctx_t* message_ctx
)
{
    mctp_packet_buffer_t* transaction = NULL;

    while((transaction = message_ctx->rx_queue_head) != NULL)
    {
        message_ctx->rx_queue_head = transaction->next;
        mctp_packet_buffer_destroy(transaction);
    }

    free(message_ctx);
}

void mctp_message_ctx_add_transaction(
    mctp_message_ctx_t* message_ctx,
    mctp_packet_buffer_t* transaction
)
{
    if(message_ctx->rx_queue_head != NULL)
    {
        message_ctx->rx_queue_tail->next = transaction;
    }
    else
    {
        message_ctx->rx_queue_head = transaction;
    }

    message_ctx->rx_queue_tail = transaction;
    message_ctx->packet_count++;
    message_ctx->message_len += MCTP_PAYLOAD_SIZE(transaction->buffer_len);
}