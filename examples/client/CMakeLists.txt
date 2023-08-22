cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

add_executable(ClientAPIExample UseClientAPI.cpp)
target_link_libraries(ClientAPIExample Fl::client)