
if ( NOT FASLIB_DIR  )
  set(FASLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../faslib)
endif()


include_directories(${FASLIB_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
set(FAS_TESTING_CPP "${FASLIB_DIR}/fas/testing/testing.cpp")

if(UNIX)
  SET(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror -pedantic -fno-rtti -Wno-long-long -Wno-unused-variable -std=c++11")
  SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g -DNDEBUG")
  SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g -fno-inline-small-functions")
elseif(MSVC)
  SET(CMAKE_CXX_FLAGS "/GR- /W3 /wd4503 /wd4307 /EHsc /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS")
  SET(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
  SET(CMAKE_CXX_FLAGS_DEBUG  "/Od /D_DEBUG")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "/Yd /O2 /DNDEBUG")
else(UNIX)

endif(UNIX)

add_subdirectory(tests)
add_subdirectory(examples)
add_subdirectory(benchmark)