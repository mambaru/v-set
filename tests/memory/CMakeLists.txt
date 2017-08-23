add_executable(
  test_memory
  ${FAS_TESTING_CPP}
  test_memory.cpp
  basic_suite.cpp 
  manager_suite.cpp 
)

add_test(test_memory test_memory)



