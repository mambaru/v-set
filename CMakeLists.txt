cmake_minimum_required(VERSION 3.1)
project(v-set)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(mambaru)

if ( BUILD_TESTING )
  enable_testing()
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.3)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstringop-overflow=0")
  endif()
  add_subdirectory(tests)
  add_subdirectory(examples)
  add_subdirectory(tutorial)
  add_subdirectory(benchmark)
endif()

set(CPACK_PACKAGE_NAME "v-set")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "7")
set(CPACK_PACKAGE_VERSION_PATCH "0")
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
