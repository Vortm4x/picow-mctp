#include <pico/platform.h>
#include <mctp/mctp.h>
#include <packet_buffer.h>
#include <message_ctx.h>
#include <private_core.h>
#include <stdlib.h>
#include <string.h>
#include <uuid.h>


typedef struct mctp_inst_t
{
    mctp_bus_t* bus;
	size_t max_msg_size;
    mctp_message_rx_t ctrl_message_rx_callback;
    void* ctrl_message_rx_args;
    mctp_message_rx_t pldm_message_rx_callback;
    void* pldm_message_rx_args;
}
mctp_inst_t;

#define MCTP_DEFAULT_MAX_MSG_SIZE 65536


static void mctp_message_disassemble(
    mctp_bus_t* bus,
    mctp_eid_t destination, 
    bool tag_owner,
	uint8_t message_tag, 
    uint8_t* message, 
    size_t message_len
);

static void mctp_packet_queue_tx(
    mctp_bus_t* bus
);

static void mctp_packet_tx(
    mctp_bus_t* bus, 
    mctp_packet_buffer_t* packet
);

static uint8_t* mctp_message_assemble(
    mctp_message_ctx_t* message_ctx
);

static void mctp_messsage_rx(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len
);


mctp_inst_t* mctp_init()
{
    mctp_inst_t* mctp_inst = malloc(sizeof(mctp_inst_t));

    if(mctp_inst != NULL)
    {
        memset(mctp_inst, 0, sizeof(mctp_inst_t));
    }

    return mctp_inst;
}

void mctp_destroy(
    mctp_inst_t* mctp_inst
)
{
    free(mctp_inst);
}

void mctp_register_bus(
    mctp_inst_t* mctp_inst, 
    mctp_binding_t *binding
)
{
    mctp_bus_t* bus = malloc(sizeof(mctp_bus_t));

    if(bus != NULL)
    {
        memset(bus, 0, sizeof(mctp_bus_t));

        bus->eid = MCTP_EID_NULL;
        bus->is_eid_assigned = false;
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

void mctp_set_bus_eid(
    mctp_binding_t *binding,
    mctp_eid_t eid,
    bool is_assigned
)
{
    if(binding != NULL)
    {
        if(binding->bus != NULL)
        {
            binding->bus->eid = eid;
            binding->bus->is_eid_assigned = is_assigned && (eid != MCTP_EID_NULL);
            uuid_gen(&(binding->bus->uuid));
        }
    }
}

mctp_eid_t mctp_get_bus_eid(
    mctp_binding_t *binding
)
{
    if(binding == NULL)
    {
        return MCTP_EID_NULL;
    }

    if(binding->bus == NULL)
    {
        return MCTP_EID_NULL;
    }

    return binding->bus->eid;
}

bool mctp_is_bus_eid_assigned(
    mctp_binding_t *binding
)
{
    if(binding == NULL)
    {
        return false;
    }

    if(binding->bus == NULL)
    {
        return false;
    }

    return binding->bus->is_eid_assigned;
}

void mctp_get_bus_uuid(
    mctp_binding_t *binding,
    mctp_uuid_t* uuid
)
{
    if(binding != NULL)
    {
        if(binding->bus != NULL)
        {
            memcpy(uuid->bytes, binding->bus->uuid.bytes, sizeof(uuid->bytes));

            return;
        }
    }

    memset(uuid->bytes, 0, sizeof(uuid->bytes));
}

void mctp_set_ctrl_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_message_rx_t ctrl_message_rx_callback,
    void* ctrl_message_rx_args
)
{
    mctp_inst->ctrl_message_rx_callback = ctrl_message_rx_callback;
    mctp_inst->ctrl_message_rx_args = ctrl_message_rx_args;
}

void mctp_set_pldm_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_message_rx_t pldm_message_rx_callback,
    void* pldm_message_rx_args
)
{
    mctp_inst->pldm_message_rx_callback = pldm_message_rx_callback;
    mctp_inst->pldm_message_rx_args = pldm_message_rx_args;
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
    if(mctp_inst->bus == NULL)
    {
        return;
    }

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

void mctp_message_disassemble(
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
        memcpy(packet_data, &message[payload_offset], payload_size);
        packet->buffer_len = packet_size;

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

void mctp_packet_queue_tx(
    mctp_bus_t* bus
)
{
    mctp_packet_buffer_t* packet = NULL;

	while ((packet = bus->tx_queue_head) != NULL) 
    {
		mctp_packet_tx(bus, packet);

        bus->tx_queue_head = packet->next;
        mctp_packet_buffer_destroy(packet);
	}
}

void mctp_packet_tx(
    mctp_bus_t* bus, 
    mctp_packet_buffer_t* packet
)
{
    bus->binding->packet_tx(
        bus->binding,
        packet
    );
}

void mctp_transaction_rx(
    mctp_bus_t* bus,
    mctp_packet_buffer_t* transaction
)
{
    if(bus == NULL)
    {
        return;
    }

    mctp_header_t* mctp_header = mctp_packet_buffer_header(transaction);

    if(mctp_header->destination != bus->eid
    && mctp_header->destination != MCTP_EID_BROADCAST
    && mctp_header->destination != MCTP_EID_NULL)
    {
        mctp_packet_buffer_destroy(transaction);
        return;
    }

    mctp_message_ctx_t* message_ctx = bus->incoming_messages[mctp_header->message_tag];

    if(mctp_header->start_of_message)
    {
        if(mctp_header->packet_sequence != 0)
        {
            mctp_packet_buffer_destroy(transaction);
            return;
        }

        if(mctp_header->end_of_message)
        {
            mctp_messsage_rx(
                bus->mctp_inst,
                bus->binding,
                mctp_header->destination,
                mctp_header->source,
                mctp_header->message_tag,
                mctp_header->tag_owner,
                mctp_packet_buffer_data(transaction),
                MCTP_PAYLOAD_SIZE(transaction->buffer_len)
            );

            mctp_packet_buffer_destroy(transaction);
        }   
        else
        {
            if(message_ctx != NULL)
            {
                mctp_message_ctx_destroy(message_ctx);
            }

            message_ctx = mctp_message_ctx_init(
                mctp_header->source, 
                mctp_header->tag_owner, 
                mctp_header->version
            );

            mctp_message_ctx_add_transaction(message_ctx, transaction);

            bus->incoming_messages[mctp_header->message_tag] = message_ctx;
        }
    }
    else
    {
        if(message_ctx == NULL)
        {
            mctp_packet_buffer_destroy(transaction);
            return;
        }

        if(message_ctx->version != mctp_header->version)
        {
            mctp_packet_buffer_destroy(transaction);
            return;
        }

        if(message_ctx->sequence != mctp_header->packet_sequence
        || message_ctx->tag_owner != mctp_header->tag_owner
        || message_ctx->rx_queue_head->buffer_len < transaction->buffer_len)
        {
            mctp_packet_buffer_destroy(transaction);
            mctp_message_ctx_destroy(message_ctx);
            bus->incoming_messages[mctp_header->message_tag] = NULL;
            return;
        }

        mctp_message_ctx_add_transaction(message_ctx, transaction);

        if(mctp_header->end_of_message)
        {
            uint8_t* message = mctp_message_assemble(message_ctx);

            mctp_messsage_rx(
                bus->mctp_inst,
                bus->binding,
                mctp_header->destination,
                mctp_header->source,
                mctp_header->message_tag,
                mctp_header->tag_owner,
                message,
                MCTP_PAYLOAD_SIZE(message_ctx->message_len)
            );

            mctp_message_ctx_destroy(message_ctx);
            bus->incoming_messages[mctp_header->message_tag] = NULL;

            free(message);
        }
        else
        {
            if(message_ctx->rx_queue_head->buffer_len != transaction->buffer_len)
            {
                mctp_message_ctx_destroy(message_ctx);
                bus->incoming_messages[mctp_header->message_tag] = NULL;
            }
        }
    }
}

uint8_t* mctp_message_assemble(
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
    mctp_binding_t* core_binding,
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len
)
{
    mctp_generic_header_t* generic_header = (mctp_generic_header_t*)message;

    switch (generic_header->type)
    {
        case MCTP_MSG_TYPE_CONTROL:
        {
            if(mctp_inst->ctrl_message_rx_callback == NULL)
            {
                break;
            }

            mctp_inst->ctrl_message_rx_callback(
                mctp_inst,
                core_binding,
                receiver,
                sender,
                message_tag,
                tag_owner,
                message,
                message_len,
                mctp_inst->ctrl_message_rx_args
            );
        }
        break;

        case MCTP_MSG_TYPE_PLDM:
        {
            if(mctp_inst->pldm_message_rx_callback == NULL)
            {
                break;
            }

            mctp_inst->pldm_message_rx_callback(
                mctp_inst,
                core_binding,
                receiver,
                sender,
                message_tag,
                tag_owner,
                message,
                message_len,
                mctp_inst->pldm_message_rx_args
            );
        }
        break;
    }
}