
add_executable(index_storage EXCLUDE_FROM_ALL storage.cpp)

add_custom_target(
    index_storage_example DEPENDS    
    index_storage   
)
