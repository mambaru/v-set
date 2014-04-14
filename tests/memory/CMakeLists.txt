add_executable(
  test_memory EXCLUDE_FROM_ALL
  ${FAS_TESTING_CPP}
  test_memory.cpp
  basic_suite.cpp 
)

add_test(test_memory test_memory)



