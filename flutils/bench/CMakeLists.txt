set(BENCH_SOURCE
    bench_stringbuilder.cpp
)

add_executable(BenchUtils ${BENCH_SOURCE})

target_link_libraries(BenchUtils
    benchmark::benchmark
    Forward::Utils
)