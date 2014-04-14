
add_executable(compare1 EXCLUDE_FROM_ALL compare1.cpp)
add_executable(compare2 EXCLUDE_FROM_ALL compare2.cpp)

add_custom_target(
    compare_example DEPENDS    
    compare1
    compare2
)
