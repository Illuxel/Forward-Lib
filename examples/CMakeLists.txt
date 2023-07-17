cmake_minimum_required(VERSION 3.24)

set(EXAMPLE_SOURCE
    bench_stringbuilder.cpp
)

add_executable(ForwardExamples ${EXAMPLE_SOURCE})

target_link_libraries(ForwardExamples
PUBLIC
    Fl::client
)