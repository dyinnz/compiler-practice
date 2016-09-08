project(parsing-techniques)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")

include_directories(src/)

add_executable(main
        src/main.cc
        src/finite_automaton.cc
        src/regex_parser.cc
        src/tokenizer.cc)

add_executable(test_mem_manager
        test/test_mem_manager.cc)

add_executable(test_regex_parser
        test/test_regex_parser.cc
        src/finite_automaton.cc
        src/regex_parser.cc)
