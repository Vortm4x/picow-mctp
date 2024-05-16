#ifndef PLDM_TYPES_H
#define PLDM_TYPES_H

#include <stdint.h>
#include <stdbool.h>


typedef uint8_t pldm_tid_t;
typedef uint8_t mctp_eid_t;


typedef struct __attribute__ ((__packed__)) pldm_xfer_pos_t
{
    bool is_start   : 1;
    bool is_middle  : 1;
    bool is_end     : 1;
    uint8_t         : 5;
}
pldm_xfer_pos_t;


typedef enum __attribute__ ((__packed__)) time_resolution_t
{
    TIME_RESOLUTION_MICRO_SEC       = 0,
    TIME_RESOLUTION_10_MICRO_SEC    = 1,
    TIME_RESOLUTION_100_MICRO_SEC   = 2,
    TIME_RESOLUTION_MILLI_SEC       = 3,
    TIME_RESOLUTION_10_MILLI_SEC    = 4,
    TIME_RESOLUTION_100_MILLI_SEC   = 5,
    TIME_RESOLUTION_SEC             = 6,
    TIME_RESOLUTION_10_SEC          = 7,
    TIME_RESOLUTION_MIN             = 8,
    TIME_RESOLUTION_10_MIN          = 9,
    TIME_RESOLUTION_HOUR            = 10,
    TIME_RESOLUTION_DAY             = 11,
    TIME_RESOLUTION_MONTH           = 12,
    TIME_RESOLUTION_YEAR            = 13,
    TIME_RESOLUTION_NULL            = 15,
}
time_resolution_t;


typedef enum __attribute__ ((__packed__)) utc_resolution_t
{
    UTC_RESOLUTION_UNSPEC   = 0,
    UTC_RESOLUTION_MIN      = 1,
    UTC_RESOLUTION_10_MIN   = 2,
    UTC_RESOLUTION_HOUR     = 3,
}
utc_resolution_t;


typedef enum __attribute__ ((__packed__)) interval_resolution_t
{
    INTERVAL_RESOLUTION_MICRO_SEC       = 0,
    INTERVAL_RESOLUTION_10_MICRO_SEC    = 1,
    INTERVAL_RESOLUTION_100_MICRO_SEC   = 2,
    INTERVAL_RESOLUTION_MILLI_SEC       = 3,
    INTERVAL_RESOLUTION_10_MILLI_SEC    = 4,
    INTERVAL_RESOLUTION_100_MILLI_SEC   = 5,
    INTERVAL_RESOLUTION_SEC             = 6,
    INTERVAL_RESOLUTION_10_SEC          = 7,
    INTERVAL_RESOLUTION_MIN             = 8,
    INTERVAL_RESOLUTION_HOUR            = 9,
    INTERVAL_RESOLUTION_DAY             = 10,
    INTERVAL_RESOLUTION_10_DAY          = 11,
    INTERVAL_RESOLUTION_100_DAY         = 12,
}
interval_resolution_t;


typedef union __attribute__ ((__packed__)) real32_t
{
    struct __attribute__ ((__packed__))
    {
        uint32_t mantissa   : 23;
        uint32_t exponent   : 8;
        bool sign           : 1;
    };

    uint32_t binary;
}
real32_t;


typedef struct __attribute__ ((__packed__)) real64_t
{
    struct __attribute__ ((__packed__))
    {
        uint64_t mantissa   : 52;
        uint64_t exponent   : 11;
        bool sign           : 1;
    };

    uint64_t binary;
}
real64_t;


typedef struct __attribute__ ((__packed__)) timestamp104_t
{
    int16_t utc_offset;
    uint32_t micro_sec : 24;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
    time_resolution_t time_res : 4;
    utc_resolution_t utc_res : 4;
}
timestamp104_t;


typedef struct __attribute__ ((__packed__)) interval72_t
{
    uint32_t micro_sec : 24;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint16_t day;
    interval_resolution_t interval_res : 4;
    uint8_t : 4;
}
interval72_t;


typedef union __attribute__ ((__packed__)) ver32_t
{
    struct __attribute__ ((__packed__))
    {
        uint8_t alpha;
        uint8_t update;
        uint8_t minor;
        uint8_t major;
    };
    
    uint32_t version;
}
ver32_t;


typedef union __attribute__ ((__packed__)) uuid_t
{
    struct __attribute__ ((__packed__))
    {
        uint64_t node       : 48;
        uint16_t clock_seq  : 14;
        uint8_t variant     : 2;
        uint16_t time_high  : 12;
        uint8_t version     : 4;
        uint16_t time_mid   : 16;
        uint32_t time_low   : 32;
    };
    
    uint8_t bytes[16];
}
uuid_t;


typedef struct __attribute__ ((__packed__)) utf8_str_t
{
    uint16_t len;
    uint8_t data[];
}
utf8_str_t;


typedef enum __attribute__ ((__packed__)) vars_str_type_t
{
    VAR_STR_UNKNOWN     = 0,
    VAR_STR_ASCII       = 1,
    VAR_STR_UTF_8       = 2,
    VAR_STR_UTF_16      = 3,
    VAR_STR_UTF_16LE    = 4,
    VAR_STR_UTF_16BE    = 5,
}
vars_str_type_t;


typedef struct __attribute__ ((__packed__)) var_str_t
{
    vars_str_type_t str_type;
    uint8_t len;
    uint8_t data[];
}
var_str_t;


typedef enum __attribute__ ((__packed__)) pldm_rde_schema_class_t
{
    RDE_SCHEMA_CLASS_MAJOR         = 0,
    RDE_SCHEMA_CLASS_EVENT         = 1,
    RDE_SCHEMA_CLASS_ANNOTATION    = 2,
    RDE_SCHEMA_CLASS_ENTRY_TYPE    = 3,
    RDE_SCHEMA_CLASS_ERROR         = 4,
    RDE_SCHEMA_CLASS_REGISTRY      = 5,
}
pldm_rde_schema_class_t;


typedef uint16_t pldm_rde_oper_id_t;

typedef enum __attribute__ ((__packed__)) pldm_rde_oper_type_t
{
    RDE_OP_TYPE_HEAD    = 0,
    RDE_OP_TYPE_READ    = 1,
    RDE_OP_TYPE_CREATE  = 2,
    RDE_OP_TYPE_DELETE  = 3,
    RDE_OP_TYPE_UPDATE  = 4,
    RDE_OP_TYPE_REPLACE = 5,
    RDE_OP_TYPE_ACTION  = 6,
    RDE_OP_TYPE_UNKNOWN = 7,
}
pldm_rde_oper_type_t;

typedef enum __attribute__ ((__packed__)) pldm_rde_oper_status_t
{
    RDE_OP_STATUS_INACTIVE      = 0,
    RDE_OP_STATUS_NEEDS_INPUT   = 1,
    RDE_OP_STATUS_TRIGGERED     = 2,
    RDE_OP_STATUS_RUNNING       = 3,
    RDE_OP_STATUS_HAVE_RESULTS  = 4,
    RDE_OP_STATUS_COMPLETED     = 5,
    RDE_OP_STATUS_FAILED        = 6,
    RDE_OP_STATUS_ABANDONED     = 7,
}
pldm_rde_oper_status_t;


#endif // PLDM_TYPES_H