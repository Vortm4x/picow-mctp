#ifndef PLDM_H
#define PLDM_H

#include <stdint.h>
#include <stdbool.h>
#include <pico/types.h>

#define PLDM_HEADER_VER 0b00
#define PLDM_TID_UNASSIGNED 0x00
#define PLDM_TID_RESERVED 0xFF

typedef struct __attribute__ ((__packed__)) real32_t
{
    uint32_t mantissa   : 23;
    uint32_t exponent   : 8;
    bool sign           : 1;
}
real32_t;

typedef struct __attribute__ ((__packed__)) real64_t
{
    uint64_t mantissa   : 52;
    uint64_t exponent   : 11;
    bool sign           : 1;
}
real64_t;

typedef struct __attribute__ ((__packed__)) utf8_str_t
{
    uint16_t len;
    uint8_t data[];
}
utf8_str_t;

typedef union __attribute__ ((__packed__)) ver32_t
{
    struct
    {
        uint8_t alpha;
        uint8_t update;
        uint8_t minor;
        uint8_t major;
    };
    
    uint32_t version;
}
ver32_t;


typedef enum __attribute__ ((__packed__)) time_resolution_t
{
    TIME_RESOLUTION_MICRO_SEC       = 0,
    TIME_RESOLUTION_10_MICRO_SEC    = 1,
    TIME_RESOLUTION_100_MICRO_SEC   = 2,
    TIME_RESOLUTION_MILLI_SEC       = 3,
    TIME_RESOLUTION_10_MILLI_SEC    = 4,
    TIME_RESOLUTION_100_MILLI_SEC   = 5,
    TIME_RESOLUTION_SEC             = 6,
    TIME_RESOLUTION_10_SEC          = 7,
    TIME_RESOLUTION_MIN             = 8,
    TIME_RESOLUTION_10_MIN          = 9,
    TIME_RESOLUTION_HOUR            = 10,
    TIME_RESOLUTION_DAY             = 11,
    TIME_RESOLUTION_MONTH           = 12,
    TIME_RESOLUTION_YEAR            = 13,
    TIME_RESOLUTION_NULL            = 15,
}
time_resolution_t;

typedef enum __attribute__ ((__packed__)) utc_resolution_t
{
    UTC_RESOLUTION_UNSPEC   = 0,
    UTC_RESOLUTION_MIN      = 1,
    UTC_RESOLUTION_10_MIN   = 2,
    UTC_RESOLUTION_HOUR     = 3,
}
utc_resolution_t;

typedef struct __attribute__ ((__packed__)) timestamp104_t
{
    int16_t utc_offset;
    uint32_t micro_sec : 24;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
    time_resolution_t time_res : 4;
    utc_resolution_t utc_res : 4;
}
timestamp104_t;



typedef enum __attribute__ ((__packed__)) pldm_type_t
{
    PLDM_TYPE_BASE              = 0,
    PLDM_TYPE_SMBIOS            = 1,
    PLDM_TYPE_PLATFORM          = 2,
    PLDM_TYPE_BIOS_CTRL         = 3,
    PLDM_TYPE_FRU               = 4,
    PLDM_TYPE_FIRMWARE_UPD      = 5,
    PLDM_TYPE_REDFISH           = 6,
}
pldm_type_t;

typedef enum __attribute__ ((__packed__)) pldm_cmd_t
{
    PLDM_BASE_CMD_SET_TID                       = 0x01,
    PLDM_BASE_CMD_GET_TID                       = 0x02,
    PLDM_BASE_CMD_GET_PLDM_VERSION              = 0x03,
    PLDM_BASE_CMD_GET_PLDM_TYPE                 = 0x04,
    PLDM_BASE_CMD_GET_PLDM_COMMANDS             = 0x05,
    PLDM_BASE_CMD_SELECT_PLDM_VERSION           = 0x06,
    PLDM_BASE_CMD_NEGOTIATE_XFER_PARAMS         = 0x07,
    PLDM_BASE_CMD_MULTIPART_SEND                = 0x08,
    PLDM_BASE_CMD_MULTIPART_RECEIVE             = 0x09,

    PLDM_PLATFORM_CMD_TERM_PLATFORM_EVENT_MSG           = 0x0A,
    PLDM_PLATFORM_CMD_TERM_POLL_FOR_PLATFORM_EVENT_MSG  = 0x0B,
    PLDM_PLATFORM_CMD_TERM_EVENT_MSG_SUPPORTED          = 0x0C,
    PLDM_PLATFORM_CMD_TERM_EVENT_MSG_BUFF_SIZE          = 0x0D,

    PLDM_PLATFORM_CMD_NUM_SENS_SET_ENABLE           = 0x10,
    PLDM_PLATFORM_CMD_NUM_SENS_GET_READING          = 0x11,
    PLDM_PLATFORM_CMD_NUM_SENS_GET_TRESHOLDS        = 0x12,
    PLDM_PLATFORM_CMD_NUM_SENS_SET_TRESHOLDS        = 0x13,
    PLDM_PLATFORM_CMD_NUM_SENS_RESTORE_TRESHOLDS    = 0x14,
    PLDM_PLATFORM_CMD_NUM_SENS_GET_HYSTERESIS       = 0x15,
    PLDM_PLATFORM_CMD_NUM_SENS_SET_HYSTERESIS       = 0x16,
    PLDM_PLATFORM_CMD_NUM_SENS_INIT                 = 0x17,

    PLDM_PLATFORM_CMD_PRD_REPO_INFO             = 0x50,
    PLDM_PLATFORM_CMD_PRD_REPO_GET              = 0x51,
    PLDM_PLATFORM_CMD_PRD_REPO_FIND             = 0x52,
    PLDM_PLATFORM_CMD_PRD_REPO_SIG              = 0x53,
    PLDM_PLATFORM_CMD_PRD_REPO_RUN_INIT_AGET    = 0x58,
}
pldm_cmd_t;

typedef enum __attribute__ ((__packed__)) pldm_cmd_cc_t
{
    PLDM_CMD_CC_SUCCESS                             = 0x00,
    PLDM_CMD_CC_ERROR_GENERIC                       = 0x01,
    PLDM_CMD_CC_ERROR_INVALID_DATA                  = 0x02,
    PLDM_CMD_CC_ERROR_INVALID_LENGTH                = 0x03,
    PLDM_CMD_CC_ERROR_NOT_READY                     = 0x04,
    PLDM_CMD_CC_ERROR_UNSUPPORTED_CMD               = 0x05,
    PLDM_CMD_CC_INVALID_PLDM_TYPE                   = 0x20,
    PLDM_CMD_CC_INVALID_DATA_TRANSFER_HANDLE        = 0x80,
    PLDM_CMD_CC_INVALID_SENSOR_ID                   = 0x80,
    PLDM_CMD_CC_INVALID_TRANSFER_OP_FLAG            = 0x81,
    PLDM_CMD_CC_REARM_UNAVAILABLE_IN_PRESENT_STATE  = 0x81,
    PLDM_CMD_CC_INVALID_RECORD_HANDLE               = 0x82,
    PLDM_CMD_CC_INVALID_PLDM_TYPE_IN_REQ_DATA       = 0x83,
    PLDM_CMD_CC_INVALID_RECORD_CHANGE_NUMBER        = 0x83,
    PLDM_CMD_CC_INVALID_PLDM_VER_IN_REQ_DATA        = 0x84,
    PLDM_CMD_CC_TRANSFER_TIMEOUT                    = 0x84,
    PLDM_CMD_CC_REPO_UPD_IN_PROGRESS                = 0x85,
}
pldm_cmd_cc_t;

typedef struct __attribute__ ((__packed__)) pldm_base_header_t
{
    uint8_t instance        : 5;
    uint8_t                 : 1;
    bool datagram           : 1;
    bool request            : 1;
    pldm_type_t pldm_type   : 6;
    uint8_t version         : 2;
    pldm_cmd_t command;
}
pldm_base_header_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_error_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
}
pldm_resp_error_t;

typedef struct __attribute__ ((__packed__)) pldm_xfer_pos_t
{
    bool is_start   : 1;
    bool is_middle  : 1;
    bool is_end     : 1;
    uint8_t         : 5;
}
pldm_xfer_pos_t;


struct pldm_inst_t;
typedef struct pldm_inst_t pldm_inst_t;

struct pldm_transport_t;
typedef struct pldm_transport_t pldm_transport_t;

typedef uint8_t pldm_tid_t;

typedef void (*pldm_message_rx_t)(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
);

pldm_inst_t* pldm_init();

void pldm_destroy(
    pldm_inst_t* pldm_inst
);

void pldm_register_terminus(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport
);

void pldm_unregister_terminus(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport   
);

pldm_tid_t pldm_get_terminus_id(
    pldm_transport_t* transport
);

void pldm_set_base_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_message_rx_t base_message_rx,
    void* base_message_args
);

void pldm_set_platform_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_message_rx_t platform_message_rx,
    void* platform_message_args
);

void pldm_message_tx(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
);

void pldm_resp_error_tx(
    pldm_transport_t* transport,
    pldm_base_header_t* base_header,
    pldm_cmd_cc_t error_code
);

#endif // PLDM_H