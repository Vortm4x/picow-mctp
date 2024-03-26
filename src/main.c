#include <stdio.h>
#include <ctype.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <hardware/uart.h>
#include <mctp/mctp.h>
#include <mctp/serial.h>
#include <mctp/control.h>


#define UART_INDEX 1
#define UART_IRQ UART1_IRQ
#define UART_BAUD_RATE 115200
#define UART_READABLE_DELAY_US 95
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define UART_DATA_BITS 8
#define UART_STOP_BITS 1
#define UART_PARITY UART_PARITY_NONE

#define MCTP_BUS_OWNER_EID 0x08

bool init_all();

void mctp_discovery_notify(
    mctp_inst_t* mctp_inst
);

void mctp_uart_raw_tx_callback(
    uint8_t* buffer, 
    size_t buffer_len, 
    void* args
);

void mctp_ctrl_message_rx_callback(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    bool integrity_check,
    mctp_ctrl_header_t* control_header,
    uint8_t* message_body,
    size_t body_len,
    void* args
);

void mctp_pldm_message_rx_callback(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    bool integrity_check,
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

void mctp_discovery_notify(
    mctp_inst_t* mctp_inst
)
{
    size_t msg_len = sizeof(mctp_generic_header_t) + sizeof(mctp_ctrl_header_t);
    uint8_t discovery_notify[msg_len];
    mctp_generic_header_t* base = (mctp_generic_header_t*)discovery_notify;
    mctp_ctrl_header_t* ctrl_header = (mctp_ctrl_header_t*)(base + 1);
    uint8_t message_tag = 0; 

    memset(discovery_notify, 0, msg_len);
    
    base->integrity_check = false;
    base->type = MCTP_MSG_TYPE_CONTROL;
    ctrl_header->command = MCTP_CTRL_CMD_DISCOVERY_NOTIFY;
    ctrl_header->request = true;
    ctrl_header->datagram = true;
    ctrl_header->instance = 0;

    mctp_message_tx(
        mctp_inst,
        MCTP_BUS_OWNER_EID,
        true,
        message_tag,
        discovery_notify,
        msg_len
    );
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


void mctp_ctrl_message_rx_callback(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    bool integrity_check,
    mctp_ctrl_header_t* control_header,
    uint8_t* message_body,
    size_t body_len,
    void* args
)
{
    printf("Transport info\n");
    printf("Sender: 0x%02X\n", sender);
    printf("Receiver: 0x%02X\n", receiver);
    printf("Tag owner: %s\n", tag_owner ? "YES" : "NO");
    printf("Message tag: %d\n", message_tag);
    printf("Integrity check: %s\n", integrity_check ? "YES" : "NO");
    printf("\n");

    printf("Control info\n");
    printf("Command: 0x%02X\n", control_header->command);
    printf("Datagram: %s\n", control_header->datagram ? "YES" : "NO");
    printf("Request: %s\n", control_header->request ? "YES" : "NO");
    printf("Instance: %d\n", control_header->instance);

    switch (control_header->command)
    {
        case MCTP_CTRL_CMD_DISCOVERY_NOTIFY:
        {
            if(!control_header->request && body_len == 1)
            {
                if(message_body[0] == MCTP_CTRL_CC_SUCCESS)
                {
                    printf("Discovery notify accepted\n");
                }
                else
                {
                    printf("Discovery notify error: 0x%02X\n", message_body[0]);
                }
            }
        }
        break;

        case MCTP_CTRL_CMD_SET_ENDPOINT_ID:
        {
            if(control_header->request && body_len == sizeof(mctp_req_set_eid_t))
            {
                mctp_req_set_eid_t* req = (mctp_req_set_eid_t*)message_body;
                
                if(req->operation == MCTP_SET_EID_OP_SET_EID
                && req->eid != MCTP_EID_NULL
                && req->eid != MCTP_EID_BROADCAST)
                {
                    
                }
            }
        }
        break;

        default: break;
    }
}

void mctp_pldm_message_rx_callback(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    bool integrity_check,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    printf("Transport info\n");
    printf("Sender: 0x%02X\n", sender);
    printf("Receiver: 0x%02X\n", receiver);
    printf("Tag owner: %s\n", tag_owner ? "YES" : "NO");
    printf("Message tag: %d\n", message_tag);
    printf("Integrity check: %s\n", integrity_check ? "YES" : "NO");
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

    mctp_register_bus(mctp_inst, mctp_serial_get_core_binding(serial_binding), MCTP_EID_NULL);
    mctp_serial_set_raw_tx_callback(serial_binding, mctp_uart_raw_tx_callback, NULL);
    mctp_set_ctrl_message_rx_callback(mctp_inst, mctp_ctrl_message_rx_callback, NULL);

    mctp_discovery_notify(mctp_inst);

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