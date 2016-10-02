project(parsing-techniques)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")

include_directories(include/)
include_directories(src/)

file(GLOB Sources ./src/*.cc)

add_executable(main ${Sources})

add_executable(test_mem_manager
        test/test_mem_manager.cc)

add_executable(test_regex_parser
        test/test_regex_parser.cc
        src/finite_automaton.cc
        src/regex_parser.cc)

add_executable(test_tokenizer
        test/test_tokenizer.cc
        src/finite_automaton.cc
        src/regex_parser.cc
        src/tokenizer.cc)

add_executable(test_ll_parser
        test/test_ll_parser.cc
        src/ll_parser.cc
        src/expr_grammar.cc
        src/finite_automaton.cc
        src/regex_parser.cc
        src/tokenizer.cc
        )

add_executable(test_golike_tokenize
        test/test_golike_tokenize.cc
        src/finite_automaton.cc
        src/regex_parser.cc
        src/tokenizer.cc
        src/golike_grammar.cc)

add_executable(test_golike_pase
        test/test_golike_parse.cc
        src/ll_parser.cc
        src/expr_grammar.cc
        src/finite_automaton.cc
        src/regex_parser.cc
        src/tokenizer.cc
        src/golike_grammar.cc
        )
