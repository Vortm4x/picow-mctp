#ifndef CONTROL_H
#define CONTROL_H

#include <mctp/message.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t mctp_eid_t;

enum mctp_msg_type_t;
typedef enum mctp_msg_type_t mctp_msg_type_t;

typedef enum mctp_ctrl_cmd_t
{
    MCTP_CTRL_CMD_RESERVED                      = 0x00,
    MCTP_CTRL_CMD_SET_ENDPOINT_ID               = 0x01,
    MCTP_CTRL_CMD_GET_ENDPOINT_ID               = 0x02,
    MCTP_CTRL_CMD_GET_ENDPOINT_UUID             = 0x03,
    MCTP_CTRL_CMD_GET_VERSION_SUPPORT           = 0x04,
    MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT      = 0x05,
    MCTP_CTRL_CMD_GET_VENDOR_MESSAGE_SUPPORT    = 0x06,
    MCTP_CTRL_CMD_RESOLVE_ENDPOINT_ID           = 0x07,
    MCTP_CTRL_CMD_ALLOCATE_ENDPOINT_IDS         = 0x08,
    MCTP_CTRL_CMD_ROUTING_INFO_UPDATE           = 0x09,
    MCTP_CTRL_CMD_GET_ROUTING_TABLE_ENTRIES     = 0x0A,
    MCTP_CTRL_CMD_PREPARE_ENDPOINT_DISCOVERY    = 0x0B,
    MCTP_CTRL_CMD_ENDPOINT_DISCOVERY            = 0x0C,
    MCTP_CTRL_CMD_DISCOVERY_NOTIFY              = 0x0D,
    MCTP_CTRL_CMD_GET_NETWORK_ID                = 0x0E,
    MCTP_CTRL_CMD_QUERY_HOP                     = 0x0F,
    MCTP_CTRL_CMD_RESOLVE_UUID                  = 0x10,
    MCTP_CTRL_CMD_QUERY_RATE_LIMIT              = 0x11,
    MCTP_CTRL_CMD_REQUEST_TX_RATE_LIMIT         = 0x12,
    MCTP_CTRL_CMD_UPDATE_RATE_LIMIT             = 0x13,
    MCTP_CTRL_CMD_QUERY_SUPPORTED_INTERFACES    = 0x14,
    MCTP_CTRL_CMD_MAX                           = 0x15,    
}
mctp_ctrl_cmd_t;

typedef enum mctp_ctrl_cc_t
{
    MCTP_CTRL_CC_SUCCESS                    = 0x00,
    MCTP_CTRL_CC_ERROR_GENERIC              = 0x01,
    MCTP_CTRL_CC_ERROR_INVALID_DATA         = 0x02,
    MCTP_CTRL_CC_ERROR_INVALID_LENGTH       = 0x03,
    MCTP_CTRL_CC_ERROR_NOT_READY            = 0x04,
    MCTP_CTRL_CC_ERROR_UNSUPPORTED_CMD      = 0x05,
    MCTP_CTRL_CC_MSG_TYPE_NOT_SUPPORTED     = 0x80,
}
mctp_ctrl_cc_t;

typedef struct mctp_ctrl_header_t
{
    uint8_t instance : 5;
    uint8_t : 1;
    bool datagram : 1;
    bool request : 1;
    mctp_ctrl_cmd_t command;
}
mctp_ctrl_header_t;


// Set endpoint id 

typedef enum mctp_set_eid_op_t
{
    MCTP_SET_EID_OP_SET_EID     = 0b00,
    MCTP_SET_EID_OP_FORCE_EID   = 0b01,
    MCTP_SET_EID_OP_RESET_EID   = 0b10,
    MCTP_SET_EID_OP_DISCOVERED  = 0b11,
}
mctp_set_eid_op_t;

typedef struct mctp_req_set_eid_t
{   
    mctp_set_eid_op_t operation : 2;
    uint8_t : 6; 
    mctp_eid_t eid;
}
mctp_req_set_eid_t;


typedef enum mctp_eid_alloc_t
{
    MCTP_EID_ALLOC_POOL_NOT_USED = 0b00,
    MCTP_EID_ALLOC_POOL_REQUIRED = 0b01,
    MCTP_EID_ALLOC_POOL_EXIST    = 0b10,
    MCTP_EID_ALLOC_RESERVED_b11  = 0b11,
}
mctp_eid_alloc_t;

typedef enum mctp_eid_assign_t
{
    MCTP_EID_ASSIGN_ACCEPTED        = 0b00,
    MCTP_EID_ASSIGN_REJECTED        = 0b01,
    MCTP_EID_ASSIGN_RESERVED_b10    = 0b10,
    MCTP_EID_ASSIGN_RESERVED_b11    = 0b11,
}
mctp_eid_assign_t;

typedef struct mctp_resp_set_eid_t
{
    mctp_ctrl_cc_t completion_code;
    mctp_eid_alloc_t eid_alloc_status : 2; 
    uint8_t : 2;
    mctp_eid_assign_t eis_assign_status : 2;
    uint8_t : 2;
    mctp_eid_t endpoint;
    uint8_t eid_pool_size;
}
mctp_resp_set_eid_t;


// Get endpoint id

typedef enum mctp_eid_type_t
{
    MCTP_EID_DYNAMIC            = 0b00,
    MCTP_EID_STATIC_GENERIC     = 0b01,
    MCTP_EID_STATIC_MATCH       = 0b10,
    MCTP_EID_STATIC_NO_MATCH    = 0b11,
}
mctp_eid_type_t;

typedef enum mctp_endpoint_type_t
{
    MCTP_ENDPOINT_SIMPLE        = 0b00,
    MCTP_ENDPOINT_BRIDGE        = 0b01,
    MCTP_ENDPOINT_BUS_OWNER     = 0b01,
    MCTP_ENDPOINT_RESERVED_b10  = 0b10,
    MCTP_ENDPOINT_RESERVED_b11  = 0b11,
}
mctp_endpoint_type_t;


typedef struct mctp_resp_get_eid_t
{
    mctp_ctrl_cc_t completion_code;
    mctp_eid_t endpoint;
    mctp_eid_type_t eid_type : 2;
    uint8_t : 2;
    mctp_endpoint_type_t endpoint_type : 2;
    uint8_t : 2;
    uint8_t medium_info;
}
mctp_resp_get_eid_t;


// Get MCTP version support

#define MCTP_VER_BASE_SPEC 0xFF

typedef struct mctp_req_get_mctp_ver_t
{
    uint8_t message_type;
}
mctp_req_get_mctp_ver_t;

typedef struct mctp_ver_t
{
    uint8_t alpha;
    uint8_t update;
    uint8_t minor;
    uint8_t major;
}
mctp_ver_t;

typedef struct mctp_resp_get_mctp_ver_t
{
    mctp_ctrl_cc_t completion_code;
    uint8_t version_count;
    mctp_ver_t version_entry;
}
mctp_resp_get_mctp_ver_t;


// Get message type support

typedef struct mctp_resp_get_msg_type_t
{
    mctp_ctrl_cc_t completion_code;
    uint8_t msg_type_count;
    mctp_msg_type_t msg_type_entry;
}
mctp_resp_get_msg_type_t;

#endif //CONTROL_H