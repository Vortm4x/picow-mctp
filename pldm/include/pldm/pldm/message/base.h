#ifndef PLDM_MESSAGE_BASE_H
#define PLDM_MESSAGE_BASE_H

#include <pldm/pldm.h>

typedef enum __attribute__ ((__packed__)) pldm_xfer_op_t
{
    PLDM_XFER_OP_FIRST      = 0,
    PLDM_XFER_OP_NEXT       = 1,
    PLDM_XFER_OP_ABORT      = 2,
    PLDM_XFER_OP_COMPLETE   = 3,
    PLDM_XFER_OP_CURRENT    = 4,
}
pldm_xfer_op_t;

typedef struct __attribute__ ((__packed__)) pldm_protocol_support_t
{
    bool base           : 1;
    bool smbios         : 1;
    bool monitor        : 1;
    bool bios_ctrl      : 1;
    bool fru            : 1;
    bool firmware_upd   : 1;
    bool redfish        : 1;
    uint64_t            : 57;
}
pldm_protocol_support_t;

typedef union __attribute__ ((__packed__)) pldm_cmd_support_t
{
    struct __attribute__ ((__packed__))
    {
        bool : 1; // 0x00
        bool set_tid                               : 1; // 0x01
        bool get_tid                               : 1; // 0x02
        bool get_pldm_version                      : 1; // 0x03
        bool get_pldm_type                         : 1; // 0x04
        bool get_pldm_commands                     : 1; // 0x05
        bool select_pldm_version                   : 1; // 0x06
        bool negotiate_xfer_params                 : 1; // 0x07
        bool multipart_send                        : 1; // 0x08
        bool multipart_receive                     : 1; // 0x09
        bool : 1; // 0x0A
        bool : 1; // 0x0B
        bool : 1; // 0x0C
        bool : 1; // 0x0D
        bool : 1; // 0x0E
        bool : 1; // 0x0F

        uint64_t : 48;
        uint64_t : 64;
        uint64_t : 64;
        uint64_t : 64;
    }
    base;

    struct __attribute__ ((__packed__))
    {
        bool : 1; // 0x00
        bool : 1; // 0x01
        bool : 1; // 0x02
        bool : 1; // 0x03
        bool : 1; // 0x04
        bool : 1; // 0x05
        bool : 1; // 0x06
        bool : 1; // 0x07
        bool : 1; // 0x08
        bool : 1; // 0x09
        bool term_platform_event_msg           : 1; // 0x0A
        bool term_poll_for_platform_event_msg  : 1; // 0x0B
        bool term_event_msg_supported          : 1; // 0x0C
        bool term_event_msg_buff_size          : 1; // 0x0D
        bool : 1; // 0x0E
        bool : 1; // 0x0F

        bool num_sens_set_enable               : 1; // 0x10
        bool num_sens_get_reading              : 1; // 0x11
        bool num_sens_get_tresholds            : 1; // 0x12
        bool num_sens_set_tresholds            : 1; // 0x13
        bool num_sens_restore_tresholds        : 1; // 0x14
        bool num_sens_get_hysteresis           : 1; // 0x15
        bool num_sens_set_hysteresis           : 1; // 0x16
        bool num_sens_init                     : 1; // 0x17
        bool : 1; // 0x18
        bool : 1; // 0x19
        bool : 1; // 0x1A
        bool : 1; // 0x1B
        bool : 1; // 0x1C
        bool : 1; // 0x1D
        bool : 1; // 0x1E
        bool : 1; // 0x1F

        bool state_sens_set_enable             : 1; // 0x20
        bool state_sens_get_reading            : 1; // 0x21
        bool state_sens_init                   : 1; // 0x22
        bool : 1; // 0x23
        bool : 1; // 0x24
        bool : 1; // 0x25
        bool : 1; // 0x26
        bool : 1; // 0x27
        bool : 1; // 0x28
        bool : 1; // 0x29
        bool : 1; // 0x2A
        bool : 1; // 0x2B
        bool : 1; // 0x2C
        bool : 1; // 0x2D
        bool : 1; // 0x2E
        bool : 1; // 0x2F

        bool num_effect_set_enable             : 1; // 0x30
        bool num_effect_set_value              : 1; // 0x31
        bool num_effect_get_value              : 1; // 0x32
        bool : 1; // 0x33
        bool : 1; // 0x34
        bool : 1; // 0x35
        bool : 1; // 0x36
        bool : 1; // 0x37
        bool state_effect_set_enable           : 1; // 0x38
        bool state_effect_set_value            : 1; // 0x39
        bool state_effect_get_value            : 1; // 0x3A
        bool : 1; // 0x3B
        bool : 1; // 0x3C
        bool : 1; // 0x3D
        bool : 1; // 0x3E
        bool : 1; // 0x3F

        bool event_log_info                    : 1; // 0x40
        bool event_log_enable                  : 1; // 0x41
        bool event_log_clear                   : 1; // 0x42
        bool event_log_get_timestamp           : 1; // 0x43
        bool event_log_set_timestamp           : 1; // 0x44
        bool event_log_read                    : 1; // 0x45
        bool event_log_get_policy              : 1; // 0x46
        bool event_log_set_policy              : 1; // 0x47
        bool event_log_find                    : 1; // 0x48
        bool : 1; // 0x49
        bool : 1; // 0x4A
        bool : 1; // 0x4B
        bool : 1; // 0x4C
        bool : 1; // 0x4D
        bool : 1; // 0x4E
        bool : 1; // 0x4F

        bool pdr_repo_info                     : 1; // 0x50
        bool pdr_repo_get                      : 1; // 0x51
        bool pdr_repo_find                     : 1; // 0x52
        bool pdr_repo_sig                      : 1; // 0x53
        bool : 1; // 0x54
        bool : 1; // 0x55
        bool : 1; // 0x56
        bool : 1; // 0x57
        bool pdr_repo_run_init_aget            : 1; // 0x58
        bool : 1; // 0x59
        bool : 1; // 0x5A
        bool : 1; // 0x5B
        bool : 1; // 0x5C
        bool : 1; // 0x5D
        bool : 1; // 0x5E
        bool : 1; // 0x5F

        uint64_t : 32;
        uint64_t : 64;
        uint64_t : 64;
    }
    platform;

    struct __attribute__ ((__packed__))
    {
        bool : 1; // 0x00
        bool negotiate_redfish_params   : 1; // 0x01
        bool negotiate_medium_params    : 1; // 0x02
        bool get_schema_dict            : 1; // 0x03
        bool get_schema_uri             : 1; // 0x04
        bool get_resource_etag          : 1; // 0x05
        bool get_oem_count              : 1; // 0x06
        bool get_oem_name               : 1; // 0x07
        bool get_registry_count         : 1; // 0x08
        bool get_registry_details       : 1; // 0x09
        bool select_registry_version    : 1; // 0x0A
        bool get_message_registry       : 1; // 0x0B
        bool get_schema_file            : 1; // 0x0C
        bool : 1; // 0x0D
        bool : 1; // 0x0E
        bool : 1; // 0x0F

        bool rde_operation_init         : 1; // 0x10
        bool supply_req_params          : 1; // 0x11
        bool retrieve_resp_params       : 1; // 0x12
        bool rde_operation_complete     : 1; // 0x13
        bool rde_operation_status       : 1; // 0x14
        bool rde_operation_kill         : 1; // 0x15
        bool rde_operation_enumerate    : 1; // 0x16
        bool : 1; // 0x17
        bool : 1; // 0x18
        bool : 1; // 0x19
        bool : 1; // 0x1A
        bool : 1; // 0x1B
        bool : 1; // 0x1C
        bool : 1; // 0x1D
        bool : 1; // 0x1E
        bool : 1; // 0x1F

        bool : 1; // 0x20
        bool : 1; // 0x21
        bool : 1; // 0x22
        bool : 1; // 0x23
        bool : 1; // 0x24
        bool : 1; // 0x25
        bool : 1; // 0x26
        bool : 1; // 0x27
        bool : 1; // 0x28
        bool : 1; // 0x29
        bool : 1; // 0x2A
        bool : 1; // 0x2B
        bool : 1; // 0x2C
        bool : 1; // 0x2D
        bool : 1; // 0x2E
        bool : 1; // 0x2F

        bool rde_multipart_send         : 1; // 0x30
        bool rde_multipart_receive      : 1; // 0x31
        bool : 1; // 0x32
        bool : 1; // 0x33
        bool : 1; // 0x34
        bool : 1; // 0x35
        bool : 1; // 0x36
        bool : 1; // 0x37
        bool : 1; // 0x38
        bool : 1; // 0x39
        bool : 1; // 0x3A
        bool : 1; // 0x3B
        bool : 1; // 0x3C
        bool : 1; // 0x3D
        bool : 1; // 0x3E
        bool : 1; // 0x3F

        uint64_t : 64;
        uint64_t : 64;
        uint64_t : 64;
    }
    redfish;



}
pldm_cmd_support_t;


// PLDM_BASE_CMD_SET_TID

typedef struct __attribute__ ((__packed__)) pldm_req_set_tid_t
{
    pldm_base_header_t header;
    pldm_tid_t tid;
}
pldm_req_set_tid_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_set_tid_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
}
pldm_resp_set_tid_t;


// PLDM_BASE_CMD_GET_TID

typedef struct __attribute__ ((__packed__)) pldm_req_get_tid_t
{
    pldm_base_header_t header;
}
pldm_req_get_tid_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_get_tid_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_tid_t tid;
}
pldm_resp_get_tid_t;



// PLDM_BASE_CMD_GET_PLDM_VERSION

typedef struct __attribute__ ((__packed__)) pldm_req_get_pldm_ver_t
{
    pldm_base_header_t header;
    uint32_t xfer_handle;
    bool is_first_xfer  : 1;
    uint8_t             : 7;
    uint8_t pldm_type;
}
pldm_req_get_pldm_ver_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_get_pldm_ver_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    uint32_t next_xfer_handle;
    pldm_xfer_pos_t xfer_pos;
    ver32_t versions[];
}
pldm_resp_get_pldm_ver_t;


// PLDM_BASE_CMD_GET_PLDM_TYPE

typedef struct __attribute__ ((__packed__)) pldm_req_get_pldm_type_t
{
    pldm_base_header_t header;
}
pldm_req_get_pldm_type_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_get_pldm_type_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_protocol_support_t protocol_support;
}
pldm_resp_get_pldm_type_t;



// PLDM_BASE_CMD_GET_PLDM_COMMANDS

typedef struct __attribute__ ((__packed__)) pldm_req_get_pldm_cmd_t
{
    pldm_base_header_t header;
    uint8_t pldm_type;
    ver32_t pldm_version;
}
pldm_req_get_pldm_cmd_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_get_pldm_cmd_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_cmd_support_t cmd_support;
}
pldm_resp_get_pldm_cmd_t;



// PLDM_BASE_CMD_SELECT_PLDM_VERSION

typedef struct __attribute__ ((__packed__)) pldm_req_select_pldm_ver_t
{
    pldm_base_header_t header;
    uint8_t pldm_type;
    ver32_t pldm_version;
}
pldm_req_select_pldm_ver_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_select_pldm_ver_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
}
pldm_resp_select_pldm_ver_t;



// PLDM_BASE_CMD_NEGOTIATE_XFER_PARAMS

typedef struct __attribute__ ((__packed__)) pldm_req_negotiate_xfer_params_t
{
    pldm_base_header_t header;
    uint16_t part_size;
    pldm_protocol_support_t protocol_support;
}
pldm_req_negotiate_xfer_params_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_negotiate_xfer_params_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    uint16_t part_size;
    pldm_protocol_support_t protocol_support;
}
pldm_resp_negotiate_xfer_params_t;



// PLDM_BASE_CMD_MULTIPART_SEND

typedef struct __attribute__ ((__packed__)) pldm_req_multipart_send_t
{
    pldm_base_header_t header;
    uint8_t pldm_type;
    pldm_xfer_pos_t xfer_pos;
    uint32_t xfer_ctx;
    uint32_t xfer_handle;
    uint32_t next_xfer_handle;
    uint32_t section_offset;
    uint32_t section_len;
    uint32_t data_len;
    uint8_t data[];
}
pldm_req_multipart_send_t;


typedef struct __attribute__ ((__packed__)) pldm_resp_multipart_send_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_xfer_op_t xfer_op;
}
pldm_resp_multipart_send_t;



// PLDM_BASE_CMD_MULTIPART_RECEIVE

typedef struct __attribute__ ((__packed__)) pldm_req_multipart_receive_t
{
    pldm_base_header_t header;
    pldm_xfer_op_t xfer_op;
    uint32_t xfer_ctx;
    uint32_t section_offset;
    uint32_t section_len;
}
pldm_req_multipart_receive_t;


typedef struct __attribute__ ((__packed__)) pldm_resp_multipart_receive_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_xfer_pos_t xfer_pos;
    uint32_t next_xfer_handle;
    uint32_t data_len;
    uint8_t data[];
}
pldm_resp_multipart_receive_t;

#endif // PLDM_MESSAGE_BASE_H