
add_executable(compare1 compare1.cpp)
add_executable(compare2 compare2.cpp)

add_custom_target(
    compare_example DEPENDS    
    compare1
    compare2
)
