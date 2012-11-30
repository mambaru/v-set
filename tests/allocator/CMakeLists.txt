add_executable(
  test_allocator
  test_allocator.cpp
  ${FASLIB_DIR}/fas/testing/testing.cpp
  basic_suite.cpp 
)

add_test(test_allocator test_allocator)



