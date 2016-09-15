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
  LLTable ll_table;
  bool result = BuildLLTable(grammar, ll_table);

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

int main() {
  logger.set_log_level(kDebug);

  // Example();
  // TEST_Tokenizer();
  TEST_ExprGrammar();

  return 0;
}

