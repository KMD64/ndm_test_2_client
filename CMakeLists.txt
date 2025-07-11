cmake_minimum_required(VERSION 3.0)

project(ndm_test_2_client)


add_executable(ndm_test_2_client main.cpp)
find_library(json11 "libjson11.so")
target_link_libraries(ndm_test_2_client json11)
target_include_directories(ndm_test_2_client PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc")
target_sources(ndm_test_2_client PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/genlclient.cpp")



install(TARGETS ndm_test_2_client RUNTIME DESTINATION bin)
