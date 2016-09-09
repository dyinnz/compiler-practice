//
// Created by coder on 16-9-5.
//

#include "tokenizer.h"
#include "simplelogger.h"

extern simple_logger::BaseLogger logger;


void Tokenizer::BuildTokenizer(const std::vector<std::string> &rules) {
  RegexParser re_parser;
  NFAComponent *result_comp = nullptr;
  ResetLabel();

  for (auto &s : rules) {
    NFAComponent *comp = re_parser.ParseToNFAComponent(s);
    if (!comp) {
      logger.error("{}(): nullptr NFAComponent pointer", __func__);
      return;
    }

    comp->end()->set_type(NextLabel());

    if (result_comp) {
      result_comp = re_parser.GetNFAManager().UnionWithMultiEnd(result_comp,
                                                                comp);
    } else {
      result_comp = comp;
    }
  }

  auto token_nfa = re_parser.GetNFAManager().BuildNFA(result_comp);
  PrintNFA(token_nfa);

  auto normal_dfa = ConvertNFAToDFA(token_nfa);
  PrintDFA(normal_dfa.get());

  token_dfa_ = normal_dfa;

  logger.log("finish build");
}
