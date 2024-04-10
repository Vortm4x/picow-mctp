#include "pldm_platform_handler.h"
#include "dump.h"
#include <pldm/pldm.h>
#include <pldm/platform.h>
#include <pldm/pdr/term_loc.h>
#include <pldm/pdr/num_sens.h>
#include <pldm/pdr/redfish_resource.h>

void pldm_platform_message_rx_callback(
    pldm_inst_t* pldm_inst,
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len,
    void* args
)
{
    pldm_base_header_t* base_header = (pldm_base_header_t*) message;

    pldm_dump_base(base_header);

    switch (base_header->command)
    {


        default:
        {
            if(base_header->request)
            {
                pldm_resp_error_tx(
                    transport,
                    base_header,
                    PLDM_CMD_CC_ERROR_UNSUPPORTED_CMD
                );
            }
        }
        break;
    }
}


void handle_req_num_sens_get_reading(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
)
{

}

void handle_req_pdr_repo_info(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
)
{

}

void handle_req_pdr_repo_get(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
)
{

}

void handle_req_pdr_repo_sig(
    pldm_transport_t* transport,
    uint8_t* message,
    size_t message_len
)
{

}