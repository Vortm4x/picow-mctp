#ifndef PLDM_PRIVATE_MULTIPART_H
#define PLDM_PRIVATE_MULTIPART_H

#include <pldm/multipart.h>
#include <private_terminus.h>

pldm_multipart_t* pldm_multipart_outcomming_init(
    pldm_terminus_t* terminus,
    uint32_t data_len,
    uint8_t* data
);

pldm_multipart_t* pldm_multipart_incomming_init(
    pldm_terminus_t* terminus,
    uint32_t xfer_handle
);

void pldm_multipart_destroy(
    pldm_multipart_t* multipart
);

pldm_multipart_t* pldm_multipart_get_next(
    pldm_multipart_t* multipart
);

void pldm_multipart_set_next(
    pldm_multipart_t* multipart,
    pldm_multipart_t* next
);

#endif // PLDM_PRIVATE_MULTIPART_H