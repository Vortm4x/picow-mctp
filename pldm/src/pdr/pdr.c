#include <pldm/types.h>
#include <pldm/pdr/pdr.h>
#include <pldm/pdr/repo.h>
#include <pldm/pdr/entry.h>
#include <pldm/pdr/record/numeric_sensor.h>
#include <pldm/pdr/record/redfish_resource.h>
#include <private_pdr.h>
#include <stdlib.h>
#include <string.h>


typedef struct __attribute__ ((__packed__)) pldm_pdr_header_t
{
    uint32_t record_handle;
    uint8_t version;
    pldm_pdr_type_t pdr_type;
    uint16_t record_change;
    uint16_t record_data_len;
    uint8_t record_data[];
}
pldm_pdr_header_t;


pldm_pdr_header_t* pldm_pdr_init(
    pldm_pdr_type_t pdr_type,
    uint16_t record_change,
    uint8_t record_data[],
    uint16_t record_data_len
)
{
    pldm_pdr_header_t* pdr = malloc(sizeof(pldm_pdr_header_t) + record_data_len);

    if(pdr != NULL)
    {
        memset(pdr, 0, sizeof(pldm_pdr_header_t) + record_data_len);

        pdr->record_handle = pldm_pdr_next_record_handle();
        pdr->version = PLDM_PDR_HEADER_VER;
        pdr->pdr_type = pdr_type;
        pdr->record_change = record_change;
        pdr->record_data_len = record_data_len;

        memcpy(pdr->record_data, record_data, record_data_len);
    }

    return pdr;
}


void pldm_pdr_destroy(
    pldm_pdr_header_t* pdr
)
{
    free(pdr);
}


uint32_t pldm_pdr_get_record_handle(
    pldm_pdr_header_t* pdr
)
{
    if(pdr == NULL)
    {
        return 0;
    }

    return pdr->record_handle;
}


pldm_pdr_type_t pldm_pdr_get_type(
    pldm_pdr_header_t* pdr
)
{
    if(pdr == NULL)
    {
        return 0;
    }

    return pdr->pdr_type;
}


uint16_t pldm_pdr_get_record_change(
    pldm_pdr_header_t* pdr
)
{
    if(pdr == NULL)
    {
        return 0;
    }

    return pdr->record_change;
}


void pldm_pdr_set_record_change(
    pldm_pdr_header_t* pdr,
    uint16_t record_change
)
{
    if(pdr == NULL)
    {
        return;
    }

    pdr->record_change = record_change;
}


uint16_t pldm_pdr_get_data_len(
    pldm_pdr_header_t* pdr
)
{
    if(pdr == NULL)
    {
        return 0;
    }

    return pdr->record_data_len;
}


uint8_t* pldm_pdr_get_data(
    pldm_pdr_header_t* pdr
)
{
    if(pdr == NULL)
    {
        return NULL;
    }

    return pdr->record_data;
}

uint32_t pldm_pdr_get_size(
    pldm_pdr_header_t* pdr
)
{
    return (pldm_pdr_get_data_len(pdr) + sizeof(pldm_pdr_header_t));
}


// PLDM_PDR_TYPE_NUM_SENS

typedef struct pldm_pdr_numeric_sensor_ref_t
{
    pldm_pdr_entry_t* entry;
    pldm_numeric_sensor_read_callback_t read_callback;
    void* read_args;
    pldm_pdr_numeric_sensor_ref_t* next;
}
pldm_pdr_numeric_sensor_ref_t;


pldm_pdr_numeric_sensor_ref_t* pldm_pdr_numeric_sensor_ref_init(
    pldm_pdr_entry_t* entry
)
{
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor = malloc(sizeof(pldm_pdr_numeric_sensor_ref_t));

    if(numeric_sensor != NULL)
    {
        memset(numeric_sensor, 0, sizeof(pldm_pdr_numeric_sensor_ref_t));

        numeric_sensor->entry = entry;
    }

    return numeric_sensor;
}


void pldm_pdr_numeric_sensor_ref_destroy(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor
)
{
    free(numeric_sensor);
}


pldm_pdr_entry_t* pldm_pdr_numeric_sensor_ref_get_entry(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor
)
{
    if(numeric_sensor == NULL)
    {
        return NULL;
    }

    return numeric_sensor->entry;    
}


pldm_pdr_numeric_sensor_ref_t* pldm_pdr_numeric_sensor_ref_get_next(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor
)
{
    if(numeric_sensor == NULL)
    {
        return NULL;
    }

    return numeric_sensor->next;
}


pldm_pdr_base_numeric_sensor_t* pldm_pdr_numeric_sensor_ref_get_base(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor
)
{
    if(numeric_sensor == NULL)
    {
        return NULL;
    }

    pldm_pdr_header_t* record = pldm_pdr_entry_get_record(numeric_sensor->entry);

    return (pldm_pdr_base_numeric_sensor_t*)(record->record_data);
}

uint16_t pldm_pdr_numeric_sensor_ref_get_id(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor
)
{
    if(numeric_sensor == NULL)
    {
        return 0;
    }

    return pldm_pdr_numeric_sensor_ref_get_base(numeric_sensor)->base_accessor.accessor_id;
}


void pldm_pdr_numeric_sensor_ref_set_read_callback(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor,
    pldm_numeric_sensor_read_callback_t read_callback,
    void* read_args
)
{
    numeric_sensor->read_callback = read_callback;
    numeric_sensor->read_args = read_args;
}


bool pldm_pdr_numeric_sensor_ref_read_value(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor,
    pldm_numeric_sensor_value_t* value
)
{
    if(numeric_sensor == NULL)
    {
        return false;
    }

    if(numeric_sensor->read_callback == NULL)
    {
        return false;
    }
    
    pldm_pdr_base_numeric_sensor_t* base_numeric_sensor = pldm_pdr_numeric_sensor_ref_get_base(numeric_sensor);
    pldm_pdr_data_size_t size = *((pldm_pdr_data_size_t*)(base_numeric_sensor + 1));

    return numeric_sensor->read_callback(
        size,
        value,
        numeric_sensor->read_args
    );
}


void pldm_pdr_numeric_sensor_ref_set_next(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor,
    pldm_pdr_numeric_sensor_ref_t* next
)
{
    if(numeric_sensor == NULL)
    {
        return;
    }

    numeric_sensor->next = next;
}


// PLDM_PDR_TYPE_REDFISH_RESOURCE

pldm_pdr_redfish_res_major_t* pldm_pdr_redfish_resource_major_schema(
    pldm_pdr_base_redfish_resource_t* pdr
)
{
    utf8_str_t* prop_containing_resource = (utf8_str_t*)(pdr + 1);
    utf8_str_t* sub_uri = (utf8_str_t*)(prop_containing_resource->data + prop_containing_resource->len);
    
    uint16_t* add_res_count = (uint16_t*)(sub_uri->data + sub_uri->len);
    pldm_pdr_redfish_add_res_t* add_res = (pldm_pdr_redfish_add_res_t*)(add_res_count + 1);

    for(uint16_t i = 0; i < *add_res_count; ++i)
    {
        add_res = (pldm_pdr_redfish_add_res_t*)(add_res->sub_uri.data + add_res->sub_uri.len);
    }

    pldm_pdr_redfish_res_major_t* major = (pldm_pdr_redfish_res_major_t*)add_res;

    return major;  
}
