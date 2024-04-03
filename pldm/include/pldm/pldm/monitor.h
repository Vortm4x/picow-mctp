#ifndef PLATFORM_H
#define PLATFORM_H

#include <pldm/pldm.h>

typedef enum __attribute__ ((__packed__)) pldm_event_class_t
{
    PLDM_EVENT_CLASS_SENSOR                   = 0x00,
    PLDM_EVENT_CLASS_EFFECTER                 = 0x01,
    PLDM_EVENT_CLASS_REDFISH_EXE              = 0x02,
    PLDM_EVENT_CLASS_REDFISH_MSG              = 0x03,
    PLDM_EVENT_CLASS_PDR_REPO_CHANGED         = 0x04,
    PLDM_EVENT_CLASS_PLDM_MSG_POLL            = 0x05,
    PLDM_EVENT_CLASS_KEEPALIVE_ELAPSED        = 0x06,
}
pldm_event_class_t;



#endif // PLATFORM_H