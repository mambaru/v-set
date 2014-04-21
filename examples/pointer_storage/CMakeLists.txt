
add_executable(pointer_storage EXCLUDE_FROM_ALL storage.cpp)

add_custom_target(
    pointer_storage_example DEPENDS    
    pointer_storage   
)
