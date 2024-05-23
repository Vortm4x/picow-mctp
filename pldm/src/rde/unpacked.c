#include <pldm/rde/unpacked.h>
#include <pldm/rde/bej.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef uint64_t nnint_t_unpacked_t;

typedef struct bej_s_tuple_unpacked_t
{
    nnint_t_unpacked_t seq : 63;
    bool annotation : 1;
}
bej_s_tuple_unpacked_t;

typedef bej_f_tuple_t bej_f_tuple_unpacked_t;

typedef nnint_t_unpacked_t bej_l_tuple_unpacked_t;

typedef struct bej_sfl_tuple_unpacked_t
{
    bej_s_tuple_unpacked_t s_tuple;
    bej_f_tuple_unpacked_t f_tuple;
    bej_l_tuple_unpacked_t l_tuple;
}
bej_sfl_tuple_unpacked_t;

typedef struct bej_encoding_unpacked_t
{
    bej_sfl_tuple_unpacked_t sfl_tuple;
    void* v_tuple;
}
bej_encoding_unpacked_t;


typedef struct bej_collection_t
{
    nnint_t_unpacked_t len;
    bej_encoding_unpacked_t** entries;
}
bej_collection_t;


static bej_encoding_unpacked_t* bej_encoding_init();

static void bej_collection_destroy(
    bej_collection_t* collection
);

static bej_collection_t* bej_collection_init(
    nnint_t_unpacked_t len
);

static void bej_collection_destroy(
    bej_collection_t* collection
);

static bej_encoding_unpacked_t* bej_value_unpack(
    uint8_t encoding_data[],
    size_t* encoding_offset,
    size_t encoding_stream_size,
    uint8_t major_schema_dict[],
    bej_dict_entry_header* entry_headers,
    uint16_t entry_headers_count,
    bool is_array_entry
);

static void bej_sfl_tuple_unpack(
    uint8_t encoding_data[],
    size_t* encoding_offset,
    bej_sfl_tuple_unpacked_t* sfl_tuple
);

static void bej_nnint_unpack(
    uint8_t encoding_data[],
    size_t* encoding_offset,
    nnint_t_unpacked_t* nnint
);

static void bej_data_unpack(
    uint8_t encoding_data[],
    size_t* encoding_offset,
    void* data,
    size_t data_len
);

static bej_dict_entry_header* bej_dict_get_child_entry_headers(
    bej_dict_entry_header* parent_entry,
    uint8_t schema_dict[]
);


bej_encoding_unpacked_t* bej_encoding_init()
{
    bej_encoding_unpacked_t* encoding = malloc(sizeof(bej_encoding_unpacked_t));

    if(encoding != NULL)
    {
        memset(encoding, 0, sizeof(bej_encoding_unpacked_t));
    }

    return encoding;
}


void bej_encoding_destroy(
    bej_encoding_unpacked_t* encoding
)
{
    if(encoding == NULL)
    {
        return;
    }

    switch (encoding->sfl_tuple.f_tuple.bej_type)
    {
        case BEJ_TYPE_SET:
        case BEJ_TYPE_ARRAY:
        {
            bej_collection_destroy(encoding->v_tuple);
        }
        break;

        case BEJ_TYPE_INTEGER:
        case BEJ_TYPE_ENUM:
        case BEJ_TYPE_STRING:
        case BEJ_TYPE_REAL:
        case BEJ_TYPE_BOOLEAN:
        case BEJ_TYPE_BYTE_STRING:
        {
            free(encoding->v_tuple);
        }
    
        default:
        {}
        break;
    }
}


bej_collection_t* bej_collection_init(
    nnint_t_unpacked_t len
)
{
    bej_collection_t* collection = malloc(sizeof(bej_collection_t));

    if(collection != NULL)
    {
        memset(collection, 0, sizeof(bej_collection_t));

        if(len > 0)
        {
            collection->len = len;
            collection->entries = calloc(len, sizeof(bej_encoding_unpacked_t*));
        }
    }

    return collection;
}


void bej_collection_destroy(
    bej_collection_t* collection
)
{
    if(collection == NULL)
    {
        return;
    }

    for(nnint_t_unpacked_t i = 0; i < collection->len; ++i)
    {
        bej_encoding_destroy(collection->entries[i]);
    }
}


bej_encoding_unpacked_t* bej_encoding_unpack(
    uint8_t encoding_data[],
    size_t encoding_data_size,
    uint8_t major_schema_dict[]
)
{

    bej_encoding_header_t* bej_header = (bej_encoding_header_t*)encoding_data;
    size_t encoding_offset = sizeof(bej_encoding_header_t);

    if(bej_header->version.version != BEJ_VERSION_1_0_0
    && bej_header->version.version != BEJ_VERSION_1_1_0)
    {
        return NULL;
    }

    if(bej_header->schema_class != RDE_SCHEMA_CLASS_MAJOR)
    {
        return NULL;   
    }

    bej_dict_entry_header* entry_header = (bej_dict_entry_header*)(major_schema_dict + sizeof(bej_dict_header_t));

    bej_encoding_unpacked_t* encoding = bej_value_unpack(
        encoding_data,
        &encoding_offset,
        encoding_data_size,
        major_schema_dict,
        entry_header,
        1,
        false
    );

    return encoding;
}


bej_encoding_unpacked_t* bej_value_unpack(
    uint8_t encoding_data[],
    size_t* encoding_offset,
    size_t encoding_stream_size,
    uint8_t major_schema_dict[],
    bej_dict_entry_header* entry_headers,
    uint16_t entry_headers_count,
    bool is_array_entry
)
{
    bej_sfl_tuple_unpacked_t sfl_tuple = {};

    bej_encoding_unpacked_t* encoding = bej_encoding_init();
    bej_sfl_tuple_unpack(encoding_data, encoding_offset, &sfl_tuple);

    bej_dict_entry_header* value_entry = NULL;

    if(is_array_entry)
    {
        value_entry = &entry_headers[0];
    }
    else
    {
        if(sfl_tuple.s_tuple.seq < entry_headers_count)
        {
            value_entry = &entry_headers[sfl_tuple.s_tuple.seq];
        }
    }

    if(value_entry == NULL)
    {
        return NULL;
    }
    
    if(memcmp(&value_entry, &sfl_tuple.f_tuple, sizeof(bej_f_tuple_unpacked_t)) != 0)
    {
        return NULL;        
    }

    switch (sfl_tuple.f_tuple.bej_type)
    {
        case BEJ_TYPE_SET:
        {
            nnint_t_unpacked_t set_len;
            bej_nnint_unpack(encoding_data, encoding_offset, &set_len);

            bej_collection_t* set = bej_collection_init(set_len);

            bej_dict_entry_header* child_entry_headers = bej_dict_get_child_entry_headers(
                value_entry,
                major_schema_dict
            );

            for(nnint_t_unpacked_t j = 0; j < set->len; ++j)
            {
                set->entries[j] = bej_value_unpack(
                    encoding_data, 
                    encoding_offset, 
                    encoding_stream_size,
                    major_schema_dict,
                    child_entry_headers,
                    value_entry->child_count,
                    false
                );
            }

            encoding->v_tuple = set;
        }
        break;

        case BEJ_TYPE_ARRAY:
        {
            nnint_t_unpacked_t array_len;
            bej_nnint_unpack(encoding_data, encoding_offset, &array_len);

            bej_collection_t* array = bej_collection_init(array_len);

            bej_dict_entry_header* child_entry_headers = bej_dict_get_child_entry_headers(
                value_entry, 
                major_schema_dict
            );
            
            for(nnint_t_unpacked_t j = 0; j < array->len; ++j)
            {
                array->entries[j] = bej_value_unpack(
                    encoding_data, encoding_offset,
                    encoding_stream_size,
                    major_schema_dict,
                    child_entry_headers,
                    1,
                    true
                );
            }

            encoding->v_tuple = array;
        }
        break;

        case BEJ_TYPE_NULL:
        {
            encoding->v_tuple = NULL;
        }
        break;

        case BEJ_TYPE_INTEGER:
        case BEJ_TYPE_ENUM:
        case BEJ_TYPE_STRING:
        case BEJ_TYPE_REAL:
        case BEJ_TYPE_BOOLEAN:
        case BEJ_TYPE_BYTE_STRING:
        {
            if(encoding->sfl_tuple.l_tuple > 0)
            {
                encoding->v_tuple = malloc(encoding->sfl_tuple.l_tuple);

                bej_data_unpack(
                    encoding_data, 
                    encoding_offset, 
                    encoding->v_tuple,
                    encoding->sfl_tuple.l_tuple
                );
            }
            else
            {
                encoding->v_tuple = NULL;
            }
        }
        break;

        default:
        {
            encoding = NULL;
        }
        break;
    }

    return encoding;
}


void bej_sfl_tuple_unpack(
    uint8_t encoding_data[],
    size_t* encoding_offset,
    bej_sfl_tuple_unpacked_t* sfl_tuple
)
{
    memset(sfl_tuple, 0, sizeof(bej_sfl_tuple_unpacked_t));

    bej_nnint_unpack(
        encoding_data, 
        encoding_offset, 
        (nnint_t_unpacked_t*)&sfl_tuple->s_tuple
    );

    bej_data_unpack(
        encoding_data, 
        encoding_offset, 
        &sfl_tuple->f_tuple,
        sizeof(bej_f_tuple_t)
    );

    bej_nnint_unpack(
        encoding_data, 
        encoding_offset, 
        (nnint_t_unpacked_t*)&sfl_tuple->l_tuple
    );
}


void bej_nnint_unpack(
    uint8_t encoding_data[],
    size_t* encoding_offset,
    nnint_t_unpacked_t* nnint
)
{
    memset(nnint, 0, sizeof(nnint_t_unpacked_t));

    uint8_t len = 0;

    bej_data_unpack(
        encoding_data, 
        encoding_offset, 
        &len,
        sizeof(uint8_t)
    );

    bej_data_unpack(
        encoding_data, 
        encoding_offset, 
        nnint,
        len
    );
}


void bej_data_unpack(
    uint8_t encoding_data[],
    size_t* encoding_offset,
    void* data,
    size_t data_len
)
{                
    memcpy(data, &encoding_data[*encoding_offset], data_len);
    *encoding_offset += data_len;
}


bej_dict_entry_header* bej_dict_get_child_entry_headers(
    bej_dict_entry_header* parent_entry,
    uint8_t schema_dict[]
)
{
    bej_dict_entry_header* child_entry_headers = (bej_dict_entry_header*)(schema_dict + parent_entry->offset);

    return child_entry_headers;
}


/*
bej_sflv_tuple_t* bej_sflv_tuple_init(
    uint8_t* encoding_data
)
{
    bej_sflv_tuple_t* sflv_tuple = malloc(sizeof(bej_sflv_tuple_t));

    if(sflv_tuple != NULL)
    {
        memset(sflv_tuple, 0, sizeof(bej_sflv_tuple_t));

        bej_s_tuple_t* s_tuple = (bej_s_tuple_t*)(encoding_data);
        bej_f_tuple_t* f_tuple = (bej_f_tuple_t*)(s_tuple->seq.data + s_tuple->seq.len);
        bej_l_tuple_t* l_tuple = (bej_l_tuple_t*)(f_tuple + 1);
        uint8_t* v_tuple = (uint8_t*)(l_tuple->len.data + l_tuple->len.len);



    }

    return sflv_tuple;
}

bej_sflv_tuple_t* bej_sflv_tuple_destroy(
    bej_sflv_tuple_t* sflv_tuple
)
{

}
*/

// bej_sflv_tuple_t* bej_encoding_unpack(
//     uint8_t encoding_data[]
// )
// {
//     bej_s_tuple_t* s_tuple = (bej_s_tuple_t*)(encoding_data);
//     bej_f_tuple_t* f_tuple = (bej_f_tuple_t*)(s_tuple->seq.data + s_tuple->seq.len);
//     bej_l_tuple_t* l_tuple = (bej_l_tuple_t*)(f_tuple + 1);
//     uint8_t* v_tuple = (uint8_t*)(l_tuple->len.data + l_tuple->len.len);
// }
