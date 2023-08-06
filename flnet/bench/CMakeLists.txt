cmake_minimum_required(VERSION 3.18)

set(BENCH_SOURCE
    bench_url.cpp
)

add_executable(BenchNet ${BENCH_SOURCE})

target_link_libraries(BenchNet
PUBLIC
    benchmark::benchmark
    Fl::net
)