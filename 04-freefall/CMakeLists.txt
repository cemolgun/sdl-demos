cmake_minimum_required(VERSION 3.16)

project(04-freefall)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(../SDL SDL)

add_executable(
    ${PROJECT_NAME}
    main.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_compile_definitions(${PROJECT_NAME} PRIVATE APP_TITLE="${PROJECT_NAME}")
