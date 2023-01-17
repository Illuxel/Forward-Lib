@echo off

set REPETITIONS=25

echo Staring benchmarking...

cd ../build-cmake/flutils/bench/Release

BenchUtils --benchmark_repetitions=%REPETITIONS%

cd ../../..