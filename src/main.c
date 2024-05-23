#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <pico/time.h>
#include <hardware/uart.h>

#include <mctp/mctp.h>
#include <mctp/serial.h>
#include <mctp/control.h>

#include <pldm/pldm.h>
#include <pldm/util.h>
#include <pldm/mctp_transport.h>
#include <pldm/message/base.h>

#include <pldm/pdr/repo.h>
#include <pldm/pdr/pdr.h>
#include <pldm/pdr/record/terminus_locator.h>
#include <pldm/pdr/record/numeric_sensor.h>
#include <pldm/pdr/record/redfish_resource.h>
#include <pldm/rde/provider.h>
#include <pldm/rde/resource.h>

#include "dump.h"
#include "control_handler.h"
#include "pldm_base_handler.h"
#include "pldm_platform_handler.h"
#include "pldm_redfish_handler.h"
#include "dht11/driver.h"
#include "dict/chassis.h"
#include "dict/sensor_collection.h"
#include "dict/sensor.h"
#include "rde_resources.h"


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

#define PLDM_MAX_MESSAGE_LEN 1024


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

void mctp_bus_eid_changed_callback(
    mctp_binding_t* core_binding,
    void* args
); 

void pldm_tid_changed_callback(
    pldm_transport_t* transport,
    void* args
);

uint16_t pdr_gen_record_change();

pldm_pdr_header_t* pdr_create_mctp_terminus_locator(
    pldm_transport_t* core_transport,
    mctp_binding_t* core_binding
);

pldm_pdr_header_t* pdr_create_dht11_humidity_sens();

pldm_pdr_header_t* pdr_create_dht11_temperature_sens();

bool dht11_humidity_sens_read(
    pldm_pdr_data_size_t size,
    pldm_numeric_sensor_value_t* value,
    void* args
);

bool dht11_temperature_sens_read(
    pldm_pdr_data_size_t size,
    pldm_numeric_sensor_value_t* value,
    void* args
);

pldm_pdr_header_t* pdr_create_redfish_resource(
    uint32_t res_id,
    uint32_t containing_res_id,
    bool is_collection,
    bool is_entry,
    uint8_t prop_containing_res_name_data[],
    uint16_t prop_containing_res_name_data_len,
    uint8_t sub_uri_data[],
    uint16_t sub_uri_data_len,
    uint8_t schema_name_data[],
    uint16_t schema_name_data_len,
    uint32_t schema_version,
    uint16_t schema_dict_len,
    uint32_t schema_dict_sig
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


void mctp_bus_eid_changed_callback(
    mctp_binding_t* core_binding,
    void* args
)
{
    pldm_pdr_entry_t* terminus_locator_entry = (pldm_pdr_entry_t*)args;
    pldm_pdr_header_t* terminus_locator_record = pldm_pdr_entry_get_record(terminus_locator_entry);

    if(terminus_locator_record == NULL)
    {
        return;
    }

    pldm_pdr_terminus_locator_t* terminus_locator = (pldm_pdr_terminus_locator_t*)(pldm_pdr_get_data(terminus_locator_record));
    pldm_terminus_locator_mctp_t* mctp_locator = (pldm_terminus_locator_mctp_t*)(terminus_locator->locator_data);

    mctp_locator->mctp_eid = mctp_get_bus_eid(core_binding);
    pldm_pdr_set_record_change(
        terminus_locator_record,
        pdr_gen_record_change()
    );
}


void pldm_tid_changed_callback(
    pldm_transport_t* transport,
    void* args
)
{
    pldm_pdr_entry_t* terminus_locator_entry = (pldm_pdr_entry_t*)args;
    pldm_pdr_header_t* terminus_locator_record = pldm_pdr_entry_get_record(terminus_locator_entry);

    if(terminus_locator_record == NULL)
    {
        return;
    }

    pldm_pdr_terminus_locator_t* terminus_locator = (pldm_pdr_terminus_locator_t*)(pldm_pdr_get_data(terminus_locator_record));

    terminus_locator->terminus_id = pldm_terminus_get_id(transport);
    pldm_pdr_set_record_change(
        terminus_locator_record,
        pdr_gen_record_change()
    );
}


uint16_t pdr_gen_record_change()
{
    uint16_t ms_since_boot = to_ms_since_boot(
        get_absolute_time()
    );

    return ms_since_boot;
}


pldm_pdr_header_t* pdr_create_mctp_terminus_locator(
    pldm_transport_t* core_transport,
    mctp_binding_t* core_binding
)
{
    uint8_t locator_len = sizeof(pldm_terminus_locator_mctp_t);
    uint16_t pdr_data_len = sizeof(pldm_pdr_terminus_locator_t) + locator_len;
    uint8_t pdr_data[pdr_data_len];

    pldm_pdr_terminus_locator_t* pdr = (pldm_pdr_terminus_locator_t*)(&pdr_data[0]);
    pldm_terminus_locator_mctp_t* mctp_loc = (pldm_terminus_locator_mctp_t*)(pdr->locator_data);

    memset(pdr_data, 0, pdr_data_len);

    pdr->is_valid = true;
    pdr->locator_type = PLDM_LOCATOR_TYPE_MCTP;
    pdr->locator_len = locator_len;
    pdr->container_id = PLDM_PDR_CONTAINER_PLDM_SYSTEM;
    pdr->terminus_handle = 1;
    pdr->terminus_id = pldm_terminus_get_id(core_transport);
    mctp_loc->mctp_eid = mctp_get_bus_eid(core_binding);

    return pldm_pdr_init(
        PLDM_PDR_TYPE_TERM_LOCATOR,
        pdr_gen_record_change(),
        pdr_data,
        pdr_data_len
    );
}


pldm_pdr_header_t* pdr_create_dht11_humidity_sens()
{
    uint16_t sens_data_len = sizeof(pldm_pdr_numeric_sensor_data_type(uint8_t));
    uint16_t sens_range_len = sizeof(pldm_pdr_numeric_sensor_range_type(uint8_t));

    uint16_t pdr_data_len = sizeof(pldm_pdr_base_numeric_sensor_t) + sens_data_len + sens_range_len;
    uint8_t pdr_data[pdr_data_len];

    pldm_pdr_base_numeric_sensor_t* num_sens_base = (pldm_pdr_base_numeric_sensor_t*)&pdr_data[0];
    pldm_pdr_numeric_sensor_data_type(uint8_t)* num_sens_data = (pldm_pdr_numeric_sensor_data_type(uint8_t)*)(pdr_data + 1);
    pldm_pdr_numeric_sensor_range_type(uint8_t)* num_sens_range = (pldm_pdr_numeric_sensor_range_type(uint8_t)*)(num_sens_data + 1);

    memset(pdr_data, 0, pdr_data_len);

    num_sens_base->base_accessor.terminus_handle = 1;
    num_sens_base->base_accessor.entity_info.type.id = PLDM_ENTITY_EXT_ENV;
    num_sens_base->base_accessor.entity_info.type.is_logical = false;
    num_sens_base->base_accessor.entity_info.instance_number = 1;
    num_sens_base->base_accessor.entity_info.container_id = PLDM_PDR_CONTAINER_PLDM_SYSTEM;
    num_sens_base->base_accessor.accessor_id = pldm_pdr_next_sensor_id();

    num_sens_base->numeric_accessor.init = PLDM_PDR_ACCESOR_INIT_NO_INIT;
    num_sens_base->numeric_accessor.aux_names_pdr = false;
    num_sens_base->numeric_accessor.base_unit.measure = PLDM_NUM_MEASURE_UNIT_PERCENTAGE;
    num_sens_base->numeric_accessor.base_unit.modifier = 0;
    num_sens_base->numeric_accessor.base_unit.rate = PLDM_NUM_RATE_UNIT_NONE;
    num_sens_base->numeric_accessor.base_oem_unit_handle = 0;
    num_sens_base->numeric_accessor.aux_unit.measure = PLDM_NUM_MEASURE_UNIT_NONE;
    num_sens_base->numeric_accessor.aux_unit.modifier = 0;
    num_sens_base->numeric_accessor.aux_unit.rate = PLDM_NUM_RATE_UNIT_NONE;

    num_sens_base->directly_proportional = true;
    num_sens_base->aux_oem_unit_handle = 0;
    num_sens_base->linear = true;

    num_sens_data->base_data.format = PLDM_PDR_DATA_SIZE_UNSIGNED_INT8;
    num_sens_data->base_data.resolution.exponent = 0;
    num_sens_data->base_data.resolution.mantissa = 1;
    num_sens_data->base_data.resolution.sign = false;
    num_sens_data->base_data.offset.exponent = 0;
    num_sens_data->base_data.offset.mantissa = 0;
    num_sens_data->base_data.resolution.sign = false;
    num_sens_data->base_data.accuracy = 500;
    num_sens_data->base_data.plus_tolerance = 5;
    num_sens_data->base_data.minus_tolerance = 5;

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

    return pldm_pdr_init(
        PLDM_PDR_TYPE_NUM_SENS,
        pdr_gen_record_change(),
        pdr_data,
        pdr_data_len
    );
}


pldm_pdr_header_t* pdr_create_dht11_temperature_sens()
{
    uint16_t sens_data_len = sizeof(pldm_pdr_numeric_sensor_data_type(uint8_t));
    uint16_t sens_range_len = sizeof(pldm_pdr_numeric_sensor_range_type(uint8_t));

    uint16_t pdr_data_len = sizeof(pldm_pdr_base_numeric_sensor_t) + sens_data_len + sens_range_len;
    uint8_t pdr_data[pdr_data_len];

    pldm_pdr_base_numeric_sensor_t* num_sens_base = (pldm_pdr_base_numeric_sensor_t*)&pdr_data[0];
    pldm_pdr_numeric_sensor_data_type(uint8_t)* num_sens_data = (pldm_pdr_numeric_sensor_data_type(uint8_t)*)(pdr_data + 1);
    pldm_pdr_numeric_sensor_range_type(uint8_t)* num_sens_range = (pldm_pdr_numeric_sensor_range_type(uint8_t)*)(num_sens_data + 1);

    memset(pdr_data, 0, pdr_data_len);

    num_sens_base->base_accessor.terminus_handle = 1;
    num_sens_base->base_accessor.entity_info.type.id = PLDM_ENTITY_EXT_ENV;
    num_sens_base->base_accessor.entity_info.type.is_logical = false;
    num_sens_base->base_accessor.entity_info.instance_number = 2;
    num_sens_base->base_accessor.entity_info.container_id = PLDM_PDR_CONTAINER_PLDM_SYSTEM;
    num_sens_base->base_accessor.accessor_id = pldm_pdr_next_sensor_id();

    num_sens_base->numeric_accessor.init = PLDM_PDR_ACCESOR_INIT_NO_INIT;
    num_sens_base->numeric_accessor.aux_names_pdr = false;
    num_sens_base->numeric_accessor.base_unit.measure = PLDM_NUM_MEASURE_UNIT_DEG_C;
    num_sens_base->numeric_accessor.base_unit.modifier = 0;
    num_sens_base->numeric_accessor.base_unit.rate = PLDM_NUM_RATE_UNIT_NONE;
    num_sens_base->numeric_accessor.base_oem_unit_handle = 0;
    num_sens_base->numeric_accessor.aux_unit.measure = PLDM_NUM_MEASURE_UNIT_NONE;
    num_sens_base->numeric_accessor.aux_unit.modifier = 0;
    num_sens_base->numeric_accessor.aux_unit.rate = PLDM_NUM_RATE_UNIT_NONE;

    num_sens_base->directly_proportional = true;
    num_sens_base->aux_oem_unit_handle = 0;
    num_sens_base->linear = true;

    num_sens_data->base_data.format = PLDM_PDR_DATA_SIZE_UNSIGNED_INT8;
    num_sens_data->base_data.resolution.exponent = 0;
    num_sens_data->base_data.resolution.mantissa = 1;
    num_sens_data->base_data.resolution.sign = false;
    num_sens_data->base_data.offset.exponent = 0;
    num_sens_data->base_data.offset.mantissa = 0;
    num_sens_data->base_data.resolution.sign = false;
    num_sens_data->base_data.accuracy = 400;
    num_sens_data->base_data.plus_tolerance = 2;
    num_sens_data->base_data.minus_tolerance = 2;

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
    num_sens_range->normal_max = 50;
    num_sens_range->normal_min = 0;

    return pldm_pdr_init(
        PLDM_PDR_TYPE_NUM_SENS,
        pdr_gen_record_change(),
        pdr_data,
        pdr_data_len
    );
}


bool dht11_humidity_sens_read(
    pldm_pdr_data_size_t size,
    pldm_numeric_sensor_value_t* value,
    void* args
)
{
    if(size != PLDM_PDR_DATA_SIZE_UNSIGNED_INT8)
    {
        return false;
    }

    dht11_request_data(DHT11_PIN);

    sleep_ms(300);

    dht11_data_t* sensor_data = dht11_sensor_data();
    
    value->uint8_value = sensor_data->integral_RH;

    return true;
}


bool dht11_temperature_sens_read(
    pldm_pdr_data_size_t size,
    pldm_numeric_sensor_value_t* value,
    void* args
)
{
    if(size != PLDM_PDR_DATA_SIZE_UNSIGNED_INT8)
    {
        return false;
    }

    dht11_request_data(DHT11_PIN);

    sleep_ms(300);

    dht11_data_t* sensor_data = dht11_sensor_data();
    
    value->uint8_value = sensor_data->integral_T;

    return true;
}


pldm_pdr_header_t* pdr_create_redfish_resource(
    uint32_t res_id,
    uint32_t containing_res_id,
    bool is_collection,
    bool is_entry,
    uint8_t prop_containing_res_name_data[],
    uint16_t prop_containing_res_name_data_len,
    uint8_t sub_uri_data[],
    uint16_t sub_uri_data_len,
    uint8_t schema_name_data[],
    uint16_t schema_name_data_len,
    uint32_t schema_version,
    uint16_t schema_dict_len,
    uint32_t schema_dict_sig
)
{
    uint16_t pdr_data_len = sizeof(pldm_pdr_base_redfish_resource_t) + prop_containing_res_name_data_len;
    pdr_data_len += sizeof(utf8_str_t) + sub_uri_data_len;
    pdr_data_len += sizeof(uint16_t);
    pdr_data_len += sizeof(pldm_pdr_redfish_res_major_t) + schema_name_data_len;
    pdr_data_len += sizeof(uint16_t);

    uint8_t pdr_data[pdr_data_len];
    memset(pdr_data, 0, pdr_data_len);

    pldm_pdr_base_redfish_resource_t* redfish_res = (pldm_pdr_base_redfish_resource_t*)&pdr_data[0];
    redfish_res->resource_id = res_id;
    redfish_res->is_collection_entry = is_entry;
    redfish_res->is_redfish_collection = is_collection;
    redfish_res->containing_resource_id = containing_res_id;

    utf8_str_t* prop_containing_resource = (utf8_str_t*)(redfish_res + 1);
    prop_containing_resource->len = prop_containing_res_name_data_len;
    memcpy(
        prop_containing_resource->data, 
        prop_containing_res_name_data,
        prop_containing_resource->len
    );

    utf8_str_t* sub_uri = (utf8_str_t*)(prop_containing_resource->data + prop_containing_resource->len);
    sub_uri->len = sub_uri_data_len;
    memcpy(
        sub_uri->data, 
        sub_uri_data,
        sub_uri->len
    );

    uint16_t* add_res_count_data = (uint16_t*)(sub_uri->data + sub_uri->len);
    uint16_t add_res_count = 0;
    memcpy(add_res_count_data, &add_res_count, sizeof(uint16_t));

    pldm_pdr_redfish_res_major_t* major_schema = (pldm_pdr_redfish_res_major_t*)(add_res_count_data + 1);
    major_schema->version.version = schema_version;
    major_schema->dict_len = schema_dict_len;
    major_schema->dict_sig = schema_dict_sig;

    utf8_str_t* schema_name = &major_schema->name;
    schema_name->len = schema_name_data_len;
    memcpy(
        schema_name->data, 
        schema_name_data,
        schema_name->len
    );

    uint16_t* oem_count_data = (uint16_t*)(schema_name->data + schema_name->len);
    uint16_t oem_count = 0;
    memcpy(oem_count_data, &oem_count, sizeof(uint16_t));

    return  pldm_pdr_init(
        PLDM_PDR_TYPE_REDFISH_RESOURCE,
        pdr_gen_record_change(),
        pdr_data,
        pdr_data_len
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

    pldm_pdr_repo_init();
    pldm_rde_provider_init();

    mctp_register_bus(mctp_inst, core_binding);
    mctp_serial_set_raw_tx_callback(serial_binding, mctp_uart_raw_tx_callback, NULL);
    mctp_set_ctrl_message_rx_callback(mctp_inst, mctp_ctrl_message_rx_callback, NULL);    
    mctp_set_pldm_message_rx_callback(mctp_inst, mctp_pldm_message_rx_callback, mctp_transport);

    pldm_terminus_register(pldm_inst, core_transport);
    pldm_mctp_set_message_tx_callback(mctp_transport, pldm_mctp_message_tx_callback, core_binding);    
    pldm_set_base_message_rx_callback(pldm_inst, pldm_base_message_rx_callback, NULL);
    pldm_set_platform_message_rx_callback(pldm_inst, pldm_platform_message_rx_callback, NULL);
    pldm_set_redfish_message_rx_callback(pldm_inst, pldm_redfish_message_rx_callback, NULL);


    pldm_set_max_message_len(core_transport, PLDM_MAX_MESSAGE_LEN);

    {
        pldm_pdr_entry_t* mctp_terminus_locator = pldm_pdr_repo_add_entry(
            pdr_create_mctp_terminus_locator(
                core_transport,
                core_binding
            )
        );

        mctp_set_bus_eid_changed_callback(core_binding, mctp_bus_eid_changed_callback, mctp_terminus_locator);
        pldm_terminus_set_id_changed_callback(core_transport, pldm_tid_changed_callback, mctp_terminus_locator);
    }

    {        
        pldm_pdr_entry_t* dht11_hum_sens_entry = pldm_pdr_repo_add_entry(
            pdr_create_dht11_humidity_sens()
        );

        pldm_pdr_header_t* dht11_hum_sens_record = pldm_pdr_entry_get_record(dht11_hum_sens_entry);

        pldm_pdr_base_numeric_sensor_t* dht11_hum_sens_accessor = (pldm_pdr_base_numeric_sensor_t*)(pldm_pdr_get_data(dht11_hum_sens_record));

        pldm_pdr_numeric_sensor_ref_t* dht11_hum_sens_ref = pldm_pdr_repo_get_numeric_sensor(dht11_hum_sens_accessor->base_accessor.accessor_id);

        pldm_pdr_numeric_sensor_ref_set_read_callback(
            dht11_hum_sens_ref,
            dht11_humidity_sens_read,
            NULL
        );
    }

    {
        pldm_pdr_entry_t* dht11_temp_sens_entry = pldm_pdr_repo_add_entry(
            pdr_create_dht11_temperature_sens()
        );

        pldm_pdr_header_t* dht11_temp_sens_record = pldm_pdr_entry_get_record(dht11_temp_sens_entry);

        pldm_pdr_base_numeric_sensor_t* dht11_temp_sens_accessor = (pldm_pdr_base_numeric_sensor_t*)(pldm_pdr_get_data(dht11_temp_sens_record));

        pldm_pdr_numeric_sensor_ref_t* dht11_temp_sens_ref = pldm_pdr_repo_get_numeric_sensor(dht11_temp_sens_accessor->base_accessor.accessor_id);

        pldm_pdr_numeric_sensor_ref_set_read_callback(
            dht11_temp_sens_ref,
            dht11_temperature_sens_read,
            NULL
        );
    }

    {
        pldm_rde_schema_t* schema_chassis = pldm_rde_schema_init(
            "Chassis",
            schema_uri_chassis_v1,
            sizeof(schema_uri_chassis_v1) / sizeof(schema_uri_chassis_v1[0]),
            (bej_dict_header_t*)dict_data_chassis_v1
        );

        pldm_rde_schema_t* schema_sensor_collection = pldm_rde_schema_init(
            "SensorCollection",
            schema_uri_sensor_collection_v1,
            sizeof(schema_uri_sensor_collection_v1) / sizeof(schema_uri_sensor_collection_v1[0]),
            (bej_dict_header_t*)dict_data_sensor_collection_v1
        );

        pldm_rde_schema_t* schema_sensor = pldm_rde_schema_init(
            "Sensor",
            schema_uri_sensor_v1,
            sizeof(schema_uri_sensor_v1) / sizeof(schema_uri_sensor_v1[0]),
            (bej_dict_header_t*)dict_data_sensor_v1
        );


        {
            uint8_t prop_containing_res_name[] = "ChassisCollection.ChassisCollection";
            uint8_t sub_uri[] = "";
            char* schema_name = pldm_rde_schema_get_name(schema_chassis);
            bej_dict_header_t* dict = pldm_rde_schema_get_dict(schema_chassis);

            pldm_pdr_entry_t* pdr_repo_entry = pldm_pdr_repo_add_entry(
                pdr_create_redfish_resource(
                    1, PLDM_PDR_RES_ID_EXTERNAL, false, true,
                    prop_containing_res_name, sizeof(prop_containing_res_name),
                    sub_uri, sizeof(sub_uri),
                    (uint8_t*)schema_name, strlen(schema_name) + 1,
                    dict->schema_version,
                    dict->dict_size,
                    pldm_rde_schema_get_dict_sig(schema_chassis)
                )
            );

            pldm_rde_provider_add_resource(
                picow_chassis_data,
                sizeof(picow_chassis_data),
                pldm_pdr_entry_get_record(pdr_repo_entry),
                schema_chassis
            );
        }

        {
            uint8_t prop_containing_res_name[] = "";
            uint8_t sub_uri[] = "Sensors";
            char* schema_name = pldm_rde_schema_get_name(schema_sensor_collection);
            bej_dict_header_t* dict = pldm_rde_schema_get_dict(schema_sensor_collection);

            pldm_pdr_entry_t* pdr_repo_entry = pldm_pdr_repo_add_entry(
                pdr_create_redfish_resource(
                    2, 1, true, false,
                    prop_containing_res_name, sizeof(prop_containing_res_name),
                    sub_uri, sizeof(sub_uri),
                    (uint8_t*)schema_name, strlen(schema_name) + 1,
                    dict->schema_version,
                    dict->dict_size,
                    pldm_rde_schema_get_dict_sig(schema_sensor_collection)
                )
            );

            pldm_rde_provider_add_resource(
                picow_sensor_collection_data,
                sizeof(picow_sensor_collection_data),
                pldm_pdr_entry_get_record(pdr_repo_entry),
                schema_sensor_collection
            );

        }

        {
            uint8_t prop_containing_res_name[] = "";
            uint8_t sub_uri[] = "DHT11_RH";
            char* schema_name = pldm_rde_schema_get_name(schema_sensor);
            bej_dict_header_t* dict = pldm_rde_schema_get_dict(schema_sensor);

            pldm_pdr_entry_t* pdr_repo_entry = pldm_pdr_repo_add_entry(
                pdr_create_redfish_resource(
                    3, 2, false, true,
                    prop_containing_res_name, sizeof(prop_containing_res_name),
                    sub_uri, sizeof(sub_uri),
                    (uint8_t*)schema_name, strlen(schema_name) + 1,
                    dict->schema_version,
                    dict->dict_size,
                    pldm_rde_schema_get_dict_sig(schema_sensor)
                )
            );

            pldm_rde_provider_add_resource(
                dht11_rh_data,
                sizeof(dht11_rh_data),
                pldm_pdr_entry_get_record(pdr_repo_entry),
                schema_sensor
            );
        }

        {
            uint8_t prop_containing_res_name[] = "";
            uint8_t sub_uri[] = "DHT11_Temperature";
            char* schema_name = pldm_rde_schema_get_name(schema_sensor);
            bej_dict_header_t* dict = pldm_rde_schema_get_dict(schema_sensor);

            pldm_pdr_entry_t* pdr_repo_entry = pldm_pdr_repo_add_entry(
                pdr_create_redfish_resource(
                    4, 2, false, true,
                    prop_containing_res_name, sizeof(prop_containing_res_name),
                    sub_uri, sizeof(sub_uri),
                    (uint8_t*)schema_name, strlen(schema_name) + 1,
                    dict->schema_version,
                    dict->dict_size,
                    pldm_rde_schema_get_dict_sig(schema_sensor)
                )
            );

            pldm_rde_provider_add_resource(
                dht11_temperature_data,
                sizeof(dht11_temperature_data),
                pldm_pdr_entry_get_record(pdr_repo_entry),
                schema_sensor
            );
        }
    }

    while (true)
    {
        if(uart_is_readable(uart_id))
        {
            mctp_serial_byte_rx(serial_binding, uart_getc(uart_id));
        }
    }

    pldm_rde_provider_deinit();
    pldm_pdr_repo_deinit();

    pldm_mctp_destroy(mctp_transport);
    pldm_destroy(pldm_inst);

    mctp_serial_destroy(serial_binding);
    mctp_destroy(mctp_inst);
    
    return 0;
}