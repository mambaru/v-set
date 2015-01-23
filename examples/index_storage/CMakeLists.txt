
add_executable(index_storage EXCLUDE_FROM_ALL storage.cpp)

add_custom_target(
    index_storage_example DEPENDS    
    index_storage   
)

if ( NOT ${gcc_version_number} LESS 4006 )
  
  add_executable(index_storage11 EXCLUDE_FROM_ALL storage11.cpp)

  add_custom_target(
    index_storage_example11 DEPENDS    
    index_storage11   
  )

endif()