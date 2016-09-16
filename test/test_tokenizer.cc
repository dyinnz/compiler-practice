//
// Created by coder on 16-9-8.
//

#define CATCH_CONFIG_MAIN
#define DEBUG

#include "catch.hpp"

#include "tokenizer.h"
#include "simplelogger.h"

using namespace simple_logger;

using std::string;
using std::vector;

BaseLogger logger;

TEST_CASE("test build dfa using vector, two rules", "[Test DFA]") {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({{"110", 1},
                            {"[0123]+", 2}});
  auto dfa = tokenizer.GetTokenDFA();

  REQUIRE(dfa->Match("110"));
  REQUIRE(dfa->Match("01230123"));
  REQUIRE_FALSE(dfa->Match("1104"));
}

TEST_CASE("test build dfa", "[Test DFA]") {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({{"110", 1},
                            {"[0123]+", 2}});
  auto dfa = tokenizer.GetTokenDFA();

  REQUIRE(dfa->Match("110"));
  REQUIRE(dfa->Match("01230123"));
  REQUIRE_FALSE(dfa->Match("1104"));
}

TEST_CASE("test one token", "[Test Token]") {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({{"110", 1},
                            {"[0123]+", 2}});

  string s{"110 12321"};

  vector<Token> tokens;
  tokenizer.LexicalAnalyze(s, tokens);

  const char *p = s.c_str();

  Token token = tokenizer.GetNextToken(p);
  REQUIRE(1 == token.type);
  REQUIRE("110" == token.str);

  REQUIRE(' ' == *p);
  p += 1;

  token = tokenizer.GetNextToken(p);
  REQUIRE(2 == token.type);
  REQUIRE("12321" == token.str);

  REQUIRE(s.c_str() + s.length() == p);
}

TEST_CASE("test lexical analyse", "[Test Analyse]") {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({{"if", 0},
                            {"\\d+", 1},
                            {"\\w+", 2}});

  string s{"if there\tare\n1000 dogs"};

  vector<Token> tokens;
  REQUIRE(tokenizer.LexicalAnalyze(s, tokens));

  REQUIRE(tokens[0].str == "if");
  REQUIRE(tokens[1].str == "there");
  REQUIRE(tokens[2].str == "are");
  REQUIRE(tokens[3].str == "1000");
  REQUIRE(tokens[4].str == "dogs");
  REQUIRE(tokens[5].str == "kEofToken");

  REQUIRE(tokens[0].type == 0);
  REQUIRE(tokens[1].type == 2);
  REQUIRE(tokens[2].type == 2);
  REQUIRE(tokens[3].type == 1);
  REQUIRE(tokens[4].type == 2);
  REQUIRE(tokens[5].type == kEofTokenType);
}
