//
// Created by coder on 16-9-5.
//

#include "tokenizer.h"

void Tokenizer::BuildTokenizer(const std::string &lhs,
                               const std::string &rhs) {
  NFAComponent *lhs_comp = REParser::ParseToNFAComponent(lhs);
  lhs_comp->end()->set_type(0);

  NFAComponent *rhs_comp = REParser::ParseToNFAComponent(rhs);
  rhs_comp->end()->set_type(1);

  // PrintNFA(rhs_comp->BuildNFA());

  NFA* token_nfa = TokenUnion(lhs_comp, rhs_comp);
  PrintNFA(token_nfa);

  DFA* token_dfa = DFA::ConvertFromNFA(token_nfa);
  PrintDFA(token_dfa);
}