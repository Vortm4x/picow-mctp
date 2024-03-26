#ifndef MESSAGE_CTX_H
#define MESSAGE_CTX_H

struct mctp_packet_buffer_t;
typedef struct mctp_packet_buffer_t mctp_packet_buffer_t;


typedef struct mctp_message_ctx_t
{
    mctp_packet_buffer_t* rx_queue_head;
    mctp_packet_buffer_t* rx_queue_tail;
    size_t message_len;
    mctp_eid_t sender;
    bool tag_owner : 1;
    uint8_t sequence : 2;
    uint8_t version : 4;
}
mctp_message_ctx_t;


mctp_message_ctx_t* mctp_message_ctx_init(
    mctp_eid_t sender,
    bool tag_owner,
    uint8_t version
);

void mctp_message_ctx_destroy(
    mctp_message_ctx_t* message_ctx
);

void mctp_message_ctx_add_transaction(
    mctp_message_ctx_t* message_ctx,
    mctp_packet_buffer_t* transaction
);

#endif // MESSAGE_CTX_H