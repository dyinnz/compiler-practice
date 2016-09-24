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

static const Symbol k110 {Symbol::kTerminal, kStartID + 1};
static const Symbol kNumber {Symbol::kTerminal, kStartID + 2};

TEST_CASE("test build dfa using vector, two rules", "[Test DFA]") {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({{"110", k110},
                            {"[0123]+", kNumber}});
  auto dfa = tokenizer.GetTokenDFA();

  REQUIRE(dfa->Match("110"));
  REQUIRE(dfa->Match("01230123"));
  REQUIRE_FALSE(dfa->Match("1104"));
}

TEST_CASE("test build dfa", "[Test DFA]") {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({{"110", k110},
                            {"[0123]+", kNumber}});
  auto dfa = tokenizer.GetTokenDFA();

  REQUIRE(dfa->Match("110"));
  REQUIRE(dfa->Match("01230123"));
  REQUIRE_FALSE(dfa->Match("1104"));
}

TEST_CASE("test one token", "[Test Token]") {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({{"110", k110},
                            {"[0123]+", kNumber}});

  string s{"110 12321"};

  vector<Token> tokens;
  tokenizer.LexicalAnalyze(s, tokens);

  const char *p = s.c_str();

  Token token = tokenizer.GetNextToken(p);
  REQUIRE(k110 == token.symbol);
  REQUIRE("110" == token.str);

  REQUIRE(' ' == *p);
  p += 1;

  token = tokenizer.GetNextToken(p);
  REQUIRE(kNumber == token.symbol);
  REQUIRE("12321" == token.str);

  REQUIRE(s.c_str() + s.length() == p);
}

static const Symbol kIf {Symbol::kTerminal, kStartID + 3};
static const Symbol kWord {Symbol::kTerminal, kStartID + 4};

TEST_CASE("test lexical analyse", "[Test Analyse]") {
  // logger.set_log_level(kDebug);

  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({{"if", kIf},
                            {"\\d+", kNumber},
                            {"\\w+", kWord}});

  string s{"if there\tare\n1000 dogs"};

  vector<Token> tokens;
  REQUIRE(tokenizer.LexicalAnalyze(s, tokens));

  REQUIRE(5 == tokens.size());

  REQUIRE(tokens[0].str == "if");
  REQUIRE(tokens[1].str == "there");
  REQUIRE(tokens[2].str == "are");
  REQUIRE(tokens[3].str == "1000");
  REQUIRE(tokens[4].str == "dogs");

  REQUIRE(tokens[0].symbol == kIf);
  REQUIRE(tokens[1].symbol == kWord);
  REQUIRE(tokens[2].symbol == kWord);
  REQUIRE(tokens[3].symbol == kNumber);
  REQUIRE(tokens[4].symbol == kWord);

  REQUIRE(tokens[0].row == 0);
  REQUIRE(tokens[1].row == 0);
  REQUIRE(tokens[2].row == 0);
  REQUIRE(tokens[3].row == 1);
  REQUIRE(tokens[4].row == 1);

  REQUIRE(tokens[0].column == 0);
  REQUIRE(tokens[1].column == 3);
  REQUIRE(tokens[2].column == 9);
  REQUIRE(tokens[3].column == 0);
  REQUIRE(tokens[4].column == 5);
}
