//
// Created by coder on 16-9-11.
//

#pragma once

#include <climits>
#include <cassert>

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "symbol.h"

typedef std::vector<Symbol> Sequence;

class ProductionRule {
 public:
  ProductionRule(const Symbol &left, Sequence &&right)
      : left_(left), right_(right) {}

  ProductionRule(const ProductionRule &) = delete;
  ProductionRule(ProductionRule &&) = default;

  const Symbol &left() const {
    return left_;
  }

  const Sequence &right() const {
    return right_;
  }

 private:
  Symbol left_;
  Sequence right_;
};

class Grammar {
 public:
  typedef std::vector<ProductionRule> RuleRecord;
  typedef std::unordered_multimap<Symbol, const Sequence *const> RuleMap;
  typedef std::pair<RuleMap::const_iterator, RuleMap::const_iterator> RuleRange;
  typedef std::unordered_set<Symbol> Table;

  friend class GrammarBuilder;

 public:
  size_t RuleNumber() const {
    return rule_record_.size();
  }

  const RuleRecord &AllRules() const {
    return rule_record_;
  }

  const ProductionRule &GetRule(size_t index) const {
    assert(index < rule_record_.size());
    return rule_record_[index];
  }

  RuleRange GetRule(Symbol symbol) const {
    return rule_map_.equal_range(symbol);
  }

  const Table &AllSymbols() const {
    return table_;
  }

  bool CheckSymbol(Symbol symbol) const {
    return table_.end() != table_.find(symbol);
  }

 private:
  RuleRecord rule_record_;
  RuleMap rule_map_;
  Table table_;
};

class GrammarBuilder {
 public:
  GrammarBuilder &SetSymbolTable(Grammar::Table &&table) {
    grammar_.table_ = table;
    return *this;
  }

  GrammarBuilder &InsertRule(Symbol left, Sequence &&right) {
    assert(left.IsNonTerminal());
    grammar_.rule_record_.emplace_back(left, std::move(right));
    return *this;
  }

  GrammarBuilder &InsertRule(ProductionRule &&rule) {
    assert(rule.left().IsNonTerminal());
    grammar_.rule_record_.push_back(std::move(rule));
    return *this;
  }

  Grammar &&Build() {
    for (auto &rule : grammar_.rule_record_) {
      grammar_.rule_map_.insert({rule.left(), &rule.right()});
    }
    return std::move(grammar_);
  }

 private:
  Grammar grammar_;
};
