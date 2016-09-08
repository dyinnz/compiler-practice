project(test_mem_manager)
cmake_minimum_required(VERSION 3.0)

include_directories(.)
include_directories(${CommonInclude})

add_executable(test_mem_manager
        test_mem_manager.cc test_mem_manager.cc)
