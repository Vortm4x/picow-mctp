#include <pico/platform.h>
#include <mctp/mctp.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>


#define MCTP_DEFAULT_MAX_MSG_SIZE 65536


mctp_inst_t* mctp_init()
{
    mctp_inst_t* mctp_inst = malloc(sizeof(mctp_inst_t));

    if(mctp_inst != NULL)
    {
        memset(mctp_inst, 0, sizeof(mctp_inst_t));
        mctp_inst->max_msg_size = MCTP_DEFAULT_MAX_MSG_SIZE;
    }

    return mctp_inst;
}

void mctp_destroy(mctp_inst_t* mctp_inst)
{
    free(mctp_inst);
}

void mctp_set_max_msg_size(mctp_inst_t* mctp_inst, size_t max_msg_size)
{
    mctp_inst->max_msg_size = max_msg_size;
}

void mctp_register_bus(
    mctp_inst_t* mctp_inst, 
    mctp_binding_t *binding,
	mctp_eid_t eid
)
{
    mctp_bus_t* bus = malloc(sizeof(mctp_bus_t));

    if(bus != NULL)
    {
        memset(bus, 0, sizeof(mctp_bus_t));

        bus->eid = eid;
        bus->binding = binding;
        bus->mctp_inst = mctp_inst;

        mctp_inst->bus = bus;
        binding->bus = bus;
    }
}

void mctp_unregister_bus(
    mctp_inst_t* mctp_inst, 
    mctp_binding_t *binding
)
{
	free(mctp_inst->bus);
    mctp_inst->bus = NULL;
    binding->bus = NULL;
}

mctp_header_t* mctp_packet_buffer_header(
    mctp_packet_buffer_t* packet_buffer
)
{
    return (mctp_header_t*)(&packet_buffer->buffer[packet_buffer->mctp_header_offset]);
}

uint8_t* mctp_packet_buffer_data(
    mctp_packet_buffer_t* packet_buffer
)
{
    return (uint8_t*)(mctp_packet_buffer_header(packet_buffer) + 1);
}

mctp_packet_buffer_t* mctp_packet_buffer_init(
    size_t alloc_size,
    size_t mctp_header_offset
)
{
    mctp_packet_buffer_t* packet_buffer = malloc(sizeof(mctp_packet_buffer_t));
    memset(packet_buffer, 0, sizeof(mctp_packet_buffer_t));

    packet_buffer->buffer = calloc(alloc_size, sizeof(uint8_t));
    packet_buffer->mctp_header_offset = mctp_header_offset;

    return packet_buffer;
}

void mctp_packet_buffer_destroy(
    mctp_packet_buffer_t* packet_buffer
)
{
    free(packet_buffer->buffer);
    free(packet_buffer);
}

static void mctp_packet_tx(
    mctp_bus_t* bus, 
    mctp_packet_buffer_t* packet
)
{
    bus->binding->packet_tx(
        bus->binding,
        packet
    );
}

static void mctp_packet_queue_tx(
    mctp_bus_t* bus
)
{
    mctp_packet_buffer_t* packet = NULL;

	while ((packet = bus->tx_queue_head) != NULL) 
    {
		mctp_packet_tx(bus, packet);

        bus->tx_queue_head = packet->next;
        free(packet->buffer);
        free(packet);
	}
}

static void mctp_message_disassemble(
    mctp_bus_t* bus,
    mctp_eid_t destination, 
    bool tag_owner,
	uint8_t message_tag, 
    uint8_t* message, 
    size_t message_len
)
{
    mctp_packet_buffer_t* packet = NULL;
    mctp_packet_buffer_t* tx_queue_tail = NULL;
    size_t payload_offset = 0;

    mctp_header_t mctp_header = {
        .destination = destination,
        .source = bus->eid,
        .tag_owner = tag_owner,
        .message_tag = message_tag,
        .version = bus->binding->version,
    };

    while (payload_offset < message_len)
    {
        size_t bytes_left = message_len - payload_offset;
        size_t payload_size = MIN(bytes_left, MCTP_MAX_PAYLOAD_SIZE);
        size_t packet_size = MCTP_PACKET_SIZE(
            MCTP_TRANSACTION_SIZE(payload_size),
            bus->binding->binding_header_size,
            bus->binding->binding_trailer_size
        );

        packet = mctp_packet_buffer_init(packet_size, bus->binding->binding_header_size);
        
        mctp_header_t* packet_header = mctp_packet_buffer_header(packet);
        uint8_t* packet_data = mctp_packet_buffer_data(packet);

        memcpy(packet_header, &mctp_header, sizeof(mctp_header_t));
        memcpy(packet_data, &message[payload_offset], sizeof(mctp_header_t));

        if (tx_queue_tail != NULL)
        {
            tx_queue_tail->next = packet;
        }	
		else
        {
			bus->tx_queue_head = packet;
        }

		tx_queue_tail = packet;
        
        mctp_header.packet_sequence++;
        payload_offset += payload_size;
    }

    mctp_packet_buffer_header(bus->tx_queue_head)->start_of_message = true;
    mctp_packet_buffer_header(tx_queue_tail)->end_of_message = true;
}

void mctp_message_tx(
    mctp_inst_t* mctp_inst, 
    mctp_eid_t destination,
    bool tag_owner,
	uint8_t message_tag, 
    uint8_t* message, 
    size_t message_len
)
{
    mctp_message_disassemble(
        mctp_inst->bus, 
        destination, 
        tag_owner, 
        message_tag, 
        message, 
        message_len
    );

    mctp_packet_queue_tx(mctp_inst->bus);
}


static uint8_t* mctp_message_assemble(
    mctp_message_ctx_t* message_ctx
)
{
    uint8_t* message = calloc(message_ctx->message_len, sizeof(uint8_t));
    mctp_packet_buffer_t* transaction = message_ctx->rx_queue_head;
    size_t payload_offset = 0;

    while (transaction != NULL)
    {
        size_t payload_size = MCTP_PAYLOAD_SIZE(transaction->buffer_len);
        memcpy(&message[payload_offset], mctp_packet_buffer_data(transaction), payload_size);
        
        payload_offset += payload_size;
        transaction = transaction->next;
    }

    return message;
}

void mctp_messsage_rx(
    mctp_inst_t* mctp_inst,
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t* message,
    size_t message_len
)
{
    mctp_inst->message_rx_callback(
        receiver,
        sender,
        message,
        message_len,
        mctp_inst->message_rx_args
    );
}

void mctp_transaction_rx(
    mctp_bus_t* bus,
    mctp_packet_buffer_t* transaction
)
{
    mctp_header_t* mctp_header = mctp_packet_buffer_header(transaction);
    uint8_t* payload = mctp_packet_buffer_data(transaction);

    if(mctp_header->destination != bus->eid)
    {
        mctp_packet_buffer_destroy(transaction);
        return;
    }

    mctp_message_ctx_t* message_ctx = bus->incoming_messages[mctp_header->message_tag];

    if(mctp_header->start_of_message 
    && mctp_header->end_of_message)
    {
        if(mctp_header->packet_sequence == 0)
        {
            mctp_messsage_rx(
                bus->mctp_inst,
                bus->eid,
                mctp_header->source,
                mctp_packet_buffer_data(transaction),
                MCTP_PAYLOAD_SIZE(transaction->buffer_len)
            );
        }
        else
        {
            printf("packet drop\n");
        }

        mctp_packet_buffer_destroy(transaction);
    }
    else
    if(mctp_header->start_of_message)
    {
        if(message_ctx != NULL)
        {
            mctp_message_ctx_destroy(message_ctx);
        }

        message_ctx = mctp_message_ctx_init(mctp_header->source);
        mctp_message_ctx_add_transaction(message_ctx, transaction);
    }
    else
    if(mctp_header->end_of_message)
    {
        if(message_ctx == NULL)
        {
            mctp_packet_buffer_destroy(transaction);
            return;
        }

        if(message_ctx->packet_count % 4 == mctp_header->packet_sequence
        && transaction->buffer_len <= message_ctx->rx_queue_head->buffer_len)
        {
            mctp_message_ctx_add_transaction(message_ctx, transaction);

            uint8_t* message = mctp_message_assemble(message_ctx);
            mctp_message_ctx_destroy(message_ctx);

            mctp_messsage_rx(
                bus->mctp_inst,
                bus->eid,
                mctp_header->source,
                message,
                message_ctx->message_len
            );

            free(message);
        }
        else
        {
            mctp_message_ctx_add_transaction(message_ctx, transaction);
            mctp_message_ctx_destroy(message_ctx);
        }

        message_ctx = NULL;
    }
    else
    {
        if(message_ctx == NULL)
        {
            mctp_packet_buffer_destroy(transaction);
            return;
        }

        if(message_ctx->packet_count % 4 == mctp_header->packet_sequence
        && transaction->buffer_len == message_ctx->rx_queue_head->buffer_len)
        {
            mctp_message_ctx_add_transaction(message_ctx, transaction);
        }
        else
        {
            mctp_message_ctx_add_transaction(message_ctx, transaction);
            mctp_message_ctx_destroy(message_ctx);
            message_ctx = NULL;
        }
    }

    bus->incoming_messages[mctp_header->message_tag] = message_ctx;
}

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

void mctp_set_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_message_rx_t message_rx_callback,
    void* message_rx_args
)
{
    mctp_inst->message_rx_callback = message_rx_callback;
    mctp_inst->message_rx_args = message_rx_args;
}