#ifndef PLDM_MULTIPART_H
#define PLDM_MULTIPART_H

#include <pldm/types.h>

#define PLDM_MULTIPART_NULL_XFER_HANDLE 0x00000000

struct pldm_multipart_outcomming_t;
typedef struct pldm_multipart_outcomming_t pldm_multipart_outcomming_t;


uint32_t pldm_multipart_outcomming_get_xfer_handle(
    pldm_multipart_outcomming_t* outcomming
);

uint32_t pldm_multipart_outcomming_get_xfer_offset(
    pldm_multipart_outcomming_t* outcomming
);

uint8_t* pldm_multipart_outcomming_get_data(
    pldm_multipart_outcomming_t* outcomming
);

uint32_t pldm_multipart_outcomming_get_data_len(
    pldm_multipart_outcomming_t* outcomming
);

pldm_xfer_pos_t pldm_multipart_outcomming_update_xfer(
    pldm_multipart_outcomming_t* outcomming,
    uint32_t chunk_size
);


#endif // PLDM_MULTIPART_H