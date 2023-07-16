#include "fl/net/http/HttpUrl.hpp"
using namespace Forward;

#include <benchmark/benchmark.h>

static void BM_UrlParse(benchmark::State& state) 
{
    for (auto _ : state) 
    {
        HttpUrl url("http://www.example.com/path/to/resource?param1=value1#fragment");
    }
}
BENCHMARK(BM_UrlParse);

BENCHMARK_MAIN();