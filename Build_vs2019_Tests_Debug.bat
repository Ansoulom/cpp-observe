@echo off
cd %~dp0
mkdir build
cd build
conan install -s build_type=Debug .. --build missing
cmake -DBUILD_TESTS=ON .. -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake -G "Visual Studio 16 2019"
cmake --build . --config debug
pause
