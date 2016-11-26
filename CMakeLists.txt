project(parsing-techniques)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")

include_directories(include/)
include_directories(src/)

add_library(regex.o OBJECT
        src/finite_automaton.cc
        src/regex_parser.cc)

add_library(tokenizer.o OBJECT
        src/tokenizer.cc)

add_library(ll_parser.o OBJECT
        src/ll_parser.cc)

add_library(expr_grammar.o OBJECT
        src/expr_grammar.cc)

add_library(golike_grammar.o OBJECT
        src/golike_grammar.cc)

################################################################################

add_executable(test_mem_manager
        test/test_mem_manager.cc)

add_executable(test_regex_parser
        $<TARGET_OBJECTS:regex.o>
        test/test_regex_parser.cc)

add_executable(test_tokenizer
        $<TARGET_OBJECTS:regex.o>
        $<TARGET_OBJECTS:tokenizer.o>
        test/test_tokenizer.cc)

add_executable(test_ll_parser
        $<TARGET_OBJECTS:regex.o>
        $<TARGET_OBJECTS:tokenizer.o>
        $<TARGET_OBJECTS:ll_parser.o>
        $<TARGET_OBJECTS:expr_grammar.o>
        test/test_ll_parser.cc)

add_executable(test_golike_tokenize
        $<TARGET_OBJECTS:regex.o>
        $<TARGET_OBJECTS:tokenizer.o>
        $<TARGET_OBJECTS:ll_parser.o>
        $<TARGET_OBJECTS:golike_grammar.o>
        test/test_golike_tokenize.cc)

add_executable(test_golike_parse
        $<TARGET_OBJECTS:regex.o>
        $<TARGET_OBJECTS:tokenizer.o>
        $<TARGET_OBJECTS:ll_parser.o>
        $<TARGET_OBJECTS:golike_grammar.o>
        test/test_golike_parse.cc)

add_executable(main
        $<TARGET_OBJECTS:regex.o>
        $<TARGET_OBJECTS:tokenizer.o>
        $<TARGET_OBJECTS:ll_parser.o>
        $<TARGET_OBJECTS:expr_grammar.o>
        $<TARGET_OBJECTS:golike_grammar.o>
        src/main.cc)
