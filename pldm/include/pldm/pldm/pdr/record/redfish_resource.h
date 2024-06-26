#ifndef PLDM_PDR_REDFISH_RESOURCE_H
#define PLDM_PDR_REDFISH_RESOURCE_H

#include <pldm/pldm.h>
#include <pldm/pdr/pdr.h>

#define PLDM_PDR_RES_ID_EXTERNAL 0x00000000

typedef struct __attribute__ ((__packed__)) pldm_pdr_redfish_add_res_t
{
    uint32_t res_id;
    utf8_str_t sub_uri;
}
pldm_pdr_redfish_add_res_t;

typedef struct __attribute__ ((__packed__)) pldm_pdr_redfish_res_major_t
{
    ver32_t version;
    uint16_t dict_len;
    uint32_t dict_sig;
    utf8_str_t name;
}
pldm_pdr_redfish_res_major_t;

typedef struct __attribute__ ((__packed__)) pldm_pdr_base_redfish_resource_t
{
    uint32_t resource_id;
    bool is_device_root         : 1;
    bool is_collection_entry    : 1;
    bool is_redfish_collection  : 1;
    uint8_t                     : 5;
    uint32_t containing_resource_id;
}
pldm_pdr_base_redfish_resource_t;


pldm_pdr_redfish_res_major_t* pldm_pdr_redfish_resource_major_schema(
    pldm_pdr_base_redfish_resource_t* pdr
);

#endif // PLDM_PDR_REDFISH_RESOURCE_H