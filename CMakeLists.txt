cmake_minimum_required(VERSION 2.6)

project(v-set CXX)

SET(FASLIB_DIR "${CMAKE_SOURCE_DIR}/../faslib2")

include_directories(${FASLIB_DIR})
include_directories(${CMAKE_SOURCE_DIR})

if(UNIX)
  SET(CMAKE_CXX_FLAGS "-W -Wall -Werror -pedantic -fno-rtti -Wno-long-long -Wno-unused-variable -std=c++11")
 #-std=c++11
  SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g -DNDEBUG")
  SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")
elseif(MSVC)
  SET(CMAKE_CXX_FLAGS "/GR- /W3 /wd4503 /wd4307 /EHsc /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS")
  SET(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
  SET(CMAKE_CXX_FLAGS_DEBUG  "/Od /D_DEBUG")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "/Yd /O2 /DNDEBUG")
else(UNIX)

endif(UNIX)

add_subdirectory(tests)
add_subdirectory(examples)