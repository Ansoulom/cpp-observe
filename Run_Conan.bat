@echo off
cd %~dp0
mkdir build
cd build
conan install .. --build missing
pause
