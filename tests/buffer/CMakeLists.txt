ENABLE_TESTING()

add_executable(
  test_buffer EXCLUDE_FROM_ALL
  ${FAS_TESTING_CPP}
  test_buffer.cpp
  basic_suite.cpp 
)

add_test(ctest_buffer test_buffer)