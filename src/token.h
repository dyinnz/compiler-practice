//
// Created by coder on 16-9-15.
//

#pragma once

#include <string>
#include "symbol.h"

struct Token {
  Token(std::string str, const Symbol &symbol)
      : str(std::move(str)), symbol(symbol) {}

  bool operator==(const Token &rhs) const {
    return str == rhs.str && symbol == rhs.symbol;
  }

  bool operator!=(const Token &rhs) const {
    return !operator==(rhs);
  }

  std::string str;
  Symbol symbol;
  size_t row{0};
  size_t column{0};
};

static const Token kErrorToken{"kErrorToken", kErrorSymbol};
static const Token kEofToken{"kEofToken", kEofSymbol};

inline std::string to_string(const Token &token) {
  return '[' + std::to_string(token.symbol.ID()) + ']' + token.str + "("
      + std::to_string(token.row) + ',' + std::to_string(token.column) + ')';
}
