add_executable(
  test_multiset EXCLUDE_FROM_ALL
  ${FAS_TESTING_CPP}
  test_multiset.cpp
  multiset_suite.cpp
  multiset2_suite.cpp
  multiset_mmap_suite.cpp
  multiset_test_impl.cpp
  multiset_mmap_cmp_suite.cpp
)

add_test(test_multiset test_multiset)



