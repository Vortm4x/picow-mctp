#ifndef BASE_H
#define BASE_H

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

typedef struct __attribute__ ((__packed__)) pldm_cmd_support_t
{
    bool : 1; // 0x00
    bool base_set_tid                               : 1; // 0x01
    bool base_get_tid                               : 1; // 0x02
    bool base_get_pldm_version                      : 1; // 0x03
    bool base_get_pldm_type                         : 1; // 0x04
    bool base_get_pldm_commands                     : 1; // 0x05
    bool base_select_pldm_version                   : 1; // 0x06
    bool base_negotiate_xfer_params                 : 1; // 0x07
    bool base_multipart_send                        : 1; // 0x08
    bool base_multipart_receive                     : 1; // 0x09
    bool platform_term_platform_event_msg           : 1; // 0x0A
    bool platform_term_poll_for_platform_event_msg  : 1; // 0x0B
    bool platform_term_event_msg_supported          : 1; // 0x0C
    bool platform_term_event_msg_buff_size          : 1; // 0x0D
    bool : 1; // 0x0E
    bool : 1; // 0x0F
    bool platform_num_sens_set_enable               : 1; // 0x10
    bool platform_num_sens_get_reading              : 1; // 0x11
    bool platform_num_sens_get_tresholds            : 1; // 0x12
    bool platform_num_sens_set_tresholds            : 1; // 0x13
    bool platform_num_sens_restore_tresholds        : 1; // 0x14
    bool platform_num_sens_get_hysteresis           : 1; // 0x15
    bool platform_num_sens_set_hysteresis           : 1; // 0x16
    bool platform_num_sens_init                     : 1; // 0x17
    bool : 1; // 0x18
    bool : 1; // 0x19
    bool : 1; // 0x1A
    bool : 1; // 0x1B
    bool : 1; // 0x1C
    bool : 1; // 0x1D
    bool : 1; // 0x1E
    bool : 1; // 0x1F
    bool platform_state_sens_set_enable             : 1; // 0x20
    bool platform_state_sens_get_reading            : 1; // 0x21
    bool platform_state_sens_init                   : 1; // 0x22
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
    bool platform_num_effect_set_enable             : 1; // 0x30
    bool platform_num_effect_set_value              : 1; // 0x31
    bool platform_num_effect_get_value              : 1; // 0x32
    bool : 1; // 0x33
    bool : 1; // 0x34
    bool : 1; // 0x35
    bool : 1; // 0x36
    bool : 1; // 0x37
    bool platform_state_effect_set_enable           : 1; // 0x38
    bool platform_state_effect_set_value            : 1; // 0x39
    bool platform_state_effect_get_value            : 1; // 0x3A
    bool : 1; // 0x3B
    bool : 1; // 0x3C
    bool : 1; // 0x3D
    bool : 1; // 0x3E
    bool : 1; // 0x3F
    bool platform_event_log_info                    : 1; // 0x40
    bool platform_event_log_enable                  : 1; // 0x41
    bool platform_event_log_clear                   : 1; // 0x42
    bool platform_event_log_get_timestamp           : 1; // 0x43
    bool platform_event_log_set_timestamp           : 1; // 0x44
    bool platform_event_log_read                    : 1; // 0x45
    bool platform_event_log_get_policy              : 1; // 0x46
    bool platform_event_log_set_policy              : 1; // 0x47
    bool platform_event_log_find                    : 1; // 0x48
    bool : 1; // 0x49
    bool : 1; // 0x4A
    bool : 1; // 0x4B
    bool : 1; // 0x4C
    bool : 1; // 0x4D
    bool : 1; // 0x4E
    bool : 1; // 0x4F
    bool platform_pdr_repo_info                     : 1; // 0x50
    bool platform_pdr_repo_get                      : 1; // 0x51
    bool platform_pdr_repo_find                     : 1; // 0x52
    bool platform_pdr_repo_sig                      : 1; // 0x53
    bool : 1; // 0x54
    bool : 1; // 0x55
    bool : 1; // 0x56
    bool : 1; // 0x57
    bool platform_pdr_repo_run_init_aget            : 1; // 0x58
    bool : 1; // 0x59
    bool : 1; // 0x5A
    bool : 1; // 0x5B
    bool : 1; // 0x5C
    bool : 1; // 0x5D
    bool : 1; // 0x5E
    bool : 1; // 0x5F
    bool : 1; // 0x60
    bool : 1; // 0x61
    bool : 1; // 0x62
    bool : 1; // 0x63
    bool : 1; // 0x64
    bool : 1; // 0x65
    bool : 1; // 0x66
    bool : 1; // 0x67
    bool : 1; // 0x68
    bool : 1; // 0x69
    bool : 1; // 0x6A
    bool : 1; // 0x6B
    bool : 1; // 0x6C
    bool : 1; // 0x6D
    bool : 1; // 0x6E
    bool : 1; // 0x6F
    bool : 1; // 0x70
    bool : 1; // 0x71
    bool : 1; // 0x72
    bool : 1; // 0x73
    bool : 1; // 0x74
    bool : 1; // 0x75
    bool : 1; // 0x76
    bool : 1; // 0x77
    bool : 1; // 0x78
    bool : 1; // 0x79
    bool : 1; // 0x7A
    bool : 1; // 0x7B
    bool : 1; // 0x7C
    bool : 1; // 0x7D
    bool : 1; // 0x7E
    bool : 1; // 0x7F
    bool : 1; // 0x80
    bool : 1; // 0x81
    bool : 1; // 0x82
    bool : 1; // 0x83
    bool : 1; // 0x84
    bool : 1; // 0x85
    bool : 1; // 0x86
    bool : 1; // 0x87
    bool : 1; // 0x88
    bool : 1; // 0x89
    bool : 1; // 0x8A
    bool : 1; // 0x8B
    bool : 1; // 0x8C
    bool : 1; // 0x8D
    bool : 1; // 0x8E
    bool : 1; // 0x8F
    bool : 1; // 0x90
    bool : 1; // 0x91
    bool : 1; // 0x92
    bool : 1; // 0x93
    bool : 1; // 0x94
    bool : 1; // 0x95
    bool : 1; // 0x96
    bool : 1; // 0x97
    bool : 1; // 0x98
    bool : 1; // 0x99
    bool : 1; // 0x9A
    bool : 1; // 0x9B
    bool : 1; // 0x9C
    bool : 1; // 0x9D
    bool : 1; // 0x9E
    bool : 1; // 0x9F
    bool : 1; // 0xA0
    bool : 1; // 0xA1
    bool : 1; // 0xA2
    bool : 1; // 0xA3
    bool : 1; // 0xA4
    bool : 1; // 0xA5
    bool : 1; // 0xA6
    bool : 1; // 0xA7
    bool : 1; // 0xA8
    bool : 1; // 0xA9
    bool : 1; // 0xAA
    bool : 1; // 0xAB
    bool : 1; // 0xAC
    bool : 1; // 0xAD
    bool : 1; // 0xAE
    bool : 1; // 0xAF
    bool : 1; // 0xB0
    bool : 1; // 0xB1
    bool : 1; // 0xB2
    bool : 1; // 0xB3
    bool : 1; // 0xB4
    bool : 1; // 0xB5
    bool : 1; // 0xB6
    bool : 1; // 0xB7
    bool : 1; // 0xB8
    bool : 1; // 0xB9
    bool : 1; // 0xBA
    bool : 1; // 0xBB
    bool : 1; // 0xBC
    bool : 1; // 0xBD
    bool : 1; // 0xBE
    bool : 1; // 0xBF
    bool : 1; // 0xC0
    bool : 1; // 0xC1
    bool : 1; // 0xC2
    bool : 1; // 0xC3
    bool : 1; // 0xC4
    bool : 1; // 0xC5
    bool : 1; // 0xC6
    bool : 1; // 0xC7
    bool : 1; // 0xC8
    bool : 1; // 0xC9
    bool : 1; // 0xCA
    bool : 1; // 0xCB
    bool : 1; // 0xCC
    bool : 1; // 0xCD
    bool : 1; // 0xCE
    bool : 1; // 0xCF
    bool : 1; // 0xD0
    bool : 1; // 0xD1
    bool : 1; // 0xD2
    bool : 1; // 0xD3
    bool : 1; // 0xD4
    bool : 1; // 0xD5
    bool : 1; // 0xD6
    bool : 1; // 0xD7
    bool : 1; // 0xD8
    bool : 1; // 0xD9
    bool : 1; // 0xDA
    bool : 1; // 0xDB
    bool : 1; // 0xDC
    bool : 1; // 0xDD
    bool : 1; // 0xDE
    bool : 1; // 0xDF
    bool : 1; // 0xE0
    bool : 1; // 0xE1
    bool : 1; // 0xE2
    bool : 1; // 0xE3
    bool : 1; // 0xE4
    bool : 1; // 0xE5
    bool : 1; // 0xE6
    bool : 1; // 0xE7
    bool : 1; // 0xE8
    bool : 1; // 0xE9
    bool : 1; // 0xEA
    bool : 1; // 0xEB
    bool : 1; // 0xEC
    bool : 1; // 0xED
    bool : 1; // 0xEE
    bool : 1; // 0xEF
    bool : 1; // 0xF0
    bool : 1; // 0xF1
    bool : 1; // 0xF2
    bool : 1; // 0xF3
    bool : 1; // 0xF4
    bool : 1; // 0xF5
    bool : 1; // 0xF6
    bool : 1; // 0xF7
    bool : 1; // 0xF8
    bool : 1; // 0xF9
    bool : 1; // 0xFA
    bool : 1; // 0xFB
    bool : 1; // 0xFC
    bool : 1; // 0xFD
    bool : 1; // 0xFE
    bool : 1; // 0xFF
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

#endif // BASE_H