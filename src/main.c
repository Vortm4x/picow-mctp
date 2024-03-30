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
#define UART_BAUD_RATE 9600
#define UART_READABLE_DELAY_US 95
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define UART_DATA_BITS 8
#define UART_STOP_BITS 1
#define UART_PARITY UART_PARITY_NONE

#define MCTP_BUS_OWNER_EID 0x08
#define MCTP_LOCAL_EID 0xC8


bool init_all();

void mctp_discovery_notify(
    mctp_inst_t* mctp_inst
);

void mctp_get_version(
    mctp_inst_t* mctp_inst
);


void mctp_uart_raw_tx_callback(
    uint8_t* buffer, 
    size_t buffer_len, 
    void* args
);

void mctp_ctrl_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    mctp_ctrl_header_t* ctrl_header,
    uint8_t* message_body,
    size_t body_len,
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
    uint8_t message_tag = 4; 

    mctp_ctrl_header_t discovery_notify = {
        .base.integrity_check = false,
        .base.type = MCTP_MSG_TYPE_CONTROL,
        .command = MCTP_CTRL_CMD_DISCOVERY_NOTIFY,
        .request = true,
        .datagram = false,
        .instance = 0
    };

    uint8_t* message = (uint8_t*)&discovery_notify;
    size_t message_len = sizeof(mctp_ctrl_header_t);

    mctp_message_tx(
        mctp_inst,
        MCTP_BUS_OWNER_EID,
        true,
        message_tag,
        message,
        message_len
    );
}

void mctp_get_version(
    mctp_inst_t* mctp_inst
)
{
    size_t message_len = sizeof(mctp_ctrl_header_t) + sizeof(mctp_req_get_mctp_ver_t);
    uint8_t message[message_len];

    mctp_ctrl_header_t* ctrl_header =  (mctp_ctrl_header_t*)message;
    mctp_req_get_mctp_ver_t* req =  (mctp_req_get_mctp_ver_t*)(ctrl_header + 1);


    ctrl_header->base.integrity_check = false;
    ctrl_header->base.type = MCTP_MSG_TYPE_CONTROL;
    ctrl_header->command = MCTP_CTRL_CMD_GET_VERSION_SUPPORT;
    ctrl_header->request = true;
    ctrl_header->datagram = true;
    ctrl_header->instance = 2;

    req->message_type = MCTP_MSG_TYPE_BASE_SPEC;

    mctp_message_tx(mctp_inst, MCTP_BUS_OWNER_EID, true, 0x0, message, message_len);
    // mctp_message_tx(mctp_inst, MCTP_BUS_OWNER_EID, true, 0x1, message, message_len);
    // mctp_message_tx(mctp_inst, MCTP_BUS_OWNER_EID, true, 0x2, message, message_len);
    // mctp_message_tx(mctp_inst, MCTP_BUS_OWNER_EID, true, 0x3, message, message_len);
    // mctp_message_tx(mctp_inst, MCTP_BUS_OWNER_EID, true, 0x4, message, message_len);
    // mctp_message_tx(mctp_inst, MCTP_BUS_OWNER_EID, true, 0x5, message, message_len);
    // mctp_message_tx(mctp_inst, MCTP_BUS_OWNER_EID, true, 0x6, message, message_len);
    // mctp_message_tx(mctp_inst, MCTP_BUS_OWNER_EID, true, 0x7, message, message_len);
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
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
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
    printf("\n");

    printf("Message info\n");
    printf("Type: ctrl[%02X]\n", control_header->base.type);
    printf("Integrity check: %s\n", control_header->base.integrity_check ? "YES" : "NO");
    printf("Command: 0x%02X\n", control_header->command);
    printf("Datagram: %s\n", control_header->datagram ? "YES" : "NO");
    printf("Request: %s\n", control_header->request ? "YES" : "NO");
    printf("Instance: %d\n", control_header->instance);
    printf("\n");

    switch (control_header->command)
    {
        case MCTP_CTRL_CMD_DISCOVERY_NOTIFY:
        {
            if(!control_header->request)
            {
                mctp_resp_discovery_notify_t* resp = (mctp_resp_discovery_notify_t*)message_body;

                if(resp->completion_code == MCTP_CTRL_CC_SUCCESS)
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
            if(control_header->request)
            {
                mctp_req_set_endpoint_id_t* req = (mctp_req_set_endpoint_id_t*)message_body;
                
                size_t resp_len = sizeof(mctp_ctrl_header_t) + sizeof(mctp_resp_set_endpoint_id_t);
                uint8_t resp[resp_len];

                mctp_ctrl_header_t* resp_header = (mctp_ctrl_header_t*)resp;
                mctp_resp_set_endpoint_id_t* resp_body = (mctp_resp_set_endpoint_id_t*)(resp_header + 1);

                resp_header->base.integrity_check = false;
                resp_header->base.type = MCTP_MSG_TYPE_CONTROL;
                resp_header->command = MCTP_CTRL_CMD_SET_ENDPOINT_ID;
                resp_header->datagram = false;
                resp_header->request = false;
                resp_header->instance = control_header->instance;


                if(req->operation == MCTP_SET_EID_OP_SET_EID
                && req->eid != MCTP_EID_NULL
                && req->eid != MCTP_EID_BROADCAST)
                {
                    mctp_set_bus_eid(core_binding, req->eid);

                    resp_body->eis_assign_status = MCTP_EID_ASSIGN_ACCEPTED;
                }
                else
                {
                    resp_body->eis_assign_status = MCTP_EID_ASSIGN_REJECTED;
                }

                resp_body->completion_code = MCTP_CTRL_CC_SUCCESS;
                resp_body->eid_alloc_status = MCTP_EID_ALLOC_POOL_NOT_USED;
                resp_body->endpoint = mctp_get_bus_eid(core_binding);
                resp_body->eid_pool_size = 0;

                mctp_message_tx(
                    mctp_inst,
                    sender,
                    false,
                    message_tag,
                    resp,
                    resp_len
                );
            }
        }
        break;

        case MCTP_CTRL_CMD_GET_ENDPOINT_ID:
        {
            if(control_header->request)
            {
                size_t resp_len = sizeof(mctp_ctrl_header_t) + sizeof(mctp_resp_get_endpoint_id_t);
                uint8_t resp[resp_len];

                mctp_ctrl_header_t* resp_header = (mctp_ctrl_header_t*)resp;
                mctp_resp_get_endpoint_id_t* resp_body = (mctp_resp_get_endpoint_id_t*)(resp_header + 1);

                resp_header->base.integrity_check = false;
                resp_header->base.type = MCTP_MSG_TYPE_CONTROL;
                resp_header->command = MCTP_CTRL_CMD_GET_ENDPOINT_ID;
                resp_header->datagram = false;
                resp_header->request = false;
                resp_header->instance = control_header->instance;

                resp_body->completion_code = MCTP_CTRL_CC_SUCCESS;
                resp_body->endpoint = mctp_get_bus_eid(core_binding);
                resp_body->endpoint_type = MCTP_ENDPOINT_SIMPLE;
                resp_body->eid_type = MCTP_EID_STATIC_GENERIC;

                mctp_message_tx(
                    mctp_inst,
                    sender,
                    false,
                    message_tag,
                    resp,
                    resp_len
                );
            }
        }
        break;

        case MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT: 
        {
            if(control_header->request)
            {
                size_t resp_len = sizeof(mctp_ctrl_header_t) + sizeof(mctp_resp_get_msg_type_t) + sizeof(uint8_t);
                uint8_t resp[resp_len];

                mctp_ctrl_header_t* resp_header = (mctp_ctrl_header_t*)resp;
                mctp_resp_get_msg_type_t* resp_body = (mctp_resp_get_msg_type_t*)(resp_header + 1);
                uint8_t* supported_types = &resp_body->msg_type_entry;

                resp_header->base.integrity_check = false;
                resp_header->base.type = MCTP_MSG_TYPE_CONTROL;
                resp_header->command = MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT;
                resp_header->datagram = false;
                resp_header->request = false;
                resp_header->instance = control_header->instance;

                resp_body->completion_code = MCTP_CTRL_CC_SUCCESS;
                resp_body->msg_type_count = 2;
                supported_types[0] = MCTP_MSG_TYPE_CONTROL;
                supported_types[1] = MCTP_MSG_TYPE_PLDM;

                mctp_message_tx(
                    mctp_inst,
                    sender,
                    false,
                    message_tag,
                    resp,
                    resp_len
                );
            }
        }

        case MCTP_CTRL_CMD_GET_VERSION_SUPPORT:
        {
            mctp_req_get_mctp_ver_t* req = (mctp_req_get_mctp_ver_t*)message_body;

            size_t resp_len = sizeof(mctp_ctrl_header_t) + sizeof(mctp_resp_get_mctp_ver_t) + sizeof(mctp_ver_t) * 3;
            uint8_t resp[resp_len];
            
            mctp_ctrl_header_t* resp_header = (mctp_ctrl_header_t*)resp;
            mctp_resp_get_mctp_ver_t* resp_body = (mctp_resp_get_mctp_ver_t*)(resp_header + 1);
            mctp_ver_t* supported_versions = &resp_body->version_entry;

            resp_header->base.integrity_check = false;
            resp_header->base.type = MCTP_MSG_TYPE_CONTROL;
            resp_header->command = MCTP_CTRL_CMD_GET_VERSION_SUPPORT;
            resp_header->datagram = false;
            resp_header->request = false;
            resp_header->instance = control_header->instance;

            switch (req->message_type)
            {
                case MCTP_MSG_TYPE_BASE_SPEC:
                case MCTP_MSG_TYPE_CONTROL:
                {
                    resp_body->completion_code = MCTP_CTRL_CC_SUCCESS;
                    resp_body->version_count = 4;

                    supported_versions[0].version = 0xF1F0FF00;
                    supported_versions[1].version = 0xF1F1FF00;
                    supported_versions[2].version = 0xF1F2FF00;
                    supported_versions[3].version = 0xF1F3F100;
                }
                break;
            
                default:
                {
                    resp_body->completion_code = MCTP_CTRL_CC_MSG_TYPE_NOT_SUPPORTED;
                    resp_body->version_count = 0;
                    resp_body->version_entry.version = 0;

                    resp_len -= sizeof(mctp_ver_t) * 3;
                }
                break;
            }

            mctp_message_tx(
                mctp_inst,
                sender,
                false,
                message_tag,
                resp,
                resp_len
            );
        }

        default: break;
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
    printf("Transport info\n");
    printf("Sender: 0x%02X\n", sender);
    printf("Receiver: 0x%02X\n", receiver);
    printf("Tag owner: %s\n", tag_owner ? "YES" : "NO");
    printf("Message tag: %d\n", message_tag);
    printf("\n");

    mctp_set_bus_eid(core_binding, sender);

    mctp_message_tx(
        mctp_inst,
        receiver,
        tag_owner,
        message_tag,
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

    mctp_register_bus(mctp_inst, core_binding);
    mctp_serial_set_raw_tx_callback(serial_binding, mctp_uart_raw_tx_callback, NULL);
    mctp_set_ctrl_message_rx_callback(mctp_inst, mctp_ctrl_message_rx_callback, NULL);
    mctp_set_pldm_message_rx_callback(mctp_inst, mctp_pldm_message_rx_callback, NULL);

    //mctp_discovery_notify(mctp_inst);

    mctp_set_bus_eid(core_binding, MCTP_LOCAL_EID);
    mctp_get_version(mctp_inst);


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