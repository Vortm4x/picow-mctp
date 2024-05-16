#ifndef PLDM_RDE_SCHEMA
#define PLDM_RDE_SCHEMA

#include <pldm/types.h>
#include <pldm/rde/bej.h>

struct pldm_rde_schema_t;
typedef struct pldm_rde_schema_t pldm_rde_schema_t;


pldm_rde_schema_t* pldm_rde_schema_init(
    char* name,
    char** uri_frags,
    uint16_t uri_frags_count,
    bej_dict_header_t* dict
);

void pldm_rde_schema_destroy(
    pldm_rde_schema_t* schema
);

char* pldm_rde_schema_get_name(
    pldm_rde_schema_t* schema
);

uint16_t pldm_rde_schema_get_uri_frags_count(
    pldm_rde_schema_t* schema
);

char** pldm_rde_schema_get_uri_frags(
    pldm_rde_schema_t* schema
);

bej_dict_header_t* pldm_rde_schema_get_dict(
    pldm_rde_schema_t* schema
);

uint32_t pldm_rde_schema_get_dict_sig(
    pldm_rde_schema_t* schema
);



#endif // PLDM_RDE_SCHEMA