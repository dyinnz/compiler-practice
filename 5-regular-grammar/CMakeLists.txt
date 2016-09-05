project(regular-grammar)
cmake_minimum_required(VERSION 3.0)

include_directories(.)
include_directories(${CommonInclude})

add_executable(main
        main.cc
        finite_automaton.cc
        re_parser.cc
        tokenizer.cc)

add_executable(test_re_parser
        test_re_parser.cc
        finite_automaton.cc
        re_parser.cc)
