//
// Created by coder on 16-9-15.
//

#pragma once

#include "grammar.h"

namespace expr_grammar {

constexpr int kExprID = kStartID + 1;
constexpr int kExprRecurID = kStartID + 2;
constexpr int kTermID = kStartID + 3;
constexpr int kTermRecurID = kStartID + 4;
constexpr int kFactorID = kStartID + 5;
constexpr int kNumberID = kStartID + 6;
constexpr int kNameID = kStartID + 7;

std::string to_string(const Symbol &symbol);

Grammar BuildExprGrammar();

} // end of namespace expr_grammar
