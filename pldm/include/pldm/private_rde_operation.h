#ifndef PLDM_PRIVATE_RDE_OPERATION
#define PLDM_PRIVATE_RDE_OPERATION

#include <pldm/rde/operation.h>

pldm_rde_operation_t* pldm_rde_operation_init(
    pldm_rde_resource_t* resource,
    pldm_rde_oper_type_t type,
    pldm_rde_oper_id_t id,
    pldm_multipart_t* multipart
);

void pldm_rde_operation_destroy(
    pldm_rde_operation_t* operation
);

void pldm_rde_operation_set_next(
    pldm_rde_operation_t* operation,
    pldm_rde_operation_t* next
);

#endif // PLDM_PRIVATE_RDE_OPERATION