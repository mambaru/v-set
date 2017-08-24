
include_directories(.)
include_directories(..)
include_directories(../../faslib/)

add_executable(hitconsole hitconsole.cpp)

add_custom_target(
    tutorial DEPENDS    
    hitconsole
)
