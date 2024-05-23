#ifndef PLDM_RDE_UNPACK
#define PLDM_RDE_UNPACK

#include <stddef.h>
#include <stdint.h>

struct bej_encoding_unpacked_t;
typedef struct bej_encoding_unpacked_t bej_encoding_unpacked_t;


bej_encoding_unpacked_t* bej_encoding_unpack(
    uint8_t encoding_data[],
    size_t encoding_data_size,
    uint8_t major_schema_dict[]
);

void bej_encoding_destroy(
    bej_encoding_unpacked_t* encoding
);


#endif // PLDM_RDE_UNPACK