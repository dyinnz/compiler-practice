//
// Created by coder on 16-9-15.
//

#pragma once

#include <set>
#include <unordered_map>

#include "grammar.h"

typedef std::unordered_map<Symbol, std::set<Symbol>> SymbolAuxSet;
typedef std::vector<std::set<Symbol>> ExtendFirst;

SymbolAuxSet CalcFirst(const Grammar &grammar);
SymbolAuxSet CalcFollow(const Grammar &grammar, const SymbolAuxSet &firsts);
ExtendFirst CalcExtendFirst(const Grammar &grammar,
                            const SymbolAuxSet &firsts,
                            const SymbolAuxSet &follows);

class LLParser {
 public:

 private:

};
