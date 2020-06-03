add_executable(
  test_memory
    test_memory.cpp
    basic_suite.cpp 
    manager_suite.cpp 
)

wci_test(TARGET test_memory COMMAND test_memory)
target_link_libraries(test_memory PRIVATE v-set fas::testing)



