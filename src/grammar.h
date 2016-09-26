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

class ParserAccept {
};

class ProductionRule {
 public:
  typedef std::function<void(void *)> SnippetCallback;

  ProductionRule(const Symbol &left, Sequence &&right, SnippetCallback snippet)
      : left_(left), right_(std::move(right)), snippet_(std::move(snippet)) {
  }

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

class Grammar {
 public:
  typedef std::vector<ProductionRule> RuleRecord;
  typedef std::unordered_multimap<Symbol, const Sequence *const> RuleMap;
  typedef std::pair<RuleMap::const_iterator, RuleMap::const_iterator> RuleRange;
  typedef std::unordered_set<Symbol> Table;
  typedef std::function<void(Token &)> TokenFeeder;

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

  const TokenFeeder &token_feeder() const {
    return token_feeder_;
  }

 private:
  RuleRecord rule_record_;
  RuleMap rule_map_;
  Table table_;
  TokenFeeder token_feeder_;
};

class GrammarBuilder {
 public:
  GrammarBuilder &SetSymbolTable(Grammar::Table &&table) {
    grammar_.table_ = table;
    return *this;
  }

  GrammarBuilder &SetTokenFeeder(Grammar::TokenFeeder &&token_feeder) {
    grammar_.token_feeder_ = std::move(token_feeder);
    return *this;
  }

  GrammarBuilder &InsertRule(Symbol left, Sequence &&right) {
    assert(left.IsNonTerminal());
    grammar_.rule_record_.emplace_back(left,
                                       std::move(right),
                                       [](void *p) {});
    return *this;
  }

  template<class F>
  GrammarBuilder &InsertRule(Symbol left, Sequence &&right, F &&f) {
    assert(left.IsNonTerminal());
    grammar_.rule_record_.emplace_back(left, std::move(right), f);
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
