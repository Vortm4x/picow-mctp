#ifndef PLDM_PDR_NUMERIC_SENSOR_H
#define PLDM_PDR_NUMERIC_SENSOR_H

#include <pldm/pldm.h>
#include <pldm/pdr/pdr.h>
#include <pldm/pdr/record/accessor.h>
#include <pldm/pdr/record/numeric.h>


typedef struct __attribute__ ((__packed__)) pldm_pdr_base_numeric_sensor_t
{
    pldm_pdr_base_accessor_t base_accessor;
    pldm_base_numeric_accessor_t numeric_accessor;
    bool directly_proportional;
    uint8_t aux_oem_unit_handle;
    bool linear;
} 
pldm_pdr_base_numeric_sensor_t;


typedef struct __attribute__ ((__packed__)) pldm_numeric_sensor_treshold_support_t
{
    bool upper_treshold_warn    : 1;
    bool upper_treshold_crit    : 1;
    bool upper_treshold_fatal   : 1;
    bool lower_treshold_warn    : 1;
    bool lower_treshold_crit    : 1;
    bool lower_treshold_fatal   : 1;
    uint8_t                     : 2;
}
pldm_numeric_sensor_treshold_support_t;

typedef struct __attribute__ ((__packed__)) pldm_numeric_sensor_volatility_t
{
    bool init_agent_upd     : 1;
    bool pldm_power_up      : 1;
    bool system_hard_reset  : 1;
    bool system_warm_reset  : 1;
    bool pldm_term_online   : 1;
    uint8_t                 : 3;
}
pldm_numeric_sensor_volatility_t;

#define pldm_pdr_numeric_sensor_data_type(numeric_type) \
    pldm_pdr_numeric_sensor_data_##numeric_type

#define pldm_pdr_numeric_sensor_data_decl(numeric_type)                                             \
    typedef struct __attribute__ ((__packed__)) pldm_pdr_numeric_sensor_data_type(numeric_type)     \
    {                                                                                               \
        pldm_base_numeric_accessor_data_t base_data;                                                \
        numeric_type hysteresis;                                                                    \
        pldm_numeric_sensor_treshold_support_t treshold_support;                                    \
        pldm_numeric_sensor_volatility_t volatility;                                                    \
        real32_t state_transition_interval;                                                         \
        real32_t update_interval;                                                                   \
        numeric_type max_readable;                                                                  \
        numeric_type min_readable;                                                                  \
    }                                                                                               \
    pldm_pdr_numeric_sensor_data_type(numeric_type)

pldm_pdr_numeric_sensor_data_decl(uint8_t);
pldm_pdr_numeric_sensor_data_decl(int8_t);
pldm_pdr_numeric_sensor_data_decl(uint16_t);
pldm_pdr_numeric_sensor_data_decl(int16_t);
pldm_pdr_numeric_sensor_data_decl(uint32_t);
pldm_pdr_numeric_sensor_data_decl(int32_t);


typedef struct __attribute__ ((__packed__)) pldm_numeric_sensor_range_support_t
{
    bool nominal_value  : 1;
    bool normal_max     : 1;
    bool normal_min     : 1;
    bool crit_high      : 1;
    bool crit_low       : 1;
    bool fatal_high     : 1;
    bool fatal_low      : 1;
    uint8_t             : 1;
}
pldm_numeric_sensor_range_support_t;

#define pldm_pdr_numeric_sensor_range_type(numeric_type) \
    pldm_pdr_numeric_sensor_range_##numeric_type

#define pldm_pdr_numeric_sensor_range_decl(numeric_type)                                            \
    typedef struct __attribute__ ((__packed__)) pldm_pdr_numeric_sensor_range_type(numeric_type)    \
    {                                                                                               \
        pldm_pdr_data_size_t range_size;                                                            \
        pldm_numeric_sensor_range_support_t range_support;                                          \
        numeric_type nominal_value;                                                                 \
        numeric_type normal_max;                                                                    \
        numeric_type normal_min;                                                                    \
        numeric_type crit_high;                                                                     \
        numeric_type crit_low;                                                                      \
        numeric_type fatal_high;                                                                    \
        numeric_type fatal_low;                                                                     \
    }                                                                                               \
    pldm_pdr_numeric_sensor_range_type(numeric_type)

pldm_pdr_numeric_sensor_range_decl(uint8_t);
pldm_pdr_numeric_sensor_range_decl(int8_t);
pldm_pdr_numeric_sensor_range_decl(uint16_t);
pldm_pdr_numeric_sensor_range_decl(int16_t);
pldm_pdr_numeric_sensor_range_decl(uint32_t);
pldm_pdr_numeric_sensor_range_decl(int32_t);
pldm_pdr_numeric_sensor_range_decl(real32_t);


typedef union pldm_numeric_sensor_value_t
{
    uint8_t uint8_value;
    int8_t sint8_value; 
    uint16_t uint16_value; 
    int16_t sint16_value; 
    uint32_t uint32_value; 
    int32_t sint32_value; 
}
pldm_numeric_sensor_value_t;


typedef bool(*pldm_numeric_sensor_read_callback_t)(
    pldm_pdr_data_size_t size,
    pldm_numeric_sensor_value_t* value,
    void* args
);

struct pldm_pdr_numeric_sensor_ref_t;
typedef struct pldm_pdr_numeric_sensor_ref_t pldm_pdr_numeric_sensor_ref_t;


pldm_pdr_numeric_sensor_ref_t* pldm_pdr_numeric_sensor_ref_init(
    pldm_pdr_entry_t* entry
);

void pldm_pdr_numeric_sensor_ref_destroy(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor
);

pldm_pdr_entry_t* pldm_pdr_numeric_sensor_ref_get_entry(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor
);

pldm_pdr_numeric_sensor_ref_t* pldm_pdr_numeric_sensor_ref_get_next(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor
);

pldm_pdr_base_numeric_sensor_t* pldm_pdr_numeric_sensor_ref_get_base(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor
);

uint16_t pldm_pdr_numeric_sensor_ref_get_id(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor
);

void pldm_pdr_numeric_sensor_ref_set_read_callback(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor,
    pldm_numeric_sensor_read_callback_t read_callback,
    void* read_args
);

bool pldm_pdr_numeric_sensor_ref_read_value(
    pldm_pdr_numeric_sensor_ref_t* numeric_sensor,
    pldm_numeric_sensor_value_t* value
);

#endif // PLDM_PDR_NUMERIC_SENSOR_H