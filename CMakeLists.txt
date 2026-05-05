cmake_minimum_required(VERSION 3.20)

project(koda)

add_executable(koda)

target_sources(
    koda
    PRIVATE
        src/main.cpp
        src/editor_fsm.cpp
        src/gap_buffer.cpp
)

