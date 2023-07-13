#include "fl/net/http/HttpUrl.hpp"
using namespace Forward;

#include <benchmark/benchmark.h>

static void BM_UrlParse(benchmark::State& state) 
{
    std::string urlStr = "http://www.example.com/path/to/resource?param1=value1#fragment";

    for (auto _ : state) 
    {
        HttpUrl url(urlStr);
    }
}
BENCHMARK(BM_UrlParse);

BENCHMARK_MAIN();