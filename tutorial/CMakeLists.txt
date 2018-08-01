
include_directories(.)
include_directories(..)
include_directories(../../faslib/)

add_executable(hitlist1 hitconsole.cpp hitlist1.cpp)

add_custom_target(
    tutorial DEPENDS    
    hitconsole
)
