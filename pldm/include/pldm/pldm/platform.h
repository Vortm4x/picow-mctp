#ifndef PLATFORM_H
#define PLATFORM_H

#include <pldm/pldm.h>
#include <pldm/pdr/pdr.h>

typedef enum __attribute__ ((__packed__)) pldm_sensor_oper_state_t
{
    PLDM_SENSOR_OPER_STATE_ENABLED,
    PLDM_SENSOR_OPER_STATE_DISABLED,
    PLDM_SENSOR_OPER_STATE_UNAVAILABLE,
    PLDM_SENSOR_OPER_STATE_UNKNOWN,
    PLDM_SENSOR_OPER_STATE_FAILED,
    PLDM_SENSOR_OPER_STATE_INITIALIZING,
    PLDM_SENSOR_OPER_STATE_SHUTTING_DOWN,
    PLDM_SENSOR_OPER_STATE_IN_TEST,
}
pldm_sensor_oper_state_t;

typedef enum __attribute__ ((__packed__)) pldm_sensor_event_msg_enable_t
{
    PLDM_SENSOR_EVENT_MSG_NO_GEN,
    PLDM_SENSOR_EVENT_MSG_DISABLED,
    PLDM_SENSOR_EVENT_MSG_ENABLED,
    PLDM_SENSOR_EVENT_MSG_OP_ONLY,
    PLDM_SENSOR_EVENT_MSG_STATE_ONLY,
}
pldm_sensor_event_msg_enable_t;

typedef enum __attribute__ ((__packed__)) pldm_sensor_state_t
{
    PLDM_SENSOR_STATE_UNKNOWN,
    PLDM_SENSOR_STATE_NORMAL,
    PLDM_SENSOR_STATE_WARN,
    PLDM_SENSOR_STATE_CRIT,
    PLDM_SENSOR_STATE_FATAL,
    PLDM_SENSOR_STATE_LOW_WARN,
    PLDM_SENSOR_STATE_LOW_CRIT,
    PLDM_SENSOR_STATE_LOW_FATAL,
    PLDM_SENSOR_STATE_UP_WARN,
    PLDM_SENSOR_STATE_UP_CRIT,
    PLDM_SENSOR_STATE_UP_FATAL
}
pldm_sensor_state_t;


// PLDM_PLATFORM_CMD_NUM_SENS_GET_READING

typedef struct __attribute__ ((__packed__)) pldm_req_num_sens_get_reading_t
{
    pldm_base_header_t header;
    uint16_t sensor_id;
    bool rearm;
}
pldm_req_num_sens_get_reading_t;


typedef struct __attribute__ ((__packed__)) pldm_resp_num_sens_get_reading_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_pdr_data_size_t data_size;
    pldm_sensor_oper_state_t oper_state;
    pldm_sensor_event_msg_enable_t event_msg_enable;
    pldm_sensor_state_t present_state;
    pldm_sensor_state_t prev_state;
    pldm_sensor_state_t event_state;
    uint8_t reading_data[];
}
pldm_resp_num_sens_get_reading_t;


// PLDM_PLATFORM_CMD_PRD_REPO_INFO

typedef enum __attribute__ ((__packed__)) pldm_pdr_repo_state_t
{
    PLDM_PDR_REPO_STATE_AVAILABLE,
    PLDM_PDR_REPO_STATE_UPDATING,
    PLDM_PDR_REPO_STATE_ERROR,
}
pldm_pdr_repo_state_t;


typedef struct __attribute__ ((__packed__)) pldm_req_pdr_repo_info_t
{
    pldm_base_header_t header;
}
pldm_req_pdr_repo_info_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_pdr_repo_info_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    pldm_pdr_repo_state_t repo_state;
    timestamp104_t upd_time;
    timestamp104_t oem_upd_time;
    uint32_t record_count;
    uint32_t repo_size;
    uint32_t largest_record_size;
    uint8_t xfer_handle_timeout;
}
pldm_resp_pdr_repo_info_t;



// PLDM_PLATFORM_CMD_PRD_REPO_GET

typedef struct __attribute__ ((__packed__)) pldm_req_pdr_repo_get_t
{
    pldm_base_header_t header;
    uint32_t record_hanlde;
    uint32_t xfer_hanlde;
    bool is_first_xfer  : 1;
    uint8_t             : 7;
    uint16_t max_record_len;
    uint16_t record_change;
}
pldm_req_pdr_repo_get_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_data_pdr_repo_get_t
{
    pldm_xfer_pos_t xfer_pos;
    uint16_t record_len;
    uint8_t record_data[];
}
pldm_resp_data_pdr_repo_get_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_pdr_repo_get_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    uint32_t next_record_handle;
    uint32_t next_xfer_handle;
}
pldm_resp_pdr_repo_get_t;



// PLDM_PLATFORM_CMD_PRD_REPO_SIG

typedef struct __attribute__ ((__packed__)) pldm_req_pdr_repo_sig_t
{
    pldm_base_header_t header;
}
pldm_req_pdr_repo_sig_t;

typedef struct __attribute__ ((__packed__)) pldm_resp_pdr_repo_sig_t
{
    pldm_base_header_t header;
    pldm_cmd_cc_t completion_code;
    uint32_t pdr_repo_sig;
}
pldm_resp_pdr_repo_sig_t;

#endif // PLATFORM_H