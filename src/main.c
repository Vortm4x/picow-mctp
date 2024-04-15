#include <stdio.h>
#include <ctype.h>

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <pico/time.h>
#include <hardware/uart.h>

#include <mctp/mctp.h>
#include <mctp/serial.h>
#include <mctp/control.h>

#include <pldm/pldm.h>
#include <pldm/base.h>
#include <pldm/mctp_transport.h>
#include <pldm/pdr/repo.h>
#include <pldm/pdr/term_loc.h>
#include <pldm/pdr/num_sens.h>

#include "dump.h"
#include "control_handler.h"
#include "pldm_base_handler.h"
#include "pldm_platform_handler.h"
#include "dht11/driver.h"


#define UART_INDEX 1
#define UART_IRQ UART1_IRQ
#define UART_BAUD_RATE 9600
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define UART_DATA_BITS 8
#define UART_STOP_BITS 1
#define UART_PARITY UART_PARITY_NONE

#define DHT11_PIN 15

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

uint16_t pdr_gen_record_change();

pldm_pdr_header_t* pdr_create_mctp_term_loc(
    pldm_transport_t* core_transport,
    mctp_binding_t* core_binding
);

pldm_pdr_header_t* pdr_create_dht11_humidity_sens();

pldm_pdr_header_t* pdr_create_dht11_temperature_sens();

void dht11_humidity_sens_read(
    uint8_t data[],
    size_t data_len
);

void dht11_temperature_sens_read(
    uint8_t data[],
    size_t data_len
);

void mctp_bus_eid_changed_callback(
    mctp_binding_t* core_binding,
    void* args
); 

void pldm_tid_changed_callback(
    pldm_transport_t* transport,
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

    gpio_init(DHT11_PIN);

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


uint16_t pdr_gen_record_change()
{
    uint16_t ms_since_boot = to_ms_since_boot(
        get_absolute_time()
    );

    return ms_since_boot;
}


pldm_pdr_header_t* pdr_create_mctp_term_loc(
    pldm_transport_t* core_transport,
    mctp_binding_t* core_binding
)
{
    uint8_t locator_len = sizeof(pldm_term_loc_mctp_t);
    uint16_t pdr_data_len = sizeof(pldm_pdr_term_locator_t) + locator_len;
    uint8_t pdr_data[pdr_data_len];

    pldm_pdr_term_locator_t* pdr = (pldm_pdr_term_locator_t*)(&pdr_data[0]);
    pldm_term_loc_mctp_t* mctp_loc = (pldm_term_loc_mctp_t*)(pdr->locator_data);

    memset(pdr_data, 0, pdr_data_len);

    pdr->is_valid = true;
    pdr->locator_type = PLDM_LOCATOR_TYPE_MCTP;
    pdr->locator_len = locator_len;
    pdr->container_id = 0;
    pdr->terminus_handle = 0;
    pdr->terminus_id = pldm_get_terminus_id(core_transport);
    mctp_loc->mctp_eid = mctp_get_bus_eid(core_binding);

    return pldm_pdr_create_record(
        PLDM_PDR_TYPE_TERM_LOCATOR,
        pdr_gen_record_change(),
        pdr_data,
        pdr_data_len
    );
};


pldm_pdr_header_t* pdr_create_dht11_humidity_sens()
{
    uint16_t sens_data_len = sizeof(pldm_pdr_num_sens_data_type(uint8_t));
    uint16_t sens_range_len = sizeof(pldm_pdr_num_sens_range_type(uint8_t));

    uint16_t pdr_data_len = sizeof(pldm_pdr_num_sens_t) + sens_data_len + sens_range_len;
    uint8_t pdr_data[pdr_data_len];

    pldm_pdr_num_sens_t* nums_sens = (pldm_pdr_num_sens_t*)&pdr_data[0];
    pldm_pdr_num_sens_data_type(uint8_t)* num_sens_data = (pldm_pdr_num_sens_data_type(uint8_t)*)(pdr_data + 1);
    pldm_pdr_num_sens_range_type(uint8_t)* num_sens_range = (pldm_pdr_num_sens_range_type(uint8_t)*)(num_sens_data + 1);

    memset(pdr_data, 0, pdr_data_len);

    nums_sens->terminus_handle = 0;
    nums_sens->entity_type = 0;
    nums_sens->entity_inst = 0;
    nums_sens->container_id = 0;
    nums_sens->sensor_init = PLDM_PDR_INIT_NO_INIT;
    nums_sens->aux_names = false;
    nums_sens->base_unit.measure = PLDM_NUM_MEASURE_UNIT_PERCENTAGE;
    nums_sens->base_unit.modifier = 0;
    nums_sens->base_unit.rate = PLDM_NUM_RATE_UNIT_NONE;
    nums_sens->base_oem_unit_handle = 0;
    nums_sens->aux_unit.measure = PLDM_NUM_MEASURE_UNIT_NONE;
    nums_sens->aux_unit.modifier = 0;
    nums_sens->aux_unit.rate = PLDM_NUM_RATE_UNIT_NONE;
    nums_sens->unit_rel = PLDM_SENSOR_UNIT_REL_MUL_BY;
    nums_sens->aux_oem_unit_handle = 0;
    nums_sens->is_linear = true;

    num_sens_data->data_size = PLDM_PDR_DATA_SIZE_UNSIGNED_INT8;
    num_sens_data->resolution.exponent = 0;
    num_sens_data->resolution.mantissa = 1;
    num_sens_data->resolution.sign = false;
    num_sens_data->offset.exponent = 0;
    num_sens_data->offset.mantissa = 0;
    num_sens_data->resolution.sign = false;
    num_sens_data->accuracy = 500;
    num_sens_data->plus_tolerance = 5;
    num_sens_data->minus_tolerance = 5;
    num_sens_data->hysteresis = 1;
    num_sens_data->state_transition_interval.exponent = 0;
    num_sens_data->state_transition_interval.mantissa = 0;
    num_sens_data->state_transition_interval.sign = false;
    num_sens_data->update_interval.exponent = 0;
    num_sens_data->update_interval.mantissa = 30;
    num_sens_data->update_interval.sign = false;
    num_sens_data->min_readable = 20;
    num_sens_data->max_readable = 90;

    num_sens_range->range_size = PLDM_PDR_DATA_SIZE_UNSIGNED_INT8;
    num_sens_range->range_support.normal_max = true;
    num_sens_range->range_support.normal_min = true;
    num_sens_range->normal_max = 90;
    num_sens_range->normal_min = 20;

    return  pldm_pdr_create_record(
        PLDM_PDR_TYPE_NUM_SENS,
        pdr_gen_record_change(),
        pdr_data,
        pdr_data_len
    );
}

pldm_pdr_header_t* pdr_create_dht11_temperature_sens()
{
    uint16_t sens_data_len = sizeof(pldm_pdr_num_sens_data_type(uint8_t));
    uint16_t sens_range_len = sizeof(pldm_pdr_num_sens_range_type(uint8_t));

    uint16_t pdr_data_len = sizeof(pldm_pdr_num_sens_t) + sens_data_len + sens_range_len;
    uint8_t pdr_data[pdr_data_len];

    pldm_pdr_num_sens_t* nums_sens = (pldm_pdr_num_sens_t*)&pdr_data[0];
    pldm_pdr_num_sens_data_type(uint8_t)* num_sens_data = (pldm_pdr_num_sens_data_type(uint8_t)*)(pdr_data + 1);
    pldm_pdr_num_sens_range_type(uint8_t)* num_sens_range = (pldm_pdr_num_sens_range_type(uint8_t)*)(num_sens_data + 1);

    memset(pdr_data, 0, pdr_data_len);

    nums_sens->terminus_handle = 0;
    nums_sens->entity_type = 0;
    nums_sens->entity_inst = 0;
    nums_sens->container_id = 0;
    nums_sens->sensor_init = PLDM_PDR_INIT_NO_INIT;
    nums_sens->aux_names = false;
    nums_sens->base_unit.measure = PLDM_NUM_MEASURE_UNIT_DEG_C;
    nums_sens->base_unit.modifier = 0;
    nums_sens->base_unit.rate = PLDM_NUM_RATE_UNIT_NONE;
    nums_sens->base_oem_unit_handle = 0;
    nums_sens->aux_unit.measure = PLDM_NUM_MEASURE_UNIT_NONE;
    nums_sens->aux_unit.modifier = 0;
    nums_sens->aux_unit.rate = PLDM_NUM_RATE_UNIT_NONE;
    nums_sens->unit_rel = PLDM_SENSOR_UNIT_REL_MUL_BY;
    nums_sens->aux_oem_unit_handle = 0;
    nums_sens->is_linear = true;

    num_sens_data->data_size = PLDM_PDR_DATA_SIZE_UNSIGNED_INT8;
    num_sens_data->resolution.exponent = 0;
    num_sens_data->resolution.mantissa = 1;
    num_sens_data->resolution.sign = false;
    num_sens_data->offset.exponent = 0;
    num_sens_data->offset.mantissa = 0;
    num_sens_data->resolution.sign = false;
    num_sens_data->accuracy = 400;
    num_sens_data->plus_tolerance = 2;
    num_sens_data->minus_tolerance = 2;
    num_sens_data->hysteresis = 0;
    num_sens_data->state_transition_interval.exponent = 0;
    num_sens_data->state_transition_interval.mantissa = 0;
    num_sens_data->state_transition_interval.sign = false;
    num_sens_data->update_interval.exponent = 0;
    num_sens_data->update_interval.mantissa = 30;
    num_sens_data->update_interval.sign = false;
    num_sens_data->min_readable = 0;
    num_sens_data->max_readable = 50;

    num_sens_range->range_size = PLDM_PDR_DATA_SIZE_UNSIGNED_INT8;
    num_sens_range->range_support.normal_max = true;
    num_sens_range->range_support.normal_min = true;
    num_sens_range->normal_max = 0;
    num_sens_range->normal_min = 50;

    return  pldm_pdr_create_record(
        PLDM_PDR_TYPE_NUM_SENS,
        pdr_gen_record_change(),
        pdr_data,
        pdr_data_len
    );
}


void dht11_humidity_sens_read(
    uint8_t data[],
    size_t data_len
)
{
    if(data_len != sizeof(uint8_t))
    {
        return;
    }

    dht11_request_data(DHT11_PIN);

    sleep_ms(300);

    dht11_data_t* sensor_data = dht11_sensor_data();

    memcpy(data, &sensor_data->integral_RH, data_len);
}


void dht11_temperature_sens_read(
    uint8_t data[],
    size_t data_len
)
{
    if(data_len != sizeof(uint8_t))
    {
        return;
    }

    dht11_request_data(DHT11_PIN);

    sleep_ms(300);

    dht11_data_t* sensor_data = dht11_sensor_data();

    memcpy(data, &sensor_data->integral_T, data_len);
}


void mctp_bus_eid_changed_callback(
    mctp_binding_t* core_binding,
    void* args
)
{
    pldm_pdr_repo_entry_t* term_loc_entry = (pldm_pdr_repo_entry_t*)args;

    pldm_pdr_header_t* term_loc_record = pldm_pdr_repo_entry_get_record(term_loc_entry);

    if(term_loc_record == NULL)
    {
        return;
    }

    pldm_pdr_term_locator_t* term_loc = (pldm_pdr_term_locator_t*)term_loc_record->data;
    pldm_term_loc_mctp_t* mctp_loc = (pldm_term_loc_mctp_t*)term_loc->locator_data;

    mctp_loc->mctp_eid = mctp_get_bus_eid(core_binding);
    term_loc_record->record_change = pdr_gen_record_change();
}


void pldm_tid_changed_callback(
    pldm_transport_t* transport,
    void* args
)
{
    pldm_pdr_repo_entry_t* term_loc_entry = (pldm_pdr_repo_entry_t*)args;

    pldm_pdr_header_t* term_loc_record = pldm_pdr_repo_entry_get_record(term_loc_entry);

    if(term_loc_record == NULL)
    {
        return;
    }

    pldm_pdr_term_locator_t* term_loc = (pldm_pdr_term_locator_t*)term_loc_record->data;

    term_loc->terminus_id = pldm_get_terminus_id(transport);
    term_loc_record->record_change = pdr_gen_record_change();
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

    pldm_pdr_repo_t* pdr_repo = pldm_pdr_repo_init();

    mctp_register_bus(mctp_inst, core_binding);
    mctp_serial_set_raw_tx_callback(serial_binding, mctp_uart_raw_tx_callback, NULL);
    mctp_set_ctrl_message_rx_callback(mctp_inst, mctp_ctrl_message_rx_callback, NULL);    
    mctp_set_pldm_message_rx_callback(mctp_inst, mctp_pldm_message_rx_callback, mctp_transport);

    pldm_register_terminus(pldm_inst, core_transport);
    pldm_mctp_set_message_tx_callback(mctp_transport, pldm_mctp_message_tx_callback, core_binding);    
    pldm_set_base_message_rx_callback(pldm_inst, pldm_base_message_rx_callback, NULL);
    pldm_set_platform_message_rx_callback(pldm_inst, pldm_platform_message_rx_callback, pdr_repo);


    pldm_pdr_repo_entry_t* mctp_term_loc = pldm_pdr_repo_add_entry(
        pdr_repo, 
        pdr_create_mctp_term_loc(
            core_transport, 
            core_binding
        )
    );
    
    mctp_set_bus_eid_changed_callback(core_binding, mctp_bus_eid_changed_callback, mctp_term_loc);
    pldm_set_terminus_id_changed_callback(core_transport, pldm_tid_changed_callback, mctp_term_loc);


    pldm_pdr_num_sens_t* dht11_hum_sens = (pldm_pdr_num_sens_t*)pldm_pdr_repo_entry_get_record(
        pldm_pdr_repo_add_entry(
            pdr_repo, 
            pdr_create_dht11_humidity_sens()
        )
    )->data;

    pldm_pdr_repo_sensor_set_read_callback(
        pldm_pdr_repo_get_sensor(
            pdr_repo, 
            dht11_hum_sens->sensor_id
        ),
        dht11_humidity_sens_read
    );


    pldm_pdr_num_sens_t* dht11_temp_sens = (pldm_pdr_num_sens_t*)pldm_pdr_repo_entry_get_record(
        pldm_pdr_repo_add_entry(
            pdr_repo, 
            pdr_create_dht11_temperature_sens()
        )
    )->data;

    pldm_pdr_repo_sensor_set_read_callback(
        pldm_pdr_repo_get_sensor(
            pdr_repo, 
            dht11_temp_sens->sensor_id
        ),
        dht11_temperature_sens_read
    );


    while (true)
    {
        if(uart_is_readable(uart_id))
        {
            mctp_serial_byte_rx(serial_binding, uart_getc(uart_id));
        }
    }

    pldm_pdr_repo_destroy(pdr_repo);

    pldm_mctp_destroy(mctp_transport);
    pldm_destroy(pldm_inst);

    mctp_serial_destroy(serial_binding);
    mctp_destroy(mctp_inst);
    
    return 0;
}