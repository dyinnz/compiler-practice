//
// Created by coder on 16-9-15.
//

#pragma once

#include <stack>

#include "grammar.h"
#include "tokenizer.h"

namespace expr_grammar {

constexpr int kExprID = kStartID + 1;
constexpr int kExprRecurID = kStartID + 2;
constexpr int kTermID = kStartID + 3;
constexpr int kTermRecurID = kStartID + 4;
constexpr int kFactorID = kStartID + 5;
constexpr int kNumberID = kStartID + 6;
constexpr int kNameID = kStartID + 7;

extern const Symbol kExpr;
extern const Symbol kExprRecur;
extern const Symbol kTerm;
extern const Symbol kTermRecur;
extern const Symbol kFactor;

extern const Symbol kAdd;
extern const Symbol kSub;
extern const Symbol kMul;
extern const Symbol kDiv;
extern const Symbol kLeftParen;
extern const Symbol kRightParen;
extern const Symbol kNumber;
extern const Symbol kName;

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
