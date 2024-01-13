#include "fl/core/StringBuilder.hpp"
using namespace Forward;

#include <benchmark/benchmark.h>

static const char* str =
    "Lorem ipsum dolor sit amet,consectetur adipiscing elit, sed do eiusmod "
    "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim "
    "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea "
    "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate "
    "velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint "
    "occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
    "mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur "
    "adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore "
    "magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco "
    "laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
    "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
    "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa "
    "qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit "
    "amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut "
    "labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud "
    "exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. "
    "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum "
    "dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
    "proident, sunt in culpa qui officia deserunt mollit anim id est laborum. "
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
    "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim "
    "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea "
    "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate "
    "velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint "
    "occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
    "mollit anim id est laborum.";

static void StringArgCreation(benchmark::State& state)
{
    for (auto _ : state) 
    {
        StringArg arg("arg", str);
    }
}
BENCHMARK(StringArgCreation);

static void StringArgCopy(benchmark::State& state) 
{
    StringArg arg("arg", str);

    for (auto _ : state) 
    {
        StringArg copy(str);
    }
}
BENCHMARK(StringArgCopy);

static void StringArgComparing(benchmark::State& state) 
{
    StringArg arg1("arg", str), arg2("arg", str), arg3;

    for (auto _ : state)
    {
        if (arg1 == arg2) {
        }
        else if (arg2 == arg3) {
        }
        else if (arg3 == "Some text %arg and hidden arg") 
        {
        }
    }
}
BENCHMARK(StringArgComparing);

static void StringBuilderCreation(benchmark::State& state) 
{
    StringArg arg1("arg", "інша мова"), arg2("test", "some new data");
    StringBuilder strb(str);

    for (auto _ : state)
    {
        std::string_view view = strb.Arg(arg1).Arg(arg2);
    }
}
BENCHMARK(StringBuilderCreation);