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
using namespace finite_automaton;

BaseLogger logger;

bool TEST_SimpleNFA() {
  cout << __func__ << endl;
  string s{"a"};
  NFA *nfa = NFA::CreateFromString("a");
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Concatenate() {
  cout << __func__ << endl;
  string s{"abc"};
  NFA *nfa = Concatenate(NFA::CreateFromString("a"),
                         NFA::CreateFromString("b"),
                         NFA::CreateFromString("c"));
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_LogicalOr() {
  cout << __func__ << endl;
  string s{"y"};
  NFA *nfa = LogicalOr(NFA::CreateFromString("x"),
                       NFA::CreateFromString("y"),
                       NFA::CreateFromString("z"));
  vector<Node *> nodes = RecordNFA(nfa);
  PrintFA(nfa->start(), nodes.size());
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_KleenStar() {
  cout << __func__ << endl;
  string s{"aaa"};
  NFA *nfa = KleenStar(NFA::CreateFromString("a"));
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Optional() {
  cout << __func__ << endl;
  string s{"aa"};
  NFA *nfa = Optional(NFA::CreateFromString("a"));
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_LeastOne() {
  cout << __func__ << endl;
  string s{"aaa"};
  NFA *nfa = LeastOne(NFA::CreateFromString("a"));
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Combination() {
  cout << __func__ << endl;
  string s{"abaa"};
  NFA *nfa = KleenStar(LogicalOr(NFA::CreateFromString("a"),
                                 NFA::CreateFromString("b")));
  vector<Node *> nodes = RecordNFA(nfa);
  PrintFA(nfa->start(), nodes.size());
  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Number() {
  cout << __func__ << endl;
  string s{"aaab"};
  NFA *nfa = LogicalOr(KleenStar(NFA::CreateFromString("a")),
                       Concatenate(NFA::CreateFromString("a"),
                                   LeastOne(NFA::CreateFromString("b"))));
  vector<Node *> nodes = RecordNFA(nfa);
  PrintFA(nfa->start(), nodes.size());

  if (nfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_Convert() {
  cout << __func__ << endl;

  {
    NFA *nfa = LogicalOr(KleenStar(NFA::CreateFromString("a")),
                         Concatenate(NFA::CreateFromString("a"),
                                     LeastOne(NFA::CreateFromString("b"))));
    vector<Node *> nfa_nodes = RecordNFA(nfa);
    PrintFA(nfa->start(), nfa_nodes.size());
  }

  NFA *nfa = LogicalOr(KleenStar(NFA::CreateFromString("a")),
                       Concatenate(NFA::CreateFromString("a"),
                                   LeastOne(NFA::CreateFromString("b"))));
  DFA *dfa = DFA::ConvertFromNFA(nfa);
  logger.debug("dfa size: {}", dfa->size());
  PrintFA(dfa->start(), dfa->size());

  string s{"abbb"};

  if (dfa->Match(s.c_str(), s.c_str() + s.length())) {
    return true;
  } else {
    return false;
  }
}

bool TEST_MinimizeDFA() {
  cout << __func__ << endl;

  NFA *nfa = LogicalOr(NFA::CreateFromString("a"),
                       Concatenate(
                           LeastOne(Concatenate(NFA::CreateFromString("x"),
                                                NFA::CreateFromString("x"))),
                           NFA::CreateFromString("a")));
  vector<Node *> nfa_nodes = RecordNFA(nfa);
  PrintFA(nfa->start(), nfa_nodes.size());
  ClearNFARecordRecur(nfa_nodes);


  DFA *dfa = DFA::ConvertFromNFA(nfa);
  PrintFA(dfa->start(), dfa->size());

  DFA *minimum_dfa = DFAOptimizer::Minimize(dfa);
  PrintFA(minimum_dfa->start(), minimum_dfa->size());

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

  return 0;
}
