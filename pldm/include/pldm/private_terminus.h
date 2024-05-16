#ifndef PLDM_PRIVATE_TERMINUS_H
#define PLDM_PRIVATE_TERMINUS_H

#include <pldm/pldm.h>
#include <pldm/multipart.h>


typedef struct pldm_terminus_t
{
    pldm_transport_t* transport;
    pldm_inst_t* pldm_inst;
    pldm_tid_t tid;
    pldm_tid_changed_t pldm_tid_changed_callback;
    void* pldm_tid_changed_args; 
    uint32_t next_xfer_handle;
    pldm_multipart_outcomming_t* multipart_out_first;
    pldm_multipart_outcomming_t* multipart_out_last;
}
pldm_terminus_t;

uint32_t pldm_terminus_next_xfer_handle(
    pldm_terminus_t* endpoint
);





#endif // PLDM_PRIVATE_TERMINUS_H