#ifndef PLDM_PDR_TERMINUS_LOCATOR_H
#define PLDM_PDR_TERMINUS_LOCATOR_H

#include <pldm/pldm.h>


typedef enum __attribute__ ((__packed__)) pldm_term_loc_type_t
{
    PLDM_LOCATOR_TYPE_UID,
    PLDM_LOCATOR_TYPE_MCTP,
    PLDM_LOCATOR_TYPE_SMBUS,
    PLDM_LOCATOR_TYPE_SYS_SW,
}
pldm_term_loc_type_t;

typedef enum __attribute__ ((__packed__)) pldm_sys_sw_class_t
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
pldm_sys_sw_class_t;

typedef struct __attribute__ ((__packed__)) pldm_terminus_locator_uid_t
{
    uint8_t terminus_inst;
    uuid_t device_uuid;
}
pldm_terminus_locator_uid_t;


typedef struct __attribute__ ((__packed__)) pldm_terminus_locator_mctp_t
{
    uint8_t mctp_eid;
}
pldm_terminus_locator_mctp_t;


typedef struct __attribute__ ((__packed__)) pldm_terminus_locator_smbus_t
{
    uuid_t device_uuid;
    uint8_t bus_number;    
    uint8_t             : 1;
    uint8_t slave_addr  : 7;
}
pldm_terminus_locator_smbus_t;


typedef struct __attribute__ ((__packed__)) pldm_terminus_locator_software_t
{
    pldm_sys_sw_class_t sw_class;
    uuid_t sw_uuid;
}
pldm_terminus_locator_software_t;


typedef struct __attribute__ ((__packed__)) pldm_pdr_terminus_locator_t
{
    uint16_t terminus_handle;
    bool is_valid   : 1;
    uint8_t         : 7;
    pldm_tid_t terminus_id;
    uint16_t container_id;
    pldm_term_loc_type_t locator_type;
    uint8_t locator_len;
    uint8_t locator_data[];
}
pldm_pdr_terminus_locator_t;

#endif // PLDM_PDR_TERMINUS_LOCATOR_H