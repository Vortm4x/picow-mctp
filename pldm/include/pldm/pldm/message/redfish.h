#ifndef PLDM_MESSAGE_REDFISH_H
#define PLDM_MESSAGE_REDFISH_H

#include <pldm/pldm.h>

#define PLDM_REDFISH_MIN_XFER_LEN 64


typedef enum __attribute__ ((__packed__)) pldm_rde_xfer_oper_t
{
    RDE_XFER_OPER_FIRST = 0,
    RDE_XFER_OPER_NEXT  = 1,
    RDE_XFER_OPER_ABORT = 2,
}
pldm_rde_xfer_oper_t;

typedef enum __attribute__ ((__packed__)) pldm_rde_xfer_flag_t
{
    RDE_XFER_FLAG_START         = 0,
    RDE_XFER_FLAG_MIDDLE        = 1,
    RDE_XFER_FLAG_END           = 2,
    RDE_XFER_FLAG_START_AND_END = 3,
}
pldm_rde_xfer_flag_t;


typedef struct __attribute__ ((__packed__)) pldm_base_redfish_feature_support_t
{
    bool head       : 1;
    bool read       : 1;
    bool create     : 1;
    bool delete     : 1;
    bool update     : 1;
    bool replace    : 1;
    bool action     : 1;
    bool event      : 1;
}
pldm_base_redfish_feature_support_t;

typedef struct __attribute__ ((__packed__)) pldm_rde_oper_exec_flags_t
{
    bool task_spawned       : 1;
    bool custom_resp_params : 1;
    bool result_payload     : 1;
    bool cache_allowed      : 1;
    uint8_t                 : 1;
}
pldm_rde_oper_exec_flags_t;


// PLDM_REDFISH_CMD_NEGOTIATE_REDFISH_PARAMS

typedef struct __attribute__ ((__packed__)) pldm_req_negotiate_redfish_params_t
{
    pldm_base_header_t header;
    uint8_t mc_concurrency;
    bool bej_v1_1   : 1;
    uint8_t         : 7;
    pldm_base_redfish_feature_support_t base_feature;
}
pldm_req_negotiate_redfish_params_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_negotiate_redfish_params_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    uint8_t device_concurrency;
    bool atomic_res_read    : 1;
    bool expand_req         : 1;
    bool bej_v1_1           : 1;
    uint16_t                : 13;
    pldm_base_redfish_feature_support_t base_feature;
    uint32_t device_config_sig;
    var_str_t device_provider;
}
pldm_resp_negotiate_redfish_params_t;


// PLDM_REDFISH_CMD_NEGOTIATE_MEDIUM_PARAMS

typedef struct __attribute__ ((__packed__)) pldm_req_negotiate_medium_params_t
{
    pldm_base_header_t header;
    uint32_t mc_max_xfer_size;
}
pldm_req_negotiate_medium_params_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_negotiate_medium_params_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    uint32_t device_max_xfer_size;
}
pldm_resp_negotiate_medium_params_t;


// PLDM_REDFISH_CMD_GET_SCHEMA_DICT

typedef struct __attribute__ ((__packed__)) pldm_req_get_schema_dict_t
{
    pldm_base_header_t header;
    uint32_t resource_id;
    pldm_rde_schema_class_t schema_class;
}
pldm_req_get_schema_dict_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_get_schema_dict_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    uint8_t dict_format;
    uint32_t xfer_handle;
}
pldm_resp_get_schema_dict_t;


// PLDM_REDFISH_CMD_GET_SCHEMA_URI

typedef struct __attribute__ ((__packed__)) pldm_req_get_schema_uri_t
{
    pldm_base_header_t header;
    uint32_t resource_id;
    pldm_rde_schema_class_t schema_class;
    uint8_t oem_ext_number;
}
pldm_req_get_schema_uri_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_get_schema_uri_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    uint8_t string_fragment_count;
}
pldm_resp_get_schema_uri_t;



// PLDM_REDFISH_CMD_GET_RESOURCE_ETAG

typedef struct __attribute__ ((__packed__)) pldm_req_get_resource_etag_t
{
    pldm_base_header_t header;
    uint32_t resource_id;
}
pldm_req_get_resource_etag_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_get_resource_etag_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    var_str_t etag;
}
pldm_resp_get_resource_etag_t;


// PLDM_REDFISH_CMD_RDE_OPERATION_INIT

typedef struct __attribute__ ((__packed__)) pldm_rde_oper_flags_t
{
    bool locator_valid      : 1;
    bool req_payload        : 1;
    bool req_custom_params  : 1;
    bool excerpt_read       : 1;
    uint8_t                 : 4;
}
pldm_rde_oper_flags_t;

typedef struct __attribute__ ((__packed__)) pldm_rde_oper_permission_flags_t
{
    bool read       : 1;
    bool update     : 1;
    bool replace    : 1;
    bool create     : 1;
    bool delete     : 1;
    bool head       : 1;
    uint8_t         : 2;
}
pldm_rde_oper_permission_flags_t;

typedef struct __attribute__ ((__packed__)) pldm_req_rde_operation_init_t
{
    pldm_base_header_t header;
    uint32_t resource_id;
    pldm_rde_oper_id_t oper_id;
    pldm_rde_oper_type_t oper_type;
    pldm_rde_oper_flags_t oper_flags;
    uint32_t send_xfer_handle;
    uint8_t oper_locator_len;
    uint32_t req_payload_len;
}
pldm_req_rde_operation_init_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_rde_operation_init_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_rde_oper_status_t oper_status;
    uint8_t completion_percentage;
    uint32_t completion_time_seconds;
    pldm_rde_oper_exec_flags_t oper_exec_flags;
    uint32_t result_xfer_handle;
    pldm_rde_oper_permission_flags_t oper_permission_flags;
    uint32_t response_payload_len;
}
pldm_resp_rde_operation_init_t;


// PLDM_REDFISH_CMD_SUPPLY_REQ_PARAMS

typedef enum __attribute__ ((__packed__)) pldm_rde_etag_oper_t
{
    RDE_ETAG_OPER_IGNORE        = 0,
    RDE_ETAG_OPER_IF_MATCH      = 1,
    RDE_ETAG_OPER_IF_NONE_MATCH = 2,
}
pldm_rde_etag_oper_t;

typedef struct __attribute__ ((__packed__)) pldm_req_supply_req_params_t
{
    pldm_base_header_t header;
    uint32_t resource_id;
    pldm_rde_oper_id_t oper_id;
    uint16_t link_expand;
    uint16_t collection_skip;
    uint16_t collection_top;
    uint16_t pagination_offset;
    pldm_rde_etag_oper_t etag_oper;
    uint8_t etag_count;   
}
pldm_req_supply_req_params_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_supply_req_params_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_rde_oper_status_t oper_status;    
    uint8_t completion_percentage;
    uint32_t completion_time_seconds;
    pldm_rde_oper_exec_flags_t oper_exec_flags;
    uint32_t result_xfer_handle;
    pldm_rde_oper_permission_flags_t oper_permission_flags;
    uint32_t response_payload_len;
}
pldm_resp_supply_req_params_t;


// PLDM_REDFISH_CMD_RDE_OPERATION_COMPLETE

typedef struct __attribute__ ((__packed__)) pldm_req_rde_operation_complete_t
{
    pldm_base_header_t header;
    uint32_t resource_id;
    pldm_rde_oper_id_t oper_id;
}
pldm_req_rde_operation_complete_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_rde_operation_complete_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
}
pldm_resp_rde_operation_complete_t;


// PLDM_REDFISH_CMD_RDE_OPERATION_STATUS

typedef struct __attribute__ ((__packed__)) pldm_req_rde_operation_status_t
{
    pldm_base_header_t header;
    uint32_t resource_id;
    pldm_rde_oper_id_t oper_id;
}
pldm_req_rde_operation_status_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_rde_operation_status_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_rde_oper_status_t oper_status;    
    uint8_t completion_percentage;
    uint32_t completion_time_seconds;
    pldm_rde_oper_exec_flags_t oper_exec_flags;
    uint32_t result_xfer_handle;
    pldm_rde_oper_permission_flags_t oper_permission_flags;
    uint32_t response_payload_len;
}
pldm_resp_rde_operation_status_t;


// PLDM_REDFISH_CMD_RDE_OPERATION_ENUMERATE

typedef struct __attribute__ ((__packed__)) pldm_rde_oper_info_t
{
    uint32_t resource_id;
    pldm_rde_oper_id_t oper_id;
    pldm_rde_oper_type_t oper_type;
}
pldm_rde_oper_info_t;

typedef struct __attribute__ ((__packed__)) pldm_req_rde_operation_enumerate_t
{
    pldm_base_header_t header;
}
pldm_req_rde_operation_enumerate_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_rde_operation_enumerate_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    uint16_t oper_count;
    pldm_rde_oper_info_t operations[];
}
pldm_resp_rde_operation_enumerate_t;


// PLDM_REDFISH_CMD_RDE_MULTIPART_RECEIVE

typedef struct __attribute__ ((__packed__)) pldm_req_rde_multipart_receive_t
{
    pldm_base_header_t header;
    uint32_t xfer_handle;
    pldm_rde_oper_id_t oper_id;
    pldm_rde_xfer_oper_t xfer_oper;
}
pldm_req_rde_multipart_receive_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_rde_multipart_receive_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_rde_xfer_flag_t xfer_flag;
    uint32_t next_xfer_handle;
    uint32_t data_len;
    uint8_t data[];
}
pldm_resp_rde_multipart_receive_t;


#endif // PLDM_MESSAGE_REDFISH_H