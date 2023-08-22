cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(BENCH_SOURCE
    bench_stringbuilder.cpp
)

add_executable(BenchUtils ${BENCH_SOURCE})

target_link_libraries(BenchUtils
    benchmark::benchmark
    Fl::utils
)