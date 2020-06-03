add_executable(
  test_vtree
    test_vtree.cpp
    array_suite.cpp
    vtree_basic_suite.cpp
    vtree_mmap_suite.cpp
    vtree_ctor_suite.cpp
    vtree_count_suite.cpp
    vtree_compare_suite.cpp
)

wci_test(TARGET test_vtree COMMAND test_vtree)
target_link_libraries(test_vtree PRIVATE v-set fas::testing)
