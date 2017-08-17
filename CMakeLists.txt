cmake_minimum_required(VERSION 2.6)

project(v-set CXX)

#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#include(VSet)

if ( BUILD_TESTING )

  if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Werror -pedantic -fno-rtti -std=c++11")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")
    if (PARANOID_WARNING)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-align \
                         -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization\
                         -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations \
                         -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual \
                         -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
                          -Wswitch-default -Wundef")
                        #-Wstrict-overflow=5
                         #-Wno-stringop-overflow
    endif(PARANOID_WARNING)
  endif (UNIX)

  include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  include_directories(${CMAKE_SOURCE_DIR}/../faslib)

  enable_testing()
  add_subdirectory(tests)
  #add_subdirectory(examples)
  #add_subdirectory(benchmark)
  
endif()
