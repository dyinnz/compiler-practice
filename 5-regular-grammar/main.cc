/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#include <iostream>
#include "simplelogger.h"
#include "finite_automaton.h"

using namespace std;
using namespace simple_logger;
using namespace finite_automaton;

BaseLogger logger;

bool TEST_SimpleNFA() {
  cout << __func__ << endl;
  string s{"a"};
  NFA *nfa = NFAComponent::CreateFromString("a")->BuildNFA();
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Concatenate() {
  cout << __func__ << endl;
  string s{"abc"};
  NFA *nfa = Concatenate(NFAComponent::CreateFromString("a"),
                         NFAComponent::CreateFromString("b"),
                         NFAComponent::CreateFromString("c"))->BuildNFA();
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_LogicalOr() {
  cout << __func__ << endl;
  string s{"y"};
  NFA *nfa = LogicalOr(NFAComponent::CreateFromString("x"),
                       NFAComponent::CreateFromString("y"),
                       NFAComponent::CreateFromString("z"))->BuildNFA();
  PrintNFA(nfa->start(), nfa->size());
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_KleenStar() {
  cout << __func__ << endl;
  string s{"aaa"};
  NFA *nfa = KleenStar(NFAComponent::CreateFromString("a"))->BuildNFA();
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Optional() {
  cout << __func__ << endl;
  string s{"aa"};
  NFA *nfa = Optional(NFAComponent::CreateFromString("a"))->BuildNFA();
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_LeastOne() {
  cout << __func__ << endl;
  string s{"aaa"};
  NFA *nfa = LeastOne(NFAComponent::CreateFromString("a"))->BuildNFA();
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Combination() {
  cout << __func__ << endl;
  string s{"abaa"};
  NFA *nfa = KleenStar(LogicalOr(NFAComponent::CreateFromString("a"),
                                 NFAComponent::CreateFromString(
                                     "b")))->BuildNFA();
  PrintNFA(nfa->start(), nfa->size());
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Number() {
  cout << __func__ << endl;
  string s{"aaab"};
  NFA *nfa = LogicalOr(KleenStar(NFAComponent::CreateFromString("a")),
                       Concatenate(NFAComponent::CreateFromString("a"),
                                   LeastOne(NFAComponent::CreateFromString(
                                       "b"))))->BuildNFA();
  PrintNFA(nfa->start(), nfa->size());

  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Convert() {
  cout << __func__ << endl;
  NFA *nfa = LogicalOr(KleenStar(NFAComponent::CreateFromString("a")),
                       Concatenate(NFAComponent::CreateFromString("a"),
                                   LeastOne(
                                       NFAComponent::CreateFromString("b"))))
      ->BuildNFA();

  DFA *dfa = DFA::ConvertFromNFA(nfa);
  logger.debug("dfa size: {}", dfa->size());
  PrintDFA(dfa->start(), dfa->size());

  string s{"abbb"};

  if (dfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_MinimizeDFA() {
  cout << __func__ << endl;

  NFA *nfa = LogicalOr(NFAComponent::CreateFromString("a"),
                       Concatenate(
                           LeastOne(
                               Concatenate(NFAComponent::CreateFromString("x"),
                                           NFAComponent::CreateFromString(
                                               "x"))),
                           NFAComponent::CreateFromString("a")))->BuildNFA();
  PrintNFA(nfa->start(), nfa->size());

  DFA *dfa = DFA::ConvertFromNFA(nfa);
  PrintDFA(dfa->start(), dfa->size());

  DFA *minimum_dfa = DFAOptimizer::Minimize(dfa);
  PrintDFA(minimum_dfa->start(), minimum_dfa->size());

  string s{"xxxxxxa"};

  if (minimum_dfa->Match(s.c_str(), s.c_str() + s.length())) {
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
  cout << TEST_Number() << endl;
  cout << TEST_Convert() << endl;
  cout << TEST_MinimizeDFA() << endl;

  {
    cout << "begin of block" << endl;
    int *p = new int {99};
    ScopeGuard [=] {
      cout << "hello world" << endl;
      delete p;
    };
    cout << "end of block" << endl;
  }
  cout << "out of block" << endl;

  return 0;
}
