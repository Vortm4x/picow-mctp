#include <pldm/rde/schema.h>
#include <pldm/util.h>
#include <stdlib.h>
#include <string.h>


typedef struct pldm_rde_schema_t
{
    char* name;
    char** uri_frags;
    uint16_t uri_frags_count;
    bej_dict_header_t* dict;
    uint32_t dict_sig;
}
pldm_rde_schema_t;


pldm_rde_schema_t* pldm_rde_schema_init(
    char* name,
    char** uri_frags,
    uint16_t uri_frags_count,
    bej_dict_header_t* dict
)
{
    if(name == NULL 
    || uri_frags == NULL
    || uri_frags_count == 0
    || dict == NULL)
    {
        return NULL;
    }

    pldm_rde_schema_t* schema = malloc(sizeof(pldm_rde_schema_t));

    if(schema != NULL)
    {
        memset(schema, 0, sizeof(pldm_rde_schema_t));

        schema->name = strdup(name);

        schema->uri_frags = calloc(sizeof(char*), uri_frags_count);

        for (uint16_t i = 0; i < uri_frags_count; ++i)
        {
            schema->uri_frags[i] = strdup(uri_frags[i]);
        }
        
        schema->uri_frags_count = uri_frags_count;

        schema->dict = dict;

        schema->dict_sig = crc32_calc_block(
            CRC32_INIT, 
            (uint8_t*)dict, 
            dict->dict_size
        );
    }

    return schema;
}


void pldm_rde_schema_destroy(
    pldm_rde_schema_t* schema
)
{
    free(schema->name);

    for (uint16_t i = 0; i < schema->uri_frags_count; ++i)
    {
        free(schema->uri_frags[i]);
    }

    free(schema->uri_frags);

    free(schema);
}


char* pldm_rde_schema_get_name(
    pldm_rde_schema_t* schema
)
{
    if(schema == NULL)
    {
        return NULL;
    }

    return schema->name;
}


uint16_t pldm_rde_schema_get_uri_frags_count(
    pldm_rde_schema_t* schema
)
{
    if(schema == NULL)
    {
        return 0;
    }

    return schema->uri_frags_count;
}


char** pldm_rde_schema_get_uri_frags(
    pldm_rde_schema_t* schema
)
{
    if(schema == NULL)
    {
        return NULL;
    }

    return schema->uri_frags;
}


bej_dict_header_t* pldm_rde_schema_get_dict(
    pldm_rde_schema_t* schema
)
{
    if(schema == NULL)
    {
        return NULL;
    }

    return schema->dict;
}


uint32_t pldm_rde_schema_get_dict_sig(
    pldm_rde_schema_t* schema
)
{
    if(schema == NULL)
    {
        return 0;
    }

    return schema->dict_sig;
}