#include <pldm/multipart.h>
#include <private_multipart.h>
#include <private_terminus.h>
#include <stdlib.h>
#include <string.h>

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
        outcomming->data = data;
    }

    return outcomming;
}


void pldm_multipart_outcomming_destroy(
    pldm_multipart_outcomming_t* outcomming
)
{
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