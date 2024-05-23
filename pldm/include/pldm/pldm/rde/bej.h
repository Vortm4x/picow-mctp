#ifndef PLDM_RDE_BEJ_H
#define PLDM_RDE_BEJ_H

#include <pldm/types.h>
#include <pldm/rde/unpacked.h>

#define BEJ_VERSION_1_0_0   0xF1F0F000
#define BEJ_VERSION_1_1_0   0xF1F1F000


typedef enum __attribute__ ((__packed__)) bej_type_t
{
    BEJ_TYPE_SET                        = 0x00,
    BEJ_TYPE_ARRAY                      = 0x01,
    BEJ_TYPE_NULL                       = 0x02,
    BEJ_TYPE_INTEGER                    = 0x03,
    BEJ_TYPE_ENUM                       = 0x04,
    BEJ_TYPE_STRING                     = 0x05,
    BEJ_TYPE_REAL                       = 0x06,
    BEJ_TYPE_BOOLEAN                    = 0x07,
    BEJ_TYPE_BYTE_STRING                = 0x08,
    BEJ_TYPE_CHOICE                     = 0x09,
    BEJ_TYPE_PROPERTY_ANNOTATION        = 0x0A,
    BEJ_TYPE_REGISTRY_ITEM              = 0x0B,
    BEJ_TYPE_RESOURCE_LINK              = 0x0E,
    BEJ_TYPE_RESOURCE_LINK_EXPANSION    = 0x0F,
}
bej_type_t;


typedef struct __attribute__ ((__packed__)) bej_encoding_header_t
{
    ver32_t version;
    uint16_t : 16;
    pldm_rde_schema_class_t schema_class;
}
bej_encoding_header_t;


typedef struct __attribute__ ((__packed__)) nnint_t
{
    uint8_t len;
    uint8_t data[];
}
nnint_t;


typedef union __attribute__ ((__packed__)) bej_s_tuple_t
{
    nnint_t seq;

    struct __attribute__ ((__packed__))
    {
        uint8_t         : 8;
        bool annotation : 1;
    };
}
bej_s_tuple_t;


typedef union __attribute__ ((__packed__)) bej_f_tuple_t
{
    struct __attribute__ ((__packed__))
    {
        bool deferred_binding   : 1;
        bool read_only          : 1;
        bool nullabale          : 1;
        uint8_t                 : 1;
        bej_type_t bej_type     : 4;
    };

    struct __attribute__ ((__packed__))
    {
        uint8_t                     : 1;
        bool top_level_annotation   : 1;
        uint8_t                     : 6;
    };
}
bej_f_tuple_t;


typedef struct __attribute__ ((__packed__)) bej_l_tuple_t
{
    nnint_t len;
}
bej_l_tuple_t;


typedef struct __attribute__ ((__packed__)) bej_dict_header_t
{
    uint8_t version_tag;
    uint8_t                 : 7;
    bool truncation_flag    : 1;
    uint16_t entry_count;
    uint32_t schema_version;
    uint32_t dict_size;
}
bej_dict_header_t;


typedef struct __attribute__ ((__packed__)) bej_dict_entry_header
{
    bej_f_tuple_t format;
    uint16_t sequence;
    uint16_t offset;
    uint16_t child_count;
    uint8_t name_size;
    uint16_t name_ofset;
}
bej_dict_entry_header;















/*
typedef struct bej_sflv_tuple_t
{
    bej_s_tuple_t* s_tuple;
    bej_f_tuple_t* f_tuple;
    bej_l_tuple_t* l_tuple;
    void* sfl_tuple_data;
    void* v_tuple_data;
}
bej_sflv_tuple_t;

struct bej_entry_t;
typedef struct bej_entry_t bej_entry_t;

typedef struct bej_entry_t
{
    bej_sflv_tuple_t* value;
    bej_entry_t* next;
}
bej_entry_t;
*/


/*
typedef uint64_t nnint_t_unpacked_t;

typedef struct bej_s_tuple_unpacked_t
{
    nnint_t_unpacked_t seq : 63;
    bool annotation : 1;
}
bej_s_tuple_unpacked_t;

typedef bej_f_tuple_t bej_f_tuple_unpacked_t;

typedef nnint_t_unpacked_t bej_l_tuple_unpacked_t;

struct bej_sflv_tuple_unpacked_t;
typedef struct bej_sflv_tuple_unpacked_t bej_sflv_tuple_unpacked_t;

typedef struct bej_entry_t bej_entry_t;
typedef struct bej_entry_t
{
    bej_sflv_tuple_unpacked_t* value;
    bej_entry_t* next;
}
bej_entry_t;


typedef void bej_null_unpacked_t;

typedef int64_t bej_integer_unpacked_t;

typedef nnint_t_unpacked_t bej_enum_unpacked_t;

typedef char bej_string_unpacked_t;

typedef struct bej_real_unpacked_t {
    nnint_t_unpacked_t whole_len;
    bej_integer_unpacked_t whole;
    nnint_t_unpacked_t lead_zero_count;
    nnint_t_unpacked_t fract;
    nnint_t_unpacked_t exponent_len;
    bej_integer_unpacked_t exponent;
}
bej_real_unpacked_t;

typedef bool bej_boolean_unpacked_t;

typedef uint8_t bej_bytestring_unpacked_t;


typedef struct bej_entry_t {

}
bej_entry_t;

typedef struct bej_sflv_tuple_unpacked_t bej_choice_unpacked_t;

typedef struct bej_sflv_tuple_unpacked_t bej_choice_unpacked_t;

typedef struct bej_sflv_tuple_unpacked_t
{
    bej_s_tuple_unpacked_t s_tuple;
    bej_f_tuple_unpacked_t f_tuple;
    bej_l_tuple_unpacked_t l_tuple;
    void* v_tuple;
}
bej_sflv_tuple_unpacked_t;


*/












#endif // PLDM_RDE_BEJ_H