#ifndef CONTROL_H
#define CONTROL_H

#include <mctp/mctp.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t mctp_eid_t;

enum mctp_msg_type_t;
typedef enum mctp_msg_type_t mctp_msg_type_t;

typedef enum __attribute__ ((__packed__)) mctp_ctrl_cmd_t
{
    MCTP_CTRL_CMD_RESERVED                          = 0x00,
    MCTP_CTRL_CMD_SET_ENDPOINT_ID                   = 0x01,
    MCTP_CTRL_CMD_GET_ENDPOINT_ID                   = 0x02,
    MCTP_CTRL_CMD_GET_ENDPOINT_UUID                 = 0x03,
    MCTP_CTRL_CMD_GET_VERSION_SUPPORT               = 0x04,
    MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT          = 0x05,
    MCTP_CTRL_CMD_GET_VENDOR_MESSAGE_SUPPORT        = 0x06,
    MCTP_CTRL_CMD_RESOLVE_ENDPOINT_ID               = 0x07,
    MCTP_CTRL_CMD_ALLOCATE_ENDPOINT_IDS             = 0x08,
    MCTP_CTRL_CMD_ROUTING_INFO_UPDATE               = 0x09,
    MCTP_CTRL_CMD_GET_ROUTING_TABLE_ENTRIES         = 0x0A,
    MCTP_CTRL_CMD_PREPARE_FOR_ENDPOINT_DISCOVERY    = 0x0B,
    MCTP_CTRL_CMD_ENDPOINT_DISCOVERY                = 0x0C,
    MCTP_CTRL_CMD_DISCOVERY_NOTIFY                  = 0x0D,
    MCTP_CTRL_CMD_GET_NETWORK_ID                    = 0x0E,
    MCTP_CTRL_CMD_QUERY_HOP                         = 0x0F,
    MCTP_CTRL_CMD_RESOLVE_UUID                      = 0x10,
    MCTP_CTRL_CMD_QUERY_RATE_LIMIT                  = 0x11,
    MCTP_CTRL_CMD_REQUEST_TX_RATE_LIMIT             = 0x12,
    MCTP_CTRL_CMD_UPDATE_RATE_LIMIT                 = 0x13,
    MCTP_CTRL_CMD_QUERY_SUPPORTED_INTERFACES        = 0x14,
    MCTP_CTRL_CMD_MAX                               = 0x15,    
}
mctp_ctrl_cmd_t;

typedef enum __attribute__ ((__packed__)) mctp_ctrl_cc_t
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

typedef struct __attribute__ ((__packed__)) mctp_ctrl_header_t
{
    mctp_generic_header_t base;
    uint8_t instance    : 5;
    uint8_t             : 1;
    bool datagram       : 1;
    bool request        : 1;
    mctp_ctrl_cmd_t command;
}
mctp_ctrl_header_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_error_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
}
mctp_resp_error_t;


// MCTP_CTRL_CMD_SET_ENDPOINT_ID

typedef enum __attribute__ ((__packed__)) mctp_set_eid_op_t
{
    MCTP_SET_EID_OP_SET_EID     = 0b00,
    MCTP_SET_EID_OP_FORCE_EID   = 0b01,
    MCTP_SET_EID_OP_RESET_EID   = 0b10,
    MCTP_SET_EID_OP_DISCOVERED  = 0b11,
}
mctp_set_eid_op_t;

typedef enum __attribute__ ((__packed__)) mctp_eid_alloc_status_t
{
    MCTP_EID_ALLOC_STATUS_POOL_NOT_USED = 0b00,
    MCTP_EID_ALLOC_STATUS_POOL_REQUIRED = 0b01,
    MCTP_EID_ALLOC_STATUS_POOL_EXIST    = 0b10,
    MCTP_EID_ALLOC_STATUS_RESERVED_b11  = 0b11,
}
mctp_eid_alloc_status_t;

typedef enum __attribute__ ((__packed__)) mctp_eid_assign_status_t
{
    MCTP_EID_ASSIGN_STATUS_ACCEPTED        = 0b00,
    MCTP_EID_ASSIGN_STATUS_REJECTED        = 0b01,
    MCTP_EID_ASSIGN_STATUS_RESERVED_b10    = 0b10,
    MCTP_EID_ASSIGN_STATUS_RESERVED_b11    = 0b11,
}
mctp_eid_assign_status_t;

typedef struct __attribute__ ((__packed__)) mctp_req_set_endpoint_id_t
{
    mctp_ctrl_header_t header;
    mctp_set_eid_op_t operation : 2;
    uint8_t                     : 6;
    mctp_eid_t eid;
}
mctp_req_set_endpoint_id_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_set_endpoint_id_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    mctp_eid_alloc_status_t eid_alloc_status    : 2; 
    uint8_t                                     : 2;
    mctp_eid_assign_status_t eid_assign_status  : 2;
    uint8_t                                     : 2;
    mctp_eid_t eid_setting;
    uint8_t eid_pool_size;
}
mctp_resp_set_endpoint_id_t;



// MCTP_CTRL_CMD_GET_ENDPOINT_ID

typedef enum __attribute__ ((__packed__)) mctp_eid_type_t
{
    MCTP_EID_DYNAMIC            = 0b00,
    MCTP_EID_STATIC_GENERIC     = 0b01,
    MCTP_EID_STATIC_MATCH       = 0b10,
    MCTP_EID_STATIC_NO_MATCH    = 0b11,
}
mctp_eid_type_t;

typedef enum __attribute__ ((__packed__)) mctp_endpoint_type_t
{
    MCTP_ENDPOINT_SIMPLE        = 0b00,
    MCTP_ENDPOINT_BRIDGE        = 0b01,
    MCTP_ENDPOINT_BUS_OWNER     = 0b01,
    MCTP_ENDPOINT_RESERVED_b10  = 0b10,
    MCTP_ENDPOINT_RESERVED_b11  = 0b11,
}
mctp_endpoint_type_t;


typedef struct __attribute__ ((__packed__)) mctp_req_get_endpoint_id_t
{
    mctp_ctrl_header_t header;
}
mctp_req_get_endpoint_id_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_get_endpoint_id_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    mctp_eid_t eid;
    mctp_eid_type_t eid_type            : 2;
    uint8_t                             : 2;
    mctp_endpoint_type_t endpoint_type  : 2;
    uint8_t                             : 2;
    uint8_t medium_info;
}
mctp_resp_get_endpoint_id_t;



// MCTP_CTRL_CMD_GET_ENDPOINT_UUID

typedef struct __attribute__ ((__packed__)) mctp_req_get_endpoint_uuid_t
{
    mctp_ctrl_header_t header;   
}
mctp_req_get_endpoint_uuid_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_get_endpoint_uuid_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    mctp_uuid_t uuid;
}
mctp_resp_get_endpoint_uuid_t;



// MCTP_CTRL_CMD_GET_VERSION_SUPPORT

typedef union __attribute__ ((__packed__)) mctp_ver_t
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
mctp_ver_t;


typedef struct __attribute__ ((__packed__)) mctp_req_get_mctp_ver_t
{
    mctp_ctrl_header_t header;
    mctp_msg_type_t message_type;
}
mctp_req_get_mctp_ver_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_get_mctp_ver_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    uint8_t version_count;
    mctp_ver_t version;
}
mctp_resp_get_mctp_ver_t;



// MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT

typedef struct __attribute__ ((__packed__)) mctp_req_get_msg_type_t
{
    mctp_ctrl_header_t header;   
}
mctp_req_get_msg_type_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_get_msg_type_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    uint8_t msg_type_count;
    mctp_msg_type_t msg_type;
}
mctp_resp_get_msg_type_t;



// MCTP_CTRL_CMD_GET_VENDOR_MESSAGE_SUPPORT

typedef struct __attribute__ ((__packed__)) mctp_req_get_vendor_t
{
    mctp_ctrl_header_t header;
    uint8_t vendor_id_selector;
}
mctp_req_get_vendor_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_get_vendor_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    uint8_t vendor_id_selector;
}
mctp_resp_get_vendor_t;



// MCTP_CTRL_CMD_RESOLVE_ENDPOINT_ID

typedef struct __attribute__ ((__packed__)) mctp_req_resolve_eid_t
{
    mctp_ctrl_header_t header;
    mctp_eid_t target_eid;
}
mctp_req_resolve_eid_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_resolve_eid_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    mctp_eid_t bridge_eid;
    uint8_t physical_address[];
}
mctp_resp_resolve_eid_t;



// MCTP_CTRL_CMD_ALLOCATE_ENDPOINT_IDS

typedef enum __attribute__ ((__packed__)) mctp_eid_alloc_op_t
{
    MCTP_EID_ALLOC_OP_ALLOC           = 0b00,
    MCTP_EID_ALLOC_OP_FORCE           = 0b01,
    MCTP_EID_ALLOC_OP_INFO            = 0b10,
    MCTP_EID_ALLOC_OP_RESERVED_b11    = 0b11,
}
mctp_eid_alloc_op_t;


typedef struct __attribute__ ((__packed__)) mctp_req_alloc_eid_t
{
    mctp_ctrl_header_t header;
    mctp_eid_alloc_op_t operation   : 2;
    uint8_t                         : 6;
    uint8_t eid_pool_size;
    mctp_eid_t start_eid;
}
mctp_req_alloc_eid_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_alloc_eid_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    mctp_eid_assign_status_t eid_assign_status : 2;
    uint8_t                             : 6;
    uint8_t eid_pool_size;
    mctp_eid_t start_eid;
}
mctp_resp_alloc_eid_t;



// MCTP_CTRL_CMD_ROUTING_INFO_UPDATE

typedef enum mctp_routing_table_entry_type_t
{
    MCTP_ROUTING_ENTRY_SINGLE_NON_BRIDGE    = 0b00,
    MCTP_ROUTING_ENTRY_RANGE_BRIDGE_INCLUDE = 0b01,
    MCTP_ROUTING_ENTRY_SINGLE_BRIDGE        = 0b10,
    MCTP_ROUTING_ENTRY_RANGE_BRIDGE_EXCLUDE = 0b11,
}
mctp_routing_table_entry_type_t;

typedef struct __attribute__ ((__packed__)) mctp_routing_info_upd_t
{
    mctp_routing_table_entry_type_t type    : 2;
    uint8_t                                 : 6;
    uint8_t range_size;
    mctp_eid_t start_eid;
    uint8_t physical_address[];
}
mctp_routing_info_upd_t;


typedef struct __attribute__ ((__packed__)) mctp_req_routing_info_upd_t
{
    mctp_ctrl_header_t header;
    uint8_t routing_info_upd_count;
    mctp_routing_info_upd_t routing_info_upd; 
}
mctp_req_routing_info_upd_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_routing_info_upd_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
}
mctp_resp_routing_info_upd_t;



// MCTP_CTRL_CMD_GET_ROUTING_TABLE_ENTRIES

typedef struct __attribute__ ((__packed__)) mctp_routing_table_entry_t
{
    uint8_t eid_range_size;
    mctp_eid_t start_eid;
    uint8_t port                            : 5;
    bool is_static                          : 1;
    mctp_routing_table_entry_type_t type    : 2;
    mctp_binding_type_t binding_type;
    mctp_physical_medium_t medium;
    uint8_t physical_address_size;
    uint8_t physical_address[];
}
mctp_routing_table_entry_t;


typedef struct __attribute__ ((__packed__)) mctp_req_get_routing_table_t
{
    mctp_ctrl_header_t header;

    uint8_t entry_handle : 8;
}
mctp_req_get_routing_table_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_get_routing_table_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    uint8_t next_entry_handle;
    uint8_t routing_table_entry_count;
    mctp_routing_table_entry_t routing_table_entry; 
}
mctp_resp_get_routing_table_t;



// MCTP_CTRL_CMD_PREPARE_FOR_ENDPOINT_DISCOVERY

typedef struct __attribute__ ((__packed__)) mctp_req_prepare_for_endpoint_discovery_t
{
    mctp_ctrl_header_t header;
}
mctp_req_prepare_for_endpoint_discovery_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_prepare_for_endpoint_discovery_t
{
    mctp_ctrl_header_t header;
}
mctp_resp_prepare_for_endpoint_discovery_t;



// MCTP_CTRL_CMD_ENDPOINT_DISCOVERY

typedef struct __attribute__ ((__packed__)) mctp_req_endpoint_discovery_t
{
    mctp_ctrl_header_t header;
}
mctp_req_endpoint_discovery_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_endpoint_discovery_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
}
mctp_resp_endpoint_discovery_t;



// MCTP_CTRL_CMD_DISCOVERY_NOTIFY

typedef struct __attribute__ ((__packed__)) mctp_req_discovery_notify_t
{
    mctp_ctrl_header_t header;
}
mctp_req_discovery_notify_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_discovery_notify_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
}
mctp_resp_discovery_notify_t;



// MCTP_CTRL_CMD_GET_NETWORK_ID

typedef struct __attribute__ ((__packed__)) mctp_req_get_network_id_t
{
    mctp_ctrl_header_t header;
}
mctp_req_get_network_id_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_get_network_id_t
{
    mctp_ctrl_header_t header;

    mctp_ctrl_cc_t completion_code;
    uint8_t network_id[16];
}
mctp_resp_get_network_id_t;



// MCTP_CTRL_CMD_QUERY_HOP

typedef struct __attribute__ ((__packed__)) mctp_req_query_hop_t
{
    mctp_ctrl_header_t header;
    mctp_eid_t target_eid;
    mctp_msg_type_t msg_type;
}
mctp_req_query_hop_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_query_hop_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    mctp_eid_t next_bridge;
    mctp_msg_type_t msg_type;
    uint16_t maxt_tu_in;
    uint16_t maxt_tu_out;
}
mctp_resp_query_hop_t;



// MCTP_CTRL_CMD_RESOLVE_UUID

typedef struct __attribute__ ((__packed__)) mctp_resolve_uuid_entry_t
{
    mctp_eid_t eid;      
    mctp_binding_type_t binding_type;       
    mctp_physical_medium_t medium;         
    uint8_t physical_address_size;          
    uint8_t physical_address[];
}
mctp_resolve_uuid_entry_t;



typedef struct __attribute__ ((__packed__)) mctp_req_resolve_uuid_t
{
    mctp_ctrl_header_t header;
    uint8_t uuid[16];
    uint8_t target_eid;
}
mctp_req_resolve_uuid_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_resolve_uuid_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    uint8_t next_entry_handle;
    uint8_t entries_count;
    mctp_resolve_uuid_entry_t first_entry;
}
mctp_resp_resolve_uuid_t;



// MCTP_CTRL_CMD_QUERY_RATE_LIMIT

typedef struct __attribute__ ((__packed__)) mctp_req_query_rate_limit_t
{
    mctp_ctrl_header_t header;
}
mctp_req_query_rate_limit_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_query_rate_limit_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    uint32_t rx_buffer_size;
    uint32_t rx_rate;
    uint32_t tx_max_rate_sup;
    uint32_t tx_min_rate_sup;
    uint32_t max_burst_sup : 24;
    uint32_t eid_tx_max_burst : 24;
    uint32_t eid_tx_max_rate;
    bool tx_limiting_resp_only : 1;
    bool tx_limiting_sup : 1;
    uint8_t : 6;
}
mctp_resp_query_rate_limit_t;



// MCTP_CTRL_CMD_REQUEST_TX_RATE_LIMIT

typedef struct __attribute__ ((__packed__)) mctp_req_request_tx_rate_limit_t
{
    mctp_ctrl_header_t header;
    uint32_t eid_tx_max_burst : 24;
    uint32_t eid_tx_max_rate;
}
mctp_req_request_tx_rate_limit_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_request_tx_rate_limit_t
{
    mctp_ctrl_header_t header;

    mctp_ctrl_cc_t completion_code;
    uint32_t eid_tx_burst : 24;
    uint32_t eid_tx_rate;
}
mctp_resp_request_tx_rate_limit_t;



// MCTP_CTRL_CMD_UPDATE_RATE_LIMIT

typedef struct __attribute__ ((__packed__)) mctp_req_upd_rate_limit_t
{
    mctp_ctrl_header_t header;
    uint32_t eid_tx_burst : 24;
    uint32_t eid_tx_rate;
}
mctp_req_upd_rate_limit_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_upd_rate_limit_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
}
mctp_resp_upd_rate_limit_t;



// MCTP_CTRL_CMD_QUERY_SUPPORTED_INTERFACES

typedef struct __attribute__ ((__packed__)) mctp_host_interface_t
{
    mctp_host_interface_type_t if_type;
    mctp_eid_t eid;
}
mctp_interface_t;

typedef struct __attribute__ ((__packed__)) mctp_req_query_interfaces_t
{
    mctp_ctrl_header_t header;
}
mctp_req_query_interfaces_t;

typedef struct __attribute__ ((__packed__)) mctp_resp_query_interfaces_t
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
    uint8_t interface_count;
    mctp_interface_t firts_intefrace;
}
mctp_resp_query_interfaces_t;


#endif //CONTROL_H