//
// Created by coder on 16-9-15.
//

#pragma once

#include <string>
#include "symbol.h"

struct Token {
  Token(std::string str, Symbol symbol) : str(str), symbol(symbol) {}
  Token() : symbol(kErrorSymbol) {}

  bool operator ==(const Token& rhs) const {
    return str == rhs.str && symbol == rhs.symbol;
  }

  bool operator !=(const Token& rhs) const {
    return !operator==(rhs);
  }

  std::string str;
  Symbol symbol;
};

static const Token kErrorToken {"kErrorToken", kErrorSymbol};
