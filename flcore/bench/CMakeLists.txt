set(BENCH_SOURCE
    bench_main.cpp
    bench_stringbuilder.cpp)

add_executable(BenchCore ${BENCH_SOURCE})

target_link_libraries(BenchCore
    benchmark::benchmark
    Forward::Core)