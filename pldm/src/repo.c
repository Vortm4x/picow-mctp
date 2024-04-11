#include <pldm/pdr/repo.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


typedef struct pldm_pdr_repo_entry_t
{
    pldm_pdr_header_t* record;
    pldm_pdr_repo_entry_t* next;
}
pldm_pdr_repo_entry_t;

typedef struct pldm_pdr_repo_t
{
    uint32_t next_handle;
    pldm_pdr_repo_entry_t* entry_head;
    uint32_t record_count;
} 
pldm_pdr_repo_t;


static pldm_pdr_repo_entry_t* pldm_pdr_repo_create_entry(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_header_t* record
);

static void pldm_pdr_repo_desroy_entry(
    pldm_pdr_repo_entry_t* entry
);




pldm_pdr_repo_t* pldm_pdr_repo_init()
{
    pldm_pdr_repo_t* pdr_repo = malloc(sizeof(pldm_pdr_repo_t));

    if(pdr_repo != NULL)
    {
        memset(pdr_repo, 0, sizeof(pldm_pdr_repo_t));

        pdr_repo->next_handle = 1;
    }

    return pdr_repo;
}


void pldm_pdr_repo_destroy(
    pldm_pdr_repo_t* pdr_repo
)
{
    pldm_pdr_repo_entry_t* curr = pdr_repo->entry_head;

    while(curr != NULL)
    {
        pldm_pdr_repo_entry_t* temp = curr;        
        curr = curr->next;

        pldm_pdr_repo_desroy_entry(temp);
    }

    free(pdr_repo);
}


uint32_t pldm_pdr_repo_add_entry(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_header_t* record
)
{
    pldm_pdr_repo_entry_t* prev = NULL;
    pldm_pdr_repo_entry_t* curr = pdr_repo->entry_head;

    while(curr != NULL)
    {
        prev = curr;
        curr = curr->next;
    }

    pldm_pdr_repo_entry_t* new_entry = pldm_pdr_repo_create_entry(
        pdr_repo,
        record    
    );

    if(prev != NULL)
    {
        prev->next = new_entry;
    }
    else
    {
        pdr_repo->entry_head = new_entry;
    }

    pdr_repo->record_count++;
    pdr_repo->next_handle++;

    return record->record_handle;
}


pldm_pdr_header_t* pldm_pdr_repo_get_entry(
    pldm_pdr_repo_t* pdr_repo,
    uint32_t record_handle
)
{
    pldm_pdr_repo_entry_t* curr = pdr_repo->entry_head;

    while(curr != NULL)
    {
        if(curr->record->record_handle == record_handle)
        {
            return curr->record;
        }

        curr = curr->next;
    }

    return NULL;
}


bool pldm_pdr_repo_remove_entry(
    pldm_pdr_repo_t* pdr_repo,
    uint32_t record_handle
)
{
    pldm_pdr_repo_entry_t* prev = NULL;
    pldm_pdr_repo_entry_t* curr = pdr_repo->entry_head;

    while(curr != NULL)
    {
        if(curr->record->record_handle == record_handle)
        {
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL)
    {
        return false;
    }

    if(prev != NULL)
    {
        prev->next = curr->next;
    }
    else
    {
        pdr_repo->entry_head = curr->next;
    }

    pldm_pdr_repo_desroy_entry(curr);

    pdr_repo->record_count--;

    return true;
}


pldm_pdr_repo_handle_t* pldm_pdr_repo_create_handle(
    pldm_pdr_repo_t* pdr_repo,
    uint32_t record_handle
)
{
    pldm_pdr_repo_handle_t* repo_handle = malloc(sizeof(pldm_pdr_repo_handle_t));

    if(repo_handle != NULL)
    {
        memset(repo_handle, 0, sizeof(pldm_pdr_repo_handle_t));

        repo_handle->pdr_repo = pdr_repo;
        repo_handle->record_handle = record_handle;
    }

    return repo_handle;
}


void pldm_pdr_repo_desroy_handle(
    pldm_pdr_repo_handle_t* record_handle
)
{
    free(record_handle);
}


pldm_pdr_repo_entry_t* pldm_pdr_repo_create_entry(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_header_t* record
)
{
    pldm_pdr_repo_entry_t* entry = malloc(sizeof(pldm_pdr_repo_entry_t));

    if(entry != NULL)
    {
        memset(entry, 0, sizeof(pldm_pdr_repo_entry_t));

        entry->record = record;
        record->record_handle = pdr_repo->next_handle;
    }

    return entry;
}


void pldm_pdr_repo_desroy_entry(
    pldm_pdr_repo_entry_t* entry
)
{
    pldm_pdr_destroy_record(entry->record);
    free(entry);
}