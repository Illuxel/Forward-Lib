@echo off

set PROJECT_PATH=E:/My Projects/Work/Forward (Messenger)/Sandbox

echo Path to your project: %PROJECT_PATH%

cd ..

mkdir build-cmake
cd build-cmake

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH="%PROJECT_PATH%/fl" ..
cmake --build . --target install