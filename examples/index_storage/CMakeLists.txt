
add_executable(index_storage  storage.cpp)

target_link_libraries(index_storage PRIVATE v-set)
wci_utils(index_storage)

add_custom_target(index_storage_example DEPENDS index_storage)



if ( NOT ${gcc_version_number} LESS 4006 )
  
  add_executable(index_storage11 storage11.cpp)

  add_custom_target(
    index_storage_example11 DEPENDS    
    index_storage11   
  )

endif()
