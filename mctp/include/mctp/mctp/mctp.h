#ifndef MCTP_H
#define MCTP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <mctp/uuid.h>

#define MCTP_VERSION                0x01
#define MCTP_EID_NULL	            0x00
#define MCTP_EID_BROADCAST          0xFF

struct mctp_inst_t;
typedef struct mctp_inst_t mctp_inst_t;

struct mctp_binding_t;
typedef struct mctp_binding_t mctp_binding_t;

typedef uint8_t mctp_eid_t;


typedef enum __attribute__ ((__packed__)) mctp_binding_type_t
{
    MCTP_BINDING_TYPE_RESERVED  = 0x00,
    MCTP_BINDING_TYPE_SMBUS     = 0x01,
    MCTP_BINDING_TYPE_PCIE_VDM  = 0x02,
    MCTP_BINDING_TYPE_USB       = 0x03,
    MCTP_BINDING_TYPE_KCS       = 0x04,
    MCTP_BINDING_TYPE_SERIAL    = 0x05,
}
mctp_binding_type_t;

typedef enum __attribute__ ((__packed__)) mctp_physical_medium_t
{
    MCTP_PHYSICAL_MEDIUM_UNSPEC                 = 0x00,
    MCTP_PHYSICAL_MEDIUM_SMBUS_20_100KHZ        = 0x01,
    MCTP_PHYSICAL_MEDIUM_SMBUS_20_I2C_100KHZ    = 0x02,
    MCTP_PHYSICAL_MEDIUM_I2C_STANDARD_MODE      = 0x03,
    MCTP_PHYSICAL_MEDIUM_SMBUS30_400KHZ         = 0x04,
    MCTP_PHYSICAL_MEDIUM_I2C_FAST_MODE          = 0x04,
    MCTP_PHYSICAL_MEDIUM_SMBUS30_1000KHZ        = 0x05,
    MCTP_PHYSICAL_MEDIUM_I2C_FAST_MODE_PLUS     = 0x05,    
    MCTP_PHYSICAL_MEDIUM_I2C_HIGH_SPEED_MODE    = 0x06,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV11             = 0x08,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV20             = 0x09,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV21             = 0x0A,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV3X             = 0x0B,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV4X             = 0x0C,
    MCTP_PHYSICAL_MEDIUM_PCIE_REV5X             = 0x0D,
    MCTP_PHYSICAL_MEDIUM_CXL_1X                 = 0x0D,
    MCTP_PHYSICAL_MEDIUM_CXL_2X                 = 0x0D,
    MCTP_PHYSICAL_MEDIUM_PCI                    = 0x0F,
    MCTP_PHYSICAL_MEDIUM_USB_10                 = 0x10,
    MCTP_PHYSICAL_MEDIUM_USB_20                 = 0x11,
    MCTP_PHYSICAL_MEDIUM_USB_30                 = 0x12,
    MCTP_PHYSICAL_MEDIUM_NC_SI_OVER_RBT         = 0x18,
    MCTP_PHYSICAL_MEDIUM_KCS_LEGACY             = 0x20,
    MCTP_PHYSICAL_MEDIUM_KCS_PCI                = 0x21,
    MCTP_PHYSICAL_MEDIUM_SERIAL_HOST_LEGACY     = 0x22,
    MCTP_PHYSICAL_MEDIUM_SERIAL_HOST_PCI        = 0x23,
    MCTP_PHYSICAL_MEDIUM_ASYNC_SERIAL           = 0x24,
    MCTP_PHYSICAL_MEDIUM_I3C                    = 0x30,
}
mctp_physical_medium_t;

typedef enum __attribute__ ((__packed__)) mctp_msg_type_t
{
    MCTP_MSG_TYPE_CONTROL       = 0x00,
    MCTP_MSG_TYPE_PLDM          = 0x01,
    MCTP_MSG_TYPE_NCSI          = 0x02,
    MCTP_MSG_TYPE_ETHERNET      = 0x03,
    MCTP_MSG_TYPE_NVM_EXPRESS   = 0x04,
    MCTP_MSG_TYPE_VENDOR_PCI    = 0x7E,
    MCTP_MSG_TYPE_VENDOR_IANA   = 0x7F,
    MCTP_MSG_TYPE_BASE_SPEC     = 0xFF,
}
mctp_msg_type_t;

typedef enum __attribute__ ((__packed__)) mctp_host_interface_type_t
{
    MCTP_HOST_IF_KCS        = 0x02,
    MCTP_HOST_UART_8250     = 0x03,
    MCTP_HOST_UART_16450    = 0x04,
    MCTP_HOST_UART_16550    = 0x05,
    MCTP_HOST_UART_16550A   = 0x05,
    MCTP_HOST_UART_16650    = 0x06,
    MCTP_HOST_UART_16650A   = 0x06,
    MCTP_HOST_UART_16750    = 0x07,
    MCTP_HOST_UART_16750A   = 0x07,
    MCTP_HOST_UART_16850    = 0x08,
    MCTP_HOST_UART_16850A   = 0x08,
    MCTP_HOST_I2C           = 0x09,
    MCTP_HOST_SMBUS         = 0x09,
    MCTP_HOST_I3C           = 0x0A,
    MCTP_HOST_PCIe_VDM      = 0x0B,
    MCTP_HOST_MMBI          = 0x0C,
}
mctp_host_interface_type_t;

typedef struct __attribute__ ((__packed__)) mctp_generic_header_t
{
    uint8_t type : 7;
    bool integrity_check : 1;
}
mctp_generic_header_t;

typedef void (*mctp_message_rx_t)(
    mctp_inst_t* mctp_inst,
    mctp_binding_t* core_binding,
    mctp_eid_t receiver,
    mctp_eid_t sender,
    uint8_t message_tag,
    bool tag_owner,
    uint8_t* message,
    size_t message_len,
    void* args
);


mctp_inst_t* mctp_init();

void mctp_destroy(
    mctp_inst_t* mctp_inst
);

void mctp_register_bus(
    mctp_inst_t* mctp_inst, 
    mctp_binding_t *binding
);

void mctp_unregister_bus(
    mctp_inst_t* mctp_inst, 
    mctp_binding_t *binding
);

void mctp_set_bus_eid(
    mctp_binding_t *binding,
    mctp_eid_t eid,
    bool is_assigned
);

mctp_eid_t mctp_get_bus_eid(
    mctp_binding_t *binding
);

bool mctp_is_bus_eid_assigned(
    mctp_binding_t *binding
);

void mctp_get_bus_uuid(
    mctp_binding_t *binding,
    mctp_uuid_t* uuid
);

void mctp_set_ctrl_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_message_rx_t ctrl_message_rx_callback,
    void* ctrl_message_rx_args
);

void mctp_set_pldm_message_rx_callback(
    mctp_inst_t* mctp_inst,
    mctp_message_rx_t pldm_message_rx_callback,
    void* pldm_message_rx_args
);

void mctp_message_tx(
    mctp_inst_t* mctp_inst, 
    mctp_eid_t destination, 
    bool tag_owner,
	uint8_t message_tag, 
    uint8_t* message, 
    size_t message_len
);

#endif //MCTP_H