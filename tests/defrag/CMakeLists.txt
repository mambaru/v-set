add_executable(
  test_defrag 
    test_defrag.cpp
    defrag_suite.cpp
)

wci_test(TARGET test_defrag COMMAND  test_defrag)
target_link_libraries(test_defrag PRIVATE v-set fas::testing)
