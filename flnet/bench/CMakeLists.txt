set(BENCH_SOURCE
    bench_main.cpp
    bench_ssl.cpp
    bench_tcp.cpp)

add_executable(BenchNet ${BENCH_SOURCE})

target_link_libraries(BenchNet
    benchmark::benchmark
    Forward::Net)