#include <pico/platform.h>
#include <mctp/mctp.h>
#include <stdlib.h>
#include <string.h>

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
    void* transport_binding,
	mctp_eid_t eid
)
{
    mctp_inst->bus = malloc(sizeof(mctp_bus_t));

    if(mctp_inst->bus != NULL)
    {
        memset(mctp_inst->bus, 0, sizeof(mctp_bus_t));
        mctp_inst->bus->binding = binding;
        mctp_inst->bus->binding->transport_binding = transport_binding;
        mctp_inst->bus->eid = eid;
        binding->mctp_inst = mctp_inst;
    }
}

void mctp_unregister_bus(
    mctp_inst_t* mctp_inst, 
    mctp_binding_t *binding
)
{
	free(mctp_inst->bus);
    mctp_inst->bus = NULL;
    binding->mctp_inst = NULL;
}



/*
static int mctp_message_tx_on_bus(
    mctp_bus_t* bus,
    mctp_eid_t source,
    mctp_eid_t destination,
    bool tag_owner,
    uint8_t msg_tag,
    void *msg, 
    size_t msg_len
)
{
	size_t max_payload_len, payload_len, p;
	struct mctp_pktbuf *pkt;
	struct mctp_hdr *hdr;
	int i;

    //check if bus created
    //check msg tag

	max_payload_len = MCTP_BODY_SIZE(bus->binding->pkt_size);

	// {
	// 	const bool valid_mtu = max_payload_len >= MCTP_BTU;
	// 	assert(valid_mtu);
	// 	if (!valid_mtu)
	// 		return -EINVAL;
	// }

	// mctp_prdebug(
	// 	"%s: Generating packets for transmission of %zu byte message from %hhu to %hhu",
	// 	__func__, msg_len, src, dest);

	// queue up packets, each of max MCTP_MTU size 
	

    for (p = 0, i = 0; p < msg_len; i++) {
		payload_len = msg_len - p;
		if (payload_len > max_payload_len)
			payload_len = max_payload_len;

		pkt = mctp_pktbuf_alloc(bus->binding,
					payload_len + sizeof(*hdr));
		hdr = mctp_pktbuf_hdr(pkt);

		hdr->ver = bus->binding->version & 0xf;
		hdr->dest = dest;
		hdr->src = src;
		hdr->flags_seq_tag = (tag_owner << MCTP_HDR_TO_SHIFT) |
				     (msg_tag << MCTP_HDR_TAG_SHIFT);

		if (i == 0)
			hdr->flags_seq_tag |= MCTP_HDR_FLAG_SOM;
		if (p + payload_len >= msg_len)
			hdr->flags_seq_tag |= MCTP_HDR_FLAG_EOM;
		hdr->flags_seq_tag |= (i & MCTP_HDR_SEQ_MASK)
				      << MCTP_HDR_SEQ_SHIFT;

		memcpy(mctp_pktbuf_data(pkt), (uint8_t *)msg + p, payload_len);

		// add to tx queue 
		if (bus->tx_queue_tail)
			bus->tx_queue_tail->next = pkt;
		else
			bus->tx_queue_head = pkt;
		bus->tx_queue_tail = pkt;

		p += payload_len;

        
	}

	mctp_prdebug("%s: Enqueued %d packets", __func__, i);

	mctp_send_tx_queue(bus);

	return 0;
}

*/
static mctp_header_t* mctp_packet_header(
    mctp_packet_t* packet,
    mctp_binding_t* binding
)
{
    return (mctp_header_t*)(&packet->buffer[binding->binding_header_size]);
}

static uint8_t* mctp_packet_data(
    mctp_packet_t* packet,
    mctp_binding_t* binding
)
{
    return (uint8_t*)(&packet->buffer[binding->binding_header_size + sizeof(mctp_header_t)]);
}


static void mctp_packet_tx(
    mctp_bus_t* bus, 
    mctp_packet_t* packet
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
    mctp_packet_t* packet = NULL;

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
    mctp_packet_t* packet = NULL;
    mctp_packet_t* tx_queue_tail = NULL;

    size_t max_payload_size = MCTP_PAYLOAD_SIZE(
        bus->binding->max_transaction_size
    );
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
        size_t payload_size = MIN(bytes_left, max_payload_size);

        packet = malloc(sizeof(mctp_packet_t));
        memset(packet, 0, sizeof(mctp_packet_t));
        
        packet->buffer_len = MCTP_PACKET_SIZE(
            MCTP_TRANSACTION_SIZE(payload_size),
            bus->binding->binding_header_size,
            bus->binding->binding_trailer_size
        );
        packet->buffer = calloc(packet->buffer_len, sizeof(uint8_t));

        mctp_header_t* packet_header = mctp_packet_header(packet, bus->binding);
        uint8_t* packet_data = mctp_packet_data(packet, bus->binding);

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

    mctp_packet_header(bus->tx_queue_head, bus->binding)->start_of_message = true;
    mctp_packet_header(tx_queue_tail, bus->binding)->end_of_message = true;
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
