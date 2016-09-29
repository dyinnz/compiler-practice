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
#include "ll_parser.h"

#include "expr_grammar.h"
#include "golike_grammar.h"

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

void TEST_ExprGrammar() {
  Grammar grammar = expr_grammar::BuildExprGrammar();
  LLTable ll_table;
  bool result = BuildLLTable(grammar, ll_table);

  cout << "========= test LL(1) SymbolTable =========" << endl;
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

void PrintASTRecur(AstNode *node, size_t deep = 0) {
  using namespace expr_grammar;

  for (size_t i = 0; i < deep; ++i) {
    cout << "  ";
  }

  cout << to_string(node->symbol()) << ":" << node->str() << std::endl;

  for (auto child : node->children()) {
    PrintASTRecur(child, deep + 1);
  }
}

void TEST_LLParser() {
  using namespace expr_grammar;

  Grammar grammar = BuildExprGrammar();
  LLTable ll_table;
  BuildLLTable(grammar, ll_table);

  LLParser ll_parser(grammar, ll_table);

  Tokenizer tokenizer = BuildExprTokenizer();
  string s{"a + 999 * (c - 1) "};
  vector<Token> tokens;
  tokenizer.LexicalAnalyze(s, tokens);

  auto expr_data = expr_grammar::CreateGrammarData();

  bool result = ll_parser.Parse(expr_data.get(), tokens);

  cout << expr_data->node_record().size() << endl;
  for (auto node : expr_data->node_record()) {
    PrintASTRecur(node, 0);
  }
}

void TEST_AnsiC() {
}

int main() {
  logger.set_log_level(kDebug);

  // Example();
  // TEST_Tokenizer();
  // TEST_ExprGrammar();
  TEST_LLParser();

  return 0;
}

