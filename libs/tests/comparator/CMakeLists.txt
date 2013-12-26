add_executable(
  test_comparator
  ${FAS_TESTING_CPP}
  test_comparator.cpp
  comparator_suite.cpp
)

add_test(test_comparator test_comparator)



