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

using namespace std;
using namespace simple_logger;
using namespace regular_expression;

BaseLogger logger;

void Example() {
  shared_ptr<DFA> dfa {RegexParser::ParseToDFA("\\d+@(\\w+\\.)+(com|cn)")};

  PrintDFA(dfa.get());

  cout << dfa->Match("527621747@test.qq.cn") << endl;
  cout << dfa->Match("ml_143@test.qq.com") << endl;
}


void TEST_Tokenizer() {
  Tokenizer tokenizer;
  tokenizer.BuildTokenizer("110", "[0123]+");
}


int main() {
  logger.set_log_level(kDebug);

  Example();
  TEST_Tokenizer();

  // Node node(Node::kStart);
  NFANode nfaNode(Node::kStart);

  return 0;
}

