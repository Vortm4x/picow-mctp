#include <pldm/pdr/entry.h>
#include <pldm/pdr/pdr.h>
#include <private_pdr.h>
#include <stdlib.h>
#include <string.h>


typedef struct pldm_pdr_entry_t 
{
    pldm_pdr_header_t* record;
    pldm_pdr_entry_t* next;
} 
pldm_pdr_entry_t;


pldm_pdr_entry_t* pldm_pdr_entry_init(
    pldm_pdr_header_t* record
)
{
    if (record == NULL)
    {
        return NULL;
    }

    pldm_pdr_entry_t* entry = malloc(sizeof(pldm_pdr_entry_t));

    if(entry != NULL)
    {
        memset(entry, 0, sizeof(pldm_pdr_entry_t));

        entry->record = record;
    }

    return entry;
}


void pldm_pdr_entry_destroy(
    pldm_pdr_entry_t* entry
)
{
    free(entry);
}


pldm_pdr_entry_t* pldm_pdr_entry_get_next(
    pldm_pdr_entry_t* entry
)
{
    if(entry == NULL)
    {
        return NULL;
    }

    return entry->next;
}


pldm_pdr_header_t* pldm_pdr_entry_get_record(
    pldm_pdr_entry_t* entry
)
{
    if(entry == NULL)
    {
        return NULL;
    }

    return entry->record;   
}


uint32_t pldm_pdr_entry_get_record_handle(
    pldm_pdr_entry_t* entry
)
{
    if(entry == NULL)
    {
        return 0;
    }

    return pldm_pdr_get_record_handle(entry->record);
}


pldm_pdr_type_t pldm_pdr_entry_get_type(
    pldm_pdr_entry_t* entry
)
{
    if(entry == NULL)
    {
        return 0;
    }

    return pldm_pdr_get_type(entry->record);
}


uint16_t pldm_pdr_entry_get_size(
    pldm_pdr_entry_t* entry
)
{
    if(entry == NULL)
    {
        return 0;
    }

    return pldm_pdr_get_size(entry->record);
}


void pldm_pdr_entry_set_next(
    pldm_pdr_entry_t* entry,
    pldm_pdr_entry_t* next
)
{
    if(entry == NULL)
    {
        return;
    }

    entry->next = next;
}