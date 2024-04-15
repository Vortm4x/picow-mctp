#include <pldm/pdr/repo.h>
#include <pldm/pdr/num_sens.h>
#include <pldm/util.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


typedef struct pldm_pdr_repo_entry_t
{
    pldm_pdr_repo_t* pdr_repo;
    pldm_pdr_header_t* record;
    pldm_pdr_repo_entry_t* next;
    uint32_t xfer_handle;
    uint32_t xfer_offset;
}
pldm_pdr_repo_entry_t;

typedef struct pldm_pdr_repo_sens_ref_t
{
    uint16_t sensor_id;
    pldm_pdr_repo_entry_t* entry;
    pldm_pdr_repo_sens_ref_t* next;
    pldm_pdr_sensor_read_t sensor_read_callback;
}
pldm_pdr_repo_sens_ref_t;

typedef struct pldm_pdr_repo_t
{
    uint32_t next_record_handle;
    uint32_t next_xfer_handle;
    uint32_t next_sensor_id;
    pldm_pdr_repo_entry_t* first_entry;
    pldm_pdr_repo_sens_ref_t* first_sens_ref;

    uint32_t record_count;
    uint32_t repo_size;
    uint32_t largest_record_size;
} 
pldm_pdr_repo_t;


static pldm_pdr_repo_entry_t* pldm_pdr_repo_entry_init(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_header_t* record
);

static void pldm_pdr_repo_entry_desroy(
    pldm_pdr_repo_entry_t* entry
);

static pldm_pdr_repo_sens_ref_t* pldm_pdr_repo_sens_ref_init(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_repo_entry_t* entry
);

static void pldm_pdr_repo_sens_ref_destroy(
    pldm_pdr_repo_sens_ref_t* sens_ref
);

static void pldm_pdr_repo_add_sens_ref(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_repo_entry_t* entry
);

static void pldm_pdr_repo_remove_sens_ref(
    pldm_pdr_repo_t* pdr_repo,
    uint16_t sensor_id
);


pldm_pdr_repo_t* pldm_pdr_repo_init()
{
    pldm_pdr_repo_t* pdr_repo = malloc(sizeof(pldm_pdr_repo_t));

    if(pdr_repo != NULL)
    {
        memset(pdr_repo, 0, sizeof(pldm_pdr_repo_t));

        pdr_repo->next_record_handle = 1;
        pdr_repo->next_xfer_handle = 1;
        pdr_repo->next_sensor_id = 1;
    }

    return pdr_repo;
}

void pldm_pdr_repo_destroy(
    pldm_pdr_repo_t* pdr_repo
)
{
    pldm_pdr_repo_entry_t* curr = pdr_repo->first_entry;

    while(curr != NULL)
    {
        pldm_pdr_repo_entry_t* temp = curr;        
        curr = curr->next;

        pldm_pdr_repo_entry_desroy(temp);
    }

    free(pdr_repo);
}


pldm_pdr_repo_entry_t* pldm_pdr_repo_add_entry(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_header_t* record
)
{
    pldm_pdr_repo_entry_t* prev = NULL;
    pldm_pdr_repo_entry_t* curr = pdr_repo->first_entry;

    while(curr != NULL)
    {
        prev = curr;
        curr = curr->next;
    }

    pldm_pdr_repo_entry_t* new_entry = pldm_pdr_repo_entry_init(
        pdr_repo,
        record
    );

    if(prev != NULL)
    {
        prev->next = new_entry;
    }
    else
    {
        pdr_repo->first_entry = new_entry;
    }

    if(record->pdr_type == PLDM_PDR_TYPE_NUM_SENS)
    {
        pldm_pdr_repo_add_sens_ref(
            pdr_repo,
            new_entry
        );
    }

    if(pldm_pdr_record_len(record) > pdr_repo->largest_record_size)
    {
        pdr_repo->largest_record_size = pldm_pdr_record_len(record);
    }

    pdr_repo->record_count++;
    pdr_repo->repo_size += pldm_pdr_record_len(record);

    return new_entry;
}


pldm_pdr_repo_entry_t* pldm_pdr_repo_get_entry(
    pldm_pdr_repo_t* pdr_repo,
    uint32_t record_handle
)
{
    if(record_handle == PLDM_PDR_NULL_HANDLE)
    {
        return pdr_repo->first_entry;
    }

    pldm_pdr_repo_entry_t* curr = pdr_repo->first_entry;

    while(curr != NULL)
    {
        if(curr->record->record_handle == record_handle)
        {
            return curr;
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
    pldm_pdr_repo_entry_t* curr = pdr_repo->first_entry;
    uint32_t largest_record_size = 0;

    while(curr != NULL)
    {
        if(curr->record->record_handle == record_handle)
        {
            break;
        }

        if(pldm_pdr_record_len(curr->record) > largest_record_size)
        {
            largest_record_size = pldm_pdr_record_len(curr->record);
        }

        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL)
    {
        return false;
    }

    pdr_repo->record_count--;
    pdr_repo->repo_size -= pldm_pdr_record_len(curr->record);

    if(prev != NULL)
    {
        prev->next = curr->next;
    }
    else
    {
        pdr_repo->first_entry = curr->next;
    }

    pldm_pdr_header_t* record = curr->record;

    if (record->pdr_type == PLDM_PDR_TYPE_NUM_SENS)
    {
        pldm_pdr_num_sens_t* num_sens = (pldm_pdr_num_sens_t*)record->data;

        uint16_t sensor_id = num_sens->sensor_id; 

        pldm_pdr_repo_remove_sens_ref(
            pdr_repo,
            sensor_id
        );
    }

    pldm_pdr_repo_entry_desroy(curr);

    curr = prev->next;

    while(curr != NULL)
    {
        if(pldm_pdr_record_len(curr->record) > largest_record_size)
        {
            largest_record_size = pldm_pdr_record_len(curr->record);
        }

        curr = curr->next;
    }

    pdr_repo->largest_record_size = largest_record_size;

    return true;
}


uint32_t pldm_pdr_repo_total_size(
    pldm_pdr_repo_t* pdr_repo
)
{
    if(pdr_repo == NULL)
    {
        return 0;
    }

    return pdr_repo->repo_size;
}


uint32_t pldm_pdr_repo_largest_record_size(
    pldm_pdr_repo_t* pdr_repo
)
{
    if(pdr_repo == NULL)
    {
        return 0;
    }

    return pdr_repo->largest_record_size;
}

uint32_t pldm_pdr_repo_signature(
    pldm_pdr_repo_t* pdr_repo
)
{
    uint32_t crc = CRC32_INIT;
    
    pldm_pdr_repo_entry_t* curr = pdr_repo->first_entry;

    while (curr != NULL)
    {
        pldm_pdr_header_t* record = curr->record;

        crc = crc32_calc_block(crc, (uint8_t*)record, pldm_pdr_record_len(record));

        curr = curr->next;
    }
    
    return crc;
}


pldm_pdr_repo_entry_t* pldm_pdr_repo_entry_init(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_header_t* record
)
{
    pldm_pdr_repo_entry_t* entry = malloc(sizeof(pldm_pdr_repo_entry_t));

    if(entry != NULL)
    {
        memset(entry, 0, sizeof(pldm_pdr_repo_entry_t));

        entry->pdr_repo = pdr_repo;
        entry->record = record;
        
        record->record_handle = pdr_repo->next_record_handle++;
    }

    return entry;
}


void pldm_pdr_repo_entry_desroy(
    pldm_pdr_repo_entry_t* entry
)
{
    pldm_pdr_destroy_record(entry->record);
    free(entry);
}


pldm_pdr_header_t* pldm_pdr_repo_entry_get_record(
    pldm_pdr_repo_entry_t* repo_entry
)
{
    if(repo_entry == NULL)
    {
        return NULL;
    }

    return repo_entry->record;
}

uint32_t pldm_pdr_repo_entry_get_xfer_handle(
    pldm_pdr_repo_entry_t* repo_entry
)
{
    if(repo_entry == NULL)
    {
        return 0;
    }

    return repo_entry->xfer_handle;
}


uint32_t pldm_pdr_repo_entry_get_xfer_offset(
    pldm_pdr_repo_entry_t* repo_entry
)
{
    if(repo_entry == NULL)
    {
        return 0;
    }

    return repo_entry->xfer_offset;
}


pldm_pdr_repo_entry_t* pldm_pdr_repo_entry_get_next_entry(
    pldm_pdr_repo_entry_t* repo_entry
)
{
    if(repo_entry == NULL)
    {
        return NULL;
    }

    return repo_entry->next;
}


pldm_xfer_pos_t pldm_pdr_repo_entry_xfer_upd(
    pldm_pdr_repo_entry_t* repo_entry,
    uint32_t data_sent
)
{
    pldm_xfer_pos_t xfer_pos = {};

    if(repo_entry != NULL)
    {
        xfer_pos.is_start = repo_entry->xfer_offset == 0;

        repo_entry->xfer_offset += data_sent;

        xfer_pos.is_end = repo_entry->xfer_offset == pldm_pdr_record_len(repo_entry->record);

        xfer_pos.is_middle = !(xfer_pos.is_start || xfer_pos.is_end);

        if(xfer_pos.is_end)
        {
            pldm_pdr_repo_entry_xfer_reset(repo_entry);
        }
        else
        {
            repo_entry->xfer_handle = repo_entry->pdr_repo->next_xfer_handle++;
        }
    }

    return xfer_pos;
}


void pldm_pdr_repo_entry_xfer_reset(
    pldm_pdr_repo_entry_t* repo_entry
)
{
    if(repo_entry == NULL)
    {
        return;
    }

    repo_entry->xfer_handle = 0;
    repo_entry->xfer_offset = 0;
}


pldm_pdr_repo_sens_ref_t* pldm_pdr_repo_get_sensor(
    pldm_pdr_repo_t* pdr_repo,
    uint16_t sensor_id
)
{
    if(pdr_repo == NULL)
    {
        return NULL;
    }

    pldm_pdr_repo_sens_ref_t* curr = pdr_repo->first_sens_ref;

    while(curr != NULL)
    {
        if(curr->sensor_id == sensor_id)
        {
            return curr;
        }

        curr = curr->next;
    }

    return NULL;
}


pldm_pdr_repo_sens_ref_t* pldm_pdr_repo_sens_ref_init(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_repo_entry_t* entry
)
{
    pldm_pdr_repo_sens_ref_t* sens_ref = malloc(sizeof(pldm_pdr_repo_sens_ref_t));

    if(entry != NULL)
    {
        memset(sens_ref, 0, sizeof(pldm_pdr_repo_sens_ref_t));

        sens_ref->entry = entry;

        pldm_pdr_header_t* record = entry->record;

        if (record->pdr_type == PLDM_PDR_TYPE_NUM_SENS)
        {
            pldm_pdr_num_sens_t* num_sens = (pldm_pdr_num_sens_t*)record->data;

            num_sens->sensor_id = pdr_repo->next_sensor_id++;
            sens_ref->sensor_id = num_sens->sensor_id;     
        }
    }

    return sens_ref;
}


void pldm_pdr_repo_sens_ref_destroy(
    pldm_pdr_repo_sens_ref_t* sens_ref
)
{
    free(sens_ref);
}


void pldm_pdr_repo_add_sens_ref(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_repo_entry_t* entry
)
{
    pldm_pdr_repo_sens_ref_t* prev = NULL;
    pldm_pdr_repo_sens_ref_t* curr = pdr_repo->first_sens_ref;

    while(curr != NULL)
    {
        prev = curr;
        curr = curr->next;
    }

    pldm_pdr_repo_sens_ref_t* new_sens_ref = pldm_pdr_repo_sens_ref_init(
        pdr_repo,
        entry
    );

    if(prev != NULL)
    {
        prev->next = new_sens_ref;
    }
    else
    {
        pdr_repo->first_sens_ref = new_sens_ref;
    }
}


void pldm_pdr_repo_remove_sens_ref(
    pldm_pdr_repo_t* pdr_repo,
    uint16_t sensor_id
)
{
    pldm_pdr_repo_sens_ref_t* prev = NULL;
    pldm_pdr_repo_sens_ref_t* curr = pdr_repo->first_sens_ref;

    while(curr != NULL)
    {
        if(curr->sensor_id == sensor_id)
        {
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL)
    {
        return;
    }

    if(prev != NULL)
    {
        prev->next = curr->next;
    }
    else
    {
        pdr_repo->first_sens_ref = curr->next;
    }

    pldm_pdr_repo_sens_ref_destroy(curr);

    curr = prev->next;
}


void pldm_pdr_repo_sensor_read(
    pldm_pdr_repo_sens_ref_t* sens_ref,
    uint8_t data[],
    size_t data_len
)
{
    if(sens_ref->sensor_read_callback != NULL)
    {
        sens_ref->sensor_read_callback(data, data_len);
    }
}


pldm_pdr_repo_entry_t* pldm_pdr_repo_sensor_get_entry(
    pldm_pdr_repo_sens_ref_t* sens_ref
)
{
    if(sens_ref == NULL)
    {
        return NULL;
    }

    return sens_ref->entry;
}


void pldm_pdr_repo_sensor_set_read_callback(
    pldm_pdr_repo_sens_ref_t* sens_ref,
    pldm_pdr_sensor_read_t sensor_read_callback
)
{
    if(sens_ref != NULL)
    {
        sens_ref->sensor_read_callback = sensor_read_callback;
    }
}
