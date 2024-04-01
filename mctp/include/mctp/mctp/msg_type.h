#ifndef MSG_TYPE_H
#define MSG_TYPE_H

typedef enum __attribute__ ((__packed__)) mctp_msg_type_t
{
    MCTP_MSG_TYPE_CONTROL       = 0x00,
    MCTP_MSG_TYPE_PLDM          = 0x01,
    MCTP_MSG_TYPE_NCSI          = 0x02,
    MCTP_MSG_TYPE_ETHERNET      = 0x03,
    MCTP_MSG_TYPE_NVM_EXPRESS   = 0x04,
    MCTP_MSG_TYPE_VENDOR_PCI    = 0x7E,
    MCTP_MSG_TYPE_VENDOR_IANA   = 0x7F,
}
mctp_msg_type_t;

typedef struct __attribute__ ((__packed__)) mctp_generic_header_t
{
    mctp_msg_type_t type : 7;
    bool integrity_check : 1;
}
mctp_generic_header_t;


#endif // MSG_TYPE_H