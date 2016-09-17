//
// Created by coder on 16-9-16.
//

#pragma once

#include <climits>
#include <ostream>

class Symbol {
 public:
  enum Type { kTerminal, kNonTerminal };

  Symbol(Type type, int id) : type_(type), id_(id) {}

  bool IsTerminal() const {
    return type_ == kTerminal;
  }

  bool IsNonTerminal() const {
    return type_ == kNonTerminal;
  }

  Type type() const {
    return type_;
  }

  int ID() const {
    return id_;
  }

  bool operator<(const Symbol &rhs) const {
    return id_ < rhs.id_;
  }

  bool operator==(const Symbol &rhs) const {
    return id_ == rhs.id_ && type_ == rhs.type_;
  }

 private:
  Type type_;
  int id_;
};

namespace std {

template<>
struct hash<Symbol> {
  std::size_t operator()(const Symbol &symbol) const {
    return static_cast<size_t>(symbol.ID());
  }
};

} // end of namespace std

constexpr int kErrorID = CHAR_MAX + 1;
constexpr int kEofID = CHAR_MAX + 2;
constexpr int kEpsilonID = CHAR_MAX + 3;
constexpr int kStartID = CHAR_MAX + 4;

static const Symbol kErrorSymbol{Symbol::kTerminal, kErrorID};
static const Symbol kEofSymbol{Symbol::kTerminal, kEofID};
static const Symbol kEpsilonSymbol{Symbol::kTerminal, kEpsilonID};
static const Symbol kStartSymbol{Symbol::kNonTerminal, kStartID};

inline std::ostream& operator<<(std::ostream &os, const Symbol &symbol) {
  os << symbol.ID() << std::endl;
  return os;
}
