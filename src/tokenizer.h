//
// Created by coder on 16-9-5.
//

#pragma once

#include "finite_automaton.h"
#include "regex_parser.h"

using namespace regular_expression;

constexpr int kEndTokenType = -1;
constexpr int kErrorTokenType = -2;

struct Token {
  Token() = default;
  Token(std::string str,  int type) : str(str), type(type) {}

  bool operator ==(const Token& rhs) {
    return str == rhs.str && type == rhs.type;
  }

  bool operator !=(const Token& rhs) {
    return operator==(rhs);
  }

  std::string str;
  int type{-1};
};

const Token kEndToken {
    .str = "kEndToken",
    .type = kEndTokenType
};

const Token kErrorToken {
    .str = "kErrorToken",
    .type = kErrorTokenType,
};

class Tokenizer {
 public:
  ~Tokenizer() {
  }

  void BuildTokenizer(const std::vector<std::string> &rules);

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
