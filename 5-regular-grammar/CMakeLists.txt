project(regular-grammar)
cmake_minimum_required(VERSION 3.0)

include_directories(.)
include_directories(${CommonInclude})

add_executable(main
        main.cc
        finite_automaton.cc
        regex_parser.cc
        tokenizer.cc)

add_executable(test_regex_parser
        test_regex_parser.cc
        finite_automaton.cc
        regex_parser.cc)
