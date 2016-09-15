//
// Created by coder on 16-9-15.
//

#pragma once

#include <set>
#include <unordered_map>

#include "grammar.h"

typedef std::unordered_map<Symbol, std::set<Symbol>> SymbolAuxSet;
typedef std::vector<std::set<Symbol>> ExtendFirst;
typedef std::unordered_map<Symbol, std::unordered_map<Symbol, int>> LLTable;

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
 public:

 private:
};
