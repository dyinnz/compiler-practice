/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#include <iostream>
#include "simplelogger.h"
#include "nfa_graph.h"

using namespace std;
using namespace simple_logger;
using namespace nfa_graph;

BaseLogger logger;

bool TEST_SimpleNFA() {
  string s {"aa"};
  NFA *nfa = NFA::CreateSimpleNFAFromString("a");
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Concatenate() {
  string s {"abc"};
  NFA *nfa = Concatenate(NFA::CreateSimpleNFAFromString("a"),
                         NFA::CreateSimpleNFAFromString("b"),
                         NFA::CreateSimpleNFAFromString("c"));
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_LogicalOr() {
  cout << __func__ << endl;
  string s {"ay"};
  NFA *nfa = LogicalOr(NFA::CreateSimpleNFAFromString("x"),
                       NFA::CreateSimpleNFAFromString("y"),
                       NFA::CreateSimpleNFAFromString("z"));
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_KleenStar() {
  cout << __func__ << endl;
  string s {"aaa"};
  NFA *nfa = KleenStar(NFA::CreateSimpleNFAFromString("a"));
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Optional() {
  cout << __func__ << endl;
  string s {"aa"};
  NFA *nfa = Optional(NFA::CreateSimpleNFAFromString("a"));
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_LeastOne() {
  cout << __func__ << endl;
  string s {"aaa"};
  NFA *nfa = LeastOne(NFA::CreateSimpleNFAFromString("a"));
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Combination() {
  cout << __func__ << endl;
  string s {"abaa"};
  NFA *nfa = KleenStar(LogicalOr(NFA::CreateSimpleNFAFromString("a"),
                                 NFA::CreateSimpleNFAFromString("b")));
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

int main() {
  logger.set_log_level(kDebug);
  cout << "hello world" << endl;

  cout << TEST_SimpleNFA() << endl;
  cout << TEST_Concatenate() << endl;
  cout << TEST_LogicalOr() << endl;
  cout << TEST_KleenStar() << endl;
  cout << TEST_Optional() << endl;
  cout << TEST_LeastOne() << endl;
  cout << TEST_Combination() << endl;

  return 0;
}
