#ifndef PLDM_PDR_REPO_H
#define PLDM_PDR_REPO_H

#include <pldm/pldm.h>
#include <pldm/pdr/entry.h>
#include <pldm/pdr/record/numeric_sensor.h>


void pldm_pdr_repo_init();

void pldm_pdr_repo_deinit();

bool pldm_pdr_repo_is_init();

uint32_t pldm_pdr_next_record_handle();

uint32_t pldm_pdr_next_sensor_id();

uint32_t pldm_pdr_repo_get_record_count();

uint32_t pldm_pdr_repo_get_largest_record_size();

uint32_t pldm_pdr_repo_get_signature();

uint32_t pldm_pdr_repo_get_size();

pldm_pdr_entry_t* pldm_pdr_repo_add_entry(
    pldm_pdr_header_t* record
);

pldm_pdr_entry_t* pldm_pdr_repo_get_entry(
    uint32_t record_handle
);

pldm_pdr_numeric_sensor_ref_t* pldm_pdr_repo_get_numeric_sensor(
    uint16_t sensor_id
);

pldm_pdr_entry_t* pldm_pdr_repo_get_entry_by_type(
    pldm_pdr_type_t pdr_type
);

#endif // PLDM_PDR_REPO_H