add_executable(
  test_multiset test_multiset.cpp
    multiset_suite.cpp
    multiset2_suite.cpp
    multiset_mmap_suite.cpp
    multiset_test_impl.cpp
    multiset_mmap_cmp_suite.cpp
)

wci_test(TARGET test_multiset COMMAND test_multiset)
target_link_libraries(test_multiset PRIVATE v-set fas::testing)
