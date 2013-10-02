include_directories(../../faslib2)
include_directories(../../cdaemon/v-set)
include_directories(/home/migashko/distr/stx-btree-0.8.6/include)

SET(CMAKE_CXX_FLAGS "-W -Wall -Werror -pedantic -fno-rtti -Wno-long-long -Wno-unused-variable -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g -DNDEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")


add_executable(compare1 compare1.cpp)
add_executable(compare2 compare2.cpp)
