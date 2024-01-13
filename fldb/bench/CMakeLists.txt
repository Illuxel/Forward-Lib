set(BENCH_SOURCE
    bench_main.cpp
    bench_mysql.cpp
    bench_sqlite.cpp)

add_executable(BenchDatabase ${BENCH_SOURCE})

target_link_libraries(BenchDatabase
PUBLIC 
    benchmark::benchmark
    Forward::Database)
