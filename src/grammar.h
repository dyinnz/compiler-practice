//
// Created by coder on 16-9-11.
//

#pragma once

#include <climits>
#include <cassert>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>

#include "token.h"
#include "ast.h"

typedef std::vector<Symbol> Sequence;
typedef std::function<void(void *)> SnippetCallback;

class ProductionRule {
 public:
  ProductionRule(const Symbol &left, Sequence &&right, SnippetCallback snippet)
      : left_(left), right_(std::move(right)), snippet_(snippet) {}

  ProductionRule(const ProductionRule &) = delete;
  ProductionRule(ProductionRule &&) = default;

  const Symbol &left() const {
    return left_;
  }

  const Sequence &right() const {
    return right_;
  }

  const SnippetCallback &snippet() const {
    return snippet_;
  }

 private:
  Symbol left_;
  Sequence right_;
  SnippetCallback snippet_;
};

inline std::ostream &operator<<(std::ostream &os, const ProductionRule &rule) {
  os << rule.left() << " ->";
  for (auto &s : rule.right()) {
    os << ' ' << s;
  }
  return os;
}

class Grammar {
 public:
  typedef std::vector<ProductionRule> RuleRecord;
  typedef std::unordered_multimap<Symbol, size_t> RuleMap;
  typedef std::pair<RuleMap::const_iterator, RuleMap::const_iterator> RuleRange;
  typedef std::unordered_set<Symbol> SymbolTable;
  typedef std::function<void(void *, Token &&)> TokenFeeder;

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

  const SymbolTable &symbol_table() const {
    return symbol_table_;
  }

  bool CheckSymbol(Symbol symbol) const {
    return symbol_table_.end() != symbol_table_.find(symbol);
  }

  const TokenFeeder &token_feeder() const {
    return token_feeder_;
  }

 private:
  RuleRecord rule_record_;
  RuleMap rule_map_;
  SymbolTable symbol_table_;
  TokenFeeder token_feeder_;
};

class GrammarBuilder {
 public:
  GrammarBuilder &SetSymbolTable(Grammar::SymbolTable &&table) {
    grammar_.symbol_table_ = table;
    return *this;
  }

  GrammarBuilder &SetTokenFeeder(Grammar::TokenFeeder token_feeder) {
    grammar_.token_feeder_ = token_feeder;
    return *this;
  }

  GrammarBuilder &InsertRule(Symbol left,
                             Sequence &&right,
                             SnippetCallback snippet) {
    assert(left.IsNonTerminal());
    grammar_.rule_record_.emplace_back(left, std::move(right), snippet);
    return *this;
  }

  Grammar &&Build() {
    for (size_t i = 0; i < grammar_.rule_record_.size(); ++i) {
      grammar_.rule_map_.insert(std::make_pair(grammar_.rule_record_[i].left(),
                                               i));
    }
    return std::move(grammar_);
  }

 private:
  Grammar grammar_;
};
