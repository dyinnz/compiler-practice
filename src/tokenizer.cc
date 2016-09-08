//
// Created by coder on 16-9-5.
//

#include "tokenizer.h"

void Tokenizer::BuildTokenizer(const std::string &lhs,
                               const std::string &rhs) {
  /*
  NFAManager nfa_manager_;

  NFAComponent *lhs_comp = RegexParser::ParseToNFAComponent(lhs);
  lhs_comp->end()->set_type(0);

  NFAComponent *rhs_comp = RegexParser::ParseToNFAComponent(rhs);
  rhs_comp->end()->set_type(1);

  std::cout << __func__ << " " << lhs_comp << " " << rhs_comp << std::endl;

  auto token_nfa = nfa_manager_.UnionWithMultiEnd(lhs_comp, rhs_comp);
  PrintNFA(token_nfa);
  */

  // DFA* token_dfa = ConvertNFAToDFA(token_nfa);
  // PrintDFA(token_dfa);
}