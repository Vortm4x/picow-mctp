#include <pldm/pldm.h>
#include <private_core.h>
#include <private_terminus.h>
#include <private_multipart.h>
#include <stdlib.h>
#include <string.h>


void pldm_terminus_register(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport
)
{
    pldm_terminus_t* terminus = malloc(sizeof(pldm_terminus_t));

    if(terminus != NULL)
    {
        memset(terminus, 0, sizeof(pldm_terminus_t));

        terminus->tid = PLDM_TID_UNASSIGNED;
        terminus->transport = transport;
        terminus->pldm_inst = pldm_inst;

        pldm_inst->endpoint = terminus;

        transport->endpoint = terminus;
    }
}


void pldm_terminus_unregister(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport   
)
{
    free(pldm_inst->endpoint);
    pldm_inst->endpoint = NULL;
    transport->endpoint = NULL;
}


pldm_tid_t pldm_terminus_get_id(
    pldm_transport_t* transport   
)
{
    if(transport == NULL)
    {
        return PLDM_TID_UNASSIGNED;
    }

    if(transport->endpoint == NULL)
    {
        return PLDM_TID_UNASSIGNED;
    }

    return transport->endpoint->tid;
}


void pldm_terminus_set_id(
    pldm_transport_t* transport,
    pldm_tid_t tid
)
{
    if(transport != NULL)
    {
        pldm_terminus_t* endpoint = transport->endpoint;

        if(endpoint != NULL)
        {
            bool tid_changed = (endpoint->tid != tid);

            endpoint->tid = tid;

            if(tid_changed)
            {
                endpoint->pldm_tid_changed_callback(
                    transport,
                    endpoint->pldm_tid_changed_args
                );
            }
        }
    }
}


void pldm_terminus_set_id_changed_callback(
    pldm_transport_t* transport,
    pldm_tid_changed_t pldm_tid_changed_callback,
    void* pldm_tid_changed_args
)
{
    pldm_terminus_t* endpoint = transport->endpoint;

    if(endpoint != NULL)
    {
        endpoint->pldm_tid_changed_callback = pldm_tid_changed_callback;
        endpoint->pldm_tid_changed_args = pldm_tid_changed_args;
    }
}


uint32_t pldm_terminus_next_xfer_handle(
    pldm_terminus_t* endpoint
)
{
    return ++endpoint->next_xfer_handle;
}


pldm_multipart_t*  pldm_terminus_add_multipart_outcomming(
    pldm_transport_t* transport,
    uint8_t data[],
    uint16_t data_len
)
{
    if(transport == NULL)
    {
        return NULL;
    }

    if(transport->endpoint == NULL || data == NULL || data_len == 0)
    {
        return NULL;
    }

    pldm_terminus_t* endpoint = transport->endpoint;

    pldm_multipart_t* new_outcomming = pldm_multipart_outcomming_init(
        endpoint,
        data_len, 
        data
    );

    if(endpoint->multipart_out_first == NULL)
    {
        endpoint->multipart_out_first = new_outcomming;
    }
    else
    {
        pldm_multipart_set_next(endpoint->multipart_out_last, new_outcomming);
    }

    endpoint->multipart_out_last = new_outcomming;

    return new_outcomming;
}


pldm_multipart_t*  pldm_terminus_add_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle
)
{
    if(transport == NULL)
    {
        return NULL;
    }

    if(transport->endpoint == NULL 
    || xfer_handle == PLDM_MULTIPART_NULL_XFER_HANDLE
    || xfer_handle == PLDM_MULTIPART_NOT_FINISHED_XFER_HANDLE)
    {
        return NULL;
    }

    pldm_terminus_t* endpoint = transport->endpoint;

    pldm_multipart_t* new_incomming = pldm_multipart_incomming_init(
        endpoint,
        xfer_handle
    );

    if(endpoint->multipart_in_first == NULL)
    {
        endpoint->multipart_in_first = new_incomming;
    }
    else
    {
        pldm_multipart_set_next(endpoint->multipart_in_last, new_incomming);
    }

    endpoint->multipart_in_last = new_incomming;

    return new_incomming;   
}


pldm_multipart_t* pldm_terminus_get_multipart(
    pldm_transport_t* transport,
    uint32_t xfer_handle,
    bool is_outcomming
)
{
    if(transport == NULL)
    {
        return NULL;
    }

    if(transport->endpoint == NULL)
    {
        return NULL;
    }

    pldm_terminus_t* endpoint = transport->endpoint;
   
    pldm_multipart_t* curr = NULL;

    if(is_outcomming)
    {
        curr = endpoint->multipart_out_first;
    }
    else
    {
        curr = endpoint->multipart_in_first;
    }


    while(curr != NULL)
    {
        if(pldm_multipart_get_xfer_handle(curr) == xfer_handle)
        {
            return curr;
        }

        curr = pldm_multipart_get_next(curr);
    }

    return NULL;
}


void pldm_terminus_remove_multipart(
    pldm_transport_t* transport,
    uint32_t xfer_handle,
    bool is_outcomming
)
{
    if(transport == NULL)
    {
        return;
    }

    if(transport->endpoint == NULL)
    {
        return;
    }

    pldm_terminus_t* endpoint = transport->endpoint;
   
    pldm_multipart_t* prev = NULL;
    pldm_multipart_t* curr = NULL;
    

    if(is_outcomming)
    {
        curr = endpoint->multipart_out_first;
    }
    else
    {
        curr = endpoint->multipart_in_first;
    }

    while(curr != NULL)
    {
        if(pldm_multipart_get_xfer_handle(curr) == xfer_handle)
        {
            break;
        }

        prev = curr;
        curr = pldm_multipart_get_next(curr);
    }

    if(curr == NULL)
    {
        return;
    }

    if(prev != NULL)
    {
        pldm_multipart_set_next(
            prev,
            pldm_multipart_get_next(curr)
        );
    }
    else
    {
        if(is_outcomming)
        {
            endpoint->multipart_out_first = pldm_multipart_get_next(curr);
        }
        else
        {
            endpoint->multipart_in_first = pldm_multipart_get_next(curr);
        }
    }

    pldm_multipart_t** last_ref = NULL;

    if(is_outcomming)
    {
        last_ref = &endpoint->multipart_out_last;
    }
    else
    {
        last_ref = &endpoint->multipart_in_last;
    }

    if(curr == *last_ref)
    {
        *last_ref = prev;
    }
    
    pldm_multipart_destroy(curr);
}


/*
pldm_multipart_outcomming_t*  pldm_terminus_add_multipart_outcomming(
    pldm_transport_t* transport,
    uint8_t data[],
    uint16_t data_len
)
{
    if(transport == NULL)
    {
        return NULL;
    }

    if(transport->endpoint == NULL || data == NULL || data_len == 0)
    {
        return NULL;
    }

    pldm_terminus_t* endpoint = transport->endpoint;

    pldm_multipart_outcomming_t* new_outcomming = pldm_multipart_outcomming_init(
        endpoint,
        data_len, 
        data
    );

    if(endpoint->multipart_out_first == NULL)
    {
        endpoint->multipart_out_first = new_outcomming;
    }
    else
    {
        pldm_multipart_outcomming_set_next(endpoint->multipart_out_last, new_outcomming);
    }

    endpoint->multipart_out_last = new_outcomming;

    return new_outcomming;
}


void pldm_terminus_remove_multipart_outcomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
)
{
    if(transport == NULL)
    {
        return;
    }

    if(transport->endpoint == NULL)
    {
        return;
    }

    pldm_terminus_t* endpoint = transport->endpoint;
   
    pldm_multipart_outcomming_t* prev = NULL;
    pldm_multipart_outcomming_t* curr = endpoint->multipart_out_first;

    while(curr != NULL)
    {
        if(pldm_multipart_outcomming_get_xfer_handle(curr) == xfer_handle)
        {
            break;
        }

        prev = curr;
        curr = pldm_multipart_outcomming_get_next(curr);
    }

    if(curr == NULL)
    {
        return;
    }

    if(prev != NULL)
    {
        pldm_multipart_outcomming_set_next(
            prev,
            pldm_multipart_outcomming_get_next(curr)
        );
    }
    else
    {
        endpoint->multipart_out_first = pldm_multipart_outcomming_get_next(curr);
    }

    if(curr == endpoint->multipart_out_last)
    {
        endpoint->multipart_out_last = prev;
    }

    pldm_multipart_outcomming_destroy(curr);
}


pldm_multipart_outcomming_t* pldm_terminus_get_multipart_outcomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
)
{
    if(transport == NULL)
    {
        return NULL;
    }

    if(transport->endpoint == NULL)
    {
        return NULL;
    }

    pldm_terminus_t* endpoint = transport->endpoint;
   
    pldm_multipart_outcomming_t* curr = endpoint->multipart_out_first;

    while(curr != NULL)
    {
        if(pldm_multipart_outcomming_get_xfer_handle(curr) == xfer_handle)
        {
            return curr;
        }

        curr = pldm_multipart_outcomming_get_next(curr);
    }

    return NULL;
}






pldm_multipart_incomming_t*  pldm_terminus_add_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle
)
{
    if(transport == NULL)
    {
        return NULL;
    }

    if(transport->endpoint == NULL 
    || xfer_handle == PLDM_MULTIPART_NULL_XFER_HANDLE
    || xfer_handle == PLDM_MULTIPART_NOT_FINISHED_XFER_HANDLE)
    {
        return NULL;
    }

    pldm_terminus_t* endpoint = transport->endpoint;

    pldm_multipart_incomming_t* new_incomming = pldm_multipart_incomming_init(
        endpoint,
        xfer_handle
    );

    if(endpoint->multipart_in_first == NULL)
    {
        endpoint->multipart_in_first = new_incomming;
    }
    else
    {
        pldm_multipart_incomming_set_next(endpoint->multipart_in_last, new_incomming);
    }

    endpoint->multipart_in_last = new_incomming;

    return new_incomming;
}


void pldm_terminus_remove_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
)
{
    if(transport == NULL)
    {
        return;
    }

    if(transport->endpoint == NULL)
    {
        return;
    }

    pldm_terminus_t* endpoint = transport->endpoint;
   
    pldm_multipart_incomming_t* prev = NULL;
    pldm_multipart_incomming_t* curr = endpoint->multipart_in_first;

    while(curr != NULL)
    {
        if(pldm_multipart_incomming_get_xfer_handle(curr) == xfer_handle)
        {
            break;
        }

        prev = curr;
        curr = pldm_multipart_incomming_get_next(curr);
    }

    if(curr == NULL)
    {
        return;
    }

    if(prev != NULL)
    {
        pldm_multipart_incomming_set_next(
            prev,
            pldm_multipart_incomming_get_next(curr)
        );
    }
    else
    {
        endpoint->multipart_in_first = pldm_multipart_incomming_get_next(curr);
    }

    if(curr == endpoint->multipart_in_last)
    {
        endpoint->multipart_in_last = prev;
    }

    pldm_multipart_incomming_destroy(curr);
}


pldm_multipart_incomming_t* pldm_terminus_get_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
)
{
    if(transport == NULL)
    {
        return NULL;
    }

    if(transport->endpoint == NULL)
    {
        return NULL;
    }

    pldm_terminus_t* endpoint = transport->endpoint;
   
    pldm_multipart_incomming_t* curr = endpoint->multipart_in_first;

    while(curr != NULL)
    {
        if(pldm_multipart_incomming_get_xfer_handle(curr) == xfer_handle)
        {
            return curr;
        }

        curr = pldm_multipart_incomming_get_next(curr);
    }

    return NULL;
}
*/











/*
pldm_multipart_outcomming_t* pldm_terminus_add_multipart_outcomming(
    pldm_transport_t* transport,
    uint32_t data_len,
    uint8_t* data
)
{
    if(transport == NULL)
    {
        return NULL;
    }

    if(transport->endpoint == NULL || data == NULL || data_len == 0)
    {
        return NULL;
    }

    pldm_terminus_t* endpoint = transport->endpoint;


    pldm_multipart_outcomming_t* outcomming = pldm_multipart_outcomming_init(
        data_len, data
    );

    if(endpoint->multipart_out_first == NULL)
    {
        endpoint->multipart_out_first = outcomming;
    }
    else
    {
        endpoint->multipart_out_last->

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





// pldm_multipart_outcomming_t* pldm_terminus_add_multipart_outcomming(
//     pldm_transport_t* transport,
//     uint32_t data_len,
//     uint8_t* data
// )
// {

// }



// uint32_t pldm_terminus_next_xfer_handle(
//     pldm_transport_t* transport
// )
// {
//     if(transport == NULL)
//     {
//         return 0;
//     }
// }


// void pldm_terminus_multipart_add_outcomming(
//     pldm_transport_t* transport

// )


*/