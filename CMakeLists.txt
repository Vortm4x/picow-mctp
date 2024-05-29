cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)

project(pico-uart-responder C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(lwip_import.cmake)

pico_sdk_init()


add_subdirectory(mctp)
add_subdirectory(pldm)


add_executable(${PROJECT_NAME}
    src/main.c
    
    src/dht11/driver.c

    src/responder/dump.c
    src/responder/mctp/control_handler.c
    src/responder/pldm/base_handler.c
    src/responder/pldm/platform_handler.c
    src/responder/pldm/redfish_handler.c
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    pico_stdlib
    hardware_uart
    pico_cyw43_arch_none
    LWIP_PORT
    mctp
    pldm
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

# Run the entire project in SRAM
# pico_set_binary_type(pico-freertos copy_to_ram)

pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)