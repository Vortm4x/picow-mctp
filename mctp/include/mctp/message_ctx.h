#ifndef MESSAGE_CTX_H
#define MESSAGE_CTX_H

struct mctp_packet_buffer_t;
typedef struct mctp_packet_buffer_t mctp_packet_buffer_t;


typedef struct mctp_message_ctx_t
{
    mctp_packet_buffer_t* rx_queue_head;
    mctp_packet_buffer_t* rx_queue_tail;
    size_t message_len;
    size_t packet_count;
    mctp_eid_t sender;
    bool tag_owner;
}
mctp_message_ctx_t;


mctp_message_ctx_t* mctp_message_ctx_init(
    mctp_eid_t sender
);

void mctp_message_ctx_destroy(
    mctp_message_ctx_t* message_ctx
);

void mctp_message_ctx_add_transaction(
    mctp_message_ctx_t* message_ctx,
    mctp_packet_buffer_t* transaction
);

#endif // MESSAGE_CTX_H