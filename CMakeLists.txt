project(parsing-techniques)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")

set(CommonInclude ${PROJECT_SOURCE_DIR}/common)

add_subdirectory(./5-regular-grammar)
