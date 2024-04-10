#include <stdio.h>
#include <ctype.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <hardware/uart.h>
#include <mctp/mctp.h>
#include <mctp/serial.h>
#include <mctp/control.h>
#include <pldm/pldm.h>
#include <pldm/base.h>
#include <pldm/mctp_transport.h>
#include "dump.h"
#include "control_handler.h"
#include "pldm_base_handler.h"
#include "pldm_platform_handler.h"


#define UART_INDEX 1
#define UART_IRQ UART1_IRQ
#define UART_BAUD_RATE 9600
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define UART_DATA_BITS 8
#define UART_STOP_BITS 1
#define UART_PARITY UART_PARITY_NONE

#define MCTP_BUS_OWNER_EID 0x08
#define MCTP_LOCAL_EID 0xC8


bool init_all();

void mctp_uart_raw_tx_callback(
    uint8_t* buffer, 
    size_t buffer_len, 
    void* args
);

void mctp_pldm_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len,
    void* args
);

void pldm_mctp_message_tx_callback(
    uint8_t receiver,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len,
    void* args
);


bool init_all()
{
    if(!stdio_init_all())
    {
        printf("stdio init failed\n");
        return false;
    }
    
    if (cyw43_arch_init() != PICO_OK) 
    {
        printf("CYW43 arch init failed\n");
        return false;
    }

    uart_inst_t* uart_id = uart_get_instance(UART_INDEX);


    uart_init(uart_id, UART_BAUD_RATE);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_set_hw_flow(uart_id, false, false);
    uart_set_format(uart_id, UART_DATA_BITS, UART_STOP_BITS, UART_PARITY);
    uart_set_fifo_enabled(uart_id, false);

    // irq_set_exclusive_handler(UART_IRQ, uart_isr);
    // irq_set_enabled(UART_IRQ, true);    
    // uart_set_irq_enables(uart_id, true, false);

    return true;
}


void mctp_uart_raw_tx_callback(
    uint8_t* buffer, 
    size_t buffer_len, 
    void* args
)
{
    uart_inst_t* uart_id = uart_get_instance(UART_INDEX);

    uart_write_blocking(uart_id, buffer, buffer_len);
}


void pldm_mctp_message_tx_callback(
    uint8_t receiver,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    mctp_binding_t* core_binding = (mctp_binding_t*)args;

    mctp_message_tx(
        core_binding,
        receiver,
        tag_owner,
        message_tag,
        message,
        message_len
    );
}


void mctp_pldm_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len,
    void* args
)
{   
    mctp_dump_transport(receiver, sender, message_tag, tag_owner);
    
    pldm_mctp_transport_t* mctp_transport = (pldm_mctp_transport_t*)args;

    pldm_mctp_message_rx(
        mctp_transport,
        receiver,
        sender,
        message_tag,
        tag_owner,
        message,
        message_len
    );
}


int main() 
{   
    if(!init_all())
    {
        return -1;
    }

    uart_inst_t* uart_id = uart_get_instance(UART_INDEX);

    mctp_inst_t* mctp_inst = mctp_init();
    mctp_serial_binding_t* serial_binding = mctp_serial_init();
    mctp_binding_t* core_binding = mctp_serial_get_core_binding(serial_binding);

    pldm_inst_t* pldm_inst = pldm_init();
    pldm_mctp_transport_t* mctp_transport = pldm_mctp_init();
    pldm_transport_t* core_transport = pldm_mctp_get_core_transport(mctp_transport);


    mctp_register_bus(mctp_inst, core_binding);
    mctp_serial_set_raw_tx_callback(serial_binding, mctp_uart_raw_tx_callback, NULL);
    mctp_set_ctrl_message_rx_callback(mctp_inst, mctp_ctrl_message_rx_callback, NULL);    
    mctp_set_pldm_message_rx_callback(mctp_inst, mctp_pldm_message_rx_callback, mctp_transport);

    pldm_register_terminus(pldm_inst, core_transport);
    pldm_mctp_set_message_tx_callback(mctp_transport, pldm_mctp_message_tx_callback, core_binding);    
    pldm_set_base_message_rx_callback(pldm_inst, pldm_base_message_rx_callback, NULL);
    pldm_set_platform_message_rx_callback(pldm_inst, pldm_platform_message_rx_callback, NULL);

    
    while (true)
    {
        if(uart_is_readable(uart_id))
        {
            mctp_serial_byte_rx(serial_binding, uart_getc(uart_id));
        }
    }

    pldm_mctp_destroy(mctp_transport);
    pldm_destroy(pldm_inst);

    mctp_serial_destroy(serial_binding);
    mctp_destroy(mctp_inst);
    
    return 0;
}