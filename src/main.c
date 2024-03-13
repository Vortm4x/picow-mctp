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



// void uart_isr();
bool init_all();
// void mctp_trace_packet(serial_pkt_t* uart_pkt);
// void mctp_trace_raw_packet(serial_pkt_t* uart_pkt);

/*
void uart_isr()
{
    uart_inst_t* uart_id = uart_get_instance(UART_INDEX);
    uart_hw_t* uart_hw = uart_get_hw(uart_id);

    if((uart_hw->ris & UART_UARTRIS_RXRIS_BITS) == 0)
    {
        return;
    }
    
    serial_pkt_t* uart_pkt = &uart_packets[UART_INDEX];

    while(uart_is_readable_within_us(uart_id, UART_READABLE_DELAY_US) && uart_pkt->pkt_buffer_len < MCTP_SERIAL_PKT_MAX)
    {
        uart_pkt->pkt_buffer[uart_pkt->pkt_buffer_len++] = uart_getc(uart_id);
    }

    uart_hw->icr = UART_UARTICR_RXIC_BITS;

    uart_pkt->received = true;
}
*/

/*
void mctp_control_response(serial_pkt_t* uart_pkt)
{


}

void mctp_trace_packet(serial_pkt_t* uart_pkt)
{
    mctp_serial_header_t* serial_header = (mctp_serial_header_t*)(uart_pkt->pkt_buffer);
    {
        printf("Serial header\n");
        printf("Framing flag (start): 0x%02X\n", serial_header->framing_flag);
        printf("Revision: 0x%02X\n", serial_header->revision);
        printf("Byte count: %d\n", serial_header->byte_count);
        printf("\n");
    }

    if(serial_header->framing_flag != MCTP_SERIAL_FF
    || serial_header->byte_count <= sizeof(mctp_header_t)
    || serial_header->revision != MCTP_SERIAL_REV)
    {
        printf("Packet error\n");
        return;
    }

    mctp_header_t* mctp_header = (mctp_header_t*)(serial_header + 1);
    {
        printf("MCTP header\n");
        printf("Version: %d\n", mctp_header->version);
        printf("Destination: 0x%02X\n", mctp_header->destination);
        printf("Source: 0x%02X\n", mctp_header->source);
        printf("SOM: %s\n", mctp_header->start_of_message ? "YES" : "NO");
        printf("EOM: %s\n", mctp_header->end_of_message ? "YES" : "NO");
        printf("Sequence: %d\n", mctp_header->packet_sequence);
        printf("Tag owner: %s\n", mctp_header->tag_owner ? "YES" : "NO");
        printf("Message tag: %d\n", mctp_header->message_tag);
        printf("\n");
    }

    mctp_generic_header_t* message_type = (mctp_generic_header_t*)(mctp_header + 1);
    {
        printf("MCTP body\n");
        printf("Integrity check: %s\n", message_type->integrity_check ? "YES" : "NO");
        printf("Message type: %d\n", message_type->type);
    }
    
    uint8_t* mctp_message = (uint8_t*)(message_type + 1);
    uint8_t mctp_message_len = serial_header->byte_count;
    {
        mctp_message_len -= ((uint8_t)sizeof(mctp_header_t));
        mctp_message_len -= ((uint8_t)sizeof(mctp_generic_header_t));

        printf("Message bytes[%zu]:", mctp_message_len);

        for(uint8_t i = 0; i < mctp_message_len; ++i)
        {
            printf(" %02X", mctp_message[i]);
        }
        printf("\n");
    }
    
    mctp_serial_trail_t* serial_tail = (mctp_serial_trail_t*)(mctp_message + mctp_message_len);
    {
        printf("CRC16: 0x%02X%02X\n", serial_tail->crc16_high, serial_tail->crc16_low);
        printf("Framing flag (end): 0x%02X\n", serial_tail->framing_flag);
        printf("\n");
    }

    printf("\n");    
}

void mctp_trace_raw_packet(serial_pkt_t* uart_pkt)
{
    size_t x = 0;

    printf("Serial header raw:");
    for(size_t i = 0; i < sizeof(mctp_serial_header_t); ++i, ++x)
    {
        printf(" %02X", uart_pkt->pkt_buffer[x]);
    }
    printf("\n");

    printf("MCTP header raw:");
    for(size_t i = 0; i < sizeof(mctp_header_t); ++i, ++x)
    {
        printf(" %02X", uart_pkt->pkt_buffer[x]);
    }
    printf("\n");
    
    printf("MCTP body raw:");
    for(size_t i = sizeof(mctp_header_t); i < uart_pkt->pkt_buffer[2]; ++i, ++x)
    {
        printf(" %02X", uart_pkt->pkt_buffer[x]);
    }
    printf("\n");

    printf("Serial tail raw:");
    for(size_t i = 0; i < sizeof(mctp_serial_trail_t); ++i, ++x)
    {
        printf(" %02X", uart_pkt->pkt_buffer[x]);
    }
    printf("\n");
}
*/

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


void mctp_uart_raw_tx_callback(uint8_t* buffer, size_t buffer_len, void* args)
{
    uart_inst_t* uart_id = uart_get_instance(UART_INDEX);

    uart_write_blocking(uart_id, buffer, buffer_len);
}


int main() 
{   
    if(!init_all())
    {
        return -1;
    }

    mctp_eid_t responder_eid = 0x08;
    mctp_eid_t requester_eid = 0xCE;
    uint8_t message_tag = 0;

    mctp_inst_t* mctp_inst = mctp_init();
    mctp_serial_binding_t* serial_binding = mctp_serial_init();

    mctp_serial_set_raw_tx_callback(serial_binding, mctp_uart_raw_tx_callback, NULL);
    mctp_register_bus(mctp_inst, mctp_serial_get_core_binding(serial_binding), serial_binding, requester_eid);

    size_t req_len = sizeof(mctp_generic_header_t) + sizeof(mctp_ctrl_header_t) + 0xFF;
    uint8_t get_eid_req[req_len];
    memset(get_eid_req, 0, req_len);
    mctp_generic_header_t* base = (mctp_generic_header_t*)get_eid_req;
    mctp_ctrl_header_t* ctrl_header = (mctp_ctrl_header_t*)(base + 1);

    base->integrity_check = false;
    base->type = MCTP_MSG_TYPE_CONTROL;
    ctrl_header->command = MCTP_CTRL_CMD_GET_ENDPOINT_ID;
    ctrl_header->datagram = false;
    ctrl_header->request = true;
    ctrl_header->instance = 0;

    while (true)
    {
        mctp_message_tx(
            mctp_inst,
            responder_eid,
            true,
            message_tag,
            get_eid_req,
            req_len
        );

        sleep_ms(2);
    }

    mctp_serial_destroy(serial_binding);
    mctp_destroy(mctp_inst);
    
    return 0;
}