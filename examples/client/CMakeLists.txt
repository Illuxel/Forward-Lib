cmake_minimum_required(VERSION 3.24)

add_executable(ClientAPIExample UseClientAPI.cpp)
target_link_libraries(ClientAPIExample Fl::client)