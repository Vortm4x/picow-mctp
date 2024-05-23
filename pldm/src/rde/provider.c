#include <pldm/rde/provider.h>
#include <private_rde_resource.h>
#include <pldm/rde/operation.h>
#include <private_rde_operation.h>
#include <pldm/util.h>
#include <stdlib.h>
#include <string.h>


typedef struct pldm_rde_provider_t
{
    pldm_rde_resource_t* first_resource;
    pldm_rde_resource_t* last_resource;
    pldm_rde_operation_t* first_operation;
    pldm_rde_operation_t* last_operation;
    pldm_rde_oper_id_t next_oper_id;
    uint16_t oper_count;
}
pldm_rde_provider_t;

pldm_rde_provider_t* rde_provider = NULL;


void pldm_rde_provider_init()
{
    if(pldm_rde_provider_is_init())
    {
        return;
    }

    rde_provider = malloc(sizeof(pldm_rde_provider_t));

    if(rde_provider != NULL)
    {
        memset(rde_provider, 0, sizeof(pldm_rde_provider_t));
    }    
}


void pldm_rde_provider_deinit()
{
    if(!pldm_rde_provider_is_init())
    {
        return;
    }

    free(rde_provider);

    rde_provider = NULL;
}


bool pldm_rde_provider_is_init()
{
    return (rde_provider != NULL);
}

uint32_t pldm_rde_provider_get_signature()
{
    uint32_t crc = 0;

    if(!pldm_rde_provider_is_init())
    {
        return 0;
    }


    pldm_rde_resource_t* curr = rde_provider->first_resource;
    
    while (curr != NULL)
    {
        pldm_pdr_header_t* pdr = pldm_rde_resource_get_pdr(curr);

        crc = crc32_calc(crc, (uint8_t*)pdr, pldm_pdr_get_size(pdr));

        curr = pldm_rde_resource_get_next(curr);
    }

    
    curr = rde_provider->first_resource;

    while (curr != NULL)
    {
        uint32_t dict_sig = pldm_rde_schema_get_dict_sig(
            pldm_rde_resource_get_schema(curr)
        );
    
        pldm_rde_resource_t* dict_match = rde_provider->first_resource;
        uint32_t dict_match_sig = 0;

        while (dict_match != curr)
        {
            dict_match_sig = pldm_rde_schema_get_dict_sig(
                pldm_rde_resource_get_schema(dict_match)
            );
            
            if(dict_match_sig == dict_sig)
            {
                break;
            }

            dict_match = pldm_rde_resource_get_next(dict_match);
        }

        if(dict_sig != dict_match_sig)
        {
            bej_dict_header_t* dict = pldm_rde_schema_get_dict(
                pldm_rde_resource_get_schema(curr)
            );

            crc = crc32_calc(crc, (uint8_t*)dict, dict->dict_size);
        }

        curr = pldm_rde_resource_get_next(curr);
    }

    return crc;
}


pldm_rde_resource_t* pldm_rde_provider_add_resource(
    uint8_t encoding_data[],
    size_t encoding_data_size,
    pldm_pdr_header_t* pdr,
    pldm_rde_schema_t* schema
)
{
    if(!pldm_rde_provider_is_init())
    {
        return NULL;
    }

    pldm_rde_resource_t* new_resource = pldm_rde_resource_init(
        encoding_data, 
        encoding_data_size, 
        pdr, 
        schema
    );

    if(new_resource == NULL)
    {
        return NULL;
    }

    if(rde_provider->first_resource == NULL)
    {
        rde_provider->first_resource = new_resource;
    }
    else
    {
        pldm_rde_resource_set_next(rde_provider->last_resource, new_resource);
    }

    rde_provider->last_resource = new_resource;

    return new_resource;
}


void pldm_rde_provider_remove_resource(
    uint32_t resource_id
)
{
    if(!pldm_rde_provider_is_init())
    {
        return;
    }
   
    pldm_rde_resource_t* prev = NULL;
    pldm_rde_resource_t* curr = rde_provider->first_resource;

    while(curr != NULL)
    {
        if(pldm_rde_resource_get_id(curr) == resource_id)
        {
            break;
        }

        prev = curr;
        curr = pldm_rde_resource_get_next(curr);
    }

    if(curr == NULL)
    {
        return;
    }

    if(prev != NULL)
    {
        pldm_rde_resource_set_next(
            prev,
            pldm_rde_resource_get_next(curr)
        );
    }
    else
    {
        rde_provider->first_resource = pldm_rde_resource_get_next(curr);
    }

    if(curr == rde_provider->last_resource)
    {
        rde_provider->last_resource = prev;
    }

    pldm_rde_resource_destroy(curr);
}


pldm_rde_resource_t* pldm_rde_provider_get_resource(
    uint32_t resource_id
)
{
    if(!pldm_rde_provider_is_init())
    {
        return NULL;
    }

    pldm_rde_resource_t* curr = rde_provider->first_resource;

    while(curr != NULL)
    {
        if(pldm_rde_resource_get_id(curr) == resource_id)
        {
            return curr;
        }

        curr = pldm_rde_resource_get_next(curr);
    }

    return NULL;
}


pldm_rde_operation_t* pldm_rde_provider_add_operation(
    pldm_rde_resource_t* resource,
    pldm_rde_oper_type_t type,
    pldm_multipart_outcomming_t* outcomming
)
{
    if(!pldm_rde_provider_is_init())
    {
        return NULL;
    }

    pldm_rde_operation_t* new_operation = pldm_rde_operation_init(
        resource,
        type,
        (++rde_provider->next_oper_id),
        outcomming
    );

    if(new_operation == NULL)
    {
        return NULL;
    }

    if(rde_provider->first_operation == NULL)
    {
        rde_provider->first_operation = new_operation;
    }
    else
    {
        pldm_rde_operation_set_next(rde_provider->last_operation, new_operation);
    }

    rde_provider->last_operation = new_operation;
    rde_provider->oper_count++;

    return new_operation;
}


void pldm_rde_provider_remove_operation(
    pldm_rde_oper_id_t operation_id
)
{
    if(!pldm_rde_provider_is_init())
    {
        return;
    }
   
    pldm_rde_operation_t* prev = NULL;
    pldm_rde_operation_t* curr = rde_provider->first_operation;

    while(curr != NULL)
    {
        if(pldm_rde_operation_get_id(curr) == operation_id)
        {
            break;
        }

        prev = curr;
        curr = pldm_rde_operation_get_next(curr);
    }

    if(curr == NULL)
    {
        return;
    }

    if(prev != NULL)
    {
        pldm_rde_operation_set_next(
            prev,
            pldm_rde_operation_get_next(curr)
        );
    }
    else
    {
        rde_provider->first_operation = pldm_rde_operation_get_next(curr);
    }

    if(curr == rde_provider->last_operation)
    {
        rde_provider->last_operation = prev;
    }

    rde_provider->oper_count--;

    pldm_rde_operation_destroy(curr);
}


pldm_rde_operation_t* pldm_rde_provider_get_operation(
    pldm_rde_oper_id_t operation_id
)
{
    if(!pldm_rde_provider_is_init())
    {
        return NULL;
    }

    pldm_rde_operation_t* curr = rde_provider->first_operation;

    while(curr != NULL)
    {
        if(pldm_rde_operation_get_id(curr) == operation_id)
        {
            return curr;
        }

        curr = pldm_rde_operation_get_next(curr);
    }

    return NULL;
}


pldm_rde_operation_t* pldm_rde_provider_get_first_operation()
{
    if(!pldm_rde_provider_is_init())
    {
        return NULL;
    }

    return rde_provider->first_operation;
}


uint16_t pldm_rde_provider_get_operation_count()
{
    if(!pldm_rde_provider_is_init())
    {
        return 0;
    }

    return rde_provider->oper_count;
}