cmake_minimum_required(VERSION 3.24)

set(UTILS_BENCH_SOURCE
    bench_stringbuilder.cpp
)

add_executable(BenchUtils ${UTILS_BENCH_SOURCE})

target_link_libraries(BenchUtils
PUBLIC
    Qt6::Core
    Fl::utils
    benchmark::benchmark
)