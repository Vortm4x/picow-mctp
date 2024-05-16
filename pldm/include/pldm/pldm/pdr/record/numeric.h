#ifndef PLDM_PDR_NUMERIC_H
#define PLDM_PDR_NUMERIC_H

#include <pldm/pdr/record/accessor.h>

typedef enum __attribute__ ((__packed__)) pldm_numeric_measure_unit_t
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
pldm_numeric_measure_unit_t;

typedef enum __attribute__ ((__packed__)) pldm_numeric_rate_unit_t
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
pldm_numeric_rate_unit_t;


typedef struct __attribute__ ((__packed__)) pldm_numeric_unit_t
{
    pldm_numeric_measure_unit_t measure;
    int8_t modifier;
    pldm_numeric_rate_unit_t rate;
}
pldm_numeric_unit_t;

typedef struct __attribute__ ((__packed__)) pldm_base_numeric_accessor_t
{
    pldm_pdr_accessor_init_t init;
    bool aux_names_pdr;
    pldm_numeric_unit_t base_unit;
    uint8_t base_oem_unit_handle;
    pldm_numeric_unit_t aux_unit;
}
pldm_base_numeric_accessor_t;

typedef struct __attribute__ ((__packed__)) pldm_base_numeric_accessor_data_t
{
    pldm_pdr_data_size_t format;
    real32_t resolution;
    real32_t offset;
    uint16_t accuracy;
    uint8_t plus_tolerance;
    uint8_t minus_tolerance;
}
pldm_base_numeric_accessor_data_t;

#endif // PLDM_PDR_NUMERIC_H