#ifndef PLDM_RDE_BEJ_H
#define PLDM_RDE_BEJ_H

#include <pldm/types.h>
#include <stddef.h>

#define BEJ_VERSION_1_0_0   0xF1F0F000
#define BEJ_VERSION_1_1_0   0xF1F1F000


typedef enum __attribute__ ((__packed__)) bej_type_t
{
    BEJ_TYPE_SET                        = 0b0000,
    BEJ_TYPE_ARRAY                      = 0b0001,
    BEJ_TYPE_NULL                       = 0b0010,
    BEJ_TYPE_INTEGER                    = 0b0011,
    BEJ_TYPE_ENUM                       = 0b0100,
    BEJ_TYPE_STRING                     = 0b0101,
    BEJ_TYPE_REAL                       = 0b0110,
    BEJ_TYPE_BOOLEAN                    = 0b0111,
    BEJ_TYPE_BYTE_STRING                = 0b1000,
    BEJ_TYPE_CHOICE                     = 0b1001,
    BEJ_TYPE_PROPERTY_ANNOTATION        = 0b1010,
    BEJ_TYPE_REGISTRY_ITEM              = 0b1011,
    BEJ_TYPE_RESOURCE_LINK              = 0b1110,
    BEJ_TYPE_RESOURCE_LINK_EXPANSION    = 0b1111,
}
bej_type_t;


typedef struct __attribute__ ((__packed__)) bej_encoding_header_t
{
    ver32_t version;
    uint16_t : 16;
    pldm_rde_schema_class_t schema_class;
}
bej_encoding_header_t;


typedef uint64_t nnint_t;

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

typedef union bej_s_tuple_t
{
    struct __attribute__ ((__packed__))
    {
        bool annotation : 1;
        nnint_t seq : 63;
    };
    
    nnint_t nnint;
}
bej_s_tuple_t;

typedef nnint_t bej_l_tuple_t;

typedef struct bej_sfl_tuple_t
{
    bej_s_tuple_t s_tuple;
    bej_f_tuple_t f_tuple;
    bej_l_tuple_t l_tuple;
}
bej_sfl_tuple_t;

struct bej_encoding_t;
typedef struct bej_encoding_t bej_encoding_t;

typedef struct bej_collection_t
{
    nnint_t len;
    bej_encoding_t** entries;
}
bej_collection_t;

typedef struct bej_value_t
{
    nnint_t size;
    void* data;
}
bej_value_t;


typedef struct bej_encoding_t
{
    bej_f_tuple_t format;
    bej_s_tuple_t seq_num;

    union
    {
        bej_collection_t* collection;
        bej_value_t* value;
    };
}
bej_encoding_t;


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


bej_encoding_t* bej_encoding_unpack(
    uint8_t encoding_data[],
    size_t encoding_data_size,
    uint8_t major_schema_dict[]
);

uint8_t* bej_encoding_pack(
    bej_encoding_t* encoding,
    size_t* data_len
);

void bej_encoding_destroy(
    bej_encoding_t* encoding
);




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
typedef uint64_t nnint_t_t;

typedef struct bej_s_tuple_t
{
    nnint_t_t seq : 63;
    bool annotation : 1;
}
bej_s_tuple_t;

typedef bej_f_tuple_t bej_f_tuple_t;

typedef nnint_t_t bej_l_tuple_t;

struct bej_sflv_tuple_t;
typedef struct bej_sflv_tuple_t bej_sflv_tuple_t;

typedef struct bej_entry_t bej_entry_t;
typedef struct bej_entry_t
{
    bej_sflv_tuple_t* value;
    bej_entry_t* next;
}
bej_entry_t;


typedef void bej_null_t;

typedef int64_t bej_integer_t;

typedef nnint_t_t bej_enum_t;

typedef char bej_string_t;

typedef struct bej_real_t {
    nnint_t_t whole_len;
    bej_integer_t whole;
    nnint_t_t lead_zero_count;
    nnint_t_t fract;
    nnint_t_t exponent_len;
    bej_integer_t exponent;
}
bej_real_t;

typedef bool bej_boolean_t;

typedef uint8_t bej_bytestring_t;


typedef struct bej_entry_t {

}
bej_entry_t;

typedef struct bej_sflv_tuple_t bej_choice_t;

typedef struct bej_sflv_tuple_t bej_choice_t;

typedef struct bej_sflv_tuple_t
{
    bej_s_tuple_t s_tuple;
    bej_f_tuple_t f_tuple;
    bej_l_tuple_t l_tuple;
    void* v_tuple;
}
bej_sflv_tuple_t;


*/












#endif // PLDM_RDE_BEJ_H