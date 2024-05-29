#include <pldm/pdr/repo.h>
#include <pldm/pdr/pdr.h>
#include <pldm/pdr/record/numeric_sensor.h>
#include <pldm/util.h>
#include <private_pdr.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


typedef struct pldm_pdr_repo_t
{
    pldm_pdr_entry_t* first_entry;
    pldm_pdr_entry_t* last_entry;
    pldm_pdr_numeric_sensor_ref_t* first_numeric_sensor_ref;
    pldm_pdr_numeric_sensor_ref_t* last_numeric_sensor_ref;
    uint32_t record_count;
    uint32_t repo_size;
    uint32_t largest_record_size;
    uint32_t next_record_handle;
    uint32_t next_sensor_id;
}
pldm_pdr_repo_t;

pldm_pdr_repo_t* pdr_repo = NULL;


static void pldm_pdr_repo_post_add(
    pldm_pdr_entry_t* entry
);

static void pldm_pdr_repo_post_remove(
    pldm_pdr_entry_t* entry
);


void pldm_pdr_repo_init()
{
    if(pldm_pdr_repo_is_init())
    {
        return;
    }

    pdr_repo = malloc(sizeof(pldm_pdr_repo_t));

    if(pdr_repo != NULL)
    {
        memset(pdr_repo, 0, sizeof(pldm_pdr_repo_t));
    }    
}


void pldm_pdr_repo_deinit()
{
    if(!pldm_pdr_repo_is_init())
    {
        return;
    }

    pldm_pdr_entry_t* curr = pdr_repo->first_entry;

    while(curr != NULL)
    {
        pldm_pdr_entry_t* temp = curr;        
        curr = pldm_pdr_entry_get_next(curr);

        pldm_pdr_entry_destroy(temp);
    }

    free(pdr_repo);
    pdr_repo = NULL;
}


bool pldm_pdr_repo_is_init()
{
    return (pdr_repo != NULL);
}


uint32_t pldm_pdr_next_record_handle()
{
    if(!pldm_pdr_repo_is_init())
    {
        return 0;
    }

    return (++pdr_repo->next_record_handle);
}


uint32_t pldm_pdr_next_sensor_id()
{
    if(!pldm_pdr_repo_is_init())
    {
        return 0;
    }

    return (++pdr_repo->next_sensor_id);
}


uint32_t pldm_pdr_repo_get_record_count()
{
    if(!pldm_pdr_repo_is_init())
    {
        return 0;
    }

    return pdr_repo->record_count;
}


uint32_t pldm_pdr_repo_get_largest_record_size()
{
    if(!pldm_pdr_repo_is_init())
    {
        return 0;
    }

    return pdr_repo->largest_record_size;
}


uint32_t pldm_pdr_repo_get_signature()
{
    uint32_t crc = 0;

    if(!pldm_pdr_repo_is_init())
    {
        return crc;
    }
    
    pldm_pdr_entry_t* curr = pdr_repo->first_entry;

    while (curr != NULL)
    {
        pldm_pdr_header_t* record = pldm_pdr_entry_get_record(curr);

        crc = crc32_calc(crc, (uint8_t*)record, pldm_pdr_entry_get_size(curr));

        curr = pldm_pdr_entry_get_next(curr);
    }
    
    return crc;
}


uint32_t pldm_pdr_repo_get_size()
{
    if(!pldm_pdr_repo_is_init())
    {
        return 0;
    }

    return pdr_repo->repo_size;
}


pldm_pdr_entry_t* pldm_pdr_repo_add_entry(
    pldm_pdr_header_t* record
)
{
    if(!pldm_pdr_repo_is_init())
    {
        return NULL;
    }

    pldm_pdr_entry_t* new_entry = pldm_pdr_entry_init(record);

    if(pdr_repo->first_entry == NULL)
    {
        pdr_repo->first_entry = new_entry;
    }
    else
    {
        pldm_pdr_entry_set_next(pdr_repo->last_entry, new_entry);
    }

    pdr_repo->last_entry = new_entry;


    if(pldm_pdr_entry_get_size(new_entry) > pdr_repo->largest_record_size)
    {
        pdr_repo->largest_record_size = pldm_pdr_entry_get_size(new_entry);
    }

    pdr_repo->record_count++;
    pdr_repo->repo_size += pldm_pdr_entry_get_size(new_entry);

    pldm_pdr_repo_post_add(new_entry);

    return new_entry;
}


bool pldm_pdr_repo_remove_entry(
    uint32_t record_handle
)
{
    if(!pldm_pdr_repo_is_init())
    {
        return false;
    }

    pldm_pdr_entry_t* prev = NULL;
    pldm_pdr_entry_t* curr = pdr_repo->first_entry;
    uint32_t largest_record_size = 0;

    while(curr != NULL)
    {
        if(pldm_pdr_entry_get_record_handle(curr) == record_handle)
        {
            break;
        }

        if(pldm_pdr_entry_get_size(curr) > largest_record_size)
        {
            largest_record_size = pldm_pdr_entry_get_size(curr);
        }

        prev = curr;
        curr = pldm_pdr_entry_get_next(curr);
    }

    if(curr == NULL)
    {
        return false;
    }

    pdr_repo->record_count--;
    pdr_repo->repo_size -= pldm_pdr_entry_get_size(curr);

    if(prev != NULL)
    {
        pldm_pdr_entry_set_next(prev, pldm_pdr_entry_get_next(curr));
    }
    else
    {
        pdr_repo->first_entry = pldm_pdr_entry_get_next(curr);
    }

    if(curr == pdr_repo->last_entry)
    {
        pdr_repo->last_entry = prev;
    }

    pldm_pdr_repo_post_remove(curr);

    pldm_pdr_destroy(pldm_pdr_entry_get_record(curr));
    pldm_pdr_entry_destroy(curr);

    curr = pldm_pdr_entry_get_next(prev);

    while(curr != NULL)
    {
        if(pldm_pdr_entry_get_size(curr) > largest_record_size)
        {
            largest_record_size = pldm_pdr_entry_get_size(curr);
        }

        curr = pldm_pdr_entry_get_next(curr);
    }

    pdr_repo->largest_record_size = largest_record_size;

    return true;
}


pldm_pdr_entry_t* pldm_pdr_repo_get_entry(
    uint32_t record_handle
)
{
    if(!pldm_pdr_repo_is_init())
    {
        return NULL;
    }

    if(record_handle == PLDM_PDR_NULL_RECORD_HANDLE)
    {
        return pdr_repo->first_entry;
    }

    pldm_pdr_entry_t* curr = pdr_repo->first_entry;

    while(curr != NULL)
    {
        if(pldm_pdr_entry_get_record_handle(curr) == record_handle)
        {
            return curr;
        }

        curr = pldm_pdr_entry_get_next(curr);
    }

    return NULL;
}


pldm_pdr_numeric_sensor_ref_t* pldm_pdr_repo_get_numeric_sensor(
    uint16_t sensor_id
)
{
    if(!pldm_pdr_repo_is_init())
    {
        return NULL;
    }

    pldm_pdr_numeric_sensor_ref_t* curr = pdr_repo->first_numeric_sensor_ref;

    while(curr != NULL)
    {
        if(pldm_pdr_numeric_sensor_ref_get_id(curr) == sensor_id)
        {
            return curr;
        }

        curr = pldm_pdr_numeric_sensor_ref_get_next(curr);
    }

    return NULL;
}


pldm_pdr_entry_t* pldm_pdr_repo_get_entry_by_type(
    pldm_pdr_type_t pdr_type
)
{
    if(!pldm_pdr_repo_is_init())
    {
        return NULL;
    }

    pldm_pdr_entry_t* curr = pdr_repo->first_entry;
    pldm_pdr_entry_t* head = NULL;
    pldm_pdr_entry_t* tail = NULL;

    while(curr != NULL)
    {
        if(pldm_pdr_entry_get_type(curr) == pdr_type)
        {
            pldm_pdr_entry_t* entry = pldm_pdr_entry_init(
                pldm_pdr_entry_get_record(curr)
            );

            if(head != NULL)
            {
                pldm_pdr_entry_set_next(tail, entry);
            }
            else
            {
                head = entry;
            }

            tail = entry;
        }

        curr = pldm_pdr_entry_get_next(curr);
    }

    return head;
}


void pldm_pdr_repo_post_add(
    pldm_pdr_entry_t* entry
)
{
    switch (pldm_pdr_entry_get_type(entry))
    {
        case PLDM_PDR_TYPE_NUM_SENS:
        {
            pldm_pdr_numeric_sensor_ref_t* new_numeric_sensor = pldm_pdr_numeric_sensor_ref_init(entry);

            if(pdr_repo->first_numeric_sensor_ref == NULL)
            {
                pdr_repo->first_numeric_sensor_ref = new_numeric_sensor;
            }
            else
            {
                pldm_pdr_numeric_sensor_ref_set_next(pdr_repo->last_numeric_sensor_ref, new_numeric_sensor);
            }

            pdr_repo->last_numeric_sensor_ref = new_numeric_sensor;
        }
        break;

        default:
        {}
        break;
    }
}


void pldm_pdr_repo_post_remove(
    pldm_pdr_entry_t* entry
)
{
    switch (pldm_pdr_entry_get_type(entry))
    {
        case PLDM_PDR_TYPE_NUM_SENS:
        {
            uint32_t record_handle = pldm_pdr_entry_get_record_handle(entry);

            pldm_pdr_numeric_sensor_ref_t* prev = NULL;
            pldm_pdr_numeric_sensor_ref_t* curr = pdr_repo->first_numeric_sensor_ref;

            while(curr != NULL)
            {
                pldm_pdr_entry_t* curr_entry = pldm_pdr_numeric_sensor_ref_get_entry(curr);

                if(pldm_pdr_entry_get_record_handle(curr_entry) == record_handle)
                {
                    break;
                }

                prev = curr;
                curr = pldm_pdr_numeric_sensor_ref_get_next(curr);
            }

            if(curr == NULL)
            {
                return;
            }

            if(prev != NULL)
            {
                pldm_pdr_numeric_sensor_ref_set_next(prev, pldm_pdr_numeric_sensor_ref_get_next(curr));
            }
            else
            {
                pdr_repo->first_numeric_sensor_ref = pldm_pdr_numeric_sensor_ref_get_next(curr);
            }

            if(curr == pdr_repo->last_numeric_sensor_ref)
            {
                pdr_repo->last_numeric_sensor_ref = prev;
            }

            pldm_pdr_numeric_sensor_ref_destroy(curr);
        }
        break;

        default:
        {}
        break;
    }
}
