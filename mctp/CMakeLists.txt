cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

project(mctp C CXX)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library (${PROJECT_NAME} STATIC 
    src/mctp.c
    src/serial.c
    src/packet_buffer.c
    src/message_ctx.c
)

target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_uart
)

target_include_directories (${PROJECT_NAME}
    PRIVATE ${PROJECT_ROOT}/include/mctp/mctp
    PRIVATE ${PROJECT_ROOT}/include/mctp
)   

target_include_directories (${PROJECT_NAME}
    PUBLIC ${PROJECT_ROOT}/include/mctp
)
