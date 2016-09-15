//
// Created by coder on 16-9-15.
//

#include "expr_grammar.h"

using std::string;

namespace expr_grammar {

Symbol kExpr{false, kExprID};
Symbol kExprRecur{false, kExprRecurID};
Symbol kTerm{false, kTermID};
Symbol kTermRecur{false, kTermRecurID};
Symbol kFactor{false, kFactorID};

Symbol kAdd{true, '+'};
Symbol kSub{true, '-'};
Symbol kMul{true, '*'};
Symbol kDiv{true, '/'};
Symbol kLeftParen{true, '('};
Symbol kRightParen{true, ')'};
Symbol kNumber{true, kNumberID};
Symbol kName{true, kNameID};

string to_string(const Symbol &symbol) {
  switch (symbol.SymbolID()) {
    case kExprID:return "Expr";

    case kExprRecurID:return "ExprRecur";

    case kTermID:return "Term";

    case kTermRecurID:return "TermRecur";

    case kFactorID:return "Factor";

    case kNumberID:return "Number";

    case kNameID:return "Name";

    case kEpsilonID:return "Epsilon";

    case kEofID:return "EOF";

    case kStartID:return "Start";

    default:return string(1, static_cast<char>(symbol.SymbolID()));
  }
}

Grammar BuildExprGrammar() {
  Grammar grammar;

  grammar.set_table({kExpr, kExprRecur, kTerm, kTermRecur, kFactor,
                     kAdd, kSub, kMul, kDiv, kLeftParen, kRightParen,
                     kNumber, kName,
                     kEpsilon, kEof, kStart});

  grammar.InsertRule(kStart, {kExpr});

  grammar.InsertRule(kExpr, {kTerm, kExprRecur});

  grammar.InsertRule(kExprRecur, {kAdd, kTerm, kExprRecur});
  grammar.InsertRule(kExprRecur, {kSub, kTerm, kExprRecur});
  grammar.InsertRule(kExprRecur, {kEpsilon});

  grammar.InsertRule(kTerm, {kFactor, kTermRecur});

  grammar.InsertRule(kTermRecur, {kMul, kFactor, kTermRecur});
  grammar.InsertRule(kTermRecur, {kDiv, kFactor, kTermRecur});
  grammar.InsertRule(kTermRecur, {kEpsilon});

  grammar.InsertRule(kFactor, {kLeftParen, kExpr, kRightParen});
  grammar.InsertRule(kFactor, {kNumber});
  grammar.InsertRule(kFactor, {kName});

  return grammar;
}

} // end of namespace expr_grammar
