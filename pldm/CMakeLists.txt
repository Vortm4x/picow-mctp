cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

project(pldm C CXX)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library (${PROJECT_NAME} STATIC
    src/pldm.c
    src/pdr.c
    src/repo.c
    src/mctp_transport.c
    src/util.c
)

target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Werror
    -Wextra 
)

target_include_directories (${PROJECT_NAME}
    PRIVATE ${PROJECT_ROOT}/include/pldm/pldm
    PRIVATE ${PROJECT_ROOT}/include/pldm
)   

target_include_directories (${PROJECT_NAME}
    PUBLIC ${PROJECT_ROOT}/include/pldm
)