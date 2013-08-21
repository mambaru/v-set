add_executable(
  test_memory
  test_memory.cpp
  ${FASLIB_DIR}/fas/testing/testing.cpp
  basic_suite.cpp 
)

add_test(test_memory test_memory)



