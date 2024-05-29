#include <pldm/rde/bej.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static bej_encoding_t* bej_encoding_init();

static void bej_collection_destroy(
    bej_collection_t* collection
);

static bej_collection_t* bej_collection_init(
    nnint_t len
);

static void bej_collection_destroy(
    bej_collection_t* collection
);

static bej_value_t* bej_value_init(
    nnint_t size
);

static void bej_value_destroy(
    bej_value_t* value
);


static bej_encoding_t* bej_value_unpack(
    FILE* bej_stream,
    size_t encoding_stream_size,
    uint8_t major_schema_dict[],
    bej_dict_entry_header* entry_headers,
    uint16_t entry_headers_count,
    bool is_array_entry
);

static void bej_sfl_tuple_unpack(
    FILE* bej_stream,
    bej_sfl_tuple_t* sfl_tuple
);

static void bej_nnint_unpack(
    FILE* bej_stream,
    nnint_t* nnint
);

static bej_dict_entry_header* bej_dict_get_child_entry_headers(
    bej_dict_entry_header* parent_entry,
    uint8_t schema_dict[]
);

static bej_dict_entry_header* bej_dict_find_entry_header(
    bej_dict_entry_header entry_headers[],
    nnint_t entry_headers_count,
    nnint_t seq_num
);


bej_encoding_t* bej_encoding_init()
{
    bej_encoding_t* encoding = malloc(sizeof(bej_encoding_t));

    if(encoding != NULL)
    {
        memset(encoding, 0, sizeof(bej_encoding_t));
    }

    return encoding;
}


void bej_encoding_destroy(
    bej_encoding_t* encoding
)
{
    if(encoding == NULL)
    {
        return;
    }

    switch (encoding->format.bej_type)
    {
        case BEJ_TYPE_SET:
        case BEJ_TYPE_ARRAY:
        {
            bej_collection_destroy(encoding->collection);
        }
        break;

        case BEJ_TYPE_NULL:
        case BEJ_TYPE_INTEGER:
        case BEJ_TYPE_ENUM:
        case BEJ_TYPE_STRING:
        case BEJ_TYPE_REAL:
        case BEJ_TYPE_BOOLEAN:
        case BEJ_TYPE_BYTE_STRING:
        {
            bej_value_destroy(encoding->value);
        }
        
        default:
        {}
        break;
    }
}


bej_collection_t* bej_collection_init(
    nnint_t len
)
{
    bej_collection_t* collection = malloc(sizeof(bej_collection_t));

    if(collection != NULL)
    {
        memset(collection, 0, sizeof(bej_collection_t));

        if(len > 0)
        {
            collection->len = len;
            collection->entries = calloc(len, sizeof(bej_encoding_t*));
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

    for(nnint_t i = 0; i < collection->len; ++i)
    {
        bej_encoding_destroy(collection->entries[i]);
    }
}

static bej_value_t* bej_value_init(
    nnint_t size
)
{
    bej_value_t* value = malloc(sizeof(bej_value_t));

    if(value != NULL)
    {
        memset(value, 0, sizeof(bej_value_t));

        if(size > 0)
        {
            value->size = size;
            value->data = calloc(size, sizeof(uint8_t));
        }
    }

    return value;
}

static void bej_value_destroy(
    bej_value_t* value
)
{
    if(value == NULL)
    {
        return;
    }

    free(value->data);
    free(value);
}

static void bej_value_pack(
    FILE* bej_stream,
    bej_encoding_t* encoding
);

static void bej_nnint_pack(
    FILE* bej_stream,
    nnint_t* nnint
);

static bej_encoding_t* bej_value_retrieve(
    bej_encoding_t* encoding,
    FILE* locator_stream,
    size_t locator_stream_size
);

bej_encoding_t* bej_encoding_unpack(
    uint8_t encoding_data[],
    size_t encoding_data_size,
    uint8_t major_schema_dict[]
)
{
    if(encoding_data_size < sizeof(bej_encoding_header_t))
    {
        return NULL;
    }

    FILE* bej_stream = fmemopen(encoding_data, encoding_data_size, "r");
    fseek(bej_stream, 0, SEEK_SET);

    bej_encoding_header_t bej_header = {};
    fread(&bej_header, sizeof(bej_header), 1, bej_stream);

    if((bej_header.version.version != BEJ_VERSION_1_0_0
    && bej_header.version.version != BEJ_VERSION_1_1_0)
    || bej_header.schema_class != RDE_SCHEMA_CLASS_MAJOR)
    {
        fclose(bej_stream);
        return NULL;
    }

    bej_dict_entry_header* entry_header = (bej_dict_entry_header*)(major_schema_dict + sizeof(bej_dict_header_t));

    bej_encoding_t* encoding = bej_value_unpack(
        bej_stream,
        encoding_data_size,
        major_schema_dict,
        entry_header,
        1,
        false
    );

    fclose(bej_stream);

    return encoding;
}


bej_encoding_t* bej_value_unpack(
    FILE* bej_stream,
    size_t encoding_stream_size,
    uint8_t major_schema_dict[],
    bej_dict_entry_header* entry_headers,
    uint16_t entry_headers_count,
    bool is_array_entry
)
{
    bej_sfl_tuple_t sfl_tuple = {};
    bej_sfl_tuple_unpack(bej_stream, &sfl_tuple);

    bej_dict_entry_header* curr_entry_header = NULL;

    if(is_array_entry)
    {
        curr_entry_header = &entry_headers[0];
    }
    else
    {
        curr_entry_header = bej_dict_find_entry_header(
            entry_headers,
            entry_headers_count,
            sfl_tuple.s_tuple.seq
        );
    }

    if(curr_entry_header == NULL)
    {
        return NULL;
    }
    
    if(curr_entry_header->format.bej_type != sfl_tuple.f_tuple.bej_type)
    {
        return NULL;        
    }

    bej_encoding_t* encoding = bej_encoding_init();
    memcpy(&encoding->format, &sfl_tuple.f_tuple, sizeof(bej_f_tuple_t));
    memcpy(&encoding->seq_num, &sfl_tuple.s_tuple, sizeof(bej_s_tuple_t));

    switch (sfl_tuple.f_tuple.bej_type)
    {
        case BEJ_TYPE_SET:
        {
            nnint_t set_len;
            bej_nnint_unpack(bej_stream, &set_len);

            bej_collection_t* set = bej_collection_init(set_len);

            bej_dict_entry_header* child_entry_headers = bej_dict_get_child_entry_headers(
                curr_entry_header,
                major_schema_dict
            );

            for(nnint_t i = 0; i < set->len; ++i)
            {
                set->entries[i] = bej_value_unpack(
                    bej_stream, 
                    encoding_stream_size,
                    major_schema_dict,
                    child_entry_headers,
                    curr_entry_header->child_count,
                    false
                );
            }

            encoding->collection = set;
        }
        break;

        case BEJ_TYPE_ARRAY:
        {
            nnint_t array_len;
            bej_nnint_unpack(bej_stream, &array_len);

            bej_collection_t* array = bej_collection_init(array_len);

            bej_dict_entry_header* child_entry_headers = bej_dict_get_child_entry_headers(
                curr_entry_header, 
                major_schema_dict
            );
            
            for(nnint_t i = 0; i < array->len; ++i)
            {
                array->entries[i] = bej_value_unpack(
                    bej_stream,
                    encoding_stream_size,
                    major_schema_dict,
                    child_entry_headers,
                    1,
                    true
                );
            }

            encoding->collection = array;
        }
        break;

        case BEJ_TYPE_NULL:
        case BEJ_TYPE_INTEGER:
        case BEJ_TYPE_ENUM:
        case BEJ_TYPE_STRING:
        case BEJ_TYPE_REAL:
        case BEJ_TYPE_BOOLEAN:
        case BEJ_TYPE_BYTE_STRING:
        {
            encoding->value = bej_value_init(sfl_tuple.l_tuple);

            if(encoding->value->size > 0)
            {
                fread(encoding->value->data, sfl_tuple.l_tuple, 1, bej_stream);
            }
        }
        break;

        default:
        {
            bej_encoding_destroy(encoding);
            encoding = NULL;
        }
        break;
    }

    return encoding;
}


void bej_sfl_tuple_unpack(
    FILE* bej_stream,
    bej_sfl_tuple_t* sfl_tuple
)
{
    memset(sfl_tuple, 0, sizeof(bej_sfl_tuple_t));

    bej_nnint_unpack(bej_stream, &sfl_tuple->s_tuple.nnint);
    fread(&sfl_tuple->f_tuple, sizeof(bej_f_tuple_t), 1, bej_stream);
    bej_nnint_unpack(bej_stream, &sfl_tuple->l_tuple);
}


void bej_nnint_unpack(
    FILE* bej_stream,
    nnint_t* nnint
)
{
    memset(nnint, 0, sizeof(nnint_t));

    uint8_t len = 0;

    fread(&len, sizeof(uint8_t), 1, bej_stream);
    fread(nnint, sizeof(uint8_t), len, bej_stream);
}


bej_dict_entry_header* bej_dict_get_child_entry_headers(
    bej_dict_entry_header* parent_entry,
    uint8_t schema_dict[]
)
{
    bej_dict_entry_header* child_entry_headers = (bej_dict_entry_header*)(schema_dict + parent_entry->offset);

    return child_entry_headers;
}


bej_dict_entry_header* bej_dict_find_entry_header(
    bej_dict_entry_header entry_headers[],
    nnint_t entry_headers_count,
    nnint_t seq_num
)
{
    for(nnint_t i = 0; i < entry_headers_count; ++i)
    {
        if(entry_headers[i].sequence == seq_num)
        {
            return &entry_headers[i];
        }
    }
    
    return NULL;
}


uint8_t* bej_encoding_pack(
    bej_encoding_t* encoding,
    size_t* data_len
)
{
    uint8_t* data = NULL;
    FILE* bej_stream = open_memstream((char**)(&data), data_len);

    bej_encoding_header_t encoding_header = {
        .schema_class = RDE_SCHEMA_CLASS_MAJOR,
        .version.version = BEJ_VERSION_1_1_0,
    };

    fwrite(&encoding_header, sizeof(bej_encoding_header_t), 1, bej_stream);
    bej_value_pack(bej_stream, encoding);
    fclose(bej_stream);

    return data;
}


void bej_value_pack(
    FILE* bej_stream,
    bej_encoding_t* encoding
)
{
    bej_nnint_pack(bej_stream, &encoding->seq_num.nnint);
    fwrite(&encoding->format, sizeof(bej_f_tuple_t), 1, bej_stream);

    switch (encoding->format.bej_type)
    {
        case BEJ_TYPE_SET:
        case BEJ_TYPE_ARRAY:
        {
            uint8_t* data = NULL;
            size_t data_len = 0;

            FILE* nested_stream = open_memstream((char**)&data, &data_len);
            bej_nnint_pack(nested_stream, &encoding->collection->len);

            for(nnint_t i = 0; i < encoding->collection->len; ++i)
            {
                bej_value_pack(
                    nested_stream,
                    encoding->collection->entries[i]
                );
            }

            fclose(nested_stream);

            nnint_t value_len = data_len;
            bej_nnint_pack(bej_stream, &value_len);
            
            fwrite(data, sizeof(uint8_t), data_len, bej_stream);
        }
        break;

        case BEJ_TYPE_NULL:
        case BEJ_TYPE_INTEGER:
        case BEJ_TYPE_ENUM:
        case BEJ_TYPE_STRING:
        case BEJ_TYPE_REAL:
        case BEJ_TYPE_BOOLEAN:
        case BEJ_TYPE_BYTE_STRING:
        {
            bej_nnint_pack(bej_stream, &encoding->value->size);
            fwrite(encoding->value->data, encoding->value->size, 1, bej_stream);
        }
        break;
    
        default:
        {}
        break;
    }
}


void bej_nnint_pack(
    FILE* bej_stream,
    nnint_t* nnint
)
{
    uint8_t* data = (uint8_t*)nnint;
    uint8_t len = sizeof(nnint_t);

    for(; len > 1; --len)
    {
        if(data[len - 1] != 0)
        {
            break;
        }
    }

    fwrite(&len, sizeof(uint8_t), 1, bej_stream);
    fwrite(data, sizeof(uint8_t), len, bej_stream);
}

bej_encoding_t* bej_encoding_retrieve(
    bej_encoding_t* encoding,
    uint8_t locator_data[],
    size_t locator_data_size
)
{
    FILE* locator_stream = fmemopen(locator_data, locator_data_size, "r");
    fseek(locator_stream, 0, SEEK_SET);

    nnint_t locator_size = 0;
    bej_nnint_unpack(locator_stream, &locator_size);

    if(ftell(locator_stream) + locator_size != locator_data_size)
    {
        return NULL;
    }
    
    bej_encoding_t* node = bej_value_retrieve(
        encoding,
        locator_stream,
        locator_data_size
    );

    fclose(locator_stream);

    return node;
}

bej_encoding_t* bej_value_retrieve(
    bej_encoding_t* encoding,
    FILE* locator_stream,
    size_t locator_stream_size
)
{
    bej_collection_t root = {
        .entries = &encoding,
        .len = 1,
    };

    bej_collection_t* curr_collection = &root;
    bej_encoding_t* curr_entry = encoding;
    bej_s_tuple_t s_tuple = {
        .nnint = 0,
    };

    while ((size_t)ftell(locator_stream) < locator_stream_size)
    {
        switch (curr_entry->format.bej_type)
        {
            case BEJ_TYPE_ARRAY:
            case BEJ_TYPE_SET:
            {
                bej_nnint_unpack(locator_stream, &s_tuple.nnint);

                nnint_t i = 0;

                while (i < curr_collection->len)
                {
                    if(curr_collection->entries[i]->seq_num.seq == s_tuple.seq)
                    {
                        break;
                    }

                    i += 1;
                }

                if(i < curr_collection->len)
                {
                    curr_entry = curr_collection->entries[i];
                    curr_collection = curr_entry->collection;
                }
                else
                {
                    return NULL;
                }
            }
            break;
        
            default:
            {
                return NULL;
            }
        }
    }

    return curr_entry;
}


void bej_collection_update(
    bej_collection_t* encoding,
    bej_collection_t* update
)
{
    for(nnint_t i = 0; i < encoding->len; ++i)
    {
        for(nnint_t j = 0; j < update->len; ++j)
        {
            bej_encoding_update(encoding->entries[i], update->entries[j]);
        }
    }
}


void bej_encoding_update(
    bej_encoding_t* encoding,
    bej_encoding_t* update
)
{
    if(encoding == NULL || update == NULL)
    {
        return;
    }

    if(encoding->seq_num.nnint == update->seq_num.nnint
    && encoding->format.bej_type == update->format.bej_type
    && !encoding->format.read_only)
    {
        switch (encoding->format.bej_type)
        {
            case BEJ_TYPE_ARRAY:
            case BEJ_TYPE_SET:
            {
                bej_collection_update(encoding->collection, update->collection);
            }
            break;

            case BEJ_TYPE_NULL:
            case BEJ_TYPE_INTEGER:
            case BEJ_TYPE_ENUM:
            case BEJ_TYPE_STRING:
            case BEJ_TYPE_REAL:
            case BEJ_TYPE_BOOLEAN:
            case BEJ_TYPE_BYTE_STRING:
            {
                if(update->value->size > 0)
                {
                    bej_encoding_t temp = {};

                    memcpy(&temp, encoding, sizeof(bej_encoding_t));
                    memcpy(encoding, update, sizeof(bej_encoding_t));
                    memcpy(update, &temp, sizeof(bej_encoding_t));
                }
            }
            break;

            default:
            {}
        }
    }
}