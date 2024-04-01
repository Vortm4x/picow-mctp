#include <stdio.h>
#include <ctype.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <hardware/uart.h>
#include <mctp/mctp.h>
#include <mctp/serial.h>
#include <mctp/control.h>
#include <pldm/pldm.h>
#include "control_handler.h"


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

void mctp_message_tx_echo(
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

void mctp_ctrl_message_rx_callback(
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

void mctp_dump_transport(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner
);

void mctp_dump_ctrl(
    mctp_ctrl_header_t* ctrl_header
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

void mctp_message_tx_echo(
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
    mctp_eid_t old_eid = mctp_get_bus_eid(core_binding);
    bool is_old_assigned = mctp_is_bus_eid_assigned(core_binding);

    mctp_set_bus_eid(core_binding, sender, false);

    mctp_message_tx(
        mctp_inst,
        receiver,
        tag_owner,
        message_tag,
        message,
        message_len
    );

    mctp_set_bus_eid(core_binding, old_eid, is_old_assigned);
}

void mctp_ctrl_message_rx_callback(
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
    mctp_ctrl_header_t* ctrl_header = (mctp_ctrl_header_t*)message;

    mctp_dump_transport(receiver, sender, message_tag, tag_owner);
    mctp_dump_ctrl(ctrl_header);

    switch (ctrl_header->command)
    {
        case MCTP_CTRL_CMD_SET_ENDPOINT_ID:
        {
            if(tag_owner && ctrl_header->request)
            {
                handle_req_set_endpoint_id(
                    mctp_inst,
                    core_binding,
                    sender,
                    message_tag,
                    ctrl_header,
                    message_len
                );
            }
        }

        case MCTP_CTRL_CMD_GET_ENDPOINT_ID:
        {
            if(tag_owner && ctrl_header->request)
            {
                handle_req_get_endpoint_id(
                    mctp_inst,
                    core_binding,
                    sender,
                    message_tag,
                    ctrl_header,
                    message_len
                );
            }
        }
        break;

        case MCTP_CTRL_CMD_GET_ENDPOINT_UUID:
        {
            if(tag_owner && ctrl_header->request)
            {
                handle_req_get_endpoint_uuid(
                    mctp_inst,
                    core_binding,
                    sender,
                    message_tag,
                    ctrl_header,
                    message_len
                );
            }
        }
        break;

        case MCTP_CTRL_CMD_GET_VERSION_SUPPORT:
        {
            if(tag_owner && ctrl_header->request)
            {
                handle_req_get_mctp_ver(
                    mctp_inst,
                    core_binding,
                    sender,
                    message_tag,
                    ctrl_header,
                    message_len
                );
            }
        }
        break;

        case MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT:
        {
            if(tag_owner && ctrl_header->request)
            {
                handle_req_get_msg_type(
                    mctp_inst,
                    core_binding,
                    sender,
                    message_tag,
                    ctrl_header,
                    message_len
                );
            }   
        }
        break;

        default:
        {
            handle_error(
                mctp_inst,
                core_binding,
                sender,
                message_tag,
                ctrl_header,
                MCTP_CTRL_CC_MSG_TYPE_NOT_SUPPORTED
            );
        };
        break;
    }
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

}

void mctp_dump_transport(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner
)
{
    printf("Transport info\n");
    printf("Sender: 0x%02X\n", sender);
    printf("Receiver: 0x%02X\n", receiver);
    printf("Tag owner: %s\n", tag_owner ? "YES" : "NO");
    printf("Message tag: %d\n", message_tag);
    printf("\n");
}

void mctp_dump_ctrl(
    mctp_ctrl_header_t* ctrl_header
)
{
    printf("Control info\n");
    printf("Type: ctrl[%02X]\n", ctrl_header->base.type);
    printf("Integrity check: %s\n", ctrl_header->base.integrity_check ? "YES" : "NO");
    printf("Command: 0x%02X\n", ctrl_header->command);
    printf("Datagram: %s\n", ctrl_header->datagram ? "YES" : "NO");
    printf("Request: %s\n", ctrl_header->request ? "YES" : "NO");
    printf("Instance: %d\n", ctrl_header->instance);
    printf("\n");
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

    mctp_register_bus(mctp_inst, core_binding);
    mctp_set_bus_eid(core_binding, MCTP_LOCAL_EID, false);

    mctp_serial_set_raw_tx_callback(serial_binding, mctp_uart_raw_tx_callback, NULL);
    mctp_set_ctrl_message_rx_callback(mctp_inst, mctp_ctrl_message_rx_callback, NULL);
    mctp_set_pldm_message_rx_callback(mctp_inst, mctp_pldm_message_rx_callback, NULL);

    while (true)
    {
        if(uart_is_readable(uart_id))
        {
            mctp_serial_byte_rx(serial_binding, uart_getc(uart_id));
        }
    }

    mctp_serial_destroy(serial_binding);
    mctp_destroy(mctp_inst);
    
    return 0;
}