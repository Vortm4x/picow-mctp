#include <uuid.h>
#include <pico/rand.h>

#define UUID_RFC_VER4 0b0100
#define UUID_RFC_VARIANT 0b10


void uuid_gen(mctp_uuid_t* uuid)
{
    uint64_t* uuid_mapped = (uint64_t*)uuid->bytes;
    uuid_mapped[0] = get_rand_64();
    uuid_mapped[1] = get_rand_64();

    uuid->variant = UUID_RFC_VARIANT;
    uuid->version = UUID_RFC_VER4;
}