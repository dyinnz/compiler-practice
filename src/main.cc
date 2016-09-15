/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#include <iostream>
#include "simplelogger.h"
#include "finite_automaton.h"
#include "regex_parser.h"
#include "tokenizer.h"
#include "expr_grammar.h"
#include "ll_parser.h"

using namespace std;
using namespace simple_logger;
using namespace regular_expression;
using namespace expr_grammar;

BaseLogger logger;

void Example() {
  RegexParser re_parser;
  shared_ptr<DFA> dfa{re_parser.ParseToDFA("\\d+@(\\w+\\.)+(com|cn)")};

  PrintDFA(dfa.get());

  cout << dfa->Match("527621747@test.qq.cn") << endl;
  cout << dfa->Match("ml_143@test.qq.com") << endl;
}

void TEST_Tokenizer() {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer({{"110", 1},
                            {"[0123]+", 2}});
}

void TEST_ExprGrammar() {
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

  auto extend_firsts = CalcExtendFirst(grammar, firsts, follows);
  cout << "========= the Extend First set =========" << endl;
  for (size_t i = 0; i < extend_firsts.size(); ++i) {
    auto prule = grammar.GetRuleRecord()[i];
    cout << "[Rule] " << to_string(prule->first) << "-> ";
    for (auto &right_part : prule->second) {
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

int main() {
  logger.set_log_level(kDebug);

  // Example();
  // TEST_Tokenizer();
  TEST_ExprGrammar();

  return 0;
}

