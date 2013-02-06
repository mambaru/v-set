add_executable(
  test_multiset
  test_multiset.cpp
  ${FASLIB_DIR}/fas/testing/testing.cpp
  multiset_suite.cpp
  multiset2_suite.cpp
  multiset_mmap_suite.cpp
)

add_test(test_multiset test_multiset)



