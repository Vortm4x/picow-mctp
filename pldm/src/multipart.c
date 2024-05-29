#include <pldm/multipart.h>
#include <private_multipart.h>
#include <private_terminus.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct pldm_multipart_t
{
    uint32_t xfer_handle;
    size_t data_len;
    uint8_t* data;
    FILE* data_stream;
    pldm_terminus_t* terminus;
    pldm_multipart_t* next;
    bool is_outcomming;
}
pldm_multipart_t;


pldm_multipart_t* pldm_multipart_outcomming_init(
    pldm_terminus_t* terminus,
    uint32_t data_len,
    uint8_t* data
)
{
    pldm_multipart_t* outcomming = malloc(sizeof(pldm_multipart_t));

    if(outcomming != NULL)
    {
        memset(outcomming, 0, sizeof(pldm_multipart_t));

        outcomming->terminus = terminus;
        outcomming->data_len = data_len;
        outcomming->data = malloc(data_len);
        memcpy(outcomming->data, data, data_len);
        // outcomming->xfer_handle = pldm_terminus_next_xfer_handle(terminus);

        outcomming->data_stream = fmemopen(outcomming->data, data_len, "r");
        outcomming->is_outcomming = true;
    }

    return outcomming;
}


pldm_multipart_t* pldm_multipart_incomming_init(
    pldm_terminus_t* terminus,
    uint32_t xfer_handle
)
{
    pldm_multipart_t* incomming = malloc(sizeof(pldm_multipart_t));

    if(incomming != NULL)
    {
        memset(incomming, 0, sizeof(pldm_multipart_t));

        incomming->terminus = terminus;
        incomming->xfer_handle = xfer_handle;

        incomming->data_stream = open_memstream((char**)&incomming->data, &incomming->data_len);
        incomming->is_outcomming = false;
    }

    return incomming;
}


void pldm_multipart_destroy(
    pldm_multipart_t* multipart
)
{
    if(multipart != NULL)
    {
        free(multipart->data);
        fclose(multipart->data_stream);
    }

    free(multipart);
}


pldm_multipart_t* pldm_multipart_get_next(
    pldm_multipart_t* multipart
)
{
    if(multipart == NULL)
    {
        return NULL;
    }

    return multipart->next;
}


void pldm_multipart_set_next(
    pldm_multipart_t* multipart,
    pldm_multipart_t* next
)
{
    if(multipart == NULL)
    {
        return;
    }

    if(next != NULL)
    {
        if(multipart->is_outcomming != next->is_outcomming)
        {
            return;
        }
    }

    multipart->next = next;
}


uint32_t pldm_multipart_get_xfer_handle(
    pldm_multipart_t* multipart
)
{
    if(multipart == NULL)
    {
        return 0;
    }

    return multipart->xfer_handle;
}


void pldm_multipart_incomming_set_xfer_handle(
    pldm_multipart_t* multipart,
    uint32_t xfer_handle
)
{
    if(multipart == NULL)
    {
        return;
    }

    if(multipart->is_outcomming)
    {
        return;
    }

    multipart->xfer_handle = xfer_handle;
}


uint8_t* pldm_multipart_get_data(
    pldm_multipart_t* multipart
)
{
    if(multipart == NULL)
    {
        return NULL;
    }

    return multipart->data;
}


uint32_t pldm_multipart_get_data_len(
    pldm_multipart_t* multipart
)
{
    if(multipart == NULL)
    {
        return 0;
    }

    return (uint32_t)multipart->data_len;
}


pldm_xfer_pos_t pldm_multipart_outcomming_read(
    pldm_multipart_t* multipart,
    uint8_t* data,
    uint32_t data_len
)
{
    pldm_xfer_pos_t xfer_pos = {};

    if(multipart == NULL)
    {
        return xfer_pos;
    }

    if(!multipart->is_outcomming)
    {
        return xfer_pos;
    }

    long offset = 0;

    offset = ftell(multipart->data_stream);
    xfer_pos.is_start = (offset == 0);

    if(data != NULL)
    {
        fread(data, sizeof(uint8_t), (size_t)data_len, multipart->data_stream);
    }

    offset = ftell(multipart->data_stream);
    xfer_pos.is_end = ((size_t)offset == multipart->data_len);

    xfer_pos.is_middle = !(xfer_pos.is_start || xfer_pos.is_end);

    multipart->xfer_handle = pldm_terminus_next_xfer_handle(multipart->terminus);

    return xfer_pos;
}


void pldm_multipart_incomming_write(
    pldm_multipart_t* multipart,
    uint8_t* data,
    uint32_t data_len
)
{
    if(multipart == NULL)
    {
        return;
    }

    if(multipart->is_outcomming)
    {
        return;
    }

    if(data != NULL)
    {
        fwrite(data, sizeof(uint8_t), data_len, multipart->data_stream);
        fflush(multipart->data_stream);
    }
}


uint32_t pldm_multipart_outcomming_get_xfer_offset(
    pldm_multipart_t* multipart
)
{
    if(multipart == NULL)
    {
        return 0;
    }

    long offset = ftell(multipart->data_stream);

    return (uint32_t)offset;
}


bool pldm_multipart_is_outcomming(
    pldm_multipart_t* multipart
)
{
    if(multipart == NULL)
    {
        return false;
    }

    return multipart->is_outcomming;
}


bool pldm_multipart_is_incomming(
    pldm_multipart_t* multipart
)
{
    if(multipart == NULL)
    {
        return false;
    }

    return !multipart->is_outcomming;
}

/*
struct pldm_multipart_outcomming_t;
typedef struct pldm_multipart_outcomming_t pldm_multipart_outcomming_t;


typedef struct pldm_multipart_outcomming_t
{
    uint32_t xfer_handle;
    uint32_t xfer_offset;
    uint32_t data_len;
    uint8_t* data;
    pldm_terminus_t* terminus;
    pldm_multipart_outcomming_t* next;
}
pldm_multipart_outcomming_t;


pldm_multipart_outcomming_t* pldm_multipart_outcomming_init(
    pldm_terminus_t* terminus,
    uint32_t data_len,
    uint8_t* data
)
{
    pldm_multipart_outcomming_t* outcomming = malloc(sizeof(pldm_multipart_outcomming_t));

    if(outcomming != NULL)
    {
        memset(outcomming, 0, sizeof(pldm_multipart_outcomming_t));

        outcomming->terminus = terminus;
        outcomming->data_len = data_len;
        outcomming->data = malloc(data_len);
        memcpy(outcomming->data, data, data_len);
    }

    return outcomming;
}


void pldm_multipart_outcomming_destroy(
    pldm_multipart_outcomming_t* outcomming
)
{
    if(outcomming != NULL)
    {
        free(outcomming->data);
    }

    free(outcomming);
}


uint32_t pldm_multipart_outcomming_get_xfer_handle(
    pldm_multipart_outcomming_t* outcomming
)
{
    if(outcomming == NULL)
    {
        return 0;
    }

    return outcomming->xfer_handle;
}


uint32_t pldm_multipart_outcomming_get_xfer_offset(
    pldm_multipart_outcomming_t* outcomming
)
{
    if(outcomming == NULL)
    {
        return 0;
    }

    return outcomming->xfer_offset;
}


uint8_t* pldm_multipart_outcomming_get_data(
    pldm_multipart_outcomming_t* outcomming
)
{
    if(outcomming == NULL)
    {
        return NULL;
    }

    return outcomming->data;
}


uint32_t pldm_multipart_outcomming_get_data_len(
    pldm_multipart_outcomming_t* outcomming
)
{
    if(outcomming == NULL)
    {
        return 0;
    }

    return outcomming->data_len;
}


pldm_multipart_outcomming_t* pldm_multipart_outcomming_get_next(
    pldm_multipart_outcomming_t* outcomming
)
{
    if(outcomming == NULL)
    {
        return NULL;
    }

    return outcomming->next;
}


void pldm_multipart_outcomming_set_next(
    pldm_multipart_outcomming_t* outcomming,
    pldm_multipart_outcomming_t* next
)
{
    if(outcomming == NULL)
    {
        return;
    }

    outcomming->next = next;
}


pldm_xfer_pos_t pldm_multipart_outcomming_update_xfer(
    pldm_multipart_outcomming_t* outcomming,
    uint32_t chunk_size
)
{
    pldm_xfer_pos_t xfer_pos = {};

    if(outcomming != NULL)
    {

        xfer_pos.is_start = outcomming->xfer_offset == 0;

        outcomming->xfer_offset += chunk_size;

        xfer_pos.is_end = (outcomming->xfer_offset == outcomming->data_len);

        xfer_pos.is_middle = !(xfer_pos.is_start || xfer_pos.is_end);

        outcomming->xfer_handle = pldm_terminus_next_xfer_handle(outcomming->terminus);        
    }

    return xfer_pos;
}



struct pldm_multipart_incomming_t;
typedef struct pldm_multipart_incomming_t pldm_multipart_incomming_t;

typedef struct pldm_multipart_incomming_t
{
    uint32_t xfer_handle;
    size_t data_len;
    uint8_t* data;
    FILE* data_stream;
    pldm_terminus_t* terminus;
    pldm_multipart_incomming_t* next;
}
pldm_multipart_incomming_t;


pldm_multipart_incomming_t* pldm_multipart_incomming_init(
    pldm_terminus_t* terminus,
    uint32_t xfer_handle
)
{
    pldm_multipart_incomming_t* incomming = malloc(sizeof(pldm_multipart_incomming_t));

    if(incomming != NULL)
    {
        memset(incomming, 0, sizeof(pldm_multipart_outcomming_t));

        incomming->terminus = terminus;
        incomming->xfer_handle = xfer_handle;
        incomming->data_stream = open_memstream((char**)&incomming->data, &incomming->data_len);
    }

    return incomming;
}


void pldm_multipart_incomming_destroy(
    pldm_multipart_incomming_t* incomming
)
{
    if(incomming != NULL)
    {
        fclose(incomming->data_stream);
        free(incomming->data);
    }

    free(incomming);
}


uint32_t pldm_multipart_incomming_get_xfer_handle(
    pldm_multipart_incomming_t* incomming
)
{
    if(incomming == NULL)
    {
        return 0;
    }

    return incomming->xfer_handle;
}


void pldm_multipart_incomming_set_xfer_handle(
    pldm_multipart_incomming_t* incomming,
    uint32_t xfer_handle
)
{
    if(incomming == NULL)
    {
        return;
    }

    incomming->xfer_handle = xfer_handle;
}


uint8_t* pldm_multipart_incomming_get_data(
    pldm_multipart_incomming_t* incomming
)
{
    if(incomming == NULL)
    {
        return NULL;
    }

    return incomming->data;
}


uint32_t pldm_multipart_incomming_get_data_len(
    pldm_multipart_incomming_t* incomming
)
{
    if(incomming == NULL)
    {
        return 0;
    }

    return (uint32_t)incomming->data_len;
}


pldm_multipart_incomming_t* pldm_multipart_incomming_get_next(
    pldm_multipart_incomming_t* incomming
)
{
    if(incomming == NULL)
    {
        return NULL;
    }

    return incomming->next;
}


void pldm_multipart_incomming_set_next(
    pldm_multipart_incomming_t* incomming,
    pldm_multipart_incomming_t* next
)
{
    if(incomming == NULL)
    {
        return;
    }

    incomming->next = next;
}

void pldm_multipart_incomming_write(
    pldm_multipart_incomming_t* incomming,
    uint8_t* data,
    size_t data_len
)
{
    if(incomming == NULL)
    {
        return;
    }

    fwrite(data, sizeof(uint8_t), data_len, incomming->data_stream);
}
*/