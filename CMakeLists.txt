cmake_minimum_required(VERSION 2.6)

project(v-set CXX)

#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#include(VSet)

if ( BUILD_TESTING )

  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Werror -pedantic -fno-rtti")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")
    if (PARANOID_WARNING)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-align \
                         -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization\
                         -Wformat=2 -Winit-self  -Wmissing-declarations \
                         -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual \
                         -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
                         -Wswitch-default -Wundef -Wstrict-overflow=1")
      #-Wno-stringop-overflow -Wstrict-overflow=5 -Wlogical-op  -Wnoexcept -Wstrict-null-sentinel
      if ( CMAKE_CXX_STANDARD EQUAL 98)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-overflow=5 -Wno-stringop-overflow")
      endif()

    endif(PARANOID_WARNING)
  endif (UNIX)

  include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  include_directories(${CMAKE_SOURCE_DIR}/../faslib)

  enable_testing()
  add_subdirectory(tests)
  add_subdirectory(examples)
  add_subdirectory(benchmark)
  if ( CMAKE_CXX_STANDARD GREATER 98)
    add_subdirectory(tutorial)
  endif()
  
endif()
