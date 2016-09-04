project(regular-grammar)
cmake_minimum_required(VERSION 3.0)

include_directories(.)
include_directories(${CommonInclude})

add_executable(regular-grammar
        main.cc
        finite_automaton.cc
        re_parser.cc)

add_executable(test_re_parser
        test_re_parser.cc
        finite_automaton.cc
        re_parser.cc)
