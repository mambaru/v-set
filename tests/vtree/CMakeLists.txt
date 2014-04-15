add_executable(
  test_vtree EXCLUDE_FROM_ALL
  ${FAS_TESTING_CPP}
  test_vtree.cpp
  array_suite.cpp
  vtree_basic_suite.cpp
  vtree_mmap_suite.cpp
  vtree_ctor_suite.cpp
)

add_test(test_vtree test_vtree)
