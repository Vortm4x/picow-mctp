#ifndef REPO_H
#define REPO_H

#include <pldm/pdr/pdr.h>

struct pldm_pdr_repo_t;
typedef struct pldm_pdr_repo_t pldm_pdr_repo_t;

struct pldm_pdr_repo_entry_t;
typedef struct pldm_pdr_repo_entry_t pldm_pdr_repo_entry_t;

struct pldm_pdr_repo_sens_ref_t;
typedef struct pldm_pdr_repo_sens_ref_t pldm_pdr_repo_sens_ref_t;


typedef void(*pldm_pdr_sensor_read_t)(
    uint8_t data[],
    size_t data_len
);

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

pldm_pdr_repo_sens_ref_t* pldm_pdr_repo_get_sensor(
    pldm_pdr_repo_t* pdr_repo,
    uint16_t sensor_id
);

void pldm_pdr_repo_sensor_read(
    pldm_pdr_repo_sens_ref_t* sens_ref,
    uint8_t data[],
    size_t data_len
);

pldm_pdr_repo_entry_t* pldm_pdr_repo_sensor_get_entry(
    pldm_pdr_repo_sens_ref_t* sens_ref
);

void pldm_pdr_repo_sensor_set_read_callback(
    pldm_pdr_repo_sens_ref_t* sens_ref,
    pldm_pdr_sensor_read_t sensor_read_callback
);

void pldm_pdr_repo_sensor_set_read_callback(
    pldm_pdr_repo_sens_ref_t* sens_ref,
    pldm_pdr_sensor_read_t sensor_read_callback
);

#endif // REPO_H