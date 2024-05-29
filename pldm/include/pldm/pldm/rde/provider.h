#ifndef PLDM_RDE_PROVIDER
#define PLDM_RDE_PROVIDER

#include <pldm/types.h>
#include <pldm/rde/resource.h>
#include <pldm/rde/operation.h>


void pldm_rde_provider_init();

void pldm_rde_provider_deinit();

bool pldm_rde_provider_is_init();

uint32_t pldm_rde_provider_get_signature();

pldm_rde_resource_t* pldm_rde_provider_add_resource(
    uint8_t encoding_data[],
    size_t encoding_data_size,
    pldm_pdr_header_t* pdr,
    pldm_rde_schema_t* schema
);

void pldm_rde_provider_remove_resource(
    uint32_t resource_id
);

pldm_rde_resource_t* pldm_rde_provider_get_resource(
    uint32_t resource_id
);

pldm_rde_operation_t* pldm_rde_provider_add_operation(
    pldm_rde_resource_t* resource,
    pldm_rde_oper_id_t oper_id,
    pldm_rde_oper_type_t type,
    pldm_multipart_t* multipart
);

void pldm_rde_provider_remove_operation(
    pldm_rde_oper_id_t operation_id
);

pldm_rde_operation_t* pldm_rde_provider_get_operation(
    pldm_rde_oper_id_t operation_id
);

pldm_rde_operation_t* pldm_rde_provider_get_first_operation();

uint16_t pldm_rde_provider_get_operation_count();


#endif // PLDM_RDE_PROVIDER