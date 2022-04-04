include_directories(.)

add_executable(hitlist1 hitconsole.cpp hitlist1.cpp)
add_executable(hitlist2 hitconsole.cpp hitlist2.cpp)
add_executable(hitlist3 hitconsole.cpp hitlist3.cpp)
add_executable(hitlist4 hitconsole.cpp hitlist4.cpp)
add_executable(hitlist5 hitconsole.cpp hitlist5.cpp)
add_executable(hitlist6 hitconsole.cpp hitlist6.cpp)
add_executable(hitlist7 hitconsole.cpp hitlist7.cpp)
add_executable(hitlistX hitconsole.cpp hitlistX.cpp)

target_link_libraries(hitlist1 PUBLIC v-set)
target_link_libraries(hitlist2 PUBLIC v-set)
target_link_libraries(hitlist3 PUBLIC v-set)
target_link_libraries(hitlist4 PUBLIC v-set)
target_link_libraries(hitlist5 PUBLIC v-set)
target_link_libraries(hitlist6 PUBLIC v-set)
target_link_libraries(hitlist7 PUBLIC v-set)
target_link_libraries(hitlistX PUBLIC v-set)


wci_utils(hitlist1 hitlist2 hitlist3 hitlist4 hitlist5 hitlist6 hitlist7 hitlistX)
