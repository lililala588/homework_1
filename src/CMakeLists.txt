cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SRC_FILE "*.cc" "*.h")

add_library(src_lib ${SRC_FILE})