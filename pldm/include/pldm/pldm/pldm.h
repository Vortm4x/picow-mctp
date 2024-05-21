#ifndef PLDM_H
#define PLDM_H

#include <pldm/types.h>
#include <pldm/terminus.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


#define PLDM_HEADER_VER 0b00
#define PLDM_TID_UNASSIGNED 0x00
#define PLDM_TID_RESERVED 0xFF


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
    // PLDM TYPE 0 (BASE)
    PLDM_BASE_CMD_SET_TID                               = 0x01,
    PLDM_BASE_CMD_GET_TID                               = 0x02,
    PLDM_BASE_CMD_GET_PLDM_VERSION                      = 0x03,
    PLDM_BASE_CMD_GET_PLDM_TYPE                         = 0x04,
    PLDM_BASE_CMD_GET_PLDM_COMMANDS                     = 0x05,
    PLDM_BASE_CMD_SELECT_PLDM_VERSION                   = 0x06,
    PLDM_BASE_CMD_NEGOTIATE_XFER_PARAMS                 = 0x07,
    PLDM_BASE_CMD_MULTIPART_SEND                        = 0x08,
    PLDM_BASE_CMD_MULTIPART_RECEIVE                     = 0x09,

    // PLDM TYPE 2 (PLATFORM)
    PLDM_PLATFORM_CMD_TERM_PLATFORM_EVENT_MSG           = 0x0A,
    PLDM_PLATFORM_CMD_TERM_POLL_FOR_PLATFORM_EVENT_MSG  = 0x0B,
    PLDM_PLATFORM_CMD_TERM_EVENT_MSG_SUPPORTED          = 0x0C,
    PLDM_PLATFORM_CMD_TERM_EVENT_MSG_BUFF_SIZE          = 0x0D,

    PLDM_PLATFORM_CMD_NUM_SENS_SET_ENABLE               = 0x10,
    PLDM_PLATFORM_CMD_NUM_SENS_GET_READING              = 0x11,
    PLDM_PLATFORM_CMD_NUM_SENS_GET_TRESHOLDS            = 0x12,
    PLDM_PLATFORM_CMD_NUM_SENS_SET_TRESHOLDS            = 0x13,
    PLDM_PLATFORM_CMD_NUM_SENS_RESTORE_TRESHOLDS        = 0x14,
    PLDM_PLATFORM_CMD_NUM_SENS_GET_HYSTERESIS           = 0x15,
    PLDM_PLATFORM_CMD_NUM_SENS_SET_HYSTERESIS           = 0x16,
    PLDM_PLATFORM_CMD_NUM_SENS_INIT                     = 0x17,

    PLDM_PLATFORM_CMD_STATE_SENS_SET_ENABLE             = 0x20,
    PLDM_PLATFORM_CMD_STATE_SENS_GET_READING            = 0x21,
    PLDM_PLATFORM_CMD_STATE_SENS_INIT                   = 0x22,

    PLDM_PLATFORM_CMD_NUM_EFFECT_SET_ENABLE             = 0x30,
    PLDM_PLATFORM_CMD_NUM_EFFECT_SET_VALUE              = 0x31,
    PLDM_PLATFORM_CMD_NUM_EFFECT_GET_VALUE              = 0x32,
    PLDM_PLATFORM_CMD_STATE_EFFECT_SET_ENABLE           = 0x38,
    PLDM_PLATFORM_CMD_STATE_EFFECT_SET_VALUE            = 0x39,
    PLDM_PLATFORM_CMD_STATE_EFFECT_GET_VALUE            = 0x3A,

    PLDM_PLATFORM_CMD_EVENT_LOG_INFO                    = 0x40,
    PLDM_PLATFORM_CMD_EVENT_LOG_ENABLE                  = 0x41,
    PLDM_PLATFORM_CMD_EVENT_LOG_CLEAR                   = 0x42,
    PLDM_PLATFORM_CMD_EVENT_LOG_GET_TIMESTAMP           = 0x43,
    PLDM_PLATFORM_CMD_EVENT_LOG_SET_TIMESTAMP           = 0x44,
    PLDM_PLATFORM_CMD_EVENT_LOG_READ                    = 0x45,
    PLDM_PLATFORM_CMD_EVENT_LOG_GET_POLICY              = 0x46,
    PLDM_PLATFORM_CMD_EVENT_LOG_SET_POLICY              = 0x47,
    PLDM_PLATFORM_CMD_EVENT_LOG_FIND                    = 0x48,

    PLDM_PLATFORM_CMD_PRD_REPO_INFO                     = 0x50,
    PLDM_PLATFORM_CMD_PRD_REPO_GET                      = 0x51,
    PLDM_PLATFORM_CMD_PRD_REPO_FIND                     = 0x52,
    PLDM_PLATFORM_CMD_PRD_REPO_SIG                      = 0x53,
    PLDM_PLATFORM_CMD_PRD_REPO_RUN_INIT_AGET            = 0x58,

    // PLDM TYPE 6 (REDFISH)
    PLDM_REDFISH_CMD_NEGOTIATE_REDFISH_PARAMS           = 0x01,
    PLDM_REDFISH_CMD_NEGOTIATE_MEDIUM_PARAMS            = 0x02,
    PLDM_REDFISH_CMD_GET_SCHEMA_DICT                    = 0x03,
    PLDM_REDFISH_CMD_GET_SCHEMA_URI                     = 0x04,
    PLDM_REDFISH_CMD_GET_RESOURCE_ETAG                  = 0x05,
    PLDM_REDFISH_CMD_GET_OEM_COUNT                      = 0x06,
    PLDM_REDFISH_CMD_GET_OEM_NAME                       = 0x07,
    PLDM_REDFISH_CMD_GET_REGISTRY_COUNT                 = 0x08,
    PLDM_REDFISH_CMD_GET_REGISTRY_DETAILS               = 0x09,
    PLDM_REDFISH_CMD_SELECT_REGISTRY_VERSION            = 0x0A,
    PLDM_REDFISH_CMD_GET_MESSAGE_REGISTRY               = 0x0B,
    PLDM_REDFISH_CMD_GET_SCHEMA_FILE                    = 0x0C,

    PLDM_REDFISH_CMD_RDE_OPERATION_INIT                 = 0x10,
    PLDM_REDFISH_CMD_SUPPLY_REQ_PARAMS                  = 0x11,
    PLDM_REDFISH_CMD_RETRIEVE_RESP_PARAMS               = 0x12,
    PLDM_REDFISH_CMD_RDE_OPERATION_COMPLETE             = 0x13,
    PLDM_REDFISH_CMD_RDE_OPERATION_STATUS               = 0x14,
    PLDM_REDFISH_CMD_RDE_OPERATION_KILL                 = 0x15,
    PLDM_REDFISH_CMD_RDE_OPERATION_ENUMERATE            = 0x16,

    PLDM_REDFISH_CMD_RDE_MULTIPART_SEND                 = 0x30,
    PLDM_REDFISH_CMD_RDE_MULTIPART_RECEIVE              = 0x31,
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
    PLDM_CMD_CC_INVALID_PROTOCOL_TYPE               = 0x80,
    PLDM_CMD_CC_INVALID_SENSOR_ID                   = 0x80,
    PLDM_CMD_CC_INVALID_EFFECTER_ID                 = 0x80,
    PLDM_CMD_CC_INVALID_SEARCH_TYPE                 = 0x80,
    PLDM_CMD_CC_INVALID_FIND_HANDLE                 = 0x80,

    PLDM_CMD_CC_INVALID_TRANSFER_OP_FLAG            = 0x81,
    PLDM_CMD_CC_ENABLE_METHOD_NOT_SUPPORTED         = 0x81,
    PLDM_CMD_CC_UNSUPPORTED_EVENT_FORMAT_VER        = 0x81,
    PLDM_CMD_CC_INVALID_SENSOR_OPERATIONAL_STATE    = 0x81,
    PLDM_CMD_CC_REARM_UNAVAILABLE_IN_PRESENT_STATE  = 0x81,
    PLDM_CMD_CC_UNSUPPORTED_SENSOR_STATE            = 0x81,
    PLDM_CMD_CC_INVALID_STATE_VALUE                 = 0x81,
    PLDM_CMD_CC_INVALID_FIND_OPERATION_FLAG         = 0x81,

    PLDM_CMD_CC_EVENT_ID_NOT_VALID                  = 0x82,
    PLDM_CMD_CC_HEARTBEAT_FREQUENCY_TOO_HIGH        = 0x82,
    PLDM_CMD_CC_UNSUPPORTED_EFFECTER_STATE          = 0x82,
    PLDM_CMD_CC_INVALID_ENTRY_ID                    = 0x82,
    PLDM_CMD_CC_INVALID_RECORD_HANDLE               = 0x82,
    PLDM_CMD_CC_INVALID_PDR_TYPE                    = 0x82,

    PLDM_CMD_CC_NEGOTIATION_INCOMPLETE              = 0x83,
    PLDM_CMD_CC_INVALID_PLDM_TYPE_IN_REQ_DATA       = 0x83,
    PLDM_CMD_CC_EVENT_GENERATION_NOT_SUPPORTED      = 0x83,
    PLDM_CMD_CC_INVALID_RECORD_CHANGE_NUMBER        = 0x83,
    PLDM_CMD_CC_INVALID_PARAMETER_FORMAT_NUMBER     = 0x83,

    PLDM_CMD_CC_INVALID_PLDM_VER_IN_REQ_DATA        = 0x84,
    PLDM_CMD_CC_TRANSFER_TIMEOUT                    = 0x84,
    PLDM_CMD_CC_INVALID_FIND_PARAMETERS             = 0x84,

    PLDM_CMD_CC_REPOSITORY_UPDATE_IN_PROGRESS       = 0x85,

    PLDM_CMD_CC_ERROR_BAD_CHECKSUM                  = 0x80,
    PLDM_CMD_CC_ERROR_CANNOT_CREATE_OPERATION       = 0x81,
    PLDM_CMD_CC_ERROR_NOT_ALLOWED                   = 0x82,
    PLDM_CMD_CC_ERROR_WRONG_LOCATION_TYPE           = 0x83,
    PLDM_CMD_CC_ERROR_OPERATION_ABANDONED           = 0x84,
    PLDM_CMD_CC_ERROR_OPERATION_UNKILLABLE          = 0x85,
    PLDM_CMD_CC_ERROR_OPERATION_EXISTS              = 0x86,
    PLDM_CMD_CC_ERROR_OPERATION_FAILED              = 0x87,
    PLDM_CMD_CC_ERROR_UNEXPECTED                    = 0x88,
    PLDM_CMD_CC_ERROR_UNSUPPORTED                   = 0x89,
    PLDM_CMD_CC_ERROR_UNRECOGNIZED_CUSTOM_HEADER    = 0x90,
    PLDM_CMD_CC_ERROR_ETAG_MATCH                    = 0x91,
    PLDM_CMD_CC_ERROR_NO_SUCH_RESOURCE              = 0x92,
    PLDM_CMD_CC_ETAG_CALCULATION_ONGOING            = 0x93,
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


struct pldm_inst_t;
typedef struct pldm_inst_t pldm_inst_t;

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

uint32_t pldm_get_max_message_len(
    pldm_transport_t* transport
);

void pldm_set_max_message_len(
    pldm_transport_t* transport,
    uint32_t max_message_len
);

void pldm_terminus_register(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport
);

void pldm_terminus_unregister(
    pldm_inst_t* pldm_inst,
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

void pldm_set_redfish_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_message_rx_t redfish_message_rx,
    void* redfish_message_args
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