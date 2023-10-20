set(BENCH_SOURCE bench_main.cpp)

add_executable(BenchNet ${BENCH_SOURCE})

target_link_libraries(BenchNet
    benchmark::benchmark
    Fl::net
)