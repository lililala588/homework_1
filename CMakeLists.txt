cmake_minimum_required(VERSION 3.16)
project(train_1)

set(CMAKE_CXX_STANDARD 14)


add_subdirectory("googletest")
include_directories(${CMAKE_SOURCE_DIR}/googletest/googletest/include)
include_directories(${CMAKE_SOURCE_DIR}/googletest/googlemock/include)


include_directories(
  ${CMAKE_SOURCE_DIR}/src
)

add_subdirectory("src")
add_subdirectory("test")

set(TARGET_LIB src_lib)
set(MAIN_EXEC_NAME GenerateGraph)

add_executable(${MAIN_EXEC_NAME} main.cc)
target_link_libraries(${MAIN_EXEC_NAME} ${TARGET_LIB})