#ifndef PLDM_PDR_H
#define PLDM_PDR_H

#include <pldm/pldm.h>

typedef enum __attribute__ ((__packed__)) pldm_pdr_data_size_t
{
    PLDM_PDR_DATA_SIZE_UNSIGNED_INT8,
    PLDM_PDR_DATA_SIZE_SIGNED_INT8,
    PLDM_PDR_DATA_SIZE_UNSIGNED_INT16,
    PLDM_PDR_DATA_SIZE_SIGNED_INT16,
    PLDM_PDR_DATA_SIZE_UNSIGNED_INT32,
    PLDM_PDR_DATA_SIZE_SIGNED_INT32,
    PLDM_PDR_DATA_SIZE_REAL32,
}
pldm_pdr_data_size_t;

typedef enum __attribute__ ((__packed__)) pldm_pdr_oper_state_t
{
    PLDM_PDR_OPER_STATE_ENABLED,
    PLDM_PDR_OPER_STATE_DISABLED,
    PLDM_PDR_OPER_STATE_UNAVAILABLE,
    PLDM_PDR_OPER_STATE_UNKNOWN,
    PLDM_PDR_OPER_STATE_FAILED,
    PLDM_PDR_OPER_STATE_INITIALIZING,
    PLDM_PDR_OPER_STATE_SHUTTING_DOWN,
    PLDM_PDR_OPER_STATE_IN_TEST,
}
pldm_pdr_oper_state_t;


#define PLDM_PDR_HEADER_VER 0x01
#define PLDM_PDR_NULL_RECORD_HANDLE 0x00000000
#define PLDM_PDR_NULL_RECORD_CHANGE 0x0000
#define PLDM_PDR_CONTAINER_PLDM_SYSTEM 0x0000

#define PLDM_ENTITY_UNSPEC                              0
#define PLDM_ENTITY_OTHER                               1

// Miscellaneous Entities
#define PLDM_ENTITY_NETWORK                             2
#define PLDM_ENTITY_GROUP                               3
#define PLDM_ENTITY_REMOTE_MANAGEMENT_CONTROLLER_DEVICE 4
#define PLDM_ENTITY_EXT_ENV                             5
#define PLDM_ENTITY_COMMUNICATION_CHANNEL               6
#define PLDM_ENTITY_TERMINUS                            7
#define PLDM_ENTITY_PLATFORM_EVENT_LOG                  8

// Human Interface Entities
#define PLDM_ENTITY_KEYPAD                              15
#define PLDM_ENTITY_SWITCH                              16
#define PLDM_ENTITY_PUSHBUTTON                          17
#define PLDM_ENTITY_DISPLAY                             18
#define PLDM_ENTITY_INDICATOR                           19

// Software/Firmware Entities
#define PLDM_ENTITY_SYSTEM_MANAGEMENT_SOFTWARE          30
#define PLDM_ENTITY_SYSTEM_FIRMWARE                     31
#define PLDM_ENTITY_OPERATING_SYSTEM                    32
#define PLDM_ENTITY_VIRTUAL_MACHINE_MAANAGER            33
#define PLDM_ENTITY_OS_LOADER                           34
#define PLDM_ENTITY_DEVICE_DRIVER                       35
#define PLDM_ENTITY_MANAGEMENT_CONTROLLER_FIRMWARE      36

// Chassis/Enclosure Entities
#define PLDM_ENTITY_SYSTEM_CHASSIS                      45
#define PLDM_ENTITY_SUB_CHASSIS                         46
#define PLDM_ENTITY_DISK_DRIVE_BAY                      47
#define PLDM_ENTITY_PERIPHERAL_BAY                      48
#define PLDM_ENTITY_DEVICE_BAY                          49
#define PLDM_ENTITY_DOOR                                50
#define PLDM_ENTITY_ACCESS_PANEL                        51
#define PLDM_ENTITY_COVER                               52

// Board/Card/Module Entities
#define PLDM_ENTITY_BOARD                               60
#define PLDM_ENTITY_CARD                                61
#define PLDM_ENTITY_MODULE                              62
#define PLDM_ENTITY_SYSTEM_MANAGEMENT_MODULE            63
#define PLDM_ENTITY_SYSTEM_BOARD                        64
#define PLDM_ENTITY_MEMORY_BOARD                        65
#define PLDM_ENTITY_PROCESSOR_MODULE                    67
#define PLDM_ENTITY_ADD_IN_CARD                         68
#define PLDM_ENTITY_CHASSIS_FRONT_PANEL_BOARD           69
#define PLDM_ENTITY_BACK_PANEL_BOARD                    70
#define PLDM_ENTITY_POWER_MANAGEMENT_DISTRIBUTION_BOARD 71
#define PLDM_ENTITY_POWER_SYSTEM_BOARD                  72
#define PLDM_ENTITY_DRIVE_BACKPLANE                     73
#define PLDM_ENTITY_SYSTEM_INTERNAL_EXPANSION_BOARD     74
#define PLDM_ENTITY_OTHER_SYSTEM_BOARD                  75
#define PLDM_ENTITY_CHASSIS_BACK_PANEL_BOARD            76
#define PLDM_ENTITY_PROCESSING_BLADE                    77
#define PLDM_ENTITY_CONNECTIVITY_SWITCH                 78
#define PLDM_ENTITY_PROCESSOR_MEMORY_MODULE             79
#define PLDM_ENTITY_IO_MODULE                           80
#define PLDM_ENTITY_PROCESSOR_IO_MODULE                 81

// Cooling Entities
#define PLDM_ENTITY_COOLING_DEVICE                      90
#define PLDM_ENTITY_COOLING_SUBSYSTEM                   91
#define PLDM_ENTITY_COOLING_UNIT_DOMAIN                 92
#define PLDM_ENTITY_FAN                                 93
#define PLDM_ENTITY_PELTIER_COOLING_DEVICE              94
#define PLDM_ENTITY_LIQUID_COOLING_DEVICE               95
#define PLDM_ENTITY_LIQUID_COOLING_SUBSYSTEM            96

// Storage Device Entities
#define PLDM_ENTITY_OTHER_STORAGE_DEVICE                105
#define PLDM_ENTITY_FLOPPY_DRIVE                        106
#define PLDM_ENTITY_FIXED_DISK                          107
#define PLDM_ENTITY_CD_DRIVE                            108
#define PLDM_ENTITY_CD_DVD_DRIVE                        109
#define PLDM_ENTITY_OTHER_SILICON_STORAGE_DEVICE        110
#define PLDM_ENTITY_SOLID_STATE_DRIVE                   111

// Power Entities
#define PLDM_ENTITY_POWER_SUPPLY                        120
#define PLDM_ENTITY_BATTERY                             121
#define PLDM_ENTITY_SUPER_CAPACITOR                     122
#define PLDM_ENTITY_POWER_CONVERTER                     123
#define PLDM_ENTITY_DC_DC_CONVERTER                     124
#define PLDM_ENTITY_AC_MAINS_POWER_SUPPLY               125
#define PLDM_ENTITY_DC_MAINS_POWER_SUPPLY               126

// Chip Entities
#define PLDM_ENTITY_PROCESSOR                           135
#define PLDM_ENTITY_CHIPSET_COMPONENT                   136
#define PLDM_ENTITY_MANAGEMENT_CONTROLLER               137
#define PLDM_ENTITY_PERIPHERAL_CONTROLLER               138
#define PLDM_ENTITY_SEEPROM                             139
#define PLDM_ENTITY_NVRAM_CHIP                          140
#define PLDM_ENTITY_FLASH_MEMORY_CHIP                   141
#define PLDM_ENTITY_MEMORY_CHIP                         142
#define PLDM_ENTITY_MEMORY_CONTROLLER                   143
#define PLDM_ENTITY_NETWORK_CONTROLLER                  144
#define PLDM_ENTITY_IO_CONTROLLER                       145
#define PLDM_ENTITY_SOUTH_BRIDGE                        146
#define PLDM_ENTITY_REAL_TIME_CLOCK                     147

// Bus Entities
#define PLDM_ENTITY_OTHER_BUS                           160
#define PLDM_ENTITY_SYSTEM_BUS                          161
#define PLDM_ENTITY_I2C_BUS                             162
#define PLDM_ENTITY_SMBUS_BUS                           163
#define PLDM_ENTITY_SPI_BUS                             164
#define PLDM_ENTITY_PCI_BUS                             165
#define PLDM_ENTITY_PCI_EXPRESS_BUS                     166
#define PLDM_ENTITY_PECI_BUS                            167
#define PLDM_ENTITY_LPC_BUS                             168
#define PLDM_ENTITY_USB_BUS                             169
#define PLDM_ENTITY_FIREWIRE_BUS                        170
#define PLDM_ENTITY_SCSI_BUS                            171
#define PLDM_ENTITY_SATA_SAS_BUS                        172
#define PLDM_ENTITY_PROCESSOR_FRONT_SIDE_BUS            173
#define PLDM_ENTITY_INTER_PROCESSOR_BUS                 174

// Connectors/Cables
#define PLDM_ENTITY_CONNECTOR                           185
#define PLDM_ENTITY_SLOT                                186
#define PLDM_ENTITY_CABLE                               187
#define PLDM_ENTITY_INTERCONNECT                        188
#define PLDM_ENTITY_PLUG                                189
#define PLDM_ENTITY_SOCKET                              190

// OEM/Vendor-Defined Entities
#define PLDM_ENTITY_CHASSIS_SPECIFIC_MIN                8192
#define PLDM_ENTITY_CHASSIS_SPECIFIC_MAX                16383
#define PLDM_ENTITY_BOARD_SET_SPECIFIC_MIN              16384
#define PLDM_ENTITY_BOARD_SET_SPECIFIC_MAX              24575
#define PLDM_ENTITY_OEM_SYSTEM_INTEGRATOR_DEFINED_MIN   24576
#define PLDM_ENTITY_OEM_SYSTEM_INTEGRATOR_DEFINED_MAX   32767


typedef enum __attribute__ ((__packed__)) pldm_pdr_type_t
{
    PLDM_PDR_TYPE_TERM_LOCATOR                  = 1,
    PLDM_PDR_TYPE_NUM_SENS                      = 2,
    PLDM_PDR_TYPE_NUM_SENS_INIT                 = 3,
    PLDM_PDR_TYPE_STATE_SENS                    = 4,
    PLDM_PDR_TYPE_STATE_SENS_INIT               = 5,
    PLDM_PDR_TYPE_SENS_AUX_NAMES                = 6,
    PLDM_PDR_TYPE_OEM_UNIT                      = 7,
    PLDM_PDR_TYPE_OEM_STATE_SET                 = 8,
    PLDM_PDR_TYPE_NUM_EFFECT                    = 9,
    PLDM_PDR_TYPE_NUM_EFFECT_INIT               = 10,
    PLDM_PDR_TYPE_STATE_EFFECT                  = 11,
    PLDM_PDR_TYPE_STATE_EFFECT_INIT             = 12,
    PLDM_PDR_TYPE_EFFECT_AUX_NAMES              = 13,
    PLDM_PDR_TYPE_EFFECT_OEM_SEMANTIC           = 14,
    PLDM_PDR_TYPE_ENTITY_ASSOCIATION            = 15,
    PLDM_PDR_TYPE_ENTITY_AUX_NAMES              = 16,
    PLDM_PDR_TYPE_OEM_ENTITY_ID                 = 17,
    PLDM_PDR_TYPE_INTR_ASSOCIATION              = 18,
    PLDM_PDR_TYPE_PLDM_EVENT_LOG                = 19,
    PLDM_PDR_TYPE_FRU_RECORD_SET                = 20,
    PLDM_PDR_TYPE_COMPACT_NUM_SENS              = 21,
    PLDM_PDR_TYPE_REDFISH_RESOURCE              = 22,
    PLDM_PDR_TYPE_REDFISH_ENTITY_ASSOCIATION    = 23,
    PLDM_PDR_TYPE_REDFISH_ACTION                = 24,
    PLDM_PDR_TYPE_OEM_DEVICE                    = 126,
    PLDM_PDR_TYPE_OEM                           = 127,
}
pldm_pdr_type_t;

typedef struct __attribute__ ((__packed__)) pldm_entity_type_t
{
    uint16_t id     : 15;
    bool is_logical : 1;
}
pldm_entity_type_t;

typedef struct __attribute__ ((__packed__)) pldm_entity_info_t
{
    pldm_entity_type_t type;
    uint16_t instance_number;
    uint16_t container_id;
}
pldm_entity_info_t;


struct pldm_pdr_header_t;
typedef struct pldm_pdr_header_t pldm_pdr_header_t;


pldm_pdr_header_t* pldm_pdr_init(
    pldm_pdr_type_t pdr_type,
    uint16_t record_change,
    uint8_t record_data[],
    uint16_t record_data_len
);

void pldm_pdr_destroy(
    pldm_pdr_header_t* pdr
);

uint32_t pldm_pdr_get_record_handle(
    pldm_pdr_header_t* pdr
);

pldm_pdr_type_t pldm_pdr_get_type(
    pldm_pdr_header_t* pdr
);

uint16_t pldm_pdr_get_record_change(
    pldm_pdr_header_t* pdr
);

void pldm_pdr_set_record_change(
    pldm_pdr_header_t* pdr,
    uint16_t record_change
);

uint16_t pldm_pdr_get_data_len(
    pldm_pdr_header_t* pdr
);

uint8_t* pldm_pdr_get_data(
    pldm_pdr_header_t* pdr
);

uint32_t pldm_pdr_get_size(
    pldm_pdr_header_t* pdr
);

#endif // PLDM_PDR_H