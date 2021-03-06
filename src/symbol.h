//
// Created by Dyinnz on 16-9-16.
//

#pragma once

#include <climits>
#include <ostream>

/**
 * Predefined Symbol ID
 */
constexpr int kStartID = 10000;
constexpr int kErrorID = kStartID - 1;
constexpr int kEofID = kStartID - 2;
constexpr int kEpsilonID = kStartID - 3;
constexpr int kSpaceID = kStartID - 4;
constexpr int kLFID = kStartID - 5;

/**
 * Some marcos that declare & define symbol. Define symbol ID and string
 * representation of symbol.
 */
#define DECLARE_SYMBOL(name, index) \
constexpr int name##ID = kStartID + (index); \
extern const Symbol name;

#define DECLARE_CHAR_SYMBOL(name, index) \
constexpr int name##ID = (index); \
extern const Symbol name;

#define TERMINAL(name) \
const char *_##name() { return __func__; }\
const Symbol name(Symbol::kTerminal, name##ID, _##name() + 2);

#define NON_TERMINAL(name) \
const char *_##name() { return __func__; }\
const Symbol name(Symbol::kNonTerminal, name##ID, _##name() + 2);

/**
 * @brief   Generalized grammar symbol.
 */
class Symbol {
 public:
  enum Type { kTerminal, kNonTerminal };

  /**
   * @brief Used by the macros
   * @param type    Terminal or non-terminal
   * @param id      A unique number
   * @param str     A string representation
   */
  Symbol(Type type, int id, const char *str)
      : type_(type), id_(id), str_(str) {}

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

  const char *str() const {
    return str_;
  }

  /**
   * Override comparation operator, so that the std::set could hold symbol
   */
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
  const char *str_{"nullptr"};
};

namespace std {

/**
 * Implement the std::hash template,
 * so that std::unordered_set could hold symbol
 */
template<>
struct hash<Symbol> {
  std::size_t operator()(const Symbol &symbol) const {
    return static_cast<size_t>(symbol.ID());
  }
};

} // end of namespace std


/**
 * Predefined symbol
 */
static const Symbol kStartSymbol(Symbol::kNonTerminal, kStartID, "Start");
static const Symbol kErrorSymbol(Symbol::kTerminal, kErrorID, "Error");
static const Symbol kEofSymbol(Symbol::kTerminal, kEofID, "EOF");
static const Symbol kEpsilonSymbol(Symbol::kTerminal, kEpsilonID, "Epsilon");
static const Symbol kSpaceSymbol(Symbol::kTerminal, kSpaceID, "Space");
static const Symbol kLFSymbol(Symbol::kTerminal, kLFID, "LF");

/**
 * @brief Override the left-shift operator for easier printing
 */
inline std::ostream &operator<<(std::ostream &os, const Symbol &symbol) {
  return os << symbol.str();
}
