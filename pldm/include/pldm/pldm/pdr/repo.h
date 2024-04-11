#ifndef REPO_H
#define REPO_H

#include <pldm/pdr/pdr.h>

struct pldm_pdr_repo_t;
typedef struct pldm_pdr_repo_t pldm_pdr_repo_t;

struct pldm_pdr_repo_entry_t;
typedef struct pldm_pdr_repo_entry_t pldm_pdr_repo_entry_t;

typedef struct pldm_pdr_repo_handle_t
{
    pldm_pdr_repo_t* pdr_repo;
    uint32_t record_handle;
}
pldm_pdr_repo_handle_t;


pldm_pdr_repo_t* pldm_pdr_repo_init();

void pldm_pdr_repo_destroy(
    pldm_pdr_repo_t* pdr_repo
);

uint32_t pldm_pdr_repo_add_entry(
    pldm_pdr_repo_t* pdr_repo,
    pldm_pdr_header_t* record
);

pldm_pdr_header_t* pldm_pdr_repo_get_entry(
    pldm_pdr_repo_t* pdr_repo,
    uint32_t record_handle
);

bool pldm_pdr_repo_remove_entry(
    pldm_pdr_repo_t* pdr_repo,
    uint32_t record_handle
);

pldm_pdr_repo_handle_t* pldm_pdr_repo_create_handle(
    pldm_pdr_repo_t* pdr_repo,
    uint32_t record_handle
);

void pldm_pdr_repo_desroy_handle(
    pldm_pdr_repo_handle_t* record_handle
);


#endif // REPO_H