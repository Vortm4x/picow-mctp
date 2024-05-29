#ifndef DUMP_H
#define DUMP_H

#include <mctp/mctp.h>
#include <mctp/control.h>
#include <pldm/pldm.h>

void mctp_dump_transport(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner
);

void mctp_dump_ctrl(
    mctp_ctrl_header_t* ctrl_header
);

void pldm_dump_base(
    pldm_base_header_t* base_header
);

#endif // DUMP_H