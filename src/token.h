//
// Created by coder on 16-9-15.
//

#pragma once

#include <string>

constexpr int kErrorTokenType = -1;

struct Token {
  Token() = default;
  Token(std::string str,  int type) : str(str), type(type) {}

  bool operator ==(const Token& rhs) const {
    return str == rhs.str && type == rhs.type;
  }

  bool operator !=(const Token& rhs) const {
    return operator==(rhs);
  }

  std::string str;
  int type;
};

static const Token kErrorToken {
    .str = "kErrorToken",
    .type = kErrorTokenType,
};
