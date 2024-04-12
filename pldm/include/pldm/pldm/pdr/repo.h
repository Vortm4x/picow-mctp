#ifndef REPO_H
#define REPO_H

#include <pldm/pdr/pdr.h>

struct pldm_pdr_repo_entry_t;
typedef struct pldm_pdr_repo_entry_t pldm_pdr_repo_entry_t;

struct pldm_pdr_repo_t;
typedef struct pldm_pdr_repo_t pldm_pdr_repo_t;


pldm_pdr_repo_t* pldm_pdr_repo_init();

void pldm_pdr_repo_destroy(
    pldm_pdr_repo_t* pdr_repo
);

pldm_pdr_repo_entry_t* pldm_pdr_repo_add_entry(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_header_t* record
);

pldm_pdr_repo_entry_t* pldm_pdr_repo_get_entry(
    pldm_pdr_repo_t* pdr_repo,
    uint32_t record_handle
);

bool pldm_pdr_repo_remove_entry(
    pldm_pdr_repo_t* pdr_repo,
    uint32_t record_handle
);

uint32_t pldm_pdr_repo_total_size(
    pldm_pdr_repo_t* pdr_repo
);

uint32_t pldm_pdr_repo_largest_record_size(
    pldm_pdr_repo_t* pdr_repo
);

uint32_t pldm_pdr_repo_signature(
    pldm_pdr_repo_t* pdr_repo
);


pldm_pdr_header_t* pldm_pdr_repo_entry_get_record(
    pldm_pdr_repo_entry_t* repo_entry
);

uint32_t pldm_pdr_repo_entry_get_xfer_handle(
    pldm_pdr_repo_entry_t* repo_entry
);

uint32_t pldm_pdr_repo_entry_get_xfer_offset(
    pldm_pdr_repo_entry_t* repo_entry
);

pldm_pdr_repo_entry_t* pldm_pdr_repo_entry_get_next_entry(
    pldm_pdr_repo_entry_t* repo_entry
);

pldm_xfer_pos_t pldm_pdr_repo_entry_xfer_upd(
    pldm_pdr_repo_entry_t* repo_entry,
    uint32_t data_sent
);

void pldm_pdr_repo_entry_xfer_reset(
    pldm_pdr_repo_entry_t* repo_entry
);

#endif // REPO_H