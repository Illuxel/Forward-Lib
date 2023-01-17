@echo off

echo Building project...

cd ..

mkdir build-cmake
cd build-cmake

cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

cd ../scripts