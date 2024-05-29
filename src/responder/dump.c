#include "dump.h"
#include <stdio.h>

void mctp_dump_transport(
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner
)
{
    printf("Transport info\n");
    printf("Sender: 0x%02X\n", sender);
    printf("Receiver: 0x%02X\n", receiver);
    printf("Tag owner: %s\n", tag_owner ? "YES" : "NO");
    printf("Message tag: %d\n", message_tag);
    printf("\n");
}

void mctp_dump_ctrl(
    mctp_ctrl_header_t* ctrl_header
)
{
    printf("Control info\n");
    printf("Integrity check: %s\n", ctrl_header->base.integrity_check ? "YES" : "NO");
    printf("Command: 0x%02X\n", ctrl_header->command);
    printf("Datagram: %s\n", ctrl_header->datagram ? "YES" : "NO");
    printf("Request: %s\n", ctrl_header->request ? "YES" : "NO");
    printf("Instance: %u\n", ctrl_header->instance);
    printf("\n");
}

void pldm_dump_base(
    pldm_base_header_t* base_header
)
{
    printf("PLDM info\n");
    printf("Version: %u\n", base_header->version);
    printf("PLDM type: %u\n", base_header->pldm_type);
    printf("Command: 0x%02X\n", base_header->command);
    printf("Datagram: %s\n", base_header->datagram ? "YES" : "NO");
    printf("Request: %s\n", base_header->request ? "YES" : "NO");
    printf("Instance: %u\n", base_header->instance);
    printf("\n");
}