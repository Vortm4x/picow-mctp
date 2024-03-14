#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mctp/mctp.h>

#define MCTP_SERIAL_REVISION        0x01


typedef void (*mctp_serial_raw_tx_t)(
    uint8_t* buffer, 
    size_t buffer_len, 
    void* args
);

struct mctp_serial_binding_t;
typedef struct mctp_serial_binding_t mctp_serial_binding_t;


mctp_serial_binding_t* mctp_serial_init();

void mctp_serial_destroy(
    mctp_serial_binding_t* serial_binding
);

mctp_binding_t* mctp_serial_get_core_binding(
    mctp_serial_binding_t* serial_binding
);

void mctp_serial_set_raw_tx_callback(
    mctp_serial_binding_t* serial_binding,
    mctp_serial_raw_tx_t raw_tx_callback,
    void* raw_tx_args
);

void mctp_serial_byte_rx(
    mctp_serial_binding_t* serial_binding,
    uint8_t byte
);


#endif // SERIAL_H