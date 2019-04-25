@echo off
cd %~dp0
mkdir build
cd build
conan install .. --build missing
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake -G "Visual Studio 16 2019"
cmake --build . --target install --config release
pause
