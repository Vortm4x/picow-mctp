#ifndef PLDM_PRIVATE_RDE_RESOURCE
#define PLDM_PRIVATE_RDE_RESOURCE

#include <pldm/rde/resource.h>


pldm_rde_resource_t* pldm_rde_resource_init(
    bej_encoding_t* encoding,
    pldm_pdr_header_t* pdr,
    pldm_rde_schema_t* schema
);

void pldm_rde_resource_destroy(
    pldm_rde_resource_t* resource
);

pldm_rde_resource_t* pldm_rde_resource_get_next(
    pldm_rde_resource_t* resource
);

void pldm_rde_resource_set_next(
    pldm_rde_resource_t* resource,
    pldm_rde_resource_t* next
);

#endif // PLDM_PRIVATE_RDE_RESOURCE