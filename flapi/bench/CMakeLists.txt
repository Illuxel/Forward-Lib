set(BENCHAPI_SOURCE
    bench_main.cpp)

add_executable(BenchAPI ${BENCHAPI_SOURCE})

target_link_libraries(BenchAPI
    benchmark::benchmark
    Forward::API)

set(BENCHCLIENT_SOURCE
    bench_main.cpp)

add_executable(BenchClient ${BENCHCLIENT_SOURCE})

target_link_libraries(BenchClient
    benchmark::benchmark
    Forward::Client)