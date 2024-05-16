#ifndef PLDM_PRIVATE_PDR_H
#define PLDM_PRIVATE_PDR_H

#include <pldm/pdr/entry.h>
#include <pldm/pdr/record/numeric_sensor.h>

void pldm_pdr_entry_set_next(
    pldm_pdr_entry_t* entry,
    pldm_pdr_entry_t* next
);

void pldm_pdr_numeric_sensor_ref_set_next(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor,
    pldm_pdr_numeric_sensor_ref_t* next
);

#endif // PLDM_PRIVATE_PDR_H