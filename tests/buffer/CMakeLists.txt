add_executable(
  test_buffer 
    test_buffer.cpp
    basic_suite.cpp 
)

wci_test(TARGET test_buffer COMMAND test_buffer)
target_link_libraries(test_buffer PRIVATE v-set fas::testing)
