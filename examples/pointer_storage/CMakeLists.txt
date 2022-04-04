
add_executable(pointer_storage storage.cpp)
target_link_libraries(pointer_storage PRIVATE v-set)


wci_utils(pointer_storage)

add_custom_target(
    pointer_storage_example DEPENDS    
    pointer_storage   
)
