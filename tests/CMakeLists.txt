project(tests)

include_directories(../faslib)
include_directories(../)

ENABLE_TESTING()
add_subdirectory(buffer)
add_subdirectory(memory)
add_subdirectory(vtree)
add_subdirectory(multiset)
add_subdirectory(comparator)
add_subdirectory(defrag)
