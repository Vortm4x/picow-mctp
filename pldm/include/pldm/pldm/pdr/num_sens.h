#ifndef NUM_SENS_H
#define NUM_SENS_H

#include <pldm/pldm.h>
#include <pldm/pdr/pdr.h>

typedef enum __attribute__ ((__packed__)) pldm_num_sensor_unit_rel_t
{
    PLDM_SENSOR_UNIT_REL_DIV_BY,
    PLDM_SENSOR_UNIT_REL_MUL_BY,
}
pldm_num_sensor_unit_rel_t;

typedef struct __attribute__ ((__packed__)) pldm_num_sensor_unit_t
{
    pldm_num_measure_unit_t measure;
    int8_t modifier;
    pldm_num_rate_unit_t rate;
}
pldm_num_sensor_unit_t;

typedef struct __attribute__ ((__packed__)) pldm_num_sensor_treshold_support_t
{
    bool upper_treshold_warn    : 1;
    bool upper_treshold_crit    : 1;
    bool upper_treshold_fatal   : 1;
    bool lower_treshold_warn    : 1;
    bool lower_treshold_crit    : 1;
    bool lower_treshold_fatal   : 1;
    uint8_t                     : 2;
}
pldm_num_sensor_treshold_support_t;

typedef struct __attribute__ ((__packed__)) pldm_num_sensor_range_support_t
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
pldm_num_sensor_range_support_t;


typedef struct __attribute__ ((__packed__)) pldm_num_sensor_volatility_t
{
    bool init_agent_upd     : 1;
    bool pldm_power_up      : 1;
    bool system_hard_reset  : 1;
    bool system_warm_reset  : 1;
    bool pldm_term_online   : 1;
    uint8_t                 : 3;
}
pldm_num_sensor_volatility_t;


#define pldm_pdr_num_sens_data_type(integral_type) \
    pldm_pdr_num_sens_data_##integral_type

#define pldm_pdr_num_sens_data(integral_type)                                               \
    typedef struct __attribute__ ((__packed__)) pldm_pdr_num_sens_data_type(integral_type)  \
    {                                                                                       \
        pldm_pdr_data_size_t data_size;                                                     \
        real32_t resolution;                                                                \
        real32_t offset;                                                                    \
        uint16_t accuracy;                                                                  \
        uint8_t plus_tolerance;                                                             \
        uint8_t minus_tolerance;                                                            \
        integral_type hysteresis;                                                           \
        pldm_num_sensor_treshold_support_t treshold_support;                                \
        pldm_num_sensor_volatility_t volatility;                                            \
        real32_t state_transition_interval;                                                 \
        real32_t update_interval;                                                           \
        integral_type max_readable;                                                         \
        integral_type min_readable;                                                         \
    }                                                                                       \
    pldm_pdr_num_sens_data_type(integral_type)

pldm_pdr_decl_template_struct(pldm_pdr_num_sens_data)


#define pldm_pdr_num_sens_range_type(integral_type) \
    pldm_pdr_num_sens_range_##integral_type

#define pldm_pdr_num_sens_range(integral_type)                                              \
    typedef struct __attribute__ ((__packed__)) pldm_pdr_num_sens_range_type(integral_type) \
    {                                                                                       \
        pldm_pdr_data_size_t range_size;                                                    \
        pldm_num_sensor_range_support_t range_support;                                      \
        integral_type nominal_value;                                                        \
        integral_type normal_max;                                                           \
        integral_type normal_min;                                                           \
        integral_type crit_high;                                                            \
        integral_type crit_low;                                                             \
        integral_type fatal_high;                                                           \
        integral_type fatal_low;                                                            \
    }                                                                                       \
    pldm_pdr_num_sens_range_type(integral_type)

pldm_pdr_decl_template_struct(pldm_pdr_num_sens_range)


typedef struct __attribute__ ((__packed__)) pldm_pdr_num_sens_t
{
    uint16_t terminus_handle;
    uint16_t sensor_id;
    uint16_t entity_type;
    uint16_t entity_inst;
    uint16_t container_id;
    pldm_pdr_init_t sensor_init;
    bool aux_names;
    pldm_num_sensor_unit_t base_unit;
    uint8_t base_oem_unit_handle;
    pldm_num_sensor_unit_t aux_unit;
    pldm_num_sensor_unit_rel_t unit_rel;
    uint8_t aux_oem_unit_handle;
    bool is_linear;
} 
pldm_pdr_num_sens_t;



uint8_t* pldm_pdr_num_sens_data_ptr(
    pldm_pdr_num_sens_t* pdr
);

pldm_pdr_data_size_t pldm_pdr_num_sens_data_size(
    pldm_pdr_num_sens_t* pdr
);

size_t pldm_pdr_num_sens_data_struct_len(
    pldm_pdr_data_size_t size
);


uint8_t* pldm_pdr_num_sens_range_ptr(
    pldm_pdr_num_sens_t* pdr
);

pldm_pdr_data_size_t pldm_pdr_num_sens_range_size(
    pldm_pdr_num_sens_t* pdr
);

size_t pldm_pdr_num_sens_range_struct_len(
    pldm_pdr_data_size_t size
);




#endif // NUM_SENS_H