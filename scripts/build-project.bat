@echo off

echo Building project...

cd ..

mkdir build-cmake
cd build-cmake

cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug

cd ../scripts