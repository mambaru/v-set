include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../v-set")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../faslib2")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_library(votes votes.cpp)

add_executable(
  votes_example
  votes_example.cpp
)

target_link_libraries(votes_example votes)


add_executable( my_test
  test.cpp
  )
target_link_libraries( my_test votes )




#add_executable(
#  test_vtree
#  test_vtree.cpp
#  ${FASLIB_DIR}/fas/testing/testing.cpp
#  array_suite.cpp
#  vtree_basic_suite.cpp
#  vtree_mmap_suite.cpp
#)

#add_test(test_vtree test_vtree)



