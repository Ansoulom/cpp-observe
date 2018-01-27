cmake_minimum_required(VERSION 3.9)

set(PROJ_NAME "cpp_observe")
project(${PROJ_NAME} VERSION 0.1 LANGUAGES CXX)

# Compiler flags
set(CMAKE_CXX_STANDARD 17)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") # GCC specific commands
	add_compile_options(-W -Wall -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") # MSVC specific commands
	add_compile_options(/W3 /WX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17") # MSVC does not support CMAKE_CXX_STANDARD
endif()

# Build path variables
set(PROJ_PATH ${CMAKE_SOURCE_DIR})
set(PROJ_BUILD_PATH ${PROJ_PATH}/build)
set(CMAKE_BINARY_DIR ${PROJ_BUILD_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJ_BUILD_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJ_BUILD_PATH})

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Library variables
set(LIB_TARGET_NAME ${PROJ_NAME})
set(LIB_INCLUDE_DIR include)

# Create library
add_library(${LIB_TARGET_NAME} INTERFACE)
target_include_directories(${LIB_TARGET_NAME} INTERFACE ${LIB_INCLUDE_DIR})

# Testing variables
set(TEST_TARGET_NAME ${PROJ_NAME}_test)
set(TEST_SOURCE_DIR src)
set(TEST_SOURCE_FILES
${TEST_SOURCE_DIR}/test_main.cpp
${TEST_SOURCE_DIR}/observer_tests.cpp
)

# Create testing binary
add_executable(${TEST_TARGET_NAME} ${TEST_SOURCE_FILES})
target_link_libraries(${TEST_TARGET_NAME} ${LIB_TARGET_NAME})
target_link_libraries(${TEST_TARGET_NAME} ${CONAN_LIBS})
