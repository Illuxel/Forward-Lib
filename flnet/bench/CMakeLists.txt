set(BENCH_SOURCE
    bench_url.cpp
)

add_executable(BenchNet ${BENCH_SOURCE})

target_link_libraries(BenchNet
    benchmark::benchmark
    Forward::Net
)