#include <mctp/mctp.h>
#include <mctp/serial.h>
#include <packet_buffer.h>
#include <private_core.h>
#include <crc16.h>
#include <stdlib.h>
#include <string.h>


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


typedef struct mctp_serial_binding_t
{
    mctp_binding_t binding;
    mctp_serial_rx_state_t rx_state;
    mctp_packet_buffer_t* rx_transaction;
    size_t rx_transaction_size;
    uint16_t rx_fcs;
    uint16_t calc_fcs;
    mctp_serial_raw_tx_t raw_tx_callback;
    void* raw_tx_args;
}
mctp_serial_binding_t;


typedef struct __attribute__ ((__packed__)) mctp_serial_header_t
{
    uint8_t framing_flag;
    uint8_t revision;
    uint8_t byte_count;
}
mctp_serial_header_t;


typedef struct __attribute__ ((__packed__)) mctp_serial_trailer_t
{
    uint8_t fcs_high;
    uint8_t fcs_low;
    uint8_t framing_flag;
}
mctp_serial_trailer_t;


#define MCTP_SERIAL_HEADER_SIZE     (sizeof(mctp_serial_header_t))
#define MCTP_SERIAL_TRAILER_SIZE    (sizeof(mctp_serial_trailer_t))
#define MCTP_SERIAL_FRAME_FLAG      0x7E

#define MCTP_SERIAL_TRANSACTION_SIZE(packet_size) \
    (packet_size - MCTP_SERIAL_HEADER_SIZE - MCTP_SERIAL_TRAILER_SIZE)


static void mctp_serial_packet_tx(
    mctp_binding_t* binding,
	mctp_packet_buffer_t* packet
);

static mctp_serial_binding_t* mctp_serial_get_transport_binding(
    mctp_binding_t* binding
);

static mctp_serial_header_t* mctp_serial_get_binding_header(
    mctp_packet_buffer_t* packet
);

static mctp_serial_trailer_t* mctp_serial_get_binding_trailer(
    mctp_packet_buffer_t* packet
);


mctp_serial_binding_t* mctp_serial_init()
{
    mctp_serial_binding_t* serial_binding = malloc(sizeof(mctp_serial_binding_t));

    if(serial_binding != NULL)
    {
        memset(serial_binding, 0, sizeof(mctp_serial_binding_t));

        mctp_binding_t* binding = mctp_serial_get_core_binding(serial_binding); 

        binding->binding_type = MCTP_BINDING_TYPE_SERIAL;
        binding->version = MCTP_VERSION;        
        binding->binding_header_size = MCTP_SERIAL_HEADER_SIZE;
	    binding->binding_trailer_size = MCTP_SERIAL_TRAILER_SIZE;
        binding->packet_tx = mctp_serial_packet_tx;
        binding->transport_binding = serial_binding;
    }

    return serial_binding;
}

void mctp_serial_destroy(
    mctp_serial_binding_t* serial_binding
)
{
    free(serial_binding);
}

mctp_binding_t* mctp_serial_get_core_binding(
    mctp_serial_binding_t* serial_binding
)
{
    return &serial_binding->binding;
}

void mctp_serial_set_raw_tx_callback(
    mctp_serial_binding_t* serial_binding,
    mctp_serial_raw_tx_t raw_tx_callback,
    void* raw_tx_args
)
{
    serial_binding->raw_tx_callback = raw_tx_callback;
    serial_binding->raw_tx_args = raw_tx_args;
}

void mctp_serial_byte_rx(
    mctp_serial_binding_t* serial_binding,
    uint8_t byte
)
{
    switch(serial_binding->rx_state)
    {
        case MCTP_SERIAL_RX_STATE_WAIT_SYNC_START:
        {
            if(byte == MCTP_SERIAL_FRAME_FLAG)
            {
                serial_binding->rx_state = MCTP_SERIAL_RX_STATE_WAIT_REVISION;
                serial_binding->calc_fcs = MCTP_CRC16_INIT;
            }
        }
        break;

        case MCTP_SERIAL_RX_STATE_WAIT_REVISION:
        {
            if(byte = MCTP_SERIAL_REVISION)
            {
                serial_binding->rx_state = MCTP_SERIAL_RX_STATE_WAIT_LEN;
                serial_binding->calc_fcs = crc16_calc_byte(serial_binding->calc_fcs, byte);
            }
            else
            if(byte == MCTP_SERIAL_FRAME_FLAG)
            {} 
            else
            {
                serial_binding->rx_state = MCTP_SERIAL_RX_STATE_WAIT_SYNC_START;
            }
        }
        break;

        case MCTP_SERIAL_RX_STATE_WAIT_LEN:
        {
            if(sizeof(mctp_header_t) < byte && byte <= MCTP_TRANSACTION_SIZE(MCTP_MAX_PAYLOAD_SIZE))
            {
                mctp_packet_buffer_t* rx_transaction = mctp_packet_buffer_init(byte, 0);

                serial_binding->rx_transaction = rx_transaction;
                serial_binding->rx_transaction_size = byte;
                serial_binding->rx_state = MCTP_SERIAL_RX_STATE_DATA;
                serial_binding->calc_fcs = crc16_calc_byte(serial_binding->calc_fcs, byte);
            }
            else
            {
                serial_binding->rx_state = MCTP_SERIAL_RX_STATE_WAIT_SYNC_START;
            }
        }
        break;

        case MCTP_SERIAL_RX_STATE_DATA:
        {
            mctp_packet_buffer_t* rx_transaction = serial_binding->rx_transaction;

            rx_transaction->buffer[rx_transaction->buffer_len] = byte;
            rx_transaction->buffer_len++;

            if(rx_transaction->buffer_len == serial_binding->rx_transaction_size)
            {
                serial_binding->rx_state = MCTP_SERIAL_RX_STATE_WAIT_FCS_HIGH;
            }

            serial_binding->calc_fcs = crc16_calc_byte(serial_binding->calc_fcs, byte);
        }
        break;
        
        case MCTP_SERIAL_RX_STATE_WAIT_FCS_HIGH:
        {
            serial_binding->rx_fcs |= (byte << 8);
            serial_binding->rx_state = MCTP_SERIAL_RX_STATE_WAIT_FCS_LOW;
        }
        break;

        case MCTP_SERIAL_RX_STATE_WAIT_FCS_LOW:
        {
            serial_binding->rx_fcs |= (byte << 0);
            serial_binding->rx_state = MCTP_SERIAL_RX_STATE_WAIT_SYNC_END;
        }
        break;

        case MCTP_SERIAL_RX_STATE_WAIT_SYNC_END:
        {
            if(byte == MCTP_SERIAL_FRAME_FLAG && serial_binding->rx_fcs == serial_binding->calc_fcs)
            {
                mctp_transaction_rx(
                    serial_binding->binding.bus,
                    serial_binding->rx_transaction
                );
            }
            else
            {
                mctp_packet_buffer_destroy(serial_binding->rx_transaction);    
            }

            serial_binding->rx_transaction = NULL;
            serial_binding->rx_transaction_size = 0;
            serial_binding->rx_fcs = 0;
            serial_binding->calc_fcs = 0;
            serial_binding->rx_state = MCTP_SERIAL_RX_STATE_WAIT_SYNC_START;
        }
        break;
    }
}

void mctp_serial_packet_tx(
    mctp_binding_t* binding,
	mctp_packet_buffer_t* packet
)
{   
    mctp_serial_binding_t* serial_binding = mctp_serial_get_transport_binding(binding);
    mctp_serial_header_t* serial_header = mctp_serial_get_binding_header(packet);
    mctp_serial_trailer_t* serial_trailer = mctp_serial_get_binding_trailer(packet);

    serial_header->framing_flag = MCTP_SERIAL_FRAME_FLAG;

    serial_header->revision = MCTP_SERIAL_REVISION;
    serial_header->byte_count = MCTP_SERIAL_TRANSACTION_SIZE(packet->buffer_len);

    uint16_t crc = MCTP_CRC16_INIT;
    crc = crc16_calc_byte(crc, serial_header->revision);
    crc = crc16_calc_byte(crc, serial_header->byte_count);
    crc = crc16_calc_block(crc, (uint8_t*)mctp_packet_buffer_header(packet), serial_header->byte_count);
    serial_trailer->fcs_high = ((uint8_t)(crc >> 8));
    serial_trailer->fcs_low = ((uint8_t)(crc >> 0));


    serial_trailer->framing_flag = MCTP_SERIAL_FRAME_FLAG;

    serial_binding->raw_tx_callback(
        packet->buffer, 
        packet->buffer_len,
        serial_binding->raw_tx_args
    );
}

mctp_serial_binding_t* mctp_serial_get_transport_binding(
    mctp_binding_t* binding
)
{
    return ((mctp_serial_binding_t*)binding->transport_binding);
}

mctp_serial_header_t* mctp_serial_get_binding_header(
    mctp_packet_buffer_t* packet
)
{
    return (mctp_serial_header_t*)(&packet->buffer[0]);
}

mctp_serial_trailer_t* mctp_serial_get_binding_trailer(
    mctp_packet_buffer_t* packet
)
{
    return (mctp_serial_trailer_t*)(&packet->buffer[packet->buffer_len - MCTP_SERIAL_TRAILER_SIZE]);
}