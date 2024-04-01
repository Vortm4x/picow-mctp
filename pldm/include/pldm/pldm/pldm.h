#ifndef PLDM_H
#define PLDM_H

#include <stdint.h>
#include <stdbool.h>
#include <pico/types.h>

#define PLDM_HEADER_VER 0b00
#define PLDM_TID_UNASSIGNED 0x00
#define PLDM_TID_RESERVED 0xFF

typedef uint8_t pldm_tid_t;

typedef enum __attribute__ ((__packed__)) pldm_type_t
{
    PLDM_TYPE_BASE          = 0,
    PLDM_TYPE_SMBIOS        = 1,
    PLDM_TYPE_MONITOR       = 2,
    PLDM_TYPE_BIOS_CTRL     = 3,
    PLDM_TYPE_FRU           = 4,
    PLDM_TYPE_FIRMWARE_UPD  = 5,
    PLDM_TYPE_REDFISH       = 6,
}
pldm_type_t;

typedef enum __attribute__ ((__packed__)) pldm_base_cmd_t
{
    PLDM_BASE_CMD_SET_TID                   = 0x01,
    PLDM_BASE_CMD_GET_TID                   = 0x02,
    PLDM_BASE_CMD_GET_PLDM_VERSION          = 0x03,
    PLDM_BASE_CMD_GET_PLDM_TYPE             = 0x04,
    PLDM_BASE_CMD_GET_PLDM_COMMANDS         = 0x05,
    PLDM_BASE_CMD_SELECT_PLDM_VERSION       = 0x06,
    PLDM_BASE_CMD_NEGOTIATE_XFER_PARAMS     = 0x07,
    PLDM_BASE_CMD_MULTIPART_SEND            = 0x08,
    PLDM_BASE_CMD_MULTIPART_RECEIVE         = 0x09,
}
pldm_base_cmd_t;

typedef enum __attribute__ ((__packed__)) pldm_base_cc_t
{
    PLDM_BASE_CC_SUCCESS                        = 0x00,
    PLDM_BASE_CC_ERROR_GENERIC                  = 0x01,
    PLDM_BASE_CC_ERROR_INVALID_DATA             = 0x02,
    PLDM_BASE_CC_ERROR_INVALID_LENGTH           = 0x03,
    PLDM_BASE_CC_ERROR_NOT_READY                = 0x04,
    PLDM_BASE_CC_ERROR_UNSUPPORTED_CMD          = 0x05,
    PLDM_BASE_CC_INVALID_PLDM_TYPE              = 0x20,
    PLDM_BASE_CC_INVALID_DATA_TRANSFER_HANDLE   = 0x80,
    PLDM_BASE_CC_INVALID_TRANSFER_OP_FLAG       = 0x81,
    PLDM_BASE_CC_INVALID_PLDM_TYPE_IN_REQ_DATA  = 0x83,
    PLDM_BASE_CC_INVALID_PLDM_VER_IN_REQ_DATA   = 0x84,
}
pldm_base_cc_t;

typedef struct __attribute__ ((__packed__)) pldm_base_header_t
{
    uint8_t command     : 8;
    uint8_t pldm_type   : 6;
    uint8_t version     : 2;
    uint8_t instance    : 5;
    uint8_t             : 1;
    bool datagram       : 1;
    bool request        : 1;
}
pldm_base_header_t;




#endif // PLDM_H