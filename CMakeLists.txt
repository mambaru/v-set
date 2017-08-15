cmake_minimum_required(VERSION 2.6)

project(v-set CXX)

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#include(VSet)

include_directories(faslib)
include_directories(${CMAKE_SOURCE_DIR}/../faslib)

add_subdirectory(tests)
add_subdirectory(examples)
add_subdirectory(benchmark)
