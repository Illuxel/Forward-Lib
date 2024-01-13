#include "fl/web/HttpUrl.hpp"
using namespace Forward::Web;

#include <benchmark/benchmark.h>

static void BM_UrlParse(benchmark::State& state) 
{
    for (auto _ : state) 
    {
        HttpUrl url("http://www.example.com/path/to/resource?param1=value1#fragment");
    }
}
BENCHMARK(BM_UrlParse);