add_executable(
  test_vtree
  ${FAS_TESTING_CPP}
  test_vtree.cpp
  array_suite.cpp
  vtree_basic_suite.cpp
  vtree_mmap_suite.cpp
)

add_test(test_vtree test_vtree)
