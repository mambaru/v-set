cmake_minimum_required(VERSION 2.6)

project(v-set CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")
include(ConfigureLibrary)

add_subdirectory(faslib)
add_subdirectory(tests)
add_subdirectory(examples)
add_subdirectory(benchmark)
