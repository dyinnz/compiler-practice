//
// Created by coder on 16-9-16.
//

#pragma once

#include <climits>
#include <ostream>

constexpr int kErrorID = CHAR_MAX + 1;
constexpr int kEofID = CHAR_MAX + 2;
constexpr int kEpsilonID = CHAR_MAX + 3;
constexpr int kSpaceID = CHAR_MAX + 4;
constexpr int kLFID = CHAR_MAX + 5;
constexpr int kStartID = CHAR_MAX + 6;

#define DECLARE_SYMBOL(name, index) \
constexpr int name##ID = kStartID + (index); \
extern const Symbol name;

#define TERMINAL(name) const Symbol name(Symbol::kTerminal, name##ID);
#define NON_TERMINAL(name) const Symbol name(Symbol::kNonTerminal, name##ID);

class Symbol {
 public:
  enum Type { kTerminal, kNonTerminal };

  Symbol(Type type, int id) : type_(type), id_(id) {}
  Symbol() : type_(kTerminal), id_(kErrorID) {}

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

  bool operator!=(const Symbol &rhs) const {
    return !this->operator==(rhs);
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

static const Symbol kErrorSymbol(Symbol::kTerminal, kErrorID);
static const Symbol kEofSymbol(Symbol::kTerminal, kEofID);
static const Symbol kEpsilonSymbol(Symbol::kTerminal, kEpsilonID);
static const Symbol kSpaceSymbol(Symbol::kTerminal, kSpaceID);
static const Symbol kLFSymbol(Symbol::kTerminal, kLFID);
static const Symbol kStartSymbol{Symbol::kNonTerminal, kStartID};

inline std::ostream &operator<<(std::ostream &os, const Symbol &symbol) {
  if (symbol.IsTerminal()) {
    return os << "[T:" << symbol.ID() << ']';
  } else {
    return os << "[NT:" << symbol.ID() << ']';
  }
}
