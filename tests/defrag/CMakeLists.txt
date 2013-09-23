add_executable(
  test_defrag
  test_defrag.cpp
  ${FASLIB_DIR}/fas/testing/testing.cpp
  defrag_suite.cpp
)

add_test(test_multiset test_multiset)
