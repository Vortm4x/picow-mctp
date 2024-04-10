#ifndef PDR_H
#define PDR_H

#include <pldm/pldm.h>
#include <stdint.h>


#define PLDM_PDR_HANDLE_RESERVED 0x00000000


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

typedef enum __attribute__ ((__packed__)) pldm_pdr_data_size_t
{
    PLDM_PDR_DATA_SIZE_UNSIGNED_INT8,
    PLDM_PDR_DATA_SIZE_SIGNED_INT8,
    PLDM_PDR_DATA_SIZE_UNSIGNED_INT16,
    PLDM_PDR_DATA_SIZE_SIGNED_INT16,
    PLDM_PDR_DATA_SIZE_UNSIGNED_INT32,
    PLDM_PDR_DATA_SIZE_SIGNED_INT32,
}
pldm_pdr_data_size_t;

typedef enum __attribute__ ((__packed__)) pldm_num_measure_unit_t
{
    PLDM_NUM_MEASURE_UNIT_NONE                   = 0,
    PLDM_NUM_MEASURE_UNIT_UNSPEC                 = 1,
    PLDM_NUM_MEASURE_UNIT_DEG_C                  = 2,
    PLDM_NUM_MEASURE_UNIT_DEG_F                  = 3,
    PLDM_NUM_MEASURE_UNIT_KELVINS                = 4,
    PLDM_NUM_MEASURE_UNIT_VOLTS                  = 5,
    PLDM_NUM_MEASURE_UNIT_AMPS                   = 6,
    PLDM_NUM_MEASURE_UNIT_WATTS                  = 7,
    PLDM_NUM_MEASURE_UNIT_JOULES                 = 8,
    PLDM_NUM_MEASURE_UNIT_COULOMBS               = 9,
    PLDM_NUM_MEASURE_UNIT_VA                     = 10,
    PLDM_NUM_MEASURE_UNIT_NITS                   = 11,
    PLDM_NUM_MEASURE_UNIT_LUMENS                 = 12,
    PLDM_NUM_MEASURE_UNIT_LUX                    = 13,
    PLDM_NUM_MEASURE_UNIT_CANDELAS               = 14,
    PLDM_NUM_MEASURE_UNIT_KPA                    = 15,
    PLDM_NUM_MEASURE_UNIT_PSI                    = 16,
    PLDM_NUM_MEASURE_UNIT_NEWTONS                = 17,
    PLDM_NUM_MEASURE_UNIT_CFM                    = 18,
    PLDM_NUM_MEASURE_UNIT_RPM                    = 19,
    PLDM_NUM_MEASURE_UNIT_HERTZ                  = 20,
    PLDM_NUM_MEASURE_UNIT_SECONDS                = 21,
    PLDM_NUM_MEASURE_UNIT_MINUTES                = 22,
    PLDM_NUM_MEASURE_UNIT_HOURS                  = 23,
    PLDM_NUM_MEASURE_UNIT_DAYS                   = 24,
    PLDM_NUM_MEASURE_UNIT_WEEKS                  = 25,
    PLDM_NUM_MEASURE_UNIT_MILS                   = 26,
    PLDM_NUM_MEASURE_UNIT_INCHES                 = 27,
    PLDM_NUM_MEASURE_UNIT_FEET                   = 28,
    PLDM_NUM_MEASURE_UNIT_CUBIC_INCHES           = 29,
    PLDM_NUM_MEASURE_UNIT_CUBIC_FEET             = 30,
    PLDM_NUM_MEASURE_UNIT_METERS                 = 31,
    PLDM_NUM_MEASURE_UNIT_CUBIC_CENTIMETERS      = 32,
    PLDM_NUM_MEASURE_UNIT_CUBIC_METERS           = 33,
    PLDM_NUM_MEASURE_UNIT_LITERS                 = 34,
    PLDM_NUM_MEASURE_UNIT_FLUID_OUNCES           = 35,
    PLDM_NUM_MEASURE_UNIT_RADIANS                = 36,
    PLDM_NUM_MEASURE_UNIT_STERADIANS             = 37,
    PLDM_NUM_MEASURE_UNIT_REVOLUTIONS            = 38,
    PLDM_NUM_MEASURE_UNIT_CYCLES                 = 39,
    PLDM_NUM_MEASURE_UNIT_GRAVITIES              = 40,
    PLDM_NUM_MEASURE_UNIT_OUNCES                 = 41,
    PLDM_NUM_MEASURE_UNIT_POUNDS                 = 42,
    PLDM_NUM_MEASURE_UNIT_FOOT_POUNDS            = 43,
    PLDM_NUM_MEASURE_UNIT_OUNCE_INCHES           = 44,
    PLDM_NUM_MEASURE_UNIT_GAUSS                  = 45,
    PLDM_NUM_MEASURE_UNIT_GILBERTS               = 46,
    PLDM_NUM_MEASURE_UNIT_HENRIES                = 47,
    PLDM_NUM_MEASURE_UNIT_FARADS                 = 48,
    PLDM_NUM_MEASURE_UNIT_OHMS                   = 49,
    PLDM_NUM_MEASURE_UNIT_SIEMENS                = 50,
    PLDM_NUM_MEASURE_UNIT_MOLES                  = 51,
    PLDM_NUM_MEASURE_UNIT_BECQUERELS             = 52,
    PLDM_NUM_MEASURE_UNIT_PPM                    = 53,
    PLDM_NUM_MEASURE_UNIT_DECIBELS               = 54,
    PLDM_NUM_MEASURE_UNIT_DBA                    = 55,
    PLDM_NUM_MEASURE_UNIT_DBC                    = 56,
    PLDM_NUM_MEASURE_UNIT_GRAYS                  = 57,
    PLDM_NUM_MEASURE_UNIT_SIEVERTS               = 58,
    PLDM_NUM_MEASURE_UNIT_COLOR_TEMP_DEG_K       = 59,
    PLDM_NUM_MEASURE_UNIT_BITS                   = 60,
    PLDM_NUM_MEASURE_UNIT_BYTES                  = 61,
    PLDM_NUM_MEASURE_UNIT_WORDS                  = 62,
    PLDM_NUM_MEASURE_UNIT_DWORDS                 = 63,
    PLDM_NUM_MEASURE_UNIT_QWORDS                 = 64,
    PLDM_NUM_MEASURE_UNIT_PERCENTAGE             = 65,
    PLDM_NUM_MEASURE_UNIT_PASCALS                = 66,
    PLDM_NUM_MEASURE_UNIT_COUNTS                 = 67,
    PLDM_NUM_MEASURE_UNIT_GRAMS                  = 68,
    PLDM_NUM_MEASURE_UNIT_NEWTON_METERS          = 69,
    PLDM_NUM_MEASURE_UNIT_HITS                   = 70,
    PLDM_NUM_MEASURE_UNIT_MISSES                 = 71,
    PLDM_NUM_MEASURE_UNIT_RETRIES                = 72,
    PLDM_NUM_MEASURE_UNIT_OVERRUNS               = 73,
    PLDM_NUM_MEASURE_UNIT_UNDERRUNS              = 74,
    PLDM_NUM_MEASURE_UNIT_COLLISIONS             = 75,
    PLDM_NUM_MEASURE_UNIT_PACKETS                = 76,
    PLDM_NUM_MEASURE_UNIT_MESSAGES               = 77,
    PLDM_NUM_MEASURE_UNIT_CHARACTERS             = 78,
    PLDM_NUM_MEASURE_UNIT_ERRORS                 = 79,
    PLDM_NUM_MEASURE_UNIT_CORRECTED_ERRORS       = 80,
    PLDM_NUM_MEASURE_UNIT_UNCORRECTABLE_ERRORS   = 81,
    PLDM_NUM_MEASURE_UNIT_SQUARE_MILS            = 82,
    PLDM_NUM_MEASURE_UNIT_SQUARE_INCHES          = 83,
    PLDM_NUM_MEASURE_UNIT_SQUARE_FEET            = 84,
    PLDM_NUM_MEASURE_UNIT_SQUARE_CENTIMETERS     = 85,
    PLDM_NUM_MEASURE_UNIT_SQUARE_METERS          = 86,
    PLDM_NUM_MEASURE_UNIT_OEM_UNIT               = 255,
}
pldm_num_measure_unit_t;

typedef enum __attribute__ ((__packed__)) pldm_num_rate_unit_t
{
    PLDM_NUM_RATE_UNIT_NONE,
    PLDM_NUM_RATE_UNIT_MICRO_SECOND,
    PLDM_NUM_RATE_UNIT_MILLI_SECOND,
    PLDM_NUM_RATE_UNIT_SECOND,
    PLDM_NUM_RATE_UNIT_MINUTE,
    PLDM_NUM_RATE_UNIT_HOUR,
    PLDM_NUM_RATE_UNIT_DAY,
    PLDM_NUM_RATE_UNIT_WEEK,
    PLDM_NUM_RATE_UNIT_MONTH,
    PLDM_NUM_RATE_UNIT_YEAR,
}
pldm_num_rate_unit_t;

typedef enum __attribute__ ((__packed__)) pldm_pdr_init_t
{
    PLDM_PDR_INIT_NO_INIT,
    PLDM_PDR_INIT_USE_INIT_PDR,
    PLDM_PDR_INIT_ENABLE_SENSOR,
    PLDM_PDR_INIT_DISABLE_SENSOR,
}
pldm_pdr_init_t;


#define pldm_pdr_decl_template_struct(pldm_template_struct)             \
    pldm_template_struct(uint8_t);                                      \
    pldm_template_struct(int8_t);                                       \
    pldm_template_struct(uint16_t);                                     \
    pldm_template_struct(int16_t);                                      \
    pldm_template_struct(uint32_t);                                     \
    pldm_template_struct(int32_t);


typedef struct __attribute__ ((__packed__)) pldm_pdr_header_t
{
    uint32_t record_handle;
    uint8_t version;
    pldm_pdr_type_t pdr_type;
    uint16_t record_change;
    uint16_t data_len;
}
pldm_pdr_header_t;


#endif // PDR_H