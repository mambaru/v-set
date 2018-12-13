cmake_minimum_required(VERSION 3.1)
project(v-set)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ci)

if ( STANDALONE )
  getlibs(faslib)
else()
  set(BUILD_TESTING OFF)
endif()


if ( BUILD_TESTING )
  enable_testing()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})                                                                                                                 
  include_directories(.)
  add_subdirectory(tests)
  add_subdirectory(examples)
  if ( NOT "${CMAKE_CXX_STANDARD}" STREQUAL "98")
    add_subdirectory(benchmark)
    add_subdirectory(tutorial)
  endif()
endif()

set(CPACK_PACKAGE_NAME "v-set")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "7")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/Readme.md")
set(CPACK_PACKAGE_CONTACT "migashko@gmail.com")

install(DIRECTORY v-set DESTINATION include
  PATTERN "*.cmake" EXCLUDE
  PATTERN "*~" EXCLUDE
  PATTERN ".*" EXCLUDE
  PATTERN "Makefile" EXCLUDE
  PATTERN "CMakeFiles" EXCLUDE
)

install(DIRECTORY . DESTINATION share/v-set
  PATTERN "v-set" EXCLUDE
  PATTERN "*.cmake" EXCLUDE
  PATTERN "*~*" EXCLUDE
  PATTERN "*kdev*" EXCLUDE
  PATTERN ".git*" EXCLUDE
  PATTERN "Readme.md" EXCLUDE
)

include(CPack)
