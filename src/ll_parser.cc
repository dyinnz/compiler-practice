//
// Created by coder on 16-9-15.
//

#include "ll_parser.h"
#include "simplelogger.h"
#include "expr_grammar.h"

using std::unordered_multimap;
using std::unordered_map;
using std::set;

extern simple_logger::BaseLogger logger;

SymbolAuxSet CalcFirst(const Grammar &grammar) {
  SymbolAuxSet firsts;

  for (auto &symbol : grammar.SymbolTable()) {
    if (symbol.IsTerminal()) {
      firsts[symbol] = {symbol};
    } else {
      firsts[symbol] = {};
    }
  }

  bool is_change = false;
  do {
    is_change = false;
    for (auto &rule : grammar.GetRuleMap()) {
      // fetch a rule: A->B1B2...Bk
      auto &left = rule.first;
      auto &right = rule.second;

      // init loop: curr_set <- First(B1) - epsilon
      auto iter = right.begin();
      auto curr_set = firsts[*iter];
      curr_set.erase(kEpsilonSymbol);

      while (iter != right.end()
          && firsts[*iter].end() != firsts[*iter].find(kEpsilonSymbol)) {
        // curr_set <- curr_set & (First(Bk+1) - epsilon)
        auto inserted_set = firsts[*iter++];
        inserted_set.erase(kEpsilonSymbol);
        curr_set.insert(inserted_set.begin(), inserted_set.end());
      }

      // if B1B2...Bk could be epsilon, then A could be epsilon
      auto last_set = firsts[right.back()];
      if (iter == right.end() && last_set.end() != last_set.find(kEpsilonSymbol)) {
        curr_set.insert(kEpsilonSymbol);
      }

      size_t old_size = firsts[left].size();
      firsts[left].insert(curr_set.begin(), curr_set.end());

      // check whether changed
      if (!is_change && old_size < firsts[left].size()) {
        is_change = true;
      }
    }

  } while (is_change);

  return firsts;
}

SymbolAuxSet CalcFollow(const Grammar &grammar, const SymbolAuxSet &firsts) {
  SymbolAuxSet follows;

  follows[kStartSymbol].insert(kEofSymbol);

  bool is_change;
  do {
    is_change = false;
    for (auto &rule : grammar.GetRuleMap()) {
      // fetch a rule: A->B1B2...Bk
      auto &left = rule.first;
      auto &right = rule.second;

      // init loop: curr_set <- Follow(A)
      set<Symbol> curr_set = follows[left];

      for (auto iter = right.rbegin(); iter != right.rend(); ++iter) {
        auto &right_part = *iter;
        auto &right_part_first = firsts.find(right_part)->second;

        if (!right_part.IsTerminal()) {
          auto &right_part_follow = follows[right_part];

          // Follow(Bi) <- Follow(Bi) & curr_set
          size_t old_size = right_part_follow.size();
          right_part_follow.insert(curr_set.begin(), curr_set.end());

          // check wether change
          if (!is_change && old_size < right_part_follow.size()) {
            is_change = true;
          }

          if (right_part_first.end() != right_part_first.find(kEpsilonSymbol)) {
            // curr_set <- curr_set & (First(Bi) - epsilon)
            auto inserted = right_part_first;
            inserted.erase(kEpsilonSymbol);
            curr_set.insert(inserted.begin(), inserted.end());
          } else {
            // curr_set <- First(Bi)
            curr_set = right_part_first;
          }
        } else {
          // curr_set <- First(Bi)
          curr_set = right_part_first;
        }
      }
    }

  } while (is_change);

  return follows;
}

ExtendFirst CalcExtendFirst(const Grammar &grammar,
                            const SymbolAuxSet &firsts,
                            const SymbolAuxSet &follows) {
  ExtendFirst extend_first(grammar.GetRuleRecord().size());

  for (size_t i = 0; i < extend_first.size(); ++i) {
    // fetch a rule: A->B1B2...Bk
    auto &left = grammar.GetRuleRecord()[i]->first;
    auto &right = grammar.GetRuleRecord()[i]->second;

    // fill the First+(A) with (First(B1) & First(B2) & ... First(Bi)),
    // where Bi is the first one which epsilon does not belong to First(Bi)
    auto iter = right.begin();
    while (iter != right.end()) {
      auto &curr_first = firsts.find(*iter)->second;
      extend_first[i].insert(curr_first.begin(), curr_first.end());

      if (curr_first.end() == curr_first.find(kEpsilonSymbol)) {
        break;
      }
      ++iter;
    }

    // if B1B2...Bk could be epsilon, then using Follow(A)
    if (iter == right.end()) {
      auto &left_follow = follows.find(left)->second;
      extend_first[i].insert(left_follow.begin(), left_follow.end());
    }
  }

  return extend_first;
}

bool BuildLLTable(const Grammar &grammar,
                  const ExtendFirst &extend_firsts,
                  LLTable &ll_table) {
  for (size_t i = 0; i < extend_firsts.size(); ++i) {
    auto &left = grammar.GetRuleRecord()[i]->first;
    auto &left_entry = ll_table[left];
    for (auto &terminal : extend_firsts[i]) {
      if (left_entry.end() != left_entry.find(terminal)) {
        return false;
      }
      left_entry.insert({terminal, i});
    }
  }
  return true;
}

bool BuildLLTable(const Grammar &grammar, LLTable &ll_table) {
  auto firsts = CalcFirst(grammar);
  auto follows = CalcFollow(grammar, firsts);
  auto extend_firsts = CalcExtendFirst(grammar, firsts, follows);
  return BuildLLTable(grammar, extend_firsts, ll_table);
}

bool LLParser::Parse(std::vector<Token> &tokens) {
  ast_ = std::make_shared<Ast>();
  ast_->set_root(ast_->CreateNode(kStartSymbol));

  tokens.push_back({"EOF-of-LL", kEofSymbol});

  auto curr_token = tokens.begin();
  bool result = true;

  stack_.push(ast_->root());
  while (!stack_.empty()) {
    AstNode *top = stack_.top();
    stack_.pop();

    logger.debug("current symbol {}", expr_grammar::to_string(top->symbol()));

    if (!top->symbol().IsTerminal()) {
      auto ll_entry_iter =
          ll_table_.find(top->symbol());

      if (ll_table_.end() == ll_entry_iter) {
        logger.error("wrong LL(1) Table: no such non-terminal symbol");
        result = false;
        break;
      }

      auto rule_index_iter =
          ll_entry_iter->second.find(curr_token->symbol);

      if (ll_entry_iter->second.end() == rule_index_iter) {
        logger.error("unexpected token");
        result = false;
        break;
      }

      int rule_index = rule_index_iter->second;
      auto &right = grammar_.GetRuleRecord()[rule_index]->second;

      for (auto iter = right.rbegin(); iter != right.rend(); ++iter) {
        stack_.push(ast_->CreateNode(*iter));
      }

    } else {
      if (top->symbol() == kEpsilonSymbol) {
        continue;

      }
      if (top->symbol() == curr_token->symbol) {
        ++curr_token;

      } else {
        logger.error("terminal unmatched");
        result = false;
        break;
      }
    }
  }

  if (result && curr_token->symbol == kEofSymbol) {
    result = true;
  }

  return result;
}
