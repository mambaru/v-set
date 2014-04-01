add_executable(
  test_defrag
  test_defrag.cpp
  ../../faslib/fas/testing/testing.cpp
  defrag_suite.cpp
)

add_test(test_multiset test_multiset)
