//
// Created by coder on 16-9-8.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "tokenizer.h"
#include "simplelogger.h"

using namespace simple_logger;

BaseLogger logger;

TEST_CASE("test build dfa using vector, two rules", "[Test DFA]") {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({"110", "[0123]+"});
  auto dfa = tokenizer.GetTokenDFA();

  REQUIRE(dfa->Match("110"));
  REQUIRE(dfa->Match("01230123"));
  REQUIRE_FALSE(dfa->Match("1104"));
}

TEST_CASE("test build dfa", "[Test DFA]") {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({"110", "[0123]+"});
  auto dfa = tokenizer.GetTokenDFA();

  REQUIRE(dfa->Match("110"));
  REQUIRE(dfa->Match("01230123"));
  REQUIRE_FALSE(dfa->Match("1104"));
}

