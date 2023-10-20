cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(BENCH_SOURCE
    bench_main.cpp
)

add_executable(BenchClient ${BENCH_SOURCE})

target_link_libraries(BenchClient
    benchmark::benchmark
    Forward::Client
)