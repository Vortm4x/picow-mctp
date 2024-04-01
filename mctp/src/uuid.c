#include <uuid.h>
#include <pico/rand.h>
#include <string.h>

#define UUID_RFC_VER4 0b0100
#define UUID_RFC_VARIANT 0b10


void uuid_gen(mctp_uuid_t* uuid)
{
    rng_128_t uuid_rng = {};
    get_rand_128(&uuid_rng);
    memcpy(uuid->bytes, &uuid_rng, sizeof(uuid->bytes));
    
    uuid->variant = UUID_RFC_VARIANT;
    uuid->version = UUID_RFC_VER4;
}