set(BENCH_SOURCE
)

add_executable(BenchWeb ${BENCH_SOURCE})

target_link_libraries(BenchWeb
    benchmark::benchmark
    Forward::Web
)