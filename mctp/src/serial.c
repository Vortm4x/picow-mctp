#include <mctp/mctp.h>
#include <mctp/serial.h>
#include <stdlib.h>
#include <string.h>


mctp_binding_t* mctp_serial_get_core_binding(
    mctp_serial_binding_t* serial_binding
)
{
    return &serial_binding->binding;
}

mctp_serial_binding_t* mctp_serial_get_transport_binding(
    mctp_binding_t* binding
)
{
    return ((mctp_serial_binding_t*)binding->transport_binding);
}

static mctp_serial_header_t* mctp_serial_get_binding_header(
    mctp_packet_t* packet
)
{
    return (mctp_serial_header_t*)(&packet->buffer[0]);
}

static mctp_serial_trailer_t* mctp_serial_get_binding_trailer(
    mctp_packet_t* packet
)
{
    return (mctp_serial_trailer_t*)(&packet->buffer[packet->buffer_len - MCTP_SERIAL_TRAILER_SIZE]);
}

static int mctp_serial_packet_tx(
    mctp_binding_t* binding,
	mctp_packet_t* packet
)
{   
    mctp_serial_binding_t* serial_binding = mctp_serial_get_transport_binding(binding);
    
    mctp_serial_header_t* serial_header = mctp_serial_get_binding_header(packet);
    mctp_serial_trailer_t* serial_trailer = mctp_serial_get_binding_trailer(packet);

    serial_header->framing_flag = MCTP_SERIAL_FRAME_FLAG;
    serial_header->revision = MCTP_SERIAL_REVISION;
    serial_header->byte_count = MCTP_SERIAL_TRANSACTION_SIZE(packet->buffer_len);

    serial_trailer->fcs_high = 0x00;
    serial_trailer->fcs_low = 0x00;
    serial_trailer->framing_flag = MCTP_SERIAL_FRAME_FLAG;

    serial_binding->raw_tx_callback(
        packet->buffer, 
        packet->buffer_len,
        serial_binding->raw_tx_args
    );
}

mctp_serial_binding_t* mctp_serial_init()
{
    mctp_serial_binding_t* serial_binding = malloc(sizeof(mctp_serial_binding_t));

    if(serial_binding != NULL)
    {
        memset(serial_binding, 0, sizeof(mctp_serial_binding_t));

        serial_binding->binding.binding_type = MCTP_BINDING_TYPE_SERIAL;
        serial_binding->binding.version = MCTP_VERSION;
        serial_binding->binding.packet_tx = mctp_serial_packet_tx;
        
        serial_binding->binding.max_transaction_size = MCTP_SERIAL_MAX_TRANSACTION_SIZE;
	    serial_binding->binding.binding_header_size = MCTP_SERIAL_HEADER_SIZE;
	    serial_binding->binding.binding_trailer_size = MCTP_SERIAL_TRAILER_SIZE;
    }

    return serial_binding;
}

void mctp_serial_destroy(
    mctp_serial_binding_t* serial_binding
)
{
    free(serial_binding);
}


void mctp_serial_set_raw_tx_callback(
    mctp_serial_binding_t* serial_binding,
    serial_raw_tx_callback_t raw_tx_callback,
    void* raw_tx_args
)
{
    serial_binding->raw_tx_callback;
    serial_binding->raw_tx_args = raw_tx_args;
}