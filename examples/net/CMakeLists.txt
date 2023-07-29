cmake_minimum_required(VERSION 3.24)

add_executable(TcpServerExample UseTcpServer.cpp)
target_link_libraries(TcpServerExample Fl::net)