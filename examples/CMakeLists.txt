
include_directories(${CMAKE_SOURCE_DIR}/faslib)
include_directories(${CMAKE_SOURCE_DIR})

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

add_subdirectory(compare)
add_subdirectory(multiset)
add_subdirectory(pointer_storage)
add_subdirectory(index_storage)

add_custom_target(
    examples DEPENDS    
    compare_example
    multiset_example
    index_storage_example
    pointer_storage_example
)