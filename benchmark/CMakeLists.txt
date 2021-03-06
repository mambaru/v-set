if(USE_STX)
  clone_library(stx-btree "STX_DIR" "https://github.com/bingmann/stx-btree.git" "")
  if(STX_DIR)
    set(HAVE_INCLUDE_stx-btree "${STX_DIR}/include")
    include_directories(${HAVE_INCLUDE_stx-btree})
  endif()
endif()

add_executable(multiset_insert  multiset_insert.cpp)
add_executable(vset_insert  vset_insert.cpp)
add_executable(vset_insert2  vset_insert2.cpp)
# not support reverse_iterator::base
# add_executable(vset_insert3  vset_insert3.cpp)
add_executable(vmultiset  vmultiset.cpp)
add_executable(vmultiset2  vmultiset2.cpp)
add_executable(vmultiset3  vmultiset3.cpp)
add_executable(compare1_bench  compare/compare1.cpp)
add_executable(compare2_bench  compare/compare2.cpp compare/cmp3fe.cpp)


target_link_libraries(multiset_insert PUBLIC v-set)
target_link_libraries(vset_insert PUBLIC v-set)
target_link_libraries(vset_insert2 PUBLIC v-set)
target_link_libraries(vmultiset PUBLIC v-set)
target_link_libraries(vmultiset2 PUBLIC v-set)
target_link_libraries(vmultiset3 PUBLIC v-set)
target_link_libraries(compare1_bench PUBLIC v-set)
target_link_libraries(compare2_bench PUBLIC v-set)

wci_utils(multiset_insert vset_insert vset_insert2 vmultiset vmultiset2 vmultiset3 compare1_bench compare2_bench)

#include(ConfigureLibrary)
#CONFIGURE_LIBRARY(stx/btree_multiset.h "/usr/include /usr/local/include" stx-btree "")

if(HAVE_INCLUDE_stx-btree)
  set(CMAKE_CXX_FLAGS "-Wall")  
  add_executable(stx_insert  stx_insert.cpp)
  add_executable(stx_insert2  stx_insert2.cpp)
  #target_compile_options(stx_insert BEFORE  PRIVATE -Wall)
  SET( 
    stx_sources 
    stx_insert
    stx_insert2 
  )
  MESSAGE(STATUS "Building stx-btree dependant sources")
else(HAVE_INCLUDE_stx-btree)
  SET( stx_sources "" )
endif(HAVE_INCLUDE_stx-btree)

