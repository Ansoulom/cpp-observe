cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") # GCC specific commands
	add_compile_options(-W -Wall -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") # MSVC specific commands
	add_compile_options(/W3 /WX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17") # MSVC does not support CMAKE_CXX_STANDARD
endif()

set(PROJ_NAME "CppObserve")
project(${PROJ_NAME} VERSION 0.1 LANGUAGES CXX)

set(PROJ_TARGET_NAME ${PROJ_NAME})
set(PROJ_SOURCE_DIR src)
set(PROJ_INCLUDE_DIR include)

add_library(${PROJ_TARGET_NAME} INTERFACE)

target_include_directories(${PROJ_TARGET_NAME} INTERFACE include/)

# Testing
add_executable(CppObserveTest src/main.cpp)

target_link_libraries(CppObserveTest ${PROJ_TARGET_NAME})
