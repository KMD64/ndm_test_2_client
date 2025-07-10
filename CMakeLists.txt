cmake_minimum_required(VERSION 3.0)

project(ndm_test_2_client)

add_executable(ndm_test_2_client main.cpp)

install(TARGETS ndm_test_2_client RUNTIME DESTINATION bin)
