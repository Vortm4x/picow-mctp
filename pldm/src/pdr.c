#include <pldm/pdr/pdr.h>
#include <pldm/pdr/num_sens.h>
#include <pldm/pdr/redfish_resource.h>
#include <stdlib.h>
#include <string.h>


pldm_pdr_header_t* pldm_pdr_create_record(
    pldm_pdr_type_t pdr_type,
    uint16_t record_change,
    uint8_t data[],
    uint16_t data_len
)
{
    size_t record_size = sizeof(pldm_pdr_header_t) + data_len;
    pldm_pdr_header_t* new_record = malloc(record_size);

    if(new_record != NULL)
    {
        memset(new_record, 0, record_size);

        new_record->version = PLDM_PDR_HEADER_VER;
        new_record->pdr_type = pdr_type;
        new_record->record_change = record_change;
        new_record->data_len = data_len;

        memcpy(new_record->data, data, data_len);
    }

    return new_record;
}


void pldm_pdr_destroy_record(
    pldm_pdr_header_t* record
)
{
    free(record);
}


uint32_t pldm_pdr_record_len(
    pldm_pdr_header_t* record
)
{
    if(record == NULL)
    {
        return 0;
    }

    return (sizeof(pldm_pdr_header_t) + record->data_len);
}


uint32_t pldm_pdr_data_type_size(
    pldm_pdr_data_size_t data_size
)
{
    switch (data_size)
    {
        case PLDM_PDR_DATA_SIZE_UNSIGNED_INT8:
            return sizeof(uint8_t);
        break;

        case PLDM_PDR_DATA_SIZE_SIGNED_INT8:
            return sizeof(int8_t);
        break;

        case PLDM_PDR_DATA_SIZE_UNSIGNED_INT16:
            return sizeof(uint16_t);
        break;

        case PLDM_PDR_DATA_SIZE_SIGNED_INT16:
            return sizeof(int16_t);
        break;

        case PLDM_PDR_DATA_SIZE_UNSIGNED_INT32:
            return sizeof(uint16_t);
        break;

        case PLDM_PDR_DATA_SIZE_SIGNED_INT32:
            return sizeof(int16_t);
        break;

        case PLDM_PDR_DATA_SIZE_REAL32:
            return sizeof(real32_t);
        break;
    
        default:
            return 0;
        break;
    }
}


utf8_str_t* pldm_pdr_redfish_res_name_ptr(
    pldm_pdr_redfish_res_t* pdr
)
{
    return (utf8_str_t*)(pdr + 1);
}

utf8_str_t* pldm_pdr_redfish_res_sub_uri_ptr(
    pldm_pdr_redfish_res_t* pdr
)
{
    utf8_str_t* res_name = pldm_pdr_redfish_res_name_ptr(pdr);
    return (utf8_str_t*)((uint8_t*)(res_name + 1) + res_name->len);
}

uint16_t pldm_pdr_redfish_res_add_res_count(
    pldm_pdr_redfish_res_t* pdr
)
{
    utf8_str_t* sub_uri = pldm_pdr_redfish_res_sub_uri_ptr(pdr);
    return *(uint16_t*)((uint8_t*)(sub_uri + 1) + sub_uri->len);
}

pldm_pdr_redfish_add_res_t* pldm_pdr_redfish_res_add_res_first(
    pldm_pdr_redfish_res_t* pdr
)
{
    utf8_str_t* sub_uri = pldm_pdr_redfish_res_sub_uri_ptr(pdr);
    return (pldm_pdr_redfish_add_res_t*)((uint8_t*)(sub_uri + 1) + sub_uri->len + sizeof(uint16_t));
}

pldm_pdr_redfish_add_res_t* pldm_pdr_redfish_res_add_res_next(
    pldm_pdr_redfish_add_res_t* add_res
)
{
    return (pldm_pdr_redfish_add_res_t*)((uint8_t*)(add_res + 1) + add_res->sub_uri.len);
}

uint16_t pldm_pdr_redfish_res_oem_name_count(
    pldm_pdr_redfish_res_major_t* major_schema
)
{
    return *(uint16_t*)((uint8_t*)(major_schema + 1) + major_schema->name.len);
}

utf8_str_t* pldm_pdr_redfish_res_oem_name_first(
    pldm_pdr_redfish_res_major_t* major_schema
)
{
    return (utf8_str_t*)((uint8_t*)(major_schema + 1) + major_schema->name.len + sizeof(uint16_t));
}

utf8_str_t* pldm_pdr_redfish_res_oem_name_next(
    utf8_str_t* oem_name
)
{
    return (utf8_str_t*)((uint8_t*)(oem_name + 1) + oem_name->len);
}