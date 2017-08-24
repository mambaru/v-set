
add_executable(multiset  multiset.cpp)

add_custom_target(
    multiset_example DEPENDS    
    multiset   
)
