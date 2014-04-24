add_executable(
  test_defrag EXCLUDE_FROM_ALL
  ${FAS_TESTING_CPP}
  test_defrag.cpp
  defrag_suite.cpp
)

add_test(test_defrag test_defrag)
