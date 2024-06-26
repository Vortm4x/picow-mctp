#include <pldm/rde/resource.h>
#include <private_rde_resource.h>
#include <pldm/rde/bej.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct pldm_rde_resource_t
{
    bej_encoding_t* encoding;
    pldm_pdr_header_t* pdr; 
    pldm_rde_schema_t* schema;
    pldm_rde_resource_t* next;
}
pldm_rde_resource_t;


pldm_rde_resource_t* pldm_rde_resource_init(
    uint8_t encoding_data[],
    size_t encoding_data_size,
    pldm_pdr_header_t* pdr,
    pldm_rde_schema_t* schema
)
{
    if(encoding_data == NULL 
    || pdr == NULL
    || schema == NULL)
    {
        return NULL;
    }

    pldm_rde_resource_t* resource = malloc(sizeof(pldm_rde_resource_t));

    if(resource != NULL)
    {
        memset(resource, 0, sizeof(pldm_rde_resource_t));

        resource->encoding = bej_encoding_unpack(
            encoding_data,
            encoding_data_size,
            (uint8_t*)pldm_rde_schema_get_dict(schema)
        );
        resource->pdr = pdr;
        resource->schema = schema;
    }

    return resource;
}


void pldm_rde_resource_destroy(
    pldm_rde_resource_t* resource
)
{
    free(resource);
}


bej_encoding_t* pldm_rde_resource_get_encoding(
    pldm_rde_resource_t* resource
)
{
    if(resource == NULL)
    {
        return NULL;
    }

    return resource->encoding;
}

pldm_pdr_header_t* pldm_rde_resource_get_pdr(
    pldm_rde_resource_t* resource
)
{
    if(resource == NULL)
    {
        return NULL;
    }

    return resource->pdr;
}


uint32_t pldm_rde_resource_get_id(
    pldm_rde_resource_t* resource
)
{
    if(resource == NULL)
    {
        return 0;
    }

    pldm_pdr_base_redfish_resource_t* record_data = (pldm_pdr_base_redfish_resource_t*)pldm_pdr_get_data(resource->pdr);

    return record_data->resource_id;
}


pldm_rde_resource_t* pldm_rde_resource_get_next(
    pldm_rde_resource_t* resource
)
{
    if(resource == NULL)
    {
        return NULL;
    }

    return resource->next;
}


void pldm_rde_resource_set_next(
    pldm_rde_resource_t* resource,
    pldm_rde_resource_t* next
)
{
    if(resource == NULL)
    {
        return;
    }

    resource->next = next;
}


void pldm_rde_resource_get_etag(
    pldm_rde_resource_t* resource,
    pldm_rde_resource_etag_t* etag
)
{
    if(resource == NULL)
    {
        return;
    }

    memset(etag, 0, sizeof(pldm_rde_resource_etag_t));

    snprintf(
        (char*)etag->val, 
        sizeof(pldm_rde_resource_etag_t), 
        "\"%08lX\"",
        pldm_rde_resource_get_id(resource)
    );
}


pldm_rde_schema_t* pldm_rde_resource_get_schema(
    pldm_rde_resource_t* resource
)
{
    if(resource == NULL)
    {
        return NULL;
    }

    return resource->schema;
}