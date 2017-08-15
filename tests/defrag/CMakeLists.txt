add_executable(
  test_defrag 
  ${FAS_TESTING_CPP}
  test_defrag.cpp
  defrag_suite.cpp
)

add_test(test_defrag test_defrag)
