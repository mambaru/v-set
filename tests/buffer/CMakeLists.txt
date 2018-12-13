ENABLE_TESTING()

add_executable(
  test_buffer 
  ${FAS_TESTING_CPP}
  test_buffer.cpp
  basic_suite.cpp 
)

add_test(NAME ctest_buffer COMMAND test_buffer)
