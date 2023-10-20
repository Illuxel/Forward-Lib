set(BENCH_SOURCE
    bench_url.cpp
)

add_executable(BenchWeb ${BENCH_SOURCE})

target_link_libraries(BenchWeb
    benchmark::benchmark
    Forward::Web
)