/*

#ifndef BINDING_H
#define BINDING_H

#include <stdint.h>
#include "mctp/mctp.h"

struct mctp_bus 
{
	mctp_eid_t endpoint;
	bool enabled;
	struct mctp_pktbuf *tx_queue_head;
	struct mctp_pktbuf *tx_queue_tail;

	// todo: routing
};


typedef enum mctp_binding_type_t
{
    MCTP_BINDING_TYPE_RESERVED = 0x00,
    MCTP_BINDING_TYPE_SMBUS = 0x01,
    MCTP_BINDING_TYPE_PCIE_VDM = 0x02,
    MCTP_BINDING_TYPE_USB = 0x03,
    MCTP_BINDING_TYPE_KCS = 0x04,
    MCTP_BINDING_TYPE_SERIAL = 0x05
}
mctp_binding_type_t;

struct mctp_binding {
	mctp_binding_type_t type;
	uint8_t version;
	struct mctp_bus *bus;
	// struct mctp *mctp;
	size_t pkt_size;
	size_t pkt_header;
	size_t pkt_trailer;
	// int (*start)(struct mctp_binding *binding);
	// int (*tx)(struct mctp_binding *binding, struct mctp_pktbuf *pkt);
	// mctp_rx_fn control_rx;
	// void *control_rx_data;
};
#endif // BINDING_H

*/