add_executable(
  test_buffer
  test_buffer.cpp
  ../../faslib/fas/testing/testing.cpp
  basic_suite.cpp 
)

add_test(test_buffer test_buffer)



