cmake_minimum_required(VERSION 3.24)

set(BENCH_SOURCE
    bench_stringbuilder.cpp
)

add_executable(BenchUtils ${BENCH_SOURCE})

target_link_libraries(BenchUtils
PUBLIC
    benchmark::benchmark
    Fl::utils
)