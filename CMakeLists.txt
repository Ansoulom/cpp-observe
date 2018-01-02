cmake_minimum_required(VERSION 3.9)

set(PROJ_NAME "Templated_observer")
project(${PROJ_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(PROJ_PATH ${CMAKE_SOURCE_DIR})
set(PROJ_SOURCE_PATH "${PROJ_PATH}/src")

set(PROJ_SOURCE_FILES "${PROJ_SOURCE_PATH}/Templated_observer.h" src/main.cpp)

add_executable(${PROJ_NAME} ${PROJ_SOURCE_FILES})