@echo off

set PROJECT_PATH=C:/MyProject

echo Path to your project: %PROJECT_PATH%

cd ..

mkdir build-cmake
cd build-cmake

cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:PATH="%PROJECT_PATH%/fl" ..
cmake --build . --target install