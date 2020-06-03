add_executable(
  test_comparator 
    test_comparator.cpp
    comparator_suite.cpp
)

wci_test(TARGET test_comparator COMMAND test_comparator)
target_link_libraries(test_comparator PRIVATE v-set fas::testing)



