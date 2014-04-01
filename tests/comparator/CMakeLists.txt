add_executable(
  test_comparator
  test_comparator.cpp
  ../../faslib/fas/testing/testing.cpp
  comparator_suite.cpp
)

add_test(test_comparator test_comparator)



