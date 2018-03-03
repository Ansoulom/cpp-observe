@echo off
cd %~dp0
mkdir build
cd build
conan install .. --build missing
cmake .. -G "Visual Studio 15 2017 Win64"
cmake --build . --config release
pause
