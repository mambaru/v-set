project(tests)

include_directories(../faslib)
include_directories(../)

SET(FAS_TESTING_CPP ${CMAKE_SOURCE_DIR}/faslib/fas/testing/testing.cpp)

if(UNIX)
  SET(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror -pedantic -fno-rtti -Wno-long-long -Wno-unused-variable -std=c++11")
  SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g -DNDEBUG")
  SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g -fno-inline-small-functions")
elseif(MSVC)
  SET(CMAKE_CXX_FLAGS "/GR- /W3 /wd4503 /wd4307 /EHsc /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS")
  SET(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
  SET(CMAKE_CXX_FLAGS_DEBUG  "/Od /D_DEBUG")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "/Yd /O2 /DNDEBUG")
else(UNIX)

endif(UNIX)

ENABLE_TESTING()

add_subdirectory(buffer)
add_subdirectory(memory)
add_subdirectory(vtree)
add_subdirectory(multiset)
add_subdirectory(comparator)
add_subdirectory(defrag)

add_custom_target(
    tests DEPENDS
    test_buffer
    test_memory
    test_vtree
    test_multiset
    test_comparator
    test_defrag
)
