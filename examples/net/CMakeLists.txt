cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

add_executable(TcpServerExample UseTcpServer.cpp)
target_link_libraries(TcpServerExample Fl::net)