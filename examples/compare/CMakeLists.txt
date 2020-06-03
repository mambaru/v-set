
add_executable(compare1 compare1.cpp)
add_executable(compare2 compare2.cpp)

target_link_libraries(compare1 PRIVATE v-set)
target_link_libraries(compare2 PRIVATE v-set)

wci_utils(compare1 compare2)

add_custom_target(
    compare_example DEPENDS    
    compare1
    compare2
)
