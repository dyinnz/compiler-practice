/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#include <iostream>
#include "simplelogger.h"
#include "finite_automaton.h"
#include "re_parser.h"

using namespace std;
using namespace simple_logger;
using namespace regular_expression;

BaseLogger logger;

void Example() {
  shared_ptr<DFA> dfa {REParser::Parse("\\d+@(\\w+\\.)+\\w+")};

  PrintDFA(dfa.get());

  cout << dfa->Match("527621747@test.qq.com") << endl;
  cout << dfa->Match("ml_143@test.qq.com") << endl;
}

int main() {
  logger.set_log_level(kDebug);

  Example();

  return 0;
}

