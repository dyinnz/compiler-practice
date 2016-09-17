//
// Created by coder on 16-9-15.
//

#include "expr_grammar.h"
#include "tokenizer.h"

using std::string;

namespace expr_grammar {

const Symbol kExpr{Symbol::kNonTerminal, kExprID};
const Symbol kExprRecur{Symbol::kNonTerminal, kExprRecurID};
const Symbol kTerm{Symbol::kNonTerminal, kTermID};
const Symbol kTermRecur{Symbol::kNonTerminal, kTermRecurID};
const Symbol kFactor{Symbol::kNonTerminal, kFactorID};

const Symbol kAdd{Symbol::kTerminal, '+'};
const Symbol kSub{Symbol::kTerminal, '-'};
const Symbol kMul{Symbol::kTerminal, '*'};
const Symbol kDiv{Symbol::kTerminal, '/'};
const Symbol kLeftParen{Symbol::kTerminal, '('};
const Symbol kRightParen{Symbol::kTerminal, ')'};
const Symbol kNumber{Symbol::kTerminal, kNumberID};
const Symbol kName{Symbol::kTerminal, kNameID};

string to_string(const Symbol &symbol) {
  switch (symbol.ID()) {
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

    default:return string(1, static_cast<char>(symbol.ID()));
  }
}

Tokenizer BuildExprTokenizer() {
  Tokenizer tokenizer;

  tokenizer.BuildTokenizer(
      {{R"(\()", kLeftParen},
       {R"(\))", kRightParen},
       {R"(\+)", kAdd},
       {R"(-)", kSub},
       {R"(/)", kDiv},
       {R"(\*)", kMul},
       {R"(\d+)", kNumber},
       {R"([A-Za-z_]\w*)", kName},
      });

  return tokenizer;
}

Grammar BuildExprGrammar() {

  GrammarBuilder builder;

  builder.SetSymbolTable({kExpr, kExprRecur, kTerm, kTermRecur, kFactor,
                          kAdd, kSub, kMul, kDiv, kLeftParen, kRightParen,
                          kNumber, kName,
                          kEpsilonSymbol, kEofSymbol, kStartSymbol});

  builder.InsertRule(kStartSymbol, {kExpr});

  builder.InsertRule(kExpr, {kTerm, kExprRecur});

  builder.InsertRule(kExprRecur, {kAdd, kTerm, kExprRecur});
  builder.InsertRule(kExprRecur, {kSub, kTerm, kExprRecur});
  builder.InsertRule(kExprRecur, {kEpsilonSymbol});

  builder.InsertRule(kTerm, {kFactor, kTermRecur});

  builder.InsertRule(kTermRecur, {kMul, kFactor, kTermRecur});
  builder.InsertRule(kTermRecur, {kDiv, kFactor, kTermRecur});
  builder.InsertRule(kTermRecur, {kEpsilonSymbol});

  builder.InsertRule(kFactor, {kLeftParen, kExpr, kRightParen});
  builder.InsertRule(kFactor, {kNumber});
  builder.InsertRule(kFactor, {kName});

  return builder.Build();
}

} // end of namespace expr_grammar
