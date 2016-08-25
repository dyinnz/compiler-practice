project(regular-grammar)
cmake_minimum_required(VERSION 3.0)

include_directories(.)
include_directories(${CommonInclude})

file(GLOB Sources ./*.cc)

add_executable(regular-grammar ${Sources})
