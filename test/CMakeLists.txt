cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE TEST_SRC "*.cc" "*.h")

add_executable(test ${TEST_SRC})

target_link_libraries(test gmock_main src_lib)