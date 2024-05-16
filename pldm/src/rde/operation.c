#include <pldm/rde/operation.h>
#include <private_rde_operation.h>
#include <pldm/rde/resource.h>
#include <pldm/multipart.h>
#include <stdlib.h>
#include <string.h>


typedef struct pldm_rde_operation_t
{
    pldm_rde_oper_id_t id;
    pldm_rde_oper_status_t status;
    pldm_rde_oper_type_t type;
    pldm_rde_resource_t* resource;
    pldm_rde_operation_t* next;
    pldm_multipart_outcomming_t* outcomming;
}
pldm_rde_operation_t;


pldm_rde_operation_t* pldm_rde_operation_init(
    pldm_rde_resource_t* resource,
    pldm_rde_oper_type_t type,
    pldm_rde_oper_id_t id,
    pldm_multipart_outcomming_t* outcomming
)
{
    if(resource == NULL)
    {
        return NULL;
    }


    pldm_rde_operation_t* operation = malloc(sizeof(pldm_rde_operation_t));

    if(operation != NULL)
    {
        memset(operation, 0, sizeof(pldm_rde_operation_t));

        operation->resource = resource;
        operation->type = type;
        operation->id = id;
        operation->status = RDE_OP_STATUS_INACTIVE;
        operation->outcomming = outcomming;
    }

    return operation;
}


void pldm_rde_operation_destroy(
    pldm_rde_operation_t* operation
)
{
    free(operation);
}


pldm_rde_oper_id_t pldm_rde_operation_get_id(
    pldm_rde_operation_t* operation
)
{
    if(operation == NULL)
    {
        return 0;
    }

    return operation->id;
}


pldm_rde_oper_status_t pldm_rde_operation_get_status(
    pldm_rde_operation_t* operation
)
{
    if(operation == NULL)
    {
        return RDE_OP_STATUS_INACTIVE;
    }

    return operation->status;
}


void pldm_rde_operation_set_status(
    pldm_rde_operation_t* operation,
    pldm_rde_oper_status_t status
)
{
    if(operation == NULL)
    {
        return;
    }

    operation->status = status;
}


pldm_rde_oper_type_t pldm_rde_operation_get_type(
    pldm_rde_operation_t* operation
)
{
    if(operation == NULL)
    {
        return RDE_OP_TYPE_UNKNOWN;
    }

    return operation->type;
}


pldm_rde_resource_t* pldm_rde_operation_get_resource(
    pldm_rde_operation_t* operation
)
{
    if(operation == NULL)
    {
        return NULL;
    }

    return operation->resource;
}


pldm_multipart_outcomming_t* pldm_rde_operation_get_outcomming(
    pldm_rde_operation_t* operation
)
{
    if(operation == NULL)
    {
        return NULL;
    }

    return operation->outcomming;
}


pldm_rde_operation_t* pldm_rde_operation_get_next(
    pldm_rde_operation_t* operation
)
{
    if(operation == NULL)
    {
        return NULL;
    }

    return operation->next;
}


void pldm_rde_operation_set_next(
    pldm_rde_operation_t* operation,
    pldm_rde_operation_t* next
)
{
    if(operation == NULL)
    {
        return;
    }

    operation->next = next;
}