//
// Created by coder on 16-9-15.
//

#pragma once

#include <stack>

#include "grammar.h"
#include "tokenizer.h"

namespace expr_grammar {

DECLARE_SYMBOL(kExpr, 11)
DECLARE_SYMBOL(kExprRecur, 12)
DECLARE_SYMBOL(kTerm, 21)
DECLARE_SYMBOL(kTermRecur, 22)
DECLARE_SYMBOL(kFactor, 31)
DECLARE_SYMBOL(kNumber, 32)
DECLARE_SYMBOL(kName, 33)

DECLARE_CHAR_SYMBOL(kAdd, '+')
DECLARE_CHAR_SYMBOL(kSub, '-')
DECLARE_CHAR_SYMBOL(kMul, '*')
DECLARE_CHAR_SYMBOL(kDiv, '/')
DECLARE_CHAR_SYMBOL(kLeftParen, '(')
DECLARE_CHAR_SYMBOL(kRightParen, ')')

std::string to_string(const Symbol &symbol);

Tokenizer BuildExprTokenizer();
Grammar BuildExprGrammar();

class ExprGrammarData {
 public:
  ExprGrammarData() : ast_(std::make_shared<Ast>()) {}

  std::vector<AstNode *> &node_record() {
    return node_record_;
  }

  std::shared_ptr<Ast> ast() {
    return ast_;
  }

 private:
  std::shared_ptr<Ast> ast_;
  std::vector<AstNode *> node_record_;
};

std::shared_ptr<ExprGrammarData> CreateGrammarData();

} // end of namespace expr_grammar
