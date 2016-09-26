//
// Created by coder on 16-9-15.
//

#pragma once

#include <set>
#include <stack>
#include <unordered_map>
#include <memory>

#include "grammar.h"
#include "token.h"

typedef std::unordered_map<Symbol, std::set<Symbol>> SymbolAuxSet;
typedef std::vector<std::set<Symbol>> ExtendFirst;
typedef std::unordered_map<Symbol, std::unordered_map<Symbol, size_t>> LLTable;

SymbolAuxSet CalcFirst(const Grammar &grammar);
SymbolAuxSet CalcFollow(const Grammar &grammar, const SymbolAuxSet &firsts);
ExtendFirst CalcExtendFirst(const Grammar &grammar,
                            const SymbolAuxSet &firsts,
                            const SymbolAuxSet &follows);
bool BuildLLTable(const Grammar &grammar,
                  const ExtendFirst &extend_firsts,
                  LLTable &ll_table);

bool BuildLLTable(const Grammar &grammar, LLTable &ll_table);

class LLParser {
 private:
  struct StackState {
    Symbol symbol;
    size_t rule_index;
    bool is_handled;
  };

 public:
  LLParser(const Grammar &grammar, const LLTable &ll_table) :
      grammar_(grammar), ll_table_(ll_table) {}

  bool Parse(void *grammar_data, std::vector<Token> &tokens);

 private:
  bool ProductTerminal(StackState &top_state,
                       std::vector<Token>::iterator &token_iter);
  bool ProductNonTerminal(StackState &top_state,
                            std::vector<Token>::iterator &token_iter);

 private:
  const Grammar &grammar_;
  const LLTable &ll_table_;
  void *grammar_data_;

  std::stack<StackState> production_stack_;
};
