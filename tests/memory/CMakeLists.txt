add_executable(
  test_memory
  test_memory.cpp
  ../../faslib/fas/testing/testing.cpp
  basic_suite.cpp 
)

add_test(test_memory test_memory)



