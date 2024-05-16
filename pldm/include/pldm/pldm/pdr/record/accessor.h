#ifndef PLDM_PDR_ACCESSOR_H
#define PLDM_PDR_ACCESSOR_H

#include <pldm/types.h>

typedef enum __attribute__ ((__packed__)) pldm_pdr_accessor_init_t
{
    PLDM_PDR_ACCESOR_INIT_NO_INIT,
    PLDM_PDR_ACCESOR_INIT_USE_INIT_PDR,
    PLDM_PDR_ACCESOR_INIT_ENABLE,
    PLDM_PDR_ACCESOR_INIT_DISABLE,
}
pldm_pdr_accessor_init_t;

typedef struct __attribute__ ((__packed__)) pldm_pdr_base_accessor_t
{
    uint16_t terminus_handle;
    uint16_t accessor_id;
    pldm_entity_info_t entity_info;
}
pldm_pdr_base_accessor_t;


#endif // PLDM_PDR_ACCESSOR_H