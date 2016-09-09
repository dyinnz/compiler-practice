//
// Created by coder on 16-9-5.
//

#pragma once

#include "finite_automaton.h"
#include "regex_parser.h"

using namespace regular_expression;

class Tokenizer {
public:
  ~Tokenizer() {
  }

  void BuildTokenizer(const std::vector<std::string> &rules);

  const DFA *GetTokenDFA() const {
    return &*token_dfa_;
  }

private:
  void ResetLabel() {
    curr_label_ = 0;
  }

  int NextLabel() {
    return curr_label_++;
  }

private:
  std::shared_ptr<DFA> token_dfa_;
  int curr_label_;
};
