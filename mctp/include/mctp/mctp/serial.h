#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mctp/mctp.h>

#define MCTP_SERIAL_FRAME_FLAG              0x7E
#define MCTP_SERIAL_REVISION                0x01

typedef struct mctp_serial_header_t
{
    uint8_t framing_flag;
    uint8_t revision;
    uint8_t byte_count;
}
mctp_serial_header_t;

#define MCTP_SERIAL_HEADER_SIZE (sizeof(mctp_serial_header_t))

typedef struct mctp_serial_trailer_t
{
    uint8_t fcs_high;
    uint8_t fcs_low;
    uint8_t framing_flag;
}
mctp_serial_trailer_t;

#define MCTP_SERIAL_TRAILER_SIZE (sizeof(mctp_serial_trailer_t))


typedef enum mctp_serial_rx_state_t
{
    MCTP_SERIAL_RX_STATE_WAIT_SYNC_START,
	MCTP_SERIAL_RX_STATE_WAIT_REVISION,
    MCTP_SERIAL_RX_STATE_WAIT_LEN,
	MCTP_SERIAL_RX_STATE_DATA,
	MCTP_SERIAL_RX_STATE_WAIT_FCS_HIGH,
	MCTP_SERIAL_RX_STATE_WAIT_FCS_LOW,
	MCTP_SERIAL_RX_STATE_WAIT_SYNC_END,
}
mctp_serial_rx_state_t;

typedef void (*serial_raw_tx_callback_t)(uint8_t* buffer, size_t buffer_len, void* args);

typedef struct mctp_serial_binding_t
{
    mctp_binding_t binding;
    mctp_serial_rx_state_t rx_state;
    mctp_packet_buffer_t* rx_transaction;
    uint16_t rx_fcs;
    serial_raw_tx_callback_t raw_tx_callback;
    void* raw_tx_args;
}
mctp_serial_binding_t;

#define MCTP_SERIAL_TRANSACTION_SIZE(packet_size) \
    (packet_size - MCTP_SERIAL_HEADER_SIZE - MCTP_SERIAL_TRAILER_SIZE)



// #define MCTP_SERIAL_PAYLOAD_MAX  (MCTP_SERIAL_MSG_MAX - sizeof(mctp_header_t))
// #define MCTP_SERIAL_MSG_LEN(payload_len)  (payload_len + sizeof(mctp_header_t))
// #define MCTP_SERIAL_PKT_LEN(msg_len) (sizeof(mctp_serial_header_t) + msg_len + sizeof(mctp_serial_trailer_t))

// typedef struct mctp_serial_pkt_t
// {
//     uint8_t pkt_buffer[MCTP_SERIAL_PKT_LEN(MCTP_SERIAL_MSG_MAX)];
//     size_t pkt_buffer_len;
//     struct mctp_serial_pkt_t* next;
// }
// mctp_serial_pkt_t;

mctp_serial_binding_t* mctp_serial_init();

void mctp_serial_destroy(
    mctp_serial_binding_t* serial_binding
);

mctp_binding_t* mctp_serial_get_core_binding(
    mctp_serial_binding_t* serial_binding
);

void mctp_serial_set_raw_tx_callback(
    mctp_serial_binding_t* serial_binding,
    serial_raw_tx_callback_t raw_tx_callback,
    void* raw_tx_args
);

void mctp_serial_byte_rx(
    mctp_serial_binding_t* serial_binding,
    uint8_t byte
);


#endif // SERIAL_H