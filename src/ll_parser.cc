//
// Created by coder on 16-9-15.
//

#include "ll_parser.h"

#include <iostream>

using std::unordered_multimap;
using std::unordered_map;
using std::set;

SymbolAuxSet CalcFirst(const Grammar &grammar) {
  SymbolAuxSet firsts;

  for (auto &symbol : grammar.table()) {
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
      curr_set.erase(kEpsilon);

      while (iter != right.end()
          && firsts[*iter].end() != firsts[*iter].find(kEpsilon)) {
        // curr_set <- curr_set & (First(Bk+1) - epsilon)
        auto inserted_set = firsts[*iter++];
        inserted_set.erase(kEpsilon);
        curr_set.insert(inserted_set.begin(), inserted_set.end());
      }

      // if B1B2...Bk could be epsilon, then A could be epsilon
      auto last_set = firsts[right.back()];
      if (iter == right.end() && last_set.end() != last_set.find(kEpsilon)) {
        curr_set.insert(kEpsilon);
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

  follows[kStart].insert(kEof);

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

          if (right_part_first.end() != right_part_first.find(kEpsilon)) {
            // curr_set <- curr_set & (First(Bi) - epsilon)
            auto inserted = right_part_first;
            inserted.erase(kEpsilon);
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

      if (curr_first.end() == curr_first.find(kEpsilon)) {
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
