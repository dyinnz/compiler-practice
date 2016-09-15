//
// Created by coder on 16-9-11.
//

#pragma once

#include <climits>
#include <cassert>

#include <vector>
#include <unordered_map>

constexpr int kEpsilonID = -1;
constexpr int kEofID = -2;
constexpr int kStartID = CHAR_MAX + 1;

class Symbol {
 public:
  Symbol(bool is_terminal, int id)
      : is_terminal_(is_terminal), id_(id) {}

  bool IsTerminal() const {
    return is_terminal_;
  }

  int SymbolID() const {
    return id_;
  }

  bool operator<(const Symbol &rhs) const {
    return id_ < rhs.id_;
  }

  bool operator==(const Symbol &rhs) const {
    return id_ == rhs.id_;
  }

 private:
  bool is_terminal_;
  int id_;
};

static const Symbol kEpsilon{true, kEpsilonID};
static const Symbol kEof{true, kEofID};
static const Symbol kStart{false, kStartID};

namespace std {

template<>
struct hash<Symbol> {
  std::size_t operator()(const Symbol &symbol) const {
    return symbol.SymbolID();
  }
};

} // end of namespace std

typedef std::vector<Symbol> Sequence;

class Grammar {
 public:
  typedef std::unordered_multimap<Symbol, Sequence> RuleMap;
  typedef std::vector<std::pair<const Symbol, Sequence> *> RuleRecord;
  friend class GrammarBuilder;

 public:
  const std::vector<Symbol> &SymbolTable() const {
    return table_;
  }

  const RuleMap &GetRuleMap() const {
    return rule_map_;
  }

  const RuleRecord &GetRuleRecord() const {
    return rule_record_;
  }

 private:
  RuleMap rule_map_;
  RuleRecord rule_record_;
  std::vector<Symbol> table_;
};

class GrammarBuilder {
 public:
  GrammarBuilder &SetSymbolTable(std::vector<Symbol> &&table) {
    grammar_.table_ = table;
    return *this;
  }

  GrammarBuilder &InsertRule(Symbol left, Sequence &&right) {
    assert(!left.IsTerminal());
    grammar_.rule_map_.insert({left, right});
    return *this;
  }

  Grammar &&Build() {
    for (auto &p : grammar_.rule_map_) {
      grammar_.rule_record_.push_back(&p);
    }
    return std::move(grammar_);
  }

 private:
  Grammar grammar_;
};
