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

# Tests -------------------------
find_package(GTest REQUIRED)
enable_testing()

add_executable(koda_tests
    tests/gap_buffer_test.cpp
    src/gap_buffer.cpp
)

target_include_directories(koda_tests PRIVATE src)
target_link_libraries(koda_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(koda_tests)
