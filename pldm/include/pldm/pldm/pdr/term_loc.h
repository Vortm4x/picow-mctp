#ifndef TERM_LOC_H
#define TERM_LOC_H

#include <pldm/pldm.h>
#include <pldm/pdr/pdr.h>

typedef enum __attribute__ ((__packed__)) pldm_term_loc_type_t
{
    PLDM_LOCATOR_TYPE_UID,
    PLDM_LOCATOR_TYPE_MCTP,
    PLDM_LOCATOR_TYPE_SMBUS,
    PLDM_LOCATOR_TYPE_SYS_SW,
}
pldm_term_loc_type_t;

typedef enum __attribute__ ((__packed__)) pldm_sys_sw_loc_class_t
{
    PLDM_SYS_SW_CLASS_UNSPEC,
    PLDM_SYS_SW_CLASS_OTHER,
    PLDM_SYS_SW_CLASS_SYS_FIRMWARE,
    PLDM_SYS_SW_CLASS_OS_LOADER,
    PLDM_SYS_SW_CLASS_OS,
    PLDM_SYS_SW_CLASS_CIM_PROVIDER,
    PLDM_SYS_SW_CLASS_OTHER_PROVIDER,
    PLDM_SYS_SW_CLASS_VM_MANAGER,
}
pldm_sys_sw_loc_class_t;

typedef struct __attribute__ ((__packed__)) pldm_term_loc_uid_t
{
    uint8_t terminus_inst;
    uint8_t device_uuid[16];
}
pldm_term_loc_uid_t;

typedef struct __attribute__ ((__packed__)) pldm_term_loc_mctp_t
{
    uint8_t mctp_eid;
}
pldm_term_loc_mctp_t;

typedef struct __attribute__ ((__packed__)) pldm_term_loc_smbus_t
{
    uint8_t device_uuid[16];
    uint8_t bus_number;    
    uint8_t             : 1;
    uint8_t slave_addr  : 7;
}
pldm_term_loc_smbus_t;

typedef struct __attribute__ ((__packed__)) pldm_term_loc_software_t
{
    pldm_sys_sw_loc_class_t sw_class;
    uint8_t sw_uuid[16];
}
pldm_term_loc_software_t;

typedef struct __attribute__ ((__packed__)) pldm_pdr_term_locator_t
{
    pldm_pdr_header_t header;
    uint16_t terminus_handle;
    bool is_valid   : 1;
    uint8_t         : 7;
    pldm_tid_t terminus_id;
    uint16_t container_id;
    pldm_term_loc_type_t locator_type;
    uint8_t locator_len;
    uint8_t locator_data[];
}
pldm_pdr_term_locator_t;

#endif // TERM_LOC_H