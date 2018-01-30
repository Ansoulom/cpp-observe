project(PackageTest CXX)
cmake_minimum_required(VERSION 3.9)

# Compiler flags
set(CMAKE_CXX_STANDARD 17)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") # MSVC specific commands
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17") # MSVC does not support CMAKE_CXX_STANDARD
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(example example.cpp)
target_link_libraries(example ${CONAN_LIBS})

# CTest is a testing tool that can be used to test your project.
# enable_testing()
# add_test(NAME example
#          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#          COMMAND example)
