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
    delete dfa_;
  }

  void BuildTokenizer(const std::string &lhs, const std::string &rhs);

  const DFA *GetDFA() {
    return dfa_;
  }

private:
  DFA *dfa_ {nullptr};
};
