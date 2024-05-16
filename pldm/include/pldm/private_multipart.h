#ifndef PLDM_PRIVATE_MULTIPART_H
#define PLDM_PRIVATE_MULTIPART_H

#include <pldm/multipart.h>
#include <private_terminus.h>


pldm_multipart_outcomming_t* pldm_multipart_outcomming_init(
    pldm_terminus_t* terminus,
    uint32_t data_len,
    uint8_t* data
);

void pldm_multipart_outcomming_destroy(
    pldm_multipart_outcomming_t* outcomming
);

pldm_multipart_outcomming_t* pldm_multipart_outcomming_get_next(
    pldm_multipart_outcomming_t* outcomming
);

void pldm_multipart_outcomming_set_next(
    pldm_multipart_outcomming_t* outcomming,
    pldm_multipart_outcomming_t* next
);


#endif // PLDM_PRIVATE_MULTIPART_H