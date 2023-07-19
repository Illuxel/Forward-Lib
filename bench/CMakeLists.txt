cmake_minimum_required(VERSION 3.24)

set(BENCH_SOURCE
    
)

add_executable(BenchClient ${BENCH_SOURCE})

target_link_libraries(BenchClient
    benchmark::benchmark
    Fl::client
)