add_executable(
  test_vtree
  test_vtree.cpp
  ../../faslib/fas/testing/testing.cpp
  array_suite.cpp
  vtree_basic_suite.cpp
  vtree_mmap_suite.cpp
)

add_test(test_vtree test_vtree)