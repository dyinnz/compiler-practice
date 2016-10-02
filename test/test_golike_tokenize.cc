//
// Created by coder on 16-9-30.
//

#define CATCH_CONFIG_MAIN
#define DEBUG

#include <cstring>
#include <fstream>

#include "catch.hpp"
#include "simplelogger.h"
#include "golike_grammar.h"

using namespace simple_logger;
using namespace golike_grammar;
BaseLogger logger;

using std::fstream;
using std::ifstream;
using std::string;

/*----------------------------------------------------------------------------*/

static const string kTestPath("test/testgo/src/");

static std::streampos GetFileLength(ifstream &fin) {
  auto backup = fin.tellg();
  fin.seekg(0, fin.end);
  auto length = fin.tellg();
  fin.seekg(backup, fin.beg);
  return length;
}

static char *ReadFileData(const string &path, size_t &size) {
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

static void TestTokenizeFile(const string &path) {
  GET_FILE_DATA_SAFELY(data, size, path)

  REQUIRE(data);

  static auto tokenizer = BuildGolikeTokenizer();
  std::vector<Token> tokens;
  tokenizer.LexicalAnalyze(data, data + size, tokens);

  REQUIRE(0 != tokens.size());
  for (auto &token : tokens) {
    logger.log("{}", to_string(token));
  }
}

TEST_CASE("Read test sources file") {
  GET_FILE_DATA_SAFELY(data, size, "main/hellogo.go")

  REQUIRE(data);
  REQUIRE(data == strstr(data, "package"));
}

TEST_CASE("Tokenize for basic type") {
  GET_FILE_DATA_SAFELY(data, size, "testcase/basic_type.go")

  REQUIRE(data);

  auto tokenizer = BuildGolikeTokenizer();
  std::vector<Token> tokens;
  auto result = tokenizer.LexicalAnalyze(data, data + size, tokens);

  REQUIRE(result);
  REQUIRE(0 != tokens.size());
  for (auto &token : tokens) {
    logger.debug("{}", to_string(token));
  }
}

TEST_CASE("Tokenize for for") {
  TestTokenizeFile("testcase/for.go");
}

TEST_CASE("Tokenize for func") {
  TestTokenizeFile("testcase/func.go");
}

TEST_CASE("Tokenize for if") {
  TestTokenizeFile("testcase/if.go");
}

TEST_CASE("Tokenize for import") {
  TestTokenizeFile("testcase/import.go");
}

TEST_CASE("Tokenize for switch") {
  TestTokenizeFile("testcase/switch.go");
}

TEST_CASE("Tokenize for var") {
  TestTokenizeFile("testcase/var.go");
}

TEST_CASE("Tokenizer for main") {
  TestTokenizeFile("main/hellogo.go");
}

TEST_CASE("Tokenizer for add") {
  TestTokenizeFile("simpleadd/add.go");
}

TEST_CASE("Tokenizer for sub") {
  TestTokenizeFile("simplesub/sub.go");
}

TEST_CASE("Tokenizer for comment") {
  TestTokenizeFile("testcase/comment.go");
}
