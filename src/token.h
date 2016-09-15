//
// Created by coder on 16-9-15.
//

#pragma once

#include <string>

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

static const Token kEndToken {
    .str = "kEndToken",
    .type = kEndTokenType
};

static const Token kErrorToken {
    .str = "kErrorToken",
    .type = kErrorTokenType,
};
