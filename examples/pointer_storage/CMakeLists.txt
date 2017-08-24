
add_executable(pointer_storage storage.cpp)

add_custom_target(
    pointer_storage_example DEPENDS    
    pointer_storage   
)
