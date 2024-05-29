#ifndef PLDM_TERMINUS_H
#define PLDM_TERMINUS_H

#include <pldm/types.h>
#include <pldm/multipart.h>


struct pldm_transport_t;
typedef struct pldm_transport_t pldm_transport_t;

typedef void (*pldm_tid_changed_t)(
    pldm_transport_t* transport,
    void* args
);


pldm_tid_t pldm_terminus_get_id(
    pldm_transport_t* transport
);

void pldm_terminus_set_id(
    pldm_transport_t* transport,
    pldm_tid_t tid
);

void pldm_terminus_set_id_changed_callback(
    pldm_transport_t* transport,
    pldm_tid_changed_t pldm_tid_changed_callback,
    void* pldm_tid_changed_args
);


pldm_multipart_t*  pldm_terminus_add_multipart_outcomming(
    pldm_transport_t* transport,
    uint8_t data[],
    uint16_t data_len
);

pldm_multipart_t*  pldm_terminus_add_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle
);

pldm_multipart_t* pldm_terminus_get_multipart(
    pldm_transport_t* transport,
    uint32_t xfer_handle,
    bool is_outcomming
);

void pldm_terminus_remove_multipart(
    pldm_transport_t* transport,
    uint32_t xfer_handle,
    bool is_outcomming
);


/*
pldm_multipart_t*  pldm_terminus_add_multipart_outcomming(
    pldm_transport_t* transport,
    uint8_t data[],
    uint16_t data_len
);

void pldm_terminus_remove_multipart_outcomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
);

pldm_multipart_t* pldm_terminus_get_multipart_outcomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
);

pldm_multipart_t*  pldm_terminus_add_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle
);

void pldm_terminus_remove_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
);

pldm_multipart_t* pldm_terminus_get_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
);
*/

/*
pldm_multipart_outcomming_t*  pldm_terminus_add_multipart_outcomming(
    pldm_transport_t* transport,
    uint8_t data[],
    uint16_t data_len
);

void pldm_terminus_remove_multipart_outcomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
);

pldm_multipart_outcomming_t* pldm_terminus_get_multipart_outcomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
);




pldm_multipart_incomming_t*  pldm_terminus_add_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle
);

void pldm_terminus_remove_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
);

pldm_multipart_incomming_t* pldm_terminus_get_multipart_incomming(
    pldm_transport_t* transport,
    uint32_t xfer_handle 
);

*/

#endif // PLDM_TERMINUS_H