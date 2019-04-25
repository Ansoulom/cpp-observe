project(PackageTest CXX)
cmake_minimum_required(VERSION 3.9)

add_executable(example example.cpp)
target_compile_features(example PRIVATE cxx_std_17)

find_package(cpp_observe REQUIRED)
target_link_libraries(example PRIVATE cpp_observe::cpp_observe)
