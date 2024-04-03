#ifndef BASE_H
#define BASE_H

#include <pldm/pldm.h>

typedef struct __attribute__ ((__packed__)) pldm_xfer_pos_t
{
    bool is_start   : 1;
    bool is_middle  : 1;
    bool is_end     : 1;
    uint8_t         : 5;
}
pldm_xfer_pos_t;

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
    uint64_t                            : 1;
    bool set_tid                        : 1;    // 0x01
    bool get_tid                        : 1;    // 0x02
    bool get_pldm_ver                   : 1;    // 0x03
    bool get_pldm_type                  : 1;    // 0x04
    bool get_pldm_cmd                   : 1;    // 0x05
    bool select_pldm_ver                : 1;    // 0x06
    bool negotiate_xfer_params          : 1;    // 0x07
    bool multipart_send                 : 1;    // 0x08
    bool multipart_receive              : 1;    // 0x09
    uint64_t                            : 54;
    uint64_t                            : 64;
    uint64_t                            : 64;
    uint64_t                            : 64;
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

typedef union __attribute__ ((__packed__)) pldm_ver_t
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
pldm_ver_t;


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
    pldm_ver_t version;
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
    pldm_ver_t pldm_version;
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
    pldm_ver_t pldm_version;
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