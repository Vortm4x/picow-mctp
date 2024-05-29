#ifndef PLDM_RDE_RESOURCE
#define PLDM_RDE_RESOURCE

#include <pldm/types.h>
#include <pldm/pdr/record/redfish_resource.h>
#include <pldm/rde/bej.h>
#include <pldm/rde/schema.h>

struct pldm_rde_resource_t;
typedef struct pldm_rde_resource_t pldm_rde_resource_t;


#define PLDM_RDE_RESOURCE_ETAG_STR_LEN 8

typedef struct __attribute__ ((__packed__)) pldm_rde_resource_etag_t
{
    uint8_t val[1 + PLDM_RDE_RESOURCE_ETAG_STR_LEN + 1 + 1]; 
}
pldm_rde_resource_etag_t;


bej_encoding_t* pldm_rde_resource_get_encoding(
    pldm_rde_resource_t* resource
);

pldm_pdr_header_t* pldm_rde_resource_get_pdr(
    pldm_rde_resource_t* resource
);

uint32_t pldm_rde_resource_get_id(
    pldm_rde_resource_t* resource
);

void pldm_rde_resource_get_etag(
    pldm_rde_resource_t* resource,
    pldm_rde_resource_etag_t* etag
);

pldm_rde_schema_t* pldm_rde_resource_get_schema(
    pldm_rde_resource_t* resource
);

#endif // PLDM_RDE_RESOURCE