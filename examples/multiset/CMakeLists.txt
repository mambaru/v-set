
add_executable(multiset  multiset.cpp)
add_executable(multiset0  multiset0.cpp)

target_link_libraries(multiset PRIVATE v-set)
target_link_libraries(multiset0 PRIVATE v-set)

wci_utils(multiset multiset0)

add_custom_target(
    multiset_example DEPENDS
    multiset 
    multiset0
)
