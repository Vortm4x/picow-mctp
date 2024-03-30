
#ifndef MESSAGE_H
#define MESSAGE_H

#if defined(MCTP_H) || defined(CONTROL_H)

#include <stdbool.h>
#include <stdint.h>

typedef enum mctp_msg_type_t
{
    MCTP_MSG_TYPE_CONTROL       = 0x00,
    MCTP_MSG_TYPE_PLDM          = 0x01,
    MCTP_MSG_TYPE_NCSI          = 0x02,
    MCTP_MSG_TYPE_ETHERNET      = 0x03,
    MCTP_MSG_TYPE_NVM_EXPRESS   = 0x04,
    MCTP_MSG_TYPE_VENDOR_PCI    = 0x7E,
    MCTP_MSG_TYPE_VENDOR_IANA   = 0x7F,
    MCTP_MSG_TYPE_BASE_SPEC     = 0xFF,
}
mctp_msg_type_t;

typedef struct __attribute__ ((__packed__)) mctp_generic_header_t
{
    uint8_t type : 7;
    bool integrity_check : 1;
}
mctp_generic_header_t;

typedef uint8_t mctp_eid_t;

#endif // MCTP_H || CONTROL_H

#endif // MESSAGE_H