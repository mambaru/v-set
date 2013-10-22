include_directories(../../faslib2)
include_directories(../../cdaemon/v-set)
include_directories(/home/migashko/distr/stx-btree-0.8.6/include)

SET(CMAKE_CXX_FLAGS "-W -Wall -Werror -pedantic -fno-rtti -Wno-long-long -std=c++11 -Wno-unused-variable")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g -DNDEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")


add_executable(multiset_insert multiset_insert.cpp)
add_executable(stx_insert  stx_insert.cpp)
#add_executable(stx_insert2  stx_insert2.cpp)
#add_executable(vset_insert vset_insert.cpp)
#add_executable(vset_insert2 vset_insert2.cpp)
# not support reverse_iterator::base
#add_executable(vset_insert3 vset_insert3.cpp)
add_executable(vmultiset vmultiset.cpp)
add_executable(vmultiset2 vmultiset2.cpp)
add_executable(vmultiset3 vmultiset3.cpp)
add_executable(compare compare.cpp)
