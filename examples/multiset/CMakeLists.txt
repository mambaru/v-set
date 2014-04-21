
add_executable(multiset EXCLUDE_FROM_ALL multiset.cpp)

add_custom_target(
    multiset_example DEPENDS    
    multiset   
)
