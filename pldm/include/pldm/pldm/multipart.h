#ifndef PLDM_MULTIPART_H
#define PLDM_MULTIPART_H

#include <pldm/types.h>
#include <stddef.h>

#define PLDM_MULTIPART_NULL_XFER_HANDLE 0x00000000
#define PLDM_MULTIPART_NOT_FINISHED_XFER_HANDLE 0xFFFFFFFF

struct pldm_multipart_t;
typedef struct pldm_multipart_t pldm_multipart_t;

uint32_t pldm_multipart_get_xfer_handle(
    pldm_multipart_t* multipart
);

void pldm_multipart_incomming_set_xfer_handle(
    pldm_multipart_t* multipart,
    uint32_t xfer_handle
);

uint8_t* pldm_multipart_get_data(
    pldm_multipart_t* multipart
);

uint32_t pldm_multipart_get_data_len(
    pldm_multipart_t* multipart
);

pldm_xfer_pos_t pldm_multipart_outcomming_read(
    pldm_multipart_t* multipart,
    uint8_t* data,
    uint32_t data_len
);

void pldm_multipart_incomming_write(
    pldm_multipart_t* multipart,
    uint8_t* data,
    uint32_t data_len
);

uint32_t pldm_multipart_outcomming_get_xfer_offset(
    pldm_multipart_t* multipart
);

bool pldm_multipart_is_outcomming(
    pldm_multipart_t* multipart
);

bool pldm_multipart_is_incomming(
    pldm_multipart_t* multipart
);

#endif // PLDM_MULTIPART_H