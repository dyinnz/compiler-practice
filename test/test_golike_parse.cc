//
// Created by coder on 16-10-2.
//

#define CATCH_CONFIG_MAIN
#define DEBUG

#include <cstring>
#include <fstream>

#include "catch.hpp"
#include "simplelogger.h"
#include "golike_grammar.h"
#include "ll_parser.h"

using namespace simple_logger;
using namespace golike_grammar;
BaseLogger logger;

using std::fstream;
using std::ifstream;
using std::string;
using std::vector;

/*----------------------------------------------------------------------------*/

static const string kTestPath("test/testgo/src/");

static std::streampos GetFileLength(ifstream &fin) {
  auto backup = fin.tellg();
  fin.seekg(0, fin.end);
  auto length = fin.tellg();
  fin.seekg(backup, fin.beg);
  return length;
}

char *ReadFileData(const string &path, size_t &size) {
  ifstream fin(path);
  if (!fin) {
    return nullptr;
  }

  size = static_cast<size_t>(GetFileLength(fin));
  auto data = new char[size + 1];
  memset(data, 0, sizeof(char) * (size + 1));

  if (fin.read(data, size)) {
    return data;

  } else {
    delete data;
    return nullptr;
  }
}

#define GET_FILE_DATA_SAFELY(name, size, relative_path) \
size_t size = 0; \
const char *name = ReadFileData(kTestPath + (relative_path), size); \
ScopeGuard [&] { delete[] name; };

static vector<Token> GetTokensFromFile(const string &path) {
  GET_FILE_DATA_SAFELY(data, size, path);
  REQUIRE(data);

  static auto tokenizer = BuildGolikeTokenizer();
  std::vector<Token> tokens;
  REQUIRE(tokenizer.LexicalAnalyze(data, data+size, tokens));

  return tokens;
}

static LLParser &GetLLParser() {
  static Grammar grammar = BuildGolikeGrammar();
  static LLTable ll_table;

  static bool is_first_run = true;
  if (is_first_run) {
    is_first_run = false;

    for (size_t i = 0; i < grammar.RuleNumber(); ++i) {
      logger.log("Rule {}: {}", i, grammar.GetRule(i));
    }

    REQUIRE(BuildLLTable(grammar, ll_table));
  }

  static LLParser ll_parser(grammar, ll_table);
  return ll_parser;
}

TEST_CASE("Parsing for basic type") {
  logger.set_log_level(kDebug);
  auto tokens = GetTokensFromFile("testcase/basic_type.go");
  auto ll_parser = GetLLParser();
  auto parse_data = CreateGolikeGrammarData();

  auto result = ll_parser.Parse(parse_data.get(), tokens);
  REQUIRE(result);
}

TEST_CASE("Parsing for comment") {
  logger.set_log_level(kDebug);
  auto tokens = GetTokensFromFile("testcase/comment.go");
  auto ll_parser = GetLLParser();
  auto parse_data = CreateGolikeGrammarData();

  auto result = ll_parser.Parse(parse_data.get(), tokens);
  REQUIRE(result);
}
