add_executable(
  test_multiset 
  ${FAS_TESTING_CPP}
  test_multiset.cpp
  multiset_suite.cpp
  multiset2_suite.cpp
  multiset_mmap_suite.cpp
  multiset_test_impl.cpp
  multiset_mmap_cmp_suite.cpp
)

add_test(NAME test_multiset COMMAND test_multiset)



