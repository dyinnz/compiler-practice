//
// Created by coder on 16-9-15.
//

#define CATCH_CONFIG_MAIN
#define DEBUG

#include "catch.hpp"
#include "simplelogger.h"
#include "ll_parser.h"
#include "expr_grammar.h"

using namespace simple_logger;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using expr_grammar::to_string;

BaseLogger logger;

TEST_CASE("test first set", "[First]") {
  Grammar grammar = expr_grammar::BuildExprGrammar();
  auto firsts = CalcFirst(grammar);

  cout << "========= the First set =========" << endl;
  for (auto &p : firsts) {
    auto &symbol = p.first;
    auto &first = p.second;
    cout << "[Symbol] " << to_string(symbol) << endl;
    for (auto &terminal : first) {
      cout << to_string(terminal) << " ";
    }
    cout << endl;
  }
}

TEST_CASE("test follow set", "[Follow]") {
  Grammar grammar = expr_grammar::BuildExprGrammar();
  auto firsts = CalcFirst(grammar);
  auto follows = CalcFollow(grammar, firsts);

  cout << "========= the Follow set =========" << endl;
  for (auto &p : follows) {
    auto &symbol = p.first;
    auto &follow = p.second;
    cout << "[Symbol] " << to_string(symbol) << endl;
    for (auto &terminal : follow) {
      cout << to_string(terminal) << " ";
    }
    cout << endl;
  }
}

TEST_CASE("test extend first set", "[First+]") {
  Grammar grammar = expr_grammar::BuildExprGrammar();
  auto firsts = CalcFirst(grammar);
  auto follows = CalcFollow(grammar, firsts);
  auto extend_firsts = CalcExtendFirst(grammar, firsts, follows);

  cout << "========= the Extend First set =========" << endl;
  for (size_t i = 0; i < extend_firsts.size(); ++i) {
    auto &rule = grammar.GetRule(i);
    cout << "[Rule " << i << "] " << to_string(rule.left()) << "-> ";
    for (auto &right_part : rule.right()) {
      cout << to_string(right_part) << " ";
    }
    cout << endl;

    cout << "[First] ";
    for (auto &terminal : extend_firsts[i]) {
      cout << to_string(terminal) << " ";
    }
    cout << endl;
  }
}

TEST_CASE("test ll table", "[LL Table]") {
  Grammar grammar = expr_grammar::BuildExprGrammar();
  auto firsts = CalcFirst(grammar);
  auto follows = CalcFollow(grammar, firsts);
  auto extend_firsts = CalcExtendFirst(grammar, firsts, follows);

  LLTable ll_table;
  bool result = BuildLLTable(grammar, extend_firsts, ll_table);

  REQUIRE(result);

  cout << "========= test LL(1) Table =========" << endl;
  for (auto &nonterminal_pair : ll_table) {
    auto &nonterminal = nonterminal_pair.first;
    auto &jump_map = nonterminal_pair.second;

    cout << "[Nonterminal] " << to_string(nonterminal) << endl;
    for (auto &p : jump_map) {
      auto &terminal = p.first;
      auto rule_index = p.second;

      cout << "{" << to_string(terminal) << "->" << rule_index << "}" << " ";
    }
    cout << endl;
  }
}


TEST_CASE("test expr tokenizer", "[Expr Tokenizer]") {
  using namespace expr_grammar;

  auto tokenizer = expr_grammar::BuildExprTokenizer();
  string s{"a + 999 * (c - 1) "};
  vector<Token> tokens;
  bool result = tokenizer.LexicalAnalyze(s, tokens);
  REQUIRE(result);

  REQUIRE(tokens[0].str == "a");
  REQUIRE(tokens[1].str == "+");
  REQUIRE(tokens[2].str == "999");
  REQUIRE(tokens[3].str == "*");
  REQUIRE(tokens[4].str == "(");
  REQUIRE(tokens[5].str == "c");
  REQUIRE(tokens[6].str == "-");
  REQUIRE(tokens[7].str == "1");
  REQUIRE(tokens[8].str == ")");

  REQUIRE(tokens[0].symbol == kName);
  REQUIRE(tokens[1].symbol == kAdd);
  REQUIRE(tokens[2].symbol == kNumber);
  REQUIRE(tokens[3].symbol == kMul);
  REQUIRE(tokens[4].symbol == kLeftParen);
  REQUIRE(tokens[5].symbol == kName);
  REQUIRE(tokens[6].symbol == kSub);
  REQUIRE(tokens[7].symbol == kNumber);
  REQUIRE(tokens[8].symbol == kRightParen);
}

TEST_CASE("test ll parser", "[LL Parser]") {
  using namespace expr_grammar;
  logger.set_log_level(kDebug);

  Grammar grammar = BuildExprGrammar();
  LLTable ll_table;
  BuildLLTable(grammar, ll_table);

  LLParser ll_parser(grammar, ll_table);

  Tokenizer tokenizer = BuildExprTokenizer();
  string s{"a + 999 * (c - 1) "};
  vector<Token> tokens;
  tokenizer.LexicalAnalyze(s, tokens);

  bool result = ll_parser.Parse(nullptr, tokens);
  REQUIRE(result);
}
