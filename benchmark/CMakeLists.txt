#include_directories(../../faslib)
#include_directories(../faslib)
#include_directories(../)

#if(UNIX)
#  SET(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror -pedantic -fno-rtti -Wno-long-long -Wno-unused-variable -std=c++11")
#  SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")
#  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g -DNDEBUG")
#  SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g -fno-inline-small-functions")
#elseif(MSVC)
#  SET(CMAKE_CXX_FLAGS "/GR- /W3 /wd4503 /wd4307 /EHsc /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS")
#  SET(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
#  SET(CMAKE_CXX_FLAGS_DEBUG  "/Od /D_DEBUG")
#  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "/Yd /O2 /DNDEBUG")
#else(UNIX)
#
#endif(UNIX)

add_executable(multiset_insert  multiset_insert.cpp)
add_executable(vset_insert  vset_insert.cpp)
add_executable(vset_insert2  vset_insert2.cpp)
# not support reverse_iterator::base
#add_executable(vset_insert3  vset_insert3.cpp)
add_executable(vmultiset  vmultiset.cpp)
add_executable(vmultiset2  vmultiset2.cpp)
add_executable(vmultiset3  vmultiset3.cpp)
add_executable(compare  compare.cpp)

#include(ConfigureLibrary)
#CONFIGURE_LIBRARY(stx/btree_multiset.h "/usr/include /usr/local/include" stx-btree "")

if(HAVE_INCLUDE_stx-btree)
  add_executable(stx_insert  stx_insert.cpp)
  add_executable(stx_insert2  stx_insert2.cpp)
  SET( 
    stx_sources 
    stx_insert
    stx_insert2 
  )
  MESSAGE(STATUS "Building stx-btree dependant sources")
else(HAVE_INCLUDE_stx-btree)
  SET( stx_sources "" )
endif(HAVE_INCLUDE_stx-btree)

add_custom_target(
    benchmark DEPENDS
    multiset_insert    
    vmultiset
    vmultiset2
    vmultiset3
    compare
        
    vset_insert
    vset_insert2
        
    ${stx_sources}
)
