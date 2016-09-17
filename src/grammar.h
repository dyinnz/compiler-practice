//
// Created by coder on 16-9-11.
//

#pragma once

#include <climits>
#include <cassert>

#include <vector>
#include <unordered_map>

#include "symbol.h"

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

  bool IsTerminal(int id) const {
    return is_terminal_(id);
  }

 private:
  RuleMap rule_map_;
  RuleRecord rule_record_;
  std::vector<Symbol> table_;
  bool (*is_terminal_)(int id);
};

class GrammarBuilder {
 public:
  GrammarBuilder &SetSymbolTable(std::vector<Symbol> &&table) {
    grammar_.table_ = table;
    return *this;
  }

  GrammarBuilder &SetIsTerminal(bool (*is_terminal)(int id)) {
    grammar_.is_terminal_ = is_terminal;
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
