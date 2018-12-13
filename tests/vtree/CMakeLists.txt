add_executable(
  test_vtree 
  ${FAS_TESTING_CPP}
  test_vtree.cpp
  array_suite.cpp
  vtree_basic_suite.cpp
  vtree_mmap_suite.cpp
  vtree_ctor_suite.cpp
  vtree_count_suite.cpp
  vtree_compare_suite.cpp
)

add_test(NAME test_vtree COMMAND test_vtree)
