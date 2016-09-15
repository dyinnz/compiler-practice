//
// Created by coder on 16-9-5.
//

#pragma once

#include "finite_automaton.h"
#include "regex_parser.h"

using namespace regular_expression;

#include "token.h"

class Tokenizer {
 public:
  ~Tokenizer() {
  }

  void BuildTokenizer(const std::vector<std::pair<std::string, int>> &pattern);

  const DFA *GetTokenDFA() const {
    return &*token_dfa_;
  }

  const char *CurrentPos() {
    return curr_;
  }

  Token GetNextToken(const char *&p);

  bool LexicalAnalyze(const std::string &s,
                      std::vector<Token> &tokens);

  bool LexicalAnalyze(const char *beg,
                      const char *end,
                      std::vector<Token> &tokens);

 private:
  void ResetLabel() {
    label_num_ = 0;
  }

  int NextLabel() {
    return label_num_++;
  }

  const char *SkipSpace(const char *p);

 private:
  std::shared_ptr<DFA> token_dfa_;
  const char *beg_;
  const char *end_;
  const char *curr_;
  int label_num_;
};