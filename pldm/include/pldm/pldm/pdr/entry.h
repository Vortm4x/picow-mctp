#ifndef PLDM_PDR_ENTRY_H
#define PLDM_PDR_ENTRY_H

#include <pldm/pldm.h>
#include <pldm/multipart.h>
#include <pldm/pdr/pdr.h>

struct pldm_pdr_entry_t;
typedef struct pldm_pdr_entry_t pldm_pdr_entry_t;


pldm_pdr_entry_t* pldm_pdr_entry_init(
    pldm_pdr_header_t* record
);

void pldm_pdr_entry_destroy(
    pldm_pdr_entry_t* entry
);

pldm_pdr_entry_t* pldm_pdr_entry_get_next(
    pldm_pdr_entry_t* entry
);

pldm_pdr_header_t* pldm_pdr_entry_get_record(
    pldm_pdr_entry_t* entry
);

uint32_t pldm_pdr_entry_get_record_handle(
    pldm_pdr_entry_t* entry
);

pldm_pdr_type_t pldm_pdr_entry_get_type(
    pldm_pdr_entry_t* entry
);

uint16_t pldm_pdr_entry_get_size(
    pldm_pdr_entry_t* entry
);


#endif // PLDM_PDR_ENTRY_H