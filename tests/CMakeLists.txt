add_executable(qp_test qp_test.cpp)
target_link_libraries(qp_test qp)
add_test(NAME qp_test COMMAND $<TARGET_FILE:qp_test>)