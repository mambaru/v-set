add_executable(
  test_comparator EXCLUDE_FROM_ALL
  ${FAS_TESTING_CPP}
  test_comparator.cpp
  comparator_suite.cpp
)

add_test(test_comparator test_comparator)



