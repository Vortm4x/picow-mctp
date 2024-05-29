#ifndef PLDM_RDE_OPERATION
#define PLDM_RDE_OPERATION

#include <pldm/types.h>
#include <pldm/rde/resource.h>

#define PLDM_RDE_OPER_ID_NULL 0x0000


struct pldm_rde_operation_t;
typedef struct pldm_rde_operation_t pldm_rde_operation_t;


pldm_rde_oper_id_t pldm_rde_operation_get_id(
    pldm_rde_operation_t* operation
);

pldm_rde_oper_status_t pldm_rde_operation_get_status(
    pldm_rde_operation_t* operation
);

void pldm_rde_operation_set_status(
    pldm_rde_operation_t* operation,
    pldm_rde_oper_status_t status
);

pldm_rde_oper_type_t pldm_rde_operation_get_type(
    pldm_rde_operation_t* operation
);

pldm_rde_resource_t* pldm_rde_operation_get_resource(
    pldm_rde_operation_t* operation
);

pldm_multipart_t* pldm_rde_operation_get_multipart(
    pldm_rde_operation_t* operation
);

pldm_rde_operation_t* pldm_rde_operation_get_next(
    pldm_rde_operation_t* operation
);

/*
pldm_multipart_outcomming_t* pldm_rde_operation_get_outcomming(
    pldm_rde_operation_t* operation
);

pldm_rde_operation_t* pldm_rde_operation_get_next(
    pldm_rde_operation_t* operation
);
*/

#endif // PLDM_RDE_OPERATION